#pragma once

template <typename InType, typename OutType>
struct ReinterpretCastEnumerable : InputRange<OutType>
{
	ReinterpretCastEnumerable(InputRange<InType> &source)
		: m_source(source)
	{}

	virtual void restart() override
	{
		m_source.restart();
	}

	virtual bool empty() override
	{
		return m_source.empty();
	}

	virtual OutType front() override
	{
		return reinterpret_cast<OutType>(m_source.front());
	}

	virtual void popFront() override
	{
		m_source.popFront();
	}

private:
	InputRange<InType> &m_source;
};

