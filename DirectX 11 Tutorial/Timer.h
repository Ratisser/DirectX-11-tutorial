#ifndef _TIMER_H_
#define _TIMER_H_

#include <Windows.h>
namespace radx
{
	class Timer
	{
	public:
		Timer() = default;
		~Timer() = default;

		void InitTime() 
		{ 
			QueryPerformanceFrequency(&mFrequency);
			QueryPerformanceCounter(&mOldTime); 
			QueryPerformanceCounter(&mCurTime);
			mbStop = true;
		}

		void StopTimer()
		{
			mbStop = true;
		}

		void StartTimer()
		{
			mbStop = false;
		}

		bool IsTimerStoped()
		{
			return mbStop;
		}

		inline void ResetTime()
		{
			mOldTime = mCurTime;
		}

		inline float GetTime() 
		{
			if (mbStop)
			{
				return 0.0f;
			}
			QueryPerformanceCounter(&mCurTime);
			return mDeltaTime = (mCurTime.QuadPart - mOldTime.QuadPart) / (float)mFrequency.QuadPart;
		}

	private:
		LARGE_INTEGER	mFrequency;
		LARGE_INTEGER	mOldTime;
		LARGE_INTEGER	mCurTime;
		float			mDeltaTime;

		bool			mbStop;
	};

}

#endif