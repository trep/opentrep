/** \mainpage Structured Associative Containers

Ternary Search Tree containers to replace \c set<string> and \c map<string, Value> </h2>

<center><table bgcolor="#fbf9e5" style="border: thin dotted #808000;" width="95%" border=0>
<tr>
<td>
<h3>Table of contents</h3>
<dl>
	<dt>\ref introduction "Introduction"</dt>
	<dt>\ref subkey_search_overview "Advanced searches overview"</dt>
	<dt>\ref tst_usage "Tutorial"</dt>
	<dt>\ref tst_reference "Reference"</dt> <dd>
		<dd>\ref structured_concept "Structured Container concept" \n
		Class \ref containers::structured_set "structured_set" \n
		Class \ref containers::structured_map "structured_map" \n
		Class \ref containers::structured_multiset "structured_multiset" \n
		Class \ref containers::structured_multimap "structured_multimap" \n
		Implementation class \ref containers::ternary_tree "ternary_tree"
	</dd></dt>
	<dt>\ref perf_notes "Performance notes"</dt>
	<dt>\ref tst_impl "Implementation details"</dt>
	<dt>\ref tst_links "Links"</dt>
	<dt>\ref tst_tests "Test Suite"</dt>
</dl>
</td>
</tr></table></center>

Download: Latest version (0.684) http://abc.se/~re/code/tst/ternary_tree.zip\n

Copyleft: <a href="mailto:rasmus%20point%20ekman%20at%20abc%20point%20se?subject=Structured Containers suck/rule">
rasmus ekman</a> 2007-2009 \n
Weblink: http://abc.se/~re/code/tst 

\anchor introduction <hr>
<h2>Introduction</h2>
<b>Structured containers</b> are \c map and \c set -like containers specialized for strings. 
They are commonly used for dictionaries.\n
Structured containers have two major benefits: 
- They offer near-match searches (wildcard search, partial match etc) that are hard to implement 
  with other containers.
- Lookup performance is on a par with hashed containers for many common applications, 
and 2-5 times faster than standard maps and sets (with string-like keys).

Of course there is a price to pay: structured containers use much more memory than
other containers: Around 6-8 bytes <b>per letter</b> inserted (whether \c char or \c wchar_t);
an English 150 k word dictionary uses eg 7.3 MB to store 1.2 MB words (2.4 MB of \c wchar_t words).

The container classes in this library can be used as drop-in replacements for \c set and \c map 
(or \c unordered_set, \c unordered_map):
 - \ref containers::structured_set "structured_set": This stores unique keys and allows structured key searches.
 - \ref containers::structured_multiset "structured_multiset": This stores non-unique keys.
 - \ref containers::structured_map "structured_map": This is a 
   <a target="sgi" href="http://www.sgi.com/tech/stl/PairAssociativeContainer.html">Pair Associative Container</a>,
   as it allows associating a value with each key.
 - \ref containers::structured_multimap "structured_multimap": Technically, a 
   <a target="sgi" href="http://www.sgi.com/tech/stl/MultipleSortedAssociativeContainer.html">Multiple, Sorted,
   Pair Associative Container</a> - it allows storing several values with each key.

While the STL standard associative containers are normally backed by a binary tree structure, 
Structured Containers are backed by a Ternary Search Tree, as presented by  
\ref note_1 "Jon Bentley and Robert Sedgewick in [1]".

Class \ref containers::ternary_tree "ternary_tree<Key, Value, Comp, Alloc>" provides the implementation backend.
Due to its internals, its interface cannot easily be made to conform with standard STL concepts,
so it is used internally by the structured* wrapper classes (much like STL's internal \c rb_tree class). 

Basically, if you have code using sets or maps, you have code to use structured containers. 
And with 1-3 lines of code, you're ready to make advanced imprecise searches in your dictionaries.\n
See \ref tst_usage "the usage section" for examples of how to use these classes.

<table bgcolor="#f0f0ff" style="border: thin dotted #808000;" border=0>
<tr><th>Library status</th></tr>
<tr><td valign="top" align="right">Compatibility:</td>
<td>Note that the file \b tst_concept_checks.cpp is currently broken. Will investigate.\n
<!-- This used to compile with Mingw GCC 3.4.2 and with MSVC7.1 (with STLport 5). Requires Boost 1.33. 
Not sure what happened in Boost 1.36-37 or if I've mangled something. \n
Due to recent changes, ternary tree does not support stateful allocators (earlier versions did this by implication) -->
</td>
<tr><td valign="top" align="right">version&nbsp;0.684: (Jan 2009)</td>
<td>Fix standard-breakage in multimap/multiset return from <code>insert(const value_type&amp;)</code>.<br>
Added <code>operator-&gt;</code> to iterator wrapper for C++0x compatibility.
Thanks to Geoffrey Noel for reports.</td>
</tr>
<tr><td valign="top" align="right">version&nbsp;0.683: (March 2007)</td>
<td>Fix portability issues for GCC and non-STLport libraries. Fix longest_match.<br>
Thanks to Arjen Wagenaar for several reports, fixes and encouragement. Thanks also to Michel Tourn for reports.</td>
</tr>
<tr><td valign="top" align="right">version&nbsp;0.68: (Dec 2006)</td>
<td>Implement TST_NODE_COUNT_TYPE macro, which can be used to control node size on 64-bit systems.
  See \ref containers::ternary_tree "class ternary_tree"</td>
</tr>
<tr><td valign="top" align="right">version&nbsp;0.68&nbsp;(alpha):</td>
<td>Reimplemented node type. Do proper management of value type (was inconsistent, partly unimplemented - duh!)</td>
</tr>
<!--tr>
<tr><td valign="top" align="right">version 0.676:</td>
<td>Modified containers to follow C++0x draft standard: \n
Added \c cbegin, \c cend methods returning \c const_iterator, and \c crbegin, \c crend 
returning \c const_reverse_iterator, to make it easier to code with const-correctness. \n
\c erase(iterator pos); and \c erase(iterator first, iterator last); methods now return iterators.</td>
<tr><td valign="top" align="right">version&nbsp;0.675:</td>
<td>All Structured Container classes implemented. Structured search interface TBD.
</td-->
</table>


\anchor subkey_search_overview <hr>
<h2>Sub-key, or Structure Searches</h2>
<span style="color:#905050;">(a new interface for these searches will be specified in the future)</span>

Ternary trees allow searches that match parts of keys and ignores mismatches in other parts.\n
In the current interface we specify a small number of searches facilitated by the tree structure; 
the Partial Match and Hamming searches are defined in several other implementations 
(showcased in \ref note_1 "Bentley and Sedgewick" code).
The Levenshtein and combinatorial searches are not found in other ternary trees (that I know of).

<table border="1" cellspacing="0">
  <tr><th bgcolor="#f0f0ff">Name (function name)</th><th bgcolor="#f0f0ff">Description</th></tr>
  <tr><th>
	Prefix match (\ref containers::ternary_tree::prefix_range "prefix_range")</th><td>
	Finds keys sharing a common prefix, returns a pair of iterators.</td></tr>
  <tr><th>
	Longest match (\ref containers::ternary_tree::longest_match "longest_match")</th><td> 
	Finds the longest key that matches beginning of search string.
	A typical application is to tokenize a string using the ternary tree as dictionary.</td></tr>
  <tr><th>
	Partial match, or wildcard search (\ref containers::ternary_tree::partial_match_search "partial_match_search")</th><td>
	Accepts a search string with wildcard characters that will match any letter, 
	eg "b?nd" would match "band", "bend", "bind", "bond" in an English dictionary.</td></tr>
  <tr><th>
	Search allowing \c N mismatches, 
	(\ref containers::ternary_tree::hamming_search "hamming_search"<span style="font-weight:normal;"></span>)</th><td>
	Accepts a search string and an integer \c dist indicating how many non-matching letters are allowed, 
	then finds keys matching search string that have at most \a dist mismatches.
	This works like a partial match search with all combinations of \a dist 
	wildcards in the search string.\n
	\c hamming_search("band", 1) matches the wildcard search plus "bald", "bane" and "wand", etc. \n
	The version here, following DDJ code, extends the strict Hamming search by also allowing shorter and longer 
	strings; a search for "band", \a dist = 1, also finds "ban" and "bandy" etc.\n
	See also http://wikipedia.org/wiki/Hamming_distance</td></tr>
  <tr><th>
	Levenshtein distance search</b> (\ref containers::ternary_tree::levenshtein_search "levenshtein_search"
	<span style="font-weight:normal;">- consider descriptive name</span>)</th><td>

	Hamming search matches characters in fixed position, allowing substitution of \a dist chars.
	Levenshtein search also allows shifting parts of the search string by insertion or skipping chars (in \a dist places).
	So&nbsp;<code>levenshtein_search("band", 1) </code> extends the hamming_search set with "and" and "bland", etc. 
	A typical application is to match mispelt words.\n
	See also http://wikipedia.org/wiki/Levenshtein_distance</td></tr>
  <tr><th>
	Combinatorial or "scrabble" search (\ref containers::ternary_tree::combinatorial_search "combinatorial_search")</th><td> 
	Finds all keys using the characters in search string. \c combinatorial_search("band") finds 
	"ad", "and", "bad", "dab", "nab", etc. A count of wildcards can be added, also allowing 
	nonmatching characters (use with care, values over 10% of average key length  
	may cause the algorithm to traverse a large part of the tree).</td></tr>
</table>

See \ref usage_imprecise_searches "advanced search overview" in the tutorial.

These searches are defined for all containers in this library. 
But they are also marked as deprecated (to be replaced by generic algorithms with same interface).
For a relative performance comparison of imprecise searches, see the second table in \ref perf_notes.

<h3>Future directions</h3>
The searches currently defined are clearly special cases in a sea of search possibilities.
We have only defined searches that are relatively efficient, compared to other combinations of containers and algorithms. 
But there can be many variations on the available searches: increasing Hamming/Levenshtein distance 
at the end of words, or matching limited ranges of characters (eg allowing mismatches only in vowels), etc.

The next step for this project is to support a more flexible low-level interface for
traversing and filtering tree nodes.
The interface for these "structured searches" is open for consideration, but it
will basically define sub-key iterators, conversion of full-key from sub-key iterators,
and a small collection of algorithms operating on these sub-key iterators.

At least the following operations are needed:

 - sub-key match: matching a part of a key (prefix, or starting from current char position)
 - key element range increment: from a sub-key position, match a range of characters 
   in next position (returns a list of sub-key iterators? - or iterator-like operation?)
 - conversion from sub-key iterator to full-key iterator range (nearest and post-furthest 
   keys in the subtree)
 - \c is_key(subkey_iterator pos): true if end-of-key exists at iterator position.
 - \c count_elements(subkey_iterator pos): returns number of available key elements at position.
 - In all predefined algorithms above, either a specific, or any char is matched, 
   we would also support arbitrary char sets (possibly with special case for char ranges).

 */

/** \page tst_reference Reference
<center><table bgcolor="#fbf9e5" style="border: thin dotted #808000;" width="95%" border=0>
<tr>
<td>
<dl>
	<dt>\ref structured_concept "Structured Container concept"</dt>
	<dt>\ref ref_sethpp "Header < structured_set.hpp >"</dt>
	<dt>\ref ref_maphpp "Header < structured_map.hpp >"</dt>
	<dt>\ref ref_tsthpp "Header < ternary_tree.hpp >"</dt>
	<dt>\ref ref_iterhpp "Header < iterator_wrapper.hpp >"</dt>
</dl>
</td>
</tr></table></center>

<hr>

\anchor ref_sethpp
<h2>Header < <a href="../structured_set.hpp">%structured_set.hpp</a> > synopsis</h2>
<pre>
\b namespace containers {
  \b template <\b class Key,
            \b class Comp = std::less<\b typename Key::value_type>,
            \b class Alloc = std::allocator<Key> >
  \b class \ref containers::structured_set "structured_set";

  \b template <\b class Key,
            \b class Comp = std::less<\b typename Key::value_type>,
            \b class Alloc = std::allocator<Key> >
  \b class \ref containers::structured_multiset "structured_multiset";
}
</pre>

\anchor ref_maphpp
<h2>Header < <a href="../structured_map.hpp">%structured_map.hpp</a> > synopsis</h2>
<pre>
\b namespace containers {
  \b template <\b class Key,
            \b class T,
            \b class Comp = std::less<\b typename Key::value_type>,
            \b class Alloc = std::allocator<std::pair<\b const Key, T> > >
  \b class \ref containers::structured_map  "structured_map";

  \b template <\b class Key,
            \b class T,
            \b class Comp = std::less<\b typename Key::value_type>,
            \b class Alloc = std::allocator<std::pair<\b const Key, T> > >
  \b class \ref containers::structured_multimap "structured_multimap";
} 
</pre>

<hr>
Supplementary header files needed to support structured_set and -map classes.


\anchor ref_tsthpp
<h2>Header < <a href="../ternary_tree.hpp">%ternary_tree.hpp</a> > synopsis</h2>
<pre>
\b namespace containers {

  \b template <\b class Key,
            \b class T,
            \b class Comp = std::less<\b typename Key::value_type>,
            \b class Alloc = std::allocator<std::pair<\b const Key, T> > >
  \b class \ref containers::ternary_tree  "ternary_tree";

  \b template <\b class TreeT, \b class IteratorT> 
      \b class \ref containers::search_results_list "search_results_list";

} 
</pre>


\anchor ref_iterhpp
<h2>Header < <a href="../iterator_wrapper.hpp">%iterator_wrapper.hpp</a> > synopsis</h2>
<pre>
\b namespace iterators {

  \b template <\b class T> \b struct const_traits;
  \b template <\b class T> \b struct nonconst_traits;

  \b template <\b class BaseIterT,
            \b class TraitsT,     // either const_traits<T> or nonconst_traits<T>
            \b class IterCatT = std::bidirectional_iterator_tag >
  \b class \ref iterators::iterator_wrapper  "iterator_wrapper";
} 
</pre>

*/

/**
\page structured_concept Structured Associative Container Concept

<span style="color:#905050;">(a preliminary sketch of the formal technical concept description)</span>

A Structured Associative Container is a specialization of the C++ 98 standard concept 
<a target="sgi" href="http://www.sgi.com/tech/stl/SortedAssociativeContainer.html">Sorted Associative Container</a>,
with extended interface.

The template parameters are similar to that of the Associated Containers:

<code> structured_set<Key, Comp, Alloc>; </code>\n
<code> structured_map<Key, Value, Comp, Alloc>; </code>\n

where: 
 - \c <b>Key</b> type is itself a container (eg a \c std::string or \c std::wstring)
 - \c <b>Comp</b> is a comparison operator that imposes a sort order on \c Key::value_type elements \n
   (so if \c Key is string, \c Comp compares \c char, if \c Key is \c wstring, \c Comp applies to \c wchar_t).
 - \c <b>Value</b> can be any Assignable type
 - \c <b>Alloc</b> is an allocator that manages all memory allocation for the container.

The \c Comp and the \c Alloc types have default template arguments.

In other words Structured containers are like Sorted Associative Containers, BUT
 - add the requirement on Key template type to be a 
   <a target="sgi" href="http://www.sgi.com/tech/stl/ForwardContainer.html">Forward Container</a>.\n
   For example, \c std::basic_string<CharT> is compatible with this requirement.
 - change the requirement on the \c Comp (comparator) template argument to operate on 
   \c key_type::value_type elements (rather than on \c key_type itself). 
   Like Sorted Associative comparator, the \c Comp type shall define a less-like comparison, a 
   <a target="sgi" href="http://www.sgi.com/tech/stl/StrictWeakOrdering.html">Strict Weak Ordering</a>
   of key-elements.

<b>Associated types</b>
 - \b char_compare: less-like comparison of key elements (establishing a Strict Weak Ordering).
   The <a target="sgi" href="http://www.sgi.com/tech/stl/AssociativeContainer.html">Associative Container</a> 
   \c key_compare type is also provided, but is defined in terms of \c char_compare. \n
 - \b subkey_iterator: Used in structure searches. Convertible to iterator (TBD).

In consequence it allows searches involving subparts of keys, ie with shared prefix and/or 
with shared middle parts. 

<hr>
<h3>Deprecated search interface</h3>

In the first iteration, additional searches are provided as methods on the containers.
This will be changed to use free functions operating on \c subkey_iterator. 
The deprecated search methods will still be provided as convenience functions;
to migrate your code from present version to the new interface, will mean moving
the object name to the first argument, but also to respecify the search_results_list type. 
(This sloppy-hackish type is by itself reason not to keep the method interface)

See \ref subkey_search_overview "Structured search overview"
and \ref tst_structsearch "ternary_tree Structure search section".
*/

/*

\b Notation \n
<table border=0>
<tr><td>\c X <td>A type that is a model of Associative Container </td></tr>
<tr><td>\c a <td>Object of type \c X </tr>
<tr><td>\c k <td>Object of type \c X::key_type </tr>
<tr><td>\c p, \c q <td>Object of type \c X::char_iterator </tr>
<tr><td>\c c  <td>Object of type \c X::char_type </tr>
<tr><td>\c o  <td>Object modelling output iterator </tr>
<tr><td>\c i  <td>Object of type \c X::size_type </tr>
</dl>

<table border=1>
<tr><th>Name</th><th>Expression</th><th>Return value</th>
<tr><td>Prefix match</td><td><code>a.prefix_range(k)</code></td><td>
	\c std::pair<iterator, iterator> if \c a is mutable, otherwise <br>\c std::pair<const_iterator, const_iterator></td></tr>
<tr><td>Longest match</td><td><code>a.longest_match(p, q)</code></td><td>
	\c iterator if \c a is mutable, otherwise \c const_iterator</td></tr>
<tr><td>Partial match, or <br>wildcard search</td><td><code>a.partial_match_search(k, o, c)</code></td><td>
	The output iterator \c o</td></tr>
<tr><td>Hamming search</td><td><code>a.hamming_search(k, o, i)</code></td><td>
	The output iterator \c o</td></tr>
<tr><td>Levenshtein search</td><td><code>a.levenshtein_search(k, o, i)</code></td><td>
	The output iterator \c o</td></tr>
<tr><td>Combinatorial or <br>"scrabble" search</td><td><code>a.combinatorial_search(k, o, i)</code></td><td>
	The output iterator \c o</td></tr>
</table>

*/

/** \page tst_impl Implementation Details
 * (In the following, "original" and "DDJ" code refers to the article by Bentley/Sedgewick
 * published in Dr Dobb's Journal, and the accompanying C source code - see \ref tst_links)
 *
 * In most implementations, a ternary tree node has the following members: \code
 * struct node {
 *    char splitchar;  // key letter, or 0 at end of key
 *    node *hikid;     // subtree of keys with higher value than splitchar
 *    node *eqkid;     // subtree matching splitchar (pointer to mapped value at end-of-key node)
 *    node *lokid;     // subtree less than splitchar
 *    node *parent;    // necessary for iteration (not needed for insert/find)
 * }; \endcode
 *
 * This means that each node is 1 char plus three or four pointers size.
 * On many systems, struct member alignment makes the char member consume size of one pointer 
 * as well, so we have 4 (or 5) x sizeof(pointer) per node in the tree. 
 * With several kinds of dictionaries, the node count ends up at around 0.3-0.5 times  
 * total key length (since keys share nodes). 
 * This is even more expensive on 64-bit machines.
 *
 * There are several variation points in the node class:
 *  -# the DDJ C code designates an invalid value of zero to indicate end-of-string. We want to 
 *   allow any string as key, so the end-of-string representation should change. 
 *   We note that on many platforms, C/C++ struct member alignment leaves a "hole" 
 *   in the binary representation of the node, between the char and the first pointer ("hikid"). 
 *   On such systems there is no space cost to use another char-sized value to indicate end node. 
 *   This also works for \c wchar_t strings on 32- or 64-bit systems.
 *  -# The original code stores a value for each string in the terminal node's "equal" pointer.
 *   The value in DDJ code is always a pointer to the terminated string. This is used to make
 *   advanced searches work (they return an array of pointers to strings stored in end-nodes).
 *   In reality this means that strings may need to be copied on insertion (not reflected in DDJ timings).
 *  -# Original DDJ code does not support iterating over strings in the tree.
 *   Idiomatic STL-like container style strongly suggests that iteration should be supported. 
 *   This is fairly simple to implement if a parent pointer is added to the node struct: 
 *   Because when an end-node is reached, the iterator must backtrack to find the previous 
 *   branch point. 
 * 
 * The parent pointer also makes it possible to recover the inserted string by walking nodes 
 * backward from a terminal node to the root. Complexity is key length, plus log(tree.size),
 * but it means inserted keys do not \b have to be copied to the end node.
 * We opt to cache keys in iterators, at no measurable extra cost in iteration.

 * Instead of the key, an arbitrary value can be associated with endnodes. 
 * However, it should not be allowed to increase node size, since most nodes in the tree are not endnodes.
 * In this library we store the mapped value directly in end-node if it is <tt> <= \c sizeof(void*). </tt>
 * Larger objects are allocated on the heap, and a pointer to the copy is stored in end-node
 * (the copy is managed by the tree).
 *
 * <h4>Now for some optimization</h4>
 * We use a \c vector<node> as pool allocator, and record eq-hi-lo links as vector index instead of pointers.
 * The pool allocation essentially follows original C code insert2() principle. 
 * For us, it also simplifies reallocation, since pointers do not have to be rebound; 
 * the indices are always valid. 
 * This has the following consequences:
 *  - allow the option of 4-byte indices also on 64-bit systems (with obvious resulting tree size limit)
 *  - When a new key is inserted, the last part (unique to the key) is always allocated in a batch.
 *    This means that one node member, \c "eqkid", becomes redundant, as it is always the next index
 *    (except after terminal nodes of course). 
 *  - in DDJ code the end-node value is stored in union with the eqkid. We note that the \c lokid node index 
 *    is also unused by end-nodes (as no char should be lower than zero), so all endnode children 
 *    are linked to the hi node. 
 *
 * (In our binary-cognizant version where zero is a regular char value, this still holds, 
 * we just change the end-node test accordingly)
 *
 * In the final cut, our node struct data members appear roughly like this: \code
 * struct node {
 *    CharType splitchar;  // key letter, or 0 at end of key (to make sure lokid is never allowed)
 *    CharType endflag;    // zero on normal nodes, 1 at end nodes, 2 at erased nodes.
 *    node_index hikid;    // subtree of keys with higher value than splitchar
 *    node_index lokid;    // subtree less than splitchar
 *    node_index parent;   // necessary for iteration (not needed for insert/find)
 * }; \endcode
 *
 * where \c CharType is defined by template \c Key::value_type, and treated as an unsigned type 
 * (so 0 is the lowest value); and \c node_index is a \c size_t -like type used by the node 
 * storage backend (currently \c std::vector).
 *
 * This optimization could also be applied to C version, trimming space requirement in DDJ code
 * to 3-word nodes.
 */

/** \page perf_notes Performance Notes
 *
 * <h3>Space considerations</h3>
 *
 * Ternary trees are notably larger than hash maps or most binary tree designs.
 * Each node holds only one character (instead of a whole key), and use 3-5 pointers.
 * Our nodes consist of 4 \c size_t values (16 bytes) regardless of platform pointer size, 
 * or char type (if at most 2-byte like \c wchar_t).
 *
 * The shared parts of strings save space: In a typical English dictionary,
 * each key shares over half its nodes with other keys, so the allocated space is about half
 * of total key-length times 16. In a scrabble dictionary like the one reported below,
 * which contains all valid word endings, most nodes are shared, so its storage cost is "only"
 * total key length times 0.35 times 16, or less than 6 bytes per char. 
 * With \c wchar_t type, the storage cost cannot be considered overly large.
 *
 * See also \ref tst_impl
 *
 * <h3>Lookup speed</h3>
 *
 * The complexity of ternary tree operations is basically the same as for binary trees,
 * (logarithmic in tree size) but with quite different constant factors. See&nbsp;\ref note_1 "[1]".
 *
 * Overall lookup and iteration speed depends on application factors - ie 
 * whether strings are inserted in random order or not, etc.
 *
 * Rough speed estimates (compared to Stlport hash_map and map).
 *  - insertion is a bit slower (>30% to 0%) than hash_map, ~30% faster than map.
 *  - finding a key is ~0-50% slower than hash_map (equal on failure, with short keys).
 *  - finding a key is 1.5-3 times faster than map (again with short keys).
 *
 * Compared to C versions (DDJ and libtst),
 *  - find and insert are slower, by factors ranging from 1.5-4.
 *  - partial_match and neighbour searches are 5-20% faster than published DDJ code -
 *    the code is essentially the same, but our implementation rolls out some recursion. 
 *    This is easily back-ported, so in effect they should be considered to run at same speed.
 *    This by itself is good news though, since eg single-key lookup is always slower.
 *
 * Since each character in a key is at a separate node in the internal tree,
 * iterating over values is a little slower than for other tree-based containers.
 *
 * For detailed test, see performance table below.
 *
 * <hr style="height: 3px; border-top: 0px; background-color: #e09060;">
 * \htmlinclude performancetable.html
 */


/** \page tst_links Links
 * ternary_tree by rasmus ekman, see http://abc.se/~re/code/tst <br>
 * Download: http://abc.se/~re/code/tst/ternary_tree.zip 
 *
 * Some other TST implementations.
 *  - <b>DDJ code:</b> Original C implementation by Jon Bentley and Robert Sedgewick. 
 *    Article in Dr Dobb's Journal, 1998 #4: http://www.ddj.com/documents/s=921/ddj9804a/9804a.htm \anchor note_1 \n
 *    See http://www.cs.princeton.edu/~rs/strings/ for C code and article on TST complexity.
 *  - \b libtst: Worked-out version of DDJ code by Peter A. Friend 2002. Version 1.3. \n
 *    See http://www.octavian.org/cs/software.html \anchor note_2 \n
 *  - \b Boost.Spirit version: C++ reimplementation by Joel de Guzman. \anchor note_3 \n
 *    See http://spirit.sourceforge.net/ internal file ./boost/spirit/symbols/impl/tst.h
 *  - <b>Hartmut Kaiser version:</b> C++ reimplementation intended for generalization of tst.
 *    Currently abandoned, available in Spirit CVS. (interesting for interface design) \n
 *    See http://lists.boost.org/Archives/boost/2005/09/93316.php \n
 *    and http://article.gmane.org/gmane.comp.parsers.spirit.general/6959
 *  - \b pytst: C++ version by Nicolas Lehuen, with SWIG wrappers for use from other languages. Version 0.97. \n
 *    See http://nicolas.lehuen.com/download/pytst/
 *    (not yet tested) \anchor note_4 
 *
 * <h2>Feature chart</h2>
 * All versions have insert and plain search, other features available as tabled below:
 * \htmlinclude featuretable.html
 */

/** \page tst_tests Test Suite

All tests require the <a href="http://boost.org">Boost library</a> to compile. 

<h3>Concept checks</h3>

The file <a href="../tst_concept_checks.cpp">tst_concept_checks.cpp</a> 
performs a compile-time test of structured containers. \n
A class \c StructuredAssociativeContainer is defined, which contains
prototypes of all required methods for structured containers (also class ternary_tree).
Relevant concepts from \c boost/concept_check.hpp are used to check the structured set/map
containers.

<h3>Correctness tests</h3>

The subdirectory \b test in the distribution contains a bunch of files hacked up during development. 
All these tests are performed by a single main test file <a href="../test/test_tst.cpp">test_tst.cpp</a>.
This file includes individual .cpp files, since we use a simplified (hacked) version
of the Boost.Test harness.

Each test prints a single line to \c std::cerr saying whether the test was "OK" or "FAIL". 
A line is added if an exception was thrown. 

These are runtime tests, several which require a file name to a dictionary-type file,
a plain-text file with one word per line. 
The file \c fill_dictionary.cpp must be compiled with test projects, 
it reads a dictionary file and fills a std::vector with strings.

Dictionary files can be found by an internet search (try eg "dictionary file").

<h3>To do</h3>

Proper organization and cleanup of this part of our library will be required before 1.0 release.

*/
 

