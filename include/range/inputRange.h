#pragma once

#include <set>
#include <exception>
#include <utility>

template<typename InType, typename TransformFunc, typename Source>
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

template <typename Source, typename OtherSource, typename Key1Func, typename Key2Func, typename JoinFunc>
struct JoinEnumerable;

template <typename T, typename PredicateFunc, typename Source>
struct WhereEnumerable;

template <typename ZipFunc, typename... Sources>
struct ZipEnumerable;

// Used to deduct enumeration type of an Enumerable
template <class Enumerable>
struct ValueType
{
	typedef decltype(std::declval<Enumerable>().value()) type;
};

template <typename T>
struct IEnumerable
{
	virtual bool moveNext() = 0;
	virtual T value() = 0;
};

template<typename T, typename Derived>
struct InputRange : virtual public IEnumerable<T>
{
	typedef InputRange<T, Derived> InputRangeType;

	Derived save()
	{
		return Derived(*static_cast<Derived*>(this));
	}

	template <typename TransformFunc>
	SelectEnumerable<T, TransformFunc, Derived> select(TransformFunc &&transformFunc)
	{
		return SelectEnumerable<T, TransformFunc, Derived>(*static_cast<Derived*>(this), std::forward<TransformFunc>(transformFunc));
	}

	template <typename SinkFunc>
	void forEach(SinkFunc &&sinkFunction)
	{
		while (this->moveNext())
		{
			sinkFunction(this->value());
		}
	}

	template <typename PredicateFunc>
	bool all(PredicateFunc&& predicateFunction)
	{
		bool ret = true;
		while (this->moveNext())
		{
			ret &= predicateFunction(this->value());
		}
		return ret;
	}

	template <typename PredicateFunc>
	bool any(PredicateFunc&& predicateFunction)
	{
		while (this->moveNext())
		{
			if (predicateFunction(this->value()))
			{
				return true;
			}
		}
		return false;
	}

	template <typename OutType>
	DynamicCastEnumerable<T, OutType, Derived> dynamicCast()
	{
		return DynamicCastEnumerable<T, OutType, Derived>(*static_cast<Derived*>(this));
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
		while (this->moveNext())
		{
			if (this->value() == _value)
			{
				return true;
			}
		}
		return false;
	}

	size_t count()
	{
		size_t count = 0;
		while (this->moveNext())
		{
			++count;
		}
		return count;
	}

	template <typename PredicateFunc>
	size_t count(PredicateFunc&& predicateFunction)
	{
		size_t count = 0;
		while (this->moveNext())
		{
			if (predicateFunction(this->value()))
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

	template <typename PredicateFunc>
	T first(PredicateFunc&& predicateFunction)
	{
		return where(std::forward<PredicateFunc>(predicateFunction)).first();
	}
	template <typename PredicateFunc>
	T firstOrDefault(PredicateFunc&& predicateFunction)
	{
		return where(std::forward<PredicateFunc>(predicateFunction)).firstOrDefault();
	}

	template <typename SetType = std::set<T>, typename OtherSource>
	IntersectEnumerable<T, SetType, Derived, OtherSource> intersect(OtherSource&& otherSource)
	{
		return IntersectEnumerable<T, SetType, Derived, OtherSource>(
			*static_cast<Derived*>(this),
			std::forward<OtherSource>(otherSource));
	}

	template <typename OtherSource, typename Key1Func, typename Key2Func, typename JoinFunc>
	JoinEnumerable <Derived, OtherSource, Key1Func, Key2Func, JoinFunc> join(
		OtherSource&& otherSource,
		Key1Func&& key1Func,
		Key2Func&& key2Func,
		JoinFunc&& joinFunc
		)
	{
		return JoinEnumerable <Derived, OtherSource, Key1Func, Key2Func, JoinFunc>(
			*static_cast<Derived*>(this),
			std::forward<OtherSource>(otherSource),
			std::forward<Key1Func>(key1Func),
			std::forward<Key2Func>(key2Func),
			std::forward<JoinFunc>(joinFunc)
			);
	}

	template <typename PredicateFunc>
	WhereEnumerable<T, PredicateFunc, Derived> where(PredicateFunc&& predicateFunc)
	{
		return WhereEnumerable<T, PredicateFunc, Derived>(
			*static_cast<Derived*>(this),
			std::forward<PredicateFunc>(predicateFunc));
	}

	T last()
	{
		Derived rangeCopy = *static_cast<Derived*>(this);
		if (!rangeCopy.moveNext())
		{
			throw std::runtime_error("Empty enumerator, cant .last()");
		}
		auto t = rangeCopy.value();
		while (rangeCopy.moveNext())
		{
			t = rangeCopy.value();

		}
		return t;
	}

	template <typename PredicateFunc>
	T last(PredicateFunc&& predicateFunc)
	{
		Derived rangeCopy = *static_cast<Derived*>(this);
		bool initialized = false;
		auto t = T();
		while (rangeCopy.moveNext())
		{
			if (predicateFunc(rangeCopy.value()))
			{
				t = rangeCopy.value();
				initialized = true;
			}
		}
		if (!initialized)
		{
			throw std::runtime_error("Empty enumerator, cant .last(filter)");
		}
		return t;
	}

	T lastOrDefault()
	{
		Derived rangeCopy = *static_cast<Derived*>(this);
		auto t = T();
		while (rangeCopy.moveNext())
		{
			t = rangeCopy.value();

		}
		return t;
	}

	template <typename PredicateFunc>
	T lastOrDefault(PredicateFunc&& predicateFunc)
	{
		Derived rangeCopy = *static_cast<Derived*>(this);
		auto t = T();
		while (rangeCopy.moveNext())
		{
			if (predicateFunc(rangeCopy.value()))
			{
				t = rangeCopy.value();
			}
		}
		return t;
	}

	T max()
	{
		Derived rangeCopy = *static_cast<Derived*>(this);
		if (!rangeCopy.moveNext())
		{
			throw std::runtime_error("Cant get max of empty enumeration");
		}

		auto t = rangeCopy.value();
		while (rangeCopy.moveNext())
		{
			if (rangeCopy.value() > t)
			{
				t = rangeCopy.value();
			}
		}
		return t;
	}

	template <typename TransformFunc>
	typename std::result_of< TransformFunc(T)>::type
		max(TransformFunc&& func)	
	{
		Derived rangeCopy = *static_cast<Derived*>(this);
		if (!rangeCopy.moveNext())
		{
			throw std::runtime_error("Cant get max of empty enumeration");
		}

		auto t = func(rangeCopy.value());
		while (rangeCopy.moveNext())
		{
			auto v = func(rangeCopy.value());
			if (v > t)
			{
				t = v;
			}
		}
		return t;
	}

	T min()
	{
		Derived rangeCopy = *static_cast<Derived*>(this);
		if (!rangeCopy.moveNext())
		{
			throw std::runtime_error("Cant get max of empty enumeration");
		}

		auto t = rangeCopy.value();
		while (rangeCopy.moveNext())
		{
			if (rangeCopy.value() < t)
			{
				t = rangeCopy.value();
			}
		}
		return t;
	}

	template <typename TransformFunc>
	typename std::result_of< TransformFunc(T)>::type
		min(TransformFunc&& func)
	{
		Derived rangeCopy = *static_cast<Derived*>(this);
		if (!rangeCopy.moveNext())
		{
			throw std::runtime_error("Cant get max of empty enumeration");
		}

		auto t = func(rangeCopy.value());
		while (rangeCopy.moveNext())
		{
			auto v = func(rangeCopy.value());
			if (v < t)
			{
				t = v;
			}
		}
		return t;
	}

	template <typename OutType>
	auto ofType()
	{
		return dynamicCast<OutType>().where([](auto ptr) { return nullptr != ptr; });
	}

	template <typename ZipFunc, typename ... Sources >
	ZipEnumerable<ZipFunc, Derived, Sources...> zip(ZipFunc&& zipFunc, Sources&&... sources)
	{
		return ZipEnumerable<ZipFunc, Derived, Sources...>(
			std::forward<zipFunc>(zipFunc), 
			*static_cast<Derived*>(this), 
			std::forward<Sources...>(sources)
		);
	}

};

