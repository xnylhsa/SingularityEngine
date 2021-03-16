#ifndef INCLUDED_CORE_BLOCKALLOCATOR_H
#define INCLUDED_CORE_BLOCKALLOCATOR_H
#include "Common.h"
#include <mutex>


namespace SingularityEngine::Core
{

	class BlockAllocator
	{
	public:
		BlockAllocator(uint32_t blocksize, uint32_t capacity);
		~BlockAllocator();

		BlockAllocator(const BlockAllocator&) = delete;
		BlockAllocator& operator=(const BlockAllocator&) = delete;

	protected:
		uint32_t GetCapacity() { return mCapacity; }
		void* Allocate();

		template<typename T>
		void SafeFree(T*& ptr) { Free(ptr); ptr = nullptr; }
		void Free(void* ptr);
		int8_t* mData {nullptr};
		uint32_t mBlockSize;
		uint32_t mCapacity;
		uint32_t mSizeOfData { 0 };
		
		int mFreeSlot { 0 };
	};


	template<class DataType>
	class ObjectPool : private BlockAllocator
	{
	public:
		ObjectPool(uint32_t capacity)
			: BlockAllocator(sizeof(DataType), capacity)
		{
		}

		~ObjectPool()
		{
		}
		uint32_t GetCapacity() const { return BlockAllocator::GetCapacity(); }
		// Variadic Template Arguments 
		template<class... Args>
		DataType* New(Args&&... args);

		void Delete(DataType* ptr);
	};

	template<class DataType>
	template<class... Args>
	DataType* ObjectPool<DataType>::New(Args&&... args)
	{
		DataType* instance = static_cast<DataType*>(Allocate());
		if (instance)
		{
			// Perfect Forwarding
			new (instance) DataType(std::forward<Args>(args)...);
		}
		return instance;
	}

	template<class DataType>
	void ObjectPool<DataType>::Delete(DataType* ptr)
	{
		if (ptr == nullptr)
		{
			return;
		}
		ptr->~T();
		Free(ptr);
	}

	template<typename T>
	class ThreadSafeObjectPool : public ObjectPool<T>
	{
	public:
		template<typename... P>
		T* New(P &&... p)
		{
			std::lock_guard<std::mutex> holder{ lock };
			return ObjectPool<T>::New(std::forward<P>(p)...);
		}

		void Delete(T* ptr)
		{
			std::lock_guard<std::mutex> holder{ lock };
			ObjectPool<T>::Delete(ptr);
		}

	private:
		std::mutex lock;
	};

} //SingularityEngine::Core

#endif // INCLUDED_CORE_BLOCKALLOCATOR_H

