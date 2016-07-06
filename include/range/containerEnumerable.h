#pragma once

template <typename T, typename Container>
struct ContainerEnumerable : public InputRange<T>
{
	ContainerEnumerable(Container&& container)
		: m_container(container)
		, m_iterator(container.begin())
	{}

	virtual void restart() override
	{
		m_iterator = m_container.begin();
	}

	virtual bool empty() override
	{
		return m_iterator == m_container.end();
	}

	virtual T front()
	{
		return *m_iterator;
	}

	virtual void popFront()
	{
		++m_iterator;
	}

private:
	Container& m_container;
	typename Container::iterator m_iterator;
};

template <typename T, template <typename, typename> class Container, typename ContainerAllocator>
ContainerEnumerable<T, Container<T, ContainerAllocator>> Enumerable(Container<T, ContainerAllocator>& container)
{
	return ContainerEnumerable<T, Container<T, ContainerAllocator>>(std::forward<Container<T, ContainerAllocator>>(container));
}
