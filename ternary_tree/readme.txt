LIBRARY
Ternary Search Tree C++ implementation by rasmus ekman
A header-only library of fast string containers with advanced search features.

version 0.67, 14 May 2006

Please send bug reports, suggestions or questions to 	rasmus.ekman@abc.se 
Get latest version at http://abc.se/~re/code/tst/

REQUIREMENTS
Library files tested with g++ 3.4.3 and MSVC 7.1 (Visual Studio 2003).
Visual Studio 6 will not work, but may only need moving the template
methods of ternary_tree and structured_* classes inline.
To generate documentation, you need Doxygen. See http://doxygen.org.


USAGE
Container classes:
	structured_set<Key [, Comp, Alloc]>
	structured_multiset<Key [, Comp, Alloc]>
	structured_map<Key, Value [, Comp, Alloc]>
	structured_multimap<Key, Value [, Comp, Alloc]>
- Key is a std::string-like type (a Forward Container),
- Value is any type,
- Comp is a less-like sort operation on Key::value_type (eg char/wchar_t)
- Alloc is std::allocator<Key [, Value]> or has same interface.

These containers can be used as nearly drop-in replacments for std::set, 
multiset, map, multimap or unordered_* containers on string-like types.

There is one difference in interface: 
If you used non-default comparator template argument with a set or map type, 
it must be changed to operate on character type, not string. 

To use standard set, map features: See documentation of these classes.
See included documentation for information about the advanced key search
facilities in all structured_* containers.


FILES
--- l i b r a r y   c o d e ---
structured_map.hpp - classes structured_map and -multimap.
structured_set.hpp - classes structured_set and multiset.
ternary_tree.hpp -  implementation backend class.
./tst_detail/.* ternary_tree implementation files
iterator_wrapper.hpp - iterator interface, included by all containers.

--- d o c s ---
tst_public.doxy - doxygen config file, generates public interface of library.
tst.doxy - doxygen config file, generates public and private interface docs.
./doxygen_input/* - extra documentation sources used by Doxygen.

./html/* - generated public and private documentation
full-docs-index.html - redirects to html directory
index.html - redirects to tst_docs directory, only useful if doxygen is used 
	with tst_public.doxy

--- t e s t s ---
tst_concept_checks.cpp - requires Boost concept_check header, portable.
iterator_compile_test.cpp - checks iterator_wrapper interoperability, portable.

test_tst.cpp - test suite; relies on non-portable wstring.hpp (but see below)
fill_dictionary.cpp - (sloppy old support file for test_tst)
	fills a vector with strings from file.
wstring.hpp - string/wstring conversion, uses Windows API
	if the MultiByteToWideChar and WideCharToMultiByte API calls are replaced, 
	test_tst may run on your platform...

---
rasmus ekman
May 14, 2006
