#ifndef ELEMENT_RANGE_TEST_HPP_INCLUDED
#define ELEMENT_RANGE_TEST_HPP_INCLUDED

#include "tests_common.hpp"


// Test construction and clear() postconditions
//template<class CharT, class DataT> 


// Check that \0 and -1 etc can be stored
template<class CharT> 
int element_range_test()
{
	typedef containers::ternary_tree<std::basic_string<CharT>, int> Tst;
	typedef typename Tst::key_type S;
	Tst tst;

	long result = errors_counter();

	widen<Tst::char_type> w;

	tst.insert(std::make_pair(w.str("a\0a\0", 4), 5));
	tst.insert(std::make_pair(w.str("b\0b\0", 4), 6));
	tst.insert(std::make_pair(w.str("\0a\0a", 4), 1));
	tst.insert(std::make_pair(w.str("\0b\0b", 4), 2));
//	tst.insert(std::make_pair(w.str("\0", 1), -1));

	BOOST_CHECK((*tst.find(w.str("a\0a\0", 4))) == 5);
	BOOST_CHECK(tst.item_count() == 4);
	BOOST_CHECK(tst.total_key_length() == 16);

	// no reinsertion
	tst.insert(std::make_pair(w.str("a\0a\0", 4), 13));
	BOOST_CHECK((*tst.find(w.str("a\0a\0", 4))) == 5);
	BOOST_CHECK(tst.item_count() == 4);
	BOOST_CHECK(tst.total_key_length() == 16);

	tst[w.str("d\0d\0", 4)] = 8;
	tst[w.str("c\0c\0", 4)] = 7;
	tst[w.str("\0d\0d", 4)] = 4;
	tst[w.str("\0c\0c", 4)] = 3;

	tst[w.str("\0d\0d", 4)] = 13;
	BOOST_CHECK(tst[w.str("\0d\0d", 4)] == 13);
	tst[w.str("\0d\0d", 4)] = 4;  // now reassignable
	BOOST_CHECK(tst.item_count() == 8);
	BOOST_CHECK(tst.total_key_length() == 32);

	tst[w.str("c\0c\0", 4)] = 15;
	BOOST_CHECK(tst[w.str("c\0c\0", 4)] == 15);
	tst[w.str("c\0c\0", 4)] = 7;

	tst[w.str("a\0\xff\0едц\0\xff\0", 10)] = 10;
	BOOST_CHECK((*tst.find(w.str("a\0\xff\0едц\0\xff\0", 10))) == 10);
	BOOST_CHECK(tst.item_count() == 9);
	BOOST_CHECK(tst.total_key_length() == 32 + 10);

	Tst::iterator it(tst.begin());
	int expect_val = 1;
	int expect_char = *w("a");
	for (size_t i = 0; i < tst.item_count(); ++i)
	{
		//Tst::value_type val = *it;
		// test the long string
		if (it.key().size() == 10) {
			BOOST_CHECK(it.key() == w.str("a\0\xff\0едц\0\xff\0", 10));
			BOOST_CHECK(*it == 10);
			++it;
			continue;
		}
		// string == "a", "b" etc
		BOOST_CHECK(it.key().size() == 4);
		int zeroindex = (i < 4)? 0 : 1;
		BOOST_CHECK(it.key().at(zeroindex) == 0);
		BOOST_CHECK(it.key().at(1 - zeroindex) == expect_char++);
		if (i == 3)
			expect_char -= 4;
		// value is 1, 2 etc
		BOOST_CHECK(*it == expect_val++);
		++it;
	}

	return errors_counter() - result;
}

#endif