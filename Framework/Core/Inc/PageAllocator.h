#ifndef SINGULARITY_ENGINE_CORE_PAGE_ALLOCATOR
#define SINGULARITY_ENGINE_CORE_PAGE_ALLOCATOR

namespace SingularityEngine::Core
{
	struct PageAllocation
	{
		void* pageAddress;
		size_t size;
	};

	bool allocatePage(size_t size, PageAllocation* pageAllocationInfo);
	bool freePage(size_t size, PageAllocation* pageAllocationInfo)
}

#endif //SINGULARITY_ENGINE_CORE_PAGE_ALLOCATOR