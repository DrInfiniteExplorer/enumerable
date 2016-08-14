#pragma once

template <typename T, typename SetType, typename Source, typename OtherSource>
struct ExceptEnumerable : EnumerableBase<T, ExceptEnumerable<T, SetType, Source, OtherSource>>
{
	ExceptEnumerable(Source &source, OtherSource&& otherSource)
		: m_source(source)
	{
		OtherSource other = otherSource;
		while (other.moveNext())
		{
			m_set.insert(other.value());
		}
	}

	virtual T value() override
	{
		return m_source.value();
	}

	virtual bool moveNext() override
	{
		while (m_source.moveNext())
		{
			if (m_set.find(m_source.value()) == m_set.end())
			{
				return true;
			}
		}
		return false;
	}

private:
	Source m_source;
	SetType m_set;
};

