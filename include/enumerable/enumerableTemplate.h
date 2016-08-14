#pragma once

#include <set>
#include <exception>
#include <stdexcept>
#include <utility>

#include "ienumerable.h"

// Forward declarations of the various enumerable types.
// They inherit from EnumerableBase<T> and have those mixed in methods.
// The source sequence object is a member of their instances,
//  so a final sequence objects contain all information required
//  to iterate the sequence.
// For more inforation about their respective implementations see their source files.
// For information about their use see the respective function in EnumerableBase<T>

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

// Used to deduce enumeration type of an Enumerable
template <class Enumerable>
struct ValueType
{
	typedef decltype(std::declval<Enumerable>().value()) type;
};

// This is the base interface which other sequence objects inherit from.
// It inherits from IEnumerable<T> but does not implement anything.
// It's purpose is to provide the interface that is all the methods of the sequence objects.
// Some methods are simple reducers, like .max() or .count() and return single values
// Other methods return new sequence objects(which implement this interface) such as .where(predicate)
//
// The template parameter Derived is used for CRTP (see https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)
//  in order to allow sequences to be composed and chained together as a single object instead
//  of being linked to each other with pointers on the heap.
// Since we know the type (Derived) of the class that inherits from EnumerableBase
//  (and implements the methods of IEnumerable<T>) we can make functions return the concrete class
//  instead of instances of the incomplete type EnumerableBase.
//  The difference is that of returning *this or *dynamic_cast<Derived&>(this). The first is incomplete,
//   the other is known and complete.
template<typename T, typename Derived>
struct EnumerableBase : virtual public IEnumerable<T>
{
	typedef EnumerableBase<T, Derived> EnumerableBaseType;

	// Creates a copy of the sequence object.
	Derived save()
	{
		return Derived(*static_cast<Derived*>(this));
	}

	// select() takes a transformation function which is applied to every element in the sequence.
	// An object representing the sequence of results from the transformation is returned.
	// No processing is performed until the resulting sequence is iterated.
	template <typename TransformFunc>
	SelectEnumerable<T, TransformFunc, Derived> select(TransformFunc &&transformFunc)
	{
		return SelectEnumerable<T, TransformFunc, Derived>(*static_cast<Derived*>(this), std::forward<TransformFunc>(transformFunc));
	}

	// forEach() passes every element in the sequence to the supplied function.
	// forEach consumes the sequence.
	template <typename SinkFunc>
	void forEach(SinkFunc &&sinkFunction)
	{
		while (this->moveNext())
		{
			sinkFunction(this->value());
		}
	}

	// all() applies a predicate to every element in the sequence.
	// all() returns true the predicate is true for every element.
	// all() consumes the sequence.
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

	// any() applies a predicate to ever elment in the sequence.
	// any() returns true if the predicate holds true for at least one element.
	// any() stops processing as soon as the predicate returns true.
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

	// dynamicCast<OutType> returns a sequence of OutType. The sequence may contain nullptr.
	template <typename OutType>
	DynamicCastEnumerable<T, OutType, Derived> dynamicCast()
	{
		return DynamicCastEnumerable<T, OutType, Derived>(*static_cast<Derived*>(this));
	}
	
	// staticCast<OutType> returns a sequence of OutType
	template <typename OutType>
	StaticCastEnumerable<T, OutType, Derived> staticCast()
	{
		return StaticCastEnumerable<T, OutType, Derived>(*static_cast<Derived*>(this));
	}

	// reinterpretCast<OutType> returns a sequence of OutType
	template <typename OutType>
	ReinterpretCastEnumerable<T, OutType, Derived> reinterpretCast()
	{
		return ReinterpretCastEnumerable<T, OutType, Derived>(*static_cast<Derived*>(this));
	}
	
	// constCast<OutType> returns a sequence of OutType
	template <typename OutType>
	ConstCastEnumerable<T, OutType, Derived> constCast()
	{
		return ConstCastEnumerable<T, OutType, Derived>(*static_cast<Derived*>(this));
	}

	// concat(Arg) returns a sequence consisting of the elements in this sequence,
	//  followed by the elements in sequence Arg
	template <typename OtherSource>
	ConcatEnumerable<T, Derived, OtherSource> concat(OtherSource&& other)
	{
		return ConcatEnumerable<T, Derived, OtherSource>(*static_cast<Derived*>(this), other);
	}

	// contains(Value) returns true if the sequence contains an element equal to Value
	// contains() stops processing as soon as an equal value is found.
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

	// count() iterates the whole sequence and returns the number of elements in the sequence.
	// count() consumes the sequence.
	size_t count()
	{
		size_t count = 0;
		while (this->moveNext())
		{
			++count;
		}
		return count;
	}

	// count(Predicate) iterates the whole sequence and returns the number of elements that
	//  satisfy Predicate.
	// count(Predicate) consumes the sequence.
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

	// defaultIfEmpty() returns a sequence with at least one element.
	DefaultIfEmptyEnumerable<T, Derived> defaultIfEmpty()
	{
		return DefaultIfEmptyEnumerable<T, Derived>(*static_cast<Derived*>(this));
	}

	// valueIfEmpty(Value) returns a sequence with at least one element.
	template <typename TT>
	ValueIfEmptyEnumerable<T, TT, Derived> valueIfEmpty(TT&& value)
	{
		return ValueIfEmptyEnumerable<T, TT, Derived>(*static_cast<Derived*>(this), std::forward<TT>(value));
	}

	// distinct() returns a sequence with distinctly separate values.
	// distinct() performs no processing at invocation, but iteration of the sequence object
	//  may cause heap allocations.
	// distinct() uses std::set as the default container to keep track of distinct elements
	template <typename SetType = std::set<T> >
	DistinctEnumerable<T, SetType, Derived> distinct()
	{
		return DistinctEnumerable<T, SetType, Derived>(*static_cast<Derived*>(this));
	}

	// elementAt iterates the sequence a number of steps, then returns the element at that point.
	// elementAt throws a std::out_of_range-exception if the sequence is to short.
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

	// elementAtOrDefault iterates the sequence a number of steps, then returns the element at that point.
	// elementAtOrDefault returns a default-initialized value of the sequence is too short.
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

	// elementAtOrValue iterates the sequence a number of steps, then returns the element at that point.
	// elementAtOrValue returns a supplied value of the sequence is too short.
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

	// except(Other) returns a sequence with elements consisting of those found
	//  in this sequence, except for the elements found in the Other sequence.
	//  One can think of the final sequence as the source sequence, but with elements from Other removed.
	// except(Other) consumes Other on creation, and performs allocation/processing of Other before
	//  iteration of the resulting sequence is started.
	template <typename SetType = std::set<T>, typename OtherSource>
	ExceptEnumerable<T, SetType, Derived, OtherSource> except(OtherSource&& otherSource)
	{
		return ExceptEnumerable<T, SetType, Derived, OtherSource>(
			*static_cast<Derived*>(this),
			std::forward<OtherSource>(otherSource));
	}

	// first() returns the first element in the sequence, or throws std::out_of_range if it is empty.
	T first()
	{
		Derived rangeCopy = *static_cast<Derived*>(this);
		if (!rangeCopy.moveNext())
		{
			throw std::out_of_range("empty source in first");
		}
		return rangeCopy.value();
	}

	// first() returns the first element in the sequence, or a default-initialied value if it is empty.
	T firstOrDefault()
	{
		Derived rangeCopy = *static_cast<Derived*>(this);
		if (!rangeCopy.moveNext())
		{
			return T();
		}
		return rangeCopy.value();
	}

	// first(Predicate) returns the first element in the sequence that fullfills Predicate
	template <typename PredicateFunc>
	T first(PredicateFunc&& predicateFunction)
	{
		return where(std::forward<PredicateFunc>(predicateFunction)).first();
	}
	// first(Predicate) returns the first element in the sequence that fullfills Predicate, or a default value if it is empty.
	template <typename PredicateFunc>
	T firstOrDefault(PredicateFunc&& predicateFunction)
	{
		return where(std::forward<PredicateFunc>(predicateFunction)).firstOrDefault();
	}

	// intersect(Other) returns a sequence of elements found both in this sequence and in Other
	template <typename SetType = std::set<T>, typename OtherSource>
	IntersectEnumerable<T, SetType, Derived, OtherSource> intersect(OtherSource&& otherSource)
	{
		return IntersectEnumerable<T, SetType, Derived, OtherSource>(
			*static_cast<Derived*>(this),
			std::forward<OtherSource>(otherSource));
	}

	// join() returns a sequence of elements that are the result of joining two
	//  sequences. Think of it as the join function in SQL. The key selector functions
	//  are used to select the keys to join on in the source sequences (ex: a.id and b.ownerId)
	//  while joinFunc accepts an element from each sequence and produces a single resulting element
	//  that will be present in the resulting sequence.
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

	// where(Predicate) returns a sequence where only elements passing Predicate are present. 
	template <typename PredicateFunc>
	WhereEnumerable<T, PredicateFunc, Derived> where(PredicateFunc&& predicateFunc)
	{
		return WhereEnumerable<T, PredicateFunc, Derived>(
			*static_cast<Derived*>(this),
			std::forward<PredicateFunc>(predicateFunc));
	}

	// last() returns the last element in the sequence, or throws std::runtime_error if it is empty.
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

	// last(Predicate) returns the last element filfilling Predicate in the sequence,
	// 	or throws std::runtime_error if it is empty.
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

	// lastOrDefault return the last element in the sequence, or a default-initialied value if the sequence is empty.
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

	// lastOrDefault(Predicate) returns the last element in the sequence that fullfills Predicate,
	//  or a default-initialied value if such an element is not found.
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

	// max() reduces the sequence to the larges value in the sequence, or throws std::runtime_error if the sequence is empty.
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

	// max(Transform) reduces the sequence to the larges value(after applying Transform) in the sequence,
	//  or throws std::runtime_error if the sequence is empty.
	// max(Transform) returns the untransformed value.
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

	// min() reduces the sequence to the smallest value in the sequence, or throws std::runtime_error if the sequence is empty.
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

	// min(Transform) reduces the sequence to the smalles value(after applying Transform) in the sequence,
	//  or throws std::runtime_error if the sequence is empty.
	// min(Transform) returns the untransformed value.
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

	// ofType<OutType> returns a sequence of OutType. Much like dynamicCast<OutType> but without nullptr elements.
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
			std::forward<Sources...>(sources...)
		);
	}

};

