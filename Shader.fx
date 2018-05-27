Texture2D diffuseTexture;
SamplerState diffuseTextureSampler;

struct VS_INPUT
{
	float4 Position   : POSITION;
	float4 Color      : COLOR;
	float2 Tex		  : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position   : SV_POSITION;
	float4 Color      : COLOR;
	float2 Tex		  : TEXCOORD0;
};
//cbuffer cb0 {
	matrix ModelMatrix;
//}

//cbuffer cb1{
	matrix ViewMatrix;
//}
//cbuffer cb2 {
	matrix ProjectionMatrix;
//}

VS_OUTPUT vsmain(VS_INPUT In)
{
	VS_OUTPUT Out;
	Out.Position = mul(In.Position, ModelMatrix);
	//Out.Position = mul(Out.Position, ViewMatrix);
	Out.Position = mul(mul(mul(In.Position, ModelMatrix), ViewMatrix), ProjectionMatrix);
	Out.Color = In.Color;
	Out.Tex = In.Tex;
	return Out;
}

float4 psmain(VS_OUTPUT In) : SV_TARGET
{
	float4 texel = diffuseTexture.Sample(diffuseTextureSampler, In.Tex);
	return In.Color*texel;
}