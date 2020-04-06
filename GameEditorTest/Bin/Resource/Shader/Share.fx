
cbuffer Transform	: register(b0)
{
	matrix	g_matWorld;
	matrix	g_matView;
	matrix	g_matProj;
	matrix	g_matWV;
	matrix	g_matWVP;
	float3	g_vMeshPivot;
	float	g_fTrEmpty;
	float3	g_vMeshSize;
	float	g_fTrEmpty1;
};

cbuffer	Material	: register(b1)
{
	float4	g_vMtrlDif;
}

Texture2D	g_BaseTexture : register(t0);

SamplerState	g_LinearSmp	: register(s0);

//SamplerState	g_LinearSmp
//{
//	Filter = MIN_MAG_MIP_LINEAR;
//	AddressU = Wrap;
//	AddressV = Wrap;
//};