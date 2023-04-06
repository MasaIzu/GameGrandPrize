#pragma once
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<assimp/cimport.h>

#include <d3d12.h>
#include <d3dx12.h>

class FbxAnimation {
public:

	void Load(const std::string& failPath);

	aiAnimation* GetAnimation();

public: // 定数
	// モデル格納ルートパス
	static const std::string baseDirectory;
	// テクスチャがない場合の標準テクスチャファイル名
	static const std::string defaultTextureFileName;

private:

	aiAnimation* modelAnimation;
	const aiScene* mScene;

};
