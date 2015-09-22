#pragma once
#include <cstdlib>
#include <new>
#include <utility>


#define _POINTER_X(T, A)	T*
#define _REFERENCE_X(T, A)	T&
#define _DESTRUCTOR(ty, ptr)	(ptr)->~ty()
#define _PDFT	ptrdiff_t
#define _SIZT	size_t

template<class _Ty> inline
_Ty   *_Allocate(_PDFT _N, _Ty*)
{
	if (_N < 0)
		_N = 0;
	return ((_Ty   *)operator new(
		(_SIZT)_N * sizeof(_Ty)));
}

template<class _T1, class _T2> inline
void _Construct(_T1   *_P, const _T2& _V)
{
	new ((void   *)_P) _T1(_V);
}

template<class _Ty> inline
void Destroy(_Ty   *_P)
{
	_DESTRUCTOR(_Ty, _P);
}
inline void Destroy(char   *_P)
{}
inline void Destroy(wchar_t   *_P)
{}


template<class Ty>
class my_allocate
{
public:
	typedef  Ty   *pointer;
	typedef  const Ty   *const_pointer;
	typedef  Ty  & reference;
	typedef  const Ty  & const_reference;
	typedef  Ty value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

public:
	pointer address(reference _X) const
	{
		return (&_X);
	}
	const_pointer address(const_reference _X) const
	{
		return (&_X);
	}
	
	_SIZT max_size() const
	{
		_SIZT _N = (_SIZT)(-1) / sizeof(_Ty);
		return (0 < _N ? _N : 1);
	}
	pointer allocate(size_type _N, const void *)
	{
		return (_Allocate((difference_type)_N, (pointer)0));
	}
	char   *_Charalloc(size_type _N) //×Ö·û·ÖÅä
	{
		return (_Allocate((difference_type)_N,
			(char   *)0));
	}
	void deallocate(void   *_P, size_type)
	{
		operator delete(_P);
	}

	void construct(pointer _P, const Ty& _V)
	{
		_Construct(_P, _V);
	}
	void destroy(pointer _P)
	{
		Destroy(_P);
	}

};


template<class Ty, class U> inline
bool operator==(const my_allocate<Ty>&, const my_allocate<U>&)
{
	return (true);
}
template<class Ty, class U> inline
bool operator!=(const my_allocate<Ty>&, const my_allocate<U>&)
{
	return (false);
}
