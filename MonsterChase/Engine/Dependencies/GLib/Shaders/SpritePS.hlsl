#include "VSOutput.hlsli"

struct PS_OUTPUT
{
	float4 Color			: COLOR0;
	float  Depth			: DEPTH;
};

texture2D			tex0;
SamplerState		s_2D;

float4 main( VS_OUTPUT i_In ) : SV_TARGET
{
	float4 o_Out;

	float4 texColor = tex0.Sample( s_2D, i_In.vTC0 );

	o_Out.r = texColor.r; // *i_In.vColor.a + i_In.vColor.r * ( 1.0f - i_In.vColor.a );
	o_Out.g = texColor.g; // *i_In.vColor.a + i_In.vColor.g * ( 1.0f - i_In.vColor.a );
	o_Out.b = texColor.b; // *i_In.vColor.a + i_In.vColor.b * ( 1.0f - i_In.vColor.a );
	o_Out.a = texColor.a;

	return o_Out;
}