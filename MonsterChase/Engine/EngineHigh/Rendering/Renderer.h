#pragma once
#include "GameCommon/GameObject/EC.h"
#include "Core/SmartPointers/WeakPtr.h"
#include "EngineLow/Timer.h"
#define RENDERER Engine::Renderer::Get()
namespace Engine
{
	class Renderer
	{
	public:
		Renderer(const Renderer& i_Other) = delete;
		Renderer& operator=(const Renderer& i_Other) = delete;
		inline static Renderer& Get()
		{
			static Renderer* Instance = new Renderer();
			return *Instance;
		}
		static bool Init();
		static void AddRendererObject(SmartPtr<Entity> i_Renderable);
		static void Run(Timer delta);
		static void ShutDown();
		void UpdateRenderables();
		void Update(float deltaTime);
	private:
		Renderer() {}
		std::vector<WeakPtr<Entity>>   m_Renderables;
		std::vector<WeakPtr<Entity>>   m_NewRenderables;
	};
}
