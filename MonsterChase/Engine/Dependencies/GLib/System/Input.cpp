#include "Input.h"

#include <assert.h>
#include <stdint.h>

#include <Windows.h>

#include <functional>

#include "BasicTypes.h"
#include "Console\ConsolePrint.h"

namespace GLib
{
	static std::function<void(unsigned int i_VKeyID, bool i_bDown)> s_KeyStateChangeCallback;

	void SetKeyStateChangeCallback(std::function<void(unsigned int i_VKeyID, bool i_bDown)> i_KeyStateChangeCallback)
	{
		s_KeyStateChangeCallback = i_KeyStateChangeCallback;
	}

	namespace Input
	{
		const size_t	m_bytesInitReadBuffer = 128;

		size_t			m_bytesReadBuffer = 0;
		LPBYTE			m_pReadBuffer = nullptr;

		const size_t	m_sizeReadTable = 256;
		uint8_t			m_VKeyStates[m_sizeReadTable];

		void ResizeReadBuffer(size_t i_bytes)
		{
			assert(i_bytes);

			if (i_bytes > m_bytesReadBuffer)
			{
				if (m_pReadBuffer)
					delete[] m_pReadBuffer;

				m_pReadBuffer = new BYTE[i_bytes];
				assert(m_pReadBuffer);

				m_bytesReadBuffer = i_bytes;
			}
		}

		bool Initialize()
		{
			assert(m_pReadBuffer == nullptr);

			ResizeReadBuffer(m_bytesInitReadBuffer);

			RAWINPUTDEVICE keyboard;

			keyboard.usUsagePage = 0x01;		// Generic desktop page
			keyboard.usUsage = 0x06;			// Keyboard
			keyboard.dwFlags = RIDEV_NOLEGACY;	// Turn off keyboard input to the Windows message handlers
			keyboard.hwndTarget = NULL;

			BOOL success = RegisterRawInputDevices(&keyboard, 1, sizeof(keyboard));
			assert(success == TRUE);

			memset(m_VKeyStates, sizeof(m_VKeyStates), 0);

			return true;
		}

		void Service(HRAWINPUT i_Input)
		{
			UINT bytesData = 0;
			UINT ret = GetRawInputData(i_Input, RID_INPUT, NULL, &bytesData, sizeof(RAWINPUTHEADER));
			assert(ret == 0);

			if (bytesData)
			{
				ResizeReadBuffer(bytesData);
				assert(m_pReadBuffer);

				ret = GetRawInputData(i_Input, RID_INPUT, m_pReadBuffer, &bytesData, sizeof(RAWINPUTHEADER));
				assert(ret == bytesData);

				RAWINPUT* pRawInput = reinterpret_cast<RAWINPUT*>(m_pReadBuffer);

				if (pRawInput->header.dwType == RIM_TYPEKEYBOARD)
				{
					if (pRawInput->data.keyboard.Flags == RI_KEY_BREAK)
					{ // up
						bool bChanged = m_VKeyStates[pRawInput->data.keyboard.VKey] != 0;

						m_VKeyStates[pRawInput->data.keyboard.VKey] = 0;

						if (bChanged)
						{
							if (s_KeyStateChangeCallback)
								s_KeyStateChangeCallback(pRawInput->data.keyboard.VKey, false);

							DEBUG_PRINT("Key 0x%02x changed state to UP\n", pRawInput->data.keyboard.VKey);
						}
					}
					else if (pRawInput->data.keyboard.Flags == RI_KEY_MAKE)
					{ // down
						bool bChanged = m_VKeyStates[pRawInput->data.keyboard.VKey] != 1;

						m_VKeyStates[pRawInput->data.keyboard.VKey] = 1;

						if (bChanged)
						{
							if (s_KeyStateChangeCallback)
								s_KeyStateChangeCallback(pRawInput->data.keyboard.VKey, true);

							DEBUG_PRINT("Key 0x%02x changed state to DOWN\n", pRawInput->data.keyboard.VKey);
						}
					}
					else
					{
						DEBUG_PRINT("Key 0x%02x Flags: 0x%04x\n", pRawInput->data.keyboard.VKey, pRawInput->data.keyboard.Flags);
					}
				}
			}

		}

		void Shutdown()
		{
			if (m_pReadBuffer)
				delete[] m_pReadBuffer;
		}
	} // namespace Input
} // namespace GLib