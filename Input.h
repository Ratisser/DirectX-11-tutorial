#pragma once

namespace radx
{
	class Input
	{
	public:
		Input();
		~Input();

		__forceinline static Input& GetInstance() 
		{
			static Input singleton; 
			return singleton;
		}

		void KeyDown(unsigned int input);
		void KeyUp(unsigned int input);

		bool IsKeyDown(unsigned int input);

	private:
		enum { KEYS = 256 };
		bool mbKeys[KEYS];
	};
}