// pure compilation/header test
// March 2007: Fixed to compile with GCC 3.4.2 (+ STLport-5 + Boost-1.33.1)

#if _MSC_VER
#pragma warning( disable: 4100 4127 4189 4239 4245 4510 4512 4610 4702 )
#endif

#include "ternary_tree.hpp"
#include "structured_map.hpp"
#include "structured_set.hpp"
#include <boost/concept_check.hpp>
#include <iterator>

using namespace boost;


// modelled on Boost concept checks
template <class StructuredAssociativeContainer>
struct StructuredAssociativeContainerConcept
{
    void constraints()
	{
		function_requires< AssociativeContainerConcept<StructuredAssociativeContainer> >();
		function_requires< ReversibleContainerConcept<StructuredAssociativeContainer> >();

		// Almost SortedAssociative: takes cc instead of kc arg to constructor
		StructuredAssociativeContainer 
		c(cc);
		// No special input method requirements
		/*StructuredAssociativeContainer 
		c2(first, last),
		c3(first, last, cc); */

		// Sorted searches
		p = c.upper_bound(k);
		p = c.lower_bound(k);
		r = c.equal_range(k);
		// Structured searches - non-const subset
		r = c.prefix_range(k);
		char_iterator b = k.begin(), e = k.end();
		p = c.longest_match(b, e);

		// const methods, but check they work anyway
		srl = c.create_search_results();
		std::back_insert_iterator<search_results_list> bit = std::back_inserter(srl);
		bit = c.partial_match_search(k, bit);
		bit = c.partial_match_search(k, bit, '?');
		bit = c.hamming_search(k, bit, s);
		bit = c.levenshtein_search(k, bit, s);
		bit = c.combinatorial_search(k, bit);
		bit = c.combinatorial_search(k, bit, 2);
		p = iterator(*srl.begin());
		cp = const_iterator(*srl.begin());
		p = iterator(srl[0]);
		cp = iterator(srl[0]);

		// no insertion specified - tst has operator[k] and insert(pair<k,t>),
		// while set/map insert value_type. Work on this?
		//c.insert(p, t);

		ignore_unused_variable_warning(c);
		const_constraints(c);
    }
    void const_constraints(const StructuredAssociativeContainer& c)
	{
		cc = c.char_comp();
		kc = c.key_comp();

		// Sorted searches
		cp = c.upper_bound(k);
		cp = c.lower_bound(k);
		cr = c.equal_range(k);
		// Structured searches
		cr = c.prefix_range(k);
		char_iterator b = k.begin(), e = k.end();
		cp = c.longest_match(b, e);

		srl = c.create_search_results();
		std::back_insert_iterator<search_results_list> bit = std::back_inserter(srl);
		bit = c.partial_match_search(k, bit);
		bit = c.partial_match_search(k, bit, '?');
		bit = c.hamming_search(k, bit, s);
		bit = c.levenshtein_search(k, bit, s);
		bit = c.combinatorial_search(k, bit);
		bit = c.combinatorial_search(k, bit, 2);
		p = iterator(*srl.begin());
		cp = iterator(*srl.begin());
		p = iterator(srl[0]);
		cp = const_iterator(srl[0]);

		ignore_unused_variable_warning(c);
	}
    typename StructuredAssociativeContainer::char_compare       cc;
    typename StructuredAssociativeContainer::key_compare        kc;
    typename StructuredAssociativeContainer::value_type         t;
    typename StructuredAssociativeContainer::key_type           k;
    typedef typename StructuredAssociativeContainer::iterator                  iterator;
    typedef typename StructuredAssociativeContainer::const_iterator            const_iterator;
	typedef typename StructuredAssociativeContainer::size_type                 size_type;
	// Forward container requirements on key type
	typedef typename StructuredAssociativeContainer::char_type                 char_type;
	typedef typename StructuredAssociativeContainer::search_results_list       search_results_list;
	// Req'd by implication; used with longest_match, and internally
    typedef typename StructuredAssociativeContainer::key_type::const_iterator  char_iterator;
	search_results_list srl;
	size_type s;
    iterator p;
    const_iterator cp;
    std::pair<iterator,iterator> r;
    std::pair<const_iterator,const_iterator> cr;
	// no special input method requirements
    //std::pair<typename StructuredAssociativeContainer::key_type,
	//		  typename StructuredAssociativeContainer::value_type> *first, *last;
};

// Simpler version of BOOST_REQUIRES: we just form a function pointer 
// to the concept<type_var>::constraints() method
#define CONCEPT_REQUIREMENTS(concept, type_var) \
	void (concept <type_var>::* func##type_var##concept)(); \
	func##type_var##concept = &concept<type_var>::constraints

// used with 
struct testdata { double d; int i; std::string s; };

int check_concepts()
{
	using namespace boost;

	typedef containers::structured_set<std::string> Sset;
	CONCEPT_REQUIREMENTS (StructuredAssociativeContainerConcept, Sset);
	CONCEPT_REQUIREMENTS (SimpleAssociativeContainerConcept, Sset);
	CONCEPT_REQUIREMENTS (UniqueAssociativeContainerConcept, Sset);

	typedef containers::structured_set<std::wstring> SWset;
	CONCEPT_REQUIREMENTS (StructuredAssociativeContainerConcept, SWset);
	CONCEPT_REQUIREMENTS (SimpleAssociativeContainerConcept, SWset);
	CONCEPT_REQUIREMENTS (UniqueAssociativeContainerConcept, SWset);

	typedef containers::structured_multiset<std::string> SMset;
	CONCEPT_REQUIREMENTS (StructuredAssociativeContainerConcept, SMset);
	CONCEPT_REQUIREMENTS (SimpleAssociativeContainerConcept, SMset);
	CONCEPT_REQUIREMENTS (MultipleAssociativeContainerConcept, SMset);

	typedef containers::structured_map<std::string, int> Smap;
	CONCEPT_REQUIREMENTS (StructuredAssociativeContainerConcept, Smap);
	CONCEPT_REQUIREMENTS (PairAssociativeContainerConcept, Smap);
	CONCEPT_REQUIREMENTS (UniqueAssociativeContainerConcept, Smap);

	typedef containers::structured_map<std::wstring, int> SWmap;
	CONCEPT_REQUIREMENTS (StructuredAssociativeContainerConcept, SWmap);
	CONCEPT_REQUIREMENTS (PairAssociativeContainerConcept, SWmap);
	CONCEPT_REQUIREMENTS (UniqueAssociativeContainerConcept, SWmap);

	typedef containers::structured_map<std::wstring, std::pair<double, double> > SWDmap;
	CONCEPT_REQUIREMENTS (StructuredAssociativeContainerConcept, SWDmap);
	CONCEPT_REQUIREMENTS (PairAssociativeContainerConcept, SWDmap);
	CONCEPT_REQUIREMENTS (UniqueAssociativeContainerConcept, SWDmap);

	typedef containers::structured_multimap<std::string, int> SMmap;
	CONCEPT_REQUIREMENTS (StructuredAssociativeContainerConcept, SMmap);
	CONCEPT_REQUIREMENTS (PairAssociativeContainerConcept, SMmap);
	CONCEPT_REQUIREMENTS (MultipleAssociativeContainerConcept, SMmap);

	typedef containers::ternary_tree<std::string, int> Tst;
	typedef containers::ternary_tree<std::string, testdata> Tst2;
	CONCEPT_REQUIREMENTS (StructuredAssociativeContainerConcept, Tst);
	CONCEPT_REQUIREMENTS (StructuredAssociativeContainerConcept, Tst2);
	// no longer holds:
	//CONCEPT_REQUIREMENTS (PairAssociativeContainerConcept, Tst);
	//CONCEPT_REQUIREMENTS (UniqueAssociativeContainerConcept, Tst);

	return 0;
}
