#pragma once
#include "Vendor/Json.h"
namespace Engine
{
	namespace GlobalSystem
	{
		void CheckForNewEntities();
		void AddEntityToWorld(std::string i_EntityName, Json EntityData);
		void Shutdown();
	}
}