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

template<typename T, typename SetType, typename Source, typename OtherSource>
struct ExceptEnumerable;

template<typename T, typename SetType, typename Source, typename OtherSource>
struct IntersectEnumerable;

template <typename Source, typename OtherSource, typename Key1Func, typename Key2Func, typename SelectFunc>
struct JoinEnumerable;


template <typename T, typename Func, typename Source>
struct WhereEnumerable;

template <class IR>
struct ValueType
{
	typedef decltype(std::declval<IR>().value()) type;
};

template<typename T, typename Derived>
struct InputRange
{
	typedef InputRange<T, Derived> InputRangeType;

	virtual bool moveNext() = 0;
	virtual T value() = 0;

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
		while (moveNext())
		{
			func(value());
		}
	}

	template <typename Func>
	bool all(Func&& func)
	{
		bool ret = true;
		while (moveNext())
		{
			ret &= func(value());
		}
		return ret;
	}

	template <typename Func>
	bool any(Func&& func)
	{
		while (moveNext())
		{
			if (func(value()))
			{
				return true;
			}
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

	bool contains(T&& _value)
	{
		while (moveNext())
		{
			if (value() == _value)
			{
				return true;
			}
		}
		return false;
	}

	size_t count()
	{
		size_t count = 0;
		while (moveNext())
		{
			++count;
		}
		return count;
	}

	template <typename Func>
	size_t count(Func&& func)
	{
		size_t count = 0;
		while (moveNext())
		{
			if (func(value()))
			{
				++count;
			}
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

	T elementAt(size_t index)
	{
		Derived rangeCopy = *static_cast<Derived*>(this);

		while (rangeCopy.moveNext())
		{
			if (!index) return rangeCopy.value();
			index--;
		}
		throw std::out_of_range("out_of_range error in elementAt");
	}

	T elementAtOrDefault(size_t index)
	{
		Derived rangeCopy = *static_cast<Derived*>(this);

		while (rangeCopy.moveNext())
		{
			if (!index) return rangeCopy.value();
			index--;
		}
		return T();
	}

	template <typename TT>
	T elementAtOrValue(size_t index, TT&& t)
	{
		Derived rangeCopy = *static_cast<Derived*>(this);

		while (rangeCopy.moveNext())
		{
			if (!index) return rangeCopy.value();
			index--;
		}
		return t;
	}

	// Difference with c# IEnumerable.except: This one does not unique(distinct) as well.
	template <typename SetType = std::set<T>, typename OtherSource>
	ExceptEnumerable<T, SetType, Derived, OtherSource> except(OtherSource&& otherSource)
	{
		return ExceptEnumerable<T, SetType, Derived, OtherSource>(
			*static_cast<Derived*>(this),
			std::forward<OtherSource>(otherSource));
	}

	T first()
	{
		Derived rangeCopy = *static_cast<Derived*>(this);
		if (!rangeCopy.moveNext())
		{
			throw std::out_of_range("empty source in first");
		}
		return rangeCopy.value();
	}

	T firstOrDefault()
	{
		Derived rangeCopy = *static_cast<Derived*>(this);
		if (!rangeCopy.moveNext())
		{
			return T();
		}
		return rangeCopy.value();
	}

	template <typename Func>
	T first(Func&& func)
	{
		return where(std::forward<Func>(func)).first();
	}
	template <typename Func>
	T firstOrDefault(Func&& func)
	{
		return where(std::forward<Func>(func)).firstOrDefault();
	}

	template <typename SetType = std::set<T>, typename OtherSource>
	IntersectEnumerable<T, SetType, Derived, OtherSource> intersect(OtherSource&& otherSource)
	{
		return IntersectEnumerable<T, SetType, Derived, OtherSource>(
			*static_cast<Derived*>(this),
			std::forward<OtherSource>(otherSource));
	}

	template <typename OtherSource, typename Key1Func, typename Key2Func, typename SelectFunc>
	JoinEnumerable <Derived, OtherSource, Key1Func, Key2Func, SelectFunc> join(
		OtherSource&& otherSource,
		Key1Func&& key1Func,
		Key2Func&& key2Func,
		SelectFunc&& selectFunc
		)
	{
		return JoinEnumerable <Derived, OtherSource, Key1Func, Key2Func, SelectFunc>(
			*static_cast<Derived*>(this),
			std::forward<OtherSource>(otherSource),
			std::forward<Key1Func>(key1Func),
			std::forward<Key2Func>(key2Func),
			std::forward<SelectFunc>(selectFunc)
			);
	}

	template <typename Func>
	WhereEnumerable<T, Func, Derived> where(Func&& func)
	{
		return WhereEnumerable<T, Func, Derived>(
			*static_cast<Derived*>(this),
			std::forward<Func>(func));
	}

};

