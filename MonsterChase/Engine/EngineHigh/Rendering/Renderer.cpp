#include "Renderer.h"
#include "Core/JobSystem/Syncronization/Mutex.h"
#include "Core/JobSystem/Syncronization/ScopeLock.h"
#include "GameCommon/GameObject/Components.h"
#include "Dependencies/GLib/GLib.h"
#include "Core/Math/MathUtilitiesh.h"
#include <assert.h>
namespace Engine
{
	Mutex NewRenderableMutex;
	bool Renderer::Init()
	{
		return true;
	}
	void Renderer::Update(float deltaTime)
	{
		GLib::BeginRendering();
		GLib::Sprites::BeginRendering();
		for (size_t i = 0; i <m_Renderables.size(); ++i)
		{
			if (m_Renderables[i])
			{
				SmartPtr<Entity> RenderObject = m_Renderables[i].Acquire();
				if (RenderObject->getactive())
				{
					SpriteRenderer& RenderCommponent = *(RenderObject->getComponent<SpriteRenderer>());
					if (RenderCommponent.SpriteObject())
					{
						GLib::Render(*(RenderCommponent.SpriteObject()), RenderCommponent.Offset(), Math::DegreeToRadians(RenderCommponent.GetZRotation()));
					}					
				}
			}
			
		}
		GLib::Sprites::EndRendering();
		GLib::EndRendering();
	}
	void Renderer::UpdateRenderables()
	{
		ScopeLock Lock(NewRenderableMutex);
		for (auto newRenderable : m_NewRenderables)
		{
			m_Renderables.push_back(newRenderable);
		}
		m_NewRenderables.clear();
	}
	void Renderer::Run(Timer delta)
	{
		RENDERER.UpdateRenderables();
		RENDERER.Update(delta.GetSeconds());
	}
	void Renderer::ShutDown()
	{
		delete &Get();
	}
	void Renderer::AddRendererObject(const SmartPtr<Entity>& i_Renderable)
	{
		ScopeLock Lock(NewRenderableMutex);
		RENDERER.m_NewRenderables.emplace_back(i_Renderable);
	}
}