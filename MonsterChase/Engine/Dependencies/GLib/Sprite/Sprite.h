#pragma once

#include "BasicTypes.h"
#include "Helpers/IReferenceCounted.h"
#include "Texture/Texture.h"

namespace GLib
{
	class Sprite : public IReferenceCounted
	{
	public:
		Sprite(unsigned int i_VBStart, Texture* i_pTexture);

		void SetTexture(Texture* i_pTexture);
		void Render(const Point2D& i_Offset, float i_zRotRadians) const;
	private:
		~Sprite();

		unsigned int	m_VBStart;
		Texture* m_pTexture;
	};
} // namespace GLib