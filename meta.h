#ifndef  LVV_META_H
#define  LVV_META_H

#include <type_traits>
#include <functional>
#include <iterator>
#include <array>
#include <stack>
#include <queue>

////////////////////////////////////////////////////////////////////////////////////////  META META

struct no_type{};

template<class From, class To> struct copy_rcv			           { typedef To type; };
template<class From, class To> struct copy_rcv<From const            , To> { typedef typename copy_rcv<From, To> ::type const           type; };
template<class From, class To> struct copy_rcv<From volatile         , To> { typedef typename copy_rcv<From, To> ::type volatile        type; };
template<class From, class To> struct copy_rcv<From const volatile   , To> { typedef typename copy_rcv<From, To> ::type const volatile  type; };
template<class From, class To> struct copy_rcv<From &                , To> { typedef typename copy_rcv<From, To> ::type&                type; };
template<class From, class To> struct copy_rcv<From const&           , To> { typedef typename copy_rcv<From, To> ::type const&          type; };
template<class From, class To> struct copy_rcv<From volatile&        , To> { typedef typename copy_rcv<From, To> ::type volatile&       type; };
template<class From, class To> struct copy_rcv<From const volatile&  , To> { typedef typename copy_rcv<From, To> ::type const volatile& type; };
template<class From, class To> struct copy_rcv<From &&               , To> { typedef typename copy_rcv<From, To> ::type&&               type; };
template<class From, class To> struct copy_rcv<From const&&          , To> { typedef typename copy_rcv<From, To> ::type const&&         type; };
template<class From, class To> struct copy_rcv<From volatile&&       , To> { typedef typename copy_rcv<From, To> ::type volatile&&      type; };
template<class From, class To> struct copy_rcv<From const volatile&& , To> { typedef typename copy_rcv<From, To> ::type const volatile&& type; };

// shortcuts
template<typename Ct>   	   using  rm_qualifier     = typename std::remove_cv<typename std::remove_reference<Ct>::type>::type;
template<typename Ct>   	   using  rm_ref           = typename std::remove_reference<Ct>::type;


////////////////////////////////////////////////////////////////////////////////////////  CL_TRAITS


template <typename T>		struct cl_traits      {
		template < typename U, typename VT = typename rm_ref<U>::value_type>	static VT	vt(rm_ref<U>* u);
		template <typename U>							static no_type	vt(...);
	typedef   decltype(vt<T>(0))   elem_type ;

		template <typename U, typename IT = typename rm_ref<U>::iterator>	static IT       it(rm_ref<U>* u);
		template <typename U>							static no_type  it(...);
	typedef   decltype(it<T>(0))   iterator;

		template <typename U, typename RF = typename rm_ref<U>::reference>	static RF       rf(rm_ref<U>* u);
		template <typename U>                                          		static no_type  rf(...);
	typedef     decltype(rf<T>(0))     reference ;
};


template <typename T, size_t N> struct cl_traits<T[N]>     { typedef  T  elem_type;   typedef  T*  iterator;   typedef  T&  reference;  };
template <typename T, size_t N> struct cl_traits<T(&)[N]>  { typedef  T  elem_type;   typedef  T*  iterator;   typedef  T&  reference;  };


template<typename Ct>   using cl_elem_type      = typename cl_traits<Ct>::elem_type;
template<typename Ct>   using cl_iterator       = typename cl_traits<Ct>::iterator;
template<typename Ct>   using cl_reference      = typename cl_traits<Ct>::reference;

template<typename T, typename Ct>  using  is_elem_of       = std::is_same<rm_ref<T>, rm_ref<cl_elem_type<Ct>> >;

/////////////////////////////////////////////////////////////////////////////////////////////////  STD SHORTCUTS
template<bool Cnd, typename T=void>     using  eIF                     = typename std::enable_if <Cnd,T>::type;
template<typename Ct1, typename Ct2>	using  is_cl2cl_convertible    = std::is_convertible<cl_elem_type<Ct1>, cl_elem_type<Ct2>>;
//template<typename Ct, typename xT>	using  is_x2cl_convertible     = std::is_convertible<std::remove_reference<xT>::type, std::remove_recl_value_type<Ct>>;
template<typename Ct, typename xT>	using  is_x2cl_convertible     = std::is_convertible<xT, cl_elem_type<Ct>>;
//template<typename It, typename Ct>	using  is_it2cl_convertible    
	//= std::is_convertible<typename iterator_traits<It>::value_type,   cl_elem_type<Ct>>;
//	= std::is_convertible<typename iterator_traits<It>::value_type,   cl_elem_type<Ct>>;


template<typename Cl>	using  cl_elem_fwd  =  typename  copy_rcv<Cl&&, cl_elem_type<Cl>>::type;

/////////////////////////////////////////////////////////////////////////////////////////////////  DEF_HAS_ ...

#define DEF_HAS_MEMBER(NAME,MEMBER)										\
		template <typename T>										\
	struct NAME {												\
		template <typename U,  typename M = typename U::MEMBER>		static char	test(U* u);	\
		template <typename U>						static long	test(...);	\
		enum { value = sizeof test <rm_qualifier<T>> (0) == 1 };					\
	}; 


#define DEF_HAS_METHOD(NAME,METHOD)										\
		template <typename T>										\
	struct NAME {												\
		template <typename U,  typename F = decltype (((U*)0)->METHOD)>	static char	test(U* u);	\
		template <typename U>						static long	test(...);	\
		enum { value = sizeof test <rm_qualifier<T>> (0) == 1 };					\
	}; 



DEF_HAS_MEMBER(has_iterator,iterator)
DEF_HAS_MEMBER(has_iterator_category,iterator_category)
DEF_HAS_MEMBER(has_value_type,value_type)
DEF_HAS_MEMBER(has_mapped_type,mapped_type)

DEF_HAS_METHOD(has_push_front,push_front(typename U::value_type()))
DEF_HAS_METHOD(has_push_back,push_back(typename U::value_type()))
DEF_HAS_METHOD(has_insert,insert(typename U::value_type()))
//DEF_HAS_METHOD(is_functor,std::function<bool(const typename U::value_type&)>::type(typename U::value_type()))
DEF_HAS_METHOD(has_pop_back,pop_back())
DEF_HAS_METHOD(has_pop_front,pop_front())
DEF_HAS_METHOD(has_size,size())
DEF_HAS_METHOD(has_empty,empty())
DEF_HAS_METHOD(has_resize,resize(size_t()))


//////////////////////////////////////////////////////////////////////////////////////// IS_CONTAINER

	template <typename T>
struct is_container {

		template <
			typename U,
			typename S = decltype (((U*)0)->size()),
			typename I = typename U::const_iterator
		>
		static char
	test(U* u);

		template <typename U>
		static long
	test(...);

	enum { value  =  sizeof test <rm_qualifier<T>> (0) == 1 };
};

template<typename T, size_t N>	struct  is_container <T[N]>		: std::true_type { };
template<typename T, size_t N>	struct  is_container <T(&)[N]>		: std::true_type { };
template<typename T, size_t N>	struct  is_container <std::array<T,N>>	: std::true_type { };


template <typename Ct>		struct container_iterator	{ typedef	typename Ct::iterator	type; };
template <typename T, size_t N>	struct container_iterator<T[N]>	{ typedef	T*			type; };

//////////////////////////////////////////////////////////////////////////////////////  IS STRING
template<typename T>	struct  is_string		: std::false_type {};
template<>		struct  is_string <std::string>	: std::true_type  {};

//////////////////////////////////////////////////////////////////////////////////////  IS_IOABLE
template<typename T>	struct  is_ioable 		: std::conditional<std::is_arithmetic<T>::value, std::true_type, std::false_type>::type  {};
template<typename T,typename CT,typename AL>	struct  is_ioable <std::basic_string<T,CT,AL>>	: std::true_type  {};
template<size_t N>	struct  is_ioable <char[N]>	: std::true_type  {};
template<size_t N>	struct  is_ioable <const char[N]>: std::true_type  {};
template<>		struct  is_ioable <char*>	: std::true_type  {};
template<>		struct  is_ioable <const char*>	: std::true_type  {};


//////////////////////////////////////////////////////////////////////////////////////  IS_STACK
	template <typename T>
struct is_stack {

		template <
			typename U,
			//typename PUSH	= decltype (((U*)0)->push(XXX)),
			typename POP	= decltype (((U*)0)->pop()),
			typename TOP	= decltype (((U*)0)->top())
		>
		static char
	test(U* u);

		template <typename U>
		static long
	test(...);

	enum { value = sizeof test<typename std::remove_reference<T>::type>(0) == 1 };
};


//////////////////////////////////////////////////////////////////////////////////////  IS_QUEUE
	template <typename T>
struct is_queue {

		template <
			typename U,
			typename POP	= decltype (((U*)0)->pop()),
			//typename PUSH	= decltype (((U*)0)->push(XXX)),
			typename FRONT	= decltype (((U*)0)->front()),
			typename BACK	= decltype (((U*)0)->back())
		>
		static char
	test(U* u);

		template <typename U>
		static long
	test(...);

	enum { value = sizeof test<T>(0) == 1 };
};


//////////////////////////////////////////////////////////////////////////////////////  IS_ITERATOR
// iterator_reference<T>::type operator*(void) const;
// iterator_pointer<T>::type operator->(void) const;
// T & operator++(void);
// T operator++(int);

/*
	template<typename T>
struct is_iterator { //: std::enable_if<std::is_same<T, decltype(std::declval<T>()++)>::value, std::true_type>::type {
					static char				test (...);	// anything else
	template<typename  U>		static void *				test (decltype((*std::declval<U>)()++)**);	// Pointer
	//template<typename  U, size_t N>	static void *			test (decltype(std::declval<U>()++)(*)[N]);	// C-array
	//template<typename  U>		static void*				test (U**);					// Pointer
	template<typename  U>		static typename U::iterator_category*	test (U*);					// Iterator

	static const bool value = sizeof(test((typename std::remove_reference<T>::type*)nullptr)) == sizeof(void *);
};
*/
	template<typename T>
struct is_iterator {
	static const bool value =
		has_iterator_category<T>::value  ||
		(std::is_pointer<T>::value  &&  ! std::is_function<typename std::remove_pointer<T>::type>::value)
	; 
};


	template<typename T>
struct is_input_iterator {
					static char				test (...);	// anything else
	template<typename  U>		static void *				test (decltype(std::declval<U>()++)**);	// Pointer
	template<typename  U, size_t N>	static void *				test (decltype(std::declval<U>()++)(*)[N]);	// C-array

	template<typename  U>		static typename
		std::enable_if<std::is_same<typename U::iterator_category, std::input_iterator_tag>::value, typename U::iterator_category*>::type
										test (U*);	// Iterator

	static const bool value = sizeof(test((typename std::remove_reference<T>::type*)nullptr)) == sizeof(void *);
};




/* TODO
 
template<typename T> struct is_input_iterator:         is_iterator<T> { template<typename U>  static typename U::input_iterator*          test (U*); };
template<typename T> struct is_output_iterator:        is_iterator<T> { template<typename U>  static typename U::output_iterator*         test (U*); };
template<typename T> struct is_forward_iterator:       is_iterator<T> { template<typename U>  static typename U::forward_iterator*        test (U*); };
template<typename T> struct is_bidirectional_iterator: is_iterator<T> { template<typename U>  static typename U::bidirectional_iterator*  test (U*); };
template<typename T> struct is_random_access_iterator: is_iterator<T> { template<typename U>  static typename U::random_access_iterator*  test (U*); };
*/

////////////////////////////////////////////////////////////////////////////////////////  IS_CALLABLE
template<typename F, typename Signature> struct is_callable;

		template<typename F, typename R, typename... Args>
struct is_callable<F, R(Args...)> {
		template<typename>   static char
	test(...);
		template<typename U> static
		typename std::enable_if<
			std::is_same<decltype(std::declval<U>()(std::declval<Args>()...)), R>::value,
			void*
		>::type
	test(int);
	static const bool value = (sizeof(test<F>(0)) == sizeof(void*));
};


#endif
