#ifndef _TIMER_H_
#define _TIMER_H_

#include <Windows.h>
// TODO : 타이머 기능 강화
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
		enum { CLASS_BYTE_SIZE = 28 };
		LARGE_INTEGER	mFrequency;
		LARGE_INTEGER	mOldTime;
		LARGE_INTEGER	mCurTime;
		float			mDeltaTime;

		bool			mbStop;
	};

}

#endif