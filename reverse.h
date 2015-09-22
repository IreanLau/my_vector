#pragma once

template<class _RI,
class _Ty,
class _Rt = _Ty&,
class _Pt = _Ty *,
class _D = ptrdiff_t>
class reverse_my_iterator {
public:
	typedef reverse_my_iterator<_RI, _Ty, _Rt, _Pt, _D> _Myt;
	typedef _RI iter_type;
	typedef _Rt reference_type;
	typedef _Pt pointer_type;
	reverse_my_iterator()
	{}
	explicit reverse_my_iterator(_RI _X)
		: current(_X) {}
	_RI base() const
	{
		return (current);
	}
	_Rt operator*() const
	{
		return (*(current - 1));
	}
	_Pt operator->() const
	{
		return (&**this);
	}
	_Myt& operator++()
	{
		--current;
		return (*this);
	}
	_Myt operator++(int)
	{
		_Myt _Tmp = *this;
		--current;
		return (_Tmp);
	}
	_Myt& operator--()
	{
		++current;
		return (*this);
	}
	_Myt operator--(int)
	{
		_Myt _Tmp = *this;
		++current;
		return (_Tmp);
	}
	_Myt& operator+=(_D _N)
	{
		current -= _N;
		return (*this);
	}
	_Myt operator+(_D _N) const
	{
		return (_Myt(current - _N));
	}
	_Myt& operator-=(_D _N)
	{
		current += _N;
		return (*this);
	}
	_Myt operator-(_D _N) const
	{
		return (_Myt(current + _N));
	}
	_Rt operator[](_D _N) const
	{
		return (*(*this + _N));
	}
protected:
	_RI current;
};
