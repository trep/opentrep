// block direct use
#ifndef TERNARY_TREE_INCLUDE_GUARD
# error This is a dependent header file, included by ternary_tree.hpp - DO NOT USE DIRECTLY
#endif


namespace containers {

//---------------------------------------------------------------------------------------
// ternary_tree:: search_results_list class
//---------------------------------------------------------------------------------------


/** Vector-like container filled by ternary_tree::partial_match_search or 
 * ternary_tree::hamming_search; it contains (items that are convertible to) 
 * ternary_tree iterators.
 * Maybe it should be a vector<ternary_tree::iterator> -- this uses twice the space,
 * but has keys stored at the ready. This is a moot point when structured search interface is ready.
 */
template<class TreeT, class IteratorT> 
class search_results_list
{
	typedef TreeT                                   ternary_tree;
	typedef typename TreeT::NodeList                NodeList;
	typedef typename TreeT::size_type               size_type;
	typedef typename TreeT::node_index              node_index;
	typedef typename TreeT::tst_iterator_base       tst_iterator_base;
	typedef std::vector< node_index,
		typename ternary_tree::allocator_type::template rebind<node_index>::other
						           >                IndexList;
	enum { invalid_index = size_type(-1) };
	// Friend with other types, to rebind from tree to structured container's iterator
	template <typename T2, typename I2> friend class search_results_list;
public:
	typedef node_index                              value_type;
	typedef node_index&                             reference;
	typedef const node_index&                       const_reference;
	typedef IteratorT                               tree_iterator;

	search_results_list(ternary_tree& tst) :  m_nodelist(tst.m_nodes) {}

	//! Converting copy constructor to get result list from tree, 
	//! bound to structured container's iterator
	template<class OtherSRL>
	search_results_list(const OtherSRL& other)
		: m_nodelist(other.m_nodelist)
		, m_indexlist(other.m_indexlist)
	{}

	//! Used to support back_insert_iterator.
	search_results_list& push_back(node_index nid) { m_indexlist.push_back(nid); return *this; }

	search_results_list& operator= (const search_results_list& other) {
		m_nodelist = other.m_nodelist;
		m_indexlist = other.m_indexlist;
		return *this;
	}

	tree_iterator operator[](size_type index) { return tree_iterator( tst_iterator_base(m_nodelist, m_indexlist[index]) ); }

	class iterator;

	iterator begin() const;
	iterator end() const;

	bool      empty() const { return m_indexlist.empty(); }
	size_type size()  const { return m_indexlist.size(); }
	void      clear()       { m_indexlist.clear(); }

private:
	// template friendly for rebinding constructor

	NodeList&  m_nodelist;
	IndexList  m_indexlist;
};


//---------------------------------------------------------------------------------------
// ternary_tree:: search_results_list:: iterator class
//---------------------------------------------------------------------------------------

template<class TreeT, class IteratorT> 
class search_results_list<TreeT, IteratorT>::iterator
{
public:
	tree_iterator operator*() { return tree_iterator( tst_iterator_base(m_nodelist, *m_iter) ); }

	iterator& operator++() { ++m_iter; return *this; }
	iterator operator++(int) { iterator tmp(*this); ++m_iter; return tmp; }

	iterator& operator=(const iterator& other) { m_iter = other.m_iter; return *this; }

	bool operator==(const iterator& rhs) const { return m_iter == rhs.m_iter; }
	bool operator!=(const iterator& rhs) const { return m_iter != rhs.m_iter; }
private:
	typedef typename IndexList::const_iterator  base_iter;
	friend class search_results_list;
	iterator(NodeList& nodes, base_iter it) : m_nodelist(nodes), m_iter(it) {}
	NodeList&  m_nodelist;
	base_iter  m_iter;
};

template<class TreeT, class IteratorT> 
typename search_results_list<TreeT, IteratorT>::iterator
search_results_list<TreeT, IteratorT>::begin() const
{
	return iterator(m_nodelist, m_indexlist.begin());
}

template<class TreeT, class IteratorT> 
typename search_results_list<TreeT, IteratorT>::iterator
search_results_list<TreeT, IteratorT>::end() const
{
	return iterator(m_nodelist, m_indexlist.end());
}

template<class S, class D, class C, class A> 
inline 
typename ternary_tree<S, D, C, A>::search_results_list
ternary_tree<S, D, C, A>::create_search_results() const {
	return search_results_list(*const_cast<ternary_tree*>(this));
}


} // end namespace containers

