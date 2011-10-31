
#include "tests_common.hpp"
#include <boost/shared_ptr.hpp>
#include <gui/graphics/shared_gdi_ptr.hpp>

struct TrackedInstance
{
	TrackedInstance(int i = -1) : m_int(i) {
		instances().insert(this);
	}
	TrackedInstance(const TrackedInstance& other) : m_int(other.m_int) {
		instances().insert(this);
	}
	~TrackedInstance() {
		//std::cout << " ~" << m_int;
		size_t count = instances().size();
		instances().erase(this);
		size_t count2 = instances().size();
		assert(count2 < count);
	}
	TrackedInstance& operator=(const TrackedInstance& other) {
		m_int = other.m_int; 
		return *this;
	}
	int m_int, dummy;
	typedef std::set<void*>  List;
	static List& instances() { static List m; return m; }
};


int mapped_value_test()
{
	long result = errors_counter();
	typedef containers::ternary_tree<std::string, double> Tst;
	Tst names;
	names["44"] = 44.44;
	names["55"] = 55.55;
	names["0"] += 1;
	names["99"] = names["44"] + names["55"];
	BOOST_CHECK(names["nothing"] == 0);
	BOOST_CHECK(names["0"] == 1.0);
	BOOST_CHECK(names["44"] == 44.44);
	BOOST_CHECK(names["55"] == 55.55);
	BOOST_CHECK(names["99"] == 99.99);
	names.erase("0");

	// Check that destructors are called with shared_ptr's
	{
		const int COUNT = 10;
		typedef boost::shared_ptr<TrackedInstance> TI_ptr;
		typedef containers::ternary_tree<std::string, TI_ptr> Tstp;
		Tstp pointers;
		size_t tic = TrackedInstance::instances().size();
		for (int i = 0; i != COUNT; ++i) {
			tic = TrackedInstance::instances().size();
			pointers[to_string(i)] = TI_ptr(new TrackedInstance(i+1));
		}
		tic = TrackedInstance::instances().size();
		BOOST_CHECK(tic == COUNT);
		// call erase on half
		for (int i = 0; i != COUNT; i += 2) {
			pointers.erase(to_string(i));
		}
		tic = TrackedInstance::instances().size();
		BOOST_CHECK(tic == COUNT/2);
	}
	BOOST_CHECK(TrackedInstance::instances().size() == 0);

	// Check that destructors are called with direct (fat) values
	{
		const int COUNT = 10;
		typedef containers::ternary_tree<std::string, TrackedInstance> Tstti;
		Tstti values;
		size_t tic = TrackedInstance::instances().size();
		for (int i = 0; i != COUNT; ++i) {
			tic = TrackedInstance::instances().size();
			values[to_string(i)] = TrackedInstance(i+1);
		}
		tic = TrackedInstance::instances().size();
		BOOST_CHECK(tic == COUNT);
		// call erase on half
		for (int i = 0; i != COUNT; i += 2) {
			values.erase(to_string(i));
		}
		tic = TrackedInstance::instances().size();
		BOOST_CHECK(tic == COUNT/2);
	}
	BOOST_CHECK(TrackedInstance::instances().size() == 0);

	return errors_counter() - result;
}
