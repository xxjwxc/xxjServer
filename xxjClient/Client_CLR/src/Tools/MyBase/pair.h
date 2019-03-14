#ifndef H_QWER_PAIR_H
#define	H_QWER_PAIR_H

using System::ArgumentOutOfRangeException;

generic<typename T, typename U>
	QDLL_EXPORT QSTRUCT pair
	{
	public:
#ifndef _USING_CLI
		pair()
		{ }
#endif

		pair(T t, U u) :
			first(t), second(u)
		{ }

	public:
		T first;
		U second;
	};

	QDLL_EXPORT QCLASS pair_helper
	{
	public:
		generic<typename T, typename U>
		static pair<T, U> make_pair(T t, U u)
		{
			return pair<T, U>(t, u);
		}
	};

#endif	/* COCTSTREAM_H */

