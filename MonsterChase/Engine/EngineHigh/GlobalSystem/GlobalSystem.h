#pragma once
#include "Vendor/Json.h"
#include "Core/SmartPointers/SmartPtr.h"
#include "GameCommon/GameObject/EC.h"
#include <string>
namespace Engine
{
	namespace GlobalSystem
	{
		SmartPtr<Entity> GetEntityByName(std::string i_Name);
		SmartPtr<Entity> GetEntityAt(size_t i_Index);
		void CheckForNewEntities();
		void AddEntityToWorld(std::string i_EntityName, Json EntityData);
		void Shutdown();
	}
}