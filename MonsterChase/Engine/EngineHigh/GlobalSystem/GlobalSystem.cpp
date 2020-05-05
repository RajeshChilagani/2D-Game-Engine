#include "EngineHigh/GlobalSystem/GlobalSystem.h"
#include "GameCommon/GameObject/Components.h"
#include "EngineHigh/Physics/PhysicsCore.h"
#include "EngineHigh/Rendering/Renderer.h"
#include "EngineHigh/Collision/CollisionSystem.h"
#include "Core/JobSystem/Syncronization/Mutex.h"
#include "Core/JobSystem/Syncronization/ScopeLock.h"
#include <type_traits>
#include <typeinfo>
#include <string.h>
namespace Engine
{
	namespace GlobalSystem
	{
		std::vector<SmartPtr<Entity>> AllEntities;
		std::vector<SmartPtr<Entity>> NewEntities;
		Mutex NewEntitiesMutex;
		SmartPtr<Entity> GetEntityAt(size_t i_Index)
		{
			//assert(!(i_Index >= AllEntities.size()));
			if (i_Index < AllEntities.size())
			{
				return AllEntities[i_Index];
			}
			return SmartPtr<Entity>(nullptr);
		}
		void CheckForNewEntities()
		{
			ScopeLock Lock(NewEntitiesMutex);
			for (auto newEntity : NewEntities)
			{
				AllEntities.push_back(newEntity);
				Manager::Get().AddEntity(newEntity);
			}
			NewEntities.clear();

		}
		template<typename T, typename...Targs>
		void AddComponent(SmartPtr<Entity>& i_Entity, Targs&&...mArgs)
		{
			assert(i_Entity);
			if (i_Entity)
			{
				T* ComponentToAttach = new T(std::forward<Targs>(mArgs)...);
				bool hasComponent = i_Entity->HasComponent<T>();
				if (hasComponent==false)
				{
					i_Entity->AddComponent<T>(ComponentToAttach);
					if (std::is_same<T, SpriteRenderer>::value)
					{
						Renderer::AddRendererObject(i_Entity);
					}
					else if (std::is_same<T, PhysicsBody>::value)
					{
						Physics::AddPhysicsObject(i_Entity);
					}
					else if (std::is_same<T, PlayerController>::value)
					{
						if (!INPUT.GetIsMappingDone())
						{
							TRACE_ERROR("Missing Key Mapping Please Provide Key Mapping");
						}
						assert(INPUT.GetIsMappingDone());

					}
					else if (std::is_same<T, ColliderComponent>::value)
					{
						Collision::AddCollisionObject(i_Entity);
					}
				}
				else
				{
					TRACE_ERROR("" << typeid(T).name() << "Component already exists Cannot add duplicate");
					delete ComponentToAttach;
				}

			}
			else
			{
				TRACE_ERROR(" Invalid Object" << " To add " << typeid(T).name() << " component");
			}

		}

		void AddEntityToWorld(std::string i_EntityName, Json EntityData)
		{
			ScopeLock Lock(NewEntitiesMutex);
			NewEntities.emplace_back(new Entity(i_EntityName));
			SmartPtr<Entity>& entity = NewEntities.back();
			for (unsigned int comps = 0; comps < EntityData["Components"].size(); ++comps)
			{
				Json component = EntityData["Components"][comps];
				std::string componentName = component["Name"];
				if (!componentName.compare("Transform"))
				{
					float positionX = component["Position"]["x"];
					float positionY = component["Position"]["y"];
					float rotationZ = component["Rotation"]["z"];
					AddComponent<Transform>(entity, MATH_API::Vector3(positionX, positionY, 0.0f),MATH_API::Vector3(0.0f,0.0f,rotationZ));
				}
				else if (!componentName.compare("PhysicsBody"))
				{
					float mass = component["mass"];
					float drag = component["drag"];
					bool enableGravity = component["EnableGravity"];
					bool simulate = component["Simulate"];
					MATH_API::Vector3 externalForce(static_cast<float>(component["ExternalForce"]["x"]), static_cast<float>(component["ExternalForce"]["y"]), static_cast<float>(component["ExternalForce"]["z"]));
					Engine::GlobalSystem::AddComponent<PhysicsBody>(entity, mass, drag, enableGravity,simulate,externalForce);
				}
				else if (!componentName.compare("SpriteRenderer"))
				{
					std::string spriteFile = component["FileLocaiton"];
					Engine::GlobalSystem::AddComponent<SpriteRenderer>(entity, spriteFile.data());
				}
				else if (!componentName.compare("PlayerController"))
				{
					Engine::GlobalSystem::AddComponent<PlayerController>(entity);
				}
				else if (!componentName.compare("EnemyController"))
				{
					float forceY = component["ForceY"];
					Engine::GlobalSystem::AddComponent<EnemyController>(entity,forceY);
				}
				else if (!componentName.compare("ColliderComponent"))
				{
					Engine::GlobalSystem::AddComponent<ColliderComponent>(entity);
				}
			}

		}
		void Shutdown()
		{
			if (AllEntities.size() > 0)
			{
				AllEntities.clear();
			}
		}

	}
	

}