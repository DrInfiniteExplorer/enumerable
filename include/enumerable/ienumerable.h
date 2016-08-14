#pragma once

// This simple interface serves as a base class for the crtp template. Anything derived from that template
//  can be cast to this, making it possible pass along references or pointers of complex sequence objects
//  to consumer functions and the like.
// Note that EnumerableBase-objects constructed from this interface hold a reference to the original object,
//  thus making the sequence usable only one. If you wish to store copies of the referenced sequence,
//  your consumer should instead accept a HeapEnumerable<T> which works much like a generic functor object.
template <typename T>
struct IEnumerable
{
	virtual bool moveNext() = 0;
	virtual T value() = 0;
};

