#ifndef COPY_TEST_HPP_INCLUDED
#define COPY_TEST_HPP_INCLUDED

#include "tests_common.hpp"
#include "../structured_map.hpp"
#include <map>

inline char* strdup(const char* str) {
	char* result = new char[strlen(str) + 1];
	strcpy(result, str);
	return result;
}

struct fat_object
{
	fat_object(const char* str = "<null>")
		: name(strdup(str)), one(1), two(2), three(3)
	{}

	fat_object(const fat_object& fo) 
		: one(fo.one + .1), two(fo.two + .2), three(fo.three + .3)
		, name(strdup(fo.name))
	{
		//std::cout << "clone " << &fo << " => ";
		//print(std::cout);
	}

	~fat_object() {
		//std::cout << "delete ";
		//print(std::cout);
		delete[] name;
	}

	fat_object& operator=(const fat_object& other) {
		fat_object tmp(other);
		tmp.swap(*this);
		return *this;
	}

	void swap(fat_object& other) {
		std::swap(one, other.one); std::swap(two, other.two); std::swap(three, other.three);
		std::swap(name, other.name);
	}

	std::ostream& print(std::ostream& ostr) const {
		ostr << this << "." << (void*)name << "(" << name << ")\n";
		return ostr;
	}

	double one;
	char* name;
	double two, three;
};

std::ostream& operator<<(std::ostream& ostr, const fat_object& o) {
	o.print(ostr);
	return ostr;
}

// insert "a" = 1, "b" = 2 etc, out of order.
// test that they are ordered and have correct value.
// insert b, d, f by operator[]
// iterator used to check order of strings.
int copy_test()
{
	long result = errors_counter();

	typedef containers::structured_map<std::string, fat_object> Tst;
	//typedef std::map<std::string, fat_object> Tst;
	Tst tst;

	tst["green"] = fat_object("green");
	fat_object& ref =  tst["green"];
	BOOST_CHECK(strcmp(ref.name, "green") == 0);

	Tst t2(tst);
	fat_object& r2 =  t2["green"];
	BOOST_CHECK(strcmp(r2.name, "green") == 0);   // check copy construction

	t2["green"].name = strdup("blue");  // fat_object is not a well-designed class
	BOOST_CHECK(strcmp(r2.name, "blue") == 0);    // check change

	BOOST_CHECK(strcmp(ref.name, "green") == 0);  // check original value

	return errors_counter() - result;
}

#endif  // COPY_TEST_HPP_INCLUDED