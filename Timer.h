#ifndef _TIMER_H_
#define _TIMER_H_

#include <Windows.h>

namespace radx
{
	class Timer
	{
	public:
		Timer() { ZeroMemory(this, CLASS_BYTE_SIZE); }
		~Timer() = default;

		void InitTime() 
		{ 
			QueryPerformanceFrequency(&mFrequency);
			QueryPerformanceCounter(&mOldTime); 
		}

		inline void SetTime()
		{
			mOldTime = mCurTime;
		}

		inline float GetTime() 
		{
			QueryPerformanceCounter(&mCurTime);
			return mDeltaTime = (mCurTime.QuadPart - mOldTime.QuadPart) / (float)mFrequency.QuadPart;
		}

	private:
		enum { CLASS_BYTE_SIZE = 28 };
		LARGE_INTEGER	mFrequency;
		LARGE_INTEGER	mOldTime;
		LARGE_INTEGER	mCurTime;
		float			mDeltaTime;
	};

}

#endif