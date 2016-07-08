#pragma once

template <typename T, typename Container>
struct ContainerEnumerable : public InputRange<T, ContainerEnumerable<T, Container>>
{
	ContainerEnumerable(Container&& container)
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

template <typename T, template <typename, typename> class Container, typename ContainerAllocator>
ContainerEnumerable<T, Container<T, ContainerAllocator>> Enumerable(Container<T, ContainerAllocator>& container)
{
	return ContainerEnumerable<T, Container<T, ContainerAllocator>>(std::forward<Container<T, ContainerAllocator>>(container));
}
