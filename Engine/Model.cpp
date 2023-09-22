#include "Model.h"

namespace Model
{

	std::vector<ModelData*>modelList;
}

int Model::Load(std::string fileName)
{
	ModelData* pData;
	pData = new ModelData;
	pData->fileName_ = fileName;
	pData->pFbx_ = nullptr;
	//filenameが同じなら読まない
	for (auto& e : modelList)
	{
		if (e->fileName_ == fileName)
		{
			pData->pFbx_ = e->pFbx_;
			break;
		}
	}

	if (pData->pFbx_ == nullptr)
	{
		pData->pFbx_ = new Fbx;
		pData->pFbx_->Load(fileName);
	}
	modelList.push_back(pData);
	return(modelList.size() - 1);
	//読んで作る

}

void Model::SetTransform(int hModel, Transform transfome)
{
	modelList[hModel]->transfome_ = transfome;
	//モデル番号はmodekListのインデックス
}

void Model::Draw(int hModel)
{
	//モデル番号はmodekListのインデックス
	modelList[hModel]->pFbx_->Draw(modelList[hModel]->transfome_);
}

void Model::Release()
{
	bool isReffered = false;
	for (int i = 0; i < modelList.size(); i++)
	{
		for (int j = i + 1; j < modelList.size(); j++)
		{
			if (modelList[i]->pFbx_ == modelList[j]->pFbx_)
			{
				isReffered = true;
				break;
			}
		}
		if (isReffered == false)
		{
			SAFE_DELETE(modelList[i]->pFbx_);
		}
		SAFE_DELETE(modelList[i]);
	}
	modelList.clear();
}

void Model::RayCast(int hModel_, RayCastData& rayData)
{
	modelList[hModel_]->transfome_.Calclation();
	//①ワールド行列の逆行列
	XMMATRIX wInv = XMMatrixInverse(nullptr, modelList[hModel_]->transfome_.GetWorldMatrix());
	//②レイの通過点を求める
	XMVECTOR vpass{ rayData.start.x + rayData.dir.x,
					rayData.start.y + rayData.dir.y,
					rayData.start.z + rayData.dir.z,
					rayData.start.w + rayData.dir.w };
	//③rayData.startをモデル空間に変換
	XMVECTOR vstart = XMLoadFloat4(&rayData.start);
	vstart = XMVector3TransformCoord(vstart, wInv);
	XMStoreFloat4(&rayData.start, vstart);
	//④(始点から方向ベクトルを伸ばした先)通過点(②)に①をかける
	vpass = XMVector3TransformCoord(vpass, wInv);
	//⑤rayData.dirを③から④に向かうベクトル
	vpass = vpass - vstart;
	XMStoreFloat4(&rayData.dir, vpass);


	//指定されたモデル番号のFBXにレイキャスト
	modelList[hModel_]->pFbx_->RayCast(rayData);
}