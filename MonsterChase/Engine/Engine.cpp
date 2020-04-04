#include "Engine.h"
#include "Core/Core.h"
#include "EngineHigh/GlobalSystem/GlobalSystem.h"
#include "EngineHigh/Physics/PhysicsCore.h"
#include "EngineHigh/Rendering/Renderer.h"
#include "EngineHigh/Collision/CollisionSystem.h"
#include "EngineHigh/Input/Input.h"
#include "GameCommon/GameObject/EC.h"
#include "EngineHigh/ClassLoader/ClassLoader.h"

bool  Engine::CloseEngine = false;
Timer Engine::deltaTime;

bool Engine::Init()
{
	cout << "Engine has started\n";
	Input::Init();                                                                //Initilize Input Class
	Physics::Init();                                                              //Initilize Physics
	Renderer::Init();                                                             //Init Renderer
	Collision::Init();
	return true;	

}
void Engine::Run()
{
	deltaTime = Timer::CalculateFrameTime();
	GlobalSystem::CheckForNewEntities();
	Input::CheckForInput();
	if (Input::IsKeyReleased(KC_Q) || Input::IsKeyReleased(KC_ESC))
		CloseEngine = true;
	//TRACE_INFO("DeltaTime in Seconds"<<deltaTime.GetSeconds());
	Manager::Get().update();
	Collision::Run(deltaTime);
	Physics::Run(deltaTime);
	bool bQuit = false;
	GLib::Service(bQuit);
	Renderer::Run(deltaTime);
}
void Engine::ShutDown()
{                                                  // release all object memory
	Collision::ShutDown();
	Physics::ShutDown();
	Renderer::ShutDown();
	Manager::Get().Shutdown();
	Input::ShutDown();
	GlobalSystem::Shutdown();
	JobSystem::RequestShutdown();
	GLib::Shutdown();
	cout << "Bye Bye \n";
}




