#include "Sprite.h"

#include <assert.h>
#include <stdint.h>

#include <Windows.h>

#include <d3dtypes.h>
#include <DirectXMath.h>

#include "BasicTypes.h"

#include "Buffers/DrawResourcesBuffer.h"
#include "Console/ConsolePrint.h"
#include "Helpers/IReferenceCounted.h"
#include "System/Device.h"
#include "System/Helpers.h"
#include "Texture/Texture.h"

#include "SpriteVS.h"
#include "SpritePS.h"

namespace GLib
{
	struct SpriteVSConstantBuffer
	{
		DirectX::XMMATRIX	m_ModelViewProjection;
	};

	struct SpriteVertex
	{
		float			Position[3];
		D3DCOLOR		Color;
		float			UV[2];
	};

	static CRITICAL_SECTION CreateSpriteCS;

	static const size_t		s_sizeSpriteVB = 256 * 1024;
	static const size_t		s_sizeSpriteIB = 128 * 1024;

	ID3D11VertexShader* m_pSpriteVS = nullptr;
	ID3D11PixelShader* m_pSpritePS = nullptr;
	ID3D11Buffer* m_pSpriteCB = nullptr;

	ID3D11InputLayout* m_pSpriteVL = nullptr;
	DrawResourcesBuffer* m_pSpriteVB = nullptr;
	DrawResourcesBuffer* m_pSpriteIB = nullptr;

	ID3D11SamplerState* m_pSpriteSampler = nullptr;

	ID3D11BlendState* m_pAlphaBlendOn = nullptr;
	ID3D11BlendState* m_pAlphaBlendOff = nullptr;

	SpriteVSConstantBuffer	m_SpriteVSContants;

	unsigned int			m_strideVB = 0;
	unsigned int			m_offsetVB = 0;

	bool					m_bRenderingEnabled = false;

	DirectX::XMMATRIX		m_Projection;

	Sprite::Sprite(unsigned int i_VBStart, Texture* i_pTexture) :
		IReferenceCounted(1),
		m_VBStart(i_VBStart),
		m_pTexture(i_pTexture)
	{
		if (m_pTexture)
			m_pTexture->AddRef();
	}

	void Sprite::SetTexture(Texture* i_pTexture)
	{
		if (m_pTexture)
			m_pTexture->ReleaseRef();

		m_pTexture = i_pTexture;

		if (m_pTexture)
			m_pTexture->AddRef();
	}

	void Sprite::Render(const Point2D& i_Offset, float i_zRotRadians) const
	{
		SafeDeviceInterface<ID3D11DeviceContext> ImmediateContext = GetImmediateContext();

		if (m_pAlphaBlendOn)
			ImmediateContext->OMSetBlendState(m_pAlphaBlendOn, nullptr, 0xffffffff);

		DirectX::XMMATRIX mModel = DirectX::XMMatrixRotationZ(i_zRotRadians);
		DirectX::XMMATRIX mView = DirectX::XMMatrixTranslation(i_Offset.x, i_Offset.y, 0.0f);

		DirectX::XMMATRIX mModelView = DirectX::XMMatrixMultiply(mModel, mView);

		m_SpriteVSContants.m_ModelViewProjection = DirectX::XMMatrixMultiplyTranspose(mModelView, m_Projection);

		ImmediateContext->UpdateSubresource(m_pSpriteCB, 0, nullptr, &m_SpriteVSContants, 0, 0);
		ImmediateContext->VSSetConstantBuffers(0, 1, &m_pSpriteCB);
		ImmediateContext->PSSetShaderResources(0, 1, &m_pTexture->m_pShaderResource);

		ImmediateContext->Draw(4, m_VBStart);
	}

	Sprite::~Sprite()
	{
		if (m_pTexture)
			m_pTexture->ReleaseRef();

	}

	Sprite* CreateSprite(const SpriteEdges& i_VertexOffsets, float i_Depth, const RGBA& i_Color, const SpriteUVs& i_UVs, Texture* i_pTexture)
	{
		assert(m_pSpriteVB);
		assert(m_pSpriteIB);

		EnterCriticalSection(&CreateSpriteCS);

		SpriteVertex	SpriteVertices[4];

		// Upper Left
		SpriteVertices[0].Position[0] = i_VertexOffsets.Left;
		SpriteVertices[0].Position[1] = i_VertexOffsets.Top;
		SpriteVertices[0].Position[2] = i_Depth;

		SpriteVertices[0].Color = RGBA_MAKE(i_Color.r, i_Color.g, i_Color.b, i_Color.a);

		SpriteVertices[0].UV[0] = i_UVs.TopLeft.u;
		SpriteVertices[0].UV[1] = i_UVs.TopLeft.v;

		// Upper Right
		SpriteVertices[1].Position[0] = i_VertexOffsets.Right;
		SpriteVertices[1].Position[1] = i_VertexOffsets.Top;
		SpriteVertices[1].Position[2] = i_Depth;

		SpriteVertices[1].Color = RGBA_MAKE(i_Color.r, i_Color.g, i_Color.b, i_Color.a);

		SpriteVertices[1].UV[0] = i_UVs.TopRight.u;
		SpriteVertices[1].UV[1] = i_UVs.TopRight.v;

		// Lower Left
		SpriteVertices[2].Position[0] = i_VertexOffsets.Left;
		SpriteVertices[2].Position[1] = i_VertexOffsets.Bottom;
		SpriteVertices[2].Position[2] = i_Depth;

		SpriteVertices[2].Color = RGBA_MAKE(i_Color.r, i_Color.g, i_Color.b, i_Color.a);

		SpriteVertices[2].UV[0] = i_UVs.BottomLeft.u;
		SpriteVertices[2].UV[1] = i_UVs.BottomLeft.v;

		// Lower Right
		SpriteVertices[3].Position[0] = i_VertexOffsets.Right;
		SpriteVertices[3].Position[1] = i_VertexOffsets.Bottom;
		SpriteVertices[3].Position[2] = i_Depth;

		SpriteVertices[3].Color = RGBA_MAKE(i_Color.r, i_Color.g, i_Color.b, i_Color.a);

		SpriteVertices[3].UV[0] = i_UVs.BottomRight.u;
		SpriteVertices[3].UV[1] = i_UVs.BottomRight.v;

		unsigned int indexVB;

		bool result = m_pSpriteVB->AddNewData(&SpriteVertices, sizeof(SpriteVertices[0]), sizeof(SpriteVertices), indexVB);
		assert(result);

		Sprite* pNewSprite = new Sprite(indexVB, i_pTexture);

		LeaveCriticalSection(&CreateSpriteCS);
		return pNewSprite;
	}

	bool Render(const Sprite& i_Sprite, const Point2D& i_Offset, float i_zRotRadians)
	{
		if (m_bRenderingEnabled == false)
		{
			DEBUG_PRINT("Attempting to render sprites outside BeginRendering() / EndRendering() block.\n Call BeginRendering() first.");
			return false;
		}

		i_Sprite.Render(i_Offset, i_zRotRadians);
		return true;
	}

	void SetTexture(Sprite& i_Sprite, Texture* i_pTexture)
	{
		i_Sprite.SetTexture(i_pTexture);
	}

	void Release(Sprite* i_pSprite)
	{
		if (i_pSprite)
			i_pSprite->ReleaseRef();
	}

	namespace Sprites
	{
		bool Initialize()
		{
			assert(g_pD3DDevice);

			HRESULT hResult = g_pD3DDevice->CreateVertexShader(SpriteVS, sizeof(SpriteVS), NULL, &m_pSpriteVS);
			assert(hResult == S_OK);

			hResult = g_pD3DDevice->CreatePixelShader(SpritePS, sizeof(SpritePS), NULL, &m_pSpritePS);
			assert(hResult == S_OK);

			m_pSpriteVB = DrawResourcesBuffer::Create(IGPUResourceBuffer::eUsage::USAGE_VERTICES, s_sizeSpriteVB, NULL, 0);
			assert(m_pSpriteVB);

			m_pSpriteIB = DrawResourcesBuffer::Create(IGPUResourceBuffer::eUsage::USAGE_INDICES, s_sizeSpriteIB, NULL, 0);
			assert(m_pSpriteIB);

			D3D11_INPUT_ELEMENT_DESC SpriteVertexDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			hResult = g_pD3DDevice->CreateInputLayout(SpriteVertexDesc, ARRAYSIZE(SpriteVertexDesc), SpriteVS, sizeof(SpriteVS), &m_pSpriteVL);
			assert(hResult == S_OK);

			m_strideVB = sizeof(SpriteVertex);
			m_offsetVB = 0;

			D3D11_BUFFER_DESC cbd = { 0 };

			cbd.ByteWidth = sizeof(m_SpriteVSContants);
			cbd.Usage = D3D11_USAGE_DEFAULT;
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbd.CPUAccessFlags = 0;
			cbd.MiscFlags = 0;
			cbd.StructureByteStride = 0;

			hResult = g_pD3DDevice->CreateBuffer(&cbd, NULL, &m_pSpriteCB);
			assert(hResult == S_OK);

			D3D11_SAMPLER_DESC sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sd.MinLOD = 0;
			sd.MaxLOD = D3D11_FLOAT32_MAX;

			hResult = g_pD3DDevice->CreateSamplerState(&sd, &m_pSpriteSampler);
			assert(hResult == S_OK);

			D3D11_BLEND_DESC bd;
			ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));

			bd.RenderTarget[0].BlendEnable = true;
			bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			hResult = g_pD3DDevice->CreateBlendState(&bd, &m_pAlphaBlendOn);
			assert(hResult == S_OK);

			bd.RenderTarget[0].BlendEnable = false;

			hResult = g_pD3DDevice->CreateBlendState(&bd, &m_pAlphaBlendOff);
			assert(hResult == S_OK);

			InitializeCriticalSection(&CreateSpriteCS);
			m_bRenderingEnabled = false;
			return true;
		}

		void Shutdown()
		{
			DeleteCriticalSection(&CreateSpriteCS);

			if (m_pSpriteVB)
			{
				delete m_pSpriteVB;
				m_pSpriteVB = nullptr;
			}

			if (m_pSpriteIB)
			{
				delete m_pSpriteIB;
				m_pSpriteIB = nullptr;
			}

			SafeRelease_Checked(m_pAlphaBlendOff);
			SafeRelease_Checked(m_pAlphaBlendOn);

			SafeRelease_Checked(m_pSpriteSampler);

			SafeRelease_Checked(m_pSpriteVS);
			SafeRelease_Checked(m_pSpritePS);
			SafeRelease_Checked(m_pSpriteCB);
			SafeRelease_Checked(m_pSpriteVL);
		}

		bool BeginRendering()
		{
			assert(m_pSpriteVS);
			assert(m_pSpriteVL);
			assert(m_pSpritePS);

			if (m_bRenderingEnabled)
				return true;

			ID3D11Buffer* pVB = m_pSpriteVB->GetDeviceBuffer();
			assert(pVB);

			SafeDeviceInterface<ID3D11DeviceContext> ImmediateContext = GetImmediateContext();

			ImmediateContext->IASetVertexBuffers(0, 1, &pVB, &m_strideVB, &m_offsetVB);

			ImmediateContext->IASetInputLayout(m_pSpriteVL);

			ImmediateContext->VSSetShader(m_pSpriteVS, nullptr, 0);
			ImmediateContext->PSSetShader(m_pSpritePS, nullptr, 0);
			ImmediateContext->PSSetSamplers(0, 1, &m_pSpriteSampler);

			ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			m_Projection = DirectX::XMMatrixOrthographicLH(float(GetDisplayWidth()), float(GetDisplayHeight()), 0.0f, 1.0f);

			m_bRenderingEnabled = true;

			return true;
		}

		void EndRendering()
		{
			m_bRenderingEnabled = false;
		}

	}
} // namespace GLib