#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

typedef std::vector<std::string> Dictionary;

//template<class Container>
size_t fill_dictionary(const char* filename, Dictionary& dictionary, size_t maxsize, size_t line_length = 0)
{
	std::ifstream input(filename);
	size_t longest_in_file = 0;
	size_t linecount = 0;
	if (!input.is_open()) {
		std::cerr << filename << ": file open fail\n";
		throw std::runtime_error("fill_dictionary failed");
	}
	if (input.is_open() && !line_length)
	{
		std::vector<char> next;
		next.resize(std::max(line_length, size_t(300)));
		while(input.good() && linecount < maxsize) {
			input.getline(&next[0], next.capacity());
			std::string s(next.begin(), next.begin() + (size_t)input.gcount());
			if (!s.empty()) {
				dictionary.push_back(s.c_str());
				++linecount;
				if (s.size() > longest_in_file)
					longest_in_file = s.size();
				//std::cerr << s.c_str() << "\n";
			}
			//next.clear();
		}
		//std::cerr << "Read " << dictionary.size() << " lines from wordlist.txt\n";
	}

	// If file not long enough, fill up with some random alphabetic strings 
	if (line_length || (dictionary.size() < maxsize && (maxsize < size_t(-1)) ) )
	{
		std::string next;
		if (!line_length) {
			line_length = linecount? longest_in_file : 10;
			std::cerr << "zero-length line, we'll have trouble";
		}
		next.reserve(line_length + 1);
		for (size_t i = dictionary.size(); i < maxsize; ++i)
		{
			size_t length = 1 + (rand() % line_length);
			next.resize(length--);
			while(length--) {
				next[length] = rand() % (127-' ') + ' ';
			}
			//std::cerr << next << '\n';
			dictionary.push_back(next.c_str());
		}
		if (line_length > longest_in_file)
			longest_in_file = line_length;
	}
	return longest_in_file;
}




