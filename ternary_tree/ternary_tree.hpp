#ifndef TERNARY_TREE_HPP_INCLUDED
#define TERNARY_TREE_HPP_INCLUDED
// Used in tst_detail/* files
#define TERNARY_TREE_INCLUDE_GUARD

/** Conversion to C++ of the DDJ article code by Jon Bentley and Robert Sedgewick.
 * See http://www.cs.princeton.edu/~rs/strings/
 *
 * rasmus ekman 2005 -- rasmus.ekman@abc.se
 *
 * version 0.67 (March 10, 2006) (pre-beta: still some interface issues)
 *
 * Version history:
 * 0.68: refurbish node type, look for '///#' to see changes
 * 0.67: added levenshtein search.
 * 0.66: added combinatorial ("scrabble") search. Fixed map/set iterators.
 * 0.65: first public relase.
 *
 * Available from http://abc.se/~re/code/tst/ \n
 * Download: http://abc.se/~re/code/tst/ternary_tree.zip
 * \file
 */

#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <functional>
#include <utility>

#include "iterator_wrapper.hpp"

#include "tst_detail/tst_node.hpp"
#include "tst_detail/new_iterator_base.ipp"

// Defines macros TST_ITERATOR_FACADE_BEGIN and TST_ITERATOR_FACADE_END
// (which we undef at end of this file)
#include "tst_detail/tst_iterator_facade.hpp"

#ifdef _MSC_VER
# pragma warning(disable: 4702)
//# ifdef NDEBUG
// msvc enums are always signed
//#  pragma warning(disable: 4127 4245 4702)
//# endif
#endif


/** \def TST_USE_FASTMAP 
 * Experimental stores an alphabet-size vector with node pointers to first chars -
 * this is no real gain, 2-3% at most, only sorted-insert successful find was 11% better.
 * Possibly a two-char fastmap could give some benefit, or with large wchar_t alphabets.
 */
//#define TST_USE_FASTMAP 1

namespace containers {

namespace tst_detail {

	//template<class StringT, class DataT, class CompT, class AllocT> class tst_iterator_base;

	template<class InputIter> struct levenshtein_search_info;

}  // end namespace tst_detail



template<class TreeT, class IteratorT> class search_results_list;


/** Ternary search tree (trie) is a sorted container for strings,
 * with advanced search possibilities (wildcard and near-match searches), 
 * and fast lookup (similar to hash_map, 3-6 times faster than map) -
 * It is typically used for dictionaries.
 *
 * A ternary tree is a Structured Associative Container. This means that its key type 
 * is required to be a Forward Container, and that the tree uses a comparator to establish 
 * a strict weak ordering among \c key::value_type elements (rather than on whole keys). 
 * This allows searches involving parts of keys, ie with shared prefix or with shared
 * middle parts. 
 *
 * ternary_tree is a Unique Associative Container. Note that it is \b not Pair Associative,
 * so the value  that is returned when dereferencing an iterator is not a \c std::pair<const Key, Data>
 * as for map or unsorted_map. Instead the Data value is returned.
 * To get the key that a ternary_tree iterator is positioned at, iterator specifies member function 
 * key() - see tst_detail::iter_method_forward.
 *
 * ternary_tree is a Sorted, and thus by implication a Reversible Container.
 * While iteration is amortized constant time, it is often slower than other
 * Associative containers since it must walk several key element nodes to find 
 * next key endnode. 
 * The iteration cost factor is proportional to node_count() / total_key_length().
 *
 * <h3>Exception safety</h3>
 * ternary_tree generally conforms to standard Associative Container exception guarantees:
 *  - clear(), erase() operations do not throw an exception.
 *  - no swap() operation throws an exception, unless thrown by the copy constructor of the
 *    key element Compare object.
 *  - If an exception is thrown by an insert() operation while inserting a single element, 
 *    that function has no effect.
 *
 * <h3>Controlling macros</h3>
 *  - \c TST_NODE_COUNT_TYPE: controls the count of nodes. On systems where std::size_t
 *     may be a 64-bit value, this can be set to a 32-bit unsigned type, if you know
 *     that the total count of characters inserted in the tree will not exceed its limits.
 *     If the macro is undefined, it defaults to \c std::size_t. 
 *     Note that the macro is undefined internally in the tree, so no code except  
 *     the node typedef uses it.
 *  - If \c TST_USE_FASTMAP is defined, a direct link to first-level symbol nodes is maintained 
 *    internally. This provides no measurable improvement with 8-bit charsets, but the
 *    interested user can try it with 16-bit chars (and please report if you can measure it)
 *
 * See further \ref tst_impl
 *
 * <h3>Balanced and unbalanced trees</h3>
 * It should be noted that insertion order affects tree performance:
 * After sorted (lexicographical) insertion, lookup is sometimes 50-100% slower, 
 * while iterators are (always) 50-90% faster than with random-order string insertion. 
 * There is currently no support for rebalancing the tree.
 *
 * \todo 
 *  - "Structured" may be an overstatement? - Any better term?
 *  - Structure search interface, sub-key iterators etc.
 *  - Implement optional pointer node over boost pool_alloc using DDJ-like nodes, 
 *    to get better erase() behaviour; (optional since 64-bit versions get large node size).
 *
 * \ingroup containers
 */
template<class StringT, 
		 class DataT,
		 class CompT  = std::less<typename StringT::value_type>,
		 class AllocT = std::allocator<DataT> >
class ternary_tree
{
public:
	typedef StringT                              key_type;
	typedef typename StringT::value_type         char_type;
	typedef CompT                                char_compare;
	typedef DataT                                value_type;
	typedef DataT                                mapped_type;

	typedef AllocT                               allocator_type;
	typedef typename AllocT::pointer             pointer;
	typedef typename AllocT::const_pointer       const_pointer;
	typedef typename AllocT::reference           reference;
	typedef typename AllocT::const_reference     const_reference;
	typedef typename AllocT::difference_type     difference_type;
	typedef typename AllocT::size_type           size_type;

private:
#ifndef TST_NODE_COUNT_TYPE
	// on 64-bit machines this can optionally be set to an unsigned 32-bit type
#  define TST_NODE_COUNT_TYPE  size_type
#endif

	/** Used for cached key, since key_type is only req to be Forward Container, 
	 * and we need push_back(), pop_back() methods (in iterator and in a search method) */
	typedef std::basic_string<char_type, std::char_traits<char_type>, 
							  typename allocator_type::template rebind<char_type>::other
							 >  string_type;

	//! Node size only configurable by code change and recompile (FIXME)
	// we don't need to rebind allocator since (de)allocate is template (?? is this still true?)
	///# typedef tst_detail::node<char_type, mapped_type, allocator_type, size_type>  node;
	typedef typename tst_detail::size_policy_node<char_type, mapped_type,
												  allocator_type, TST_NODE_COUNT_TYPE
												 >::node_type  node;
#undef TST_NODE_COUNT_TYPE

public:
	//! Dependent type, defined by the macro \c TST_NODE_COUNT_TYPE (default: size_type)
	typedef typename node::node_index            node_index;

	//! Impl note: nodes are stored in vector, effectively working as pool allocator.
	//! node_index vector offsets are used instead of pointers.
	typedef std::vector<node, typename allocator_type::template rebind<node>::other>  NodeList;

	///#typedef tst_detail::tst_iterator_base<StringT, DataT, CompT, AllocT>   tst_iterator_base;
	typedef tst_detail::tst_iterator_base<NodeList, string_type>     tst_iterator_base;

	///#friend class tst_detail::tst_iterator_base<StringT, DataT, CompT, AllocT>;
	friend class tst_detail::tst_iterator_base<NodeList, string_type>;


# ifndef TST_NO_STANDALONE_ITERATOR_FACADE
	//! Needed to construct iterators for (eg) structured_multimap
	friend struct tst_detail::iter_method_forward<ternary_tree>; 
# endif

#ifdef TST_USE_FASTMAP
	std::vector<node_index, typename allocator_type::template rebind<node_index>::other>  m_fastmap;
#   ifdef _DEBUG
#       pragma message("ternary_tree uses fastmap")
#   endif  // _DEBUG
#   define TST_INIT_FASTMAP m_fastmap.clear(); m_fastmap.resize(1 << sizeof(char_type));
#else
#   define TST_INIT_FASTMAP 
#endif

public:

	typedef TST_ITERATOR_FACADE_BEGIN
					iterators::iterator_wrapper < tst_iterator_base
												, iterators::nonconst_traits<value_type> >
			TST_ITERATOR_FACADE_END                iterator;

	typedef TST_ITERATOR_FACADE_BEGIN
					iterators::iterator_wrapper < tst_iterator_base
												, iterators::const_traits<value_type> >
			TST_ITERATOR_FACADE_END                const_iterator;

	typedef std::reverse_iterator<iterator>        reverse_iterator;
	typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;


	/** \name Construct, copy, destroy
	 * @{
	 */
	explicit ternary_tree(const char_compare& comp = char_compare(),
						  const allocator_type& alloc = allocator_type())
		: m_nodes(alloc)
		, m_comparator(comp)
		, m_allocator(alloc)
		, m_item_count(0)
		, m_total_key_length(0)
		, m_longest_key(0)
		, m_last_item(m_nodes, node::invalid_index)
	{ TST_INIT_FASTMAP }

	template<class InputIterator>
	ternary_tree(InputIterator first, InputIterator last,
				const char_compare& comp = char_compare(), 
				const allocator_type& alloc = allocator_type())
		: m_nodes(alloc)
		, m_comparator(comp)
		, m_allocator(alloc)
		, m_item_count(0)
		, m_total_key_length(0)
		, m_longest_key(0)
		, m_last_item(m_nodes, node::invalid_index)
	{
		insert(first, last);
	}

	ternary_tree(const ternary_tree& other)
		: m_nodes(other.m_nodes)
		, m_comparator(other.m_comparator)
		, m_allocator(other.m_allocator)
		, m_item_count(other.m_item_count)
		, m_total_key_length(other.m_total_key_length)
		, m_longest_key(other.m_longest_key)
		, m_last_item(other.m_last_item)
	{ TST_INIT_FASTMAP }


	~ternary_tree()
	{
		///# node::deallocate(m_nodes.begin(), m_nodes.end(), m_allocator);
	}

	ternary_tree& operator= (const ternary_tree& other) {
		ternary_tree(other).swap(*this);
		return *this;
	}

	//! Returns a copy of the allocator used to construct the tree.
	allocator_type get_allocator() const { return m_allocator; }
	/* @} */

	/** \name Iterators
	 * Includes C++0x methods cbegin, cend, crbegin, crend to make it easier 
	 * to access const iterators.
	 * @{
	 */
	iterator               begin()         { return tst_iterator_base(m_nodes, 0); }
	const_iterator         begin()   const { return tst_iterator_base(m_nodes, 0); }
	iterator               end()           { return tst_iterator_base(m_nodes, node::invalid_index); }
	const_iterator         end()     const { return tst_iterator_base(m_nodes, node::invalid_index); }

	reverse_iterator       rbegin()        { return reverse_iterator(end()); }
	const_reverse_iterator rbegin()  const { return const_reverse_iterator(end()); }
	reverse_iterator       rend()          { return reverse_iterator(begin()); }
	const_reverse_iterator rend()    const { return const_reverse_iterator(begin()); }

	const_iterator         cbegin()  const { return tst_iterator_base(m_nodes, 0); }
	const_iterator         cend()    const { return tst_iterator_base(m_nodes, node::invalid_index); }
	const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
	const_reverse_iterator crend()   const { return const_reverse_iterator(begin()); }
	/* @} */

	/** \name Capacity
	 * @{
	 */
	bool empty() const { return 0 == item_count(); }

	size_type size() const { return item_count(); }

	// each string requires two nodes (one char, one endnode), and then there's the root
	size_type max_size() const { return m_nodes.max_size() / 2 - 1; }
	/* @} */

	/** \name Element access
	 * @{
	 */
	//! Inserts a key into the tree and returns a mutable reference to its mapped_value.
	mapped_type& operator[](const key_type& key)
	{
		node_index nid = subscript_insert(key.begin(), key.end());
		return m_nodes[nid].value();
	}
	/* @} */

	/** \name Modifiers
	 * @{
	 */
	/** Returns a pair whose bool component returns true if an insertion 
	 * was made and false if the tree already contained an an equivalent key.
	 */
	std::pair<iterator, bool> insert(const std::pair<key_type, value_type>& val)
	{
		const key_type& key = val.first;
		// (we can store the empty string, so no reason to block it)
		size_type prevcount = node_count();
		node_index pos = insert_impl(key.begin(), key.end());
		// was a new insertion made?
		bool added = prevcount < node_count();
		if (added) {
			// assign value only if added key
			node& n = m_nodes[pos];
			n.assign(val.second, m_allocator);
			// maintain stats
			m_total_key_length += key.size();
			if (key.size() > m_longest_key)
				m_longest_key = key.size();
			m_item_count++;
		}
		return std::make_pair(tst_iterator_base(m_nodes, pos), added);
	}

	/** Insert a range from another ternary_tree  
	 * (or any iterator to pair with same key_type, mapped_type).
	 */
	template<class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		while (first != last) {
			insert(*first++);
		}
	}

	void insert(const_iterator first, const_iterator last) {
		while (first != last) {
			insert(first.key(), *first);
			++first;
		}
	}

	//! Associative Container method, we do not use hint, so pointless.
	iterator insert(iterator where, const std::pair<key_type, value_type>& val) {
		return insert(val).first;
	}

	/** Makes key unreachable, but usually does not reclaim nodes to memory pool.
	 * Postcondition: find(key) returns end().
	 * Does not invalidate any iterators except those pointing to an erased element.
	 * If key exists, item_count() is decremented; node_count() usually does not change.
	 * If size() == 1, clear() is called to wipe the slate clean.
	 */
	size_type erase(const key_type& key)
	{
		typename key_type::const_iterator first = key.begin();
		typename key_type::const_iterator last = key.end();
		find_result found = find_impl(first, last);
		if (found.pnid != 0)
			return 0;
		if (item_count() == 1)
			clear();
		else
			erase_impl(found.nid, key.size());
		return 1;
	}

	/** Makes key unreachable, but usually does not reclaim nodes to memory pool.
	 * Postcondition: find(key) returns end().
	 * Does not invalidate any iterators except those pointing to an erased element.
	 * If key exists, item_count() is decremented; node_count() usually does not change.
	 * If size() == 1, clear() is called to wipe the slate clean.
	 */
	iterator erase(iterator it)
	{
		iterator result(it);
		++result;
		// must call clear_data() on last remaining item(?)
		node_index pos = it.iter_base().tree_position();
		if (item_count() == 1 && it == begin()) {
			m_nodes[pos].clear_data(m_allocator);
			clear();
			return end();
		}
		node_index post = erase_impl(pos, it.iter_base().key().size());
		post = 1; // get rid of warning
		return result;
	}

	/** Erases a range of values - makes keys unreachable, but usually does not 
	 * reclaim nodes to memory pool (currently just removes each in a loop, 
	 * could try better wholesale node removal).
	 * Returns iterator as per C++0x
	 */
	iterator erase(iterator first, iterator last)
	{
		node_index post = node::invalid_index;
		while(first != last) {
			post = erase_impl(first.iter_base().tree_position(), first.key().size());
			++first;
		}
		assert(tst_iterator_base(m_nodes, post).tree_position() == last.iter_base().tree_position());
		return last;
	}

	void swap(ternary_tree& other)
	{
		// Begin with comp, since it's the only one that may throw
		std::swap(m_comparator, other.m_comparator);
		m_nodes.swap(other.m_nodes);
		std::swap(m_allocator, other.m_allocator);
		std::swap(m_item_count, other.m_item_count);
		std::swap(m_total_key_length, other.m_total_key_length);
		std::swap(m_longest_key, other.m_longest_key);
		m_last_item.swap(other.m_last_item);
	}

	void clear() { ternary_tree().swap(*this); TST_INIT_FASTMAP }
	/* @} */


	/** @name Sorted Associative Container methods.
	 * @{
	 */

	//! Returns const_iterator to key, or end() if not found.
	const_iterator find(const key_type& key) const
	{
		typename key_type::const_iterator first = key.begin(), last = key.end();
		find_result found = find_impl(first, last);
		node_index nid = found.nid;
		if (nid < node_count() && found.pnid == 0)
			return tst_iterator_base(m_nodes, nid);
		return end();
	}

	//! Returns mutable iterator to key, or end() if not found
	iterator find(const key_type& key)
	{
		/*const ternary_tree& tst(*this);
		const_iterator result = tst.find(key);
		return result.iter_base(); */
		typename key_type::const_iterator first = key.begin(), last = key.end();
		find_result found = find_impl(first, last);
		node_index nid = found.nid;
		if (nid < node_count() && found.pnid == 0)
			return tst_iterator_base(m_nodes, nid);
		return end();
	}

	/** Returns an iterator to the first element in a map with a key value that is 
	 * equal to or greater than that of the specified key. 
	 * If \a key exists, it is returned, else returns the element that has
	 * \a key for a prefix (or end() if no such element exists).
	 *
	 * Complexity is that of a find() operation.
	 * \see prefix_range
	 */
	iterator lower_bound(const key_type& key) { return iterator( lower_bound_impl(key).iter_base() ); }

	/** Returns an iterator to the first element in a map with a key value that is 
	 * greater than the specified key. 
	 * If \a key exists, this returns ++key, else it returns the element that has
	 * \a key for a prefix (or end() if no such element exists).
	 *
	 * Complexity is that of a find() operation + one iterator increment.
	 * \see prefix_range
	 */
	iterator upper_bound(const key_type& key) { return iterator( upper_bound_impl(key).iter_base() ); }

	const_iterator lower_bound(const key_type& key) const { return lower_bound_impl(key); }
	const_iterator upper_bound(const key_type& key) const { return upper_bound_impl(key); }

	/** Returns a pair of iterators respectively to the first element in a map 
	 * with a key that is equal to or greater than the specified key, and a value that
	 * is greater than key. The first is the lower_bound of the key, and the second
	 * is the upper_bound of the key.
	 *
	 * Complexity is that of a find() operation + one iterator increment.
	 * \see prefix_range
	 */
	std::pair<iterator, iterator>
	equal_range(const key_type& key) {
		std::pair<const_iterator, const_iterator> result = equal_range_impl(key);
		return std::make_pair(tst_iterator_base(m_nodes, result.first.iter_base().tree_position()),
							  tst_iterator_base(m_nodes, result.second.iter_base().tree_position()));
	}
	std::pair<const_iterator, const_iterator>
	equal_range(const key_type& key) const { return equal_range_impl(key); }

	//! Returns count of key values in tree - returns 0 or 1.
	size_type count(const key_type& key) const { return (find(key) == end())? 0 : 1; }

	/*@}*/

	//! Default wildcard for partial_match_search.
	enum { wildcard_char = char_type('?') };

	// Need to befriend all result list types, for its constructor
	template <typename Tree, typename Iter> friend class search_results_list;

	typedef search_results_list<ternary_tree, iterator>  search_results_list;

	/** @name Structured Container search methods [deprecated]
	 * \anchor tst_structsearch
	 * These searches are possible by storing key elements in strict weak ordering,
	 * anchored at beginning of keys. This allows sub-key searches, ie searching
	 * for keys with matching subparts (ie first and last letter of a four-letter 
	 * string must match, or keys differing from search string in at most \e n letters).
	 *
	 * Complexity of sub-key searches are proportional to the "breadth" of the search. 
	 * Specifying that more than about 10% of key elements may differ can lead to 
	 * the search operation traversing large parts of the tree.  The front anchoring 
	 * of key elements also have the consequence that prefix matches are same cost as
	 * find operation (log in tree size), while suffix searches are linear in tree size.
	 * @{
	 */

	/** Returns a pair of iterators, the first points to the first key that begins with the prefix 
	 * (or end()), the second to the string following the last string that begins with prefix.
	 * The first iterator is the lower_bound of the key, while the second is the first key that
	 * does not share the prefix. 
	 * In a container with values { "aa", "aaa" "ab" }, equal_range("aa") returns { "aa", "aaa" },
	 * while prefix_match returns { "aa", "ab" }.
	 * Worst-case complexity is that of two find(prefix) operations.
	 * \see equal_range
	 */
	std::pair<iterator, iterator> prefix_range(const key_type& prefix) {
		std::pair<const_iterator, const_iterator> result = prefix_range_impl(prefix.begin(), prefix.end());
		return std::make_pair(tst_iterator_base(m_nodes, result.first.iter_base().tree_position()),
							  tst_iterator_base(m_nodes, result.second.iter_base().tree_position()));
	}

	std::pair<const_iterator, const_iterator> prefix_range(const key_type& prefix) const {
		return prefix_range_impl(prefix.begin(), prefix.end());
	}

	/** Returns the longest key that matches the beginning of the key element range (or end(),
	 * if none is found). The key element iterator \a first is advanced to the element 
	 * following the last matched character. 
	 *
	 * (This method allows using the TST dictionary as a simple tokenizer over \a first, \a last.)
	 *
	 * Complexity is that of a single find operation.
	 */
	template<class CharIter>
	iterator longest_match(CharIter & first, CharIter last)
	{
		node_index nid = longest_match_impl(first, last);
		tst_iterator_base it(m_nodes, nid);
		std::advance(first, it.key().size());
		return it;
	}

	template<class CharIter>
	const_iterator longest_match(CharIter & first, CharIter last) const
	{
		node_index nid = longest_match_impl(first, last);
		tst_iterator_base it(m_nodes, nid);
		std::advance(first, it.key().size());
		return it;
	}

	//! Factory method for use with wrapper classes.
	search_results_list create_search_results() const;

	/** Partial match search - supports wildcard characters (default: '?')
	 * Proxies for iterators to the found strings are pushed at the output iterator.
	 *
	 * A wildcard in search string entails visiting all subtrees of matched node(s).
	 * The worst-case complexity of partial match search is therefore O(m * k) * regular-find, 
	 * where m is size of alphabet, k is number of wildcard characters in search string,
	 * and "regular-find" is lookup cost of the rest of the search string.
	 * In practice, only a fraction of the alphabet is present except near root level 
	 * of the tree, but concrete performance is unpredictable.  
	 * Typical timings are in order of 10 times find(key) per item returned.
	 * See article available at \link tst_links Jon Bentley and Robert Sedgewick site \endlink.
	 */
	template<class OutputIter>
	OutputIter partial_match_search(const key_type& key, OutputIter results, 
									char_type wildcard = wildcard_char) const
	{
		partial_match_search_impl(0, key.begin(), key.end(), results, wildcard);
		return results;
	}

	/** Hamming search: Finds all keys differing from \c key in at most \c dist characters
	 * (as an extension to strict hamming search, this includes matching but shorter/longer keys).
	 * Proxies for iterators to the found strings are pushed at the output iterator.
	 *
	 * Searches on values of dist > 20% of average key length are typically expensive, 
	 * since a large part of the tree is traversed (expensive may mean several microseconds per call).
	 * \see http://wikipedia.org/wiki/Hamming_distance
	 */
	template<class OutputIter>
	OutputIter hamming_search(const key_type& key, OutputIter results, size_type dist) const
	{
		assert(int(dist) >= 0);
		hamming_search_impl(0, key.begin(), key.end(), results, int(dist));
		return results;
	}

	/** Levenshtein search: Finds all strings differing from \c key in at most \c dist characters.
	 * Proxies for iterators to the found strings are pushed at the output iterator.
	 *
	 * Searches on values of dist > 20% of average key length are typically expensive, 
	 * since a large part of the tree is traversed (expensive may mean several microseconds per call).
	 * \see http://wikipedia.org/wiki/Levenshtein_distance
	 */
	template<class OutputIter>
	OutputIter levenshtein_search(const key_type& key, OutputIter results, size_type dist) const
	{
		assert(int(dist) >= 0);
		typedef tst_detail::levenshtein_search_info<typename key_type::const_iterator> lev_info;
		lev_info info;
		typename lev_info::search ls(key.begin(), int(dist));
		info.push_back(ls);
		levenshtein_search_impl(0, info, key.end(), results);
		return results;
	}

	/** Combinatorial, or "scrabble" search: Finds all keys containing the characters in the search string. 
	 * The optional wildcard count allows mismatch characters (use with care, it increases 
	 * complexity of the search dramatically).
	 */
	template<class OutputIter>
	OutputIter combinatorial_search(const key_type& key, OutputIter results, size_type wildcards = 0) const
	{
		assert(int(wildcards) >= 0);  // defensive "help" for users
		string_type searchstr(key.begin(), key.end());
		std::sort(searchstr.begin(), searchstr.end());
		combinatorial_search_impl(0, searchstr, results, wildcards);
		return results;
	}
	/*@}*/

	/** @name Observers
	 * ternary_tree defines a key_compare class with same semantics as Sorted Associative Containers.
	 * Note that there is no value_compare type - the value_type does not contain the key.
	 * @{
	 */

	//! Returns a copy of the key element comparator object.
	char_compare char_comp() const { return m_comparator; }

	/** Key comparator, defined in terms of a lexicographical_compare using the char_compare object. */
	class key_compare : public std::binary_function<key_type, key_type, bool> 
	{
	 public:
		bool operator()(const key_type& a, const key_type& b) const {
			return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), comp); 
		}
	 protected:
		friend class ternary_tree;
		key_compare(char_compare c) : comp(c) {}
		char_compare comp;
	};

	//! Returns a copy of the key_compare class.
	key_compare key_comp() const { return key_compare(char_comp()); }

	//! Returns number of nodes in tree.
	size_type node_count() const { return m_nodes.size(); }
	//! Returns number of string-value pairs (alias for size()).
	size_type item_count() const { return m_item_count; }
	//! Returns sum of lengths of all keys inserted into tree.
	size_type total_key_length() const { return m_total_key_length; }
	//! Returns length of longest key ever inserted into tree (if key was erased, this value is not updated).
	size_type longest_key() const { return m_longest_key; }

	/*@}*/

	/** @name Container comparison.
	 * @{
	 */
	bool operator==(const ternary_tree& other) const
	{
		// likely to be fast here
		if (item_count() != other.item_count() || total_key_length() != other.total_key_length())
			return false;
		// ...but worst-case linear
		const_iterator i1(begin()), i2(other.begin()), last(end());
		while (i1 != last) {
			if (i1 != i2 || *i1 != *i2)
				return false;
			++i1, ++i2;
		}
		return true;
	}

	bool operator<(const ternary_tree& other) const {
		// likely to be fast here, but worst-case linear
		return std::lexicographical_compare(begin(), end(), other.begin(), other.end(), key_comp());
	}

	/*@}*/

	static std::ostream& print_node(std::ostream& ostr, const node& n);

private:

	iterator last_item() { return tst_iterator_base(m_nodes, m_last_item.tree_position()); }
	const_iterator last_item() const { return tst_iterator_base(m_nodes, m_last_item.tree_position()); }

	//! test characters for sort order.
	bool less(char_type a, char_type b) const {
		return m_comparator(a, b);
	}

	//! \see ternary_tree::find_impl()
	struct find_result {
		// Default constructor: point to internal node_index, so pnid is always assignable!
		find_result() : nid(node::invalid_index), pnid(&nid) {}
		find_result(node_index i, node_index *p) : nid(i), pnid(p) {}
		node_index nid;
		node_index *pnid;
	};

	/** This is the iteration master service for insert and partial match, etc.
	 * Returns a type that holds lookup result: index of the last matched node, and a pointer 
	 * to a node index. 
	 * If match was successful, find_result.pnid is null, else it points to the hikid or lokid
	 * of the matched node that would point to first unmatched char if it were inserted.
	 */
	template<class CharIter>
	find_result find_impl(CharIter & first, CharIter last) const;

	//! insert chars, returns at_end node index
	template<class CharIter>
	node_index insert_impl(CharIter first, CharIter last)
	{
	# if TST_USE_FASTMAP
		CharIter f0 = first;
	# endif
		find_result found = find_impl(first, last);
		if (found.pnid == 0)
			return found.nid;
	# if TST_USE_FASTMAP
		// maintain fastmap for first key chars only
		if (f0 == first && first != last) {
			unsigned long c = *first;
			if (c < m_fastmap.size() && m_fastmap[c] == 0)
				m_fastmap[c] = m_nodes.size();
		}
	# endif  // USE_FASTMAP

		return insert_append(found, first, last);
	}

	//! append nodes to find_results point
	template<class CharIter>
	node_index insert_append(find_result& found, CharIter first, CharIter last);


	//! Support operator[]
	template<class CharIter>
	node_index subscript_insert(CharIter first, CharIter last) 
	{
		size_type prevcount = node_count();
		node_index result = insert_impl(first, last);
		if (prevcount < node_count())
		{
			size_type length = last - first;
			m_total_key_length += length;
			if (length > m_longest_key)
				m_longest_key = length;
			m_item_count++;
		}
		return result;
	}

	//! Returns index of last disabled node (that could not be released)
	node_index erase_impl(node_index erasedindex, size_type keylen);

	const_iterator lower_bound_impl(const key_type& key) const
	{
		typename key_type::const_iterator first = key.begin(), last = key.end();
		find_result found = find_impl(first, last);
		// whether found or not, return nearest following the prefix 
		return tst_iterator_base(m_nodes, found.nid);
	}

	const_iterator upper_bound_impl(const key_type& key) const
	{
		typename key_type::const_iterator first = key.begin(), last = key.end();
		find_result found = find_impl(first, last);
		const_iterator result(tst_iterator_base(m_nodes, found.nid));
		// Must ensure greater than key, so if it was found we increment
		if (found.pnid == 0)
			return ++result;
		return result;
	}

	std::pair<const_iterator, const_iterator> 
	equal_range_impl(const key_type& key) const
	{
		typename key_type::const_iterator first = key.begin(), last = key.end();
		find_result found = find_impl(first, last);
		const_iterator result(tst_iterator_base(m_nodes, found.nid));
		// Must ensure second is greater than key, so if it was found we increment
		if (found.pnid == 0) {
			const_iterator result2(result);
			return std::make_pair(result, ++result2);
		}
		return std::make_pair(result, result);
	}

	//! prefix match implementation
	template<class CharIter>
	std::pair<const_iterator,const_iterator> prefix_range_impl(CharIter first, CharIter last) const;

	//! longest match implementation
	template<class CharIter>
	node_index longest_match_impl(CharIter & first, CharIter last) const;

	//! partial match search implementation
	template<class InputIter, class OutputIter>
	void partial_match_search_impl(node_index nid, InputIter first, InputIter last, 
								   OutputIter results, char_type wildcard) const;

	//! hamming_search implementation
	template<class InputIter, class OutputIter>
	void hamming_search_impl(node_index nid, InputIter first, InputIter last,
							 OutputIter results, int dist) const;

	//! levenshtein_search implementation
	template<class InputIter, class OutputIter>
	bool levenshtein_search_impl(node_index nid, tst_detail::levenshtein_search_info<InputIter>& info,
								 InputIter last, OutputIter results) const;

	//! combinatorial_search implementation
	template<class String, class OutputIter>
	void combinatorial_search_impl( node_index nid, const String& str,
									OutputIter results, size_type jokers) const;


private:
	NodeList m_nodes;
	char_compare m_comparator;
	allocator_type m_allocator;
	//! Keeps stats for tree insertion
	size_type m_item_count;
	//! Keeps stats for tree insertion
	size_type m_total_key_length;
	//! Keeps stats for tree insertion
	size_type m_longest_key;
	//! Index to last key endnode: must be maintained to have const-time reverse iteration.
	tst_iterator_base m_last_item;
};


//#############################################################################
//# ternary_tree free functions
//#############################################################################

//! \relates ternary_tree
template<class S, class D, class C, class A>
bool operator==(ternary_tree<S, D, C, A> const & x, ternary_tree<S, D, C, A> const & y)
{
	return x.operator==(y);
}

//! \relates ternary_tree
template<class S, class D, class C, class A>
bool operator!=(ternary_tree<S, D, C, A> const & x, ternary_tree<S, D, C, A> const & y)
{
	return !(x.operator==(y));
}

//! \relates ternary_tree
template<class S, class D, class C, class A>
bool operator< (ternary_tree<S, D, C, A> const & x, ternary_tree<S, D, C, A> const & y)
{
	return x.operator<(y);
}

//! \relates ternary_tree
template<class S, class D, class C, class A>
bool operator> (ternary_tree<S, D, C, A> const & x, ternary_tree<S, D, C, A> const & y)
{
	return y.operator<(x);
}

//! \relates ternary_tree
template<class S, class D, class C, class A>
bool operator<=(ternary_tree<S, D, C, A> const & x, ternary_tree<S, D, C, A> const & y)
{
	return !(x.operator>(y));
}

//! \relates ternary_tree
template<class S, class D, class C, class A>
bool operator>=(ternary_tree<S, D, C, A> const & x, ternary_tree<S, D, C, A> const & y)
{
	return !(x.operator<(y));
}

//! \relates ternary_tree
template<class S, class D, class C, class A>
inline std::ostream& 
operator<<(std::ostream& ostr, typename ternary_tree<S, D, C, A>::node const & node)
{
	ternary_tree<S, D, C, A>::print_node(ostr, node);
	return ostr;
}

} // end namespace containers


//#############################################################################
//# ternary_tree implementation
//#############################################################################


#include "tst_detail/tst_search_results.ipp"

#include "tst_detail/tst_implementation.ipp"


#undef TST_USE_FASTMAP
#undef TST_INIT_FASTMAP
#undef TST_ITERATOR_FACADE_BEGIN
#undef TST_ITERATOR_FACADE_END

#undef TERNARY_TREE_INCLUDE_GUARD

#endif  //TERNARY_TREE_HPP_INCLUDED
