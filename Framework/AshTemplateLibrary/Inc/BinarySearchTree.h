#ifndef INCLUDED_BINARY_SEARCH_TREE_HEADER_
#define INCLUDED_BINARY_SEARCH_TREE_HEADER_
#include "Common.h"

namespace SingularityEngine
{
	namespace AshAlgo
	{

		enum BinaryPrintOrders
		{
			In_Order = 0,
			Pre_Order = 1,
			Post_Order = 2,
		};
		template<typename T>
		class BinarySearchTreeNode
		{
		public:
			BinarySearchTreeNode(T data) : mData(T), mLeft(nullptr), mRight(nullptr)
			{
				if (mLeft)
				{
					mLeft.reset();
					mLeft = nullptr;
				}
				if (mRight)
				{
					mRight.reset();
					mRight = nullptr;
				}
			}
			~BinarySearchTreeNode() {}
			T GetData() { return mData; }

			
			void SetLeftNode(std::shared_ptr<BinarySearchTreeNode<T>*> leftNode) { mLeft = leftNode; }
			void SetRightNode(std::shared_ptr<BinarySearchTreeNode<T>*> rightNode) { mRight = rightNode; }
			std::shared_ptr<BinarySearchTreeNode<T>*> GetLeftNode() { return mLeft; }
			std::shared_ptr<BinarySearchTreeNode<T>*> GetRightNode() { return mRight; }
		private:
			T mData;
			std::shared_ptr<BinarySearchTreeNode<T>*> mParent;
			std::shared_ptr<BinarySearchTreeNode<T>*> mLeft;
			std::shared_ptr<BinarySearchTreeNode<T>*> mRight;
		};
		template<typename T>
		class BinarySearchTree
		{
			void Insert(T data) 
			{
				if (root == nullptr)
				{
					root = new std::shared_ptr<BinarySearchTreeNode<T>*>(data);
				}
				else
				{
					std::shared_ptr<BinarySearchTreeNode<T>*> currentNode = root;
					while (true)
					{

						if (data < currentNode.GetData() && currentNode.GetLeft() != nullptr)
						{
							currentNode = currentNode.GetLeft();
						}
						else if (data > currentNode.GetData() && currentNode.GetRight() != nullptr)
						{
							currentNode = currentNode.GetRight();
						}
						else
						{
							if (data < currentNode.GetData() && currentNode.GetLeft() == nullptr)
							{
								currentNode = currentNode.GetLeft();
							}
							else if (data > currentNode.GetData() && currentNode.GetRight() == nullptr)
							{
								currentNode = currentNode.GetRight();
							}
							break;
						}
						if (currentNode == nullptr || currentNode.GetRight() == nullptr || currentNode.GetLeft() == nullptr)
						{
							break;
						}
					}
				}
			}
		public:
			std::shared_ptr<BinarySearchTreeNode<T>*> root;
		};
	}
}
#endif // INCLUDED_BINARY_SEARCH_TREE_HEADER_
