// Created by rasmus ekman - Mon Dec 18 12:19:57 2006
#ifndef ITERATION_IMPL_HPP_INCLUDED
#define ITERATION_IMPL_HPP_INCLUDED

// for back_insert_iterator
#include <iterator>

namespace containers {
namespace tst_detail {

//---------------------------------------------------------------------------------------
// Helper classes for key char insertion
//---------------------------------------------------------------------------------------

//! A do-nothing BackInsertionSequence
template<class ContainerT>
struct dummy_sequence
{
	typedef typename ContainerT::value_type       value_type;
	typedef typename ContainerT::reference        reference; 
	typedef typename ContainerT::const_reference  const_reference;
	void push_back(const value_type& ) { return; }
	bool empty() const { return true; }
};

//! Extend a back_insert_iterator with postfix operator--, which pops back
//! (this is required to exist for back_insert_sequence:s).
template<class BackInsertSeqT>
struct back_push_pop : public std::back_insert_iterator<BackInsertSeqT>
{
	typedef std::output_iterator_tag  iterator_category;
	using std::back_insert_iterator<BackInsertSeqT>::container;
	
	explicit back_push_pop(BackInsertSeqT& c) : std::back_insert_iterator<BackInsertSeqT>(c) {}
	back_push_pop& operator--(int) { container->erase(container->end()-1); return *this; }
	template<class T>
	back_push_pop& operator=(T& val) {
		container->push_back(val);
		return *this;
	}
};


//---------------------------------------------------------------------------------------
//! Seek from node towards root of tree, to extract chars of key that the node represents.
//---------------------------------------------------------------------------------------

template<class NodeListT, class StringT>
struct key_access
{
	typedef StringT                         string_type;
	typedef typename NodeListT::size_type   node_index;
	typedef typename NodeListT::value_type  node;

	/** Recover key string by walking nodes backward, starting from an end-node.
	 * Complexity is key length + log(tree size).
	 */
	static string_type& retrieve_key_string(const NodeListT& nodes, node_index nid, string_type& key)
	{
	//	key.clear();
		while(nid) {
			nid = skip_non_eq_nodes(nodes, nid);
			nid = retrieve_eq_chars(nodes, nid, key);
		}
		std::reverse(key.begin(), key.end());
		return key;
	}

	//! Skips over branches in node tree, stopping when parent == self - 1
	static node_index skip_non_eq_nodes(const NodeListT& nodes, node_index nid)
	{
		assert(nid < nodes.size());
		node_index parent_nid = nodes[nid].parent;
		while (parent_nid < nodes.size())
		{
			const node& n = nodes[parent_nid];
			if (n.hikid != nid && (n.at_end() || n.lokid != nid))
				break;
			nid = parent_nid;
			parent_nid = nodes[parent_nid].parent;
		}
		return nid;
	}

private:

	//! Collects all characters until reaching a branch in node tree (parent != self - 1)
	static node_index retrieve_eq_chars(const NodeListT& nodes, node_index nid, string_type& key)
	{
		assert(nid < nodes.size());
		node_index parent_nid = nodes[nid].parent;
		while (parent_nid < nodes.size())
		{
			const node& n = nodes[parent_nid];
			if (n.hikid == nid || (!n.at_end() && n.lokid == nid))
				break;
			key.push_back(n.splitchar());
			nid = parent_nid;
			parent_nid = n.parent;
		}
		return nid;
	}

};


//---------------------------------------------------------------------------------------
//! tst in-order iterator
//---------------------------------------------------------------------------------------

/** Implements tree in-order bidirectional iteration. If the optional template 
 * parameter \c KeyTrackerT is provided, it is a BackInsertionSequence that accepts 
 * \c NodeListT \c node::char_type values.
 * (back insertables are eg std::string, vector etc, but here the only methods used 
 * are \c push_back and \c empty )
 */
template<class NodeListT, class StringT> 
struct inorder_seek
{
	typedef typename NodeListT::value_type  node;
	typedef typename NodeListT::size_type   node_index;
	typedef StringT                         string_type;
	//typedef key_access<NodeList, StringT>   key_access;

//-------------------------------------------------------------------------------------------------

	/** Starting from node \a pos, walk lowest or eq path to next terminal node
	 * NOTE: does not increment if \a pos indexes an end-node!
	 */
	static node_index find_first_endnode(const NodeListT& nodes, node_index pos, string_type& str)
	{
		back_push_pop<string_type> out(str);
		node_index nid = pos;
		while (nid < nodes.size())
		{
			const node& n = nodes[nid];
			if (n.at_end()) {
				if (n.at_end() == node::erased) {
					// bypass erased node
					nid = n.hikid;
					continue;
				}
				break;
			}
			if (n.lokid < nodes.size())
				nid = n.lokid;
			else {
				++nid;
				*out++ = n.splitchar(); //key.push_back( n.splitchar() );
			}
		}
		return nid;
	}

//-------------------------------------------------------------------------------------------------

	/** Starting from node \a pos, walk rightmost path to next terminal node -
	 * takes us from root to end-value in log time --
	 * Pre: tree is not empty(); Post: m_nodes[nid].at_end() == node::terminal
	 */
	static node_index find_last_endnode(const NodeListT& nodes, node_index pos, string_type& str)
	{
		back_push_pop<string_type> out(str);
		node_index nid = pos;
		while (1)
		{
			const node& n = nodes[nid];
			// (since erased always has hikid, no special handling needed)
			if (n.hikid < nodes.size())
				nid = n.hikid;
			else {
				if (n.at_end()) {
					assert(n.at_end() != node::erased);
					break;
				}
				++nid;
				*out++ = n.splitchar(); //key.push_back(n.splitchar());
			}
		}
		assert(nodes[nid].at_end() == node::terminal);
		return nid;
	}

//-------------------------------------------------------------------------------------------------

	//! Examine parent(s) to find next decrementable path not leading here.
	//! Tracks current key value by push_back and erase-end when descending/ascending tree.
	static node_index find_forward_path(const NodeListT& nodes, node_index pos, string_type& str)
	{
		back_push_pop<string_type> out(str);
		node_index nid = pos;
		// step forward if we can
		const node& n = nodes[nid];
		if (n.hikid < nodes.size()) {
			//assert(nodes[nid].at_end() == node::terminal);
			if (n.at_end() != node::terminal)
				out--; //key.pop_back();
			return n.hikid;
		}
		// back off from endnode
		while(1) {
			//assert(current_node().hikid > nodes.size());
			if (nid == 0) {
				assert(str.empty());
				nid = node::invalid_index;
				break; 
			}
			node_index parent_nid = nodes[nid].parent;
			const node& pn = nodes[parent_nid];
			// (No special handling for erased needed, it's always hikid==nid so we go to parent)
			// Check which node pointed to us and select
			if (pn.lokid == nid) {
				*out++ = pn.splitchar(); //key.push_back( pn.splitchar() );
				nid = parent_nid + 1;
				break;
			}
			else if (pn.hikid < nodes.size()) {
				if (pn.hikid != nid) {
					out--; //key.pop_back();
					nid = pn.hikid;
					break;
				}
				nid = parent_nid;
				continue;
			}
			nid = parent_nid;
			out--; //key.pop_back();
		}
		return nid;
	}

//-------------------------------------------------------------------------------------------------

	/** Walk backward until find a node with untrodden path (not pointing to current subtree).
	 * Input is a node index where either [nid].at_end() OR parent.at_end() and parent.hikid == nid.
	 * Returns index to node where
	 *   (a) m_current.node = current_node().parent AND 
	 *       m_current.path = m_current.path + 1
	 *   (b) m_current.node == m_current.path == invalid_node
	 *
	 * Tracks current key value by push_back and pop_back when descending/ascending tree.
	 */
	static node_index find_backward_path(const NodeListT& nodes, node_index pos, string_type& str)
	{
		back_push_pop<string_type> out(str);
		node_index nid = pos;
		while(1) {
			//assert(current_node().hikid > nodes.size());
			if (nid == 0) {
				return node::invalid_index;
			}
			node_index parent_nid = nodes[nid].parent;
			const node& pn = nodes[parent_nid];
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
				*out++ = pn.splitchar(); //key.push_back(pn.splitchar());
				return find_last_endnode(nodes, parent_nid + 1, str);
			}
			else if (pn.lokid < nodes.size()) {
				if (pn.lokid != nid) {
					out--; //key.pop_back();
					return find_last_endnode(nodes, pn.lokid, str);
				}
				nid = parent_nid;
				continue;
			}
			nid = parent_nid;
			out--; //key.pop_back();
		}

		return nid;
	}
};


} } // end namespace containers::tst_detail


#endif  // ITERATION_IMPL_HPP_INCLUDED
