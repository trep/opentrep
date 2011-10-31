// Created Mon Feb 20 11:47:06 2006
#ifndef TESTS_COMMON_HPP_INCLUDED
#define TESTS_COMMON_HPP_INCLUDED

// MSVC 
#ifdef _MSC_VER
#pragma warning(disable : 4127 4189 4245 4267 4535 4702)
#endif

#include "../ternary_tree.hpp"
#include "../wstring.hpp"
#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

typedef std::vector<std::string> Dictionary;

#define _WIN32_WINNT 0x0400
#define main test_main
#include <boost/test/minimal.hpp>
#undef main
#undef min
#undef max

using boost::minimal_test::errors_counter;

#define REPORT_CHECK(testcase) \
	errors = testcase; \
	std::cerr << (errors? "FAIL " : "OK ") << #testcase << " (ternary_tree)\n";


#define TRY_WRAPPER( Pred ) \
	try { \
		Pred; \
	} catch(...) { \
		std::cerr << #Pred << " throw at " << __FILE__ << ":" << __LINE__ << "\n"; \
		++errors_counter(); \
	}


template <typename T>
inline std::basic_string<char> to_string(const T& x)
{
	std::basic_ostringstream<char> ostr;
	ostr << x;
	return ostr.str();
}



#endif  // TESTS_COMMON_HPP_INCLUDED
