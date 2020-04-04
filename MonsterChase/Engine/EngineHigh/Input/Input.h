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
			void MapKeys(int i_Left, int i_Right, int i_Up, int i_Down, float force);
			static inline Input& Get()
			{
				return *s_Instance;
			}
			inline int* GetMapppings()
			{
				return s_Mappings;
			}
			inline float GetForce()
			{
				return s_Force;
			}
			inline bool GetIsMappingDone()
			{
				return s_isMappingDone;
			}
		private:
			Input()
			{}
			int           m_Key;
			bool          m_KeyState;
			int           s_Mappings[4];
			bool          s_isMappingDone;
			float         s_Force;
			static Input* s_Instance;
	};
}
