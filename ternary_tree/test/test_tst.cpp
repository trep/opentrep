// test_main at bottom of file called by boost execution monitor main
// needs dictionary filename argument (min 2000 lines)

#include "tests_common.hpp"

// defines tests templated on char type
#include "basic_insertion_test.hpp"
#include "element_range_test.hpp"

// Boost.Minimal test doesn't allow separate modules.
// These should be separate projects then (TBD)
#include "mapped_value_test.cpp"
#include "copy_test.hpp"
#include "erase_test.cpp"
#include "iterator_test.cpp"

#include "localization_test.cpp"
#include "prefix_range_test.cpp"
#include "longest_match_test.cpp"

#include "partial_match_test.cpp"
#include "hamming_search_test.cpp"
#include "scrabble_search_test.cpp"




// Argument: pass name of a file with at least 2000 lines of text entries (words)
int main(int argc, char* argv[])
{
	int errors = argc - argc;  // 0

	REPORT_CHECK(basic_insertion_test<char>());
	REPORT_CHECK(basic_insertion_test<wchar_t>());
	REPORT_CHECK(element_range_test<char>());
	REPORT_CHECK(element_range_test<wchar_t>());

	REPORT_CHECK(mapped_value_test());
	REPORT_CHECK(copy_test());

	REPORT_CHECK(localization_test());
	REPORT_CHECK(iterator_test(argv[1]));
	REPORT_CHECK(erase_test(argv[1]));

	REPORT_CHECK(prefix_range_test(argv[1]));
	REPORT_CHECK(longest_match_test());
	REPORT_CHECK(partial_match_test(argv[1]));
	REPORT_CHECK(hamming_search_test(argv[1]));
	REPORT_CHECK(levenshtein_search_test());
	REPORT_CHECK(scrabble_search_test(argv[1]));

	return errors;
}

//#endif