#include "CollisionSystem.h"
#include "GameCommon/GameObject/Components.h"
#include "Core/JobSystem/Syncronization/ScopeLock.h"
#include "Core/JobSystem/Syncronization/Mutex.h"
#include "Core/Math/Matrix4.h"
#include <math.h>
#include <assert.h>
#include <map>
namespace Engine
{
	struct CollisionPair
	{
		float              CollisionTime;
		MATH_API::Vector3  CollisonNormal;
		WeakPtr<Entity>    CollidedObjects[2];
	};
	Mutex NewCollisionMutex;
	bool Collision::Init()
	{
		return true;
	}
	void Collision::CheckForNewCollidables()
	{
		ScopeLock Lock(NewCollisionMutex);
		for (auto newCollidable : m_NewCollisionObjects)
		{
			m_CollisionObjects.push_back(newCollidable);
		}
		m_NewCollisionObjects.clear();
	}
	void Collision::RefreshCollidables()
	{
		for (unsigned int i = 0; i < m_CollisionObjects.size(); i++)
		{
			if (!(m_CollisionObjects[i]))
			{
				m_CollisionObjects.erase(m_CollisionObjects.begin() + i);
			}
		}
	}
	void Collision::Update(Timer deltaTime)
	{	
		float tEnd = deltaTime.GetSeconds();
		float dt = tEnd;
		float tBegin = 0;
		do
		{	
			CollisionPair EarliestCollision;
			EarliestCollision.CollisionTime = dt;
			size_t CollidablesCount = m_CollisionObjects.size();
			if (CollidablesCount > 1)
			{
				for (size_t i = 0; i < CollidablesCount - 1; ++i)
				{
					SmartPtr<Entity> ICollisionObject = m_CollisionObjects[i].Acquire();
					for (size_t j = i + 1; j < CollidablesCount; ++j)
					{
						float CollisionTime = tBegin;
						MATH_API::Vector3 ColNormal;
						SmartPtr<Entity> JCollisionObject = m_CollisionObjects[j].Acquire();
						if (CheckCollision(ICollisionObject, JCollisionObject, dt, CollisionTime, ColNormal))
						{
							TRACE_INFO("Collision Occured ->Normal: " << ColNormal);
							TRACE_INFO("Collision Occured->Time" << CollisionTime);
							if (CollisionTime < EarliestCollision.CollisionTime)
							{
								EarliestCollision.CollisionTime = CollisionTime;
								EarliestCollision.CollisonNormal = ColNormal;
								EarliestCollision.CollidedObjects[0] = m_CollisionObjects[i];
								EarliestCollision.CollidedObjects[1] = m_CollisionObjects[j];
							}

						}
					}
				}
			}
			if (EarliestCollision.CollidedObjects[0] && EarliestCollision.CollidedObjects[1])
			{
				for (size_t i = 0; i < CollidablesCount; ++i)
				{
					SmartPtr<Entity>&& Object = m_CollisionObjects[i].Acquire();
					MATH_API::Vector3 Distance = Object->getComponent<Transform>()->getVelocity()*EarliestCollision.CollisionTime;
					Object->getComponent<Transform>()->setPosition() = Object->getComponent<Transform>()->getPosition() + Distance;
				}
				SmartPtr<Entity> Object1 = EarliestCollision.CollidedObjects[0].Acquire();
				SmartPtr<Entity> Object2 = EarliestCollision.CollidedObjects[1].Acquire();
				if (Object1->HasComponent<PhysicsBody>() && Object2->HasComponent<PhysicsBody>()) 
				{
					MATH_API::Vector3 Normal = EarliestCollision.CollisonNormal;
					float m1 = Object1->getComponent<PhysicsBody>()->Mass();
					float m2 = Object2->getComponent<PhysicsBody>()->Mass();
					MATH_API::Vector3 v1 = Object1->getComponent<Transform>()->getVelocity();
					MATH_API::Vector3 v2 = Object2->getComponent<Transform>()->getVelocity();				
					if (Object1->getComponent<PhysicsBody>()->Simulate() && Object2->getComponent<PhysicsBody>()->Simulate())    //if both objects simulate physics
					{
						Object1->getComponent<Transform>()->setVelocity() = ((m1 - m2) / (m1 + m2))*v1 + ((2 * m2) / (m1 + m2))*v2;
						Object2->getComponent<Transform>()->setVelocity() = ((m2 - m1) / (m1 + m2))*v2 + ((2 * m1) / (m1 + m2))*v1;
						MATH_API::Vector3 newv1 = Object1->getComponent<Transform>()->getVelocity();
						MATH_API::Vector3 newv2 = Object2->getComponent<Transform>()->getVelocity();
						float TowardsNormal = MATH_API::dot(Normal, newv1);
						if (TowardsNormal < 0)
							Object1->getComponent<Transform>()->setVelocity() = (-newv1 - 2 * (Normal*MATH_API::dot(Normal, -newv1) - newv1));
						TowardsNormal = MATH_API::dot(Normal, newv2);
						if (TowardsNormal < 0)
							Object2->getComponent<Transform>()->setVelocity() = (-newv2 - 2 * (Normal*MATH_API::dot(Normal, -newv2) - newv2));
					}
					else if (Object1->getComponent<PhysicsBody>()->Simulate())
					{
						MATH_API::Vector3 newv1 = Object1->getComponent<Transform>()->getVelocity();
						Object1->getComponent<Transform>()->setVelocity() = (-newv1 - 2 * (Normal*MATH_API::dot(Normal, -newv1) - newv1));
					}
					else if (Object2->getComponent<PhysicsBody>()->Simulate())
					{
						MATH_API::Vector3 newv2 = Object2->getComponent<Transform>()->getVelocity();
						Object2->getComponent<Transform>()->setVelocity() = (-newv2 - 2 * (Normal*MATH_API::dot(Normal, -newv2) - newv2));
					}
				}
				
			}
			tBegin = EarliestCollision.CollisionTime;
			dt=dt- EarliestCollision.CollisionTime;
		} while (tBegin < tEnd && tBegin>0);
	}
	void Collision::Run(Timer delta)
	{
		Get().CheckForNewCollidables();
		Get().RefreshCollidables();
		Get().Update(delta.GetSeconds());
	}
	void Collision::ShutDown()
	{
		delete &Get();
	}
	void Collision::AddCollisionObject(const SmartPtr<Entity>& i_CollisionBody)
	{
		ScopeLock Lock(NewCollisionMutex);
		Get().m_NewCollisionObjects.emplace_back(i_CollisionBody);
	}
	bool Collision::CheckCollision(SmartPtr<Entity>& i_A, SmartPtr<Entity>& i_B, Timer deltaTime, float& i_CollisionTime, MATH_API::Vector3& i_ColNormal)
	{
		//Acquire required values//
		MATH_API::Vector3 A_Velocity = i_A->getComponent<Transform>()->getVelocity();
		MATH_API::Vector3 B_Velocity = i_B->getComponent<Transform>()->getVelocity();
		AABB& A_BB = i_A->getComponent<ColliderComponent>()->GetBB();
		AABB& B_BB = i_B->getComponent<ColliderComponent>()->GetBB();

		float BeginOverlap = i_CollisionTime;
		float EndOverlap = 1;

		//Create Matrices//
		MATH_API::Matrix4 mRotA = MATH_API::Matrix4::CreateZRotation(i_A->getComponent<SpriteRenderer>()->GetZRotation());
		MATH_API::Matrix4 mTransA = MATH_API::Matrix4::CreateTranslation(MATH_API::Vector3(i_A->getComponent<Transform>()->getPosition().x(), i_A->getComponent<Transform>()->getPosition().y(), 0.0f));
		MATH_API::Matrix4 mAToWorld = mTransA * mRotA;
		MATH_API::Matrix4 mWorldToA = mAToWorld.GetInverse();
		//MATH_API::Matrix4 IdentityA = mWorldToA * mAToWorld;
		MATH_API::Matrix4 mRotB = MATH_API::Matrix4::CreateZRotation(i_B->getComponent<SpriteRenderer>()->GetZRotation());
		MATH_API::Matrix4 mTransB = MATH_API::Matrix4::CreateTranslation(MATH_API::Vector3(i_B->getComponent<Transform>()->getPosition().x(), i_B->getComponent<Transform>()->getPosition().y(), 0.0f));
		MATH_API::Matrix4 mBToWorld = mTransB * mRotB;
		MATH_API::Matrix4 mWorldToB = mBToWorld.GetInverse();
		//MATH_API::Matrix4 IdentityB = mWorldToB * mBToWorld;

		MATH_API::Matrix4 mAToB = mWorldToB * mAToWorld;
		MATH_API::Matrix4 mBToA = mAToB.GetInverse();


		//A in B's Unique Normal axes//
		MATH_API::Vector3 A_VelRelToB = A_Velocity - B_Velocity;
		MATH_API::Vector4 A_VelRelToBInB = mWorldToB * MATH_API::Vector4(A_VelRelToB, 0.0f);

		MATH_API::Vector4 A_BBCenterInB = mAToB * MATH_API::Vector4(A_BB.Center.x(), A_BB.Center.y(), 0.0f, 1.0f);
		MATH_API::Vector4 A_ExtentsXInB = mAToB * MATH_API::Vector4(A_BB.Extents.x(), 0.0f, 0.0f, 0.0f);
		MATH_API::Vector4 A_ExtentsYInB = mAToB * MATH_API::Vector4(0.0f, A_BB.Extents.y(), 0.0f, 0.0f);

		//Along B's X-Axis
		{
			float A_ProjectionInB_X = fabs(A_ExtentsXInB.x()) + fabs(A_ExtentsYInB.x());
			float B_Extents_X = B_BB.Extents.x() + A_ProjectionInB_X;
			float B_Left_X = B_BB.Center.x() - B_Extents_X;
			float B_Right_X = B_BB.Center.x() + B_Extents_X;
			float B_DClose_X = B_Left_X - A_BBCenterInB.x();
			float B_DOpen_X = B_Right_X - A_BBCenterInB.x();
			float B_tClose_X = 0;
			float B_tOpen_X = 0;

			if (A_VelRelToBInB.x() != 0)
			{
				B_tClose_X = B_DClose_X / A_VelRelToBInB.x();
				B_tOpen_X = B_DOpen_X / A_VelRelToBInB.x();
				if (B_tOpen_X < B_tClose_X)
				{
					float temp = B_tOpen_X;
					B_tOpen_X = B_tClose_X;
					B_tClose_X = temp;
				}
				if (B_tOpen_X < 0 || B_tClose_X > deltaTime.GetSeconds())
				{
					return false;
				}
				if (B_tClose_X > BeginOverlap)
				{
					BeginOverlap = B_tClose_X;
					//MATH_API::Vector4 NormalAxisInWorld = mRotB.Col(0);
					MATH_API::Vector4 NormalAxisInWorld = mBToWorld*Vector4(1.0f,0.0f,0.0f,0.0f);
					i_ColNormal = MATH_API::Vector3(NormalAxisInWorld.x(), NormalAxisInWorld.y(), NormalAxisInWorld.z()).GetNormalized();
				}
				if (B_tOpen_X < EndOverlap)
				{
					EndOverlap = B_tOpen_X;
				}
			}
			else
			{
				if (!(A_BBCenterInB.x() > B_Left_X && A_BBCenterInB.x() < B_Right_X))
				{
					return false;
				}
			}
		}


		//Along B's Y-Axis
		{
			float A_ProjectionInB_Y = fabs(A_ExtentsXInB.y()) + fabs(A_ExtentsYInB.y());
			float B_Extents_Y = B_BB.Extents.y() + A_ProjectionInB_Y;
			float B_Left_Y = B_BB.Center.y() - B_Extents_Y;
			float B_Right_Y = B_BB.Center.y() + B_Extents_Y;
			float B_DClose_Y = B_Left_Y - A_BBCenterInB.y();
			float B_DOpen_Y = B_Right_Y - A_BBCenterInB.y();
			float B_tClose_Y = 0;
			float B_tOpen_Y = 0;

			if (A_VelRelToBInB.y() != 0)
			{
				B_tClose_Y = B_DClose_Y / A_VelRelToBInB.y();
				B_tOpen_Y = B_DOpen_Y / A_VelRelToBInB.y();
				if (B_tOpen_Y < B_tClose_Y)
				{
					float temp = B_tOpen_Y;
					B_tOpen_Y = B_tClose_Y;
					B_tClose_Y = temp;
				}
				if (B_tOpen_Y < 0 || B_tClose_Y > deltaTime.GetSeconds())
				{
					return false;
				}
				if (B_tClose_Y > BeginOverlap)
				{
					BeginOverlap = B_tClose_Y;
					//MATH_API::Vector4 NormalAxisInWorld = mRotB.Col(1);
					MATH_API::Vector4 NormalAxisInWorld = mBToWorld * Vector4(0.0f, 1.0f, 0.0f, 0.0f);
					i_ColNormal = MATH_API::Vector3(NormalAxisInWorld.x(), NormalAxisInWorld.y(), NormalAxisInWorld.z()).GetNormalized();
				}
				if (B_tOpen_Y < EndOverlap)
				{
					EndOverlap = B_tOpen_Y;
				}
			}
			else
			{
				if (!(A_BBCenterInB.y() > B_Left_Y && A_BBCenterInB.y() < B_Right_Y))
				{
					return false;
				}
			}
		}

		//B in A's Unique Normal axes
		MATH_API::Vector3 B_VelRelToA = B_Velocity - A_Velocity;
		MATH_API::Vector4 B_VelRelToAInA = mWorldToA * MATH_API::Vector4(B_VelRelToA, 0.0f);

		MATH_API::Vector4 B_BBCenterInA = mBToA * MATH_API::Vector4(B_BB.Center.x(), B_BB.Center.y(), 0.0f, 1.0f);
		MATH_API::Vector4 B_ExtentsXInA = mBToA * MATH_API::Vector4(B_BB.Extents.x(), 0.0f, 0.0f, 0.0f);
		MATH_API::Vector4 B_ExtentsYInA = mBToA * MATH_API::Vector4(0.0f, B_BB.Extents.y(), 0.0f, 0.0f);

		//Along A's X-Axis
		{
			float B_ProjectionInA_X = fabs(B_ExtentsXInA.x()) + fabs(B_ExtentsYInA.x());
			float A_Extents_X = A_BB.Extents.x() + B_ProjectionInA_X;
			float A_Left_X = A_BB.Center.x() - A_Extents_X;
			float A_Right_X = A_BB.Center.x() + A_Extents_X;
			float A_DClose_X = A_Left_X - B_BBCenterInA.x();
			float A_DOpen_X = A_Right_X - B_BBCenterInA.x();
			float A_tClose_X = 0;
			float A_tOpen_X = 0;

			if (B_VelRelToAInA.x() != 0)
			{
				A_tClose_X = A_DClose_X / B_VelRelToAInA.x();
				A_tOpen_X = A_DOpen_X / B_VelRelToAInA.x();

				if (A_tOpen_X < A_tClose_X)
				{
					float temp = A_tOpen_X;
					A_tOpen_X = A_tClose_X;
					A_tClose_X = temp;
				}
				if (A_tOpen_X < 0 || A_tClose_X > deltaTime.GetSeconds())
				{
					return false;
				}
				if (A_tClose_X > BeginOverlap)
				{
					BeginOverlap = A_tClose_X;
					//MATH_API::Vector4 NormalAxisInWorld = mRotA.Col(0);
					MATH_API::Vector4 NormalAxisInWorld = mAToWorld * Vector4(1.0f, 0.0f, 0.0f, 0.0f);
					i_ColNormal = MATH_API::Vector3(NormalAxisInWorld.x(), NormalAxisInWorld.y(), NormalAxisInWorld.z()).GetNormalized();
				}
				if (A_tOpen_X < EndOverlap)
				{
					EndOverlap = A_tOpen_X;
				}
			}
			else
			{
				if (!(B_BBCenterInA.x() > A_Left_X && B_BBCenterInA.x() < A_Right_X))
				{
					return false;
				}
			}
		}

		//Along A's Y-Axis
		{
			float B_ProjectionInA_Y = fabs(B_ExtentsXInA.y()) + fabs(B_ExtentsYInA.y());
			float A_Extents_Y = A_BB.Extents.y() + B_ProjectionInA_Y;
			float A_Left_Y = A_BB.Center.y() - A_Extents_Y;
			float A_Right_Y = A_BB.Center.y() + A_Extents_Y;
			float A_DClose_Y = A_Left_Y - B_BBCenterInA.y();
			float A_DOpen_Y = A_Right_Y - B_BBCenterInA.y();
			float A_tClose_Y = 0;
			float A_tOpen_Y = 0;

			if (B_VelRelToAInA.y() != 0)
			{
				A_tClose_Y = A_DClose_Y / B_VelRelToAInA.y();
				A_tOpen_Y = A_DOpen_Y / B_VelRelToAInA.y();

				if (A_tOpen_Y < A_tClose_Y)
				{
					float temp = A_tOpen_Y;
					A_tOpen_Y = A_tClose_Y;
					A_tClose_Y = temp;
				}
				if (A_tOpen_Y < 0 || A_tClose_Y > deltaTime.GetSeconds())
				{
					return false;
				}
				if (A_tClose_Y > BeginOverlap)
				{
					BeginOverlap = A_tClose_Y;
					//MATH_API::Vector4 NormalAxisInWorld = mRotA.Col(1);
					MATH_API::Vector4 NormalAxisInWorld = mAToWorld * Vector4(0.0f, 1.0f, 0.0f, 0.0f);
					i_ColNormal = MATH_API::Vector3(NormalAxisInWorld.x(), NormalAxisInWorld.y(), NormalAxisInWorld.z()).GetNormalized();
				}
				if (A_tOpen_Y < EndOverlap)
				{
					EndOverlap = A_tOpen_Y;
				}
			}
			else
			{
				if (!(B_BBCenterInA.y() > A_Left_Y && B_BBCenterInA.y() < A_Right_Y))
				{
					return false;
				}
			}

		}
		i_CollisionTime = BeginOverlap;
		return BeginOverlap < EndOverlap;
	}
}