#include "Precompiled.h"
#include "Random.h"
using namespace SingularityEngine;
using namespace Core;
namespace {
	Random* randomGenerator;
}

Random* Random::Get()
{
	return randomGenerator;
}
void Random::StaticInitialize()
{
	randomGenerator = new Random();
	randomGenerator->Initialize();
}
void Random::StaticTerminate()
{
	randomGenerator->Terminate();
	delete randomGenerator;
}

void Random::Seed(const std::string seed)
{
	mSeedStr = seed;
	std::seed_seq seedSeq(mSeedStr.begin(), mSeedStr.end());
	mRandomNumberGenerator.seed(seedSeq);
}

int Random::UniformIntDistribution(int min, int max)
{
	std::uniform_int_distribution<int> uniformDistibution(min, max);
	return uniformDistibution(mRandomNumberGenerator);
}
void Random::UniformIntDistribution(int min, int max, int* outputArray, int arraySize)
{
	std::uniform_int_distribution<int> uniformDistibution(min, max);
	for (int i = 0; i < arraySize; i++)
	{
		outputArray[i] = uniformDistibution(mRandomNumberGenerator);
	}
}

float Random::UniformFloatDistibution(float min, float max)
{
	std::uniform_real_distribution<float> uniformDistibution(min, max);
	return uniformDistibution(mRandomNumberGenerator);
}
float Random::GetF(float min, float max)
{
	std::fisher_f_distribution<float> randomDist(min, max);
	return randomDist(mRandomNumberGenerator);
}

void Random::UniformFloatDistribution(float min, float max, float* outputArray, int arraySize)
{
	std::uniform_real_distribution<float> uniformDistibution(min, max);
	for (int i = 0; i < arraySize; i++)
	{
		outputArray[i] = uniformDistibution(mRandomNumberGenerator);
	}
}

void Random::UniformFloatDistribution(float min, float max, std::vector<float>& outputArray, int arraySize)
{
	std::uniform_real_distribution<float> uniformDistibution(min, max);
	outputArray.clear();
	for (int i = 0; i < arraySize; i++)
	{
		outputArray.push_back(uniformDistibution(mRandomNumberGenerator));
	}
}

Random::Random()
{
}
Random::~Random()
{

}
void Random::Initialize()
{

}
void Random::Terminate()
{

}