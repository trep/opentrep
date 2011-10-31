/** \file
 * Usage examples for Structured Containers.
 */

#include <iostream>
#include "structured_set.hpp"
#include "structured_map.hpp"

#include <iostream>
#include <string>
#include <set>
#include <functional>
//#include <boost/scoped_ptr.hpp>

//
// Basic use of structured_set
//

void basic()
{
	typedef containers::structured_set<std::string> Set;
	typedef Set::iterator SetIter;
	typedef std::pair<SetIter, SetIter> IterPair;
	
	Set names;
	names.insert("apps");
	names.insert("applets");
	names.insert("banana");

	std::cout << "The set contains\n\t";
	for (SetIter it = names.begin(); it != names.end(); ++it)
		std::cout << *it << ", ";

	IterPair p = names.prefix_range("app");
	std::cout << "\nprefix_range(\"app\") returns:\n\t";
	while (p.first != p.second) {
	    std::cout << *p.first++ << ", ";
	}
	std::cout << "\np.second points to " << *p.second;

	std::cout << "\nequal_range(\"app\") returns:\n\t";
	p = names.equal_range("app");
	if (p.first == p.second)
		std::cout << "empty range";
	std::cout << "\np.second points to " << *p.second;
}


//#############################################################################
//
// prefix_range example (compile-only, does not run as is)
//
typedef containers::structured_set<std::string> SymbolSet;
SymbolSet symbols;
bool is_defined_in_scope(std::string scope, std::string name)
{
    typedef std::pair<SymbolSet::iterator, SymbolSet::iterator> Range;
	Range r = symbols.prefix_range(scope + "::");
    SymbolSet::iterator n = symbols.find(name);
    return n != symbols.end() && *n >= *r.first && *n < *r.second;
}


//#############################################################################
//
// Case-insensitive structured containers
//
template<class CharT>
struct nocase_less : public std::binary_function<CharT, CharT, bool>
{
    bool operator()(CharT a, CharT b) const { return tolower(a) < tolower(b); }
};

void caseless_set()
{
	typedef containers::structured_multiset<std::string, nocase_less<char> > CaselessSet;
	typedef containers::structured_multimap<std::string, double, nocase_less<char> > CaselessMap;

	CaselessMap uncased;
	uncased.insert(std::make_pair("NoCase", 0.1));
	CaselessSet caseless;
	caseless.insert("NoCase");
	caseless.insert("nocase");
	caseless.insert("noCase");
	caseless.insert("NOCASE");

	std::cout << "nocase = " << (int)caseless.count("nocase");

    CaselessSet::const_iterator endit = caseless.end();
	for(CaselessSet::const_iterator it = caseless.begin(); it != endit; ++it) {
		std::cout << ", " << *it;
	}

}

//#############################################################################
//
// Localization comparator
//

#include "examples/locale_less.hpp"

void localized_comparator()
{
	typedef containers::structured_set<std::string, utility::locale_less<char> > LocalSet;

	typedef containers::structured_set<std::string> DefaultSet;

	if (utility::swedish_locale_name == "C")
		std::cout << "No locale to test\n";
	else
		std::cout << "Attempt to set Swedish locale \"" << utility::swedish_locale_name << "\"\n";

  try {
	// use comparator constructor, create Swedish locale
	LocalSet se_names(utility::locale_less<char>::locale_less(utility::swedish_locale_name));
	DefaultSet anynames;

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
	std::cout << "not:\n";
	for(DefaultSet::iterator dit = anynames.begin(); dit != anynames.end(); ++dit) {
		std::cout << *dit << ", ";
	}
  } catch(std::exception& x) {
	std::cout << "...failed - skip test\n" << x.what() << "\n";
  }
}

//#############################################################################
//
// longest_match example
//
#include <fstream>

typedef containers::structured_map<std::string, int, nocase_less<char> > Vocabulary;

void fill_wordlist(const char* filename, Vocabulary& wordlist)
{
	std::ifstream wordstream(filename);
	if (!wordstream.is_open()) {
		std::cerr << "Could not open dictionary " << filename << "\n";
		return;
	}
	char buf[300];
	int linecount = 0;
	while(wordstream.getline(buf, 300, '\n').good())
		wordlist[buf] = ++linecount;
}

std::streamsize get_filesize(std::ifstream& str)
{
	std::streamsize pos = str.tellg();
	str.seekg(0, std::ios_base::end);
	std::streamsize result = str.tellg();
	str.seekg(pos, std::ios_base::beg);
	return result;
}

namespace {
	template<class T>
	struct scoped_array
	{
		scoped_array(size_t count) : buf(new T[count]) {}
		~scoped_array() { delete[] buf; }
		T* get() { return buf; }
	private:
		T* buf;
	};
}

void longest_match_example(const char* dictfile, const char* parsefile)
{
	Vocabulary english;
	// Read dictionary from disk
	fill_wordlist(dictfile, english);
	if (english.empty())
		return;

	std::ifstream infile(parsefile);
	if (!infile.is_open())
		return;

	// longest_match does not work with istream_iterator, so must fill buffer
	size_t filesize = (size_t)get_filesize(infile);
	// instead of boost::scoped_array
	scoped_array<char> bytes(filesize);
	infile.read(bytes.get(), filesize);

    const char *first = bytes.get();
	const char *last = first + infile.gcount();

	while (first != last) 
	{
		Vocabulary::iterator word = english.longest_match(first, last);
		if (word != english.end())
			std::cout << (*word).first << " "; //= " << (*word).second << "\n";
		else {
			// No key; try next char
			++first;
		}
	}
}

//#############################################################################

int main()
{
	std::cout << "*** basic usage ***\n";
	basic();
	std::cout << "\n\n*** custom comparator ***\n";
	caseless_set();
	std::cout << "\n\n*** locale comparator ***\n";
	localized_comparator();
	std::cout << "\n\n*** longest_match ***\n";
	// You need to supply files, not included in ternary_tree distribution
	longest_match_example("../english-150k.txt", "../shakequotes.txt");
	return 0;
}
