
#include "tests_common.hpp"


void check_prefixmatch( Dictionary::iterator dit, Dictionary::iterator endit,
						const std::string& name, const std::string& match)
{
	while(dit != endit) {
		std::string& str(*dit);
		if (0 != str.compare(0, name.size(), name))
			break;
		++dit;
	}
	BOOST_CHECK(match == *dit || (dit == endit && match.empty()));
	if (match != *dit && dit != endit)
		std::cerr << match << " != " << *dit << '\n';
}

int prefix_range_test(const char* filename)
{
	long result = errors_counter();
	extern size_t fill_dictionary(const char*, Dictionary&, size_t, size_t = 0);
	Dictionary dictionary;
	size_t longest_in_file = fill_dictionary(filename, dictionary, 2000);

	// insert only half dictionary, in random order
	Dictionary::iterator endit = dictionary.begin() + dictionary.size() / 2;
	Dictionary randdic(dictionary.begin(), endit);
	std::random_shuffle(randdic.begin(), randdic.end());

	typedef containers::ternary_tree<std::string, const char*> Tst;
	Tst names;
	for (Dictionary::iterator it = randdic.begin(); it != randdic.end(); ++it)
		names.insert(std::make_pair(*it, (*it).c_str()));
	randdic.clear();

///// prefix_range test
	Dictionary::iterator dit;
	size_t prefix_found = 0;
	for(dit = dictionary.begin(); dit != endit; ++dit) {
		const std::string& name(*dit);
		std::pair<Tst::const_iterator, Tst::const_iterator> result 
			= names.prefix_range(name);
		BOOST_CHECK(result.first == names.find(name));
		if (result.first != names.end())
			prefix_found++;
		// check second
		if (result.second != names.end())
			check_prefixmatch(dit, endit, name, result.second.key());
		else
			check_prefixmatch(dit, endit, name, "");
		// don't bother to clip short names
		if (name.size() < 3) {
			continue;
		}
		// get shorter name too
		int clip = 2 + rand() % (name.size() - 2);
		std::string clipped(name.begin(), name.begin() + clip);
		result = names.prefix_range(clipped);
		if (result.first != names.end())
			prefix_found--;
		if (result.second != names.end())
			check_prefixmatch(dit, endit, clipped, result.second.key());
		else
			check_prefixmatch(dit, endit, clipped, "");
	}

	// now dit is at midpoint, so (most of) rest should be unfound
	size_t unfound = 0;
	bool should_find;
	for( ; dit != dictionary.end(); ++dit) {
		should_find = (names.find(*dit) != names.end());
		std::pair<Tst::const_iterator, Tst::const_iterator> result 
			= names.prefix_range(*dit);
		// prefix_range should get same (negative) result as find
		if (should_find == (result.first != names.end()))
			unfound++;
	}
	BOOST_CHECK(unfound > (names.size() - names.size() / 4));

	return errors_counter() - result;
}

