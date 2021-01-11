#ifndef SINGULARITY_FINITE_STATE_MACHINE_
#define SINGULARITY_FINITE_STATE_MACHINE_
#include "Common.h"
#include "AIEntity.h"
namespace SingularityEngine::AI
{
	template<typename entity_type>
	class State
	{
	public:
		virtual void Enter(entity_type* entity) = 0;

		virtual void Execute(entity_type* entity, float deltaTime) = 0;
		virtual void Exit(entity_type* entity) = 0;
		virtual bool OnMessage(entity_type*, const AITelegram& msg) { return false; };

	};

	template<typename entity_type>
	class StateMachine
	{
	public:
		StateMachine(entity_type* owner) : mOwner(owner), mCurrentState(nullptr), mPrevState(nullptr), mGlobalState(nullptr) {}
		void Terminate() {}
		void Initialize(State<entity_type>* startState, State<entity_type>*  startingPrevState, State<entity_type>*  startingGlobalState)
		{
			mCurrentState = startState;
			if (mCurrentState)
				mCurrentState->Enter(mOwner);
			mPrevState = startingPrevState;
			mGlobalState = startingGlobalState;
			if (mGlobalState)
				mGlobalState->Enter(mOwner);
		}
		void ChangeState(State<entity_type>* state)
		{
			ASSERT(state, "[Singularity::AI] State passed in was null");
			mCurrentState->Exit(mOwner);
			mPrevState = mCurrentState;
			mCurrentState = state;
			mCurrentState->Enter(mOwner);
		}
		void RevertToPreviousState() { ChangeState(mPrevState); }

		void ChangeGlobalState(State<entity_type>* state) {
			ASSERT(state, "[Singularity::AI] State passed in was null");
			if(mGlobalState)
				mGlobalState->Exit(mOwner);
			mGlobalState = state;
			mGlobalState->Enter(mOwner);
		}
		void Update(float deltaTime)
		{
			if (mGlobalState)
				mGlobalState->Execute(mOwner, deltaTime);

			if (mCurrentState)
				mCurrentState->Execute(mOwner, deltaTime);
		}
		bool HandleMessage(const AITelegram& msg) const
		{
			if (mCurrentState && mCurrentState->OnMessage(mOwner, msg))
				return true;

			if (mGlobalState && mGlobalState->OnMessage(mOwner, msg))
				return true;

			return false;
		}


		State<entity_type>* GetCurrentState() const { return mCurrentState; }
		State<entity_type>* GetGlobalState() const { return mGlobalState; };
		State<entity_type>* GetPreviousState() const { return mPrevState; }

		bool isInState(const State<entity_type>* state) { return typeid(state).name() == typeid(mCurrentState).name(); }
	private:
		entity_type * mOwner;
		State<entity_type>* mCurrentState;
		State<entity_type>* mPrevState;
		State<entity_type>* mGlobalState;
	};
}


#endif // BASICE_FINITE_STATE_MACHINE_
