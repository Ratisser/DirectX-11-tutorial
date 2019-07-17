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