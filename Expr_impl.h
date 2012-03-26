#ifndef _Expr_impl_h
#define _Expr_impl_h

#include<climits>

template<typename T>
class A_Scalar {
  private:
	T const& s; // value of the scalar

  public:
    A_Scalar(T const& v) : s(v) { }

	T operator[](size_t) const
	{
		return s;
	}

	size_t size() const
	{
		return INT_MAX;
	}
    class iterator{};
    class const_iterator{};
};

template<typename T>
class reference_traits {
public:
  	typedef T const& RET;
};

template<typename T>
class reference_traits< A_Scalar<T> > {
public:
  	typedef A_Scalar<T> RET;
};

template <typename T,typename OP1, typename OP2 >
class A_Add {
  private:
  	typename reference_traits<OP1>::RET op1;
	typename reference_traits<OP2>::RET op2;

  public:
    A_Add(OP1 const& a, OP2 const& b) : op1(a),op2(b){}

	T operator[] (size_t idx) const
	{
		return (T)op1[idx] + (T)op2[idx];
	}

	size_t size() const
	{
		if(op1.size() == 0)
			return op1.size();
		else if(op2.size()==0)
			return op2.size();
		else
			return std::min(op1.size(),op2.size());
	}
    class iterator{};
    class const_iterator{};
};

template<typename T,typename OP1, typename OP2>
class A_Mul {
  private:
  	typename reference_traits<OP1>::RET op1;
	typename reference_traits<OP2>::RET op2;

  public:
  	A_Mul(OP1 const& a, OP2 const& b) : op1(a),op2(b){ }

	T operator[] (size_t idx) const
	{
		return (T)op1[idx] * (T)op2[idx];
	}
    
    size_t size() const
	{
		if(op1.size() == 0)
			return op1.size();
		else if(op2.size()==0)
			return op2.size();
		else
			return std::min(op1.size(),op2.size());
	}
    
    class iterator{};
    class const_iterator{};

};

template<typename T,typename OP1, typename OP2>
class A_Sub {
  private:
  	typename reference_traits<OP1>::RET op1;
	typename reference_traits<OP2>::RET op2;

  public:
  	A_Sub(OP1 const& a, OP2 const& b) : op1(a),op2(b){ }

	T operator[] (size_t idx) const
	{
		return (T)op1[idx] - (T)op2[idx];
	}
    
    size_t size() const
	{
		if(op1.size() == 0)
			return op1.size();
		else if(op2.size()==0)
			return op2.size();
		else
			return std::min(op1.size(),op2.size());
	}
    
    class iterator{};
    class const_iterator{};
};

template <typename T,typename OP1, typename OP2 >
class A_Div {
private:
  	typename reference_traits<OP1>::RET op1;
	typename reference_traits<OP2>::RET op2;
    
public:
    A_Div(OP1 const& a, OP2 const& b) : op1(a),op2(b){}
    
	T operator[] (size_t idx) const
	{
		return (T)op1[idx] / (T)op2[idx];
	}
    
	size_t size() const
	{
		if(op1.size() == 0)
			return op1.size();
		else if(op2.size()==0)
			return op2.size();
		else
			return std::min(op1.size(),op2.size());
	}
    
    class iterator{};
    class const_iterator{};
};

template<typename T,typename OP1, typename OP2 >
class A_Apply {
private:
	typename reference_traits<OP1>::RET op1;
	OP2 op2;

public:
	A_Apply(OP1 const& a, OP2 const& b):op1(a),op2(b){}

	T operator[] (size_t idx) const
	{
		return (T)op2(op1[idx]);
	}

	size_t size() const
	{
		return op1.size();
	}

	class iterator{};
	class const_iterator{};
};

template<typename T,typename OP1>
class A_UnaryNeg {
private:
	typename reference_traits<OP1>::RET op1;

public:
	A_UnaryNeg(OP1 const& a):op1(a){}

	T operator[] (size_t idx) const
	{
		return -op1[idx];
	}

	size_t size() const
	{
		return op1.size();
	}

	class iterator{};
	class const_iterator{};
};

#endif /* Expr_impl.h */
