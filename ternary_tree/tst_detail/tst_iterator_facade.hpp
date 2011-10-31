// block direct use
#ifndef TERNARY_TREE_INCLUDE_GUARD
# error This is a dependent header file, included by ternary_tree.hpp - DO NOT USE DIRECTLY
#endif

#ifndef TST_ITERATOR_FACADE_HPP_INCLUDED
#define TST_ITERATOR_FACADE_HPP_INCLUDED


namespace containers {
namespace tst_detail {


/** \def TST_NO_STANDALONE_ITERATOR_FACADE 
 * By default we define a method forwarding interface, but some clients, eg structured container wrappers, 
 * do not need the additional interface complication.
 */
#ifndef TST_NO_STANDALONE_ITERATOR_FACADE
	/** Helper template that forwards the key(), value() public methods from tst_iterator_base 
	 * to the \ref ::iterators::iterator_wrapper "iterator_wrapper" type: 
	 * \b iterator_wrapper doesn't inherit from its base_iter, so this class reaches through 
	 * and forwards the needed methods. 
	 *
	 * \b Rationale: It is better for users to type \c it.key() than \c it.iter_base().key().
	 *
	 * Note that the constructors must mimic the iterator_wrapper constructors, in order to get
	 * correct iterator type copying relations.
	 * \relates ternary_tree
	 */
	template<class BaseT>
	struct iter_method_forward : public BaseT
	{
		iter_method_forward() {}

		template<class SameBase>
		iter_method_forward(const iter_method_forward < 
										iterators::iterator_wrapper<SameBase, 
										iterators::nonconst_traits<typename SameBase::value_type> > >& it)
			: BaseT(it)
		{}

		iter_method_forward(const iter_method_forward& it) : BaseT(it) {}

		iter_method_forward(const BaseT& it) : BaseT(it) {}

		iter_method_forward(typename BaseT::base_iter const & it) : BaseT(it) {}

		//! The key at current iterator position.
		typename BaseT::base_iter::string_type const&  key() const { return forward().key(); }
		//! Const value at current iterator position (use iterator dereference for mutable access).
		typename BaseT::base_iter::const_reference  value() const { return forward().value(); }

		//! Used by iterators for wrapper classes.
		typename BaseT::base_iter::node_index  tree_position() const { return forward().tree_position(); }

	// not sure how to declare template friend in other namespace - so we forward all methods publicly
	//private:
		//template<typename B, typename T, typename C>
		//	friend struct ::iterators::iterator_wrapper<B,T,C>;

		//! Used by iterators for wrapper classes.
		bool empty() const { return forward().empty(); }

		typename BaseT::reference  dereference() const { return forward().dereference(); }
		void  increment() { return forward().increment(); }
		void  decrement() { return forward().decrement(); }
		bool equal(const iter_method_forward& other) const { return forward().equal(other.iter_base()); }

	public:
		typedef typename BaseT::base_iter  forward_t;
		forward_t& forward() { return BaseT::iter_base(); }
		const forward_t& forward() const { return BaseT::iter_base(); }
	};

#  define TST_ITERATOR_FACADE_BEGIN tst_detail::iter_method_forward <
#  define TST_ITERATOR_FACADE_END   >
#else
#  define TST_ITERATOR_FACADE_BEGIN
#  define TST_ITERATOR_FACADE_END
#endif  // TST_STANDALONE_ITERATOR_FACADE

} } // end namespace containers::tst_detail


#endif  // TST_ITERATOR_FACADE_HPP_INCLUDED
