#include "CollisionSystem.h"
#include "GameCommon/GameObject/Components.h"
#include "Core/JobSystem/Syncronization/ScopeLock.h"
#include "Core/JobSystem/Syncronization/Mutex.h"
#include "Core/Math/Matrix4.h"
#include <math.h>
#include <assert.h>
namespace Engine
{
	Mutex NewCollisionMutex;
	bool Collision::Init()
	{
		return true;
	}
	void Collision::UpdateCollidingObjects()
	{
		ScopeLock Lock(NewCollisionMutex);
		for (auto newCollidable : m_NewCollisionObjects)
		{
			m_CollisionObjects.push_back(newCollidable);
		}
		m_NewCollisionObjects.clear();
	}
	void Collision::Update(float deltaTime)
	{
		Collision& Instance = COLLISION;
		for (size_t i = 0; i < Instance.m_CollisionObjects.size(); ++i)
		{
			if (Instance.m_CollisionObjects[i])
			{
				SmartPtr<Entity> ICollisionObject = Instance.m_CollisionObjects[i].Acquire();
				if (ICollisionObject->getactive())
				{
					for (size_t j = 0; j < Instance.m_CollisionObjects.size(); ++j)
					{
						if (Instance.m_CollisionObjects[j])
						{
							SmartPtr<Entity> JCollisionObject = Instance.m_CollisionObjects[j].Acquire();
							if (JCollisionObject->getactive())
							{
								if (!(ICollisionObject == JCollisionObject))
								{
									if (!CheckAABB(ICollisionObject, JCollisionObject))
									{
										ICollisionObject->getComponent<Transform>()->setVelocity() = ICollisionObject->getComponent<Transform>()->getVelocity()*-1;
										JCollisionObject->getComponent<Transform>()->setVelocity() += ICollisionObject->getComponent<Transform>()->getVelocity();
										TRACE_INFO("Collison Occured");
									}
								}
							}
						}
						
					}
				}
				
				
			}
		}
	}
	void Collision::Run(Timer delta)
	{
		Get().UpdateCollidingObjects();
		Get().Update(delta.GetSeconds());
	}
	void Collision::ShutDown()
	{
		delete &Get();
	}
	void Collision::AddCollisionObject(SmartPtr<Entity> i_CollisionBody)
	{
		ScopeLock Lock(NewCollisionMutex);
		Get().m_NewCollisionObjects.emplace_back(i_CollisionBody);
	}
	bool Collision::CheckAABB(SmartPtr<Entity>& i_A, SmartPtr<Entity>& i_B)
	{
		/*if (i_rectA.x + (i_rectA.w) >= i_rectB.x && i_rectB.x + (i_rectB.w) >= i_rectA.x
			&& i_rectA.y + (i_rectA.h) >= i_rectB.y && i_rectB.y + (i_rectB.h) >= i_rectA.y)
		{
			return true;
		}*/
		bool isSeperated = true;

		MATH_API::Matrix4 mRotA = MATH_API::Matrix4::CreateZRotation(i_A->getComponent<SpriteRenderer>()->GetZRotation());
		MATH_API::Matrix4 mTransA = MATH_API::Matrix4::CreateTranslation(MATH_API::Vector3(i_A->getComponent<SpriteRenderer>()->Offset().x, i_A->getComponent<SpriteRenderer>()->Offset().y,0.0f));
		MATH_API::Matrix4 mAToWorld = mTransA * mRotA;
		MATH_API::Matrix4 mWorldToA = mAToWorld.GetInverse();
		//MATH_API::Matrix4 IdentityA = mWorldToA * mAToWorld;
		MATH_API::Matrix4 mRotB = MATH_API::Matrix4::CreateZRotation(i_B->getComponent<SpriteRenderer>()->GetZRotation());
		MATH_API::Matrix4 mTransB = MATH_API::Matrix4::CreateTranslation(MATH_API::Vector3(i_B->getComponent<SpriteRenderer>()->Offset().x, i_B->getComponent<SpriteRenderer>()->Offset().y, 0.0f));
		MATH_API::Matrix4 mBToWorld = mTransB * mRotB;
		MATH_API::Matrix4 mWorldToB = mBToWorld.GetInverse();
		//MATH_API::Matrix4 IdentityB = mWorldToB * mBToWorld;

		MATH_API::Matrix4 mAToB = mWorldToB * mAToWorld;
		MATH_API::Matrix4 mBToA = mAToB.GetInverse();
		//MATH_API::Matrix4 mBToA1 = mWorldToA * mBToWorld;
		//Check in B
		MATH_API::Vector4 A_BBCenterInB = mAToB * MATH_API::Vector4(i_A->getComponent<ColliderComponent>()->GetBB().Center.x(), i_A->getComponent<ColliderComponent>()->GetBB().Center.y(),0.0f,1.0f);
		MATH_API::Vector4 A_ExtentsXInB = mAToB * MATH_API::Vector4(i_A->getComponent<ColliderComponent>()->GetBB().Extents.x(),0.0f,0.0f,0.0f);
		MATH_API::Vector4 A_ExtentsYInB = mAToB * MATH_API::Vector4(0.0f, i_A->getComponent<ColliderComponent>()->GetBB().Extents.y(), 0.0f, 0.0f);
		float A_ProjectionInB_X = fabs(A_ExtentsXInB.x()) + fabs(A_ExtentsYInB.x());
		float A_ProjectionInB_Y = fabs(A_ExtentsXInB.y()) + fabs(A_ExtentsYInB.y());
		//Along B X-Axis
		isSeperated = fabs(A_BBCenterInB.x() - i_B->getComponent<ColliderComponent>()->GetBB().Center.x()) > i_B->getComponent<ColliderComponent>()->GetBB().Extents.x() + A_ProjectionInB_X;
		//Along B Y-Axis
		if(!isSeperated)
			isSeperated = fabs(A_BBCenterInB.y() - i_B->getComponent<ColliderComponent>()->GetBB().Center.y()) > i_B->getComponent<ColliderComponent>()->GetBB().Extents.y() + A_ProjectionInB_Y;

		MATH_API::Vector4 B_BBCenterInA = mBToA * MATH_API::Vector4(i_B->getComponent<ColliderComponent>()->GetBB().Center.x(), i_B->getComponent<ColliderComponent>()->GetBB().Center.y(), 0.0f, 1.0f);
		MATH_API::Vector4 B_ExtentsXInA = mBToA * MATH_API::Vector4(i_B->getComponent<ColliderComponent>()->GetBB().Extents.x(), 0.0f, 0.0f, 0.0f);
		MATH_API::Vector4 B_ExtentsYInA = mBToA * MATH_API::Vector4(0.0f, i_B->getComponent<ColliderComponent>()->GetBB().Extents.y(), 0.0f, 0.0f);
		float B_ProjectionInA_X = fabs(B_ExtentsXInA.x()) + fabs(B_ExtentsYInA.x());
		float B_ProjectionInA_Y = fabs(B_ExtentsXInA.y()) + fabs(B_ExtentsYInA.y());

		if(!isSeperated)
			isSeperated = fabs(B_BBCenterInA.x() - i_A->getComponent<ColliderComponent>()->GetBB().Center.x()) > i_A->getComponent<ColliderComponent>()->GetBB().Extents.x() + B_ProjectionInA_X;

		if (!isSeperated)
			isSeperated = fabs(B_BBCenterInA.y() - i_A->getComponent<ColliderComponent>()->GetBB().Center.y()) > i_A->getComponent<ColliderComponent>()->GetBB().Extents.y() + B_ProjectionInA_Y;
		
		return isSeperated;
	}
}