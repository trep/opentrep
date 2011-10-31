// block direct use
#ifndef TERNARY_TREE_INCLUDE_GUARD
# error This is a dependent header file, included by ternary_tree.hpp - DO NOT USE DIRECTLY
#endif

#ifndef TST_NODE_HPP_INCLUDED
#define TST_NODE_HPP_INCLUDED


namespace containers {
namespace tst_detail {

	//---------------------------------------------------------------------------------------
	// ternary_tree:: node class
	// 0.68: reimplement from scratch, to manage managed pointers properly.
	//---------------------------------------------------------------------------------------

	/** Behaviourally, there are 2 1/2 types of nodes: 
	 *  - basic node stores a single character, 
	 *    and points on to paths for higher, lower and equal strings.
	 *  - end nodes store a mapped type item and points to higher children - \n
	 *    No lower or equal children exist, since end-of-string char is -inf.
	 *  - In practice, erased nodes are valueless endnodes: they have only hikid, 
	 *    and are unlinked from tree at any opportunity.
	 *
	 * We only ever return end-nodes to client code, and always wrapped in proxy types
	 * with desired behaviour (assignable and/or dereferenceable). 
	 *
	 * Invariants: 
	 *  - eqkid (nid + 1) is valid except at endnode (terminal or erased).
	 *  - lokid is always non-link at endnode.
	 *  - hikid is a valid link at reachable erased nodes.
	 *   (reachable: follows or is linked from a reachable node)
	 */
	//! Storage-independent base: clients do all access/transactions on value member.
	template<typename CharT, typename StorageT, typename SizeT> 
	struct node_base
	{
		typedef CharT     char_type;
		typedef StorageT  pointer_or_value;
		typedef SizeT     node_index;

		enum { invalid_index = node_index(-1) };
		// (terminal has valid m_value, erased have invalid_index written in lokid)
		enum { terminal = 1, erased = 2 };

		/** Client code should only see nodes where at_end == 1. */
		unsigned int at_end() const { return m_atend; }

		node_base(char_type c, node_index mom)
			: m_char(c)
			, m_atend(0)
			, parent(mom)
			, hikid(invalid_index)
			, lokid(invalid_index)
		{}

		void swap_values(node_base& other) {
			if (sizeof(pointer_or_value) > sizeof(lokid))
				std::swap(m_data, other.m_data);
			else
				std::swap(lokid, other.lokid);
		}

		void swap(node_base& other) {
			std::swap(m_char, other.m_char);
			std::swap(m_atend, other.m_atend);
			std::swap(parent, other.parent);
			std::swap(hikid, other.hikid);
			swap_values(other);
		}

		char_type splitchar() const { return m_char; }

		//! Erased and disused node; ie the rest of a twig following an erased node.
		bool is_unlinked() const {
			return at_end() == erased 
					&& hikid == invalid_index; 
					//&& lokid == node::invalid_index;
		}

		char_type   m_char;
		char_type   m_atend;
		node_index  parent;
		node_index  hikid;
		/** Terminal nodes considered having unique -inf split char,
		 * so all nodes of longer strings go to hikid. */
		union {
			// If there a compile error says this is illegal, search for "size_policy_node"
			// in ternary_tree.hpp, and change it to "always_heap_node"
			// - or change the test in struct size_policy_node (below)
			node_index        lokid;
			pointer_or_value  m_data;
		};

	};

	//---------------------------------------------------------------------------------------

	//! Stores data by managed pointer.
	template<typename CharT, typename DataT, typename AllocT, typename SizeT> 
	struct heap_node : public node_base<CharT, DataT*, SizeT>
	{
		typedef DataT                                                value_type;
		typedef DataT*                                               pointer;
		typedef node_base<CharT, pointer, SizeT>                     base_type;
		typedef typename base_type::char_type                        char_type;
		typedef typename base_type::node_index                       node_index;
		typedef typename AllocT::template rebind<value_type>::other  allocator_type;
		using base_type::at_end;
		using base_type::terminal;
		using base_type::invalid_index;
		using base_type::lokid;
		using base_type::hikid;
		using base_type::m_data;

		heap_node(char_type c, node_index mom)
			: base_type(c, mom)
		{}

		heap_node(const heap_node& other)
			: base_type(static_cast<const base_type&>(other))
		{
			if (at_end() == terminal) {
				// Undo lokid = other.lokid to avoid deleting other's value
				lokid = invalid_index;
				AllocT alloc;
				create_assign(m_data, other.value(), alloc);
			}
		}

		~heap_node() {
			if (at_end() == terminal) {
				AllocT alloc;
				clear_data(alloc);
			}
		}

		heap_node& operator=(const heap_node& other) {
			heap_node(other).swap(*this);
			return *this;
		}

		void swap(heap_node& other) { base_type::swap(other); }

		/** Requirements: Must only be called for nodes where at_end() == terminal; 
		 * Never called before data storage is allocated.
		 */
		value_type& value() 
		{
			assert(at_end() == terminal);
			return *m_data;
		}

		const value_type& value() const
		{
			assert(at_end() == terminal);
			return *m_data;
		}

		/** Requirements: Must only be called for nodes where at_end() == terminal. */
		void assign(const value_type& val, AllocT& allocator)  
		{
			assert(at_end() == terminal);
			typedef typename AllocT::template rebind<value_type>::other  allocator_type;
			allocator_type alloc(allocator);
			create_assign(m_data, val, alloc);
		}

		/* /! Called by owner to destroy data (if pointer; only calls destructor, no reset)
		template<class Iterator>
		static void deallocate(Iterator first, Iterator last, AllocT& allocator)
		{
			typename AllocT::template rebind<value_type>::other 
				alloc = typename AllocT::template rebind<value_type>::other(allocator);
			deallocate(first, last, alloc);
		} */

		void clear_data(AllocT& allocator) {
			typedef typename AllocT::template rebind<value_type>::other  allocator_type;
			allocator_type alloc(allocator);
			destroy(m_data, alloc);
			lokid = static_cast<node_index>(invalid_index);
		}

	private:
		static pointer invalid_pointer() { return reinterpret_cast<pointer>(-1); }

		static bool invalid_pointer(pointer p) { return (p == 0) || (p == invalid_pointer()); }

		//! Allocate new T for managed storage.
		static void create_assign(pointer& store, const value_type& val, allocator_type& allocator)
		{
			pointer p = allocator.allocate(1);
			allocator.construct(p, val);
			std::swap(store, p);
			destroy(p, allocator);
		}

		//! Call destructor and deallocate, but don't necessarily clear pointer.
		static void destroy(pointer& store, allocator_type& allocator)
		{
			if ( ! invalid_pointer(store)) {
				allocator.destroy(store);
				allocator.deallocate(store, 1);
				store = invalid_pointer();
			}
		}
	};

	//---------------------------------------------------------------------------------------

	//! Stores data in union - requires trivial constructor
	template<typename CharT, typename DataT, typename AllocT, typename SizeT> 
	struct inplace_node : public node_base<CharT, DataT, SizeT>
	{
		typedef DataT                                                value_type;
		typedef DataT*                                               pointer;
		typedef node_base<CharT, DataT, SizeT>                       base_type;
		typedef typename base_type::char_type                        char_type;
		typedef typename base_type::node_index                       node_index;
		typedef typename AllocT::template rebind<value_type>::other  allocator_type;
		using base_type::at_end;
		using base_type::terminal;
		using base_type::invalid_index;
		using base_type::lokid;
		using base_type::hikid;
		using base_type::m_data;

		inplace_node(char_type c, node_index mom)
			: base_type(c, mom)
		{}

		// builtin copy-constructor, destructor, assign are fine

		/** Requirements: Must only be called for nodes where at_end() == terminal; 
		 * Never called before data storage is allocated.
		 */
		value_type& value() 
		{
			assert(at_end() == terminal);
			return m_data;
		}

		const value_type& value() const
		{
			assert(at_end() == terminal);
			return m_data;
		}

		/** Requirements: Must only be called for nodes where at_end() == terminal. */
		void assign(const value_type& val, AllocT& )  
		{
			assert(at_end() == terminal);
			m_data = val;
		}

		void clear_data(AllocT& ) {
			lokid = static_cast<node_index>(invalid_index);
		}
	};

	//---------------------------------------------------------------------------------------
	//! lame ripoff from boost::mpl::if_ - replace with real thing when boostifying
	//---------------------------------------------------------------------------------------
	namespace mpl_detail {

		template<
			bool C
			, typename T1
			, typename T2
			>
		struct if_c
		{
			typedef T1 type;
		};

		template<
			typename T1
			, typename T2
			>
		struct if_c<false,T1,T2>
		{
			typedef T2 type;
		};

	}  // namespace mpl_detail

	//---------------------------------------------------------------------------------------

	/** Selects node type by size of DataT - if larger than a pointer, use 
	 * heap_node, else inplace_node. But since inplace-node uses a union,
	 * the data type must also have trivial constructor. 
	 * TODO: Define inplace_class_node. Hack for now: use always_heap_node below.
	 * (Change to use C++0x type_traits when we can count on compilers to support this)
	 */
	template<typename CharT, typename DataT, typename AllocT, typename SizeT> 
	struct size_policy_node
	{
		// type switch: heap_node if DataT larger than pointer, else inplace_node
		// 6.8.3: ensure std::string goes to heap_node, since it's not trivial type
		typedef typename mpl_detail::if_c< ((sizeof(DataT) > sizeof(void*)) 
										 || (sizeof(std::string) <= sizeof(void*)))
										 , heap_node    <CharT, DataT, AllocT, SizeT>
										 , inplace_node <CharT, DataT, AllocT, SizeT>
										 >::type  node_type;
	};

	//! When DataT is not trivially constructible
	template<typename CharT, typename DataT, typename AllocT, typename SizeT> 
	struct always_heap_node
	{
		typedef heap_node<CharT, DataT, AllocT, SizeT>  node_type;
	};


	//---------------------------------------------------------------------------------------

} } // end namespace containers::tst_detail


#endif  // TST_NODE_HPP_INCLUDED
