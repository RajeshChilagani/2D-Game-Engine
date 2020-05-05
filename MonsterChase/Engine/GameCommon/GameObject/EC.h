#pragma once
#include "Core/SmartPointers/WeakPtr.h"
#include <vector>
#include <array>
#include <bitset>
#include <string>
using namespace std;
class Entity;
class Component;

inline size_t getComponentTypeID()
{
	static size_t lastID = 0;
	return lastID++;
}
template<typename T>
inline size_t getComponentTypeID() noexcept
{
	static size_t typeID = getComponentTypeID();
	return typeID;
}
constexpr size_t maxComponents = 32;
using ComponentBitset = bitset<maxComponents>;
using ComponentArray = array<Component*, maxComponents>;
class Component
{
public:
	Entity *entity=nullptr;
	virtual void init() = 0;
	virtual void update()=0;
	virtual ~Component() {}
private:
	
};
class Entity
{
public:
	Entity():m_active(true)
	{}
	Entity(std::string i_Name) :m_Name(i_Name),m_active(true) {}
	std::string GetName()
	{
		return m_Name;
	}
	bool& getactive()
	{
		return m_active;
	}
	template<typename T>
	bool HasComponent() const
	{
		return componentBitset[getComponentTypeID<T>()];
	}
	template<typename T>
	void AddComponent(T *i_Component)
	{
		if (i_Component)
		{
			
			i_Component->entity = this;
			m_Components.push_back(i_Component);
			componentArray[getComponentTypeID<T>()] = i_Component;
			componentBitset[getComponentTypeID<T>()] = true;
		}
	}
	template<typename T>
	T* getComponent()
	{
		T*  ptr = static_cast<T*>(componentArray[getComponentTypeID<T>()]);
		return ptr;
	}
	void init()
	{
		for (size_t i = 0; i < m_Components.size(); ++i)
		{
			m_Components[i]->init();
		}
	}
	void update()
	{
		for (size_t i = 0; i < m_Components.size(); i++)
		{
			m_Components[i]->update();
		}
	}
	~Entity()
	{
		for (size_t i = 0; i < m_Components.size(); i++)
		{
			delete m_Components[i];
		}
	}
private:
	std::string               m_Name;
	bool                      m_active;
	std::vector<Component*>   m_Components;	
	ComponentArray            componentArray;
	ComponentBitset           componentBitset;
};
class Manager
{
public:
	Manager(const Manager& i_Other) = delete;
	Manager& operator=(const Manager& i_Other) = delete;
	static Manager& Get()
	{
		static Manager* Instance = new Manager(); 
		return *Instance;
	}
	void AddEntity(const SmartPtr<Entity>& i_Entity)
	{
		m_Entities.emplace_back(i_Entity);
	}
	void init()
	{
		for (size_t i = 0; i < m_Entities.size(); i++)
		{
			if (m_Entities[i])
				m_Entities[i].Acquire()->init();
		}
	}
	void update()
	{
		refresh();
		for (size_t i = 0; i < m_Entities.size(); i++)
		{	
			if(m_Entities[i])
				m_Entities[i].Acquire()->update();
		}
	}
	void refresh()
	{
		for (size_t i = 0; i < m_Entities.size(); i++)
		{
			if (m_Entities[i].Acquire()->getactive() == false)
			{
				//m_Entities[i].Acquire()->~Entity();
				m_Entities.erase(m_Entities.begin()+i);
				
			}
		}
	}
	static void Shutdown()
	{
		delete &Get();
	}
	~Manager()
	{
		
	}
private:
	Manager() {}
	vector<WeakPtr<Entity>>        m_Entities;
};

