#include "Game.h"
#include "EngineHeader.h"
#include "Core/Helpers/Helpers.h"
#include "Dependencies/GLib/GLib.h"
namespace Game
{
	bool Init(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
	{
		bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "2DGame", -1, 800, 600);
		if (bSuccess)
		{
			GLib::SetKeyStateChangeCallback(Engine::Input::InputCallBack);
		}
		Engine::Helpers::FloatingPtUnitTest();
		INPUT.MapKeys(KC_A, KC_D, KC_W, KC_S, 200.0f);           //Map Keys(L,R,U,D) and Setup Input Force
		return bSuccess;
	}
	void Run()
	{
		Engine::ClassLoader::LoadJsonFromFile("Data/Data.json");
		while (Engine::CloseEngine != true)
		{
			Engine::Run();
		}

	}
	void ShutDown()
	{
	}
}
