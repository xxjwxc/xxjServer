
#ifndef H_QWER_ORDERED_MAP_H
#define	H_QWER_ORDERED_MAP_H

#include "pair.h"


using System::Collections::Generic::Comparer;
using System::ArgumentNullException;
using System::ArgumentOutOfRangeException;
using System::Random;

#define SKIPLIST_MAX_LEVEL 16
#define BITS_IN_RANDOM 31

#define NODE_HEAD_FLAG 1
#define NODE_TAIL_FLAG -1
#define NODE_COMMON_FLAG 0


generic<typename TKey, typename TValue>
	ref class ordered_map
	{
	public:
		QCLASS _Node
		{
		public:
			_Node(pair<TKey, TValue> poVal, INT32 iLv)
			{
				val = poVal;
				level = iLv;
				next = NULL_PTR;
				down = NULL_PTR;
				prev = NULL_PTR;
				up = NULL_PTR;
			}
			INT32 level;
			pair<TKey, TValue> val;
			INT32 flag;
			PTR(_Node) next;
			PTR(_Node) down;
			PTR(_Node) prev;
			PTR(_Node) up;
		};
	public:
		QSTRUCT iterator
		{
		public:
			iterator(ordered_map<TKey, TValue>^ skiplist, PTR(_Node) node) :
				m_poSkiplist(skiplist), m_poNode(node)
			{
			}
			property TKey first
			{
				TKey get()
				{
					return m_poNode->val.first;
				}
			}

			property TValue second
			{
				TValue get()
				{
					return m_poNode->val.second;
				}

				void set(TValue v)
				{
					m_poNode->val = MakePair(m_poNode->val.first, v);
					PTR(_Node) poNode = m_poNode->up;
					while (poNode != NULL_PTR)
					{
						m_poNode->val = MakePair(m_poNode->val.first, v);
						poNode = poNode->up;
					}
					poNode = m_poNode->down;
					while (poNode != NULL_PTR)
					{
						m_poNode->val = MakePair(m_poNode->val.first, v);
						poNode = poNode->down;
					}
				}
			}
			static bool operator == (iterator arg1, iterator arg2)
			{
				return arg1.m_poSkiplist == arg2.m_poSkiplist && ordered_map::_IsSameNode(arg1.m_poNode, arg2.m_poNode);
			}
			static bool operator != (iterator arg1, iterator arg2)
			{
				return !(arg1 == arg2);
			}
			static iterator operator ++(iterator arg)
			{
				return iterator(arg.m_poSkiplist, ordered_map::_BottomNext(arg.m_poNode));
			}
			static iterator operator --(iterator arg)
			{
				return iterator(arg.m_poSkiplist, ordered_map::_BottomPrev(arg.m_poNode));
			}
			iterator next()
			{
				return m_poNode->next == NULL_PTR ? m_poSkiplist->end() : iterator(m_poSkiplist, m_poNode->next);
			}
			iterator down()
			{
				return m_poNode->down == NULL_PTR ? m_poSkiplist->end() : iterator(m_poSkiplist, m_poNode->down);
			}
		internal:
			PTR(_Node) Node()
			{
				return m_poNode;
			}
		private:
			initonly ordered_map<TKey, TValue>^ m_poSkiplist;
			initonly PTR(_Node) m_poNode;
		};
	public:
		ordered_map()
		{
			_construct(NULL_PTR);
		}

		ordered_map(Comparer<TKey>^ compareFunc)
		{
			_construct(compareFunc);
		};

		iterator begin()
		{
			PTR(_Node) n = _BottomNext(m_poHeadNode);
			return iterator(this, n);
		}

		iterator end()
		{
			PTR(_Node) n = _BottomNode(m_poTailNode);
			return iterator(this, n);
		}

		iterator head()
		{
			PTR(_Node) n = m_poHeadNode;
			return iterator(this, n);
		}

		iterator find(TKey key)
		{
			iterator it = lower_bound(key);
			if (it != end() && comp->Compare(it.first, key) == 0)
			{
				return it;
			}
			return end();
			/*PTR(_Node) p = m_poHeadNode->next;
			INT32 iHeight = m_iHeight;
			INT32 iResult = 0;
			INT32 iLastResult = -1;
			do
			{
				while (p->flag != NODE_HEAD_FLAG && p->flag != NODE_TAIL_FLAG)
				{
					iResult = comp->Compare(p->val.first, key);
					if (iResult == 0)
					{
						return iterator(this, _BottomNode(p));
					}
					iResult = iResult < 0 ? -1 : 1;
					if (iLastResult != iResult)
					{
						iLastResult = iResult;
						break;
					}
					p = iResult < 0 ? p->next : p->prev;
				}
				if (p->flag == NODE_HEAD_FLAG)
				{
					p = p->next;
				}
				else if(p->flag == NODE_TAIL_FLAG)
				{
					p = p->prev;
				}
				p = p->down;
			} while (--iHeight >= 0 && p != NULL_PTR);
			return end();*/
		}

		iterator lower_bound(TKey key)  //Return the first iterator not less than KEY
		{
			if (m_iSize == 0)
			{
				return end();
			}
			PTR(_Node) p = m_poHeadNode->next;
			INT32 iHeight = m_iHeight;
			INT32 iResult = 0;
			INT32 iLastResult = -1;
			do
			{
				while (p->flag != NODE_HEAD_FLAG && p->flag != NODE_TAIL_FLAG)
				{
					iResult = comp->Compare(p->val.first, key);
					if (iResult == 0)
					{
						return iterator(this, _BottomNode(p));
					}
					iResult = iResult < 0 ? -1 : 1;
					if (iLastResult != iResult)
					{
						iLastResult = iResult;
						break;
					}
					//if (p->prev->flag == NODE_HEAD_FLAG)
					//{
					//	break;
					//}
					p = iResult < 0 ? p->next : p->prev;
				}
				if (p->level == 0)
				{
					if (p->flag == NODE_HEAD_FLAG)
						return iterator(this, _BottomNode(p)->next);
					if (p->flag == NODE_TAIL_FLAG)
						return end();
					return iResult < 0 ? iterator(this, _BottomNode(p)->next) : iterator(this, _BottomNode(p));
				}
				if (p->flag == NODE_HEAD_FLAG)
				{
					p = p->next;
				}
				else if (p->flag == NODE_TAIL_FLAG)
				{
					p = p->prev;
				}
				p = p->down;
			} while (--iHeight >= 0 && p != NULL_PTR);
			return end();
		}

		iterator upper_bound(TKey key)  //Return the first iterator big than KEY
		{
			if (m_iSize == 0)
			{
				return end();
			}
			PTR(_Node) p = m_poHeadNode->next;
			INT32 iHeight = m_iHeight;
			INT32 iResult = 0;
			INT32 iLastResult = -1;
			do
			{
				while (p->flag != NODE_HEAD_FLAG && p->flag != NODE_TAIL_FLAG)
				{
					iResult = comp->Compare(p->val.first, key);
					if (iResult == 0)
					{
						return iterator(this, _BottomNode(p)->next);
					}
					iResult = iResult < 0 ? -1 : 1;
					if (iLastResult != iResult)
					{
						iLastResult = iResult;
						break;
					}
					//if (p->prev->flag == NODE_HEAD_FLAG)
					//{
					//	break;
					//}
					p = iResult < 0 ? p->next : p->prev;
				}
				if (p->level == 0)
				{
					if (p->flag == NODE_HEAD_FLAG)
						return iterator(this, _BottomNode(p)->next);
					if (p->flag == NODE_TAIL_FLAG)
						return end();
					return iResult <= 0 ? iterator(this, _BottomNode(p)->next) : iterator(this, _BottomNode(p));
				}
				if (p->flag == NODE_HEAD_FLAG)
				{
					p = p->next;
				}
				else if (p->flag == NODE_TAIL_FLAG)
				{
					p = p->prev;
				}
				p = p->down;
			} while (--iHeight >= 0 && p != NULL_PTR);
			return end();
		}

		inline pair<iterator, bool> insert(pair<TKey, TValue> val)
		{
			INT32 iLevel = _RandomLevel();
			return _InsertNode(val, iLevel, false);
		}

		property TValue default[TKey]
		{
			TValue get(TKey k)
			{
				iterator it = find(k);
				if (it == end())
				{
					pair<iterator, bool> ret = insert(pair<TKey, TValue>(k, TValue()));
					return ret.first.second;
				}
				else
				{
					return it.second;
				}
			}
			void set(TKey k, TValue v)
			{
				iterator it = find(k);
				if (it == end())
				{
					INT32 iLevel = _RandomLevel();
					_InsertNode(pair<TKey, TValue>(k, v), iLevel, true);
				}
				else
				{
					it.second = v;
				}
			}
		}

		void erase(TKey key)
		{
			iterator it = lower_bound(key);
			PTR(_Node) q = it.Node();
			if (it != end())
			{
				int iResult = comp->Compare(q->val.first, key);
				if (iResult == 0)
				{
					while (q != NULL_PTR)
					{
						if (q->level == m_iHeight && q->prev->flag == NODE_HEAD_FLAG
							&& q->next->flag == NODE_TAIL_FLAG)
						{
							if (m_iHeight > 0)
							{
								--m_iHeight;
								m_poHeadNode = m_poHeadNode->down;
								m_poTailNode = m_poTailNode->down;
								m_iTotal -= 3;
							}
							else
							{
								m_poHeadNode->next = m_poTailNode;
								m_poTailNode->prev = m_poHeadNode;
								m_iTotal -= 1;
								--m_iSize;
								return;
							}
						}
						else
						{
							q->prev->next = q->next;
							q->next->prev = q->prev;
							m_iTotal--;
						}
						q = q->up;
					}
					--m_iSize;
				}
			}

			/*PTR(_Node) p = m_poHeadNode->next;
			PTR(_Node) q = NULL_PTR;
			if (p == NULL_PTR)
				return;
			INT32 iHeight = m_iHeight;
			do
			{
				INT32 iResult = 0;
				INT32 iLastResult = -2;
				while (p->flag != NODE_HEAD_FLAG && p->flag != NODE_TAIL_FLAG)
				{
					iResult = comp->Compare(p->val.first, key);
					if (iResult == 0)
					{
						do
						{
							if (p->level == m_iHeight && p->prev->flag == NODE_HEAD_FLAG && p->next->flag == NODE_TAIL_FLAG)
							{
								if (m_iHeight > 0)
									-- m_iHeight;
								m_poHeadNode = m_poHeadNode->down;
								m_poTailNode = m_poTailNode->down;

								m_iTotal -= 3;
							}
							else
							{
								p->prev->next = p->next;
								p->next->prev = p->prev;
								--m_iTotal;
							}
							p = p->down;
						} while (p != NULL_PTR);
						--m_iSize;
						return;
					}
					p = iResult < 0 ? p->next : p->prev;
					iResult = iResult < 0 ? -1 : 1;
					if (iLastResult == -2)
					{
						iLastResult = iResult;
					}
					else if (iLastResult != iResult)
					{
						iLastResult = iResult;
						break;
					}
				}
				if (p->flag == NODE_HEAD_FLAG)
				{
					p = p->next;
				}
				else if (p->flag == NODE_TAIL_FLAG)
				{
					p = p->prev;
				}
				p = p->down;

			}while (--iHeight >= 0 && p != NULL_PTR);*/
		}

		void erase(iterator it)
		{
			if (it == end())
				return;
			PTR(_Node) p = _TopNode(it.Node());
			do
			{
				if (p->level == m_iHeight && p->prev->flag == NODE_HEAD_FLAG && p->next->flag == NODE_TAIL_FLAG)
				{
					if (m_iHeight > 0)
					{
						--m_iHeight;
						m_poHeadNode = m_poHeadNode->down;
						m_poTailNode = m_poTailNode->down;
						m_iTotal -= 3;
					}
					else
					{
						m_poHeadNode->next = m_poTailNode;
						m_poTailNode->prev = m_poHeadNode;
						m_iTotal -= 1;
						--m_iSize;
						return;
					}
				}
				else
				{
					p->prev->next = p->next;
					p->next->prev = p->prev;
					--m_iTotal;
				}
				p = p->down;
			} while (p != NULL_PTR);
			--m_iSize;
		}

		void clear()
		{
			m_poHeadNode->down = NULL_PTR;
			m_poHeadNode->next = m_poTailNode;
			m_poTailNode->down = NULL_PTR;
			m_poTailNode->prev = m_poHeadNode;
			m_iSize = 0;
			m_iHeight = 0;
			m_iTotal = 0;
		}


		INT32 size() { return m_iSize; }
		INT32 total() { return m_iTotal; }
		INT32 height() { return m_iHeight; }
	private:
		void _construct(Comparer<TKey>^ compareFunc)
		{
			m_iHeight = 0;
			m_iTotal = 0;
			m_iSize = 0;
			m_gLevel = 0;

			comp = compareFunc == NULL_PTR ? Comparer<TKey>::Default : compareFunc;

			m_poHeadNode = QNEW _Node(pair<TKey, TValue>(m_poHeadKey, m_poHeadValue), 0);
			m_poHeadNode->flag = NODE_HEAD_FLAG;
			m_poTailNode = QNEW _Node(pair<TKey, TValue>(m_poHeadKey, m_poTailValue), 0);
			m_poTailNode->flag = NODE_TAIL_FLAG;
			m_poHeadNode->next = m_poTailNode;
			m_poTailNode->prev = m_poHeadNode;

		}

		INT32 _RandomLevel()
		{
			INT32 iHeight = m_iHeight + 1;
			INT32 iRemain = (m_iSize + 1) % (1 << iHeight);
			while (iRemain > 0)
			{
				iRemain = (m_iSize + 1) % (1 << (--iHeight));
			}
			return iHeight;
			/*INT32 b;
			INT32 randomBits =0;//= rand();
			INT32 randomsLeft;
			do {
				b = randomBits&3;
				if (!b) level++;
				randomBits>>=2;
				if (--randomsLeft == 0) {
					randomBits = 0;//rand();
					randomsLeft = BITS_IN_RANDOM>>1;
				};
			} while (!b);*/
		}

		static PTR(_Node) _BottomNode(PTR(_Node) poNode)
		{
			PTR(_Node) node = poNode;
			INT32 level = poNode->level;
			while (--level >= 0 && node->down != NULL_PTR)
			{
				node = node->down;
			}
			return node;
		}

		PTR(_Node) _TopNode(PTR(_Node) poNode)
		{
			PTR(_Node) node = poNode;
			while (node->up != NULL_PTR)
			{
				node = node->up;
			}
			return node;
		}

		static PTR(_Node) _BottomPrev(PTR(_Node) poNode)
		{
			return _BottomNode(poNode)->prev;
		}

		static PTR(_Node) _BottomNext(PTR(_Node) poNode)
		{
			return _BottomNode(poNode)->next;
		}

		static bool _IsSameNode(PTR(_Node) poNode1, PTR(_Node) poNode2)
		{
			//if (poNode1->flag == poNode2->flag && poNode1->flag != NODE_COMMON_FLAG)
			//	return true;
			//return comp->Compare(TO_REF(poNode1->value), TO_REF(poNode2->value)) == 0;
			if (poNode1->flag != poNode2->flag)
				return false;
			return poNode1->flag != NODE_COMMON_FLAG ? true :
				comp->Compare(TO_REF(poNode1->val.first), TO_REF(poNode2->val.first)) == 0;
		}

		pair<iterator, bool> _InsertNode(pair<TKey, TValue> val, INT32 iLevel, bool bForceInsert)
		{
			iterator it = lower_bound(val.first);
			PTR(_Node) p = NULL_PTR;
			PTR(_Node) q = it.Node();
			PTR(_Node) r = NULL_PTR;
			PTR(_Node) pNewHead = NULL_PTR;
			PTR(_Node) pNewTail = NULL_PTR;
			if (it != end())
			{
				int iResult = comp->Compare(q->val.first, val.first);
				if (iResult == 0)
				{
					if (bForceInsert)
					{
						q->val = MakePair(q->val.first, val.second);
						PTR(_Node) poNode = q->up;
						while (poNode != NULL_PTR)
						{
							poNode->val = MakePair(poNode->val.first, val.second);
							poNode = poNode->up;
						}
						poNode = q->down;
						while (poNode != NULL_PTR)
						{
							poNode->val = MakePair(poNode->val.first, val.second);
							poNode = poNode->down;
						}
						return pair<iterator, bool>(iterator(this, q), true);
					}
					else
					{
						return pair<iterator, bool>(iterator(this, q), false);
					}
				}
				INT32 i = 0;
				for (; i <= iLevel&&i <= m_iHeight; ++i)
				{
					p = QNEW _Node(val, i);
					m_iTotal++;
					p->prev = q->prev;
					p->next = q;
					p->down = r;
					q->prev->next = p;
					q->prev = p;
					//q->next->prev = p;
					if (r != NULL_PTR)
					{
						r->up = p;
					}
					r = p;
					while (q->up == NULL_PTR)
					{
						if (q->flag == NODE_TAIL_FLAG)
						{
							break;
						}
						else
						{
							q = q->next;
						}
					}
					q = q->up;
				}
			}
			else
			{
				INT32 i = 0;
				PTR(_Node) poTail = _BottomNode(m_poTailNode);
				for (; i <= iLevel&&i <= m_iHeight; ++i)
				{
					p = QNEW _Node(val, i);
					p->prev = poTail->prev;
					p->next = poTail;
					p->down = r;
					poTail->prev->next = p;
					poTail->prev = p;
					if (r != NULL_PTR)
					{
						r->up = p;
					}
					r = p;
					poTail = poTail->up; //²»»áÎª¿Õ
					m_iTotal++;
				}
			}
			++m_iSize;
			if (iLevel > m_iHeight)
			{
				m_iHeight = m_iHeight + 1;
				pNewHead = QNEW _Node(pair<TKey, TValue>(m_poHeadKey, m_poHeadValue), m_iHeight);
				pNewHead->flag = NODE_HEAD_FLAG;
				pNewTail = QNEW _Node(pair<TKey, TValue>(m_poTailKey, m_poTailValue), m_iHeight);
				pNewTail->flag = NODE_TAIL_FLAG;
				r = QNEW _Node(val, m_iHeight);
				pNewHead->down = m_poHeadNode;
				pNewHead->next = r;
				pNewTail->prev = r;
				pNewTail->down = m_poTailNode;
				r->next = pNewTail;
				r->prev = pNewHead;
				r->down = p;
				p->up = r;
				m_poHeadNode->up = pNewHead;
				m_poTailNode->up = pNewTail;
				m_poHeadNode = pNewHead;
				m_poTailNode = pNewTail;
				m_iTotal += 3;
			}
			return pair<iterator, bool>(iterator(this,_BottomNode(p)), true);
			/*
			INT32 iResult = 0;
			INT32 iLastResult = -2;

			INT32 iHeight = m_iHeight;
			PTR(_Node) p = NULL_PTR;
			PTR(_Node) q = NULL_PTR;
			PTR(_Node) r = NULL_PTR;
			PTR(_Node) poInsertPlace = NULL_PTR;
			if (iLevel > m_iHeight) //Need to add a new layer, triple nodes, one for head, one for tail, one for valued node;
			{
				m_iHeight = iLevel;
				p = QNEW _Node(pair<TKey, TValue>(m_poHeadKey, m_poHeadValue), m_iHeight);
				p->flag = NODE_HEAD_FLAG;
				q = QNEW _Node(pair<TKey, TValue>(m_poTailKey, m_poTailValue), m_iHeight);
				q->flag = NODE_TAIL_FLAG;
				r = QNEW _Node(val, m_iHeight);
				p->down = m_poHeadNode;
				p->next = r;
				r->next = q;
				r->prev = p;
				q->prev = r;
				q->down = m_poTailNode;

				m_poHeadNode->up = p;
				m_poTailNode->up = q;
				m_poHeadNode = p;
				m_poTailNode = q;
				//p = m_poHeadNode->down->next;
				--iLevel;
				m_iTotal += 3;
				poInsertPlace = m_poHeadNode->down->next;
			}
			else
			{
				poInsertPlace = m_poHeadNode->next;
			}
			while(iHeight >= iLevel && poInsertPlace != NULL_PTR)
			{
				if (poInsertPlace->flag == NODE_TAIL_FLAG)
				{
					poInsertPlace = poInsertPlace->prev;
				}
				while (poInsertPlace->flag != NODE_HEAD_FLAG && poInsertPlace->flag != NODE_TAIL_FLAG)
				{
					iResult = comp->Compare(poInsertPlace->val.first, val.first);
					if (iResult == 0)
					{
						if (bForceInsert)
						{
							poInsertPlace->val.second = val.second;
							PTR(_Node) poNode = poInsertPlace->up;
							while (poNode != NULL_PTR)
							{
								poNode->val.second = val.second;
								poNode = poNode->up;
							}
							poNode = poInsertPlace->down;
							while (poNode != NULL_PTR)
							{
								poNode->val.second = val.second;
								poNode = poNode->down;
							}
							return pair<iterator, bool>(iterator(this, poInsertPlace), true);
						}
						else
						{
							return pair<iterator, bool>(iterator(this, poInsertPlace), false);
						}

					}
					iResult = iResult < 0 ? -1 : 1;
					if (iLastResult == -2)
					{
						iLastResult = iResult;
					}
					else if (iLastResult != iResult)
					{
						if (iLastResult > 0)
						{
							poInsertPlace = poInsertPlace->next;
						}
						break;
					}
					poInsertPlace = iResult < 0 ? poInsertPlace->next : poInsertPlace->prev;
					iLastResult = iResult;
				}
				if (iHeight > iLevel)
					poInsertPlace = poInsertPlace->down;
				--iHeight;
				if (poInsertPlace->flag == NODE_HEAD_FLAG)
				{
					poInsertPlace = poInsertPlace->next;
				}
			}
			bool bFirst = true;
			do
			{
				q = QNEW _Node(val, iLevel);
				q->next = poInsertPlace;
				q->prev = poInsertPlace->prev;
				poInsertPlace->prev->next = q;
				poInsertPlace->prev = q;
				if (r != NULL_PTR)
				{
					r->down = q;
					q->up = r;
				}
				r = q;
				poInsertPlace = poInsertPlace->down;
				if (bFirst)
				{
					++m_iSize;
					bFirst = false;
				}
				++m_iTotal;
			} while(--iLevel >= 0 && poInsertPlace != NULL_PTR);*/
		}
	private:
		INT32 m_iTotal;
		INT32 m_iSize;
		INT32 m_iHeight;
		TValue m_poHeadValue; //
		TValue m_poTailValue; //
		TKey m_poHeadKey;
		TKey m_poTailKey;
		PTR(_Node) m_poHeadNode;
		PTR(_Node) m_poTailNode;
		static PTR(Comparer<TKey>) comp;

		INT32 m_gLevel;
	};

#endif