#ifndef BASIC_INSERTION_TEST_HPP_INCLUDED
#define BASIC_INSERTION_TEST_HPP_INCLUDED

#include "tests_common.hpp"


// Test construction and clear() postconditions
template<class CharT, class DataT> 
int check_empty(containers::ternary_tree<std::basic_string<CharT>, DataT>& tst)
{
	typedef containers::ternary_tree<std::basic_string<CharT>, DataT> Tst;

	long result = errors_counter();
	BOOST_CHECK(tst.empty());
	BOOST_CHECK(tst.node_count() == 0);
	BOOST_CHECK(tst.item_count() == 0);

	Tst::iterator i2(tst.begin());
	BOOST_CHECK(i2 == tst.end());

	return errors_counter() - result;
}


// insert "a" = 1, "b" = 2 etc, out of order.
// test that they are ordered and have correct value.
// insert b, d, f by operator[]
// iterator used to check order of strings.
template<class CharT> 
int basic_insertion_test()
{
	long result = errors_counter();

	typedef containers::ternary_tree<std::basic_string<CharT>, int> Tst;
	Tst tst;

	const char* strings[] = { "\0", "aa", "aab", "aac", "add", "aee", "bab" };
	widen<CharT> w;

	// Construction postconditions
	check_empty(tst);

	// Standard insert()
	tst.insert(std::make_pair(w(strings[1]), 1));
	tst.insert(std::make_pair(w(strings[5]), 5));

	std::pair<std::basic_string<CharT>, int> inval(w(strings[3]), 3);
	std::pair<Tst::iterator, bool> vr = tst.insert(inval);
	BOOST_CHECK(vr.first != tst.end());
	BOOST_CHECK(vr.second == true);
	vr = tst.insert(inval);
	BOOST_CHECK(vr.first != tst.end());
	BOOST_CHECK(vr.second == false);

	BOOST_CHECK((*tst.find(w(strings[1]))) == 1);
	BOOST_CHECK((*tst.find(w(strings[3]))) == 3);
	BOOST_CHECK((*tst.find(w(strings[5]))) == 5);

	BOOST_CHECK(tst.item_count() == 3);
	BOOST_CHECK(tst.total_key_length() == 3*3-1);

	// In present version, no reassignment
	tst.insert(std::make_pair(w(strings[1]), 4));
	BOOST_CHECK((*tst.find(w(strings[1]))) == 1);
	BOOST_CHECK(tst.item_count() == 3);
	BOOST_CHECK(tst.total_key_length() == 3*3-1);

	// reference_proxy insert()
	tst[w(strings[2])] = 2;
	tst[w(strings[6])] = 6;
	tst[w(strings[4])] = 4;
	BOOST_CHECK(tst[w(strings[2])] == 2);
	BOOST_CHECK(tst[w(strings[4])] == 4);
	BOOST_CHECK(tst[w(strings[6])] == 6);

	BOOST_CHECK(tst.item_count() == 6);
	BOOST_CHECK(tst.total_key_length() == 6*3-1);

	// Now do reassignment
	tst[w(strings[1])] = 7;
	BOOST_CHECK((*tst.find(w(strings[1]))) == 7);  //1);
	tst[w(strings[1])] = 1;

	// iterator used to check sort order of strings
	Tst::const_iterator it(tst.begin());
	int expect_val = 1;
	while (it != tst.end()) {
		Tst::value_type val = *it;
		// string == "a", "b" etc
		BOOST_CHECK(expect_val == 1 || it.key().size() == 3);
		BOOST_CHECK(it.key() == w(strings[expect_val]));
		// value is 1, 2 etc
		BOOST_CHECK(val == expect_val++);
		++it;
	}
	BOOST_CHECK(expect_val == int(tst.item_count() + 1));

	// Test clear() postconditions
	tst.clear();

	check_empty(tst);

	return errors_counter() - result;
}

#endif