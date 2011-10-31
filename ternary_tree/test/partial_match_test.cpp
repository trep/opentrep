
#include "tests_common.hpp"

bool check_partial_match(const std::string& match, const std::string& ref)
{
	if (match.size() != ref.size())
		return false;
	const char* mc = match.data(), *rc = ref.data();
	int len = match.size();
	while(len--) {
		if (*mc != *rc && *rc != '?')
			return false;
		++mc, ++rc;
	}
	return true;
}

int partial_match_test(const char* filename)
{
	long result = errors_counter();
	typedef std::vector<std::string> Dictionary;
	extern size_t fill_dictionary(const char*, Dictionary&, size_t, size_t = 0);
	Dictionary dictionary;
	size_t longest_in_file = fill_dictionary(filename, dictionary, 300);
	std::random_shuffle(dictionary.begin(), dictionary.end());

	typedef containers::ternary_tree<std::string, const char*> Tst;
	Tst names;

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
		Tst::search_results_list matchresults = names.create_search_results();
		names.partial_match_search(name, std::back_inserter(matchresults));
		//if (matchresults.size() == 0)
		//	std::cout << "couldn't find " << name << '\n';
		BOOST_CHECK(matchresults.size() != 0);
		for (size_t i = 0; i != matchresults.size(); ++i)
			BOOST_CHECK(check_partial_match(matchresults[i].key(), name));
	}

	typedef containers::ternary_tree<std::wstring, const char*> WTst;
	WTst wnames;
	wnames[L"gone"] = "gone";
	wnames[L"gain"] = "gain";
	wnames[L"ban"] = "ban";
	wnames[L"band"] = "band";
	wnames[L"bind"] = "bind";
	wnames[L"bend"] = "bend";
	wnames[L"bond"] = "bond";
	wnames[L"bene"] = "bene";
	wnames[L"bent"] = "bent";
	wnames[L"bone"] = "bone";
	wnames[L"goner"] = "goner";
	wnames[L"band-aid"] = "band-aid";
	wnames[L"binding"] = "binding";
	WTst::search_results_list found = wnames.create_search_results();
	wnames.partial_match_search(L"b?nd", std::back_inserter(found));
	BOOST_CHECK(found.size() == 4);
	found.clear();
	wnames.partial_match_search(L"b.nd", std::back_inserter(found), '.');
	BOOST_CHECK(found.size() == 4);
	found.clear();
	wnames.partial_match_search(L"b?n?", std::back_inserter(found));
	BOOST_CHECK(found.size() == 7);
	found.clear();
	wnames.partial_match_search(L"b.n.", std::back_inserter(found), '.');
	BOOST_CHECK(found.size() == 7);
	found.clear();
	wnames.partial_match_search(L"??n?", std::back_inserter(found));
	BOOST_CHECK(found.size() == 8);
	found.clear();
	wnames.partial_match_search(L"..n.", std::back_inserter(found), '.');
	BOOST_CHECK(found.size() == 8);
	found.clear();

	return errors_counter() - result;
}
