#pragma once
#include"Direct3D.h"
#include "Texture.h"
#include<DirectXMath.h>
#include<vector>

using namespace DirectX;

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
class Sprite
{
	//コンスタントバッファー
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matW;//ワールド行列
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;//位置
		XMVECTOR uv;//UV
	};
protected:
	UINT64 vertexNum_;//頂点数
	std::vector<VERTEX> vertices_;//頂点情報
	ID3D11Buffer* pVertexBuffer_; //頂点バッファ

	UINT64 intdexNum;//インデックス数
	std::vector<int> index_;//インデックス型
	ID3D11Buffer* pIndexBuffer_;//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	

	Texture* pTexture_; //テクスチャ
public:
	Sprite();
	~Sprite();
	//初期化（ポリゴンを表示するための各情報を準備）
	//戻り値　成功/失敗
	HRESULT Initialize();
	//引数　ワールド行列
    void Draw(XMMATRIX& worldMatrix);
	//解放
	void Release();
private:
	//-------Initializeから呼ばれる関数----------------
	virtual void InitVertexData();//頂点情報の準備
	HRESULT CreateVertexBuffer();//頂点バッファを作成

	virtual void InitIndexData();//Index情報を準備
	HRESULT CreateIndexBuffer();//インデックスバッファを作成

	HRESULT CreateConstantBuffer();//コンスタントバッファを作成

	HRESULT LoadTexture();//テクスチャを表示

	//--------------Drawから呼ばれる関数
	void PassDataToCB(DirectX::XMMATRIX& worldMatrix);//コンスタントバッファを作成
	void SetBufferToPipeline();//各バッファをパイプラインにセット


};

