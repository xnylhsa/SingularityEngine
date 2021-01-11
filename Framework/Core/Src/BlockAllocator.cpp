#include "Precompiled.h"
#include "BlockAllocator.h"
#include "Common.h"
#include "Debug.h"
#include "DeleteUtil.h"
using namespace SingularityEngine::Core;
BlockAllocator::BlockAllocator(uint32_t blocksize, uint32_t capacity) : mBlockSize(blocksize), mCapacity(capacity), mSizeOfData(mBlockSize * mCapacity)
{
	ASSERT(mData == nullptr, "[Core::Memory] STOP WHAT IN THE FUCK YOUR DOING AND RE THINK HOW THIS HAPPENED! Honestly what is wrong with you?");
	mData = new(std::nothrow) int8_t[mBlockSize * mCapacity];
	ASSERT(mData != nullptr, "[Core::Memory] failed to allocate memory!");
	for (size_t i = 0; i < mCapacity - 1; i++)
	{
		mData[i*mBlockSize] = (int8_t)i + 1;
	}
	mData[(mCapacity - 1) * mBlockSize] = -1;
	mFreeSlot = 0;
}

BlockAllocator::~BlockAllocator()
{
	SafeDeleteArray(mData);
}

void* BlockAllocator::Allocate()
{
	ASSERT(mFreeSlot != -1, "Max allocations reached! Expand Dong!");
	if (mFreeSlot == -1) return nullptr;
	int8_t* freeslot = &mData[mFreeSlot*mBlockSize];
	mFreeSlot = *freeslot;
	*freeslot = 0;
	return freeslot;
}

void BlockAllocator::Free(void* ptr)
{
	ptrdiff_t freedSlot = ((char*)ptr - (char*)mData) / mBlockSize;
	int8_t* slot = (int8_t*)ptr;
	for (int8_t* i = slot; (char*)i < (char*)ptr + mBlockSize; i++)
	{
		*i = 0;
	}
	*slot = (int8_t)mFreeSlot;
	mFreeSlot = (int)freedSlot;
	ptr = nullptr;
}

