
#include "tests_common.hpp"
#include "check_iteration.hpp"


template<class Iter>
int test_erase2(Iter first, Iter last)
{
	long result = errors_counter();
	// fill a vector, then shake it down with random shuffle and erase all
	typedef std::vector<const char*> StrList;
	StrList strings;
	while(first != last) {
		strings.push_back(*first);
		++first;
	}

	// just do a few permutations (could do it O(n^2) times)
	for (size_t i = 0; i < strings.size(); ++i)
	{
		typedef containers::ternary_tree<std::string, const char*> Tst;
		Tst names;
		StrList::const_iterator strs = strings.begin();
		while(strs != strings.end()) {
			names[*strs] = *strs;
			++strs;
		}
		// check that all keys are visible
		iteration_quickcheck(names, names.item_count(), (const char*)0);
		
		// erase in different order from insertion (and insert in this order next time)
		std::random_shuffle(strings.begin(), strings.end());

		// remove half strings and test with iterator between each
		std::vector<const char*>::const_iterator sit = strings.begin();
		while(names.item_count() > 1)
		{
			size_t namesize = names.item_count();
			Tst::iterator nit = names.find(*sit);
			BOOST_CHECK(nit != names.end());
		//	std::cout << "erase " << (*nit).second << ": ";
			names.erase(nit++);
		//	std::cout << names.node_count() << "\n";
			// check that one string less, and that it's the erased one
			iteration_quickcheck(names, namesize - 1, *sit);
			++sit;
		}
		//std::cout << "\n";
	}
	return errors_counter() - result;
}

int erase_test(const char* dictionaryfile)
{
	//const char* s[] = { "\0", "aaa", "bbb", "ccc", "ddd", "eee", "fff", "ggg" };
	const char* s[] = { "\0", "aa", "aaa", "aba", "abaa", "abb", "abc", "abca" };
	const size_t strsum = 22;

	long result = errors_counter();
	typedef containers::ternary_tree<std::string, int> Tst;
	Tst names;
	names.insert(std::make_pair(s[1], 1));
	names.insert(std::make_pair(s[7], 7));
	names.insert(std::make_pair(s[2], 2));
	names.insert(std::make_pair(s[4], 4));
	names.insert(std::make_pair(s[6], 6));
	names.insert(std::make_pair(s[5], 5));
	names.insert(std::make_pair(s[3], 3));
	// quick check we actually have strings
	for (int i = 1; i <= 7; ++i) 
		BOOST_CHECK(*names.find(s[i]) == i);
	BOOST_CHECK(names.item_count() == 7);
	BOOST_CHECK(names.total_key_length() == strsum);
	//diagnostic
	//std::cout << "node count after all insert: " << names.node_count() << "\n";
	Tst::iterator iter = names.find(s[3]);
	Tst::iterator i2 = names.erase(iter++);
	BOOST_CHECK(i2 == iter);
	BOOST_CHECK(*iter == 4);  // erased 3
	iter = names.find(s[4]);
	i2 = names.erase(iter++);
	BOOST_CHECK(i2 == iter);
	BOOST_CHECK(*iter == 5);  // erased 4
	iter = names.find(s[2]);
	i2 = names.erase(iter++);
	BOOST_CHECK(i2 == iter);
	BOOST_CHECK(*iter == 5);  // erased 3, 4
	iter = names.find(s[7]);
	i2 = names.erase(iter++);
	BOOST_CHECK(i2 == iter);
	BOOST_CHECK(iter == names.end());
	iter = names.find(s[1]);
	i2 = names.erase(iter++);
	BOOST_CHECK(i2 == iter);
	BOOST_CHECK(*iter == 5);  // erased 2, 3, 4
	BOOST_CHECK(names.find(s[1]) == names.end());
	BOOST_CHECK(names.find(s[2]) == names.end());
	BOOST_CHECK(names.find(s[3]) == names.end());
	BOOST_CHECK(names.find(s[4]) == names.end());
	BOOST_CHECK(*names.find(s[5]) == 5);
	BOOST_CHECK(*names.find(s[6]) == 6);
	BOOST_CHECK(names.find(s[7]) == names.end());
	BOOST_CHECK(names.item_count() == 2);
	BOOST_CHECK(names.total_key_length() == 2*3);
	BOOST_CHECK(names.erase(s[5]) == 1);
	iter = names.find(s[6]);
	i2 = names.erase(iter++);
	BOOST_CHECK(i2 == iter);
	BOOST_CHECK(iter == names.end());
	BOOST_CHECK(names.empty());
	//std::cerr << "node count after all erased: " << names.node_count() << "\n";

	/*names.insert(std::make_pair(s[3], 3));
	names.insert(std::make_pair(s[7], 7));
	names.insert(std::make_pair(s[4], 4));
	names.insert(std::make_pair(s[2], 2));
	names.insert(std::make_pair(s[1], 1));
	names.insert(std::make_pair(s[6], 6));
	names.insert(std::make_pair(s[5], 5));
	//std::cerr << "node count after reinsertion: " << names.node_count() << "\n";
	*/

	extern size_t fill_dictionary(const char*, Dictionary&, size_t, size_t = 0);
	Dictionary dictionary;
	fill_dictionary(dictionaryfile, dictionary, 2000);
	std::random_shuffle(dictionary.begin(), dictionary.end());
	typedef containers::ternary_tree<std::string, int> StrTst;
	StrTst dict;
	int count = 0;
	for (Dictionary::iterator dit = dictionary.begin(); dit != dictionary.end(); ++dit)
		dict.insert(std::make_pair(*dit, count++));
	StrTst::iterator it = dict.begin();
	count = 0;
	while(it != dict.end()) {
		dict.erase(it++);
		count++;
	}

	// to exercise all paths in erase_impl, make tree dense around "abs*"
	const char* tesst[] = {
		"abacus", "abscissa", "abscond", "absent", "absentee", "absinth", "absolution",  
		"absolve", "abstain", "abstract", "abstractor", "abstruse", "acidic", "acme", 
		"acrimony", "arbiter", "baba", "babylon", "band", "bath", "bathing", "border"
	};

	errors_counter() += test_erase2(tesst, tesst + 22);

	return errors_counter() - result;
}