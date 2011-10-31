
#include "tests_common.hpp"
#include "check_iteration.hpp"


int iterator_test(const char* filename)
{
	long result = errors_counter();
	typedef std::vector<std::string> Dictionary;
	extern size_t fill_dictionary(const char*, Dictionary&, size_t, size_t = 0);
	Dictionary dictionary;
	Dictionary revdict;
	Dictionary randdict;
	size_t longest_in_file = fill_dictionary(filename, dictionary, 1000);

	// Need to make unique and sorted...
	std::set<std::string> sortme(dictionary.begin(), dictionary.end());
	dictionary.assign(sortme.begin(), sortme.end());

	revdict.insert(revdict.begin(),dictionary.rbegin(), dictionary.rend());

	randdict = dictionary;
	std::random_shuffle(randdict.begin(), randdict.end());


	typedef containers::ternary_tree<std::string, const char*> Tst;
	Tst names;


	for (Dictionary::iterator it = randdict.begin(); it != randdict.end(); ++it)
		names.insert(std::make_pair(*it, (*it).c_str()));

	// forward iteration
	Tst::iterator nit = names.begin();
	Dictionary::iterator dit = dictionary.begin();
	long count = 0;
	while(nit != names.end())
	{
		BOOST_CHECK(nit.key() == *dit);
		if (nit.key() != *dit) {
			std::cout << *dit << " != " << nit.key();
			while(*(++dit) != nit.key())
				std::cout << " : " << *dit;
			std::cout << '\n';
		}
		++nit;
		++dit;
		++count;
	}

	// backward iteration
	Tst::reverse_iterator rit = names.rbegin();
	dit = revdict.begin();
	count = 0;
	while(rit != names.rend())
	{
		Tst::iterator it = rit.base();
		--it;
		Tst::key_type key = it.key();
		BOOST_CHECK(key == *dit);
		if (key != *dit) {
			std::cout << *dit << " != " << key;
			while(*(++dit) != key)
				std::cout << " : " << *dit;
			std::cout << '\n';
		}
		++rit;
		++dit;
		++count;
	}
	return errors_counter() - result;
}

