#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include<vector>
#include "Transform.h"
#include"Direct3D.h"

#include"Camera.h"

using std::vector;

#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")
class Texture;//ポインタならこれでOK(ヘッダをインクルードしなくていい)

struct RayCastData
{
	XMFLOAT4 start;
	XMFLOAT4 dir;
	bool hit;
	float dist = 0;
};


class Fbx
{
	//マテリアル
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4	diffuse;
	};
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal;
		XMFLOAT4		diffuseColor;		// ディフューズカラー（マテリアルの色）
		int		isTextured;		// テクスチャ貼ってあるかどうか
	};

	struct VERTEX
	{
		XMVECTOR position;//
		XMVECTOR uv;//ポリゴン数
		XMVECTOR normal;//法線ベクトル
	};



	VERTEX* pVertices_;
	int** ppIndex_;

	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_;	//マテリアルの個数

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer** pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	MATERIAL* pMaterialList_;
	vector<int>index_Count_;

public:

	Fbx();
	HRESULT Load(std::string fileName);
	void    InitVertex(fbxsdk::FbxMesh* mesh);
	void    InitIndex(fbxsdk::FbxMesh* mesh);
	void    IntConstantBuffer();	//コンスタントバッファ準備
	void    InitMaterial(fbxsdk::FbxNode* pNode);
	void    Draw(Transform& transform);
	void    Release();

	void RayCast(RayCastData& rayData);
};