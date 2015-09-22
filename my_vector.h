#pragma once
#include "my_allocate.h"
#include "reverse.h"
//#include "JJallocated.h"

template<class Ty, class _A = my_allocate<Ty> >
class my_vector {
public:
	typedef my_vector<Ty, _A> _Myt;
	typedef typename _A allocator_type;
	typedef typename _A::pointer _Tptr;
	typedef typename _A::const_pointer _Ctptr;
	typedef typename _A::reference reference;
	typedef typename _A::const_reference const_reference;
	typedef typename _A::value_type value_type;
	typedef typename _A::size_type size_type;
	typedef typename _A::difference_type difference_type;
	typedef _Tptr my_iterator; 
	typedef _Ctptr const_my_iterator;
	typedef reverse_my_iterator<const_my_iterator, value_type,
		const_reference, _Ctptr, difference_type>
		const_reverse_my_iterator;
	typedef reverse_my_iterator<my_iterator, value_type,
		reference, _Tptr, difference_type>
		reverse_my_iterator;

public:
	my_vector(const _A& _Al = _A())	//	= _A()允许使用默认空间配置器
		: allocator(_Al), _First(0), _Last(0), _End(0) {}
	my_vector(size_type _N, const Ty& _V = Ty(),
		const _A& _Al = _A())
		: allocator(_Al)
	{
		_First = allocator.allocate(_N, (void *)0);
		_Ufill(_First, _N, _V);
		_Last = _First + _N;
		_End = _Last;
	}

	my_vector(const _Myt& _X)
		: allocator(_X.allocator)
	{
		_First = allocator.allocate(_X.size(), (void *)0);
		_Last = _Ucopy(_X.begin(), _X.end(), _First);
		_End = _Last;
	}

	typedef const_my_iterator _It;
	my_vector(_It _F, _It _L, const _A& _Al = _A())
		: allocator(_Al), _First(0), _Last(0), _End(0)
	{
		//insert(begin(), _F, _L);
	}
	~my_vector()
	{
		_Destroy(_First, _Last);
		allocator.deallocate(_First, _End - _First);
		_First = 0, _Last = 0, _End = 0;
	}

	_Myt& operator=(const _Myt& _X)	//Q
	{
		if (this == &_X);
		else if (_X.size() <= size())
		{
			my_iterator _S = copy(_X.begin(), _X.end(), _First);
			_Destroy(_S, _Last);
			_Last = _First + _X.size();
		}
		else if (_X.size() <= capacity())
		{
			const_my_iterator _S = _X.begin() + size();
			copy(_X.begin(), _S, _First);
			_Ucopy(_S, _X.end(), _Last);
			_Last = _First + _X.size();
		}
		else
		{
			_Destroy(_First, _Last);
			allocator.deallocate(_First, _End - _First);
			_First = allocator.allocate(_X.size(), (void *)0);
			_Last = _Ucopy(_X.begin(), _X.end(),
				_First);
			_End = _Last;
		}
		return (*this);
	}

	void reserve(size_type _N)
	{
		if (capacity() < _N)
		{
			my_iterator _S = allocator.allocate(_N, (void *)0);
			_Ucopy(_First, _Last, _S);
			_Destroy(_First, _Last);
			allocator.deallocate(_First, _End - _First);
			_End = _S + _N;
			_Last = _S + size();
			_First = _S;
		}
	}
	size_type capacity() const
	{
		return (_First == 0 ? 0 : _End - _First);
	}
	my_iterator begin()
	{
		return (_First);
	}
	const_my_iterator begin() const
	{
		return ((const_my_iterator)_First);
	}
	my_iterator end()
	{
		return (_Last);
	}
	const_my_iterator end() const
	{
		return ((const_my_iterator)_Last);
	}
	reverse_my_iterator rbegin()
	{
		return (reverse_my_iterator(end()));
	}
	const_reverse_my_iterator rbegin() const
	{
		return (const_reverse_my_iterator(end()));
	}
	reverse_my_iterator rend()
	{
		return (reverse_my_iterator(begin()));
	}
	const_reverse_my_iterator rend() const
	{
		return (const_reverse_my_iterator(begin()));
	}
	void resize(size_type _N, const Ty& _X = Ty())
	{
		if (size() < _N)
			insert(end(), _N - size(), _X);
		else if (_N < size())
			erase(begin() + _N, end());
	}
	size_type size() const
	{
		return (_First == 0 ? 0 : _Last - _First);
	}
	size_type max_size() const
	{
		return (allocator.max_size());
	}
	bool empty() const
	{
		return (size() == 0);
	}
	_A get_allocator() const
	{
		return (allocator);
	}
	const_reference at(size_type _P) const
	{
		if (size() <= _P)
			_Xran();
		return (*(begin() + _P));
	}
	reference at(size_type _P)
	{
		if (size() <= _P)
			_Xran();
		return (*(begin() + _P));
	}
	const_reference operator[](size_type _P) const
	{
		return (*(begin() + _P));
	}
	reference operator[](size_type _P)
	{
		return (*(begin() + _P));
	}
	reference front()
	{
		return (*begin());
	}
	const_reference front() const
	{
		return (*begin());
	}
	reference back()
	{
		return (*(end() - 1));
	}
	const_reference back() const
	{
		return (*(end() - 1));
	}
	void push_back(const Ty& _X)
	{
		insert(end(), _X);
	}
	void pop_back()
	{
		erase(end() - 1);
	}
	void assign(_It _F, _It _L)
	{
		erase(begin(), end());
		insert(begin(), _F, _L);
	}
	void assign(size_type _N, const Ty& _X = Ty())
	{
		erase(begin(), end());
		insert(begin(), _N, _X);
	}
	my_iterator insert(my_iterator _P, const Ty& _X = Ty())
	{
		size_type _O = _P - begin();
		insert(_P, 1, _X);	// _P可能改变
		return (begin() + _O);// 返回新插入元素的位置(空间不够，begin()失效)
	}


	void insert(my_iterator _P, size_type _M, const Ty& _X)
	{
		if (_End - _Last < (int)_M)
		{
			size_type _N = size() + (_M < size() ? size() : _M);
			my_iterator _S = allocator.allocate(_N, (void *)0);
			my_iterator _Q = _Ucopy(_First, _P, _S);
			_Ufill(_Q, _M, _X);
			_Ucopy(_P, _Last, _Q + _M);
			_Destroy(_First, _Last);	//析构对象(vector<stirng>)
			allocator.deallocate(_First, _End - _First);
			_End = _S + _N;
			_Last = _S + size() + _M;
			_First = _S;
		}
		else if (_Last - _P < (int)_M)
		{
			_Ucopy(_P, _Last, _P + _M);
			_Ufill(_Last, _M - (_Last - _P), _X);//有构造
			fill(_P, _Last, _X);//无构造
			_Last += _M;
		}
		else if (0 < _M)
		{
			_Ucopy(_Last - _M, _Last, _Last);
			copy_backward(_P, _Last - _M, _Last);//反向
			fill(_P, _P + _M, _X);
			_Last += _M;
		}
	}
	void insert(my_iterator _P, _It _F, _It _L)
	{
		size_type _M = 0;
		//size_type _M = (_L > _F) ? (_L - _F) : 0;  //考虑_L < _F 的情况
		_Distance(_F, _L, _M);

		if (_End - _Last < _M)
		{
			size_type _N = size() + (_M < size() ? size() : _M);
			my_iterator _S = allocator.allocate(_N, (void *)0);
			my_iterator _Q = _Ucopy(_First, _P, _S);
			_Q = _Ucopy(_F, _L, _Q);
			_Ucopy(_P, _Last, _Q);
			_Destroy(_First, _Last);
			allocator.deallocate(_First, _End - _First);
			_End = _S + _N;
			_Last = _S + size() + _M;
			_First = _S;
		}

		/* 计算需要的长度 ， 从_P位置后移M 腾出空间  放置需要的内容*/
		else if (_Last - _P < _M)
		{
			_Ucopy(_P, _Last, _P + _M);
			/*将需要的内容分成两部分，先拷没构造的，再拷贝剩下的*/
			_Ucopy(_F + (_Last - _P), _L, _Last);
			copy(_F, _F + (_Last - _P), _P);

			//_Ucopy(_F, _L, _P);  //可替代上两句，但效率问题

			_Last += _M;
		}
		else if (0 < _M)
		{
			/*将需要的内容分成两部分，没构造的，剩下的	R*/
			_Ucopy(_Last - _M, _Last, _Last);
			copy_backward(_P, _Last - _M, _Last);
			
			//_Ucopy(_P, _Last, _Last);//同上

			copy(_F, _L, _P);
			_Last += _M;
		}
	}

	my_iterator erase(my_iterator _P)
	{
		copy(_P + 1, end(), _P);
		_Destroy(_Last - 1, _Last);
		--_Last;
		return (_P);
	}
	my_iterator erase(my_iterator _F, my_iterator _L)
	{
		my_iterator _S = copy(_L, end(), _F);
		_Destroy(_S, end());
		_Last = _S;
		return (_F);
	}
	void clear()
	{
		erase(begin(), end());
	}

	void swap(_Myt& _X)
	{
		if (allocator == _X.allocator)
		{
			std::swap(_First, _X._First);
			std::swap(_Last, _X._Last);
			std::swap(_End, _X._End);
		}
		else
		{
			_Myt _Ts = *this; 
			*this = _X, _X = _Ts;
		}
	}
	friend void swap(_Myt& _X, _Myt& _Y)
	{
		_X.swap(_Y);
	}

protected:
	my_iterator _First;
	my_iterator _Last;
	my_iterator _End;
	_A allocator;
protected:
	my_iterator _Ucopy(const_my_iterator _F, const_my_iterator _L,
		my_iterator _P)
	{
		for (; _F != _L; ++_P, ++_F)
			allocator.construct(_P, *_F);
		return (_P);
	}
	void _Ufill(my_iterator _F, size_type _N, const Ty &_X)
	{
		for (; 0 < _N; --_N, ++_F)
			allocator.construct(_F, _X);
	}
	void _Xran() const
	{
		_THROW(out_of_range, "invalid vector<T> subscript");
	}

	void _Destroy(my_iterator _F, my_iterator _L)
	{
		for (; _F != _L; ++_F)
			allocator.destroy(_F);
	}
};


