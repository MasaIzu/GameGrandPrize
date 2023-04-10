#include "Particle.hlsli"

////Z^[©çÌItZbg
//static const float4 offset_array[20][3] =
//{
//	{
//		float4(1  ,0  ,0.5,0),//¶º
//		float4(0.5,1  ,0  ,0),//¶ã
//		float4(0  ,0.5,1  ,0),//¶º
//	},
//	{
//		float4(-1  ,0  ,0.5,0),//¶º
//		float4(-0.5,1  ,0  ,0),//¶ã
//		float4(0  ,0.5  ,1,0)//Eº
//	},
//	{
//		float4(1  , 0  ,0.5,0),//¶º
//		float4(0.5,-1  ,0  ,0),//¶ã
//		float4(0  ,-0.5,1  ,0),//¶º
//	},
//	{
//		float4(-1  , 0  ,0.5,0),//¶º
//		float4(-0.5,-1  ,0  ,0),//¶ã
//		float4(0   ,-0.5,1  ,0),//¶º
//	},
//
//	{
//		float4(0.5,1  ,0,0),//¶º
//		float4(-0.5,1  ,0,0),//¶ã
//		float4(0  ,0.5,1,0),//¶º
//	},
//	{
//		float4(0.5,1  , 0,0),//¶º
//		float4(-0.5,1  , 0,0),//¶ã
//		float4(0   ,0.5,-1,0),//¶º
//	},
//	{
//		float4(0.5,-1  ,0,0),//¶º
//		float4(-0.5,-1  ,0,0),//¶ã
//		float4(0  ,-0.5,1,0),//¶º
//	},
//	{
//		float4(0.5,-1  , 0,0),//¶º
//		float4(-0.5,-1  , 0,0),//¶ã
//		float4(0  ,-0.5,-1,0)//Eº
//	},
//
//	{
//		float4(1  ,0, 0.5,0),//¶º
//		float4(1  ,0,-0.5,0),//¶ã
//		float4(0.5,1, 0  ,0)//Eº
//	},
//	{
//		float4(1  , 0, 0.5,0),//¶º
//		float4(1  , 0,-0.5,0),//¶ã
//		float4(0.5,-1, 0  ,0)//Eº
//	},
//	{
//		float4(-1  ,0, 0.5,0),//¶º
//		float4(-1  ,0,-0.5,0),//¶ã
//		float4(-0.5,1, 0  ,0)//Eº
//	},
//	{
//		float4(-1  , 0, 0.5,0),//¶º
//		float4(-1  , 0,-0.5,0),//¶ã
//		float4(-0.5,-1, 0  ,0)//Eº
//	},
//
//	{
//		float4(0, 0.5,1  ,0),//¶º
//		float4(0,-0.5,1  ,0),//¶ã
//		float4(1, 0  ,0.5,0),//¶º
//	},
//	{
//		float4( 0, 0.5,1  ,0),//¶º
//		float4( 0,-0.5,1  ,0),//¶ã
//		float4(-1, 0  ,0.5,0),//¶º
//	},
//	{
//		float4(0, 0.5,-1  ,0),//¶º
//		float4(0,-0.5,-1  ,0),//¶ã
//		float4(1, 0  ,-0.5,0)//Eº
//	},
//	{
//		float4( 0, 0.5,-1  ,0),//¶º
//		float4( 0,-0.5,-1  ,0),//¶ã
//		float4(-1, 0  ,-0.5,0)//Eº
//	},
//
//	{
//		float4(1  , 0  ,-0.5,0),//¶º
//		float4(0.5, 1  , 0  ,0),//¶ã
//		float4(0  ,0.5,-1  ,0),//¶º
//	},
//	{
//		float4(-1  ,0  ,-0.5,0),//¶º
//		float4(-0.5,1  , 0  ,0),//¶ã
//		float4( 0  ,0.5,-1  ,0),//¶º
//	},
//	{
//		float4(1  , 0  ,-0.5,0),//¶º
//		float4(0.5,-1  , 0  ,0),//¶ã
//		float4(0  ,-0.5,-1  ,0),//¶º
//	},
//	{
//		float4(-1  , 0  ,-0.5,0),//¶º
//		float4(-0.5,-1  , 0  ,0),//¶ã
//		float4( 0  ,-0.5,-1  ,0),//¶º
//	},
//};
//
//static const float2 uv_array[3] =
//{
//	float2(0.0,1.0),
//	float2(0.5,0.0),
//	float2(1.0,1.0),
//};
//
//[maxvertexcount(20 * 3)]
//void main(
//	point VSOutput input[1] : SV_POSITION,
//	inout TriangleStream<GSOutput>output
//)
//{
//	float4 offset;
//	GSOutput element;
//	for (uint j = 0; j < 20; j++)
//	{
//		for (uint i = 0; i < 3; i++)
//		{
//			//offset = mul(matBillboard, offset_array[i]);
//			offset = offset_array[j][i] * input[0].scale;
//			//offset = mul(matBillboard, offset);
//			element.svpos = input[0].pos + offset;
//			//element.svpos = input[0].pos + offset_array[i];
//			element.svpos =  element.svpos;
//			element.uv = uv_array[i]; 
//			element.color = input[0].color;
//			output.Append(element);
//		}
//		output.RestartStrip();
//	}
//}

static const int vnum = 4;

//Z^[©çÌItZbg
static const float4 offset_array[vnum] =
{
	float4(-0.3f,-0.3f,0,0),//¶º
	float4(-0.3f,+0.3f,0,0),//¶ã
	float4(+0.3f,-0.3f,0,0),//Eº
	float4(+0.3f,+0.3f,0,0)//Eã
};

static const float2 uv_array[vnum] =
{
	float2(0,1),
	float2(0,0),
	float2(1,1),
	float2(1,0),
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput>output
)
{
	//for (uint i = 0; i < 3; i++)
	//{
	//	GSOutput element;
	//	element.svpos = input[i].svpos;
	//	element.normal = input[i].normal;
	//	element.uv = input[i].uv;
	//	output.Append(element);
	//}

	////»ÝÌXgbvðI¹
	//output.RestartStrip();

	////2ÂÚÌOp`
	//for (uint i = 0; i < 3; i++)
	//{
	//	GSOutput element;
	//	element.svpos = input[i].svpos+float4(20.0f,0.0f,0.0f,0.0f);
	//	element.normal = input[i].normal;
	//	//UVð5{É
	//	element.uv = input[i].uv * 5.0f;
	//	output.Append(element);
	//}
	float4 offset;
	GSOutput element;
	for (uint i = 0; i < vnum; i++)
	{
		//offset = mul(matBillboard, offset_array[i]);
		offset = offset_array[i] * input[0].scale;
		offset = mul(matBillboard, offset);
		element.svpos = input[0].pos + offset;
		//element.svpos = input[0].pos + offset_array[i];
		element.svpos = mul(mat, element.svpos);
		element.uv = uv_array[i];
		element.color = input[0].color;
		output.Append(element);
	}
}