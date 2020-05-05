#pragma once
#include "Engine.h"
#include "GameCommon/GameObject/EC.h"
#include "Core/Math/Vector3.h"
#include "EngineHigh/Input/Input.h"
#include "EngineHigh/Physics/PhysicsCore.h"
#include "EngineHigh/Rendering/Renderer.h"
#include "EngineLow/FileIO/LoadFile.h"
#include "Core/Structs/Structs.h"
#include "Dependencies/GLib/GLib.h"
#include <conio.h>
#include <time.h>


class Transform : public Component
{
public:
	Transform(MATH_API::Vector3 i_position=MATH_API::Vector3::zero, MATH_API::Vector3 i_Rotation = MATH_API::Vector3::zero):m_position(i_position),m_velocity(MATH_API::Vector3(0.0f, 0.0f, 0.0f)), m_rotation(i_Rotation)
	{}
	inline MATH_API::Vector3 getPosition()
	{
		return m_position;
	}
	inline MATH_API::Vector3& setPosition()
	{
		return m_position;
	}
	inline MATH_API::Vector3 getRotation()
	{
		return m_rotation;
	}
	inline MATH_API::Vector3& setRotation()
	{
		return m_rotation;
	}
	inline MATH_API::Vector3 getVelocity()
	{
		return m_velocity;
	}
	inline MATH_API::Vector3& setVelocity()
	{
		return m_velocity;
	}
	inline void init() override
	{

	}
	inline void update() override
	{
		
	}
private:
	MATH_API::Vector3          m_position;
	MATH_API::Vector3          m_velocity;
	MATH_API::Vector3          m_rotation;
};
class PhysicsBody :public Component
{
public:
	PhysicsBody()
	{}
	PhysicsBody(float i_Mass = 0.0f, float i_Drag = 0.0f,bool i_EnableGravity=false,bool i_Simulate=false,MATH_API::Vector3 i_ExternalForce=MATH_API::Vector3::zero) :m_Mass(i_Mass), m_Drag(i_Drag),m_EnableGravity(i_EnableGravity), m_Simulate(i_Simulate),m_ExternalForce(i_ExternalForce) {}
	inline void init() override
	{

	}
	inline void update() override
	{

	}
	inline float Mass() const { return m_Mass; }
	inline float Drag() const { return m_Drag; }
	inline bool EnableGravity() const { return m_EnableGravity; }
	inline bool Simulate() const { return m_Simulate; }
	inline MATH_API::Vector3& ExternalForce() { return m_ExternalForce; }
private:
	float m_Mass, m_Drag;
	bool m_EnableGravity;
	bool m_Simulate;
	MATH_API::Vector3 m_ExternalForce;
};
class PlayerController : public Component
{
public:
	PlayerController() {}
	inline void init() override
	{
	}
	inline void update() override
	{
		if (entity->getComponent<PhysicsBody>())
		{
			Engine::Input& IP = INPUT;
			PhysicsBody& PB = *entity->getComponent<PhysicsBody>();
			if (Engine::Input::IsKeyPressed(IP.GetMapppings()[0]))
				PB.ExternalForce() = MATH_API::Vector3(-(IP.GetForceX()), 0.0f, 0.0f);
			else if (Engine::Input::IsKeyPressed(INPUT.GetMapppings()[1]))
				PB.ExternalForce() = MATH_API::Vector3((IP.GetForceX()), 0.0f, 0.0f);
			else if (Engine::Input::IsKeyPressed(INPUT.GetMapppings()[2]))
				PB.ExternalForce() = MATH_API::Vector3(0.0f, IP.GetForceY(), 0.0f);
			else if (Engine::Input::IsKeyPressed(INPUT.GetMapppings()[3]))
				PB.ExternalForce() = MATH_API::Vector3(0.0f, -(IP.GetForceY()), 0.0f);
			else
				PB.ExternalForce() = MATH_API::Vector3(0.0f, 0.0f, 0.0f);
		}
	}
private:
};
class EnemyController : public Component
{
public:
	EnemyController(float i_ForceY = 0) :m_ForceY(i_ForceY) {}
	void init() override
	{
		
	}
	void update() override
	{
		if (entity && entity->getComponent<PhysicsBody>())
		{
			Engine::Input& IP = INPUT;
			PhysicsBody& PB = *entity->getComponent<PhysicsBody>();
			Transform& transform = *entity->getComponent<Transform>();
			if (transform.getPosition().y() >=-10)
			{
				m_ForceY = -50;
			}
			else if (transform.getPosition().y() <= -50)
			{
				m_ForceY = 50;
			}
			PB.ExternalForce() = MATH_API::Vector3(0.0f, m_ForceY, 0.0f);
		}
	}
private:
	float m_ForceY;
};

class SpriteRenderer : public Component
{
public:
	SpriteRenderer(const char* i_FilePath) :m_Sprite(CreateSprite(i_FilePath,m_Rect.w, m_Rect.h)), m_OffSet(GLib::Point2D()){}
	inline void init() override
	{
	}
	inline void update() override
	{
		//TRACE_INFO("Sprite Dimensions are:" << m_Rect.w << "," << m_Rect.h);
		if (m_EntityTransform)
		{
			m_OffSet.x = static_cast<float>(m_EntityTransform->getPosition().x());
			m_OffSet.y = static_cast<float>(m_EntityTransform->getPosition().y());
			m_Rect.x = m_OffSet.x;
			m_Rect.y = m_OffSet.y;
			m_RotationInZ = m_EntityTransform->getRotation().z();
		}
		else
		{
			if (entity->HasComponent<Transform>())
			{
				m_EntityTransform = entity->getComponent<Transform>();
				m_OffSet.x = static_cast<float>(m_EntityTransform->getPosition().x());
				m_OffSet.y = static_cast<float>(m_EntityTransform->getPosition().y());
				m_Rect.x = m_OffSet.x;
				m_Rect.y = m_OffSet.y;
				m_RotationInZ = m_EntityTransform->getRotation().z();
			}
			
		}
				
	}
	inline GLib::Point2D Offset()
	{
		return m_OffSet;
	}
	inline GLib::Sprite* SpriteObject ()
	{
		return m_Sprite;
	}
	inline Engine::Rect& GetRect()
	{
		return m_Rect;
	}
	inline float GetZRotation()
	{
		return m_RotationInZ;
	}
	~SpriteRenderer()
	{
		GLib::Release(m_Sprite);
	}
private:
	GLib::Sprite*           m_Sprite;
	GLib::Point2D           m_OffSet;
	float                   m_RotationInZ=0;
	Engine::Rect            m_Rect;
	Transform*              m_EntityTransform;
};
class ColliderComponent : public Component
{
public:
	ColliderComponent()
	{
		
	}
	inline void init() override
	{	
		
	}
	Engine::AABB& GetBB()
	{
		return m_BoundingBox;
	}
	inline void update() override
	{
		if (entity->HasComponent<SpriteRenderer>())
		{
			Math::Point2D center(0.0f	, static_cast<float>(entity->getComponent<SpriteRenderer>()->GetRect().h / 2));
			Math::Point2D extents(static_cast<float>(entity->getComponent<SpriteRenderer>()->GetRect().w / 2), static_cast<float>(entity->getComponent<SpriteRenderer>()->GetRect().h / 2));
			m_BoundingBox.Center = center;
			m_BoundingBox.Extents = extents;
		}
	}
	~ColliderComponent() {}
private:
	Engine::AABB       m_BoundingBox;
	
};
