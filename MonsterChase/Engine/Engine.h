#pragma once
#include "EngineLow/Timer.h"
#include <Windows.h>
#include <iostream>
using namespace std;
namespace Engine
{
	extern bool CloseEngine;
	extern Timer deltaTime;
	bool Init();
	void Run();
	void ShutDown();
	
}