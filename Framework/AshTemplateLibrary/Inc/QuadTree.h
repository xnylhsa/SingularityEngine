#ifndef ASHLYN_QUAD_TREE 
#define ASHLYN_QUAD_TREE
#include "Common.h"


namespace SingularityEngine
{
	namespace AshTL
	{
		template<typename T>
		struct QuadTreeData
		{
			T* mData;
			Math::Rectangle mBounds;
			bool mFlag;
			T* operator->() { return mData; }
			inline QuadTreeData(T* pData, const Math::Rectangle& bounds) : mBounds(bounds), mData(pData), mFlag(false) {}
		};

		template<typename T>
		class QuadTreeNode
		{
		public:
			inline QuadTreeNode(const Math::Rectangle& bounds) : mNodeBounds(bounds), mCurrentDepth(0) {}
			bool IsLeaf();
			size_t NumObjects();
			void Insert(QuadTreeData<T>& data);
			void Remove(QuadTreeData<T>& data);
			void Update(QuadTreeData<T>& data);
			void Shake();
			void Split();
			void Reset();
			std::vector<QuadTreeData<T>*> Query(const Math::Rectangle& area);

		protected:
			std::vector<QuadTreeNode<T>> mChildren;
			std::vector<QuadTreeData<T>*> mContents;
			size_t mCurrentDepth;
			static size_t maxDepth; // replace these with a definition elsewhere
			static size_t maxObjectsPerNode; //
			Math::Rectangle mNodeBounds;

		};

		template<typename T>
		size_t QuadTreeNode<T>::maxObjectsPerNode = 15;

		template<typename T>
		size_t QuadTreeNode<T>::maxDepth = 5;

		template<typename T>
		class QuadTree
		{
		public:
			inline QuadTree() : mRootNode(mQuadTreeBounds) {}
			void Initialize(const Math::Rectangle& bounds);
			size_t NumObjects();
			void Insert(QuadTreeData<T>& data);
			void Remove(QuadTreeData<T>& data);
			void Update(QuadTreeData<T>& data);
			void ShakeTree();
			std::vector<QuadTreeData<T>*> Query(const Math::Rectangle& area);

		protected:
			QuadTreeNode<T> mRootNode;
			Math::Rectangle mQuadTreeBounds;

		};










		template<typename T>
		bool QuadTreeNode<T>::IsLeaf()
		{
			return mChildren.size() == 0;
		}

		template<typename T>
		std::vector<QuadTreeData<T>*> QuadTreeNode<T>::Query(const Math::Rectangle& area)
		{
			std::vector <QuadTreeData<T>*> result;
			if (!IsColliding(mNodeBounds, area))
			{
				return result;
			}
			if (IsLeaf())
			{
				for (size_t i = 0; i < mContents.size(); i++)
				{
					if (IsColliding(mContents[i]->mBounds, area))
					{
						result.push_back(mContents[i]);
					}
				}
			}
			else
			{
				for (size_t i = 0, size = mChildren.size(); i < size; ++i)
				{
					std::vector<QuadTreeData*> childResult = mChildren[i].Query(area);
					if (childResult.size() > 0)
					{
						result.insert(result.end(), childResult.begin(), childResult.end());
					}
				}
			}
			return result;
		}

		template<typename T>
		void QuadTreeNode<T>::Reset()
		{
			if (IsLeaf())
			{
				for (size_t i = 0; i < mContents.size(); i++)
				{
					mContents[i]->mFlag = false;
				}
			}
			else
			{
				for (size_t i = 0; i < mContents.size(); i++)
				{
					mChildren[i].Reset();
				}
			}
		}

		template<typename T>
		void QuadTreeNode<T>::Split()
		{
			if (mCurrentDepth + 1 >= maxDepth)
			{
				return;
			}
			Math::Vector2 min = GetMin(mNodeBounds);
			Math::Vector2 max = GetMin(mNodeBounds);
			Math::Vector2 center = min + ((max - min)*0.5f);

			Math::Rectangle childAreas[4] =
			{
				Math::Rectangle::FromMinMax(min, center),
				Math::Rectangle::FromMinMax(Math::Vector2(center.x, min.y), Math::Vector2(max.x, center.y)) ,
				Math::Rectangle::FromMinMax(center, max),
				Math::Rectangle::FromMinMax(Math::Vector2(min.x, center.y), Math::Vector2(center.x, max.y))
			};
		}

		template<typename T>
		void QuadTreeNode<T>::Shake()
		{
			size_t numObjects = NumObjects();

			if (!IsLeaf())
			{
				if (numObjects == 0)
				{
					mChildren.clear();
				}
			}
			else if (numObjects < maxObjectsPerNode)
			{
				std::queue<QuadTreeNode<T> *> processingQueue;
				processingQueue.push(this);
				while (processingQueue.size() > 0)
				{
					QuadTreeNode<T>* nodeToProcess = processingQueue.back();
					if (!nodeToProcess->IsLeaf())
					{
						for (size_t i = 0; i < mChildren.size(); i++)
						{
							processingQueue.push(&nodeToProcess->mChildren[i]);
						}
					}
					else
					{
						mContents.insert(mContents.end(),
							nodeToProcess->mContents.begin(),
							nodeToProcess->mContents.end());
					}
					processingQueue.pop();
				}
				mChildren.clear();
			}

		}

		template<typename T>
		void QuadTreeNode<T>::Update(QuadTreeData<T>& data)
		{
			Remove(data);
			Insert(data);
		}

		template<typename T>
		void QuadTreeNode<T>::Remove(QuadTreeData<T>& data)
		{
			if (IsLeaf())
			{
				size_t removalIndex = 0xffffffff;
				for (size_t i = 0; i < mContents.size(); i++)
				{
					if (mContents[i]->mData == data.mData)
					{
						removalIndex = i;
						break;
					}
				}
				if (removalIndex != 0xffffffff)
				{
					mContents.erase(mContents.begin() + removalIndex);
				}
				else
				{
					for (size_t i = 0; i < mChildren.size(); i++)
					{
						mChildren[i].Remove(data);
					}
				}
				Shake();
			}
		}

		template<typename T>
		void QuadTreeNode<T>::Insert(QuadTreeData<T>& data)
		{
			if (!Math::IsColliding(data.mBounds, mNodeBounds))
			{
				return;
			}
			if (IsLeaf() && mContents.size() + 1 > maxObjectsPerNode)
			{
				Split();
			}
			if (IsLeaf())
			{
				mContents.push_back(&data);
			}
			else
			{
				for (size_t i = 0; i < mChildren.size(); ++i)
				{
					mChildren[i].Insert(data);
				}
			}
		}

		template<typename T>
		size_t QuadTreeNode<T>::NumObjects()
		{
			Reset();
			size_t objectCount = mContents.size();
			for (size_t i = 0; i < objectCount; ++i)
			{
				mContents[i]->mFlag = true;
			}
			std::queue<QuadTreeNode<T>*> processingQueue;
			processingQueue.push(this);
			while (processingQueue.size() > 0)
			{
				QuadTreeNode* nodeToProcess = processingQueue.back();
				if (!nodeToProcess->IsLeaf())
				{
					for (size_t i = 0, numChildren = nodeToProcess->mChildren.size(); i < numChildren; i++)
					{
						processingQueue.push(&nodeToProcess->mChildren[i]);
					}
				}
				else
				{
					for (size_t i = 0, numContents = nodeToProcess->mContents.size(); i < numContents; i++)
					{
						if (nodeToProcess->mContents[i]->mFlag)
						{
							objectCount += 1;
							nodeToProcess->mContents[i]->mFlag = true;
						}
					}
					processingQueue.pop();
				}
			}
			Reset();
			return objectCount;
		}



		template<typename T>
		size_t QuadTree<T>::NumObjects()
		{
			return mRootNode.NumObjects();
		}



		template<typename T>
		void QuadTree<T>::Remove(QuadTreeData<T>& data)
		{
			return mRootNode.Remove(data);
		}

		template<typename T>
		void QuadTree<T>::Update(QuadTreeData<T>& data)
		{
			return mRootNode.Update(data);
		}


		template<typename T>
		std::vector<QuadTreeData<T>*> QuadTree<T>::Query(const Math::Rectangle& area)
		{
			return mRootNode.Query(area);
		}

		template<typename T>
		void QuadTree<T>::Initialize(const Math::Rectangle& bounds)
		{
			mQuadTreeBounds = bounds;
			mRootNode(mQuadTreeBounds);
		}

		template<typename T>
		void  QuadTree<T>::ShakeTree()
		{
			mRootNode.Shake();
		}

		template<typename T>
		void QuadTree<T>::Insert(QuadTreeData<T>& data)
		{
			return mRootNode.Insert(data);
		}

	}
}

#endif
