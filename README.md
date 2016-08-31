[![Build Status](https://travis-ci.org/DrInfiniteExplorer/enumerable.svg?branch=master)](https://travis-ci.org/DrInfiniteExplorer/enumerable)

# enumerable
Enumerable is a library inspired by [IEnumerable](https://msdn.microsoft.com/en-us/library/9eekhta0(v=vs.110).aspx) in c#

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

