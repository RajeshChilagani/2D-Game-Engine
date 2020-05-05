#pragma once
#include "GameCommon/GameObject/EC.h"
#include "Core/Math/Vector3.h"
#include "EngineLow/Timer.h"
#include "Core/SmartPointers/WeakPtr.h"
#include "Core/SmartPointers/SmartPtr.h"
#define PHYSICS Engine::Physics::Get()
namespace Engine
{
	class Physics
	{
		public:
			Physics(const Physics& i_Other) = delete;
			Physics& operator=(const Physics& i_Other) = delete;
			inline static Physics& Get()
			{
				static Physics* Instance = new Physics();
				return *Instance;
			}
			static bool Init();
			static void AddPhysicsObject(const SmartPtr<Entity>& i_PhysicsBody);
			static void Run(Timer delta);
			static void ShutDown();
			void UpdatePhysicsObjects();
			void RefreshPhysicsObjects();
			void Update(float deltaTime);
		private:
			Physics() :m_Gravity(MATH_API::Vector3(0.0f, -9.8f, 0.0f)) {}
			MATH_API::Vector3 m_Gravity;
			vector<WeakPtr<Entity>>   m_PhysicsObjects;
			vector<WeakPtr<Entity>>   m_NewPhysicsObjects;
	};
}