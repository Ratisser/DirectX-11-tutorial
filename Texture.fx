
SamplerState SampleType;

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VS_INPUT
{
	float4 position : POSITION;
	float2 tex : TEXCOORD;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
};