// block direct use
#ifndef TERNARY_TREE_INCLUDE_GUARD
# error This is a dependent header file, included by ternary_tree.hpp - DO NOT USE DIRECTLY
#endif


namespace containers {


//------------------------------------------------------------------------------
// ternary_tree implementation methods
//------------------------------------------------------------------------------


/** Ternary tree structure
 * The insert_impl method defines primary tree structure. 
 * The difference from previous contents is inserted as a "twig", a stretch of nodes,
 * and linked from the longest matching prefix.
 * The resulting invariant is: 
 *    if a char in search string matches a node char, there exists an adjacent next 
 *    node with another char to test.
 *    The only exception is terminal nodes, ending a key, which have a special marker 
 *    that ensures they cannot match any char in any possible key. 
 *
 * find and iteration are defined to work with the resulting structure as best they can...
 *
 * This is mentioned due to the problem with erased nodes: When strings are erased,
 * the invariant is broken, and must be mended with a special case: the erased-endnode.
 * This is an endnode with a special marker. 
 * It must be silently bypassed by all parties, always taking the same path.
 */
template<class S, class D, class C, class A>
template<class CharIter>
inline typename ternary_tree<S, D, C, A>::find_result
ternary_tree<S, D, C, A>::find_impl(CharIter& first, CharIter last) const
{
	if (empty())
		return find_result();

#if TST_USE_FASTMAP
	node_index startnode = (first != last && unsigned(*first) < m_fastmap.size())?
							m_fastmap[unsigned(*first)] : 0;
#else
	static const node_index startnode = 0;
#endif  // TST_USE_FASTMAP

	// set initial node index so pnid has something to point to before any node found
	node_index fake_eqkid = startnode; 
	const node_index * pnid = &fake_eqkid;

	find_result result;

	while(*pnid < node_count())
	{
		const node& n = m_nodes[*pnid];
		result.nid = *pnid;
		if (n.at_end())
		{
			if (first == last && n.at_end() != node::erased)
				return find_result(*pnid, 0);
			// If found endnode, go to hikid since shorter string precedes in sort order
			pnid = &n.hikid;
			continue;
		}
		if (first == last) {
			// if search string ended, spin to next free lokid
			// since shorter string precedes in sort order.
			pnid = &n.lokid;
			continue;
		}
		char_type c = *first;
		if ( less(c, n.splitchar()) )
			pnid = &n.lokid;
		else if ( less(n.splitchar(), c) )
			pnid = &n.hikid;
		else { //if ( equal(c, n.splitchar()) ) {
			++first;
			// this is never the last node, no worry that we return pointer to stack
			fake_eqkid = (*pnid) + 1;
			pnid = &fake_eqkid;
		}
	}
	assert(pnid != &fake_eqkid);
	result.pnid = const_cast<node_index*>(pnid);
	return result;
}


//---------------------------------------------------------------------------------------
// insert_append
//---------------------------------------------------------------------------------------
namespace util
{
	//! vector::capacity and reserve don't operate in 2^x steps, so implement here 
	template<class VecT>
	inline void 
	reserve(VecT& v, typename VecT::size_type require)
	{
		size_t capacity = v.capacity();
		if (capacity < require) {
			// initial 16: Don't fiddle with micro-trees?
			size_t request = 16;
			while (request < require)
				request <<= 1;
			v.reserve(request);
		}
	}
}

template<class S, class D, class C, class A>
template<class CharIter>
inline typename ternary_tree<S, D, C, A>::node_index
ternary_tree<S, D, C, A>::insert_append(find_result& found, CharIter first, CharIter last)
{
	size_type prevsize = m_nodes.size();
	bool updatelast = found.nid == 0 || found.nid > node_count();
	// Watch out, endnode creation is smeared over whole function...
	node endnode(0, node::invalid_index);
	try {
		if (!updatelast && found.nid < node_count()) {
			updatelast = m_nodes[found.nid].hikid == *found.pnid;
		}
		// Must grab parent before writing to pnid: the root node pnid points to nid!
		node_index parent = found.nid;
		// must assign to pointer-to-kid before possible reallocation (in m_nodes.reserve())
		node_index nextindex = prevsize;
		if (found.pnid) {
			//m_nodes[*found.pnid].hikid == found.nid;
			*found.pnid = nextindex;
		}

		// If we're gonna throw, throw now, so allocate endnode (on heap if pointer).
		endnode.m_atend = 1;
		endnode.assign(mapped_type(), m_allocator);
		// ... and get NodeList storage
		util::reserve(m_nodes, prevsize + (last - first) + 1);

		// Append unconsumed key chars
		while(first != last) {
			m_nodes.push_back(node(*first++, parent));
			parent = nextindex++;
		}
		// set parent and append endnode
		endnode.parent = parent;
		m_nodes.push_back(endnode);

		//tst_iterator_base ib(m_nodes, 0);
		//ib.find_last_endnode(0);
		// The "dummy_string" is a hack to avoid pushing chars in internal iterators.
		typedef tst_detail::dummy_sequence<std::basic_string<char_type> >  dummy_string;
		dummy_string voidstring;
		node_index endnid = 
			tst_detail::inorder_seek<NodeList, dummy_string>::find_last_endnode(m_nodes, 0, voidstring);
		if (m_last_item.tree_position() != endnid) { //ib.tree_position()) {
			assert(updatelast);  // some time we replace the test above with this
			m_last_item = tst_iterator_base(m_nodes, endnid);
		}

		// update to reflect state
		found.nid = nextindex;
		found.pnid = 0;

	// since we use reserve() we can get both length_error and bad_alloc.
	} catch (std::exception&) {
		// In case reserve failed after successful allocation
		endnode.clear_data(m_allocator);
		// Only rollback needed: erase path from found node to this twig
		if (found.pnid)
			*found.pnid = node::invalid_index;
		throw;
	}
	return found.nid;
}

//---------------------------------------------------------------------------------------
// erase_impl
//---------------------------------------------------------------------------------------

namespace tst_erase_impl_detail {

	template<class Node>
	bool lokid_link(const Node& n) {
		return !n.at_end() && n.lokid != size_t(-1);
	}
	template<class Node>
	bool hikid_link(const Node& n) {
		return n.hikid != size_t(-1);
	}
	template<class Node>
	size_t count_branches(const Node& n) {
		return hikid_link(n) + lokid_link(n);
	}
	template<class Node>
	size_t is_branch(const Node& n) {
		return hikid_link(n) || lokid_link(n);
	}

} // namespace tst_detail

/** Scan twig backward looking for branches, unlink all nodes that are not used.
 * Three distinct cases:
 *  -# zero branches: twig can be completely unlinked; the hikid/lokid pointer in twig root 
 *     is set to invalid_index. Check that twig_root is still used, else erase it recursively.
 *  -# one branch in all of twig. This in turn has two cases:
 *     - branch occurs in first twig node: relink from twig_root.hi/lokid 
 *       directly to branched node, then unlink twig.
 *     - branch occurs in subsequent twig node: close off branch node and erase subsequent twig nodes.
 *       If branch is in lokid, set branch node splitchar to zero and move link to hikid.
 *  -# two or more branches: Just mark branch node erased.
 *
 * Finally, if erased nodes happen to be at very end of node vector, erase them from vector.
 * If we wanted even more trouble, we could try to swap the erased nodes with last twig in tree.
 *
 * This is complicated, somewhat brittle code.
 */
template<class S, class D, class C, class A>
typename ternary_tree<S, D, C, A>::node_index
ternary_tree<S, D, C, A>::erase_impl(node_index nid, size_type keylen)
{
	using namespace tst_erase_impl_detail;
	// should assert?
	if (nid >= node_count())
		return node::invalid_index;

	// tree stats not changed on recursive erase calls
	static const size_type invalid_key_len = size_type(-1);
	if (keylen != invalid_key_len) {
		m_item_count--;
		m_total_key_length -= keylen;
	}

	const node_index endnode = nid;
	// "free" data at endnode - Post: lokid is invalid_index, so no false branching
	m_nodes[endnode].clear_data(m_allocator);

	// get twig info: root, branches...
	node_index twig_first = endnode;
	node_index twig_root = node::invalid_index;
	node_index last_branch = node::invalid_index;
	size_t last_branch_count = 0;
	size_t branch_count = 0;
	while(twig_first < node_count())
	{
		const node& n = m_nodes[twig_first];
		assert(!n.is_unlinked() || (keylen == invalid_key_len && twig_first == endnode));
		// Special case: previous node (in backtracking) is a child, not continuation
		if (twig_first < endnode && n.hikid == twig_first + 1 || (n.lokid == twig_first + 1 && !n.at_end())) {
			twig_root = twig_first;
			twig_first++;
			break;
		}
		// count branches at each node
		if (is_branch(n)) {
			// record last twig branch
			if (last_branch >= node_count()) {
				branch_count = last_branch_count = count_branches(n);
				last_branch = twig_first;
			}
			else {   // continue count for rest of twig
				branch_count += count_branches(n);
			}
		}
		if (n.parent != twig_first - 1) {
			twig_root = n.parent;
			break;
		}
		--twig_first;
	}

	// Major decision: either we can just relink from twig_root (0 branches, or twig_first has single branch),
	// or some twig nodes are used, so must make an erased end-node
	if (branch_count == 0 || (twig_first == last_branch && branch_count == 1))
	{
		node_index branch = node::invalid_index;

		// simplest case: clear or relink root pointer
		if (branch_count == 1 && twig_root < node_count()) {
			node& bn = m_nodes[last_branch];
			//assert(!bn.at_end());
			branch = (bn.hikid < node_count())? bn.hikid : bn.lokid;
			bn.m_char = 0;
			bn.m_atend = node::erased;
			bn.hikid = bn.lokid = node::invalid_index;
			m_nodes[branch].parent = twig_root;
		}
		// relink root to point to branch
		if (twig_root < node_count()) {
			node& pn = m_nodes[twig_root];
			if (pn.hikid == twig_first)
				pn.hikid = branch;
			else {
				assert(pn.lokid == twig_first && !pn.at_end());
				pn.lokid = branch;
			}
			// Need to chop more branches if this became a dead end
			if (pn.is_unlinked())
				erase_impl(twig_root, invalid_key_len);
		}
	}
	else {
		node& bn = m_nodes[last_branch];
		// Enforce invariant: move last_branch link to hikid branch, and clear lokid
		if (last_branch_count == 1)
		{
			assert(bn.lokid < node_count() != bn.hikid < node_count());
			if (bn.at_end() != node::terminal && bn.lokid < node_count()) {
				bn.hikid = bn.lokid;
				bn.lokid = node::invalid_index;
			}
		}
		else {
			// move lokid to hikid branch - just search hikid's lokids until empty is found
			node_index hikid = bn.hikid;
			while (lokid_link(m_nodes[hikid])) {
				hikid = m_nodes[hikid].lokid;
				assert(m_nodes[hikid].splitchar() > m_nodes[bn.lokid].splitchar());
			}
			m_nodes[hikid].lokid = bn.lokid;
			m_nodes[bn.lokid].parent = hikid;
			bn.lokid = node::invalid_index;
		}
		// rewrite node
		bn.m_atend = node::erased;
		bn.m_char = 0;
	}

	// wipe unused nodes : never wipe root
	if (last_branch >= node_count())
		last_branch = (twig_first > 1 && twig_first < node_count())? twig_first : 1;

	node_index erasable = endnode;
	while(erasable > last_branch) {
		node& n = m_nodes[erasable];
		assert(n.m_atend != node::erased || keylen == invalid_key_len);
		n.m_atend = node::erased;
		n.m_char = 0;
		n.hikid = n.lokid = node::invalid_index;
		--erasable;
	}
	// now close off last_branch
	node& lbn = m_nodes[erasable];
	lbn.m_atend = node::erased;
	lbn.m_char = 0;  // is this right?

	// finally, see if we can free nodes at end
	if (endnode == node_count() - 1)
	{
		size_type i = endnode;
		while(i) {
			const node& n = m_nodes[i];
			if (!n.is_unlinked()) {
				++i;
				break;
			}
			--i;
		}
		if (i <= endnode)
			m_nodes.erase(m_nodes.begin() + i, m_nodes.end());
	}

	if (m_last_item.tree_position() >= node_count() || 
		m_nodes[m_last_item.tree_position()].at_end() != node::terminal)
	{
		//m_last_item.clear();
		if (!m_nodes.empty()) {
			tst_iterator_base ib(m_nodes, 0);
			//node_index lid = ib.find_last_endnode(0);
			node_index lid = ib.tree_position();
			tst_iterator_base ib2(m_nodes, lid);
			m_last_item.swap(ib2);
		}
	}

	return last_branch < node_count()? last_branch : twig_root;
}


//---------------------------------------------------------------------------------------
// prefix_range implementation
//---------------------------------------------------------------------------------------

template<class S, class D, class C, class A>
template<class CharIter>
std::pair<typename ternary_tree<S, D, C, A>::const_iterator,
		  typename ternary_tree<S, D, C, A>::const_iterator>
ternary_tree<S, D, C, A>::prefix_range_impl(CharIter first, CharIter last) const
{
	CharIter begin = first;
	find_result found = find_impl(first, last);
	// If all prefix chars not consumed, no such strings exist
	if (first != last)
		return std::make_pair(end(), end());
	// we're at the first node past prefix. 
	iterator firstIter(tst_iterator_base(m_nodes, found.nid));

	// the second iterator just differs in final char (aaa... -> aab...)

	//tst_iterator_base ib(m_nodes);
	//ib.skip_non_eq_nodes(found.nid);
	// go to last matched char
	node_index nid = tst_detail::key_access<NodeList, string_type>::skip_non_eq_nodes(m_nodes, found.nid);
	// pop it
	node_index prevnid = nid;
	nid = (nid < node_count())? m_nodes[nid].parent : node::invalid_index;
	//node_index simplerend = ib2.find_first_endnode();
	while(nid < node_count())
	{
		const node& n = m_nodes[nid];

		// find forward prefix path 
		if (n.lokid == prevnid && !n.at_end()) {
			return std::make_pair(firstIter, tst_iterator_base(m_nodes, nid+1));
		}
		if (n.hikid != prevnid) {
			if (n.hikid < node_count()) {
				return std::make_pair(firstIter, tst_iterator_base(m_nodes, n.hikid));
			}
		}
		// else we just need a non-eq higher value
		prevnid = nid;
		nid = n.parent;
	}
	// not found
	return std::make_pair(firstIter, end());
}


//---------------------------------------------------------------------------------------
// longest_match implementation
//---------------------------------------------------------------------------------------

template<class S, class D, class C, class A>
template<class CharIter>
typename ternary_tree<S, D, C, A>::node_index
ternary_tree<S, D, C, A>::longest_match_impl(CharIter & first, CharIter last) const
{
	if (first == last)
		return node::invalid_index;
	CharIter f0 = first;
	find_result found = find_impl(f0, last);
	// Not sure which of these actually detect non-match?
	// First and last do. Is last correct?
	if (f0 == first || found.nid == 0 || found.nid >= node_count() || f0 == last && found.pnid) {
		return node::invalid_index;
	}
	// We may have matched more chars than the longest actual *key*
	// So we must look for endnode, and backup until one is found.

	// Found endnode?
	const node& hitnode = m_nodes[found.nid];
	if (hitnode.at_end() == node::terminal) {
		return found.nid;
	}
	// else: check for lokid, to find possible endnode
	node_index nid = hitnode.lokid < node_count()? found.nid : hitnode.parent;
	while(nid < node_count())
	{
		// Check if we can find an endnode here
		node_index testnid = nid;
		do {
			const node& n = m_nodes[testnid];
			if (n.at_end()) {
				if (n.at_end() == node::terminal)
					return testnid;
				// erased node has no lokids, so go to parent
				break;
			}
			testnid = n.lokid;
		} while(testnid < node_count());
		const node& n = m_nodes[nid];
		nid = n.parent;
	}
	return node::invalid_index;
}

//---------------------------------------------------------------------------------------
// partial_match_search implementation
//---------------------------------------------------------------------------------------

//! Repeats code from find_impl, recursive over hi-lo-eqkid when a wildcard char is matched.
template<class S, class D, class C, class A>
template<class InputIter, class OutputIter>
inline void
ternary_tree<S, D, C, A>::partial_match_search_impl ( node_index nid
													, InputIter first, InputIter last
													, OutputIter results
													, char_type wildcard ) const 
{
	while(nid < node_count())
	{
		const node& n = m_nodes[nid];
		if (n.at_end())
		{
			if (first == last && n.at_end() != node::erased) {
				*results++ = nid; 
				return;
			}
			nid = n.hikid;
			continue;
		}
		if (first == last) {
			// if search string ended, spin to lokid
			// in case there's an endnode somewhere
			nid = n.lokid;
			continue;
		}
		char_type c = *first;
		if (c == wildcard) {
			// recursively match all values in subtree
			if (n.lokid < node_count())
				partial_match_search_impl(n.lokid, first, last, results, wildcard);
			InputIter next = first;
			partial_match_search_impl(nid + 1, ++next, last, results, wildcard);
			if (n.hikid < node_count())
				partial_match_search_impl(n.hikid, first, last, results, wildcard);
			return;
		}
		else if ( less(c, n.splitchar()) )
			nid = n.lokid;
		else if ( less(n.splitchar(), c) )
			nid = n.hikid;
		else {  // c == n.splitchar
			++first;
			++nid;
		}
	}
	return;
}


//---------------------------------------------------------------------------------------
// hamming_search implementation
//---------------------------------------------------------------------------------------

template<class S, class D, class C, class A>
template<class InputIter, class OutputIter>
inline void
ternary_tree<S, D, C, A>::hamming_search_impl( node_index nid
											 , InputIter first, InputIter last
											 , OutputIter results
											 , int dist) const
{
	while(nid < node_count())
	{
		const node& n = m_nodes[nid];
		if (n.at_end())
		{
			// strlen of non-matched remainder <= dist
			if (last - first <= dist && n.at_end() != node::erased) {
				*results++ = nid; 
			}
			nid = n.hikid;
			continue;
		}
		if (dist > 0)
		{
			if (n.lokid < node_count())
				hamming_search_impl(n.lokid, first, last, results, dist);
			bool nomatch = (first == last)
						|| less(*first, n.splitchar())
						|| less(n.splitchar(), *first);
			InputIter next = first;
			if (first != last)
				++next;
			hamming_search_impl(nid + 1, next, last, results, dist - nomatch);
			if (n.hikid < node_count())
				hamming_search_impl(n.hikid, first, last, results, dist);
			return;
		}

		if (first == last) {
			// if search string ended, spin to lokid
			// in case there's an endnode somewhere
			nid = n.lokid;
			continue;
		}

		char_type c = *first;
		if ( less(c, n.splitchar()) ) {
			nid = n.lokid;
		}
		else if ( less(n.splitchar(), c) ) {
			nid = n.hikid;
		}
		else {  // c == n.splitchar
			++first;
			++nid;
		}
	}
}


//---------------------------------------------------------------------------------------
// levenshtein_search implementation
//---------------------------------------------------------------------------------------

/** \def EXHAUSTIVE_LEVENSHTEIN_ALGO 
 * Define this to do complete recursive tree search along all levenshtein "edit" paths, 
 * to generate all matches (and massive duplicates), pass through a (structured) set
 * to filter out duplicates, then use this to compare with result of the optimized version.
 * By default undefined.
 */
#ifndef EXHAUSTIVE_LEVENSHTEIN_ALGO

namespace tst_detail {

	template<class InputIter>
	struct levenshtein_search_info
	{
		levenshtein_search_info() : have_pre(false), maxdist(0) {}

		// readonly by clients
		bool have_pre;
		int maxdist;
	
		struct search
		{
			InputIter pre, first;
			int dist;
			search(InputIter it, int d)
				: pre(it)
				, first(it)
				, dist(d)
			{}
			search(InputIter pit, InputIter it, int d)
				: pre(pit)
				, first(it)
				, dist(d)
			{}
			bool operator<=(const search& other) const {
				return pre == other.pre
					&& first == other.first
					&& dist <= other.dist;
			}
		};
		typedef typename std::vector<search>::iterator iterator;
		iterator begin() { return m_searches.begin(); }
		iterator end()   { return m_searches.end(); }
	
		size_t size() const { return m_searches.size(); }
		bool empty() const { return m_searches.empty(); }
	
		void push_back(search& s)
		{
			// Filter identical searches (keep best)
			for (iterator sit = begin(); sit != end(); ++sit)
			{
				search& curr(*sit);
				if (s <= curr)
					return;
				if (curr <= s) {
					curr.dist = s.dist;
					return;
				}
			}
			have_pre = have_pre || (s.pre != s.first);
			if (s.dist > maxdist)
				maxdist = s.dist;
			m_searches.push_back(s);
		}
	private:
		std::vector<search> m_searches;
	};

} // end namespace tst_detail

// note: ifNdef above, so this is the optimized version
template<class S, class D, class C, class A>
template<class InputIter, class OutputIter>
inline bool
ternary_tree<S, D, C, A>::levenshtein_search_impl( node_index nid
												 , tst_detail::levenshtein_search_info<InputIter>& info
												 , InputIter last
												 , OutputIter results) const
{
	typedef tst_detail::levenshtein_search_info<InputIter>  lev_info;
	typedef typename lev_info::search                       lev_search;
	typedef typename lev_info::iterator                     lev_iter;
	
	bool descend = info.maxdist > 0 || info.size() > 1 || info.have_pre;
	InputIter first = (*info.begin()).first;

	while(nid < node_count())
	{
		const node& n = m_nodes[nid];
		// Check pre-iter even if dist=0
		if (descend)
		{
			if (n.at_end())
			{
				// strlen of non-matched remainder <= dist
				if (n.at_end() != node::erased)
				{
					for (lev_iter sit = info.begin(); sit != info.end(); ++sit)
					{
						lev_search look(*sit);
						if (last - look.first <= look.dist) {
							*results++ = nid;
							break;
						}
					}
				}
				nid = n.hikid;
				continue;
			}

			if (n.lokid < node_count())
				levenshtein_search_impl(n.lokid, info, last, results);

			lev_info nextinfo;
			for (lev_iter sit = info.begin(); sit != info.end(); ++sit)
			{
				lev_search look(*sit);
				// match earlier unmatched chars in search string
				InputIter iter = look.first;
				while (iter != look.pre)
				{
					--iter;
					char_type p = *iter;
					bool match = !(less(p, n.splitchar()) || less(n.splitchar(), p));
					if (match) {
						//matchinfo.first = iter;
						lev_search s(iter + 1, look.dist);
						nextinfo.push_back(s);
						//levenshtein_search_impl(nid + 1, iter + 1, iter + 1, last, results, dist);  // */
					}
				}

				// match current char
				bool match_here = false;
				if (look.first != last)
				{
					char_type c = *look.first;
					match_here = !(less(c, n.splitchar()) || less(n.splitchar(), c));
					if (match_here) {
						//matchinfo.first = first;
						lev_search s(look.first + 1, look.dist);
						nextinfo.push_back(s);
						//levenshtein_search_impl(nid + 1, first + 1, first + 1, last, results, dist);
					}
				}
				// try step ahead, for replacment or skipped chars
				if (look.dist > 0 && !match_here)
				{
					InputIter next = look.first + (look.first != last);
					// no match, so search window stays at pre
					lev_search ps(look.pre, next, look.dist - 1);
					nextinfo.push_back(ps);
					//levenshtein_search_impl(nid + 1, pre, next, last, results, dist - 1);
				}
				// check for match of later chars in search string
				int postdist = 1;
				iter = look.first + (look.first != last);
				while (postdist <= look.dist && iter != last)
				{
					char_type c = *iter;
					bool match = !(less(c, n.splitchar()) || less(n.splitchar(), c));
					if (match) {
						// Cost of matching by skipping chars is met immediately
						lev_search s(iter + 1, look.dist - postdist);
						nextinfo.push_back(s);
						//levenshtein_search_impl(nid + 1, iter + 1, iter + 1, last, results, dist - postdist);  // */
					}
					++postdist;
					++iter;
				}
			}
			
			// Pass all searches to mid-tree
			if (!nextinfo.empty())
				levenshtein_search_impl(nid + 1, nextinfo, last, results);

			if (n.hikid < node_count())
				levenshtein_search_impl(n.hikid, info, last, results);
			return true;
		}

		// stop trying to match previous chars
		descend = false;

		if (n.at_end())
		{
			// strlen of non-matched remainder <= dist
			if (n.at_end() != node::erased)
			{
				if (last == first)
					*results++ = nid;
			}
			nid = n.hikid;
			continue;
		}

		if (first == last) {
			// if search string ended, spin to lokid
			// in case there's an endnode somewhere
			nid = n.lokid;
			continue;
		}

		char_type c = *first;
		if ( less(c, n.splitchar()) ) {
			nid = n.lokid;
		}
		else if ( less(n.splitchar(), c) ) {
			nid = n.hikid;
		}
		else {  // c == n.splitchar
			++first;
			++nid;
		}
	}
	return true;
}


#else // EXHAUSTIVE_LEVENSHTEIN_ALGO

// This is the expensive exhaustive search
template<class S, class D, class C, class A>
template<class InputIter, class OutputIter>
inline bool
ternary_tree<S, D, C, A>::levenshtein_search_impl( node_index nid
												 , InputIter first, InputIter last
												 , OutputIter results
												 , int dist) const
{
	while(nid < node_count())
	{
		const node& n = m_nodes[nid];
		if (n.at_end())
		{
			// strlen of non-matched remainder <= dist
			if (last - first <= dist && n.at_end() != node::erased) {
				// break repeat searches (what if depth changed?)
				static node_index prev;
				if (prev != nid)
					*results++ = prev = nid; 
			}
			nid = n.hikid;
			continue;
		}
		if (dist > 0)
		{
			if (n.lokid < node_count())
				levenshtein_search_impl(n.lokid, unused, first, last, results, dist);
			bool keyend = (first == last);
			char_type c = (!keyend)? *first : 0;
			bool failmatch = keyend
						|| less(c, n.splitchar())
						|| less(n.splitchar(), c);
			InputIter next = first + (1-keyend);
			/*if (!failmatch) {
				levenshtein_search_impl(nid + 1, next, last, results, dist);
			}
			else*/ if (keyend) {
				levenshtein_search_impl(nid + 1, unused, first, last, results, dist - 1);
			}
			else {
				levenshtein_search_impl(nid, unused, next, last, results, dist - 1);
				levenshtein_search_impl(nid + 1, unused, first, last, results, dist - 1);
				levenshtein_search_impl(nid + 1, unused, next, last, results, dist - failmatch);
			}
			if (n.hikid < node_count())
				levenshtein_search_impl(n.hikid, unused, first, last, results, dist);
			return true;
		}

		if (first == last) {
			// if search string ended, spin to lokid
			// in case there's an endnode somewhere
			nid = n.lokid;
			continue;
		}

		char_type c = *first;
		if ( less(c, n.splitchar()) ) {
			nid = n.lokid;
		}
		else if ( less(n.splitchar(), c) ) {
			nid = n.hikid;
		}
		else {  // c == n.splitchar
			++first;
			++nid;
		}
	}
	return true;
}

#endif  // EXHAUSTIVE_LEVENSHTEIN_ALGO

//---------------------------------------------------------------------------------------
// combinatorial_search implementation
//---------------------------------------------------------------------------------------


template<class S, class D, class C, class A>
template<class String, class OutputIter>
inline void
ternary_tree<S, D, C, A>::combinatorial_search_impl( node_index nid
													, const String& key
													, OutputIter results
													, size_type jokers) const
{
	if (nid >= node_count())
		return;

	const node& n = m_nodes[nid];

	if (n.at_end())
	{
		if (n.at_end() != node::erased) {
			*results++ = nid;
		}
		if (n.hikid < node_count())
			combinatorial_search_impl(n.hikid, key, results, jokers);
		return;
	}

	if (n.lokid < node_count())
		combinatorial_search_impl(n.lokid, key, results, jokers);
	bool matched = false;
	for(typename string_type::const_iterator ci = key.begin(); ci != key.end(); ++ci)
	{
		char_type c = *ci;
		if (less(c, n.splitchar()))
			continue;
		if (less(n.splitchar(), c))
			break;
		matched = true;
		string_type newkey(key.begin(), ci);
		newkey.append(++ci, key.end());
		combinatorial_search_impl(nid + 1, newkey, results, jokers);
		break;
	}
	if (jokers && !matched)
		combinatorial_search_impl(nid + 1, key, results, jokers-1);
	if (n.hikid < node_count())
		combinatorial_search_impl(n.hikid, key, results, jokers);

}


//! \relates ternary_tree
template<class S, class D, class C, class A>
inline std::ostream& 
ternary_tree<S, D, C, A>::print_node(std::ostream& ostr, const node& n)
{
	if (n.at_end()) {
		ostr << "<tst:node up=\"" << n.parent << '"';
		if (n.at_end() == node::terminal)
			ostr << " val=\"" << n.value() << '"';
		else 
			ostr << " erased=\"1\"";
		if (n.hikid != node::invalid_index)
			ostr << " hi=\"" << n.hikid << '"';
		ostr << ">\n";
	} else {
		ostr << "<tst:node up=\"" << n.parent << '"';
		if (n.m_char > 32 && n.m_char < 255)
			ostr << char(n.m_char);
		else ostr << (size_type)n.m_char;
		ostr << " hi=\"" << long(n.hikid) << '"' /*<< ", eq " << long(n.eqkid)*/
			 << " lo=\"" << long(n.lokid) << "\">\n";
	}
	return ostr;
}

} // end namespace containers 
