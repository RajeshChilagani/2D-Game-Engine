#include "PhysicsCore.h"
#include "GameCommon/GameObject/Components.h"
#include "Core/JobSystem/Syncronization/Mutex.h"
#include "Core/JobSystem/Syncronization/ScopeLock.h"
#include <assert.h>
namespace Engine
{
	Mutex NewPhysicsMutex;
	bool Physics::Init()
	{
		return true;
	}
	void Physics::UpdatePhysicsObjects()
	{
		ScopeLock Lock(NewPhysicsMutex);
		for (auto newRenderable : m_NewPhysicsObjects)
		{
			m_PhysicsObjects.push_back(newRenderable);
		}
		m_NewPhysicsObjects.clear();
	}
	void Physics::RefreshPhysicsObjects()
	{
		for (unsigned int i = 0; i < m_PhysicsObjects.size(); i++)
		{
			if (!(m_PhysicsObjects[i]))
			{
				m_PhysicsObjects.erase(m_PhysicsObjects.begin() + i);
			}
		}
	}
	void Physics::Update(float deltaTime)
	{
		Physics& Instance = PHYSICS;
		for (size_t i = 0; i < Instance.m_PhysicsObjects.size(); ++i)
		{				
			if (Instance.m_PhysicsObjects[i])
			{
				SmartPtr<Entity> PhysicsObject = Instance.m_PhysicsObjects[i].Acquire();
				if (PhysicsObject->getComponent<PhysicsBody>()->Simulate())
				{
					MATH_API::Vector3 Force = PhysicsObject->getComponent<PhysicsBody>()->ExternalForce() + (-PhysicsObject->getComponent<PhysicsBody>()->Drag() * PhysicsObject->getComponent<Transform>()->getVelocity());
					if (PhysicsObject->getComponent<PhysicsBody>()->EnableGravity())
					{
						Force += m_Gravity;
					}
					if (PhysicsObject->getactive())
					{
						MATH_API::Vector3 Acceleration = Force / PhysicsObject->getComponent<PhysicsBody>()->Mass();
						MATH_API::Vector3 OldVelocity = PhysicsObject->getComponent<Transform>()->getVelocity();
						PhysicsObject->getComponent<Transform>()->setVelocity() = OldVelocity + Acceleration * deltaTime;
						PhysicsObject->getComponent<Transform>()->setPosition() = PhysicsObject->getComponent<Transform>()->getPosition() + ((OldVelocity + PhysicsObject->getComponent<Transform>()->getVelocity()) / 2)*deltaTime;

					}
				}
			}
		}
	}
	void Physics::Run(Timer delta)
	{
		Get().UpdatePhysicsObjects();
		Get().Update(delta.GetSeconds());
	}
	void Physics::ShutDown()
	{
		delete &Get();
	}
	void Physics::AddPhysicsObject(const SmartPtr<Entity>& i_PhysicsBody)
	{
		ScopeLock Lock(NewPhysicsMutex);
		Get().m_NewPhysicsObjects.emplace_back(i_PhysicsBody);
	}
}