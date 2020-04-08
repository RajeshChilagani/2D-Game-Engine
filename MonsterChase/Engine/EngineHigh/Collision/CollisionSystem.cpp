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
	void Collision::Update(Timer deltaTime)
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
									if (CheckAABB(ICollisionObject, JCollisionObject,deltaTime))
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
	bool Collision::CheckAABB(SmartPtr<Entity>& i_A, SmartPtr<Entity>& i_B, Timer deltaTime)
	{
		/*if (i_rectA.x + (i_rectA.w) >= i_rectB.x && i_rectB.x + (i_rectB.w) >= i_rectA.x
			&& i_rectA.y + (i_rectA.h) >= i_rectB.y && i_rectB.y + (i_rectB.h) >= i_rectA.y)
		{
			return true;
		}*/
		
		//Acquire required values//
		MATH_API::Vector3 A_Velocity = i_A->getComponent<Transform>()->getVelocity();
		MATH_API::Vector3 B_Velocity = i_B->getComponent<Transform>()->getVelocity();
		AABB& A_BB = i_A->getComponent<ColliderComponent>()->GetBB();
		AABB& B_BB = i_B->getComponent<ColliderComponent>()->GetBB();

		std::vector<float>tOpens;
		std::vector<float>tCloses;

		bool isSeperated = true;

		//Create Matrices//
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


//A in B's Unique Normal axes//
		MATH_API::Vector3 A_VelRelToB = A_Velocity - B_Velocity;
		MATH_API::Vector4 A_VelRelToBInB = mWorldToB * MATH_API::Vector4(A_VelRelToB, 0.0f);

		MATH_API::Vector4 A_BBCenterInB = mAToB * MATH_API::Vector4(A_BB.Center.x(), A_BB.Center.y(),0.0f,1.0f);
		MATH_API::Vector4 A_ExtentsXInB = mAToB * MATH_API::Vector4(A_BB.Extents.x(),0.0f,0.0f,0.0f);
		MATH_API::Vector4 A_ExtentsYInB = mAToB * MATH_API::Vector4(0.0f, A_BB.Extents.y(), 0.0f, 0.0f);
		
		//Along B's X-Axis
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
			tOpens.push_back(B_tOpen_X);
			tCloses.push_back(B_tClose_X);
		}
		else
		{
			if (!(A_BBCenterInB.x() > B_Left_X && A_BBCenterInB.x() < B_Right_X))
			{
				return false;
			}
		}
		////Edge Cases
		
		isSeperated = fabs(A_BBCenterInB.x() - B_BB.Center.x()) > B_BB.Extents.x() + A_ProjectionInB_X;

		//Along B's Y-Axis
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
			tOpens.push_back(B_tOpen_Y);
			tCloses.push_back(B_tClose_Y);
		}
		else
		{
			if (!(A_BBCenterInB.y() > B_Left_Y && A_BBCenterInB.y() < B_Right_Y))
			{
				return false;
			}
		}
		

		if(!isSeperated)
			isSeperated = fabs(A_BBCenterInB.y() - B_BB.Center.y()) > B_BB.Extents.y() + A_ProjectionInB_Y;
		
//B in A's Unique Normal axes
		MATH_API::Vector3 B_VelRelToA = B_Velocity - A_Velocity;
		MATH_API::Vector4 B_VelRelToAInA = mWorldToA * MATH_API::Vector4(B_VelRelToA, 0.0f);

		MATH_API::Vector4 B_BBCenterInA = mBToA * MATH_API::Vector4(B_BB.Center.x(), B_BB.Center.y(), 0.0f, 1.0f);
		MATH_API::Vector4 B_ExtentsXInA = mBToA * MATH_API::Vector4(B_BB.Extents.x(), 0.0f, 0.0f, 0.0f);
		MATH_API::Vector4 B_ExtentsYInA = mBToA * MATH_API::Vector4(0.0f, B_BB.Extents.y(), 0.0f, 0.0f);

		//Along A's X-Axis
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
			tOpens.push_back(A_tOpen_X);
			tCloses.push_back(A_tClose_X);
		}
		else
		{
			if (!(B_BBCenterInA.x() > A_Left_X && B_BBCenterInA.x() < A_Right_X))
			{
				return false;
			}
		}
		

		if (!isSeperated)
			isSeperated = fabs(B_BBCenterInA.x() - A_BB.Center.x()) > A_BB.Extents.x() + B_ProjectionInA_X;
		
		//Along B's Y-Axis
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
			tOpens.push_back(A_tOpen_Y);
			tCloses.push_back(A_tClose_Y);
		}
		else
		{
			if (!(B_BBCenterInA.y() > A_Left_Y && B_BBCenterInA.y() < A_Right_Y))
			{
				return false;
			}
		}

		if (!isSeperated)
			isSeperated = fabs(B_BBCenterInA.y() - A_BB.Center.y()) > A_BB.Extents.y() + B_ProjectionInA_Y;
		
		if (tCloses.size() > 0 && tOpens.size()>0)
		{
			float latestTClose = *(min_element(tCloses.begin(), tCloses.end()));
			float earliestTOpen = *(max_element(tOpens.begin(), tOpens.end()));
			if (!(latestTClose < earliestTOpen))
			{
				return false;
			}
		}
		//return isSeperated;
		return true;
	}
}