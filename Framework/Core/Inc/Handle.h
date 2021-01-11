#ifndef INCLUDED_HANDLE_HEADER_
#define INCLUDED_HANDLE_HEADER_

namespace SingularityEngine::Core
{
	template<class DataType>
	class HandlePool;


	template<class DataType>
	class Handle
	{
	public:
		Handle();
		bool IsValid() const;
		void Invalidate();
		DataType* Get() const;
		DataType* operator->() const;
		
		bool operator == (Handle rhs) const { return mIndex == rhs.mIndex && mGeneration == rhs.mGeneration; }



		bool operator!=(Handle rhs) const { return !(*this == rhs); }
	private:
		friend class HandlePool<DataType>;
		static const HandlePool<DataType>* sPool;
		uint32_t mIndex : 24;
		uint32_t mGeneration : 8;
	};

	template<class DataType>
	const HandlePool<DataType>* Handle<DataType>::sPool = nullptr;


	template<class DataType>
	DataType* SingularityEngine::Core::Handle<DataType>::operator->() const
	{
		return sPool ? sPool->Get(*this) : nullptr;
	}

	template<class DataType>
	DataType* SingularityEngine::Core::Handle<DataType>::Get() const
	{
		return sPool ? sPool->Get(*this) : nullptr;
	}

	template<class DataType>
	void SingularityEngine::Core::Handle<DataType>::Invalidate()
	{
		*this = Handle();
	}

	template<class DataType>
	bool SingularityEngine::Core::Handle<DataType>::IsValid() const
	{
		return sPool && sPool->IsValid(*this);
	}

	template<class DataType>
	SingularityEngine::Core::Handle<DataType>::Handle() : mIndex(0), mGeneration(0)
	{

	}
}






#endif