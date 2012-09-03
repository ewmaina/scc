
						#ifndef  STO_ITERATOR_RANGE_H
						#define  STO_ITERATOR_RANGE_H

						#include "scc/meta.h"
						namespace sto {

/////////////////////////////////////////////////////////////////////////////////////////  ITERATOR_RANGE

	template<typename I>
struct  iterator_range {
		typedef		      I							iterator;
		typedef		const I							const_iterator;
		typedef		typename std::iterator_traits<I>::value_type		value_type;
		typedef		typename std::iterator_traits<I>::difference_type	difference_type ;
		typedef		typename std::iterator_traits<I>::pointer		pointer ;
		typedef		typename std::iterator_traits<I>::reference		reference ;
	iterator b_, e_;

	// CTOR
	iterator_range()  : b_(0), e_(0) {};
	iterator_range(iterator b, iterator e)  : b_(b), e_(e) {};


	//template<typename Ct>
	//iterator_range( typename std::enable_if<std::is_same<I, typename Ct::iterator>::value, Ct>::type&  C)  : b_(begin(C)), e_(end(C)) {};

	iterator	begin()		{ return b_; };
	iterator	end()		{ return e_; };
	const_iterator	begin() const	{ return b_; };
	const_iterator	end()   const	{ return e_; };

	iterator_range& operator= (value_type v) { for(auto& el: *this) el = v;  return *this; };


	/* to bool conversion --
	*/
	 
			struct PointerConversion { int valid; };
			typedef int PointerConversion::* datamemptr;

	operator datamemptr  () const {
		return  (b_ != e_) ? &PointerConversion::valid : 0;
	};

 };

////////////////////////////////////////////////////////////////  TRAITS

//template<typename T>	struct  is_range_t	<iterator_range<T>> 	: std::true_type  {};
template<typename I>	struct  is_range_t	<iterator_range<I>>	: std::true_type { };


////////////////////////////////////////////////////////////////  RANGE() -- range maker

	template<typename I>
	eIF<is_iterator<I>(), iterator_range<I>>
range(I b, I e) { return iterator_range<I>(b,e); };


						};
						#endif //  STO_ITERATOR_RANGE_H