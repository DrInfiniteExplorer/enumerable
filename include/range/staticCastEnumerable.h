#pragma once

template <typename InType, typename OutType, typename Source>
struct StaticCastEnumerable : InputRange<OutType, StaticCastEnumerable<InType, OutType, Source>>
{
	StaticCastEnumerable(Source &source)
		: m_source(source)
	{}

	virtual OutType value() override
	{
		return static_cast<OutType>(m_source.value());
	}

	virtual bool moveNext() override
	{
		return m_source.moveNext();
	}

private:
	Source m_source;
};

