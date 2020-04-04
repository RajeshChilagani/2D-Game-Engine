#include "Input.h"
namespace Engine
{
	Input* Input::s_Instance = nullptr;
	void Input::CheckForInput()
	{

	}
	void Input::Init()
	{
		s_Instance = new Input();
		s_Instance->MapKeys(KC_A, KC_D, KC_W, KC_S, 100.0f);         //Default Mapping for keys
	}
	void Input::InputCallBack(unsigned int i_VKeyID, bool bWentDown)
	{
		s_Instance->m_Key = i_VKeyID;
		s_Instance->m_KeyState = bWentDown;
	}
	bool Input::IsKeyPressed(int KeyCode)
	{
		if (KeyCode == s_Instance->m_Key && s_Instance->m_KeyState==true)
			return true;
		return false;
	}
	bool Input::IsKeyReleased(int KeyCode)
	{
		if (KeyCode == s_Instance->m_Key && s_Instance->m_KeyState == false)
			return true;
		return false;
	}
	void Input::MapKeys(int i_Left, int i_Right, int i_Up, int i_Down, float force)
	{
		s_Mappings[0] = i_Left;
		s_Mappings[1] = i_Right;
		s_Mappings[2] = i_Up;
		s_Mappings[3] = i_Down;
		s_Force = force;
		s_isMappingDone = true;
	}
	void Input::ShutDown()
	{
		delete s_Instance;
	}
}