
#include "tests_common.hpp"

#include "../ddj_header.h"


// check that difference is MORE than diff
bool check_leven_diff(const std::string& match, const std::string& ref, int diff)
{
	int lendiff = abs((int)match.size() - (int)ref.size());
	if (lendiff > diff)
		return true;
	const char* mc = match.data(), *rc = ref.data();
	int len = std::min(ref.size(), match.size());
	int mismatch = 0;
	while(len--) {
		if (*mc != *rc)
			++mismatch;
		++mc, ++rc;
	}
	return (lendiff + mismatch) <= diff;
}


void mutate(std::string& s, int pos)
{
	char cc[] = "FGHIOPQRSTUVWXYZ";
	enum { replace, shift, remove };
	int action = rand() % 3;
	char val = 'A' + char(rand() % 25); // cc[RAND() % 16]
	if (action <= replace)
		s[pos] = val;
	else if (action <= shift)
		s.insert(pos, 1, s[pos]);  //val);
	else
		s.erase(pos, 1);
}

int levenshtein_search_test()
{
	long result = errors_counter();

	// Create a tree with known levenshtein count = 100
	std::string b0("abcdefjklmn");
	int testcount = 3;
	while ( testcount ) {
		std::random_shuffle(b0.begin(), b0.end());
		int baselen = (int)b0.size();
		typedef containers::ternary_tree<std::string, std::string> Tst;
		Tst names;
		int L1count = 0, L2count = 0, L3count = 0, L4count = 0;
		for (int i = 0; i < baselen; i++)
		{
			std::string s1(b0);
			mutate(s1, i);
			if (names.insert(std::make_pair(s1, s1)).second)
				L1count++;
			for (int j = 0; j < (int)s1.size(); j++)
			{
				if (j == i)
					continue;
				std::string s2(s1);
				mutate(s2, j);
				if (names.insert(std::make_pair(s2, s2)).second)
					L2count++;
				for (int k = 0; k < (int)s2.size(); k++)
				{
					if (abs(k - j) <= 1 || abs(k - i) <= 1)
						continue;
					std::string s3(s2);
					mutate(s3, k);
					//if (s3.size() < s2.size() && s3.size() == s1.size())
					//	continue;
					//if (names.insert(std::make_pair(s3, s3)).second)
					//	L3count++;
					for (int m = 2; m < (int)s3.size() - 2; m++)
					{
						if (abs(m - j) <= 2 || abs(m - i) <= 2 || abs(m - k) <= 2)
							continue;
						std::string s4(s3);
						mutate(s4, m);
						if (m + 1 >= (int)s4.size())
							continue;
						mutate(s4, m + 1);
						if (m - 1 < 0)
							continue;
						mutate(s4, m - 1);
						if (m + 2 >= (int)s4.size())
							continue;
						mutate(s4, m + 2);
						if (m - 2 < 0)
							continue;
						mutate(s4, m - 2);
						if (abs((int)s4.size() - (int)b0.size()) < 2) {
							s4.insert(rand()%s4.size(), 1, 'Z');
							s4.insert(rand()%s4.size(), 1, 'Z');
						}
						if (names.insert(std::make_pair(s4, s4)).second)
							L4count++;
					}
				}
			}
		}

		if (names.size() != L1count + L2count + L3count + L4count) {
			std::cout << "levenshtein tree name collision, try again";
			continue;
		}
		Tst::search_results_list results = names.create_search_results();
		typedef Tst::search_results_list::iterator ResultsIter;
		names.levenshtein_search(b0, std::back_inserter(results), 2);
		if (results.size() != L1count + L2count + L3count)
		{
			Tst found;
			for (ResultsIter rit = results.begin(); rit != results.end(); ++rit)
				found[**rit] = **rit;
			BOOST_CHECK(found.size() == L1count + L2count); // + L3count);
			std::cout << "\nSearch " << b0 //<< ", L-count: " << names.levenshtein_search_count
					<< ", found " << results.size() << " strings";
			std::cout << " (" << found.size() << " in " << found.node_count() << " nodes)\n";
			if (found.size() != L1count + L2count) { // + L3count) {
				Tst::iterator fit = found.begin();
				for (Tst::iterator nit = names.begin(); nit != names.end(); ++nit)
				{
					if (fit != found.end() && *nit == *fit) {
						std::cout << *nit << "\n";
						++fit;
					}
					else
						std::cout << *nit << " ###\n";
				}
			//	std::cout << "full-frontal:\n";
			//	for (ResultsIter rit = results.begin(); rit != results.end(); ++rit)
			//		std::cout << **rit << "\n";
			}
		}
		testcount--;
	}
	return errors_counter() - result;
}


bool check_hamming_match(const std::string& match, const std::string& ref, int diff)
{
	int lendiff = abs((int)match.size() - (int)ref.size());
	if (lendiff > diff)
		return false;
	const char* mc = match.data(), *rc = ref.data();
	int len = std::min(ref.size(), match.size());
	int mismatch = 0;
	while(len--) {
		if (*mc != *rc)
			++mismatch;
		++mc, ++rc;
	}
	return (lendiff + mismatch) <= diff;
}

template<class CharT>
struct nocase_less : public std::binary_function<CharT, CharT, bool>
{
    bool operator()(const CharT& a, const CharT& b) const { return tolower(a) < tolower(b); }
};


int hamming_search_test(const char* filename)
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

	// hamming_search(2): ABcd, AbcD, Abcd, aBCd, abCD, abCd, abc, abcde
	// levenshtein_search(2): hamming + abCDd, abCcd bcd, bcdc

	Tst::search_results_list matches = names.create_search_results();
	typedef Tst::search_results_list::iterator ResultIter;

	//names.levenshtein_search_count = 0;
	names.levenshtein_search("abcd", std::back_inserter(matches), 2);
	//std::cout << "l-count: " << names.levenshtein_search_count << "\n";
	//names.levenshtein_search_count = 0;
	BOOST_CHECK(matches.size() == 12);
	if (matches.size() != 12)
	{
		// compare with DDJ
		{
			Tptr root = 0;
			for (Tst::iterator it = names.begin(); it != names.end(); ++it) {
				insertstr = (char*)*it;
				root = insert2(root, insertstr);
			}
			nearsearch(root, "abcd", 2);
			std::cout << "DDJ nearsearch abcd:\n";
			for (int i = 0; i != srchtop; i++)
				std::cout << srcharr[i] << "\n";
			std::cout << "\n";
			cleanup2(root);
		}
		int i = 0;
		for (ResultIter rit = matches.begin(); rit != matches.end(); ++rit, ++i) {
			std::cout << **rit << " = " << (*rit).key();
			std::cout /*<< " = " << matches[i].key()*/ << "\n";
		}
	}

	typedef std::vector<std::string> Dictionary;
	extern size_t fill_dictionary(const char*, Dictionary&, size_t, size_t = 0);
	Dictionary dictionary;
	size_t longest_in_file = fill_dictionary(filename, dictionary, 300);
	std::random_shuffle(dictionary.begin(), dictionary.end());

	names.clear();

	Dictionary wildnames;
	// Add names with mini-variations
	long zeroes = 0;
	Dictionary::iterator dit;
	for (dit = dictionary.begin(); dit != dictionary.end(); ++dit) {
		const std::string& name(*dit);
		std::string namecopy(*dit);
		names[namecopy] = name.c_str();
		std::string searchstr(name);
		for (int i = 0; i < 5; ++i) {
			int where = rand() % name.size();
			// make string with ? at place of changed char
			if (searchstr[where] == '?')
				continue;
			searchstr[where] = '?';
			wildnames.push_back(searchstr);
			char c = (char)rand();
			if (!c) zeroes++;
			namecopy[where] = c;
			names[namecopy] = name.c_str();
		}
	}

	for(dit = wildnames.begin(); dit != wildnames.end(); ++dit) {
		const std::string& name(*dit);
		for (int diff = 1; diff != 3; ++diff) {
			Tst::search_results_list matchresults = names.create_search_results();
			names.hamming_search(name, std::back_inserter(matchresults), diff);
			//if (matchresults.size() == 0)
			//	std::cout << "couldn't find " << name << '\n';
			//BOOST_CHECK(found == matchresults.size());
			for (size_t i = 0; i != matchresults.size(); ++i)
				BOOST_CHECK(check_hamming_match(matchresults[i].key(), name, diff));
		}
	}

	return errors_counter() - result;
}
