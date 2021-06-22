/*
타이머들을 전역으로 관리하기 위해만든 클래스
*/

#pragma once

#include "Timer.h"

namespace radx
{
	class TimerManager
	{
	public:
		TimerManager() = default;
		~TimerManager() = default;

		__forceinline static TimerManager& GetInstance()
		{
			static TimerManager instance;
			return instance;
		}

		Timer SystemTimer;
		Timer DeltaTimeTimer;
	};


}