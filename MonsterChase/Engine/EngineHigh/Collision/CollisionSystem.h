#pragma once
#include "Core/SmartPointers/WeakPtr.h"
#include "GameCommon/GameObject/EC.h"
#include "EngineLow/Timer.h"
#include "Core/Math/Vector3.h"
#include "Core/Structs/Structs.h"
#define COLLISION Engine::Collision::Get()
namespace Engine
{
	class Collision
	{
	public:
		Collision(const Collision& i_Other) = delete;
		Collision& operator=(const Collision& i_Other) = delete;
		static Collision& Get()
		{
			static Collision* Instance = new Collision();
			return *Instance;
		}
		static bool Init();
		static void AddCollisionObject(const SmartPtr<Entity>& i_CollisionBody);
		static void Run(Timer delta);
		static void ShutDown();
		void CheckForNewCollidables();
		void Update(Timer deltaTime);
		void RefreshCollidables();
		bool CheckCollision(SmartPtr<Entity>& i_A, SmartPtr<Entity>& i_B, Timer deltaTime,float& i_CollisionTime,MATH_API::Vector3& i_ColNormal);
	private:
		Collision() {}
		vector<WeakPtr<Entity>>   m_CollisionObjects;
		vector<WeakPtr<Entity>>   m_NewCollisionObjects;
	};
}