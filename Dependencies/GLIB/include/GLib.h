#pragma once

#include <Windows.h>

#include <functional>

#include "BasicTypes.h"

namespace GLib
{
	bool Initialize( HINSTANCE i_hInstance, int i_CmdShow, const char * i_pWindowName, WORD i_IconID, unsigned int i_WindowWidth, unsigned int i_WindowHeight, bool i_bDebugDevice = false );
	void Shutdown();

	void Service( bool & o_bQuitRequested );

	void SetKeyStateChangeCallback(std::function<void(unsigned int i_VKeyID, bool i_bDown)> i_KeyStateChangeCallback);

	bool BeginRendering();
	void EndRendering();

	struct Texture;
	
	Texture * CreateTexture( void * i_pTextureData, size_t i_sizeTextureData );
	void Release( Texture * i_pTexture );

	bool GetDimensions( Texture * i_pTexture, unsigned int & o_Width, unsigned int & o_Height, unsigned int & o_Depth );

	struct Sprite;

	namespace Sprites
	{
		bool Initialize();
		void Shutdown();

		bool BeginRendering();
		void EndRendering();

		Sprite * CreateSprite( const SpriteEdges & i_VertexOffsets, float i_Depth, const RGBA & i_Color, const SpriteUVs & i_UVs );
		void SetTexture( Sprite & i_Sprite, Texture & i_Texture );
	
		bool RenderSprite( const Sprite & i_Sprite, const Point2D & i_Offset, float i_zRotRadians );

		void Release( Sprite * i_pSprite );
	} // namespace Sprites
} // namespace GLib