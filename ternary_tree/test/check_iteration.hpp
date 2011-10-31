#ifndef TEST_ITERATION_HPP_INCLUDED
#define TEST_ITERATION_HPP_INCLUDED

#include "tests_common.hpp"


// iterate, check that string count matches expectation, 
// optionally check that one string is NOT encountered
// works on ternary_tree<std::basic_string<CharT>, const CharT*>
template<class Tst, class CharT>
int iteration_quickcheck(Tst& tst, size_t expectcount,
						 CharT* unexpected = 0)
{
	long result = errors_counter();
	size_t count = 0;
	Tst::iterator first = tst.begin();
	Tst::iterator last = tst.end();
	const char *last_value = 0;
	while(first != last) {
		if (first.key() != *first)
			std::cout << first.key() << " != " << *first << '\n';
		BOOST_CHECK(first.key() == *first);
		last_value = first.value();
		if (unexpected)
			BOOST_CHECK(first.key() != unexpected);
		++first;
		count++;
	}
	BOOST_CHECK(first == tst.end());
	if (count != expectcount)
		std::cout << count << " < " << expectcount << '\n';
	BOOST_CHECK(count == expectcount);

	count = 0;
	first = --tst.end();
	Tst::const_reverse_iterator rit = tst.rbegin();
	BOOST_CHECK(*first == last_value);
	last = tst.end();
	while(first != last) {
		if (first.key() != *first)
			std::cout << first.key() << " != " << *first << '\n';
		if (*first != *rit)
			std::cout << *first << " != " << *rit << '\n';
		BOOST_CHECK(first.key() == *first);
		BOOST_CHECK(*first == *rit);
		if (unexpected)
			BOOST_CHECK(first.key() != unexpected);
		--first;
		++rit;
		count++;
	}
	BOOST_CHECK(rit == const_cast<const Tst&>(tst).rend());
	if (count != expectcount)
		std::cout << count << " < " << expectcount << '\n';
	return errors_counter() - result;
}

#endif