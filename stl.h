#ifndef  LVV_STL_H
#define  LVV_STL_H

#include <stddef.h>
#include <iterator>
#include <string>
#include <cstring>
#include <deque>
#include <tuple>

#include "meta.h"

/////////////////////////////////////////////////////////////////////////////////////////  MEMBERS ALIASES

//  +Ct / -Ct   ---   end() /begin()
	template<typename Ct >
	typename std::enable_if <is_container<Ct>::value, typename Ct::iterator>::type
operator+      (Ct& C) { return begin(C); };

	template<typename Ct >
	typename std::enable_if <is_container<Ct>::value, typename Ct::iterator>::type
operator-      (Ct& C) { return  end(C); };

//  !Ct  --- size()
	template<typename Ct>
	typename std::enable_if <is_container<Ct>::value, ssize_t>::type
operator!      (const Ct& C) { return C.size(); };

//  if (Ct)  --- if (!Ct.empty())


//  ++T / T++  ---  front()/back()/.first/.second
	template<typename Ct>
	typename std::enable_if <is_container<Ct>::value, typename Ct::reference>::type
operator++      (Ct& C) { return C.front(); };

	template<typename Ct>
	typename std::enable_if <is_container<Ct>::value, typename Ct::reference>::type
operator++      (Ct& C, int) { return C.back(); };


//  x >> Ct << x   ---  push_back/push_front replaement;   usage: scc 'vint V;  V << 1 << 2'   prints: {1, 2}
	template<typename Ct>
	typename std::enable_if <is_container<Ct>::value   &&   has_push_back<Ct>::value,   Ct&>::type
operator<<      (Ct& C, const typename Ct::value_type& x)    { C.push_back(x);   return C; };

	template<typename Ct>
	typename std::enable_if <is_container<Ct>::value   &&   has_insert<Ct>::value,   Ct&>::type
operator<<      (Ct& C, const typename Ct::value_type& x)    { C.insert(x);   return C; };

	template<typename Ct>
	typename std::enable_if <is_container<Ct>::value   &&   has_push_front<Ct>::value,   Ct>::type
operator>>      (typename Ct::value_type x, Ct& C)    { C.push_front(x);  return C; };


//  x << Ct >> x   ---  remove head / tail;   usage: scc 'llong V{1,2,3};  i << V >> j; __ i, V, j;'   prints: 1 {2} 3 
	template<typename Ct>
	typename std::enable_if <is_container<Ct>::value, Ct>::type&
operator>>      (Ct& C, typename Ct::value_type& x)    { x = C.back();   C.pop_back();    return C; };

	template<typename Ct>
	typename std::enable_if <is_container<Ct>::value, Ct&>::type
operator<<      (typename Ct::value_type& x, Ct& C)    { x = C.front();  C.pop_front();  return C; };


// Ct1 << Ct2
	template<typename Ct1, typename Ct2>
	typename std::enable_if <
		is_container<Ct1>::value   &&  is_container<Ct2>::value
			&&  std::is_same<typename Ct1::value_type, typename Ct2::value_type>::value
		, Ct1
	>::type &
operator <<      (Ct1& C1, const Ct2& C2)    { for(auto x: C2) C1.push_back(x);   return  C1; };


// Ct1 >> Ct2
	template<typename Ct1, typename Ct2>
	typename std::enable_if <
		is_container<Ct1>::value  
			&&  is_container<Ct2>::value  
			&&  std::is_same<typename Ct1::value_type, typename Ct2::value_type>::value 
			&&  has_push_front<Ct2>::value
		, Ct2
	>::type &
operator >>      (const Ct1& C1, Ct2& C2)    { std::copy(C1.rbegin(), C1.rend(), std::front_inserter(C2));  return C2; };


// --Ct/Ct--  ---  pop_back/pop_front;     usage:   scc 'vint V{1,2}, W;  W << --V;  __ V, W;'   prints:    {2}, {1}
	template<typename Ct>
	typename std::enable_if <is_container<Ct>::value, typename Ct::value_type>::type &&
operator--      (Ct& C)         { typename Ct::value_type x = C.front();   C.pop_front();   return  std::move(x); };


	template<typename Ct>
	typename std::enable_if <is_container<Ct>::value, typename Ct::value_type>::type &&
operator--      (Ct& C, int)    { typename Ct::value_type x = C.back();    C.pop_back();    return  std::move(x); };


// Ct1 <= Ct2
	template<typename Ct1, typename Ct2>
	typename std::enable_if <
		is_container<Ct1>::value   &&  is_container<Ct2>::value
			&&  std::is_same<typename Ct1::value_type, typename Ct2::value_type>::value
		, Ct1
	>::type &
operator <=      (Ct1& C1, const Ct2& C2)    { C1.clear(); for(auto x: C2) C1.push_back(x);   return  C1; };

	template<typename Ct, size_t N>
	typename std::enable_if < is_container<Ct>::value , Ct >::type &
operator <=      (Ct& C, const typename Ct::value_type (&A)[N])    { C.clear();  for(auto x: A) C.push_back(x);   return  C; };

/////////////////////////////////////////////////////////////////////////////////////////////////////  SEARCH

//  Ct / x   ---  find() --> it	   usage: scc 'copy(v9/2, v9/5,oi)'
	template<typename Ct>
	typename std::enable_if <is_container<Ct>::value,  typename Ct::iterator>::type
operator /       (Ct& C, const typename Ct::value_type& x)    {  return find(C.begin(), C.end(), x); };


//  Ct % x   ---  find() --> bool	
	template<typename Ct>
	typename std::enable_if <is_container<Ct>::value,  bool>::type
operator %       (const Ct& C, const typename Ct::value_type& x)    {  return C.end() != find(C.begin(), C.end(), x); };


//  Ct1 % Ct2   ---  search() --> bool	
	template<typename Ct>
	typename std::enable_if <is_container<Ct>::value,  bool>::type
operator %       (const Ct& C1, const Ct& C2)    {  return C1.end() != search(C1.begin(), C1.end(), C2.begin(), C2.end()); };


//  Ct | x   ---  find() --> *it	   usage: scc 'v9|3=33; v9'
	template<typename Ct>
	typename std::enable_if <is_container<Ct>::value,  typename Ct::value_type&>::type
operator |       (Ct& C, const typename Ct::value_type& x)    { 
	auto it = find(C.begin(), C.end(), x);
	if (it == C.end()) {
		C.push_back(typename Ct::value_type());
		return C.back();
	}
	return *it;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////  TUPLE / PAIR

template<typename U, typename V>   U&     operator++   (std::pair<U,V>& P)      { return P.first;  };
template<typename U, typename V>   V&     operator++   (std::pair<U,V>& P, int) { return P.second; };

	template <class... Types>
	typename std::tuple_element<std::tuple_size<std::tuple<Types...> >::value-1, typename std::tuple<Types...> >::type&
operator++	(typename std::tuple<Types...>& Tpl, int)  {  return  std::get<std::tuple_size<std::tuple<Types...> >::value-1>(Tpl); };


	template <class... Types>
	typename std::tuple_element<0, std::tuple<Types...> >::type&
operator++	(typename std::tuple<Types...>& Tpl)  {  return  std::get<0>(Tpl); };

	template <class... Types>
	constexpr size_t
operator!	(const typename std::tuple<Types...>& Tpl)  {  return  std::tuple_size<std::tuple<Types...> >::value; };


/////////////////////////////////////////////////////////////////////////////////////////////////////  STACK

//  Stack << x
	template<typename Ct>
	typename std::enable_if <is_stack<Ct>::value, Ct&>::type&
operator<<      (Ct& C, const typename Ct::value_type& x)    { C.push(x);   return C; };

//  Stack--
	template<typename Ct>
	typename std::enable_if <is_stack<Ct>::value, Ct&>::type&
operator--      (Ct& C, int)    { C.pop();   return C; };

//  Stack >> x
	template<typename Ct>
	typename std::enable_if <is_stack<Ct>::value, typename Ct::value_type&>::type&
operator>>      (Ct& C, typename Ct::value_type& x)    { x = C.top();  C.pop();   return x; };

//  Stack++
	template<typename Ct>
	typename std::enable_if <is_stack<Ct>::value, typename Ct::value_type&>::type&
operator++      (Ct& C, int)    { return C.top(); };

//  !Stack
	template<typename Ct>
	typename std::enable_if <is_stack<Ct>::value, size_t>::type
operator!      (Ct& C)    { return C.size(); };


/////////////////////////////////////////////////////////////////////////////////////////////////////  QUEUE

//  Queue << x
	template<typename Ct>
	typename std::enable_if <is_queue<Ct>::value, Ct&>::type&
operator<<      (Ct& C, const typename Ct::value_type& x)    { C.push(x);   return C; };

//  --Queue
	template<typename Ct>
	typename std::enable_if <is_queue<Ct>::value, Ct&>::type&
operator--      (Ct& C)    { C.pop();   return C; };

//  x << Queue
	template<typename Ct>
	typename std::enable_if <is_queue<Ct>::value, typename Ct::value_type&>::type&
operator<<      (typename Ct::value_type& x, Ct& C)    { x = C.front();  C.pop();   return x; };

//  Queue++
	template<typename Ct>
	typename std::enable_if <is_queue<Ct>::value, typename Ct::value_type&>::type&
operator++      (Ct& C, int)    { return C.back(); };

//  ++Queue
	template<typename Ct>
	typename std::enable_if <is_queue<Ct>::value, typename Ct::value_type&>::type&
operator++      (Ct& C)    { return C.front(); };

//  !Queue
	template<typename Ct>
	typename std::enable_if <is_queue<Ct>::value, size_t>::type
operator!      (Ct& C)    { return C.size(); };




#endif	// LVV_STL_H
