#ifndef  LVV_STL_H
#define  LVV_STL_H

#include <stddef.h>

#include <type_traits>	 // for META
#include <string>

/////////////////////////////////////////////////////////////////////////////////////////  CONTAINER MANIP

////// push_back/push_front replaement
//
// usage:
//    scc 'vint V;  V << 1 << 2'
//    {1, 2}
//
//    scc 'dint V;  11 >> (22 >> V)'
//    {11}
//

	template<typename T, template<typename T, typename Ct> class Ct >
	Ct <T,std::allocator<T>> &
operator<<      (Ct<T, std::allocator<T> >& C, T x)    { C.push_back(T(x)); return C; };

	template<typename T, template<typename T, typename Ct> class Ct >
	Ct <T,std::allocator<T>> &
operator>>      (T x, Ct<T, std::allocator<T> >& C)    { C.push_front(x); return C; };

// operator-=  -- remove member from container
	template<typename T, template<typename T, typename Ct> class Ct >
	Ct <T,std::allocator<T>> &
operator-=      (Ct<T, std::allocator<T> >& C, T x )    { C.erase(remove(C.begin(), C.end(), x), C.end()); return C; };

/////////////////////////////////////////////////////////////////////////////////////////  META


///// IS_CONTAINER
	template <typename T>
struct is_container {	// from http://stackoverflow.com/questions/4347921/sfinae-compiler-troubles
	template <typename U>
	static char test(
		U* u,
		typename U::const_iterator b = ((U*)0)->begin(),
		typename U::const_iterator e = ((U*)0)->end()
	);
	template <typename U> static long test(...);

	enum { value = sizeof test<T>(0) == 1 };
};

template<typename T, size_t N>	struct  is_container <T[N]>	: std::true_type { };
template<typename T, size_t N>	struct  is_container <std::array<T,N>>	: std::true_type { };

/////  IS STRING
template<typename T>	struct  is_string		: std::false_type {};
template<>		struct  is_string <std::string>	: std::true_type  {};


/////////////////////////////////////////////////////////////////////////////////////////  BEGIN/END

// begin()
	template<typename Ct >
	typename std::enable_if <is_container<Ct>::value, typename Ct::iterator>::type
operator+      (Ct& C) { return begin(C); };

// end()
	template<typename Ct >
	typename std::enable_if <is_container<Ct>::value, typename Ct::iterator>::type
operator-      (Ct& C) { return  end(C); };

// size()
	template<typename Ct>
	typename std::enable_if <is_container<Ct>::value, typename Ct::size_type>::type
operator!      (const Ct& C) { return C.size(); };	// FIXME: specialization for non-contigues-mem-Ct, C-arr


#endif	// LVV_STL_H
