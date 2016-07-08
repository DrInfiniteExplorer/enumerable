#pragma once

#include <map>

template <typename S1, typename S2, typename K1F, typename K2F, typename SF, typename Derived>
struct JoinEnumerableHelper;

template <typename S1, typename S2, typename K1F, typename K2F, typename SF>
struct JoinEnumerable : JoinEnumerableHelper<S1, S2, K1F, K2F, SF,
	JoinEnumerable<S1, S2, K1F, K2F, SF>>::type
{
	JoinEnumerable(S1 &source, S2&& otherSource,
	K1F&& key1, K2F&& key2, SF&& select)
	: JoinEnumerableHelper < S1, S2, K1F, K2F, SF,
	JoinEnumerable < S1, S2, K1F, K2F, SF >> ::type(source
		, std::forward<S2>(otherSource)
		, std::forward<K1F>(key1)
		, std::forward<K2F>(key2)
		, std::forward<SF>(select))
	{}
};

template <typename S1, typename S2, typename K1F, typename K2F, typename SF, typename Derived>
struct JoinEnumerableHelper
{
	typedef typename ValueType<S1>::type T1;
	typedef typename ValueType<S2>::type T2;

	typedef typename std::result_of< K1F(T1)>::type K1;
	typedef typename std::result_of< K2F(T2)>::type K2;
	typedef typename std::result_of< SF(T1, T2)>::type SFT;

	struct type : InputRange<SFT, Derived>
	{
		type(S1 &source, S2&& otherSource,
			K1F&& key1, K2F&& key2, SF&& select)
			: m_source(source)
			, m_otherSource(otherSource)
			, m_key1(key1)
			, m_key2(key2)
			, m_select(select)
		{
			while (m_otherSource.moveNext())
			{
				const auto& t = m_otherSource.value();
				m_multimap.insert(std::make_pair(m_key2(t), t));
			}
		}

		virtual SFT value() override
		{
			return m_select(m_source.value(), m_setIterator->second);
		}

		virtual bool moveNext() override
		{
			if (m_setIterator != m_setEndIterator)
			{
				++m_setIterator;
				return true;
			}
			while (m_source.moveNext())
			{
				auto pair = m_multimap.equal_range(m_key1(m_source.value()));
				m_setIterator = pair.first;
				m_setEndIterator = pair.second;
				if (m_setIterator != m_setEndIterator)
				{
					return true;
				}
			}
			return false;
		}

	private:
		S1 m_source;
		S2 m_otherSource;
		K1F m_key1;
		K2F m_key2;
		SF m_select;
		typedef std::multimap<K2, T2> MultiMap;
		MultiMap m_multimap;
		typename MultiMap::iterator m_setIterator;
		typename MultiMap::iterator m_setEndIterator;
	};

};
