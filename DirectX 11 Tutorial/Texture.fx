cbuffer MatrixBuffer : register(b0)
{
	matrix gWorld;
	matrix gView;
	matrix gProjection;
};

cbuffer LightBuffer : register(b1)
{
	float4 gLightPos;
	float4 gLightColor;
};

cbuffer CameraBuffer : register(b2)
{
	float4 gCamPos;
};

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float3 mNormal : NORMAL;
	float2 mTexCoord : TEXCOORD;
};

struct PS_INPUT
{
	float4 mPosition : SV_POSITION;
	float3 mDiffuse : TexCOORD0;
	float3 mViewDirection : TEXCOORD1;
	float3 mReflection : TEXCOORD2;
	float2 mTexCoord : TEXCOORD3;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.mPosition = mul(input.mPosition, gWorld);

	float3 lightDirection = output.mPosition.xyz - gLightPos.xyz;
	lightDirection = normalize(lightDirection);

	float3 viewDirection = normalize(output.mPosition.xyz - gCamPos.xyz);
	output.mViewDirection = viewDirection;

	output.mPosition = mul(output.mPosition, gView);
	output.mPosition = mul(output.mPosition, gProjection);

	float3 worldNormal = mul(input.mNormal, (float3x3)gWorld);
	worldNormal = normalize(worldNormal);

	output.mTexCoord = input.mTexCoord;
	output.mDiffuse = dot(-lightDirection, worldNormal);
	output.mReflection = reflect(lightDirection, worldNormal);

	return output;
}

Texture2D shaderTexture;
SamplerState SampleType;

float4 PS(PS_INPUT input) : SV_TARGET
{
	float3 diffuse = saturate(input.mDiffuse);
	float3 ambient = float3(0.4f, 0.4f, 0.4f);
	float3 viewDirection = normalize(input.mViewDirection);
	float3 reflection = normalize(input.mReflection);
	float3 specular = 0;

	if (diffuse.y > 0.0f)
	{
		specular = saturate(dot(reflection, -viewDirection));
		specular = pow(specular, 40.0f);
	}
	
	//float4 textureColor = shaderTexture.Sample(SampleType, input.mTexCoord);
	float4 textureColor = float4(0.45703125f, 0.21484375f, 0.0546875f, 0.0f);
	textureColor = float4(textureColor.xyz * (diffuse + ambient) + specular, 1);
	return textureColor;
}
