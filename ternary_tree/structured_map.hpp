#ifndef STRUCTURED_MAP_HPP_INCLUDED
#define STRUCTURED_MAP_HPP_INCLUDED

#define TST_NO_STANDALONE_ITERATOR_FACADE
#include "ternary_tree.hpp"
#undef TST_NO_STANDALONE_ITERATOR_FACADE
// note: also #include <list> in structured_multimap section

namespace containers {


/** Structured Map is a Sorted Associative Container that stores objects of type pair<Key, Data>.
 * Structured Map is a Structured Container, meaning that its key type is required to be 
 * a Forward Container, and that the map uses a comparator to establish 
 * a strict weak ordering among key::value_type elements (rather than on whole keys).
 * This allows searches in the set involving parts of keys, ie with shared prefix 
 * or with shared middle parts. 
 *
 * Structured Map is a Pair Associative Container, meaning that its value type 
 * is pair<const Key, T>. 
 * It is also a Unique Associative Container, meaning that no two elements are the same.
 *
 * A std::map is normally backed by a binary tree. A structured map is instead backed
 * by a ternary_tree, which manages structured ordering of keys. 
 * For string-like keys, a ternary tree is typically as fast as an unordered_map, 
 * and several times faster than most std::map implementations.
 * \ingroup containers
 */
template<class KeyT, class DataT, class CompT = std::less<typename KeyT::value_type>, 
		 class AllocT = std::allocator<std::pair<const KeyT, DataT> > >
class structured_map
{
public:
	typedef KeyT                                        key_type;
	typedef DataT                                       mapped_type;
	typedef std::pair<const KeyT, DataT>                value_type;
	typedef typename KeyT::value_type                   char_type;
	typedef CompT                                       char_compare;

	typedef AllocT                                      allocator_type;
	typedef typename AllocT::difference_type            difference_type;
	typedef typename AllocT::size_type                  size_type;
	typedef typename AllocT::pointer                    pointer;
	typedef typename AllocT::const_pointer              const_pointer;
	typedef typename AllocT::reference                  reference;
	typedef typename AllocT::const_reference            const_reference;

private:
	// Internal value type is pair<Key, Value> (non-const Key).
	typedef ternary_tree< KeyT, value_type, CompT, 
						  typename AllocT::template rebind<value_type>::other
						>                               ternary_tree;
	typedef typename ternary_tree::iterator             tst_iterator;
	typedef typename ternary_tree::iterator::base_iter  tst_iterator_base;
	typedef typename ternary_tree::const_iterator       tst_const_iterator;

	enum { invalid_index = size_type(-1) };

public:

	typedef typename ternary_tree::key_compare          key_compare;


	typedef iterators::iterator_wrapper < tst_iterator_base
										, iterators::nonconst_traits<value_type> 
										>               iterator;

	typedef iterators::iterator_wrapper < tst_iterator_base
										, iterators::const_traits<value_type> 
										>               const_iterator;

	typedef std::reverse_iterator<iterator>             reverse_iterator;
	typedef std::reverse_iterator<const_iterator>       const_reverse_iterator;


	/** \name Construct, copy, destroy
	 * @{
	 */
	structured_map() : m_tree(char_compare(), allocator_type()) {}

	explicit structured_map(const char_compare& comp)
		: m_tree(comp, allocator_type())
	{}

	structured_map(const char_compare& comp, const allocator_type& alloc)
		: m_tree(comp, alloc)
	{}

	template<class InputIterator>
	structured_map( InputIterator first, InputIterator last,
					const char_compare& comp = char_compare(), 
					const allocator_type& alloc = allocator_type())
		: m_tree(comp, alloc)
	{
		insert(first, last);
	}

	structured_map(const structured_map& other)
		: m_tree(other.m_tree)
	{}

	~structured_map() {}

	structured_map& operator= (const structured_map& other) {
		structured_map(other).swap(*this);
		return *this;
	}

	allocator_type get_allocator() const { return m_tree.get_allocator(); }
	/* @} */

	/** \name Iterators
	 * Includes C++0x methods cbegin, cend, crbegin, crend to make it easier 
	 * to access const iterators.
	 * @{
	 */
	iterator                begin()         { return iterator(m_tree.begin()); }
	const_iterator          begin()   const { return const_iterator(m_tree.begin()); }
	iterator                end()           { return iterator(m_tree.end()); }
	const_iterator          end()     const { return const_iterator(m_tree.end()); }

	reverse_iterator        rbegin()        { return reverse_iterator(end()); }
	const_reverse_iterator  rbegin()  const { return const_reverse_iterator(end()); }  
	reverse_iterator        rend()          { return reverse_iterator(begin()); }
	const_reverse_iterator  rend()    const { return const_reverse_iterator(begin()); }

	// C++0x additions
	const_iterator          cbegin()  const { return const_iterator(m_tree.begin()); }
	const_iterator          cend()    const { return const_iterator(m_tree.end()); }
	const_reverse_iterator  crbegin() const { return const_reverse_iterator(end()); }  
	const_reverse_iterator  crend()   const { return const_reverse_iterator(begin()); }
	/* @} */

	/** \name Capacity
	 * @{
	 */
	bool empty() const { return m_tree.empty(); }
	size_type size() const { return m_tree.size(); }
	size_type max_size() const { return m_tree.max_size(); }
	/* @} */

	/** \name Element access
	 * @{
	 */
	mapped_type& operator[](const key_type& key)
	{
		value_type& ref = m_tree[key];
		if (ref.first.empty())
			const_cast<key_type&>(ref.first) = key;
		return ref.second;
	}
	/* @} */

	/** \name Modifiers
	 * @{
	 */
	std::pair<iterator, bool> insert(const value_type& val)
	{
		std::pair<tst_iterator, bool> result = 
				m_tree.insert( std::make_pair(val.first, val) );
		return std::make_pair(iterator(result.first), result.second);
	}

	iterator insert(iterator where, const value_type& val) { return insert(val).first; }

	template<class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		while(first != last)
			insert(*first++);
	}

	// returns iterator as per C++0x
	iterator erase(iterator pos) { return m_tree.erase(pos); }

	size_type erase(const key_type& key) { return m_tree.erase(key); }

	// returns iterator as per C++0x
	iterator erase(iterator first, iterator last) { return m_tree.erase(first, last); }

	void swap(structured_map& other) {
		m_tree.swap(other.m_tree);
	}

	void clear() { m_tree.clear(); }
	/* @} */

	/** @name Container comparison.
	 * @{
	 */
	bool operator==(const structured_map& rhs) const { return m_tree == rhs.m_tree; }
	bool operator< (const structured_map& rhs) const { return m_tree < rhs.m_tree; }
	/* @} */

	/** \name Observers
	 * @{
	 */
	//! Returns a copy of the key element comparator object.
	char_compare  char_comp() const { return m_tree.char_comp(); }

	/** Returns a copy of the key_compare class; this is defined in terms of a 
	 * lexicographical_compare using the char_compare object. */
	key_compare   key_comp() const { return m_tree.key_comp(); }

	//! Value comparator, applying the key_compare to the key part of value_type.
	class value_compare : public std::binary_function<value_type, value_type, bool> 
	{
	 public:
		bool operator()(const value_type& a, const value_type& b) const {
			return comp(a.first, b.first); 
		}
	 protected:
		//friend class ternary_tree;
		value_compare(key_compare c) : comp(c) {}
		key_compare comp;
	};

	value_compare value_comp() const { return value_compare(key_comp()); }
	/* @} */

	/** \name Map operations
	 * @{
	 */
	iterator       find(const key_type& key)       { return m_tree.find(key); }
	const_iterator find(const key_type& key) const { return m_tree.find(key); }
	size_type count(const key_type& key) const { return m_tree.count(key); }
	
	iterator       lower_bound(const key_type& key)       { return m_tree.lower_bound(key); }
	iterator       upper_bound(const key_type& key)       { return m_tree.upper_bound(key); }
	const_iterator lower_bound(const key_type& key) const { return m_tree.lower_bound(key); }
	const_iterator upper_bound(const key_type& key) const { return m_tree.upper_bound(key); }

	std::pair<iterator, iterator>
	equal_range(const key_type& key)
	{
		std::pair<tst_iterator, tst_iterator> result(m_tree.equal_range(key));
		return std::make_pair(iterator(result.first), iterator(result.second));
	}

	std::pair<const_iterator, const_iterator>
	equal_range(const key_type& key) const {
		std::pair<tst_const_iterator, tst_const_iterator>
			result(m_tree.equal_range(key));
		return std::make_pair(const_iterator(result.first), const_iterator(result.second));
	}
	/* @} */

	//! Default for partial_match_search ('?')
	enum constants { wildcard_char = ternary_tree::wildcard_char };

	//! Results from partial_match_search and hamming_search
	typedef search_results_list<ternary_tree, iterator>  search_results_list;

	/** \name Structured search operations [deprecated]
	 * The interface for structure searches remains to be defined.
	 * See \ref tst_structsearch "ternary_tree Structured Search section"
	 * @{
	 */
	//! See ternary_tree::prefix_range
	std::pair<iterator, iterator>
	prefix_range(const key_type& key) {
		std::pair<tst_iterator, tst_iterator> result(m_tree.prefix_range(key));
		return std::make_pair(iterator(result.first), iterator(result.second));
	}

	std::pair<const_iterator, const_iterator>
	prefix_range(const key_type& key) const
	{
		std::pair<tst_const_iterator, tst_const_iterator> result(m_tree.prefix_range(key));
		return std::make_pair(const_iterator(result.first), const_iterator(result.second));
	}

	//! See ternary_tree::longest_match
	template<class CharIter>
	iterator longest_match(CharIter & first, const CharIter last) {
		return iterator(m_tree.longest_match(first, last));
	}

	template<class CharIter>
	const_iterator longest_match(CharIter & first, CharIter last) const {
		return const_iterator(m_tree.longest_match(first, last));
	}

	//! Required for partial_match_search and hamming_search calls.
	search_results_list create_search_results() const {
		return m_tree.create_search_results();
	}

	//! See ternary_tree::partial_match_search
	template<class OutputIter>
	OutputIter partial_match_search(const key_type& key, OutputIter results, 
									char_type wildcard = wildcard_char) const
	{
		return m_tree.partial_match_search(key, results, wildcard);
	}

	//! See ternary_tree::hamming_search
	template<class OutputIter>
	OutputIter hamming_search(const key_type& key, OutputIter results, size_type dist) const
	{
		return m_tree.hamming_search(key, results, dist);
	}

	//! See ternary_tree::levenshtein_search
	template<class OutputIter>
	OutputIter levenshtein_search(const key_type& key, OutputIter results, size_type dist) const
	{
		return m_tree.levenshtein_search(key, results, dist);
	}

	//! See ternary_tree::combinatorial_search (not part of Structured container interface)
	template<class OutputIter>
	OutputIter combinatorial_search(const key_type& key, OutputIter results, 
									size_type wildcards = 0) const
	{
		return m_tree.combinatorial_search(key, results, wildcards);
	}
	/* @} */

private:
	ternary_tree m_tree;
};



//! \relates structured_map
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator==(const structured_map<KeyT, DataT, CompT, AllocT>& x,
				const structured_map<KeyT, DataT, CompT, AllocT>& y) { return x.operator==(y); }
//! \relates structured_map
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator!=(const structured_map<KeyT, DataT, CompT, AllocT>& x,
				const structured_map<KeyT, DataT, CompT, AllocT>& y) { return !(x == y); }
//! \relates structured_map
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator< (const structured_map<KeyT, DataT, CompT, AllocT>& x,
				const structured_map<KeyT, DataT, CompT, AllocT>& y) { return x.operator<(y); }
//! \relates structured_map
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator> (const structured_map<KeyT, DataT, CompT, AllocT>& x,
				const structured_map<KeyT, DataT, CompT, AllocT>& y) { return y < x; }
//! \relates structured_map
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator<=(const structured_map<KeyT, DataT, CompT, AllocT>& x,
				const structured_map<KeyT, DataT, CompT, AllocT>& y) { return !(y > x); }
//! \relates structured_map
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator>=(const structured_map<KeyT, DataT, CompT, AllocT>& x,
				const structured_map<KeyT, DataT, CompT, AllocT>& y) { return !(x < y); }

//! \relates structured_map
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator==(typename structured_map<KeyT, DataT, CompT, AllocT>::const_iterator& a, 
				typename structured_map<KeyT, DataT, CompT, AllocT>::const_iterator& b)
{
	return a.operator==(b);
}

//! \relates structured_map
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator!=(typename structured_map<KeyT, DataT, CompT, AllocT>::const_iterator& a, 
				typename structured_map<KeyT, DataT, CompT, AllocT>::const_iterator& b)
{
	return !(a == b);
}

}  // namespace containers


// specialized algorithm
namespace std {

	//! \relates structured_map
	template<class KeyT, class DataT, class CompT, class AllocT>
	inline void swap(containers::structured_map<KeyT, DataT, CompT, AllocT> a, 
					 containers::structured_map<KeyT, DataT, CompT, AllocT> b)
	{
		a.swap(b);
	}
}


//----------------------------------------------------------------------------------------
// structured_multimap
//----------------------------------------------------------------------------------------

#include <list>

namespace containers {

namespace smap_detail {

	template<class TstIterT> 
	struct multimap_iterator
	{
	 private:
		typedef TstIterT                                             tst_iterator;
		typedef typename TstIterT::base_iter                         tst_iterator_base;
		typedef multimap_iterator<TstIterT>                          this_type;

		typedef typename TstIterT::base_iter::value_type::iterator   list_iterator;
		//! std::list stores pair<non-const Key, Val>
		typedef typename list_iterator::value_type                   internal_value_type;
		typedef typename internal_value_type::first_type             key_type;
		typedef typename internal_value_type::second_type            mapped_type;

	public:
		typedef std::pair<const key_type, mapped_type>               value_type;
		typedef value_type&                                          reference;

		multimap_iterator() {}

		multimap_iterator(const multimap_iterator& it)
			: m_iter(it.iter_base())
			, m_current(it.m_current)
		{}

		multimap_iterator(const iterators::iterator_wrapper<tst_iterator_base,
								iterators::const_traits<typename TstIterT::base_iter::value_type> >& it)
			: m_iter(it.iter_base())
		{
			set_current();
		}

		multimap_iterator(const tst_iterator& it)
			: m_iter(it.iter_base())
		{
			set_current();
		}

		multimap_iterator(const tst_iterator_base& it)
			: m_iter(it)
		{
			set_current();
		}

		reference dereference() const
		{
			internal_value_type * p = const_cast<internal_value_type*>(&(*m_current));
			// There's no nice way of casting pair<A, B> to pair<const A, B>
			return *reinterpret_cast<value_type*>(p);
		}

		void increment() {
			list_iterator end = (*m_iter).end();
			if (m_current == end || ++m_current == end) {
				++m_iter;
				set_current();
			}
		}

		void decrement()
		{
			// at rbegin
			if (m_iter == tst_iterator()) {
				--m_iter;
				set_current(false);
				return;
			}
			if (m_current-- == (*m_iter).begin()) {
				--m_iter;
				set_current(false);
			}
		}

		void swap(this_type& other) {
			std::swap(m_iter, other.m_iter);
			std::swap(m_current, other.m_current);
		}

		bool equal(const this_type& other) const {
			return m_iter == other.iter_base() && m_current == other.current();
		}

		tst_iterator&        iter_base()       { return m_iter; }
		const tst_iterator&  iter_base() const { return m_iter; }

		list_iterator&       current()       { return m_current; }
		const list_iterator& current() const { return m_current; }

	private:
		//! Called after m_key_values is changed, to reinit m_current
		void set_current(bool forward = true)
		{
			// end-iterator... should only decrement rend?
			if (m_iter.iter_base().empty() || m_iter == tst_iterator())
				m_current = list_iterator();
			else 
				m_current = forward? (*m_iter).begin() : --(*m_iter).end();
		}

	private:
		tst_iterator    m_iter;
		list_iterator   m_current;
	};


}  //  namespace structured_map_detail


/** Structured Multimap is a Sorted Associative Container that stores objects of type pair<Key, Data>.
 * Structured Multimap is a Structured Container, meaning that its key type is required to be 
 * a Forward Container, and that the map uses a comparator to establish 
 * a strict weak ordering among key::value_type elements (rather than on whole keys).
 * This allows searches in the set involving parts of keys, ie with shared prefix 
 * or with shared middle parts. 
 *
 * Structured Multimap is a Pair Associative Container, meaning that its value type 
 * is pair<const Key, T>. 
 * It is also a Multiple Associative Container, meaning that more than one elements 
 * can be stored under the same key.
 *
 * A std::multimap is normally backed by a binary tree. A structured multimap is instead 
 * backed by a ternary_tree, which manages structured ordering of keys.
 * For string-like keys, a ternary tree is typically as fast as an unordered_multimap, 
 * and several times faster than most std::multimap implementations.
 * \ingroup containers
 */
template< class KeyT
		, class DataT
		, class CompT = std::less<typename KeyT::value_type>
		, class AllocT = std::allocator<std::pair<const KeyT, DataT> > >
class structured_multimap
{
public:
	typedef KeyT                                           key_type;
	typedef DataT                                          mapped_type;
	typedef std::pair<const KeyT, DataT>                   value_type;
	typedef typename KeyT::value_type                      char_type;
	typedef CompT                                          char_compare;
														 
	typedef AllocT                                         allocator_type;
	typedef typename AllocT::difference_type               difference_type;
	typedef typename AllocT::size_type                     size_type;
	typedef typename AllocT::pointer                       pointer;
	typedef typename AllocT::const_pointer                 const_pointer;
	typedef typename AllocT::reference                     reference;
	typedef typename AllocT::const_reference               const_reference;

private:
	//! Internal value is a pair<Key, Val>, non-const Key
	typedef std::list< std::pair<KeyT, DataT> 
					 , typename AllocT::template rebind<std::pair<KeyT, DataT> >::other
					 >                                     ValueList;

	// the tree backend just stores size_type index into our data vector.
	typedef ternary_tree< KeyT, ValueList, CompT
						, typename AllocT::template rebind<ValueList>::other
						>                                  ternary_tree;
	typedef typename ternary_tree::iterator                tst_iterator;
	typedef typename ternary_tree::const_iterator          tst_const_iterator;

	typedef smap_detail::multimap_iterator<tst_iterator>   multimap_iterator;

	enum { invalid_index = size_type(-1) };

public:

	typedef typename ternary_tree::key_compare        key_compare;

	typedef typename iterators::iterator_wrapper< multimap_iterator
												, iterators::nonconst_traits<value_type> 
												>     iterator;

	typedef typename iterators::iterator_wrapper< multimap_iterator
												, iterators::const_traits<value_type> 
												>     const_iterator;

	typedef std::reverse_iterator<iterator>           reverse_iterator;
	typedef std::reverse_iterator<const_iterator>     const_reverse_iterator;

	/** \name Construct, copy, destroy
	 * @{
	 */
	structured_multimap() 
		: m_tree(char_compare(), allocator_type())
		, m_size(0)
	{}

	explicit structured_multimap(const char_compare& comp)
		: m_tree(comp, allocator_type())
		, m_size(0)
	{}

	structured_multimap(const char_compare& comp, const allocator_type& alloc)
		: m_tree(comp, alloc)
		, m_size(0)
	{}

	template<class InputIterator>
	structured_multimap( InputIterator first, InputIterator last,
					const char_compare& comp = char_compare(), 
					const allocator_type& alloc = allocator_type())
		: m_tree(comp, alloc)
		, m_size(0)
	{
		insert(first, last);
	}

	structured_multimap(const structured_multimap& other)
		: m_tree(other.m_tree)
		, m_size(other.m_size)
	{}

	~structured_multimap() {}

	structured_multimap& operator= (const structured_multimap& other) {
		structured_multimap(other).swap(*this);
		return *this;
	}

	allocator_type get_allocator() const { return m_tree.get_allocator(); }
	/* @} */

	/** \name Iterators
	 * Includes C++0x methods cbegin, cend, crbegin, crend to make it easier 
	 * to access const iterators.
	 * @{
	 */
	iterator                begin()         { return iterator(m_tree.begin()); }
	const_iterator          begin()   const { return const_iterator(m_tree.begin()); }
	iterator                end()           { return iterator(m_tree.end()); }
	const_iterator          end()     const { return const_iterator(m_tree.end()); }

	reverse_iterator        rbegin()        { return reverse_iterator(end()); }
	const_reverse_iterator  rbegin()  const { return const_reverse_iterator(end()); }  
	reverse_iterator        rend()          { return reverse_iterator(begin()); }
	const_reverse_iterator  rend()    const { return const_reverse_iterator(begin()); }

	// C++0x additions
	const_iterator          cbegin()  const { return const_iterator(m_tree.begin()); }
	const_iterator          cend()    const { return const_iterator(m_tree.end()); }
	const_reverse_iterator  crbegin() const { return const_reverse_iterator(end()); }  
	const_reverse_iterator  crend()   const { return const_reverse_iterator(begin()); }
	/* @} */

	/** \name Capacity
	 * @{
	 */
	bool empty() const { return m_tree.empty(); }
	size_type size() const { return m_tree.size(); }
	size_type max_size() const { return m_tree.max_size(); }
	/* @} */

	/** \name Modifiers
	 * @{
	 */
	iterator insert(const value_type& val)
	{
		iterator result(insert_impl(val).first);
		// Return inserted value by incrementing tst iterator then decrementing multimap iterator
		// (implicitly backing up to previous tst iter, at last list element)
		// These operations should be no-throw!
		result.iter_base().iter_base().iter_base().increment();
		result.iter_base().decrement();
		return result;
	}

	iterator insert(iterator where, const value_type& val) { return insert(val); }

	template<class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		while(first != last)
			insert_impl(*first++);
	}

	// returns iterator as per C++0x
	iterator erase(iterator pos)
	{
		tst_iterator& it = pos.iter_base().iter_base();
		ValueList& values = *it;
		size_type count = values.size();
		m_size--;
		if (count == 1)
			 return iterator(m_tree.erase(it));
		else {
			iterator result(pos);
			++result;
			values.erase(pos.iter_base().current());
			return result;
		}
	}

	size_type erase(const key_type& key)
	{
		tst_iterator it = m_tree.find(key);
		if (it == m_tree.end())
			return 0;
		size_type count = (*it).size();
		m_size -= count;
		m_tree.erase(it);
		return count;
	}

	// returns iterator as per C++0x
	iterator erase(iterator first, iterator last) {
		iterator result = last;
		while (first != last)
			result = erase(first++);
		return result;
	}

	void swap(structured_multimap& other) {
		m_tree.swap(other.m_tree);
		std::swap(m_size, other.m_size);
	}

	void clear() {
		m_tree.clear();
		m_size = 0;
	}
	/* @} */

	/** @name Container comparison.
	 * @{
	 */
	bool operator==(const structured_multimap& rhs) const { return m_tree == rhs.m_tree; }
	bool operator< (const structured_multimap& rhs) const { return m_tree < rhs.m_tree; }
	/* @} */

	/** \name Observers
	 * @{
	 */
	//! Returns a copy of the key element comparator object.
	char_compare  char_comp() const { return m_tree.char_comp(); }

	/** Returns a copy of the key_compare class; this is defined in terms of a 
	 * lexicographical_compare using the char_compare object. */
	key_compare   key_comp() const { return m_tree.key_comp(); }

	//! Value comparator, applying the key_compare to the key part of value_type.
	class value_compare : public std::binary_function<value_type, value_type, bool> 
	{
	 public:
		bool operator()(const value_type& a, const value_type& b) const {
			return comp(a.first, b.first); 
		}
	 protected:
		//friend class ternary_tree;
		value_compare(key_compare c) : comp(c) {}
		key_compare comp;
	};

	value_compare value_comp() const { return value_compare(key_comp()); }
	/* @} */

	/** \name Map operations
	 * @{
	 */
	iterator       find(const key_type& key)       { return iterator( m_tree.find(key) ); }
	const_iterator find(const key_type& key) const { return const_iterator( m_tree.find(key) ); }

	size_type count(const key_type& key) const {
		tst_const_iterator it = m_tree.find(key);
		return (it != m_tree.end())? (*it).size() : 0;
	}
	
	iterator       lower_bound(const key_type& key)       { return iterator( m_tree.lower_bound(key) ); }
	iterator       upper_bound(const key_type& key)       { return iterator( m_tree.upper_bound(key) ); }
	const_iterator lower_bound(const key_type& key) const { return const_iterator( m_tree.lower_bound(key) ); }
	const_iterator upper_bound(const key_type& key) const { return const_iterator( m_tree.upper_bound(key) ); }

	std::pair<iterator, iterator> equal_range(const key_type& key)
	{
		std::pair<tst_iterator, tst_iterator> result(m_tree.equal_range(key));
		return std::make_pair(iterator(result.first), iterator(result.second));
	}

	std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const
	{
		std::pair<tst_const_iterator, tst_const_iterator>
			result(m_tree.equal_range(key));
		return std::make_pair(const_iterator(result.first), const_iterator(result.second));
	}
	/* @} */

	//! Default for partial_match_search ('?')
	enum constants { wildcard_char = ternary_tree::wildcard_char };

	//! Results from partial_match_search and hamming_search
	typedef search_results_list<ternary_tree, iterator>  search_results_list;

	/** \name Structured search operations [deprecated]
	 * The interface for structure searches remains to be defined.
	 * See \ref tst_structsearch "ternary_tree Structured Search section"
	 * @{
	 */
	//! See ternary_tree::prefix_range
	std::pair<iterator, iterator>
	prefix_range(const key_type& key) {
		std::pair<tst_iterator, tst_iterator> result(m_tree.prefix_range(key));
		return std::make_pair(multimap_iterator(result.first), multimap_iterator(result.second));
	}

	std::pair<const_iterator, const_iterator>
	prefix_range(const key_type& key) const
	{
		std::pair<tst_const_iterator, tst_const_iterator> result(m_tree.prefix_range(key));
		return std::make_pair(const_iterator(result.first), const_iterator(result.second));
	}

	//! See ternary_tree::longest_match
	template<class CharIter>
	iterator longest_match(CharIter & first, CharIter last) {
		return iterator( m_tree.longest_match(first, last) );
	}

	template<class CharIter>
	const_iterator longest_match(CharIter & first, const CharIter last) const {
		return const_iterator( m_tree.longest_match(first, last) );
	}

	//! Required for partial_match_search and hamming_search calls.
	search_results_list create_search_results() const {
		return m_tree.create_search_results();
	}

	//! See ternary_tree::partial_match_search
	template<class OutputIter>
	OutputIter partial_match_search(const key_type& key, OutputIter results, 
									char_type wildcard = wildcard_char) const
	{
		return m_tree.partial_match_search(key, results, wildcard);
	}

	//! See ternary_tree::hamming_search
	template<class OutputIter>
	OutputIter hamming_search(const key_type& key, OutputIter results, size_type dist) const
	{
		return m_tree.hamming_search(key, results, dist);
	}

	//! See ternary_tree::levenshtein_search
	template<class OutputIter>
	OutputIter levenshtein_search(const key_type& key, OutputIter results, size_type dist) const
	{
		return m_tree.levenshtein_search(key, results, dist);
	}

	//! See ternary_tree::combinatorial_search (not part of Structured container interface)
	template<class OutputIter>
	OutputIter combinatorial_search(const key_type& key, OutputIter results, 
									size_type wildcards = 0) const
	{
		return m_tree.combinatorial_search(key, results, wildcards);
	}
	/* @} */

private:

	std::pair<tst_iterator, bool> insert_impl(const value_type& val)
	{
		ValueList values(get_allocator());
		std::pair<tst_iterator, bool> tst_result =
			m_tree.insert( std::make_pair(val.first, values) );
		try {
			(*tst_result.first).push_back(val);
		} catch(std::exception&) {
			// rollback is complete, tst internal node vector is pop_back'd
			// if we erase the last inserted value
			if (tst_result.second)
				m_tree.erase(tst_result.first);
			throw;
		}
		m_size++;
		return tst_result;
	}

private:
	// To get non-const iterator base, and instead of specifying tree mutable
	ternary_tree& mutable_tree() const { return const_cast<ternary_tree&>(m_tree); }

	ternary_tree  m_tree;
	size_type     m_size;
};



//! \relates structured_multimap
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator==(const structured_multimap<KeyT, DataT, CompT, AllocT>& x,
				const structured_multimap<KeyT, DataT, CompT, AllocT>& y) { return x.operator==(y); }
//! \relates structured_multimap
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator!=(const structured_multimap<KeyT, DataT, CompT, AllocT>& x,
				const structured_multimap<KeyT, DataT, CompT, AllocT>& y) { return !(x == y); }
//! \relates structured_multimap
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator< (const structured_multimap<KeyT, DataT, CompT, AllocT>& x,
				const structured_multimap<KeyT, DataT, CompT, AllocT>& y) { return x.operator<(y); }
//! \relates structured_multimap
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator> (const structured_multimap<KeyT, DataT, CompT, AllocT>& x,
				const structured_multimap<KeyT, DataT, CompT, AllocT>& y) { return y < x; }
//! \relates structured_multimap
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator<=(const structured_multimap<KeyT, DataT, CompT, AllocT>& x,
				const structured_multimap<KeyT, DataT, CompT, AllocT>& y) { return !(y > x); }
//! \relates structured_multimap
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator>=(const structured_multimap<KeyT, DataT, CompT, AllocT>& x,
				const structured_multimap<KeyT, DataT, CompT, AllocT>& y) { return !(x < y); }

//! \relates structured_multimap
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator==(typename structured_multimap<KeyT, DataT, CompT, AllocT>::const_iterator& a, 
				typename structured_multimap<KeyT, DataT, CompT, AllocT>::const_iterator& b)
{
	return a.operator==(b);
}

//! \relates structured_multimap
template<class KeyT, class DataT, class CompT, class AllocT>
bool operator!=(typename structured_multimap<KeyT, DataT, CompT, AllocT>::const_iterator& a, 
				typename structured_multimap<KeyT, DataT, CompT, AllocT>::const_iterator& b)
{
	return !(a == b);
}

}  // namespace containers



// specialized algorithm
namespace std {

	//! \relates structured_multimap
	template<class KeyT, class DataT, class CompT, class AllocT>
	inline void swap(containers::structured_multimap<KeyT, DataT, CompT, AllocT> a, 
					 containers::structured_multimap<KeyT, DataT, CompT, AllocT> b)
	{
		a.swap(b);
	}

}



#endif  // STRUCTURED_MAP_HPP_INCLUDED
