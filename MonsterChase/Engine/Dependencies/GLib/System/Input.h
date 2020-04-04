#pragma once

#include <Windows.h>

#include "BasicTypes.h"

namespace GLib
{
	namespace Input
	{
		bool Initialize();
		void Service(HRAWINPUT i_Input);
		void Shutdown();
	} // namespace Input
} // namespace GLib

