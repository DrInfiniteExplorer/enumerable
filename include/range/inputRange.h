#pragma once

#include <set>

template <typename T>
struct InputRange;

template<typename InType, typename Func>
struct SelectEnumerable;

template<typename InType, typename OutType>
struct DynamicCastEnumerable;
template<typename InType, typename OutType>
struct StaticCastEnumerable;
template<typename InType, typename OutType>
struct ReinterpretCastEnumerable;
template<typename InType, typename OutType>
struct ConstCastEnumerable;

template<typename T>
struct ConcatEnumerable;

template<typename T>
struct DefaultIfEmptyEnumerable;

template<typename T, typename TT>
struct ValueIfEmptyEnumerable;

template<typename T, typename SetType>
struct DistinctEnumerable;

template<typename T>
struct InputRange
{
	virtual void restart() = 0;

	virtual bool empty() = 0;
	virtual T front() = 0;
	virtual void popFront() = 0;

	template <typename Func>
	SelectEnumerable<T, Func> select(Func &&t)
	{
		return SelectEnumerable<T, Func>(*this, std::forward<Func>(t));
	}

	template <typename Func>
	void forEach(Func &&func)
	{
		restart();
		while (!empty())
		{
			func(front());
			popFront();
		}
	}

	template <typename Func>
	bool all(Func&& func)
	{
		restart();
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
		restart();
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
	DynamicCastEnumerable<T, OutType> dynamicCast()
	{
		return DynamicCastEnumerable<T, OutType>(*this);
	}
	template <typename OutType>
	StaticCastEnumerable<T, OutType> staticCast()
	{
		return StaticCastEnumerable<T, OutType>(*this);
	}
	template <typename OutType>
	ReinterpretCastEnumerable<T, OutType> reinterpretCast()
	{
		return ReinterpretCastEnumerable<T, OutType>(*this);
	}
	template <typename OutType>
	ConstCastEnumerable<T, OutType> constCast()
	{
		return ConstCastEnumerable<T, OutType>(*this);
	}

	ConcatEnumerable<T> concat(InputRange<T>&& other)
	{
		return ConcatEnumerable<T>(*this, other);
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

	DefaultIfEmptyEnumerable<T> defaultIfEmpty()
	{
		return DefaultIfEmptyEnumerable<T>(*this);
	}

	template <typename TT>
	ValueIfEmptyEnumerable<T, TT> valueIfEmpty(TT&& value)
	{
		return ValueIfEmptyEnumerable<T, TT>(*this, std::forward<TT>(value));
	}

	template <typename SetType = std::set<T> >
	DistinctEnumerable<T, SetType> distinct()
	{
		return DistinctEnumerable<T, SetType>(*this);
	}
};

