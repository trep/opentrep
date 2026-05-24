"""
OpenTREP travel search - FastAPI backend
"""
import json, math, logging
from contextlib import asynccontextmanager
from typing import Optional
from fastapi import FastAPI, HTTPException, Query
from fastapi.staticfiles import StaticFiles
from fastapi.responses import HTMLResponse, FileResponse
from pydantic import BaseModel, Field

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# Blue-green deployment: 0 = current (SQLite), 1 = next (PostgreSQL)
# Change deploymentNb and restart the service to switch deployments.
deploymentNb = 0

TREP_DIR    = "/var/www/webapps/opentrep/trep"
POR_PATH    = f"{TREP_DIR}/share/opentrep/data/por/optd_por_public_{deploymentNb}.csv"
XAPIAN_DIR  = f"{TREP_DIR}/traveldb"   # opentrep appends deploymentNb automatically
LOG_PATH    = "/var/log/webapps/search/pyopentrep.log"

# SQL backend per deployment slot
_SQL_TYPE = {
    0: ("sqlite", f"{TREP_DIR}/sqlite_travel.db"),
    1: ("pg",     "dbname=trep_trep user=trep password=trep host=localhost"),
}

_trep = None

@asynccontextmanager
async def lifespan(app: FastAPI):
    global _trep
    from pyopentrep.pyopentrep import OpenTrepSearcher
    _trep = OpenTrepSearcher()
    sql_type, sql_conn = _SQL_TYPE[deploymentNb]
    ok = _trep.init(POR_PATH, XAPIAN_DIR, sql_type, sql_conn,
                    deploymentNb, False, True, True, LOG_PATH)
    if not ok:
        raise RuntimeError("OpenTREP init failed")
    logger.info("OpenTREP initialised")
    yield
    _trep.finalize()
    logger.info("OpenTREP finalised")

# ── Response models ───────────────────────────────────────────────────────────

class CityDetails(BaseModel):
    iata_code:  str = Field("", description="IATA code of the serving city")
    geonames_id: str = Field("", description="Geonames identifier of the city")
    name_utf:   str = Field("", description="City name (UTF-8)")
    name_ascii: str = Field("", description="City name (ASCII)")

class Cities(BaseModel):
    city_details: Optional[CityDetails] = None

class UnlocodeCodes(BaseModel):
    unlocode_code: str = Field("", description="UN/LOCODE")

class NameEntry(BaseModel):
    name: str = Field("", description="Alternate name")

class Location(BaseModel):
    iata_code:          str   = Field(..., description="IATA 3-letter code", examples=["NCE"])
    icao_code:          str   = Field("",  description="ICAO 4-letter code (empty if none)", examples=["LFMN"])
    geonames_id:        str   = Field("",  description="Geonames numeric identifier", examples=["2990440"])
    feature_class:      str   = Field("",  description="Geonames feature class (P=populated place, S=spot/facility…)")
    feature_code:       str   = Field("",  description="Geonames feature code (AIRP, PPL, PPLA2…)")
    faa_code:           str   = Field("",  description="FAA code (US only)")
    name_common:        str   = Field("",  description="Common travel name", examples=["Nice"])
    name_ascii:         str   = Field("",  description="ASCII transliteration of the common name")
    country_code:       str   = Field("",  description="ISO 3166-1 alpha-2 country code", examples=["FR"])
    country_name:       str   = Field("",  description="Country name", examples=["France"])
    adm1_code:          str   = Field("",  description="Administrative level-1 code")
    adm1_name_utf:      str   = Field("",  description="Administrative level-1 name")
    adm2_code:          str   = Field("",  description="Administrative level-2 code")
    adm2_name_utf:      str   = Field("",  description="Administrative level-2 name")
    state_code:         str   = Field("",  description="State/region code")
    continent_code:     str   = Field("",  description="Continent code")
    continent_name:     str   = Field("",  description="Continent name")
    time_zone:          str   = Field("",  description="IANA time-zone identifier", examples=["Europe/Paris"])
    offset_gmt:         str   = Field("",  description="UTC offset (standard time), hours", examples=["1"])
    offset_dst:         str   = Field("",  description="UTC offset (daylight saving time), hours", examples=["2"])
    offset_raw:         str   = Field("",  description="Raw UTC offset, hours", examples=["1"])
    lat:                float = Field(0.0, description="Latitude (WGS84 decimal degrees)", examples=[43.7031])
    lon:                float = Field(0.0, description="Longitude (WGS84 decimal degrees)", examples=[7.2661])
    population:         str   = Field("",  description="Population (0 for non-populated features)")
    elevation:          str   = Field("",  description="Elevation above sea level (metres)")
    page_rank:          float = Field(0.0, description="OpenTREP PageRank score (0–100)", examples=[9.52])
    wiki_link:          str   = Field("",  description="Wikipedia article URL", examples=["https://en.wikipedia.org/wiki/Nice"])
    currency_code:      str   = Field("",  description="ISO 4217 currency code", examples=["EUR"])
    tvl_por_list:       str   = Field("",  description="Pipe-separated list of travel-related POR IATA codes served by this entry")
    wac:                str   = Field("",  description="World Area Code")
    wac_name:           str   = Field("",  description="World Area Code name")
    original_keywords:  str   = Field("",  description="Original query keyword(s) matched to this result (may be NA when Xapian is bypassed for direct IATA lookups — known OpenTREP issue)")
    corrected_keywords: str   = Field("",  description="Spell-corrected keyword(s) matched to this result")
    matching_percentage: str  = Field("",  description="Xapian matching percentage (0 for exact IATA matches)")
    edit_distance:      str   = Field("",  description="Edit distance between query and matched term")
    allowable_distance: str   = Field("",  description="Maximum allowable edit distance for this match")
    unlocode_codes:     Optional[UnlocodeCodes] = Field(None, description="UN/LOCODE entry")
    cities:             Optional[Cities]        = Field(None, description="Serving city information")
    names:              Optional[list[NameEntry]] = Field(None, description="All known alternate names (from Geonames)")
    distance_km:        int   = Field(0,   description="Great-circle distance from the previous location in the result list (0 for the first)")

class SearchResponse(BaseModel):
    query:      str            = Field(..., description="Query string as received", examples=["nce cdg jfk"])
    locations:  list[Location] = Field(..., description="Ordered list of matched Points of Reference")
    total_km:   int            = Field(0,   description="Total great-circle distance along the full route (km)", examples=[9808])

# ── App ───────────────────────────────────────────────────────────────────────

app = FastAPI(
    title="OpenTREP Travel Search",
    version="0.7.18",
    description=(
        "REST API for the [OpenTREP](https://github.com/trep/opentrep) open-source "
        "travel search engine.\n\n"
        "OpenTREP uses Xapian full-text search and SQLite (or MySQL/PostgreSQL) to "
        "index and query the [OpenTravelData (OPTD)](https://github.com/opentraveldata/opentraveldata) "
        "Points of Reference (POR) dataset, which covers all IATA-registered airports, "
        "cities, and heliports worldwide.\n\n"
        "**Source**: https://github.com/trep/opentrep  \n"
        "**OPTD data**: https://github.com/opentraveldata/opentraveldata"
    ),
    contact={
        "name":  "OpenTREP project",
        "url":   "https://github.com/trep/opentrep",
        "email": "denis.arnaud_opentrep@m4x.org",
    },
    license_info={
        "name": "MIT",
        "url":  "https://opensource.org/licenses/MIT",
    },
    lifespan=lifespan,
)

def _great_circle_km(lat1, lon1, lat2, lon2):
    r = math.pi / 180.0
    lat1, lon1, lat2, lon2 = lat1*r, lon1*r, lat2*r, lon2*r
    dlat = (lat2 - lat1) / 2.0
    dlon = (lon2 - lon1) / 2.0
    a = math.sin(dlat)**2 + math.cos(lat1)*math.cos(lat2)*math.sin(dlon)**2
    return 12742.0 * math.asin(math.sqrt(a))

def _enrich(locations: list) -> list:
    for loc in locations:
        # Coerce empty strings to None for optional nested models
        for field in ("unlocode_codes", "cities", "names"):
            if loc.get(field) == "":
                loc[field] = None
        loc["lat"] = float(loc.get("lat", 0))
        loc["lon"] = float(loc.get("lon", 0))
        loc["page_rank"] = float(loc.get("page_rank", 0))
    for i, loc in enumerate(locations):
        loc["distance_km"] = 0 if i == 0 else round(
            _great_circle_km(locations[i-1]["lat"], locations[i-1]["lon"],
                             loc["lat"], loc["lon"]))
    return locations

@app.get("/api/search",
         summary="Search Points of Reference",
         description=(
             "Search the OPTD POR dataset using OpenTREP.\n\n"
             "The query can be free-text (city name, airport name) or a sequence of "
             "IATA codes. When multiple locations are resolved, the response includes "
             "the great-circle distance between consecutive pairs and the total route "
             "distance."
         ),
         response_model=SearchResponse)
def search(q: str = Query(..., min_length=1, max_length=200,
                          description="Search query: free-text or IATA code(s)",
                          examples=["nce", "cdg jfk lax", "Tokyo London"])):
    raw = _trep.search("J", q)
    data = json.loads(raw)
    raw_locs = data.get("locations", [])
    if not raw_locs or raw_locs == "":
        raise HTTPException(status_code=404, detail="No location found for query: " + repr(q))
    locations = _enrich(raw_locs if isinstance(raw_locs, list) else [])
    total_km = sum(loc["distance_km"] for loc in locations)
    return {"query": q, "locations": locations, "total_km": total_km}

@app.get("/api/random",
         summary="Random airports / itinerary",
         description=(
             "Draw *n* random Points of Reference from the OPTD dataset and return "
             "the same structure as `/api/search`. Useful for demos and testing.\n\n"
             "- `n=1` → single random airport  \n"
             "- `n=3` → random three-leg itinerary"
         ),
         response_model=SearchResponse)
def random_airports(n: int = Query(1, ge=1, le=10,
                                   description="Number of random locations to draw")):
    raw = _trep.generate("S", n)
    codes = [part.split("/")[0] for part in raw.split(",") if "/" in part]
    return search(" ".join(codes))

app.mount("/static", StaticFiles(directory="/var/www/webapps/search/static"), name="static")

@app.get("/favicon.ico", include_in_schema=False)
def favicon():
    return FileResponse("/var/www/webapps/search/static/icons/favicon.ico")

INDEX_HTML = r"""<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Travel Search</title>
  <link rel="icon" href="/favicon.ico">
  <!-- Google tag (gtag.js) -->
  <script async src="https://www.googletagmanager.com/gtag/js?id=G-XJE7QT0JR3"></script>
  <script>
    window.dataLayer = window.dataLayer || [];
    function gtag(){dataLayer.push(arguments);}
    gtag('js', new Date());
    gtag('config', 'G-XJE7QT0JR3');
  </script>
  <!-- Leaflet -->
  <link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css">
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body { font-family: Arial, sans-serif; background: #f5f5f5; color: #333;
           display: flex; flex-direction: column; height: 100vh; }

    /* ── header ── */
    header { background: #fff; border-bottom: 1px solid #ddd;
             padding: 10px 20px; display: flex; align-items: center; gap: 14px;
             flex-shrink: 0; }
    header img { height: 52px; }
    .search-area { display: flex; flex-direction: column; gap: 6px; flex: 1; }
    .search-bar  { display: flex; gap: 6px; flex-wrap: wrap; }
    .search-bar input { flex: 1; min-width: 200px; padding: 7px 11px;
                        font-size: 1rem; border: 1px solid #ccc; border-radius: 4px; }
    .btn { padding: 7px 14px; font-size: 0.88rem; cursor: pointer; border: none;
           border-radius: 4px; background: #4285f4; color: #fff; white-space: nowrap; }
    .btn:hover { background: #3367d6; }
    .btn.sec { background: #5f6368; }
    .btn.sec:hover { background: #3c4043; }

    /* ── summary strip ── */
    #summary { flex-shrink: 0; background: #fff; border-bottom: 1px solid #ddd;
               padding: 6px 20px; font-size: 0.88rem; min-height: 30px;
               display: flex; align-items: center; gap: 6px; flex-wrap: wrap; }
    #summary .chip { background: #e8f0fe; color: #1a73e8; border-radius: 12px;
                     padding: 2px 9px; font-weight: bold; }
    #summary .arrow { color: #aaa; }
    #summary .km  { color: #555; }
    #summary .total { font-weight: bold; margin-left: 8px; }
    #status { color: #888; font-style: italic; }
    #err    { color: #c00; }

    /* ── map fills the rest ── */
    #map { flex: 1; }

    /* ── Leaflet popup ── */
    .pop-title { font-size: 1.05rem; font-weight: bold; margin-bottom: 4px; }
    .pop-iata  { font-size: 1.4rem; font-weight: bold; color: #1a73e8; margin-right: 6px; }
    .pop-row   { margin: 2px 0; font-size: 0.82rem; }
    .pop-row a { color: #1a73e8; }
  </style>
</head>
<body>

<header>
  <img src="/static/icons/airports.gif" alt="Travel Search">
  <div class="search-area">
    <div class="search-bar">
      <input id="q" type="text" placeholder="e.g. Nice, CDG, Tokyo London" autofocus>
      <button class="btn"     onclick="doSearch()">Search</button>
      <button class="btn sec" onclick="doRandom(1)">Random Airport</button>
      <button class="btn sec" onclick="doRandom(3)">Random Itinerary</button>
    </div>
    <div id="summary"><span id="status">Enter a query or click a button.</span></div>
  </div>
</header>

<div id="map"></div>

<!-- Leaflet + geodesic lines -->
<script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
<script src="https://cdn.jsdelivr.net/npm/leaflet.geodesic@2.7.1/dist/leaflet.geodesic.umd.min.js"></script>
<script>
// ── Map init ────────────────────────────────────────────────────────────────
var _layers = [];   // must be declared before L.map() fires any events

const map = L.map("map").setView([20, 0], 2);
L.tileLayer("https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png", {
  maxZoom: 19,
  attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
}).addTo(map);

function clearLayers() {
  _layers.forEach(l => map.removeLayer(l));
  _layers = [];
}

// ── Search ──────────────────────────────────────────────────────────────────
document.getElementById("q").addEventListener("keydown", e => {
  if (e.key === "Enter") doSearch();
});

async function doSearch() {
  const q = document.getElementById("q").value.trim();
  if (!q) return;
  setStatus("Searching…");
  try {
    const r = await fetch("/api/search?q=" + encodeURIComponent(q));
    if (!r.ok) throw new Error((await r.json()).detail || r.statusText);
    render(await r.json());
  } catch(e) { setErr(e.message); }
}

async function doRandom(n) {
  setStatus("Generating…");
  try {
    const r = await fetch("/api/random?n=" + n);
    if (!r.ok) throw new Error((await r.json()).detail || r.statusText);
    const data = await r.json();
    document.getElementById("q").value = data.locations.map(l => l.iata_code).join(" ");
    render(data);
  } catch(e) { setErr(e.message); }
}

function setStatus(msg) {
  const s = document.getElementById("summary");
  s.innerHTML = '<span id="status">' + msg + '</span>';
}
function setErr(msg) { setStatus('<span id="err">Error: ' + msg + '</span>'); }

// ── Render ───────────────────────────────────────────────────────────────────
function cityName(loc) {
  try { return loc.cities.city_details.name_utf; } catch(e) { return ""; }
}

function popupHtml(loc) {
  const city = cityName(loc);

  // Location line: city, adm2 (if not Z), adm1 (if not Z), country — mirrors CT186
  const skip = v => !v || v === "Z" || v === "NA";
  let locParts = [city];
  if (!skip(loc.adm2_name_utf)) locParts.push(loc.adm2_name_utf);
  if (!skip(loc.adm1_name_utf)) locParts.push(loc.adm1_name_utf);
  locParts.push(loc.country_name || "");

  // ICAO: suppress placeholder "ZZZZ"
  const icao = (loc.icao_code && loc.icao_code !== "ZZZZ") ? loc.icao_code : "";

  // Geonames link
  const geoLink = (loc.geonames_id && loc.geonames_id !== "0")
    ? `<a href="https://www.geonames.org/${loc.geonames_id}" target="_blank">${loc.geonames_id}</a>` : "";

  // Wikipedia link(s)
  const wikiLink = loc.wiki_link
    ? `<a href="${loc.wiki_link}" target="_blank">${loc.wiki_link}</a>.` : "";

  // Original / corrected keywords
  // Workaround: original_keywords is "NA" when Xapian is bypassed for pure
  // IATA-code queries (OpenTREP bug); fall back to corrected_keywords.
  const corrKw = (loc.corrected_keywords && loc.corrected_keywords !== "NA") ? loc.corrected_keywords : "";
  const rawOrig = (loc.original_keywords  && loc.original_keywords  !== "NA") ? loc.original_keywords  : "";
  const origKw  = rawOrig || corrKw;

  // Matching percentage
  const matchPct = parseFloat(loc.matching_percentage || 0).toFixed(1);

  // GMT offset with sign
  const gmt = loc.offset_gmt, dst = loc.offset_dst, raw = loc.offset_raw;

  return `
  <div class="pop-title"><span class="pop-iata">${loc.iata_code}</span> &mdash; ${loc.name_common}</div>
  <div class="pop-row">Travel-related name: <b>${loc.name_common}</b></div>
  <div class="pop-row">Location (city, state, country): <b>${locParts.join(", ")}</b></div>
  <div class="pop-row">PageRank: <b>${loc.page_rank.toFixed(8)}%</b></div>
  <div class="pop-row">IATA code: <b>${loc.iata_code}</b></div>
  <div class="pop-row">ICAO code: ${icao ? "<b>" + icao + "</b>" : ""}</div>
  <div class="pop-row">Geonames ID: ${geoLink}</div>
  ${wikiLink ? '<div class="pop-row">Wikipedia: ' + wikiLink + '</div>' : ""}
  <div class="pop-row">Time zone and offset (GMT, DST, raw): <b>${loc.time_zone}</b> (${gmt}, ${dst}, ${raw})</div>
  <div class="pop-row">Original keywords: ${origKw}</div>
  ${corrKw && corrKw !== origKw ? '<div class="pop-row">Corrected keywords: <b>' + corrKw + '</b></div>' : ""}
  <div class="pop-row">Matching: <b>${matchPct}%</b></div>`;
}

function render(data) {
  clearLayers();
  const locs = data.locations;

  if (!locs || locs.length === 0) {
    setStatus("No results found.");
    return;
  }

  // Summary strip
  let parts = [];
  locs.forEach((l, i) => {
    if (i > 0 && l.distance_km > 0)
      parts.push(`<span class="km arrow">→</span> <span class="km">${l.distance_km.toLocaleString()} km</span>`);
    parts.push(`<span class="chip">${l.iata_code}</span> ${l.name_common}`);
  });
  if (data.total_km > 0)
    parts.push(`<span class="total">| Total: ${data.total_km.toLocaleString()} km</span>`);
  document.getElementById("summary").innerHTML = parts.join(" ");

  // Markers — each added directly to map so bindPopup works
  const latLngs = [];
  locs.forEach(loc => {
    const ll = [loc.lat, loc.lon];
    latLngs.push(ll);
    const marker = L.marker(ll)
      .bindPopup(popupHtml(loc), { maxWidth: 360 })
      .addTo(map);
    _layers.push(marker);
  });

  // Geodesic line (great-circle arc)
  if (latLngs.length >= 2) {
    const line = L.geodesic([latLngs], {
      weight: 2.5, opacity: 0.9, color: "#e53935", steps: 100
    }).addTo(map);
    _layers.push(line);
  }

  // Fit bounds
  if (locs.length === 1) {
    map.setView([locs[0].lat, locs[0].lon], 8);
  } else {
    const group = L.featureGroup(_layers.filter(l => l instanceof L.Marker));
    map.fitBounds(group.getBounds().pad(0.15));
  }
}
</script>
</body>
</html>
"""

@app.get("/", response_class=HTMLResponse)
def index():
    return INDEX_HTML
