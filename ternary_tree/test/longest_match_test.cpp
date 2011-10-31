
#include "tests_common.hpp"
#include "check_iteration.hpp"

typedef containers::ternary_tree<std::string, const char*> Tst_LM_Test;


// test a single case
bool longest_match_check(Tst_LM_Test& tst, const std::string& str, const std::string& expect)
{
	typedef Tst_LM_Test Tst;
	const char* beg = str.data();
	const char* end = str.data() + str.size();
	Tst::iterator it = tst.longest_match(beg, end);
	BOOST_CHECK(std::string(it.key()) == expect);
	BOOST_CHECK(std::string(it.key()) == std::string(str.data(), beg));
	return expect.empty() == (it == tst.end());
}


int longest_match_test()
{
	long result = errors_counter();
	typedef Tst_LM_Test Tst;
	Tst names;

	// Arjen Wagenar's test case (+2)
    names["Amsterdam"] = "Amsterdam";
    names["San Francisco"] = "San Francisco";
    names["New York"] = "New York";
    names["Brussels"] = "Brussels";
	BOOST_CHECK(longest_match_check(names, "X", ""));
	BOOST_CHECK(longest_match_check(names, "A", ""));
	BOOST_CHECK(longest_match_check(names, "Ams", ""));
	BOOST_CHECK(longest_match_check(names, "New", ""));
	BOOST_CHECK(longest_match_check(names, "Amsterdam the", "Amsterdam"));
	// ...which is not equivalent with the following, 
	// since it gives different tree structure...

	//names.clear();
	names["abc"] = "abc";
	names["ab"] = "ab";
	names["a"] = "a";
	BOOST_CHECK(longest_match_check(names, "ax", "a"));
	BOOST_CHECK(longest_match_check(names, "bc", ""));
	names.erase("a");
	BOOST_CHECK(longest_match_check(names, "ax", ""));
	BOOST_CHECK(longest_match_check(names, "a", ""));
	names["a"] = "a";
	BOOST_CHECK(longest_match_check(names, "ax", "a"));
	BOOST_CHECK(longest_match_check(names, "abx", "ab"));
	// exercise shorter-than-matched backing-up (from "bcd" to "bc")
	names["bcdab"] = "bcdab";
	names["bcdef"] = "bcdef";
	names["bcde"] = "bcde";
	names["bcdaa"] = "bcdaa";
	names["bc"] = "bc";
	BOOST_CHECK(longest_match_check(names, "bcdx", "bc"));
	// and back-up past erase (from "bcdefg" past erased "bcdef" to "bc")
	names["bcdefgh"] = "bcdefgh";
	names.erase("bcdef");
	BOOST_CHECK(longest_match_check(names, "bcdefgx", "bcde"));
	names.erase("bcde");
	BOOST_CHECK(longest_match_check(names, "bcdefgx", "bc"));
	// erased
	names.erase("a");
	BOOST_CHECK(longest_match_check(names, "ax", ""));

	return errors_counter() - result;
}

