#pragma once

#include <set>

template<typename InType, typename Func, typename Source>
struct SelectEnumerable;

template<typename InType, typename OutType, typename Source>
struct DynamicCastEnumerable;
template<typename InType, typename OutType, typename Source>
struct StaticCastEnumerable;
template<typename InType, typename OutType, typename Source>
struct ReinterpretCastEnumerable;
template<typename InType, typename OutType, typename Source>
struct ConstCastEnumerable;

template<typename T, typename Derived, typename OtherDerived>
struct ConcatEnumerable;

template<typename T, typename Source>
struct DefaultIfEmptyEnumerable;

template<typename T, typename TT, typename Source>
struct ValueIfEmptyEnumerable;

template<typename T, typename SetType, typename Source>
struct DistinctEnumerable;

template<typename T, typename Derived>
struct InputRange
{
	virtual bool empty() = 0;
	virtual T front() = 0;
	virtual void popFront() = 0;

	Derived save()
	{
		return Derived(*static_cast<Derived*>(this));
	}

	template <typename Func>
	SelectEnumerable<T, Func, Derived> select(Func &&t)
	{
		return SelectEnumerable<T, Func, Derived>(*static_cast<Derived*>(this), std::forward<Func>(t));
	}

	template <typename Func>
	void forEach(Func &&func)
	{
		while (!empty())
		{
			func(front());
			popFront();
		}
	}

	template <typename Func>
	bool all(Func&& func)
	{
		bool ret = true;
		while (!empty())
		{
			ret &= func(front());
			popFront();
		}
		return ret;
	}

	template <typename Func>
	bool any(Func&& func)
	{
		while (!empty())
		{
			if (func(front()))
			{
				return true;
			}
			popFront();
		}
		return false;
	}

	template <typename OutType>
	DynamicCastEnumerable<T, OutType, Derived> dynamicCast()
	{
		return DynamicCastEnumerable<T, OutType>(*static_cast<Derived*>(this));
	}
	template <typename OutType>
	StaticCastEnumerable<T, OutType, Derived> staticCast()
	{
		return StaticCastEnumerable<T, OutType, Derived>(*static_cast<Derived*>(this));
	}
	template <typename OutType>
	ReinterpretCastEnumerable<T, OutType, Derived> reinterpretCast()
	{
		return ReinterpretCastEnumerable<T, OutType, Derived>(*static_cast<Derived*>(this));
	}
	template <typename OutType>
	ConstCastEnumerable<T, OutType, Derived> constCast()
	{
		return ConstCastEnumerable<T, OutType, Derived>(*static_cast<Derived*>(this));
	}

	template <typename OtherSource>
	ConcatEnumerable<T, Derived, OtherSource> concat(OtherSource&& other)
	{
		return ConcatEnumerable<T, Derived, OtherSource>(*static_cast<Derived*>(this), other);
	}

	bool contains(T&& value)
	{
		while (!empty())
		{
			if (front() == value)
			{
				return true;
			}
			popFront();
		}
		return false;
	}

	size_t count()
	{
		size_t count = 0;
		while (!empty())
		{
			++count;
			popFront();
		}
		return count;
	}

	template <typename Func>
	size_t count(Func&& func)
	{
		size_t count = 0;
		while (!empty())
		{
			if (func(front()))
			{
				++count;
			}
			popFront();
		}
		return count;
	}

	DefaultIfEmptyEnumerable<T, Derived> defaultIfEmpty()
	{
		return DefaultIfEmptyEnumerable<T, Derived>(*static_cast<Derived*>(this));
	}

	template <typename TT>
	ValueIfEmptyEnumerable<T, TT, Derived> valueIfEmpty(TT&& value)
	{
		return ValueIfEmptyEnumerable<T, TT, Derived>(*static_cast<Derived*>(this), std::forward<TT>(value));
	}

	template <typename SetType = std::set<T> >
	DistinctEnumerable<T, SetType, Derived> distinct()
	{
		return DistinctEnumerable<T, SetType, Derived>(*static_cast<Derived*>(this));
	}
};

