#include "VSOutput.hlsli"

struct VS_INPUT
{
	float4 vPosition		: POSITION;
	float4 vColor			: COLOR0;
	float2 vTC0				: TEXCOORD0;
};

cbuffer ConstantBuffer
{
	float4x4 mWorldViewProj : WORLDVIEWPROJ;
};

VS_OUTPUT main( VS_INPUT i_In )
{
	VS_OUTPUT		o_Out;

	o_Out.vPosition = mul( i_In.vPosition, mWorldViewProj );
	o_Out.vColor	= i_In.vColor;
	o_Out.vTC0		= i_In.vTC0;

	return o_Out;
}