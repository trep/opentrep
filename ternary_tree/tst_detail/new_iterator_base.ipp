// block direct use
#ifndef TERNARY_TREE_INCLUDE_GUARD
# error This is a dependent header file, included by ternary_tree.hpp - DO NOT USE DIRECTLY
#endif

#include "iteration_impl.hpp"

namespace containers {
namespace tst_detail {

//---------------------------------------------------------------------------------------
// ternary_tree::tst_iterator_base class
// 0.685: prepare for different iterator types; now uses iteration policy class: 
//        - insert-order iterator (= vector::iterator but only at end nodes)
//        - subkey iterator: can visit any non-erased node
//---------------------------------------------------------------------------------------


/** Implements forward and backward iteration in lexical order. 
 * Iteration is slightly expensive as it needs to walk nodes, and backtrack to find next value.
 * Complexity of increment is amortized constant time. Actual time taken is O(n) in average length 
 * of strings minus string common prefixes.
 *
 * tst_iterator_base is constructible from a node list and a node index; this is used 
 * with values returned from wildcard-match and neighbour-match searches.
 *
 * iteration_policy defines methods:
 *     - find_first_endnode - seek forward to the lowest/leftmost terminal node
 *     - find_last_endnode - backward, ditto
 *     - find_forward_path - seek towards trunk for forward/higher branch
 *     - find_backward_path - backward, ditto
 *
 */
template<class NodeListT, class StringT, class TraitsT = inorder_seek<NodeListT, StringT> >
class tst_iterator_base
{
private:
	typedef NodeListT                       NodeList;
	typedef TraitsT                         iter_traits;

	typedef key_access<NodeListT, StringT>  key_access;

public:
	typedef typename NodeListT::value_type  node;
	typedef typename NodeListT::size_type   node_index;
	typedef StringT                         string_type;

	typedef typename node::value_type       value_type;
	typedef value_type&                     reference;
	typedef const value_type&               const_reference;


	tst_iterator_base() : m_nodes(0), m_current_node(node::invalid_index) {}

	explicit tst_iterator_base(const NodeList& nodes)
		: m_nodes(&nodes), m_current_node(node::invalid_index)
	{}

	// ! Pre: key represents the key part at node index nid. Used internally by eg prefix_match etc; \n
	// ! Pre: This may not be used with user keys, only with keys extracted from the tree! 
	// ! (since comparator doesn't enforce identity)
/*	tst_iterator_base(const NodeList& nodes, node_index nid, const StringT& key)
		: m_nodes(&nodes), m_current_node(nid), m_current_key(key.begin(), key.end())
	{
		if (nid == node::invalid_index) {
			assert(m_current_key.empty());
			return;
		}
		m_current_node = iter_traits::find_first_endnode(*m_nodes, m_current_node, m_current_key);
		string_type tmp;
		assert(m_current_key == key_access::retrieve_key_string(*m_nodes, m_current_node, tmp));
	} */

	tst_iterator_base(const NodeList& nodes, node_index index)
		: m_nodes(&nodes), m_current_node(index)
	{
		if (index == node::invalid_index)
			return;
		if (m_nodes->empty())
			m_current_node = node::invalid_index;
		else {
			m_current_node = iter_traits::find_first_endnode(*m_nodes, m_current_node, m_current_key);
			if (index != 0) 
				key_access::retrieve_key_string(*m_nodes, m_current_node, m_current_key);
			else {
				string_type tmp;
				assert(m_current_key == key_access::retrieve_key_string(*m_nodes, m_current_node, tmp));
			}
		}
	}

public:
	/** @name publicly usable interface
	 * @{
	 */
	/** Returns the key at current iterator position - this value changes when iterator
	 * is incremented or decremented.
	 */
	const string_type& key() const { return m_current_key; }

	/** Returns a mutable reference to the value associated with current key. */
	reference value() { return dereference(); }

	/** Returns a const reference to the value associated with current key. */
	const_reference value() const { return dereference(); }

	/* @{ */

	/** @name iterator_wrapper interface
	 * @{
	 */
	/** Called from iterator wrappers - note that mutable reference is returned even though 
	 * method is const: the iterator wrapper const_traits policy must manage client access rights.
	 */
	reference dereference() const {
		return (const_cast<NodeList&>(*m_nodes)[m_current_node].value());
	}

	/** Seek to next end-of-string node (or set to invalid node)
	 * Precondition: current node is at_end() */
	void increment()
	{
		node_index nid = m_current_node;
		assert( nid >= (*m_nodes).size() ||
				(current_node().at_end() && current_node().at_end() != node::erased));

		// step forward, or back off from endnode
		m_current_node = iter_traits::find_forward_path(*m_nodes, m_current_node, m_current_key);
		{
			string_type tmp;
			assert( (m_current_node == node::invalid_index && m_current_key.empty()) || 
					m_current_key == key_access::retrieve_key_string(*m_nodes, m_current_node, tmp));
		}
		// go left/down to next
		m_current_node = iter_traits::find_first_endnode(*m_nodes, m_current_node, m_current_key);
		{
			string_type tmp;
			assert( (m_current_node == node::invalid_index && m_current_key.empty()) || 
					m_current_key == key_access::retrieve_key_string(*m_nodes, m_current_node, tmp));
		}
	}

	/** Seek to next end-of-string node (or set to invalid node)
	 * Precondition: current node is at_end() */
	void decrement() // Always true: bool endcheck)
	{
		node_index nid = m_current_node;
		assert(nid >= (*m_nodes).size() || 
				(current_node().at_end() && current_node().at_end() != node::erased));

		// special case for decrementing from end-node: back up to last element
		if (nid >= (*m_nodes).size()) {
			m_current_key.clear();
			if ((*m_nodes).empty())
				m_current_node = node::invalid_index;
			else
				m_current_node = iter_traits::find_last_endnode(*m_nodes, 0, m_current_key);
			return;
		}

		// back up from endnode - handles full iteration
		m_current_node = iter_traits::find_backward_path(*m_nodes, m_current_node, m_current_key);
	}

	bool equal(const tst_iterator_base& other) const {
		assert(!m_nodes || !other.m_nodes || m_nodes == other.m_nodes);
		return tree_position() == other.tree_position();
	}

	void swap(tst_iterator_base& rhs) {
		std::swap(m_nodes, rhs.m_nodes);
		std::swap(m_current_node, rhs.m_current_node);
		std::swap(m_current_key, rhs.m_current_key);
	}

	bool empty() const { return m_nodes? ((*m_nodes).size() == 0) : true; }

	node_index tree_position() const { return m_current_node; }

	/* @} */


private:

	const node& current_node() const { return nodes()[m_current_node]; }

	const NodeList& nodes() const { return *m_nodes; }

private:
	const NodeList*  m_nodes;
	node_index       m_current_node;
	string_type      m_current_key;
};


} } // end namespace containers::tst_detail


