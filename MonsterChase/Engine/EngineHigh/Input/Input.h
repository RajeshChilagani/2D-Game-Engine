#pragma once
#include "KeyCodes.h"
#define INPUT Engine::Input::Get()
namespace Engine
{
	class Input
	{
		public:
			static void CheckForInput();
			static void Init();
			static bool IsKeyPressed(int KeyCode);
			static bool IsKeyReleased(int KeyCode);
			static void InputCallBack(unsigned int i_VKeyID, bool bWentDown);
			static void ShutDown();
			static inline Input& Get() 
			{
				return *s_Instance;
			}
			void MapKeys(int i_Left, int i_Right, int i_Up, int i_Down, float i_ForceX, float i_ForceY);
			inline int* GetMapppings() 
			{
				return m_Mappings;
			}
			inline float GetForceX() const
			{
				return m_X;
			}
			inline float GetForceY() const
			{
				return m_Y;
			}
			inline bool GetIsMappingDone() const
			{
				return m_isMappingDone;
			}
		private:
			Input()
			{}
			int           m_Key;
			bool          m_KeyState;
			int           m_Mappings[4];
			bool          m_isMappingDone;
			float         m_X;
			float         m_Y;
			static Input* s_Instance;
	};
}
