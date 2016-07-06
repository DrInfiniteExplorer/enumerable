#pragma once

template <typename T, typename Source, typename OtherSource>
struct ConcatEnumerable : InputRange<T, ConcatEnumerable<T, Source, OtherSource>>
{
	ConcatEnumerable(Source &source1, OtherSource &source2)
		: m_source1(source1)
		, m_source2(source2)
		, m_firstDone(false)
	{}

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
	Source m_source1;
	OtherSource m_source2;
	bool m_firstDone;
};
