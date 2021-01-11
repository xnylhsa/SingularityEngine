//====================================================================================================
// Filename:	Timer.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "Timer.h"
using namespace SingularityEngine;
using namespace Core;

Timer::Timer()
	: mElapsedTime(0.0f)
	, mTotalTime(0.0f)
	, mLastUpdateTime(0.0f)
	, mFrameSinceLastSecond(0.0f)
	, mFramesPerSecond(0.0f)
{
}

void Timer::Initialize()
{
	// Get the current time
	mLastTime = std::chrono::high_resolution_clock::now();

	// Reset
	mElapsedTime = 0.0f;
	mTotalTime = 0.0f;
	mLastUpdateTime = 0.0f;
	mFrameSinceLastSecond = 0.0f;
	mFramesPerSecond = 0.0f;
}

void Timer::Update()
{
	// Get the current time 
	mCurrentTime = std::chrono::high_resolution_clock::now();

	// Calculate the total time and elapsed time
	mElapsedTime = std::chrono::duration<float>(mCurrentTime - mLastTime).count();
	mTotalTime += mElapsedTime;

	// Update the last tick count
	mLastTime = mCurrentTime;

	// Calculate the FPS
	mFrameSinceLastSecond += 1.0f;
	if (mTotalTime >= mLastUpdateTime + 1.0f)
	{
		mFramesPerSecond = mFrameSinceLastSecond / (mTotalTime - mLastUpdateTime);
		mFrameSinceLastSecond = 0.0f;
		mLastUpdateTime = mTotalTime;
	}
}

float Timer::GetElapsedTime() const
{
	return mElapsedTime;
}

float Timer::GetTotalTime() const
{
	return mTotalTime;
}

float Timer::GetFPS() const
{
	return mFramesPerSecond;
}