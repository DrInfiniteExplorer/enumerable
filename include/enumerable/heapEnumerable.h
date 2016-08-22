#pragma once

#include <functional>

template <typename T>
struct HeapEnumerable : public EnumerableBase<T, HeapEnumerable<T>>
{
	typedef std::function<HeapEnumerable<T>(const HeapEnumerable<T>&)> CopyFunctionType;

	HeapEnumerable(IEnumerable<T> *source, const CopyFunctionType& copy)
	: m_source(source)
	, m_copier(copy)
	{
	}

	HeapEnumerable()
		: m_source(nullptr)
		, m_copier()
	{
	}

	~HeapEnumerable()
	{
		delete m_source;
		m_source = nullptr;
	}

	HeapEnumerable(const HeapEnumerable<T>& other)
	{
		*this = other.m_copier(other);
	}

	HeapEnumerable(HeapEnumerable<T>&& other)
	{
		m_source = std::exchange(other.m_source, nullptr);
		m_copier = std::exchange(other.m_copier, CopyFunctionType());
	}

	HeapEnumerable<T>& operator=(const HeapEnumerable<T>& other)
	{
		m_source = other.m_source;
		m_copier = other.m_copier;
		return *this;
	}

	HeapEnumerable<T>& operator=(HeapEnumerable<T>&& other)
	{
		m_source = std::exchange(other.m_source, nullptr);
		m_copier = std::exchange(other.m_copier, CopyFunctionType());
		return *this;
	}

	virtual T value() override
	{
		return m_source->value();
	}
	
	virtual bool moveNext() override
	{
		return m_source->moveNext();
	}

//private:
	IEnumerable<T> *m_source;
	CopyFunctionType m_copier;
};

