#ifndef CORE_RANDOM_HEADER_
#define CORE_RANDOM_HEADER_
#include "Common.h"
namespace SingularityEngine
{
	namespace Core
	{

		class Random
		{
		public:
			static Random* Get();
			static void StaticInitialize();
			static void StaticTerminate();
			void Seed(const std::string seed);

			int UniformIntDistribution(int min, int max);
			void UniformIntDistribution(int min, int max, int* outputArray, int arraySize);
			float GetF(float min, float max);
			float UniformFloatDistibution(float min, float max);
			void UniformFloatDistribution(float min, float max, float* outputArray, int arraySize);
			void UniformFloatDistribution(float min, float max, std::vector<float>& outputArray, int arraySize);

		private:
			Random();
			~Random();
			void Initialize();
			void Terminate();
#ifdef ENVIRONMENT64
			std::mt19937_64 mRandomNumberGenerator;
#else
			std::mt19937 mRandomNumberGenerator;
#endif
			std::string mSeedStr;

			float mElapsedTime;
			float mTotalTime;

			float mLastUpdateTime;
			float mFrameSinceLastSecond;
			float mFramesPerSecond;
		};

	} // namespace Core
}
#endif