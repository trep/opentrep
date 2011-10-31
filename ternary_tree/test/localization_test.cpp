#include "tests_common.hpp"


#include "../examples/locale_less.hpp"


int localization_test()
{
	long result = errors_counter();

	typedef containers::ternary_tree<std::string, int, utility::locale_less<char> > LocTst;
	LocTst names(utility::locale_less<char>::locale_less(utility::swedish_locale_name));

	names["aеa"] = 2;
	names["aде"] = 3;
	names["aaе"] = 1;
	names["aце"] = 4;
	names["еее"] = 11;
	names["едц"] = 12;
	names["цде"] = 17;
	names["дец"] = 14;
	names["дце"] = 16;
	names["ддц"] = 15;
	names["еце"] = 13;

	bool display = false;
	BOOST_CHECK(names.size() == 11);
	LocTst::iterator it = names.begin();
	int x = 0, prev = 0;
	while (it != names.end())
	{
		x = *it++;
		BOOST_CHECK(x > prev);
		if (x <= prev) {
			display = true;
			break;
		}
		prev = x;
	}
	if (display) {
		for (it = names.begin(); it != names.end(); ++it) {
			std::string key = it.key();
			CharToOem(key.c_str(), (char*)key.c_str());
			std::cout << key << ", ";
		}
	}

	return errors_counter() - result;
}

