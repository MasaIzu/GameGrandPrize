#pragma once
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<assimp/cimport.h>

#include <d3d12.h>
#include <d3dx12.h>

class FbxAnimation {
public:

	FbxAnimation();
	~FbxAnimation();

	void Load(const std::string& failPath);

	aiAnimation* GetAnimation();

public: // �萔
	// ���f���i�[���[�g�p�X
	static const std::string baseDirectory;
	// �e�N�X�`�����Ȃ��ꍇ�̕W���e�N�X�`���t�@�C����
	static const std::string defaultTextureFileName;

private:

	aiAnimation* modelAnimation;
	const aiScene* mScene;

};
