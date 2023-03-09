#include "Particle.hlsli"

//static const int vnum = 8;

//センターからのオフセット
static const float4 offset_array[6][4] =
{
	//前
	{float4(-0.5f,-0.5f,-0.5f,0),//左下
		float4(-0.5f, 0.5f,-0.5f,0),//左上
		float4(0.5f,-0.5f,-0.5f,0),//右下
		float4(0.5f, 0.5f,-0.5f,0)},//右上
		//後
	{float4(-0.5f,-0.5f, 0.5f,0),//左下
		float4(-0.5f, 0.5f, 0.5f,0),//左上
		float4(0.5f,-0.5f, 0.5f,0),//右下
		float4(0.5f, 0.5f, 0.5f,0)},//右上
		//左
	{float4(-0.5f,-0.5f,-0.5f,0),//左下
		float4(-0.5f,-0.5f, 0.5f,0),//左上
		float4(-0.5f, 0.5f,-0.5f,0),//右下
		float4(-0.5f, 0.5f, 0.5f,0) },//右上
		//右
	{float4(0.5f,-0.5f,-0.5f,0),//左下
		float4(0.5f,-0.5f, 0.5f,0),//左上
		float4(0.5f, 0.5f,-0.5f,0),//右下
		float4(0.5f, 0.5f, 0.5f,0) },//右上
		//下
	{float4(-0.5f,-0.5f,-0.5f,0),//左下
		float4(-0.5f,-0.5f, 0.5f,0),//左上
		float4(0.5f,-0.5f,-0.5f,0),//右下
		float4(0.5f,-0.5f, 0.5f,0)},//右上
		//上
	{float4(-0.5f, 0.5f,-0.5f,0),//左下
		float4(-0.5f, 0.5f, 0.5f,0),//左上
		float4(0.5f, 0.5f,-0.5f,0),//右下
		float4(0.5f, 0.5f, 0.5f,0) },//右上
};
//センターからのオフセット
//unsigned short indices[] =
//{
//	//前
//	0,1,2,//三角形1
//	1,3,2,//三角形2
//	//後ろ
//	6,5,4,//三角形1
//	6,7,5,//三角形2
//	//左
//	8,9,10,//三角形1
//	9,11,10,//三角形2
//	//右
//	14,13,12,//三角形1
//	14,15,13,//三角形2
//	//下
//	18,17,16,//三角形1
//	18,19,17,//三角形2
//	//上
//	20,21,22,//三角形1
//	21,23,22,//三角形2
//};

[maxvertexcount(4*6)]
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

	////現在のストリップを終了
	//output.RestartStrip();

	////2つ目の三角形
	//for (uint i = 0; i < 3; i++)
	//{
	//	GSOutput element;
	//	element.svpos = input[i].svpos+float4(20.0f,0.0f,0.0f,0.0f);
	//	element.normal = input[i].normal;
	//	//UVを5倍に
	//	element.uv = input[i].uv * 0.5f;
	//	output.Append(element);
	//}
	float4 offset;
	GSOutput element;
	for (uint j = 0; j < 6; j++)
	{
		for (uint i = 0; i < 4; i++)
		{
			//offset = mul(matBillboard, offset_array[i]);
			offset = offset_array[j][i] * input[0].scale;
			//offset = mul(matBillboard, offset);
			element.svpos = input[0].pos + offset;
			//element.svpos = input[0].pos + offset_array[i];
			element.svpos = mul(mat, element.svpos);
			//element.uv = uv_array[i]; 
			element.color = input[0].color;
			output.Append(element);
		}
		output.RestartStrip();
	}
}

//[maxvertexcount(2)]
//void main(
//	triangle VSOutput input[3]:SV_POSITION,
//	//線分ストリーム
//	inout LineStream<GSOutput>output
//)
//{
//	GSOutput element;
//
//	//線分の始点
//	element.svpos = input[0].svpos;
//	element.normal = input[0].normal;
//	element.uv = input[0].uv;
//	output.Append(element);
//
//	//線分の終点
//	element.svpos = input[1].svpos;
//	element.normal = input[1].normal;
//	element.uv = input[1].uv;
//	output.Append(element);
//
//}

////三角形の入力から、線分を三つ出力するサンプル
//[maxvertexcount(6)]
//void main(
//	triangle VSOutput input[3]:SV_POSITION,
//	inout LineStream<GSOutput>output
//)
//{
//	GSOutput element;
//	//三角形の頂点1点ずつ扱う
//	for (uint i = 0; i < 3; i++)
//	{
//		//線分の始点
//		element.svpos = input[i].svpos;
//		element.normal = input[i].normal;
//		element.uv = input[i].uv;
//		output.Append(element);
//		//線分の終点
//		if (i == 2) {
//			//+1すると溢れので、最初に戻る
//			element.svpos = input[0].svpos;
//			element.normal = input[0].normal;
//			element.uv = input[0].uv;
//		}
//		else{
//			element.svpos = input[i+1].svpos;
//			element.normal = input[i+1].normal;
//			element.uv = input[i+1].uv;
//		}
//		output.Append(element);
//		//現在のストリップを終了し、次のストリップを開始
//		output.RestartStrip();
//	}
//}

//三角形の入力から、線分を3つ出力するサンプル
//[maxvertexcount(4)]
//void main(
//	triangle VSOutput input[3]:SV_POSITION,
//	//線分ストリーム
//	inout LineStream<GSOutput>output
//)
//{
//	GSOutput element;
//
//	for (uint i = 0; i < 3; i++)
//	{
//		//線分の始点
//		element.svpos = input[i].svpos;
//		element.normal = input[i].normal;
//		element.uv = input[i].uv;
//		output.Append(element);
//	}
//	//線分の終点
//	element.svpos = input[0].svpos;
//	element.normal = input[0].normal;
//	element.uv = input[0].uv;
//	output.Append(element);
//
//}