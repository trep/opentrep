// Created Mon Feb 06 13:20:01 2006
#ifndef ITERATOR_WRAPPER_HPP_INCLUDED
#define ITERATOR_WRAPPER_HPP_INCLUDED

#include <iterator>

namespace iterators {

	// This is mostly a lame ripoff from Boost.Iterator, to avoid the dependency...

	//! Standard type traits for const_iterators. \see iterator_wrapper
	template <class T>
	struct const_traits {
		typedef T         value_type;
		typedef const T*  pointer;
		typedef const T*  const_pointer;
		typedef const T&  reference;
		typedef const T&  const_reference;
	};

	//! Standard type traits for (non-const) iterators. \see iterator_wrapper
	template <class T>
	struct nonconst_traits {
		typedef T         value_type;
		typedef T*        pointer;
		typedef const T*  const_pointer;
		typedef T&        reference;
		typedef const T&  const_reference;
	};

	/** Creates a bidirectional iterator from a base implementation, 
	 * which is required to supply the interface \code
	 * struct iter_impl_sample
	 * {
	 *    typedef /impl-defined/ reference;
	 *    iter_impl_sample();
	 *    iter_impl_sample(/args/);
	 *    void increment();
	 *    void decrement();
	 *    reference dereference() const;
	 *    template<class OtherIter> bool equal(const OtherIter& rhs);
	 *    void swap(this_type& rhs);
	 * }; \endcode
	 * (This class is meant for iterators you control - if you need to adapt an existing iterator 
	 * with different interface, something like boost::iterator_facade is needed.)
	 *
	 * The first template parameter is the iterator implementation class. 
	 * iterator_wrapper does not inherit from this. The second parameter is either const_traits &lt;T&gt;
	 * or nonconst_traits &lt;T&gt;, which provide the basic value_type related definitions.
	 *
	 * Note that Boost.Iterator will do the same job better, this was provided to avoid the dependency.
	 * Future versions may move to Boost instead.
	 *
	 * \ingroup utilities
	 */
	template< class BaseIterT
			, class TraitsT
			, class IterCatT = std::bidirectional_iterator_tag
			> 
	struct iterator_wrapper
	{
		typedef BaseIterT                                       base_iter;
		typedef TraitsT                                         traits_type;
		typedef iterator_wrapper<BaseIterT, TraitsT, IterCatT>  this_type;

		typedef typename TraitsT::value_type                    value_type;
		typedef typename TraitsT::pointer                       pointer;
		typedef typename TraitsT::reference                     reference;
		typedef typename TraitsT::const_reference               const_reference;

		typedef IterCatT                                        iterator_category;
		typedef ptrdiff_t                                       difference_type;
		typedef size_t                                          size_type;

		iterator_wrapper() {}

		//! Copy constructor for iterator and constructor from (non-const) iterator for const_iterator
		template<class SameBase>
		iterator_wrapper(const iterator_wrapper<SameBase, nonconst_traits<value_type>, IterCatT>& it)
			: m_iter(it.iter_base())
		{}

		iterator_wrapper(const iterator_wrapper& it) : m_iter(it.iter_base()) {}

		iterator_wrapper(const base_iter& it) : m_iter(it) {}

		reference operator*() const { return m_iter.dereference(); }

		pointer operator->() const { return &m_iter.dereference(); }

		this_type& operator++() { m_iter.increment(); return *this; }

		this_type operator++(int) {
			this_type tmp(*this);
			this->operator++();
			return tmp;
		}

		this_type& operator--() { m_iter.decrement(); return *this; }

		this_type operator--(int) {
			this_type tmp(*this);
			this->operator--();
			return tmp;
		}

		//! Assignment from non-const to const_iterator
		template<class SameBase>
		this_type& operator=(const iterator_wrapper<SameBase, nonconst_traits<value_type> >& rhs) {
			this_type(rhs).swap(*this);
			return *this;
		}

		this_type& operator=(const iterator_wrapper<BaseIterT, TraitsT>& rhs) {
			this_type(rhs).swap(*this);
			return *this;
		}

		template<class Base, class Constness>
		void swap(iterator_wrapper<Base, Constness>& other) {
			iter_base().swap(other.iter_base());
		}

		base_iter& iter_base() { return m_iter; }
		const base_iter& iter_base() const { return m_iter; }

	private:
        base_iter  m_iter;
	};

	// \relates iterator_wrapper
	template<class Base, class Val, class Val2, class Cat>
	bool operator== (const iterator_wrapper<Base, Val, Cat>& lhs, const iterator_wrapper<Base, Val2, Cat>& rhs) {
		return lhs.iter_base().equal(rhs.iter_base());
	}

	// \relates iterator_wrapper
	template<class Base, class Val, class Val2, class Cat>
	bool operator!= (const iterator_wrapper<Base, Val, Cat>& lhs, const iterator_wrapper<Base, Val2, Cat>& rhs) {
		return ! (lhs == rhs);
	}

//! \def provide equality operator for reverse const/nonconst iterators  \relates iterator_wrapper 
#define INTEROPERABLE_REVERSE_ITERATOR_WRAPPERS(ConstTraits, NonConstTraits)                                \
	template<class Base, class Val, class Cat>                                                              \
	bool operator== (const std::reverse_iterator<iterator_wrapper<Base, ConstTraits<Val>, Cat> >& lhs,      \
					 const std::reverse_iterator<iterator_wrapper<Base, NonConstTraits<Val>, Cat> >& rhs) { \
		return lhs.base() == rhs.base();                                                                    \
	}                                                                                                       \
	template<class Base, class Val, class Cat>                                                              \
	bool operator!= (const std::reverse_iterator<iterator_wrapper<Base, NonConstTraits<Val>, Cat> >& lhs,   \
					 const std::reverse_iterator<iterator_wrapper<Base, ConstTraits<Val>, Cat> >& rhs) {    \
		return !(lhs.base() == rhs.base());                                                                 \
	} 

INTEROPERABLE_REVERSE_ITERATOR_WRAPPERS(const_traits, nonconst_traits)
INTEROPERABLE_REVERSE_ITERATOR_WRAPPERS(nonconst_traits, const_traits)

#undef INTEROPERABLE_REVERSE_ITERATOR_WRAPPERS


/** \relates iterator_wrapper
 * @{
 */
template<class Base, class Val, class Val2, class Cat>
bool operator< (const iterator_wrapper<Base, Val, Cat>& lhs, const iterator_wrapper<Base, Val2, Cat>& rhs) {
	return lhs.iter_base().less(rhs.iter_base());
}


template<class Base, class Val, class Val2, class Cat>
bool operator> (const iterator_wrapper<Base, Val, Cat>& lhs, const iterator_wrapper<Base, Val2, Cat>& rhs) {
	return rhs < lhs;
}


template<class Base, class Val, class Val2, class Cat>
bool operator>= (const iterator_wrapper<Base, Val, Cat>& lhs, const iterator_wrapper<Base, Val2, Cat>& rhs) {
	return ! (lhs < rhs);
}

template<class Base, class Val, class Val2, class Cat>
bool operator<= (const iterator_wrapper<Base, Val, Cat>& lhs, const iterator_wrapper<Base, Val2, Cat>& rhs) {
	return ! (rhs > lhs);
}

// random access iter operations (+= -= etc)

template<class Base, class Val, class Dist>
iterator_wrapper<Base, Val, std::random_access_iterator_tag>&
operator+= (iterator_wrapper<Base, Val, std::random_access_iterator_tag>& it, Dist n) {
	it.iter_base().advance(n);
	return it;
}

template<class Base, class Val, class Dist>
iterator_wrapper<Base, Val, std::random_access_iterator_tag>&
operator-= (iterator_wrapper<Base, Val, std::random_access_iterator_tag>& it, Dist n) {
	it.iter_base().advance(-n);
	return it;
}

template<class Base, class Val, class Dist>
iterator_wrapper<Base, Val, std::random_access_iterator_tag>
operator+ (const iterator_wrapper<Base, Val, std::random_access_iterator_tag>& it, Dist n) {
	iterator_wrapper<Base, Val, std::random_access_iterator_tag> tmp(it);
	tmp += n;
	return tmp;
}

template<class Base, class Val, class Dist>
iterator_wrapper<Base, Val, std::random_access_iterator_tag>
operator- (const iterator_wrapper<Base, Val, std::random_access_iterator_tag>& it, Dist n) {
	iterator_wrapper<Base, Val, std::random_access_iterator_tag> tmp(it);
	tmp -= n;
	return tmp;
}

template<class Base, class Val, class Dist>
Dist
operator- (const iterator_wrapper<Base, Val, std::random_access_iterator_tag>& lhs, 
		   const iterator_wrapper<Base, Val, std::random_access_iterator_tag>& rhs) {
	return lhs - rhs;
}


/*@}*/

}  // namespace iterators



#endif  // ITERATOR_WRAPPER_HPP_INCLUDED
