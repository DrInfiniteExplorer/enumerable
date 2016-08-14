#pragma once

#include <utility>

// This class wraps a normal container, providing a sequence object implementing EnumerableBase.
// The container is captured by reference, so sequences with this as the base can be
//  copied and reused multiple times without problem as long as the source container is valid.
template <typename T, typename Container>
struct ContainerEnumerable : public EnumerableBase<T, ContainerEnumerable<T, Container>>
{
	ContainerEnumerable(Container& container)
		: m_container(container)
		, m_iterator()
		, m_firstMoved(false)
	{}

	virtual T value() override
	{
		if (!m_firstMoved)
		{
			throw std::runtime_error("ContainerEnumerable not moveNext'ed");
		}
		return *m_iterator;
	}

	virtual bool moveNext() override
	{
		if (m_firstMoved)
		{
			++m_iterator;
		}
		else
		{
			m_iterator = m_container.begin();
			m_firstMoved = true;
		}
		return m_iterator != m_container.end();
	}

private:
	Container& m_container;
	typename Container::iterator m_iterator;
	bool m_firstMoved;
};

// Creates a sequence object given a source container.
// Works with simple containers like std::vector, std::list, etc.
// Basically anything that implements ::iterator, .begin() and .end()
template <typename T, template <typename, typename> class Container, typename ContainerAllocator>
ContainerEnumerable<T, Container<T, ContainerAllocator>> Enumerable(Container<T, ContainerAllocator>& container)
{
	return ContainerEnumerable<T, Container<T, ContainerAllocator>>(container);
}
