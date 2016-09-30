
CI System | Compilers | Status (master)
---|---|---
Travis CI | gcc, clang | [![Build Status](https://travis-ci.org/DrInfiniteExplorer/enumerable.svg?branch=master)](https://travis-ci.org/DrInfiniteExplorer/enumerable)  
Appveyor | MSVC | [![Build status](https://ci.appveyor.com/api/projects/status/xwopw8a25m6p70b9/branch/master?svg=true)](https://ci.appveyor.com/project/DrInfiniteExplorer/enumerable/branch/master)

# Enumerable
Enumerable is a library inspired by [IEnumerable](https://msdn.microsoft.com/en-us/library/9eekhta0(v=vs.110).aspx) in c#. The twist is that it is implemented in c++ !

Some core concepts in the library is _deffered execution_, _dependency inversion_, _lazy evaluation_. No matter how complex a sequence object is created, there should be no memory allocation or processing performed until the sequence is actually iterated!

* [Installation](#installation)
* [Using](#using)
   * [Creating Enumerable object](#creating-enumerable-object)
   * [Reducing a sequence to a single value](#reducing-a-sequence-to-a-single-value)
   * [Transforms](#transforming-sequences)
* [Methods](#methods)
   * [Sequence transforming functions](#sequence-transforming-functions)
   * [Sequence reducing functions](#sequence-reducing-functions)
* [More details](#more-details)
	

## Installation

The installation procedure for using the library is pretty simple

1. Clone the repository

2. Add the _include_ folder to your include paths.

If you want to contribute, see _Contributing_(TBD) for more detailed instructions.

## Using

Include the header file `<enumerable/enumerable.h>`.

### Creating Enumerable object
If you have a standard container you can use the overloaded function `Enumerable(T)` to obtain an object that implements `EnumerableBase`.

```c++
std::vector<int> myIntVector;
auto intEnumerable = Enumerable(myIntVector);
```

The overloads of Enumerable return an object that reference the source collection(Unless explicitly specified). This means one can add elements to it after creating the enumerable-object. In the following snippet, we populate the source vector and sinks all the elements into the `printInt` function.

```c++
void printInt(int value)
{
   printf("%d\n", value);
}

// Populate source
myIntVector = { 1, 2, 3};

// Call printInt for every element in the sequence
intEnumerable.forEach(printInt);
// Output:
// 1
// 2
// 3
```

### Reducing a sequence to a single value

`forEach` can be considered to be a _sink_ which every member of the sequence is deposited into. Besides _sinks_ there are functions that _reduce_ the sequence into a return value.

```c++
int staticIntArray[4] = { -1, 5, 2, 3};

printf("Max: %d\n", Enumerable(staticIntArray).max());
// Output:
// Max: 5
```

Besides `max` there are `all(pred)`, `any(pred)`, `contains`, `count`, `count(pred)`, various `elementAt(idx), first, last`, `min`, `sequenceEqual(seq)`, and `toContainer`. There is also a very generic `reduce` function which you can use to reduce the sequence however you like!

Some of these mentioned functions take a `pred` parameter, which is a predicate. `count(pred)` only counts the elements that pass the predicate, while `any(pred)` returns true of any element in the sequence passes the predicate.

```c++
bool hasNegative = Enumerable(staticIntArray).any([](int x) { return x < 0;});
size_t positiveCount = Enumerable(staticIntArray).count([](int x){ return x >= 0; });

printf("Has Negative: %d", hasNegtive);
printf("Number of elements with value > 0: %d\n", positiveCount);
// Output:
// Has Negative: 1
// Number of elements with value > 0: 3
```

### Transforming sequences

The really good part comes when we start to apply transformations and filters to our sequences to create new sequence objects!

The simplest transformations are probably `select` and `where`. They implement the standard _map_ and _filter_ operations.

```c++
// Predicate that determines if a value is positive
bool isPositive(int value) { return value >= 0; }

// Take integer radius, return floating point area.
double calculateArea(int radius) { return radius*radius*3.14; }

// Create an object representing the sequence of all positive values in staticIntArray
auto positiveSequence = Enumerable(staticIntArray).where(isPositive);

// Create a sequence of areas values from positiveSequence
auto areaSequence = positiveSequence.select(calculateArea);

for( double area : areaSequence)
{
   printf("Area: %f\n", area);
}

// Output:
// Area: 78.539816339744830961566084581988
// Area: 12.566370614359172953850573533118
// Area: 3.1415926535897932384626433832795
```

In this example, we first filter the sequence with the predicate function _isPositive_. Then we transform the contents of that sequence using `select` with _calculateArea_. Then we use a ranged for loop to enumerate the results. Of special interest is that `select` not only modifies the value but also changes the domain of the value; From an `int` domain to a `double` domain. The sequence object created could of course contain more complex values, for example pointers to class objects on the heap from the source value.

Other methods that return new sequence objects (all of which implement `EnumerableBase`) include `dynamicCast<T>`, `staticCast<T>`, `constCast<T>`, `reinterpretCast<T>`, `ofType<T>`, `concat(seq)`, `defaultIfEmpty`, `valueIfEmpty(value)`, `distinct`, `except(seq)`, `intersect(seq)`, `join(seq, kf1,kf2,jf)`.

## Methods

The list of methods is split into two groups. The first group transforms a sequence into a new sequence. The second group consists of all the other sequences.

### Sequence transforming functions

In the following descriptions, `this` refers to the sequence which operations are applied to. (ex `this.defaultIfEmpty()`)

Method | Description
------ | -----------
**Basic transforms** | ---
`select(transform)` | A sequence where _transform_ is applied to every element. Can change the element type of the sequence.
`where(predicate)` | Filters the sequence, keeping elements that pass `predicate`
`concat(sequence)` | Returns a sequence which consists of the elements of `this` followed by the elements of `sequence`
`skip(count)` | Skips the first `count` elements. [Examples/Tests](test/test_skip.cpp#L13-L35)
`skipWhile(bool Predicate(T))` | Skips elements as long as `Predicate` is true. [Examples/Tests](test/test_take.cpp#L37-L53)
`skipWhile(bool Predicate(T, size_t index))` | Skips elements until `Predicate` returns false. `index` is zero-indexed. [Examples/Tests](test/test_skip.cpp#L55-L77)
`take(count)` | Returns a sequence of up to `count` elements. [Examples/Tests](test/test_take.cpp#L13-L35)
`takeWhile(bool Predicate(T))` | Contains source elements until `Predicate` returns false. [Examples/Tests](test/test_take.cpp#L37-L53)
`takeWhile(bool Predicate(T, size_t index))` | Contains source elements until `Predicate` returns false. `index` is zero-indexed. [Examples/Tests](test/test_take.cpp#L55-L77)
**Type Casts** | ---
`dynamicCast<T>` | Applies `dynamic_cast<T>` to every element in the source sequence.
`staticCast<T>` | Applies `static_cast<T>` to every element in the source sequence.
`constCast<T>` | Applies `const_cast<T>` to every element in the source sequence.
`reinterpretCast<T>` | Applies `reinterpret_cast<T>` to every element in the source sequence.
`ofType<T>` | Applies `dynamic_cast<T>` to every element in the source sequence, skipping elements for which the operation results in `nullptr`
**Set operations** | ---
`distinct` | The values of elements in the resulting sequence are unique; The same value never occurs more than once.
`except(source)` | All the elements in `this` sequence, except for the elements in the `source` sequence.
`intersect(source)` | All the elements which occur in both `this` and `source`.
**Other** | ---
`defaultIfEmpty` | If the source sequence is empty, returns a sequence with a single element with default value. Otherwise the elements from the source sequence are returned.
`valueIfEmpty(value)` | If the source sequence is empty, returns a sequence with a single element with the value `value`. Otherwise the elements from the source sequence are returned.
`join(source, key1(T1), key2(T2), joiner(T1, T2))` | This function _joins_ two sequences(`this` and `source`) into a new sequence. If both the `key` functions produce the same value, then `joiner` is called with the elements from the corresponding sequences and the result of `joiner` is present in the resulting sequence. You can think of it as similar to how tables are joined in SQL. See examples in (TDB)
`makeHeapEnumerable` | Creates a copy of the sequence object which lives on the heap. The returned object is responsible for the lifetime of the heap object. See [More details](#more-details) for further information.


### Sequence reducing functions

The following methods do not return a new sequence. Most of them reduce the sequence to a single value. At the end there are two methods which break this pattern.

Method | Description
------ | -----------
`aggregate` | There is **no** `aggregate` in this version of IEnumerable. See `reduce`. Note that the arguments are reversed when compared to Aggregate.
`bool all(pred)` | Applies the predicate `pred` to every element in the sequence. The function returns true if the predicate is true for every element. Otherwise it returns false.
`bool any(pred)` | Applies the predicate `pred` to every element in the sequence. The function returns true if the predicate is true for any element. Otherwise it returns false.
`bool contains(value)` | Returns true if an element with value `value` can be found in the sequence.
`size_t count()` | Returns the number of elements in the sequence after all filtering and transformations have been applied.
`size_t count(pred)` | Returns the number of elements in the sequence which fullfills the predicate `pred`. Same as if you'd call `this.filter(pred).count()`
`T elementAt(index)` | Returns the value of the element at index `index` in the sequence. Throws std::out_of_range if the sequence is shorter than the supplied index.
`T elementAtOrDefault(index)` | Returns the value of the element at index `index` in the sequence. Returns a default-initialized T if the sequence is shorter than the supplied index.
`T elementAtOrValue(index, value)` | Returns the value of the element at index `index` in the sequence. Returns `value` if the sequence is shorter than the supplied index.
`T first()` | Returns the value of the first element in the sequence. Throws std::out_of_range if the sequence is empty.
`T first(pred)` | Returns the value of the first element in the sequence that fullfills `pred`. Throws std::out_of_range if the sequence is empty or no such element is found.
`T firstOrDefault()` | Returns the value of the first element in the sequence. Returns a default-initialized T if the sequence is empty.
`T firstOrDefault(pred)` | Returns the value of the first element in the sequence that fullfills `pred`. Returns a default-initialized T if the sequence is empty or no such element is found.
`T last()` | Returns the value of the last element in the sequence. Throws std::out_of_range if the sequence is empty.
`T last(pred)` | Returns the value of the last element in the sequence that fullfills `pred`. Throws std::out_of_range if the sequence is empty or no such element is found.
`T lastOrDefault()` | Returns the value of the last element in the sequence. Returns a default-initialized T if the sequence is empty.
`T lastOrDefault(pred)` | Returns the value of the last element in the sequence that fullfills `pred`. Returns a default-initialized T if the sequence is empty or no such element is found.
`T max()` | Returns the largest value in the sequence. Throws std::runtime_error if the sequence is empty. Uses the `>` operator to determine largerness.
`T max(transform)` | Returns the value in the sequence that is largest after applying `transform` on the element. Throws std::runtime_error if the sequence is empty.  Uses the `>` operator to determine largerness.
`T min()` | Returns the smallest value in the sequence. Throws std::runtime_error if the sequence is empty.  Uses the `<` operator to determine largerness.
`T min(transform)` | Returns the value in the sequence that is smallest after applying `transform` on the element. Throws std::runtime_error if the sequence is empty.  Uses the `<` operator to determine largerness.
`bool sequenceEqual(source)` | Returns true if the elemens in `this` and `source` are equal. Elements are not reordered, but compared in the order they arrive in the source sequences. If the sequences are of different length, false is returned.
`Container toContainer<Container>()` | Creates a `Container` and populates it with the elements from the sequence.
`Container<T, Alloc> toContainer<Container, Alloc>()` | Creates a `Container<T,Alloc>` and populates it with the elements from the sequence. If not specified, Alloc defaults to `std::allocator<T>`
`Container<T, Alloc> toContainer<Container, Alloc>()` | Creates a `Container<T,Alloc>` and populates it with the elements from the sequence. If not specified, Alloc defaults to `std::allocator<T>`
`Container<T, Compare, Alloc> toContainer<Container, Compare, Alloc>()` | Creates a `Container<T,Compare, Alloc>` and populates it with the elements from the sequence. If not specified, `Compare` defaults to `std::less<T>` and `Alloc` defaults to `std::allocator<T>`
`reduce(Reducer(value, accumulator), Seed)` | Reduces the sequence. Throws `std::out_of_range` on empty sequences. `Seed` is used as value for `accumulator` on first iteration. [Source](include/enumerable/enumerableTemplate.h#L142-L158), [Examples/Tests](test/test_reduce.cpp)
`reduce(Reducer(value, accumulator=Seed))` | Functors(lambdas etc) only! Reduces the sequence. Throws `std::out_of_range` on empty sequences. `Seed` is used as value for `accumulator` on first iteration.  [Source](include/enumerable/enumerableTemplate.h#L102-L122), [Examples/Tests](test/test_reduce.cpp)
`reduce(Reducer(value, accumulator))` | Reduces the sequence. Throws `std::out_of_range` on empty sequences. The first element is used as value for `accumulator` on first iteration. [Source](include/enumerable/enumerableTemplate.h#L124-L140), [Examples/Tests](test/test_reduce.cpp)
--- | ---
`forEach(sink)` | Calls `sink` once for every element in the sequence.

## More details

Any sequence object really only consists of two things:

* The source sequence
* The operations to apply (filtering, transforming)

In the case where the source is an array or a std-container, the _source_ sequence is a reference and there are no operations (though there are iterators to keep track of state). If a sequence is created using `select` on a such sequence, it would look like

* Select source sequence
   * container reference
   * container iterators
* _Transform_ -function to apply 

If that object would be filtered with `where` then we'd get an object with the following contents

* Where source sequence
   * Select source sequence
      * container reference
      * container iterators
   * _Transform_ -function to apply 
* _Filter_ -function to apply

This structure is known at compile time. The objects can live on the stack, no matter how complex our chaining is. They really only contain the operations that they represent. It is not until they are iterated that memory and cpu is used.

This has a drawback(for which there is a good solution!). It makes it really hard to design functions which accept sequence objects, as just a few chained operations produce tricky templated types. It also means that it is hard to create object members which represent sequences. Thus it becomes hard to store a sequence of operations.

The way these problems are solved is two-fold. First, since every enumerable/sequence object inherits and implements `EnumerableBase` they also inherit and implement `IEnumerable<T>`, where `T` is the type of the elements in the sequence.

```c++
void mySinkFunction(IEnumerable<int>& intSequence)
{
   ...
}
```

One can then either wrap the sequence in `Enumerable(intSequence)` to obtain a new object which can be ranged-for -looped over (or sinked, or anything). Or one can use the primitives `T value()`and `bool moveNext()` to manually iterate the sequence.

_But hey, that doesn't help me with storing produced sequences as members or globals!_

There is a solution for that as well. The function `makeHeapEnumerable` returns an object of the type `HeapEnumerable<T>`. This type can be declared as a parameter type or member. The function `makeHeapEnumerable` allocates memory on the heap, puts a copy of itself(the sequence object) into that memory, and then constructs a `HeapEnumerable<T>`object which is now responsible for that memory. After that it is safe to pass the object around. It effectively becomes a first-order sequence object which contains a referene to an anonymous source.

```c++
// Declare global
HeapEnumerable<int> g_intSequence;

// Later set contents of sequence.
if(something())
{
   g_intSequence = g_networkSource
      .select(createObjects)
	  .filter(validObjects)
	  .select([](auto& obj) { return obj.timestamp; });
}
else
{
   g_intSequence = g_polygons
      .select([](auto& polygon){ return polygon.points.count(); });
}
```
Here we use `something()` to determine if we should produce random int´s from a network source or from polygons.




# Rest is old readme.

It supplies a CRTP-template which implements various functions like .all() or .select.
It expects that what inherits from it implements a moveNext() and value() function.
Various functions return values, like .all(predicate) which reduces a sequence to a boolean if the predicate evaluates to true for all elements in the sequence.
Other operations return a new sequence based on the first sequence, like .select(transform) where transform is [](auto x){ return x*2;} will return a sequence where elements are twice as large as the elements in the source sequence.
Sequences that are returned do in turn inherit from the interface, allowing multiple manipulating operations to be chained seamlessly like .where(_filter_).select(_transform_).
Computation/processing is done in a deferred manner; only when the final sequence is traversed will the operations be applied.
To force processing and allocation at known time, one can use .toContainer<T>() or .pushBack<T>(T& container)

Arrays and standard containers can be wrapped in in the interface with the Enumerable(sequence)-function which has various overloads.
This constructs an object that implements the interface. The various interface-functions which result in a new sequence create copies of the source sequence.
This means that a .select(_transform_) can be copied to multiple variables, each of which will be a sequence that can be iterated once.
They thus have their own complete state of all chained operations.
But the first sequence object will contain a reference to its source container/sequence. Thus any sequence might end up advancing the source.
If the source sequence wraps a container like a std::vector or a normal array then the resulting sequences can be iterated once each.
If the source sequence consists of bytes read from a socket, then any advancement in any sequence naturally advances the source.

The interface inherits an IEnumerable<T> interface wich moveNext() and value(). Thus a library function can accept an IEnumerable<T> without knowing the operations that lead to the sequence.
And IEnumerable can be wrapped with Enumerable() and result in a sequence with the transformation/aggregation functions and be further chained.
But at this point the new sequences will be sourced with a reference, and can only be advanced once.

