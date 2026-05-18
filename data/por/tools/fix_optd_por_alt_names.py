#!/usr/bin/env python3
"""
fix_optd_por_alt_names.py — Preprocess an OPTD POR CSV file for OpenTREP indexing.

This script fixes two classes of issues in the alt_name_section field (field 44)
of OPTD POR CSV files (e.g. optd_por_public.csv) that cause OpenTREP 0.07.x to
crash during indexing:

Issue 1 — Unknown OPTD flags ('h' and 'c')
  OpenTREP 0.07.x only recognises the 'p' (preferred) and 's' (short) alt_name
  flags.  The full set of valid OPTD flags is:
    p  preferred
    s  short
    h  historical
    c  colloquial
  An unknown flag triggers a PorFileParsingException and aborts indexing.

  Fix: strip 'h' and 'c' flags in-place.
    |h=  →  |=   (flag before the '=' entry separator)
    |c=  →  |=
    |h^  →  |^   (flag before the '^' CSV field separator / end of alt_name section)
    |c^  →  |^

  These substitutions are applied to the raw line (not field-by-field) because
  the patterns are unambiguous.  The '=' entry separator is never directly
  preceded by a single lowercase letter other than a flag.

Issue 2 — '=' character inside alt_name text
  The alt_name_section uses '=' as the entry separator.  A small number of
  place names contain a literal '=' (e.g. the Japanese katakana name of
  Mbuji-Mayi: ムブジ=マイ).  The parser splits on every '=', producing
  a malformed segment that does not start with a valid language code.

  Fix: replace '=' that is flanked on both sides by non-ASCII characters
  with a space.  This targets only the genuine in-name '=' and never the
  ASCII entry separators.

Issue 3 — Alt_name entries exceeding Xapian's 245-byte term limit
  Xapian raises Xapian::InvalidArgumentError when a term exceeds 245 UTF-8
  bytes.  OpenTREP indexes each alt_name string as a Xapian term.  A handful
  of entries (most notably Bangkok's full ceremonial Thai name, ~420 bytes)
  exceed this limit.

  Fix: drop any individual alt_name entry whose *name* component (the part
  between the first and second '|') is longer than XAPIAN_SAFE_BYTES bytes
  when encoded as UTF-8.  XAPIAN_SAFE_BYTES is set conservatively to 200 to
  leave headroom for compound terms that OpenTREP generates (e.g.
  "<name> Democratic Republic of the Congo").

  Note: the long names are dropped *silently* — they are not useful for
  interactive travel search (no user would type Bangkok's 168-character
  ceremonial name).  The proper long-term fix is in OpenTREP's C++ indexer,
  which should silently skip terms that exceed Xapian's limit.

Usage
-----
  python3 fix_optd_por_alt_names.py <src_csv> <dst_csv>

  src_csv  Path to the original OPTD POR CSV file (e.g. optd_por_public.csv).
  dst_csv  Path where the fixed CSV file will be written.

The script logs a summary of changes to stderr.

Compatibility
-------------
Tested with:
  - optd_por_public.csv (IATA-only, ~20 921 records, Apr 2026)
  - OpenTREP 0.07.18 on CentOS Stream 10 / Xapian 1.4.23

References
----------
  - OPTD alt_name_section format:
    https://github.com/opentraveldata/opentraveldata/blob/master/README.md
  - Geonames alternate-name language codes:
    https://www.geonames.org/export/codes.html
    Pseudo-codes accepted by OPTD: post, icao, iata, faac, tcid, abbr, link,
    phon, piny, wkdt, unlc, nuts, lauc
  - Xapian term-length limit: 245 bytes (Glass backend default)
    https://xapian.org/docs/apidoc/html/classXapian_1_1WritableDatabase.html
"""

import re
import sys

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------

# Maximum UTF-8 byte length for an alt_name *name* part before the entry is
# dropped.  Set below Xapian's hard 245-byte limit to leave room for compound
# term suffixes added by OpenTREP (e.g. " Democratic Republic of the Congo",
# " aerodrome", etc.).
XAPIAN_SAFE_BYTES = 200

# ---------------------------------------------------------------------------
# Regex helpers (compiled once)
# ---------------------------------------------------------------------------

# Issue 1: h/c flags before '=' entry separator
_RE_FLAG_BEFORE_SEP = re.compile(r'\|[hc]=')
# Issue 1: h/c flags before '^' CSV field separator (end of alt_name section)
_RE_FLAG_BEFORE_CARET = re.compile(r'\|([hc])\^')
# Issue 2: '=' flanked by non-ASCII characters (e.g. ムブジ=マイ)
_RE_EQUALS_IN_NAME = re.compile(r'(?<=[^\x00-\x7f])=(?=[^\x00-\x7f])')


# ---------------------------------------------------------------------------
# Per-line preprocessing (Issues 1 & 2 — applied to the raw line)
# ---------------------------------------------------------------------------

def fix_flags_and_equals(line: str) -> str:
    """Strip h/c flags and replace non-ASCII-flanked '=' with a space."""
    # Issue 1a: |h= → |=  and  |c= → |=
    line = _RE_FLAG_BEFORE_SEP.sub('|=', line)
    # Issue 1b: |h^ → |^  and  |c^ → |^
    line = _RE_FLAG_BEFORE_CARET.sub('|^', line)
    # Issue 2: ムブジ=マイ → ムブジ マイ
    line = _RE_EQUALS_IN_NAME.sub(' ', line)
    return line


# ---------------------------------------------------------------------------
# Per-field post-processing (Issue 3 — applied to field 44 only)
# ---------------------------------------------------------------------------

def drop_long_names(alt_names_field: str) -> tuple[str, int]:
    """
    Remove alt_name entries whose name component exceeds XAPIAN_SAFE_BYTES.

    Returns the cleaned field and the number of entries dropped.
    """
    entries = alt_names_field.split('=')
    kept = []
    dropped = 0
    for entry in entries:
        parts = entry.split('|')
        # alt_name entry format: lang|name|flag  OR  lang|name  OR  |name|flag
        # The name is always parts[1] (index 1).
        if len(parts) >= 2:
            name = parts[1]
            if len(name.encode('utf-8')) > XAPIAN_SAFE_BYTES:
                dropped += 1
                continue
        kept.append(entry)
    return '='.join(kept), dropped


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main() -> None:
    if len(sys.argv) != 3:
        print(f'Usage: {sys.argv[0]} <src_csv> <dst_csv>', file=sys.stderr)
        sys.exit(1)

    src_path, dst_path = sys.argv[1], sys.argv[2]

    lines_changed_flags = 0
    lines_changed_equals = 0
    total_long_names_dropped = 0
    lines_processed = 0

    with open(src_path, 'r', encoding='utf-8') as fin, \
         open(dst_path, 'w', encoding='utf-8') as fout:

        for lineno, raw_line in enumerate(fin):
            if lineno == 0:           # header — pass through unchanged
                fout.write(raw_line)
                continue

            lines_processed += 1

            # --- Issues 1 & 2: flag stripping and in-name '=' fix ---
            fixed_line = fix_flags_and_equals(raw_line)
            if fixed_line != raw_line:
                if _RE_FLAG_BEFORE_SEP.search(raw_line) or \
                   _RE_FLAG_BEFORE_CARET.search(raw_line):
                    lines_changed_flags += 1
                if _RE_EQUALS_IN_NAME.search(raw_line):
                    lines_changed_equals += 1

            # --- Issue 3: drop alt_name entries that are too long ---
            fields = fixed_line.rstrip('\n').split('^')
            if len(fields) > 43:
                fields[43], dropped = drop_long_names(fields[43])
                total_long_names_dropped += dropped
                fixed_line = '^'.join(fields) + '\n'

            fout.write(fixed_line)

    print(
        f'Processed {lines_processed:,} records.\n'
        f'  Lines changed (h/c flags):       {lines_changed_flags:,}\n'
        f'  Lines changed (= in name):        {lines_changed_equals:,}\n'
        f'  Long alt_name entries dropped:    {total_long_names_dropped:,}',
        file=sys.stderr,
    )


if __name__ == '__main__':
    main()
