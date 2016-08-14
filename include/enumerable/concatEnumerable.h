#pragma once

// A sequence object created with two other sequence objects A {a1, a2, ...} and B {b1, b2, ...}.
// The sequence objects represents the sequence formed by {A, B} = {a1, a2, ..., b1, b2, ...}
template <typename T, typename Source, typename OtherSource>
struct ConcatEnumerable : EnumerableBase<T, ConcatEnumerable<T, Source, OtherSource>>
{
	ConcatEnumerable(Source &source1, OtherSource &source2)
		: m_source1(source1)
		, m_source2(source2)
		, m_firstDone(false)
	{}

	virtual T value() override
	{
		if (m_firstDone)
		{
			return m_source2.value();
		}
		return m_source1.value();
	}

	virtual bool moveNext() override
	{
		if (!m_firstDone)
		{
			m_firstDone = !m_source1.moveNext();
			if (m_firstDone)
			{
				return m_source2.moveNext();
			}
			return true;
		}
		return m_source2.moveNext();
	}

private:
	Source m_source1;
	OtherSource m_source2;
	bool m_firstDone;
};
