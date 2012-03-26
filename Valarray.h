// this file intentionally left blank

#pragma once

#ifndef _Valarray_h
#define _Valarray_h 1

#include "EPL_traits.h"
#include "Expr_impl.h"
#include<cstddef>
#include<iostream>
#include<numeric>


/* Function object for sqrt */

template<typename T>
class SQRT {
public:
    typedef typename std::complex<double> result_type;
    
    result_type operator() (const T a) const
    {
        const result_type retVal = a;
        return sqrt(retVal);
    }
};

template< typename T, typename REP >
class Valarray {
  private:
  			REP expr_rep;
  public:
  	explicit Valarray(size_t s) : expr_rep(s) { }
	Valarray(REP const& rb) : expr_rep(rb) { }
    Valarray(){}
	void push_back(const T& a) { expr_rep.push_back(a); }
	void pop_back(const T& a) { expr_rep.pop_back(a); }

	size_t size() const { return expr_rep.size(); }
	T operator[] (size_t idx) const { return expr_rep[idx]; }
	T& operator[] (size_t idx) { return expr_rep[idx]; }

	REP const& rep() const { return expr_rep; }
	REP& rep() { return expr_rep; }

	typedef T value_type;
    /* Assignment operator */
    
    template<typename T2,typename R2>
    Valarray& operator=(Valarray<T2,R2>const& b)
    {
        for(size_t i = 0;i<std::min(b.size(),size());i++)
            expr_rep[i] = b[i];
        
        return *this;
    }
    
    /* Negation operator */
    Valarray<T, A_UnaryNeg<T, REP> > operator-(void)
    {
		return Valarray<T, A_UnaryNeg<T, REP> >(A_UnaryNeg<T,REP>(this->rep()));
    }
    
    typedef typename REP::iterator iterator;
    typedef typename REP::const_iterator const_iterator;
    
    iterator begin(void){ return expr_rep.begin(); }
    iterator end(void) { return expr_rep.end(); }
    
    const_iterator begin(void) const { return expr_rep.begin(); }
    const_iterator end(void) const { return expr_rep.end(); }
    
    /* Assignment operator for scalar*/
    
    template<typename T1>
    Valarray& operator=(const T1& a)
    {
        for(size_t i=0;i<size();i++)
            expr_rep[i] = a;

		return *this;
    }

    
    friend std::ostream& operator<<(std::ostream& os, const Valarray& val)
    {
        os<<"{";
        for(size_t i =0;i < val.size(); i++)
        {
            if( i == (int)val.size()-1 )
            {
                os<<val[(int)val.size()-1]<<"}"<<std::endl;
                continue;
            }
            os<<val[i]<<", ";
        }
            
        return os;
    }
    
    template<typename UnaryFunc, typename T2>
    typename UnaryFunc::result_type accumulate(const UnaryFunc u, const T2& a) const 
    {
        return std::accumulate(begin(),end(),a,u);
    }
    
    /* sum function */
    
    T sum() const
    {
        T retVal = T();
        
        for(size_t i = 0; i < size() ;i++)
            retVal += expr_rep[i];
        
        return retVal;
    }
    
    /* Well we know it will be complex<double> always */

    Valarray< std::complex<double>, A_Apply<std::complex<double>, REP, SQRT<T> > > sqrt() const
    {
      	return apply(SQRT<T>());  
    }
    
    /* apply function */
    
    template<typename UnaryFunc>
    Valarray<typename UnaryFunc::result_type, A_Apply<typename UnaryFunc::result_type, REP , UnaryFunc > > apply(UnaryFunc u) const
    {
		return Valarray<typename UnaryFunc::result_type, A_Apply<typename UnaryFunc::result_type, REP, UnaryFunc> >(A_Apply<typename UnaryFunc::result_type,REP,UnaryFunc>(this->rep(), u));
    }
};

/* Operations on arrays */
template <typename T1, typename T2, typename R1, typename R2>
Valarray< typename anand_traits<T1,T2>::RET, A_Add<typename anand_traits<T1,T2>::RET,R1,R2> >
operator+ (Valarray<T1,R1> const& a, Valarray<T2,R2> const& b) {
	typedef typename anand_traits<T1,T2>::RET myType;

	return Valarray<myType, A_Add<myType, R1, R2> >(A_Add<myType,R1,R2>(a.rep(),b.rep()));
}

template <typename T1, typename T2, typename R1, typename R2>
Valarray< typename anand_traits<T1,T2>::RET, A_Sub<typename anand_traits<T1,T2>::RET,R1,R2> >
operator- (Valarray<T1,R1> const& a, Valarray<T2,R2> const& b) {
	typedef typename anand_traits<T1,T2>::RET myType;
    
	return Valarray<myType, A_Sub<myType, R1, R2> >(A_Sub<myType,R1,R2>(a.rep(),b.rep()));
}

template <typename T1, typename T2, typename R1, typename R2>
Valarray< typename anand_traits<T1,T2>::RET, A_Mul<typename anand_traits<T1,T2>::RET,R1,R2> >
operator* (Valarray<T1,R1> const& a, Valarray<T2,R2> const& b) {
	typedef typename anand_traits<T1,T2>::RET myType;
    
	return Valarray<myType, A_Mul<myType, R1, R2> >(A_Mul<myType,R1,R2>(a.rep(),b.rep()));
}

template <typename T1, typename T2, typename R1, typename R2>
Valarray< typename anand_traits<T1,T2>::RET, A_Div<typename anand_traits<T1,T2>::RET,R1,R2> >
operator/ (Valarray<T1,R1> const& a, Valarray<T2,R2> const& b) {
	typedef typename anand_traits<T1,T2>::RET myType;
    
	return Valarray<myType, A_Div<myType, R1, R2> >(A_Div<myType,R1,R2>(a.rep(),b.rep()));
}

/* operations on scalar and array and vice-versa */


template <typename T1, typename T2, typename R2>
Valarray< typename anand_traits<T1,T2>::RET, A_Mul<typename anand_traits<T1,T2>::RET, A_Scalar<T1>, R2 > > 
operator* (T1 const& a, Valarray<T2,R2> const& b) {
	typedef typename anand_traits<T1,T2>::RET myType;
    
	return Valarray<myType, A_Mul<myType, A_Scalar<T1>, R2 > >(A_Mul<myType,A_Scalar<T1>, R2 >(A_Scalar<T1>(a), b.rep()));
}


template <typename T1, typename T2, typename R1>
Valarray< typename anand_traits<T1,T2>::RET, A_Mul<typename anand_traits<T1,T2>::RET,R1,A_Scalar<T2> > >
operator* (Valarray<T1,R1> const& a, T2 const& b) {
	typedef typename anand_traits<T1,T2>::RET myType;
    
	return Valarray<myType, A_Mul<myType, R1, A_Scalar<T2> > >(A_Mul<myType,R1,A_Scalar<T2> >(a.rep(),A_Scalar<T2>(b)));
}


template <typename T1, typename T2, typename R2>
Valarray< typename anand_traits<T1,T2>::RET, A_Add<typename anand_traits<T1,T2>::RET, A_Scalar<T1>, R2 > > 
operator+ (T1 const& a, Valarray<T2,R2> const& b) {
	typedef typename anand_traits<T1,T2>::RET myType;
    
	return Valarray<myType, A_Add<myType, A_Scalar<T1>, R2 > >(A_Add<myType,A_Scalar<T1>, R2 >(A_Scalar<T1>(a), b.rep()));
}


template <typename T1, typename T2, typename R1>
Valarray< typename anand_traits<T1,T2>::RET, A_Add<typename anand_traits<T1,T2>::RET,R1,A_Scalar<T2> > >
operator+ (Valarray<T1,R1> const& a, T2 const& b) {
	typedef typename anand_traits<T1,T2>::RET myType;
    
	return Valarray<myType, A_Add<myType, R1, A_Scalar<T2> > >(A_Add<myType,R1,A_Scalar<T2> >(a.rep(),A_Scalar<T2>(b)));
}

template <typename T1, typename T2, typename R2>
Valarray< typename anand_traits<T1,T2>::RET, A_Sub<typename anand_traits<T1,T2>::RET, A_Scalar<T1>, R2 > > 
operator- (T1 const& a, Valarray<T2,R2> const& b) {
	typedef typename anand_traits<T1,T2>::RET myType;
    
	return Valarray<myType, A_Sub<myType, A_Scalar<T1>, R2 > >(A_Sub<myType,A_Scalar<T1>, R2 >(A_Scalar<T1>(a), b.rep()));
}


template <typename T1, typename T2, typename R1>
Valarray< typename anand_traits<T1,T2>::RET, A_Sub<typename anand_traits<T1,T2>::RET,R1,A_Scalar<T2> > >
operator- (Valarray<T1,R1> const& a, T2 const& b) {
	typedef typename anand_traits<T1,T2>::RET myType;
    
	return Valarray<myType, A_Sub<myType, R1, A_Scalar<T2> > >(A_Sub<myType,R1,A_Scalar<T2> >(a.rep(),A_Scalar<T2>(b)));
}

template <typename T1, typename T2, typename R2>
Valarray< typename anand_traits<T1,T2>::RET, A_Div<typename anand_traits<T1,T2>::RET, A_Scalar<T1>, R2 > > 
operator/ (T1 const& a, Valarray<T2,R2> const& b) {
	typedef typename anand_traits<T1,T2>::RET myType;
    
	return Valarray<myType, A_Div<myType, A_Scalar<T1>, R2 > >(A_Div<myType,A_Scalar<T1>, R2 >(A_Scalar<T1>(a), b.rep()));
}


template <typename T1, typename T2, typename R1>
Valarray< typename anand_traits<T1,T2>::RET, A_Div<typename anand_traits<T1,T2>::RET,R1,A_Scalar<T2> > >
operator/ (Valarray<T1,R1> const& a, T2 const& b) {
	typedef typename anand_traits<T1,T2>::RET myType;
    
	return Valarray<myType, A_Div<myType, R1, A_Scalar<T2> > >(A_Div<myType,R1,A_Scalar<T2> >(a.rep(),A_Scalar<T2>(b)));
}
                                                               
#endif /* End Valarray_h */
