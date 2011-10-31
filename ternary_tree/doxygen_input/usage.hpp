/** \page tst_usage Examples

<center><table bgcolor="#fbf9e5" style="border: thin dotted rgb(128, 128, 0);" width="95%" border="0">
<tr><td>
<h3>Table of contents</h3>
<dl>
<dt>\ref usage_basic</dt>
<dt>\ref usage_searches</dt> <dd>
	\ref usage_longest_match \n
	\ref usage_search_results \n
	\ref usage_imprecise_searches \n
	\ref usage_scrabble_search
	</dd></dt>
  <dt>\ref usage_custom_comp</dt>
  <dt>\ref usage_localize</dt>
  <dt>\ref usage_ternary_tree</dt>
</dl>
</td></tr>
</table></center>

<hr><!-- ######################################### -->

\section usage_basic Basic use
The best and simplest way to use the Ternary Search Tree-backed containers
is to select the appropriate structured_[set, map, multiset or multimap] container,
and use it just like its STL counterpart. 

\b Note: in these usage notes we will not explain the behaviour of standard 
set, map or unordered_* containers. Please look at documentation and examples
for your standard library associative containers, and please double-check the
structured_* container reference when in doubt.

The class methods are exactly the same as for (multi)set and (multi)map, and show the same 
behaviour (except usually quite a bit faster). 
The only difference is the template definition, and this only becomes apparent
if you want to use a custom comparator type. Then of course there are the advanced
search methods. That's the good bit, we'll get to them shortly.

\code
//
// Basic use of structured_set
//
#include <iostream>
#include <string>
#include "structured_set.hpp"

typedef containers::structured_set<std::string> Set;
typedef Set::iterator SetIter;
typedef std::pair<SetIter, SetIter> IterPair;

// Insert some keys
names.insert("apps");
names.insert("applets");
names.insert("banana");

std::cout << "The set contains\n    ";
for (SetIter it = names.begin(); it != names.end(); ++it)
	std::cout << *it << ", ";
\endcode
This should print \code
The set contains
	applets, apps, banana, \endcode

A simple search:
\code
IterPair p = names.prefix_range("app");
std::cout << "prefix_range(\"app\") returns:\n  ";
while (p.first != p.second) {
	std::cout << *p.first++ << ", ";
}

\endcode
prints
\code
prefix_range("app") returns:
	applets, apps,
\endcode
and \c p.second points to "banana".

In contrast, the standard set/map operation \c equal_range would return
a pair of iterators pointing to the same item:
\code
std::cout << "equal_range(\"app\") returns ";
p = names.equal_range("app");
if (p.first == p.second)
	std::cout << "empty range\n";
std::cout << "p.second points to " << *p.second;
\endcode

This prints
\code
equal_range("app") returns empty range
p.second points to applets
\endcode
(So equal_range points to the place where "app" would be inserted.)

In short, this section shows that if you have used \c std::set, 
you already have code to use \c structured_set.


<hr><!-- ######################################### -->
\section usage_searches Advanced searches
Now we get to the good stuff.

Currently structured containers provide two useful single-key search methods
returning iterator(s), and four different searches that return a list of matches.
We begin with the "single-key" methods.

<h3>prefix_range</h3>
We have already seen \ref containers::ternary_tree::prefix_range "prefix_range", which returns 
a range of keys that share the search string as prefix. 
If you use a structured container to store symbols found by a parser,
a typical application of \c prefix_range might be to find all names of class members,
or all names in a name scope. Names might be stored like many C++ parsers
(eg like \c "::scope1::scopeN::classname<neverending_list_of_types>::membername"), 
or dot-separated like javascript or many other languages: \c "class.innerclass.member".
\code
typedef containers::structured_set<std::string> SymbolSet;
SymbolSet symbols;
bool is_defined_in_scope(std::string scope, std::string name)
{
	typedef std::pair<SymbolSet::iterator, SymbolSet::iterator> Range;
	Range r = symbols.prefix_range(scope);
	SymbolSet::iterator n = symbols.find(name);
	return n != symbols.end() && *n >= *r.first && *n < *r.second;
}
\endcode

Here the glory is soiled by not having random access iterators. We cannot
compare the iterators (in constant time) to find their order, instead we
have to dereference and compare the keys. (and the reason we cannot just compare
the scope and the name strings is not apparent from the above code sketch)

<hr><!-- ######################################### -->

\subsection usage_longest_match longest_match - tokenizer

\ref containers::ternary_tree::longest_match "longest_match" takes a different tack.
Its arguments are two iterators that point to a range of \c char_type (ie, some text).
It then finds the longest key that can be matched to the beginning of the char range.
If a key is found, it advances the first char iterator so it is positioned after the
found key. This way it's easy to use a structured container as a simple tokenizer,
and for large vocabularies, comparatively efficient.

Here is a tokenizer over a large dictionary which is nearly as fast as a single  
ternary_tree.find() operation. \n
<b>Note that we do NOT use a \c std::istream_iterator for the input from file.</b>
The iterators passed to \c longest_match must support distance(), that is, 
they must be Forward Iterators, an Input Iterator is not enough.

\code

void longest_match_example(const char* dictfile, const char* parsefile)
{
	typedef containers::structured_map<std::string, int, nocase_less<char> > Vocabulary;
	Vocabulary english;
	// Fill dictionary
	fill_wordlist(dictfile, english);    // (not shown here)
	if (english.empty())
		return;

	// longest_match does not work with istream_iterator, so must fill buffer
	std::ifstream infile(parsefile);
	size_t filesize = get_filesize(infile);  // (not shown here)
	boost::scoped_array<char> bytes(new char[filesize]);
	infile.read(bytes.get(), filesize);

    char *first = bytes.get();
	char *last = first + infile.gcount();
	while (first != last) 
	{
		Vocabulary::iterator word = english.longest_match(first, last);
		if (word != english.end())
			std::cout << (*word).first << " ";
		else {
			// No key; try next char
			++first;
		}
	}
}
\endcode

Note that this may return spurious results if a word is not in the dictionary.
Tokenizing Shakespeare quotes over the English Scrabble dictionary may return
\code "there" "is" "something" "rotten" "in" "den" "mark" \endcode
since "Denmark" is a proper name (and so not allowed in the Scrabble game).


<hr><!-- ######################################### -->
\subsection usage_imprecise_searches Imprecise searches overview

We have three degrees of imprecision to choose from in this ternary tree
implementation:
- Search with wildcards: letters in specific position of the search string may mismatch (\c partial_match_search).
- A limited number of letters in any position may mismatch. The count of mismatches is called
  the Hamming distance, so finding all strings within a certain distance is called a \c hamming_search.
- Some letters may mismatch, and some may also be inserted between chars in 
  the search string, or letters of the search string may be skipped. The differences can be viewed 
  as "edit operations," and the count of edits are technically known as the Levenshtein distance.
  Hence, the \c levenshtein_search (which could use a more descriptive name).

The functions have similar interface: Arguments are the search-string, an output iterator 
onto which the results are assigned, and a search configuration parameter. 
For the partial match, the configuration is the wildcard character used in the search string 
(by default '?'), for Hamming and Levenshtein searches it is the number of allowed 
mismatches/edits.
Here are the function call prototypes:
- <code>OutputIter \ref containers::ternary_tree::partial_match_search "partial_match_search"
  (const key_type& , OutputIter , char_type wildcard = '?');</code>
- <code>OutputIter \ref containers::ternary_tree::hamming_search "hamming_search"
  (const key_type& , OutputIter , size_type dist);</code>
- <code>OutputIter \ref containers::ternary_tree::levenshtein_search "levenshtein_search"
  (const key_type& , OutputIter , size_type dist);</code>

<hr><!-- ######################################### -->

\subsection usage_search_results search_results_list - pseudo vector<StructuredContainer::iterator>

The imprecise searches and the combinatorial or "scrabble" search all
feed the search results to a forward iterator that accept structured container iterators.
The \c search_results_list is a wrapper class that is almost a
\c std::vector<\a StructuredContainer:: \c iterator> but
to save space we store a single reference to the tree and a list of positions.
The \c search_results_list returns proper iterators from its methods.

Each \c search_results_list (abbr: SRL) is thus bound to a specific container instance. 
Therefore we chose to construct it by a factory method on structured containers:
\code
typedef containers::structured_map<std::string, MyStuff> StuffMap;
StuffMap mystuff;
// Create the search results list
StuffMap::search_results_list results = mystuff.create_search_results();
\endcode

To use it with the search methods, wrap the results in a \c std::back_iterator, eg
\code mystuff.levenshtein_search(key, std::back_iterator(results), 3); \endcode

The SRL class is defined with a template parameter for the source container iterator, 
so it is bound to the right iterator type.

The results list class is also limited - it doesn't do all a vector could do.
We define
 - the \c tree_iterator type, which is the typename of the owner container's iterator type
 - push_back() so we can pass a \c std::back_insert_iterator to search methods
 - an \c iterator type, which return tree_iterators when dereferenced
 - \c operator[] which also returns a tree_iterator
 - \c size(), \c empty(), \c clear() with the expected behaviour.

The \c search_results_list::iterator can be treated as a pointer-to-pointer:
\code
typedef StuffMap::search_results_list::iterator ResultIter;
// ...
for (ResultIter it = results.begin(); it != results.end(); ++it)
	std::cout << (**it).second << "\n";
\endcode
equally you may use the subscript operator:
\code
// ...
for (int i = 0; i < results.size(); ++i)
	std::cout << (*results[i]).first << " = " << (*results[i]).second << "\n";
\endcode 

and that's all there is to it. We'll put it to use in the searches coming up.

<hr><!-- ######################################### -->

\subsection usage_partial_match Wildcard lookup: partial_match_search

The wildcard search is easy to use for fixed patterns with some hooks - dates or ID's
of any kind. The partial_match_search function accepts a search string that designates
a special character as wildcard. This is by default '?', but could be anything - 
original DDJ authors use the dot '.' as wildcard.

To find all dates, if they somehow end up as a string in a structured container, 
the search is simple:
\code
search_results_list dates = mystrings.create_search_results();
mystrings.partial_match_search("??/??/??", std::back_iterator(results));
\endcode

The search will be more expensive the more wildcards are put in the beginning of the
search key. For each wildcard, every node at that level will be inspected, ie up to the
entire alphabet. 

For fixed patterns regexes would normally be the most useful tool, but if they are
stored as strings.

<hr><!-- ######################################### -->

\subsection usage_levenshtein_search Spell-checker lookup: levenshtein_search

The last type of imprecise search is more complicated: it allows for keys that differ from the 
search string by insertions, or by skipping char(s) in the search string, or by plain mismatch.
Therefore the same key may be matched in different ways. The implementation of levenshtein search 
has to try each available path, up to the allowed mismatch count.

This search can therefore be considered expensive, since several alternative searches are performed
in parallel over each node traversed. At the same time it is very powerful, and \c dist values 
of at most 2-3 is normally sufficient. (Put in another way: \c dist of 20-30% of search string length 
is usually plenty.) 

<b>Main advise: limit your searches</b>

One lesson learnt from \ref perf_notes "performance tests" is that it is cheaper to perform 
several levenshtein searches with lower distance than to overshoot the distance by one. 
So, if you for instance just want to find any match, the way to go is to perform 
all searches from \c dist=1 up until you get enough results to be happy. 

Note that a levenshtein search with \c dist = 5 on a 5-letter key will return 
\a all 1-, 2-, 3-, 4- and 5-letter words in the tree, plus all 6-letter keys containing at least 
one char from the search key, plus all 7-letter words containing at least two letters 
in the same order as in search key, etc etc. \n
See the count of found keys for different searches in the second table in \ref perf_notes.

So, normally the \c dist count should be kept low. It may often be optimal to perform several 
searches, increasing the \c dist count only if no matches are found.



<hr><!-- ######################################### -->
\subsection usage_scrabble_search Combinatorial (or "scrabble") lookup: all words using these chars


<hr><!-- ######################################### -->
\section usage_custom_comp Custom comparators

The behaviour of structured containers can be modified by a custom comparator.
Unlike the standard set, map containers, the comparator works on single characters.
Here we define a case-insensitive structured_multiset. 
The comparator is simple:
\code
//
// Case-insensitive structured containers
//
#include <functional>

template<class CharT>
struct nocase_less : public std::binary_function<CharT, CharT, bool>
{
	bool operator()(const CharT& a, const CharT& b) const { return tolower(a) < tolower(b); }
};

typedef containers::structured_multiset<std::string, nocase_less<char> > CaselessSet;
\endcode

It will store all capitalisation variants of a key under the same key, but returns 
the exact inserted strings.
\code
CaselessSet caseless;
caseless.insert("NoCase");
caseless.insert("nocase");
caseless.insert("noCase");
caseless.insert("NOCASE");

std::cout << "nocase count: " << caseless.count("nocase");

for(CaselessSet::iterator it = caseless.begin(); it != caseless.end(); ++it) {
	std::cout << ", " << *it;
}
\endcode
This prints \code nocase count: 4, NoCase, nocase, noCase, NOCASE \endcode -
so the names are stored in insertion order, not in sort order.
If we used a unique structured_set, only the first would have been stored, "NoCase".

We are not doing anything here that couldn't be done with std::multiset, 
though the comparator would look different. But we are doing it 3-4 times faster!


<hr><!-- ######################################### -->
\section usage_localize Localization comparator
Now imagine that you want your non-English dictionary properly sorted.
The Swedish alphabet ends with the letters
- A-ring: \b &Aring;, \b &aring;
- A-umlaut: \b &Auml;, \b &auml;
- O-umlaut: \b &Ouml;, \b &ouml;

The Latin-1 character table (ISO-8859-1) graciously includes even the weird A-ring letter
(an A with a small ring on top, perhaps not even representable in your browser).
So we're lucky. But, it doesn't store them in the local sort order. 
Nor does it use the Danish and Norwegian sort order: Æ (AE), &Oslash;, &Aring;.

So we need another custom comparator here. 
It is again a \c std::binary_function<CharT, CharT, bool> type -
and you can probably figure out how to do it by using \c std::locale's facet \c collate::compare
for char-by-char comparison. (We'll do better in a minute though.):
\code
#include <locale>
//! Expensive single-character comparison by std::locale<> facet collate
template<class CharT>
struct collate_comp : public std::binary_function<CharT, CharT, bool>
{
	collate_comp(const std::string& name) : m_locale(name.c_str()) {}

	bool operator()(const CharT& a, const CharT& b) const {
		const CharT * a0 = &a, *b0 = &b;
		return std::use_facet<std::collate<CharT> >(m_locale).compare(a0, a0+1, b0, b0+1) < 0;
	}

private:
	std::locale m_locale;
};
\endcode

Provided we have this, just supply the type at the Compare template parameter 
to your structured container. This comparator needs to know which locale to use,
so this must be passed to the container constructor:
\code
typedef containers::structured_set<std::string, collate_comp<char> > LocalSet;

// use comparator constructor, create Swedish locale
LocalSet se_names(collate_comp<char>("sv_SE"));

// Use a default comparator for contrast
typedef containers::structured_set<std::string> DefaultSet;
DefaultSet anynames;
\endcode

Try it with some Swedish words:
\code
se_names.insert("Äska");
se_names.insert("Åmål");
se_names.insert("Ödla");
se_names.insert("Adam");

anynames.insert("Äska");
anynames.insert("Åmål");
anynames.insert("Ödla");
anynames.insert("Adam");

 
for(LocalSet::iterator sit = se_names.begin(); sit != se_names.end(); ++sit) {
	std::cout << *sit << ", ";
}
std::cout << "not:\n"
for(DefaultSet::iterator dit = anynames.begin(); dit != anynames.end(); ++dit) {
	std::cout << *dit << ", ";
}
\endcode
prints \code Adam, Åmål, Äska, Ödla, not:
 Äska, Ödla, Åmål, Adam, \endcode

Well, actually it doesn't for me, it prints something horrible to my console,
since I didn't set the locale for the DOS console window.
But the horrors are correctly sorted anyway.

Unfortunately the \c std::collate comparison comes at a cost: In the STL
implementation I use (STLport) there is a virtual function call 
that ends up in a system or library function for every character comparison 
(on Windows we go to CompareString API).
This is a major hit on performance; now \c find is about 50-100 times slower 
than using the default \c less<char> comparator!
A std::map with a similar locale-based comparator is also slowed down, but "only" 
about 8-10 times. So now std::map is 2-4 times faster than structured containers.

To get around this, we can create an alphabet-size sort-order table using locale, 
then do character comparison through table lookup. 
The full \c wchar_t table requires 2^16 X 2 = 128 kB storage.
Your system may already provide similar technology under the hood. 
The file <a href="../examples/locale_less.hpp">examples/locale_less.hpp</a>
has a class \c utility::locale_less<CharT> which does this.

A quick test of the perfomance of this table-lookup solution indicates it is
about 2 times slower than ternary_tree with default comparator.
This is a modest price to pay for internationalization.

The better type declaration looks almost the same as above:
\code
#include "examples/locale_less.hpp"

typedef containers::structured_set< std::string, utility::locale_less<char> > LocalSet;

// use comparator constructor, create Swedish locale
LocalSet se_names(utility::locale_less<char>("sv_SE"));
	
//... etc
\endcode

By the way, if you want the locale-table optimization for a \c std::map<string, Key>, 
why not use the \ref containers::structured_set::key_compare "structured_set::key_compare" 
type from LocalSet above!
It's a simple \c lexicographical_compare using the \c utility::locale_less class.
It's faster than raw \c std::collate::compare calls anyway.
Just a thought.

<hr><!-- ######################################### -->
\section usage_ternary_tree The ternary_tree backend class

The \ref containers::ternary_tree "ternary_tree<Key, Value, Comp, Alloc>" class interface
is sufficiently similar to the more standard-like containers that it should be easy to use, 
but some special quirks must be noted.

 -# iterators are non-standard
 -# two macros are used to configure tree

First off, although \c ternary_tree is Associative and stores both a Key and Value, 
it is NOT Pair Associative (like \c std::set and \c map etc). \n
Therefore, for a \c ternary_tree<std::string, int>
 - \c value_type is the \c Value template parameter, ie \b int
 - \c reference is \c int&, \c const_reference is <tt>int const&</tt>
 - Default allocator is \c std::allocator<Value>, ie \c std::allocator<int> 
 - Default comparator is \c less<char> (like all structured containers)

Consequently, iterator \c value_type is \c int, and iterator dereference returns (const) \c int& \n
More about this next.

<hr><!-- ######################################### -->
\subsection usage_tst_iterator ternary_tree::iterator type

Due to the restrictions on iterators for Pair Associative containers
(they must be \c pair<Key, Value>), and the internals of ternary_tree, 
the iterators are non-standard. Various designs for proxy class were attempted,
but this only makes the code deceptive, it doesn't supply expected behaviour.

Therefore the ternary tree iterator dereference does NOT return a pair,
it returns the \c value_type as defined in the template.

It is still necessary to get the key for an iterator. 
For this purpose a member method \c key() is defined on the iterator. 
To complement this, the method value() returns a reference to the value. 
Here is brief reference of the type defined as iterator for ternary tree: \code
key_type const&  key() const;
const_reference  value() const;
node_index tree_position() const;  // used by wrapper classes

reference       operator*();       // available with nonconst_traits only
const_reference operator*() const;
// ... bidirectional iterator like operator++, operator-- etc
\endcode

Please be warned that the key reference returned is NOT PERSISTENT:
the key is cached in iterator, so when the iterator is incremented or decremented,
the key is changed internally. So don't try to keep the reference, 
copy it at each access. \code
typedef containers::ternary_tree<std::string, int> MyTst;
MyTst tst;  
// ...fill tree
MyTst::iterator it = tst.find("global");
MyTst::key_type& temp = it.key();  // WRONG!
std::string temp2 = it.key();      // Better
it++;                 // temp is changed now
\endcode


The typedef to get this interface uses two wrapper layers as detailed below.

<hr><!-- ######################################### -->
\subsection macro_tst_no_standalone_iterator_facade Macro TST_NO_STANDALONE_ITERATOR_FACADE

\b Where: ternary_tree.hpp, tst_detail/tst_iterator_facade.hpp

\b Define: When you write a container using ternary_tree for implementation. \n
<b>Don't define:</b> when you use ternary_tree directly in application code.

ternary_tree uses an internal class \ref containers::tst_detail::tst_iterator_base "\c tst_iterator_base"
with methods \c dereference(), \c increment(), \c decrement() etc for basic iterator support. 
There are also a few methods useful for wrapper classes like structured_set &co.

To provide standard iterator interface we use the \ref iterators::iterator_wrapper 
"iterator_wrapper" adapter. 
As noted above, the iterator base class provides some methods useful in client code
(esp \c key()). The typedef so far is just: \code 
typedef iterator_wrapper< tst_iterator_base, const_traits<value_type> >    const_iterator;
typedef iterator_wrapper< tst_iterator_base, nonconst_traits<value_type> > iterator;
\endcode (namespaces omitted to simplify)

In client code, the base iterator methods has to be accessed like 
<tt>myTstIter.\b base_iter().key()</tt> etc, which quickly gets tedious.

Therefore we also provide another wrapper layer: \ref containers::tst_detail::iter_method_forward
"\c tst_detail::iter_method_forward". This forwards the base iterator methods, 
so client code can use directly without inserting \c base_iter() everywhere.
Yes, it's a mess and backwards, but we'll change to use Boost.Iterator in the future.

But structured container wrappers don't need this forwarding, thus the macro
to avoid the \c iter_method_forward complications.


<hr><!-- ######################################### -->
\subsection macro_tst_use_fastmap Macro TST_USE_FASTMAP (deprecated)

\b Where: ternary_tree.hpp, tst_detail/ternary_tree_impl.ipp

\b Define: If you want to experiment with fastmap. \n
\b Usage: Define before including the library header file (ternary_tree.hpp, structured_set.hpp etc)

Note that the macro is undefined at end of ternary_tree.hpp header.

We have experimented with a fastmap, ie a alphabet-size array of pointers
to the node for the first letter of a key. When this macro is defined, the fastmap
is installed and maintained by insertion (but \b not by erase!)

For ex, to look up "shortcut", the \ref containers::ternary_tree::find_impl "find_impl" method 
doesn't begin at root, instead it starts at <tt>fastmap['s']</tt>. 
This turns out to give insignificant improvement for ASCII dictionaries: 
While there is a weak improvement of lookup times, 
it's rarely over 3-5%, usually less than time jitter of the test runs.
For unbalanced (sorted-insertion) trees, insert() is over 10% faster 
(improving as the tree grows).

With \c wchar_t and a large alphabet, it might be useful.

It might be useful to provide fastmap of the 2 first characters. 
This is not tested, as it would be complex to code.
Also with the future subkey_iterator type, it will be easy to build a fastmap 
in client code, eg a vector of subkey_iterators. 
So this macro will probably be removed (still it adds only half a dozen lines total).


*/ 