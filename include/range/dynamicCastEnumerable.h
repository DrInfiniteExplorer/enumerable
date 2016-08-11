#pragma once

template <typename InType, typename OutType, typename Source>
struct DynamicCastEnumerable : InputRange<OutType, DynamicCastEnumerable<InType, OutType, Source> >
{
	DynamicCastEnumerable(Source &source)
		: m_source(source)
	{}

	virtual bool moveNext() override
	{
		return m_source.moveNext();
	}

	virtual OutType value() override
	{
		return dynamic_cast<OutType>(m_source.value());
	}

private:
	Source m_source;
};

