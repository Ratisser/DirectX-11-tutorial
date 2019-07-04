#include "pch.h"
#include "Input.h"

namespace radx
{
	Input::Input()
	{
		for (size_t i = 0; i < KEYS; i++)
		{
			mbKeys[i] = 0;
		}
	}


	Input::~Input()
	{
	}

	void Input::KeyDown(unsigned int input)
	{
		mbKeys[input] = true;
	}

	void Input::KeyUp(unsigned int input)
	{
		mbKeys[input] = false;
	}

	bool Input::IsKeyDown(unsigned int input)
	{
		return mbKeys[input];
	}

}