// block direct use
#ifndef TERNARY_TREE_INCLUDE_GUARD
# error This is a dependent header file, included by ternary_tree.hpp - DO NOT USE DIRECTLY
#endif


namespace containers {
namespace tst_detail {

//---------------------------------------------------------------------------------------
// ternary_tree::tst_iterator_base class
//---------------------------------------------------------------------------------------

/** Implements forward and backward iteration in lexical order. 
 * Iteration is slightly expensive as it needs to walk nodes, and backtrack to find next value.
 * Complexity of increment is amortized constant time. Actual time taken is O(n) in average length 
 * of strings minus string common prefixes.
 *
 * tst_iterator_base is constructible from a node list and a node index; this is used 
 * with values returned from wildcard-match and neighbour-match searches.
 */
template<class StringT, class DataT, class CompT, class AllocT> 
class tst_iterator_base
{
private:
	typedef ternary_tree<StringT, DataT, CompT, AllocT>  tst;
	friend class ternary_tree<StringT, DataT, CompT, AllocT>;

	typedef typename tst::node                           node;
	typedef typename tst::NodeList                       NodeList;
public:
	typedef typename tst::mapped_type                    mapped_type;
	typedef typename tst::value_type                     value_type;
	typedef typename tst::reference                      reference;
	typedef typename tst::const_reference                const_reference;
	typedef typename tst::pointer                        pointer;
	typedef typename tst::const_pointer                  const_pointer;

	typedef typename tst::node_index                     node_index;
	typedef typename tst::key_type                       key_type;
	typedef typename tst::char_type                      char_type;
	typedef typename tst::string_type                    string_type;

	tst_iterator_base() : m_nodes(0), m_current_node(node::invalid_index) {}

	explicit tst_iterator_base(const NodeList& nodes)
		: m_nodes(&nodes), m_current_node(node::invalid_index)
	{}

	tst_iterator_base(const NodeList& nodes, node_index nid, const key_type& key, bool seekend = true)
		: m_nodes(&nodes), m_current_node(nid), m_current_key(key.begin(), key.end())
	{
		if (seekend)
			find_first_endnode();
		else
			retrieve_key_string(nid);
	}

	tst_iterator_base(const NodeList& nodes, node_index index)
		: m_nodes(&nodes), m_current_node(index)
	{
		if (index == node::invalid_index)
			return;
		if (m_nodes->empty())
			m_current_node = node::invalid_index;
		else {
			find_first_endnode();
			if (index != 0) 
				retrieve_key_string(m_current_node); //retrieve_key(m_current_node);
		}
	}

public:
	/** @name publicly usable interface
	 * @{
	 */
	/** Returns the key at current iterator position - this value changes when iterator
	 * is incremented or decremented.
	 */
	const key_type& key() const { return m_current_key; }

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
		find_forward_path();
		// go left/down to next
		find_first_endnode();
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
			if (nodes().empty())
				m_current_node = node::invalid_index;
			else
				find_last_endnode(0);
			return;
		}

		// back up from endnode - handles full iteration
		find_backward_path();
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

	bool empty() const { return m_nodes? (nodes().size() == 0) : true; }

	node_index tree_position() const { return m_current_node; }

	/* @} */

private:

	node_index node_count() const { return m_nodes? nodes().size() : 0; }

	const node& current_node() const { return nodes()[m_current_node]; }

	const node& get_node(node_index nid) const { return nodes()[nid]; }

	const NodeList& nodes() const { return *m_nodes; }

	void clear() {
		tst_iterator_base(*m_nodes).swap(*this);
	}

	/** Walk backward until find a node with untrodden path (not pointing to current subtree).
	 * Input is a node index where either [nid].at_end() OR parent.at_end() and parent.hikid == nid.
	 * Returns index to node where
	 *   (a) m_current.node = current_node().parent AND 
	 *       m_current.path = m_current.path + 1
	 *   (b) m_current.node == m_current.path == invalid_node
	 */
	node_index find_forward_path()
	{
		node_index nid = m_current_node;
		// step forward if we can
		const node& n = (*m_nodes)[nid];
		if (n.hikid < (*m_nodes).size()) {
			//assert(m_nodes[nid].at_end() == node::terminal);
			if (n.at_end() != node::terminal)
				m_current_key.pop_back();
			m_current_node = n.hikid;
			return m_current_node;
		}
		// back off from endnode
		while(1) {
			//assert(current_node().hikid > (*m_nodes).size());
			if (nid == 0) {
				assert(m_current_key.empty());
				nid = node::invalid_index;
				break; 
			}
			node_index parent_nid = (*m_nodes)[nid].parent;
			const node& pn = (*m_nodes)[parent_nid];
			// (No special handling for erased needed, it's always hikid==nid so we go to parent)
			// Check which node pointed to us and select
			if (pn.lokid == nid) {
				m_current_key.push_back( pn.splitchar() );
				nid = parent_nid + 1;
				break;
			}
			else if (pn.hikid < (*m_nodes).size()) {
				if (pn.hikid != nid) {
					m_current_key.pop_back();
					nid = pn.hikid;
					break;
				}
				nid = parent_nid;
				continue;
			}
			nid = parent_nid;
			m_current_key.pop_back();
		}

		m_current_node =  nid;
		return nid;
	}

	/** Starting from node, walk rightmost path to next terminal node -
	 * takes us from root to end-value in log time --
	 * Pre: tree is not empty(); Post: m_nodes[nid].at_end() == node::terminal
	 */
	node_index find_last_endnode(node_index nid)
	{
		while (1)
		{
			const node& n = (*m_nodes)[nid];
			// (since erased always has hikid, no special handling needed)
			if (n.hikid < (*m_nodes).size())
				nid = n.hikid;
			else {
				if (n.at_end()) {
					assert(n.at_end() != node::erased);
					break;
				}
				++nid;
				m_current_key.push_back(n.splitchar());
			}
		}
		assert((*m_nodes)[nid].at_end() == node::terminal);
		m_current_node = nid;
		return nid;
	}

	/** Starting from node, walk lowest or eq path to next terminal node
	 * NOTE: does not increment if nid indexes an end-node!
	 */
	node_index find_first_endnode() 
	{
		node_index nid = m_current_node;
		while (nid < (*m_nodes).size())
		{
			const node& n = (*m_nodes)[nid];
			if (n.at_end()) {
				if (n.at_end() == node::erased) {
					// bypass erased node
					nid = n.hikid;
					continue;
				}
				break;
			}
			if (n.lokid < (*m_nodes).size())
				nid = n.lokid;
			else {
				++nid;
				m_current_key.push_back( n.splitchar() );
			}
		}
		m_current_node = nid;
		return nid;
	}

	//! Examine parent(s) to find next decrementable path not leading here.
	node_index find_backward_path()
	{
		node_index nid = m_current_node;
		while(1) {
			//assert(current_node().hikid > (*m_nodes).size());
			if (nid == 0) {
				return m_current_node = node::invalid_index;
			}
			node_index parent_nid = (*m_nodes)[nid].parent;
			const node& pn = (*m_nodes)[parent_nid];
			if (pn.at_end()) {
				if (pn.at_end() != node::erased && pn.hikid == nid) {
					nid = parent_nid;
					break;
				}
				nid = parent_nid;
				continue;
			}
			// Check which node pointed to us and select
			if (pn.hikid == nid) {
				m_current_key.push_back(pn.splitchar());
				return find_last_endnode(parent_nid + 1);
			}
			else if (pn.lokid < (*m_nodes).size()) {
				if (pn.lokid != nid) {
					m_current_key.pop_back();
					return find_last_endnode(pn.lokid);
				}
				nid = parent_nid;
				continue;
			}
			nid = parent_nid;
			m_current_key.pop_back();
		}

		m_current_node = nid;
		return nid;
	}

	//! Collects all characters until reaching a branch in node tree (parent != self - 1)
	node_index retrieve_eq_chars(string_type& key, node_index nid)
	{
		assert(nid < (*m_nodes).size());
		node_index parent_nid = (*m_nodes)[nid].parent;
		while (parent_nid < (*m_nodes).size())
		{
			const node& n = (*m_nodes)[parent_nid];
			if (n.hikid == nid || (!n.at_end() && n.lokid == nid))
				break;
			key.push_back(n.splitchar());
			nid = parent_nid;
			parent_nid = n.parent;
		}
		return nid;
	}

	//! Skips over branches in node tree, stopping when parent == self - 1
	node_index skip_non_eq_nodes(node_index nid) const
	{
		assert(nid < (*m_nodes).size());
		node_index parent_nid = (*m_nodes)[nid].parent;
		while (parent_nid < (*m_nodes).size())
		{
			const node& n = (*m_nodes)[parent_nid];
			if (n.hikid != nid && (n.at_end() || n.lokid != nid))
				break;
			nid = parent_nid;
			parent_nid = (*m_nodes)[parent_nid].parent;
		}
		return nid;
	}

	/** Recover key string by walking nodes backward, starting from an end-node.
	 * Complexity is key length + log(tree size).
	 */
	string_type& retrieve_key_string(node_index nid)
	{
		m_current_key.clear();
		string_type& temp = m_current_key;
		while(nid) {
			nid = skip_non_eq_nodes(nid);
			nid = retrieve_eq_chars(temp, nid);
		}
		std::reverse(temp.begin(), temp.end());
		return temp;
	}

private:
	const NodeList*  m_nodes;
	node_index       m_current_node;
	string_type      m_current_key;
};


} } // end namespace containers::tst_detail




