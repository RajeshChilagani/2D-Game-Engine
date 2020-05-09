#include "Game.h"
#include "EngineHeader.h"
#include "Dependencies/GLib/GLib.h"
namespace Game
{
	bool isGameOver()
	{
		SmartPtr<Entity> Pong = Engine::GlobalSystem::GetEntityAt(2);
		if (Pong)
		{
			float xPos = Pong->getComponent<Transform>()->getPosition().x();
			float yPos = Pong->getComponent<Transform>()->getPosition().y();
			if (xPos > 400 || yPos > 300)
			{
				TRACE_INFO("AI Won");
				return true;
			}
			else if (xPos < -400 || yPos < -300)
			{
				TRACE_INFO("Player Won");
				return true;
			}
			
		}
		return false;
	}
	bool Init(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
	{
		bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "GLibTest", -1, 800, 600);
		if (bSuccess)
		{
			GLib::SetKeyStateChangeCallback(Engine::Input::InputCallBack);
		}
		Engine::Helpers::FloatingPtUnitTest();
		INPUT.MapKeys(KC_A, KC_D, KC_W, KC_S, 500.0f,500.f);           //Map Keys(Left,Right,Up,Down) and Setup Input Force for X(Left/Right) & Y(Up/Down) Directions
		return bSuccess;
	}
	void Run()
	{
		Engine::ClassLoader::LoadJsonFromFile("Data/Data.json");
		while (Engine::CloseEngine != true)
		{
			Engine::Run();	
			if (isGameOver())
				break;
		}

	}
	void ShutDown()
	{
	}
}
