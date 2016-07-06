#pragma once

template <typename T>
struct ConcatEnumerable : InputRange<T>
{
	ConcatEnumerable(InputRange<T> &source1, InputRange<T> &source2)
		: m_source1(source1)
		, m_source2(source2)
		, m_firstDone(false)
	{}

	virtual void restart() override
	{
		m_source1.restart();
		if (m_firstDone)
		{
			m_source2.restart();
		}
	}

	virtual bool empty() override
	{
		return m_source1.empty() && m_firstDone && m_source2.empty();
	}

	virtual T front() override
	{
		if (m_firstDone)
		{
			return m_source2.front();
		}
		return m_source1.front();
	}

	virtual void popFront() override
	{
		if (m_firstDone)
		{
			m_source2.popFront();
			return;
		}
		m_source1.popFront();
		if (m_source1.empty())
		{
			m_firstDone = true;
		}
	}

private:
	InputRange<T> &m_source1;
	InputRange<T> &m_source2;
	bool m_firstDone;
};
