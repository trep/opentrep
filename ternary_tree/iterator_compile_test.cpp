/** Pure compilation/header test
 *  \file
 * This file checks interoperability requirements for iterator_wrapper.hpp
 * The problem cases are those that should fail: they cannot be checked
 *   automatically by compiler.
 * So to use, you must define TEST_COMPILATION_FAILURE or 
 *   CHECK_SPECIAL_COMP_FAILURE below, and then inspect compiler warnings 
 *   to see that you get an error for every SHOULD_FAIL line (#1-F, #20-21).
 * To simplify this, uncomment one statement at a time in the function
 *   iterator_interop_checks_main() 
 *   at the end of this file, then try to compile.
 *
 * Construction/assignment to reverse_iterator from const_reverse_iterator
 * generates more complicated error messages from compilers, 
 * so were broken out to allow separate runs.
 * Define the macro CHECK_SPECIAL_COMP_FAILURE and look for 
 * SPECIAL_FAIL_1 and _2 in the compiler output.
 *
 * This (un)works as required with MSVC and Comeau online tryitout compiler.
 */

#ifdef _MSC_VER
#  pragma warning(disable: 4245 4127 4189 4700)
#endif


//#define TEST_COMPILATION_FAILURE
// Two cases must be inspected in references on MSVC
//#define CHECK_SPECIAL_COMP_FAILURE

#ifdef TEST_COMPILATION_FAILURE 
#  define SHOULD_FAIL( Pred ) Pred
#else
#  define SHOULD_FAIL( Pred ) 
#endif

#ifdef CHECK_SPECIAL_COMP_FAILURE
#  define SPECIAL_FAIL_1( Pred ) Pred
#  define SPECIAL_FAIL_2( Pred ) Pred
#else
#  define SPECIAL_FAIL_1( Pred ) 
#  define SPECIAL_FAIL_2( Pred ) 
#endif



template<class Container>
void iterator_interop_checks()
{
	typedef Container C;
	typedef typename C::const_iterator         c_t;
	typedef typename C::iterator               i_t;
	typedef typename C::const_reverse_iterator cr_t;
	typedef typename C::reverse_iterator       r_t;
	c_t  c;
	i_t  i;
	cr_t cr;
	r_t  r;
///// COPY-CONSTRUCTORS
	// Should work
	c_t i1( i );
	c_t i2( cr.base() );
	c_t i3( r.base() );
	i_t i4( r.base() );
	cr_t i5( r );
	cr_t i6( c );
	cr_t i7( i );
	r_t i8( i );

	SHOULD_FAIL( c_t i101( cr ); )  // #1
	SHOULD_FAIL( c_t i102( r );  )  // #2
	SHOULD_FAIL( i_t i103( c );  )  // #3
	SHOULD_FAIL( i_t i104( cr ); )  // #4
	SHOULD_FAIL( i_t i105( r );  )  // #5
	SHOULD_FAIL( r_t i106( c );  )  // #6

///// ASSIGNMENT 
	// Should work
	c = i;
	c = cr.base();
	c = r.base();
	i = r.base();
	cr = r;

	SHOULD_FAIL( c = cr; )  // #7
	SHOULD_FAIL( c = r;	 )  // #8
	SHOULD_FAIL( i = c;	 )  // #9
	SHOULD_FAIL( i = cr; )  // #A
	SHOULD_FAIL( i = r;	 )  // #B
	SHOULD_FAIL( cr = c; )  // #C
	SHOULD_FAIL( cr = i; )  // #D
	SHOULD_FAIL( r = c;	 )  // #E
	SHOULD_FAIL( r = i;	 )  // #F

// these fail in 2nd pass or something, compile separately
	SPECIAL_FAIL_1( r_t i107( cr ); )  // #10
	SPECIAL_FAIL_2( r = cr; )          // #11

///// ADVANCE
	++c;   --c;
	++i;   --i;
	++cr;  --cr;
	++r;   --r;
	c++;   c--;
	i++;   i--;
	cr++;  cr--;
	r++;   r--;

///// COMPARE
	if (c == i && i == c)  c = c;
	if (c == cr.base() && cr.base() == c)  c = c;
	if (i == r.base() && r.base() == i)  c = c;
	// Should this fail? - Dinkum nor StlPort don't seem to prevent it
	//if (r == cr && cr == r)  c = c;
	if (r.base() == cr.base() && cr.base() == r.base())  c = c;

///// DEREFERENCE
	typedef typename C::value_type       val_t;
	typedef typename C::pointer          ptr_t;
	typedef typename C::reference        ref_t;
	typedef typename C::const_reference  cref_t;

	val_t val1 = *c;
	val_t val2 = *i;
	val_t val3 = *cr;
	val_t val4 = *r;
	
	ref_t ref2 = *i;
	ref_t ref4 = *r;
	
	cref_t cref1 = *c;
	cref_t cref2 = *i;
	cref_t cref3 = *cr;
	cref_t cref4 = *r;

	SHOULD_FAIL( ref_t ref1 = *c );   // #20
	SHOULD_FAIL( ref_t ref3 = *cr );  // #21

}

#include <vector>
#include "ternary_tree.hpp"
#include "structured_set.hpp"
#include "structured_map.hpp"

void iterator_interop_checks_main() 
{
	typedef std::vector<int> Cont;
//	iterator_interop_checks<Cont>();
/*	typedef containers::ternary_tree<std::string, int> Tst;
	iterator_interop_checks<Tst>();
	typedef containers::structured_set<std::string> StrucSet;
	iterator_interop_checks<StrucSet>();
*/	typedef containers::structured_multiset<std::string> MStrucSet;
	iterator_interop_checks<MStrucSet>();
/*	typedef containers::structured_map<std::string, int> StrucMap;
	iterator_interop_checks<StrucMap>();
	typedef containers::structured_multimap<std::string, int> MStrucMap;
	iterator_interop_checks<MStrucMap>(); */
}



