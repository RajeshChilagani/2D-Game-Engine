#include <Windows.h>

#include "Window.h"
#include "Input.h"

#include "Console\ConsolePrint.h"

namespace GLib
{
	void Service( bool & o_bQuitRequested )
	{
		o_bQuitRequested = false;

		BOOL				bGotMsg = FALSE;

		do
		{
			MSG					msg;
			
			msg.message = WM_NULL;
			
			bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );

			if( bGotMsg )
			{
				if( TranslateAccelerator( g_hWnd, NULL, &msg ) == 0 )
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}

				switch( msg.message )
				{
				case WM_QUIT:
				{
					o_bQuitRequested = true;
					break;
				}
				case WM_INPUT:
				{
					Input::Service( HRAWINPUT( msg.lParam ) );
					break;
				}
#ifdef __SHOW_MESSAGES
				default:
					DEBUG_PRINT( "message: 0x%04x\n", msg.message );
#endif // __SHOW_MESSAGES
				}
			}
		} while( ( bGotMsg == TRUE ) && ( o_bQuitRequested == false ) );

	}
} // namespace GLib