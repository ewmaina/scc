
						#ifndef  STO_CHAIN_RANGE_H
						#define  STO_CHAIN_RANGE_H

						#include "scc/meta.h"
						#include "scc/stl.h"

						/*
						#ifndef NDEBUG
						#include <iostream>
						#include <scc/debug.h>
						#endif
						*/

						namespace sto {
						//template<class Ch> class basic_string<Ch>;

/////////////////////////////////////////////////////////////////////////////////////////  CHAIN_RANGE_ITERATOR
	template<class Rg>  struct  chain_range;

	template <class Rg, bool RO>
struct chain_range_iterator {
				

		// TYPES
		typedef SEL <
			RO,
			rg_const_iterator<Rg>,
			rg_iterator<Rg>
		>  org_iterator;

		typedef	SEL <
			RO,
			chain_range<Rg> const,
			chain_range<Rg>
		>  parent_t;


	// MEMBERS
	parent_t*	parent;
	org_iterator	current;


	// STL ITERATOR TYPES
	typedef		typename std::iterator_traits<org_iterator>::iterator_category 	iterator_category;

	typedef		typename parent_t::value_type			value_type;
	typedef		typename parent_t::const_iterator		const_iterator;
	typedef		chain_range_iterator<Rg,RO>			iterator;

	typedef		size_t  					size_type;
	typedef		ptrdiff_t 					difference_type ;
	typedef		SEL <RO, const rm_ref<value_type>*, rm_ref<value_type>*>   	pointer;

	typedef         rg_const_reference<Rg>     			const_reference;  // non-STL
	typedef		typename std::conditional <
				std::is_const<Rg>::value || RO,
				rg_const_reference<Rg>,
				rg_reference<Rg>
			>::type reference;


	// non-STL
	typedef		rg_elem_type<Rg>  				elem_type;
	typedef		rm_ref<chain_range_iterator>			self_type;
	typedef		self_type					type;

	////// CTOR
	chain_range_iterator ()				: parent(0)           			   {};	// default
	chain_range_iterator ( const self_type& rhs)	: parent(rhs.parent), current(rhs.current) {};	// copy 
	chain_range_iterator ( parent_t* parent,  const org_iterator current) 
							: parent(parent), current(current)         {};
	// ASSIGNMENT
		/* implicit */

	////// CONVERSION  non-const --> const
	operator chain_range_iterator<Rg&&,true>() { return chain_range_iterator<Rg&&,true>(parent, current); };

	////// IFACE
	reference	operator*()  		{ return  *current; };
	const_reference operator*() const 	{ return  *current; };

	pointer		operator->()		{ return  &(operator*()); }
	pointer	const 	operator->() const	{ return  &(operator*()); }

	// ++ It
	self_type&	operator++()		{ ++current;  return *this; }


	// ++ It
	self_type	operator++(int)		{
		org_iterator e = endz(parent->rg);
		assert(current !=e);
		self_type tmp=*this;
		++current;
		return std::move(tmp);
	}

	bool	operator==(const_iterator rhs)	const	{ return   current == rhs.current; }
	bool	operator!=(const_iterator rhs)	const	{ return   current != rhs.current; }

	///////////////////////////////////////////////////////////////////// INPORT ORG_ITERATOR METHODS
	
	//  bidiractional
	template<class U=org_iterator, class=decltype(std::declval<U>()--)> 			self_type&	operator--()			{ --current;  return *this; }
	template<class U=org_iterator, class=decltype(std::declval<U>()--)> 			self_type	operator--(int)			{ self_type tmp=*this;  --current;   return std::move(tmp); }
	                                                          
	// random access
	template<class U=org_iterator, class=decltype(std::declval<U>()+=1)>			iterator	operator+= (difference_type n)	{ current+=n;  return *this; }
	template<class U=org_iterator, class=decltype(std::declval<U>()-=1)>			iterator	operator-= (difference_type n)	{ current-=n;  return *this; }
	template<class U=org_iterator, class=decltype(std::declval<U>()[1])>			reference	operator[] (difference_type n)	{ return current[n]; }
	template<class U=org_iterator, class=decltype(std::declval<U>() <  std::declval<U>())>	bool		operator<  (self_type other)	{ return current <  other.current; } 
	template<class U=org_iterator, class=decltype(std::declval<U>() <= std::declval<U>())>	bool		operator<= (self_type other)	{ return current <= other.current; } 
	template<class U=org_iterator, class=decltype(std::declval<U>() >  std::declval<U>())>	bool		operator>  (self_type other)	{ return current >  other.current; } 
	template<class U=org_iterator, class=decltype(std::declval<U>() >= std::declval<U>())>	bool		operator>= (self_type other)	{ return current >= other.current; } 
 };



/////////////////////////////////////////////////////////////////////////////////////////  REF CONTAINER

template<class T>	struct  ref_container;
template<class T>	struct  ref_container<T& >  { T& value;  explicit ref_container(T&  x) : value(x)            {} };
template<class T>	struct  ref_container<T&&>  { rm_ref<T>  value;  explicit ref_container(T&& x) : value(x) {} };

/////////////////////////////////////////////////////////////////////////////////////////  CHAIN_RANGE
	template<class Rg>
struct  chain_range : ref_container<Rg&&> {


	// STL IFACE
	typedef		rg_elem_type<Rg>  				value_type;	// post-f type
	typedef		chain_range_iterator<Rg,false>     		iterator;
	typedef		chain_range_iterator<Rg,true>			const_iterator;

	typedef		size_t  					size_type;
	typedef		ptrdiff_t 					difference_type ;
	typedef		rm_ref<value_type>*				pointer;

	typedef         rg_const_reference<Rg>     			const_reference;  // non-STL
	typedef		typename std::conditional <
				std::is_const<Rg>::value /* || RO*/,      // <-- different from interator
				rg_const_reference<Rg>,
				rg_reference<Rg>
			>::type reference;


	// non-STL
	typedef		rg_elem_type<Rg>  				elem_type;	// pre-f type
	typedef		chain_range					self_type;

	// MEMBERS
	Rg&	rg;

	////  CTOR
	explicit chain_range(Rg&& rg)  : ref_container<Rg&&>(std::forward<Rg>(rg)), rg(this->value)  {};

	////  ASSIGNMENT
	self_type&   operator= (elem_type x) { std::fill(begin(), end(), x);  return *this; };


		template<class Rg2>			// TODO specialize for seq containers self-assignemet
		eIF <have_same_elem<Rg,Rg2>::value, self_type>
	operator= (const Rg2& rhs) { 			//std:: cout << " SSIGNMENT \n";
		sto::clear(rg);
		auto e = endz(rhs);
		for (auto it = std::begin(rhs);   it != e;  ++it)  {
			detail::append_elem(std::forward<Rg>(rg), *it);
		}
		return *this;
	};


	////  ITERATOR
	      iterator	end()		{ return        iterator(this, endz(rg)); }
	const_iterator	end()   const	{ return  const_iterator(this, endz(rg)); }


	      iterator	begin()		{ return        iterator(this, std::begin(rg)); };
	const_iterator	begin()	const	{ return  const_iterator(this, std::begin(rg)); };



	////  RG PROPERTIES
	size_t		size() const	{ return  sto::size (rg); };
	bool		empty () const	{ return  sto::empty(rg); }
	explicit operator bool() const	{ return !sto::empty(rg); }


	////  ELEM ACCESS
	reference const	front()  const	{ return  *std::begin(rg); }
	reference  	front()		{ return  *std::begin(rg); }

	reference const back()  const	{ return  *std::prev(sto::endz(rg)); }  
	reference  	back()		{ return  *std::prev(sto::endz(rg)); } 


	////  INPORTED RG METHODS
	template<class U=Rg>   eIF<has_push_back<U>::value>		push_back(const elem_type&  value)	{rg.push_back(value);}
	template<class U=Rg>   eIF<has_push_back<U>::value>		push_back(      elem_type&& value)	{rg.push_back(std::move(value));}

	template<class U=Rg>   eIF<has_push_front<U>::value>		push_front(const elem_type&  value)	{rg.push_front(value);}
	template<class U=Rg>   eIF<has_push_front<U>::value>		push_front(      elem_type&& value)	{rg.push_front(std::move(value));}

	template<class U=Rg>   eIF<has_1arg_insert<U>::value>		insert(const elem_type&  value)		{rg.insert(value);}
	template<class U=Rg>   eIF<has_1arg_insert<U>::value>		insert(      elem_type&& value)		{rg.insert(std::move(value));}

	template<class U=Rg>   eIF<has_pop_back<U>::value>		pop_back()				{rg.pop_back();}
	template<class U=Rg>   eIF<has_pop_front<U>::value>		pop_front()				{rg.pop_front();}
	
	template<class U=Rg>  auto  operator[] (difference_type n) -> decltype(std::declval<U>()[0])   { return  rg[n]; } // FIXME for pred

	////  ADDED RG METHODS
		template<class U=Rg>  
		eIF<is_c_string_t<U>::value>	
	push_back(char value)			{
		auto e=endz(rg);  
		assert(e < (rg + std::extent<rm_qualifier<U>>::value-1)); 
		*e = value; 
		*++e='\0';
	}
 };



////////////////////////////////////////////////////////////////  TRAITS
/*
template<class Rg>		struct is_range_t<chain_range<Rg>>		: std::true_type  {};
template<class Rg, bool RO>	struct is_range_t<chain_range_iterator<Rg,RO>>	: std::false_type {};

template<class Rg>		struct is_chain_range               		: std::false_type {};
template<class Rg>		struct is_chain_range<chain_range<Rg>>		: std::true_type {};

template<class It>		struct is_chain_range_iterator      		: std::false_type {};
template<class Rg, bool RO>	struct is_chain_range_iterator <chain_range_iterator<Rg,RO>> 	: std::true_type {};
*/

template<class Rg>		struct is_range_t<chain_range<Rg>>		: std::true_type  {};
template<class Rg, bool RO>	struct is_range_t<chain_range_iterator<Rg,RO>>	: std::false_type {};

template<class It>		struct is_chain_range_iterator      		: std::false_type {};
template<class Rg, bool RO>	struct is_chain_range_iterator <chain_range_iterator<Rg,RO>> 	: std::true_type {};



////////////////////////////////////////////////////////////////  FUNCTION RANGE() -- range maker

	template<class Rg>   
	eIF<is_range<Rg>::value, chain_range<Rg&&>>   
range(Rg&& rg)  {
	return  chain_range<Rg&&>(std::forward<Rg>(rg));  // there is no copy on return
 };
//////////////////////////////////////////////////////////////////////  Rg || F   ---  accumulate(+C+1,-C, ++C, F) -> D  		 

	template< typename Rg, typename T = rg_elem_type<Rg>, typename R = T > 
	eIF <is_range<Rg>::value, R>					// const T&(cont T&,cont T&) -- plain functions
operator ||       (Rg&& rg, const R& (*f)(const T&, const T&) )    {
	auto i = std::next(std::begin(rg));
	return  std::accumulate(i, endz(rg), front(rg), f);
 };
	
	template< typename Rg, typename T = rg_elem_type<Rg>, typename R = T > 
	eIF <is_range<Rg>::value, R>					// T(cont T&,cont T&) -- plain functions
operator ||       (Rg&& rg, R (*f)(const T&, const T&) )    {
	auto i = std::next(std::begin(rg));
	return  std::accumulate(i, endz(rg), front(rg), f);
 };
	
	template< typename Rg, typename T = rg_elem_type<Rg>, typename R = T > 
	eIF <is_range<Rg>::value, R>							// overload for: lambda, std::plus
operator ||       (Rg&& rg, identity<std::function<T(const T&, const T&)>> f )    {
	auto i = std::next(std::begin(rg));
	const T init = front(rg);
	return  std::accumulate(i, endz(rg), init, f);
 };


						

						#include "scc/cr_mapped.h"
						}; 
						#endif //  STO_CHAIN_RANGE_H
