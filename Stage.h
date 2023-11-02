#pragma once
#include "Engine/GameObject.h"
#include<vector>
#include<Windows.h>
namespace {
	const int MODEL_NUM{ 5 };
	const int XSIZE{ 15 };
	const int ZSIZE{ 15 };
	const int XDATA{ 30 };
	const int ZDATA{ 30 };
	enum BLOCKTYPE
	{
		DEFAULT, BRICK, GRASS, SAND, WATER
	};
}
struct {
	BLOCKTYPE type;
	int HEIGHT = 0;
	float raydist;
	bool IsRay;
}table_[XSIZE][ZSIZE];
//■■シーンを管理するクラス
class Stage : public GameObject
{
	int hModel_[MODEL_NUM];    //モデル番号
	//int table_[XSIZE][ZSIZE];
	int mode_;//0:上げる  1:下げる  2:種類を変える
	int select_;//種類
	int menu_mode_;//0:新規作成 1:開く 2:保存
	XMFLOAT3 keepPos;
	string data_;

public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Stage(GameObject* parent);
	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//開放
	void Release() override;
	BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
	void Save();
	void Open();
	void Table_Reset();
	void SetBlock(int _x, int _z, BLOCKTYPE _type) { table_[_x][_z].type = _type; };
	void SetBlockHeight(int _x, int _z, int _height) { table_[_x][_z].HEIGHT = _height; };
};