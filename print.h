
#ifndef  LVV_PRINT_H
#define  LVV_PRINT_H
////////////////////////////////////////////////////////////////////  OUT, OUTLN
#include <type_traits>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <tuple>
#include <sstream>
	using	std::cout;
	using	std::cout;
	using	std::endl;
	using	std::string;
	using	std::ostream;
	using	std::flush;

struct  out {
	bool  first_use;
	const char*	sep;
	const char*	paren;

	out ()					      : first_use(true), sep(0),   paren(0)     {init();};
	out (const char* sep, const char* paren=0) : first_use(true), sep(sep), paren(paren) {init();};
	void init() { if (sep   && !*sep)	sep   = 0; }


	void send_sep() { if (sep && !first_use) cout << sep;  first_use = false; };

	template<typename T>	out&  operator<<  (T x)	{ send_sep();  cout <<         x;	return *this; };
	template<typename T>	out&  operator,   (T x)	{ send_sep();  cout <<         x;	return *this; };
	//template<typename T>	out&  operator|   (T x)	{ send_sep();  cout << " "  << x;	return *this; };
	template<typename T>	out&  operator^   (T x)	{ send_sep();  cout << " " << x;	return *this; };

	// endl, hex, ..
				out&  operator<< (std::ostream&  (*pf) (std::ostream& ) ) { cout << pf;  return *this; };
				out&  operator<< (std::ios&      (*pf) (std::ios&     ) ) { cout << pf;  return *this; };
				out&  operator<< (std::ios_base& (*pf) (std::ios_base&) ) { cout << pf;  return *this; };
				out&  operator,  (std::ostream&  (*pf) (std::ostream& ) ) { cout << pf;  return *this; };
				out&  operator,  (std::ios&      (*pf) (std::ios&     ) ) { cout << pf;  return *this; };
				out&  operator,  (std::ios_base& (*pf) (std::ios_base&) ) { cout << pf;  return *this; };

	// sequance container
		template<typename T, template<typename T> class  Al, template<typename T, typename Al> class Ct >
		out&
	operator<<      (const Ct<const T, Al<const T> >& C) {
		if (paren && *paren)  	cout << paren[0];
		else			cout << '{';

		for (int i=0;   i < int(C.size());   i++)	{
			if(i && C.size()) cout  << (sep ? sep : ", ");  cout << C[i];
		}

		if (paren && *paren)	cout << paren[1];
		else 			cout << '}';

		return *this;
	};


	// C-array
		template<class T, std::size_t N>
		typename std::enable_if<!std::is_same<T, char>::value,  out&>::type
	operator<<      (const T (&A)[N]) {
		cout << "{";
			int i=0;
			for (; i<(int)N-1;  i++)	cout << A[i] <<  ", ";
			cout << A[i];
		cout << "}";
		return *this;
	};



	// alias "," for ">>"
		template<typename T, template<typename T> class  Al, template<typename T, typename Al> class Ct >
		out&
	operator,       (const Ct<T, Al<T> >& C) {  return operator<<(C); }

	operator bool   () {  return true; }
 };

struct  outln : out  {
	outln(const char* sep=0, const char* paren=0)	:out(sep, paren)	{};
	~outln()				{ cout << endl; }
 };

#define		_    out()   <<
#define		__   outln() <<


///////////////////////////////////////////////////////////////////// PRINT ANY CONTAINER

// Print any C-array

               template<class T, std::size_t N>
                                                               // disable C-array print for  char[]
               typename std::enable_if< !std::is_same<T, char>::value,  std::ostream&>::type
	operator<<      (ostream& os, const T (&A)[N]) {
		cout << "{";
			int i=0;
			for (; i<(int)N-1;  i++)	os << A[i] <<  ", ";
			os << A[i];
		cout << "}";
		return os;
	};



// print any std::sequance-containter<printable>
	template<typename T, template<typename T, typename Al> class Ct >
	std::ostream&
operator<<      (ostream& os, const Ct<T, std::allocator<T> >& C) {
	cout << "{";
		auto it=C.begin();
		for (int i=0;   i < int(C.size())-1;   i++, it++)	os  << *it <<  ", ";
		if (!C.empty())  					os  << *it;
	cout << "}";
        return os;
};


// SET/MULTISET -- print any std::set<printable>  with std comparator and allocator
	template<
		typename T,
		template<typename T>  class  Al,
		template<typename T>  class  Cmp,
		template<typename T, typename Cmp, typename Al> class Ct
	>


	typename std::enable_if<
		!std::is_same <Ct<T,Cmp<T>,Al<T>>,  std::string>::value,
		std::ostream&
	>::type

	//std::ostream&
operator<<      (ostream& os, const Ct<T,Cmp<T>,Al<T>>& C) {
	cout << "{";
	auto it=C.begin();
        for (int i=0;   i < int(C.size())-1;   i++, it++)		os  << *it <<  ", ";
	if (!C.empty())  						os  << *it;
	cout << "}";
        return os;
};



// MAP -- print any std::map<printable, printable>  with std comparator and allocator
	template<
		typename K, typename V,
		template<typename K> class Cmp,
		template<typename Pr> class Al,
		template<typename K, typename V, typename Cmp,
		typename Al> class Ct
	>
	std::ostream&
operator<<      (ostream& os, const Ct<K, V, Cmp<K>, Al<std::pair<const K,V>>>& C) {
//operator<<      (ostream& os, const map<K, V, std::less<K>, std::allocator<std::pair<const K,V> > > & C) {
	cout << "{";
	auto it = C.begin();
        for (int i=0;   i < int(C.size())-1;   i++, it++)		os  << *it <<  ", ";
	if (!C.empty())  						os  << *it;
	cout << "}";
        return os;
};




// PAIR -- print any std::pair<printable1, printable2>

		template<typename T, typename U>
		std::ostream&
operator<<      (ostream& os, const std::pair<T,U>& p) {
	os << "⟨" << p.first << "," << p.second <<"⟩";
	return os;
};


// TUPLE -- print any std::tuple<printable ...>
// 	tr1::tuple() used to be printable, but now with gcc460 - it is not.

	using	std::tuple;
	using	std::tuple_size;
	using	std::get;



			template< int RI, typename... TT>
	struct	print_tuple_elem  {
		print_tuple_elem (ostream& os, const tuple<TT...>& tup)  {
			const size_t  tsize = tuple_size<tuple<TT...>>::value;
			const size_t  i = tsize - RI;
			os <<  get<i>(tup);
			if  (i != tsize-1)   cout << ", ";
			print_tuple_elem<RI-1, TT...>(os, tup);
		};
	};

			template<typename... TT>
	struct	print_tuple_elem <0, TT...> {
		print_tuple_elem (ostream& os, const tuple<TT...>& tup)  {};
	};


		template<typename... TT>
		std::ostream&
operator<<      (ostream& os, const tuple<TT...>& tup) {
	const size_t  tsize = tuple_size<tuple<TT...>>::value;
	os << "⟨";     print_tuple_elem<tsize, TT...>(os, tup);    os << "⟩";
	return os;
};
	// Using C++0x Variadic Templates to Pretty-Print Types
	// 	-- http://mlangc.wordpress.com/2010/04/18/using-c0x-variadic-templates-to-pretty-print-types/

#endif	// LVV_PRINT_H
