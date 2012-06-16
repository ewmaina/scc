/*
is_pointer -- http://stackoverflow.com/questions/3177686/how-to-implement-is-pointer

*/
#include "scc/simple.h"
#include "scc/allocator.h"
#include "scc/scc.h"

#include <type_traits>
#include <functional>
using namespace std;


template<typename Cl>
using  forward_cl_elem  =  typename  copy_rcv<Cl&&, cl_value_type<Cl>>::type;


int main() {

	//cout <<  endl <<   left << setw(26)<<""        <<   " --- --- --- --- --- ---                  able     2  \n";

	#define for_T(name)  __ setw(26) << name << "       \t"\
		<<    (is_const<remove_reference<T>::type>    ::value  ? "C" : "")\
		<<    (is_volatile<T> ::value  ? "V" : "")\
		<<    (is_reference<T>::value && !is_rvalue_reference<T>::value   ? "&" : "")\
		<<    (is_rvalue_reference<T>::value  ? "&&" : "")\
	;

         #define LVV_STR(x) __LVV_STR__(x)
         #define __LVV_STR__(x) #x



cout << "----------------------------------------------------------  NOP\n";
#define DO_NOP(type)	 { typedef type  T;  for_T(LVV_STR(type));  }
	DO_NOP(const int)
	DO_NOP(volatile int)
	DO_NOP(const volatile int)
	DO_NOP(int&&)
	DO_NOP(const int&&)
	DO_NOP(int&)
	DO_NOP(const int&)

cout << "----------------------------------------------------------  NOP - VECTOR VALUE TYPE\n";
#define DO_NOP_V_VT(type)	 { typedef type  T;  for_T(LVV_STR(type));  }
	DO_NOP_V_VT(const vint::value_type)
	DO_NOP_V_VT(volatile vint::value_type)
	DO_NOP_V_VT(const volatile vint::value_type)
	DO_NOP_V_VT(vint::value_type&&)
	DO_NOP_V_VT(const vint::value_type&&)
	DO_NOP_V_VT(vint::value_type&)
	DO_NOP_V_VT(const vint::value_type&)

cout << "----------------------------------------------------------  NOP - VECTOR CL_TRAITS<>::VALUE TYPE\n";
#define DO_NOP_V_VT(type)	 { typedef type  T;  for_T(LVV_STR(type));  }
	DO_NOP_V_VT(cl_value_type<const vint>)
	DO_NOP_V_VT(cl_value_type<volatile vint>)
	DO_NOP_V_VT(cl_value_type<const volatile vint>)
	DO_NOP_V_VT(cl_value_type<vint>&&)
	DO_NOP_V_VT(cl_value_type<const vint>&&)
	DO_NOP_V_VT(cl_value_type<vint>&)
	DO_NOP_V_VT(cl_value_type<const vint>&)

cout << "----------------------------------------------------------  FORWARD_CL_ELEM\n";
#define DO_FWD_CL_ELEM(type)	 { typedef type  T;  for_T(LVV_STR(type));  }
	DO_FWD_CL_ELEM(forward_cl_elem<const vint>)
	DO_FWD_CL_ELEM(forward_cl_elem<volatile vint>)
	DO_FWD_CL_ELEM(forward_cl_elem<const volatile vint>)
	DO_FWD_CL_ELEM(forward_cl_elem<vint>&&)
	DO_FWD_CL_ELEM(forward_cl_elem<const vint>&&)
	DO_FWD_CL_ELEM(forward_cl_elem<vint>&)
	DO_FWD_CL_ELEM(forward_cl_elem<const vint>&)

cout << "----------------------------------------------------------  DECL\n";
#define DO_DECL(type)	 { typedef decltype((std::declval<type>()))  T;  for_T(LVV_STR(type));  }

	DO_DECL(const int)
	DO_DECL(volatile int)
	DO_DECL(const volatile int)
	DO_DECL(int&&)
	DO_DECL(const int&&)
	DO_DECL(int&)
	DO_DECL(const int&)

cout << "----------------------------------------------------------  *(*T)x \n";
#define DO_PTR(type)	 { typedef decltype(*(type*)0)  T;  for_T(LVV_STR(type));  }

	DO_PTR(const int)
	DO_PTR(volatile int)
	DO_PTR(const volatile int)
	//DO_PTR(int&&)
	//DO_PTR(const int&&)
	//DO_PTR(int&)
	//DO_PTR(const int&)

cout << "----------------------------------------------------------  MOVE\n";
#define DO_MOVE(type)	 { typedef decltype(std::move(std::declval<type>()))  T;  for_T(LVV_STR(type));  }

	DO_MOVE(const int)
	DO_MOVE(volatile int)
	DO_MOVE(const volatile int)
	DO_MOVE(int&&)
	DO_MOVE(const int&&)
	DO_MOVE(int&)
	DO_MOVE(const int&)

cout << "----------------------------------------------------------  FORWARD\n";
#define DO_FWD(type)	 { typedef decltype(std::forward<type>(std::declval<type>()))  T;  for_T(LVV_STR(type));  }

	DO_FWD(const int)
	DO_FWD(volatile int)
	DO_FWD(const volatile int)
	DO_FWD(int&&)
	DO_FWD(const int&&)
	DO_FWD(int&)
	DO_FWD(const int&)
}
