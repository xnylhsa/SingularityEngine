#ifndef SINGULARITY_AI_BLACK_BOARD_
#define SINGULARITY_AI_BLACK_BOARD_
#include "Common.h"
#include "BehaviorTypes.h"


namespace SingularityEngine::AI::Behavior
{



	template<class T>
	class BlackBoardEntry
	{
		BlackBoardEntry(std::string description) : mDescription(description) {}
		BlackBoardEntry(std::string description,const T& val) : mDescription(description), mValue(val) {}
		const T& GetValue() const { return mValue; }
		void SetValue(T val) { mValue = val; }
		const std::string& GetDescription() const { return mDescription; }
	private:
		std::string mDescription;
		T mValue;
	};

	typedef std::unordered_map<std::string, BlackBoardEntry<bool>> BoolEntryMap;
	typedef std::unordered_map<std::string, BlackBoardEntry<float>> RealEntryMap;
	typedef std::unordered_map<std::string, BlackBoardEntry<int32_t>> IntegerEntryMap;
	typedef std::unordered_map<std::string, BlackBoardEntry<Math::Vector2>> Position2DEntryMap;
	typedef std::unordered_map<std::string, BlackBoardEntry<Math::Vector3>> PositionEntryMap;
	typedef std::unordered_map<std::string, BlackBoardEntry<Math::Quaternion>> RotationEntryMap;
	typedef std::unordered_map<std::string, BlackBoardEntry<Math::Color>> ColorEntryMap;

	class BlackBoard
	{
	public:
		template<typename EntryType>
		const BlackBoardEntry<EntryType>& GetBlackBoardEntry(const std::map<std::string, BlackBoardEntry<EntryType>>& entryMap, std::string key) const
		{
			auto it = entryMap.find(key);
			ASSERT(it != entryMap.end(), "[AI:Behavior::BlackBoard] Failed to Find entry with the given key!");

			return it->second;
		}
		template<typename EntryType>
		void SetBlackBoardEntry(std::map<std::string, BlackBoardEntry<EntryType>>& entryMap, std::string key, const EntryType& entry)
		{
			auto it = entryMap.find(key);
			ASSERT(it != entryMap.end(), "[AI:Behavior::BlackBoard] Failed to Find entry with the given key! Set Function is used to modify exitsting entrys and not to Create Them!");
			it->second.SetValue(entry);
		}
		template<typename EntryType>
		void CreateBlackBoardEntry(std::map<std::string, BlackBoardEntry<EntryType>>& entryMap, std::string key, const EntryType& entry, std::string description)
		{
			auto it = entryMap.find(key);
			ASSERT(it == entryMap.end(), "[AI:Behavior::BlackBoard] Failed to create entry because a entry already existed with the given key!");
			entryMap.emplace(key, BlackBoardEntry<EntryType>(description, entry));
		}

	protected:
		BoolEntryMap mBoolEntries;
		RealEntryMap mRealEntries;
		IntegerEntryMap mIntegerEntries;
		Position2DEntryMap mPosition2DEntries;
		PositionEntryMap mPositionEntries;
		RotationEntryMap mRotationEntries;
		ColorEntryMap mColorEntries;

	};
}


#endif