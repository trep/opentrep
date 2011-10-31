
#include "tests_common.hpp"

typedef containers::ternary_tree<std::string, const char*> ScrabbleTst;

static long scrabble_testcount, scrabble_findcount, scrabble_duplicates;

bool check_scrabble_found(const std::string& searchkey, 
						  const std::string& match, int jokers)
{
	const char* mc = match.data();
	int len = match.size();
	std::string keycopy(searchkey);
	while(len--) {
		size_t pos = keycopy.find(*mc);
		if (pos == std::string::npos) {
			if (--jokers < 0) {
				std::cout << "SCRABBLE(" << jokers << "): " << match << " not found with " << searchkey << "\n";
				return false;
			}
		}
		else 
			keycopy.erase(pos, 1);
		++mc;
	}
	return true;
}

bool check_scrabble_missed(ScrabbleTst& tst, ScrabbleTst& found, int jokers)
{
	typedef std::pair<std::string::iterator, std::string::iterator> MissPair;
	for (ScrabbleTst::iterator A = tst.begin(); A != tst.end(); ++A)
	{
		std::string tstr(A.key());
		std::sort(tstr.begin(), tstr.end());
		if (found.find(tstr) != found.end())
			continue;
		for (ScrabbleTst::iterator B = found.begin(); B != found.end(); ++B)
		{
			if (tstr.size() > B.key().size())
				continue;
			++scrabble_testcount;
			std::string fs(B.key());
			MissPair miss = std::mismatch(fs.begin(), fs.end(), tstr.begin());
			if (miss.first == fs.end() && miss.second == tstr.end()) {
				std::cout << "SCRABBLE(" << jokers << "): " << tstr << " not found by " << fs << "\n";
				return false;
			}
		}
	}
	return true;
}

int exhaustive_scrabble_test(const char* filename)
{
	long result = errors_counter();
	// note that dictsize*keys*3 tests are performed
	size_t DICTSIZE = 10000, KEYS = 10;
	typedef std::vector<std::string> Dictionary;
	extern size_t fill_dictionary(const char*, Dictionary&, size_t, size_t = 0);
	Dictionary dictionary;
	size_t longest_in_file = fill_dictionary(filename, dictionary, DICTSIZE);
	std::random_shuffle(dictionary.begin(), dictionary.end());

	ScrabbleTst names;

	Dictionary::iterator dit;
	for (dit = dictionary.begin(); dit != dictionary.end(); dit += 4) {
		const std::string& name(*dit);
		names[name] = name.c_str();
	}

	int skip = dictionary.size() / KEYS;
	for(dit = dictionary.begin() + skip; dit != dictionary.end(); dit += skip)
	{
		std::string name(*dit);
		std::sort(name.begin(), name.end());
		for (int jokercount = 0; jokercount < 3; ++jokercount) {
			ScrabbleTst::search_results_list matchresults = names.create_search_results();
			names.combinatorial_search(name, std::back_inserter(matchresults), jokercount);
			//if (matchresults.size() == 0)
			//	std::cout << "couldn't find " << name << '\n';
			BOOST_CHECK(matchresults.size() != 0);
			ScrabbleTst matches;
			ScrabbleTst matchcount;
			size_t prevsize = 0;
			for (size_t i = 0; i != matchresults.size(); ++i) {
				std::string mcopy(matchresults[i].key());
				matchcount[mcopy] = mcopy.c_str();
				if (matchcount.size() > prevsize) {
					BOOST_CHECK(check_scrabble_found(name, mcopy, jokercount));
					++prevsize;
				}
				else {
					std::cout << "scrabble found duplicate " << mcopy << "\n";
				}
				std::sort(mcopy.begin(), mcopy.end());
				matches[mcopy] = mcopy.c_str();
			}
			// check for duplicates
			scrabble_findcount += matchresults.size();
			scrabble_duplicates += matchresults.size() - matchcount.size();
			BOOST_CHECK(matchcount.size() == matchresults.size());
			BOOST_CHECK(check_scrabble_missed(names, matches, jokercount));
		}
	}
	//std::cout << "scrabble duplicate matches " << scrabble_duplicates << " of " << scrabble_findcount << "\n";

	return errors_counter() - result;
}


int trivial_scrabble_test()
{
	long result = errors_counter();

	typedef containers::ternary_tree<std::string, const char*> Tst;
	Tst names;
	names["ABCD"] = "ABCD";
	names["Abcd"] = "Abcd";
	names["ABcd"] = "ABcd";
	names["aBCd"] = "aBCd";
	names["abc"] = "abc";
	names["abcde"] = "abcde";
	names["bcd"] = "bcd";
	names["abCD"] = "abCD";
	names["abCd"] = "abCd";
	names["AbcD"] = "AbcD";
	names["ABcD"] = "ABcD";
	names["aBCD"] = "aBCD";
	names["abCDE"] = "abCDE";
	names["abCDd"] = "abCDd";
	names["abCcd"] = "abCcd";
	names["bcdc"] = "bcdc";
	names["aab"] = "aab";

	Tst::search_results_list matches = names.create_search_results();
	typedef Tst::search_results_list::iterator ResultIter;

	int i = 0;
	std::string s("abcde");
	names.combinatorial_search(s, std::back_inserter(matches));
	BOOST_CHECK(matches.size() == 3);
	matches.clear();
	std::random_shuffle(s.begin(), s.end());
	names.combinatorial_search(s, std::back_inserter(matches));
	BOOST_CHECK(matches.size() == 3);
	// with wildcards
	matches.clear();
	std::random_shuffle(s.begin(), s.end());
	names.combinatorial_search(s, std::back_inserter(matches), 1);
	BOOST_CHECK(matches.size() == 8);
	//for (ResultIter rit = matches.begin(); rit != matches.end(); ++rit, ++i)
	//	std::cout << i << ": " << **rit << "\n";
	matches.clear();
	s.append(s);
	std::random_shuffle(s.begin(), s.end());
	names.combinatorial_search(s, std::back_inserter(matches));
	BOOST_CHECK(matches.size() == 5);
	//for (ResultIter rit = matches.begin(); rit != matches.end(); ++rit, ++i)
	//	std::cout << i << ": " << **rit << "\n";
	return errors_counter() - result;
}


int scrabble_search_test(const char* filename)
{
	long result = errors_counter();
	trivial_scrabble_test();
	exhaustive_scrabble_test(filename);
	return errors_counter() - result;
}

