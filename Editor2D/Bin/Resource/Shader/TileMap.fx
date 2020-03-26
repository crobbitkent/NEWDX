
#include "Share.fx"

struct VS_INPUT_INSTANCING_TEX
{
	float3	vPos	: POSITION;
	float2	vUV		: TEXCOORD;
	matrix	matWVP	: WORLD;
	float2	vImgStart	: START;
	float2	vImgEnd		: END;
	float2	vImgSize	: IMGSIZE;
	int		iImgFrame	: IMGFRAME;
	int		iImgType	: IMGTYPE;
};

struct VS_OUTPUT_INSTANCING_TEX
{
	float4	vPos	: SV_POSITION;
	float2	vUV		: TEXCOORD;
};

struct PS_OUTPUT_SINGLE
{
	float4	vColor	: SV_TARGET;
};

#define	IMAGE_ATLAS	0
#define	IMAGE_ARRAY	1

cbuffer TileMap	: register(b10)
{
	float2	g_vImgStart;
	float2	g_vImgEnd;
	float2	g_vImgSize;
	int		g_iImgFrame;
	int		g_iImgType;
};

float2 ComputeTileMapUV(float2 vUV, float2 vStart, float2 vEnd, float2 vSize)
{
	float2	vNewUV = (float2)0.f;

	if (vUV.x == 0.f)
		vNewUV.x = vStart.x / vSize.x;

	else
		vNewUV.x = vEnd.x / vSize.x;

	if (vUV.y == 0.f)
		vNewUV.y = vStart.y / vSize.y;

	else
		vNewUV.y = vEnd.y / vSize.y;

	return vNewUV;
}

VS_OUTPUT_INSTANCING_TEX TileMapVS(VS_INPUT_INSTANCING_TEX input)
{
	VS_OUTPUT_INSTANCING_TEX	output = (VS_OUTPUT_INSTANCING_TEX)0;

	float3	vPos = input.vPos - g_vMeshSize * g_vMeshPivot;

	output.vPos = mul(float4(vPos, 1.f), input.matWVP);
	output.vUV = input.vUV;

	if(g_iImgType == IMAGE_ATLAS)
		output.vUV = ComputeTileMapUV(input.vUV, input.vImgStart, input.vImgEnd, input.vImgSize);

	return output;
}

PS_OUTPUT_SINGLE TileMapPS(VS_OUTPUT_INSTANCING_TEX input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float4	vColor = g_BaseTexture.Sample(g_LinearSmp, input.vUV);

	output.vColor = vColor;

	return output;
}
