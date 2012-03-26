// File EPL_traits.h -- special template metafunctions used to test Project2
#pragma once

#ifndef _EPL_traits_h
#define _EPL_traits_h

#include <complex>
#include <vector>
#include <string>

/* this metafunctions in this file will require forward declarations of all the template classes used
 * in your soultion to project 2 (or at least a few of them)
 * So, you can either #include "Valarray.h" or you can provide the forward declarations.
 * I have chosen to do the latter. Please fix this so that it works with your solution
 */

// #include "Valarray.h" // -- I'm not using this method, I'm forward-declaring Valarray<T, R> instead

template <typename T, typename REP = std::vector<T> > class Valarray;

/* the default is empty
 * in that way, if EPL_traits is used on an unsupported type, then we'll have an compile-time error
 */
template <typename T> struct EPL_traits { };

template <> struct EPL_traits<void> { // some useful constants, exported to the real metafunctions below
	enum {
		INT = 1,
		FLOAT = 2,
		DOUBLE = 3
	};
	static std::string baseTypeName(const int s) {
		switch (s) { // s will be the SRank
		case INT: return "int";
		case FLOAT: return "float";
		case DOUBLE: return "double";
		default: return "unrecognized scalar base type";
		}
	}
};

template <> struct EPL_traits<int> {
	enum {
		SRank = EPL_traits<void>::INT,
		CRank = false,
		VRank = false
	};
};

template <> struct EPL_traits<float> {
	enum {
		SRank = EPL_traits<void>::FLOAT,
		CRank = false,
		VRank = false
	};
};

template <> struct EPL_traits<double> {
	enum {
		SRank = EPL_traits<void>::DOUBLE,
		CRank = false,
		VRank = false
	};
};

template <typename T> struct EPL_traits<std::complex<T> > {
	enum {
		SRank = EPL_traits<T>::SRank,
		CRank = true,
		VRank = false
	};
};



/* YOU MUST EDIT ALL THE FOLLOWING SPECIALIZATION(S) TO ENSURE EPL_traits WORKS WITH YOUR SOLUTION */

/* You must create a specialization for every possible Valarray and Valarray expression
 * So, if operator+(Valarray, Valarray) returns a BinOp<X, Y, Z> type, then you must create
 * a (partial) specialization for EPL_traits<BinOp<X, Y, Z>> 
 * In that specialization, VRank would be true, CRank and SRank would be determined by 
 * the value_type of your BinOp.
 *
 * NOTE: in my solution, I wrap all my BinOps and other proxy objects (e.g., scalar proxies)
 * in a Valarray wrapper. To do that, Valarray becomes a two-argument template: Valarray<T, R>
 * Since I've done that, the result type for any Valarray expression becomes Valarary<T, R>
 * as a result, I only have to write one specialization of EPL_traits (shown below). You may 
 * decide you can't (or don't want) to use this specialization. If so, delete it.
 * 
 * Recall, for any Valarray<T> and for any legal expression of Valarray<T> objects (including 
 * expressions that use sum(), apply(), accumulate and scalars), EPL_traits must work for the result type
 * of that expression and EPL_traits must export SRank, CRank and VRank correctly.
 */
template <int R>
struct SType{
};

template<>struct SType<1> { typedef int RET ;};
template<>struct SType<2> { typedef float RET ;};
template<>struct SType<3> { typedef double RET ;};


template <int C, typename T> struct CType {};
template <typename T> struct CType<0, T> { typedef T RET; };
template <typename T> struct CType<1, T> { typedef std::complex<T> RET; };


template <int C, typename T> struct VType {};
template <typename T> struct VType<0, T> { typedef T RET; };
template <typename T> struct VType<1, T> { typedef Valarray<T> RET; };

template <typename T, typename R> struct EPL_traits<Valarray<T, R> > {
	enum {
		SRank = EPL_traits<T>::SRank,
		CRank = EPL_traits<T>::CRank,
		VRank = true
	};
};

template <typename T1, typename T2>
struct anand_traits {
	enum { MySRank = (EPL_traits<T1>::SRank > EPL_traits<T2>::SRank) ? EPL_traits<T1>::SRank : EPL_traits<T2>::SRank };
	typedef typename SType<MySRank>::RET MySType;
    
	enum { MyCRank = (EPL_traits<T1>::CRank > EPL_traits<T2>::CRank) ? EPL_traits<T1>::CRank : EPL_traits<T2>::CRank };
	typedef typename CType<MyCRank, MySType>::RET MyCType;
    
    enum { MyVRank = (EPL_traits<T1>::VRank > EPL_traits<T2>::VRank) ? EPL_traits<T1>::VRank : EPL_traits<T2>::VRank };
    
    typedef typename VType<MyVRank, MyCType>::RET MyVType;
    
	typedef MyVType RET;
};

template<typename T>
struct SRank
{
    enum { RET = EPL_traits<T>::SRank };
};

template<typename T>
struct CRank
{
   enum { RET = EPL_traits<T>::CRank };
};

template<typename T>
struct VRank
{
    enum { RET = EPL_traits<T>::VRank };
};

#endif _EPL_traits_h