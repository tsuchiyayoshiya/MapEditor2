#include "Stage.h"
#include "Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Direct3D.h"
#include"resource.h"
#include"Engine/Fbx.h"
//using namespace XMINT2;

//#include "DxLib.h"
//コンストラクタ
Stage::Stage(GameObject* parent)
	: GameObject(parent, "Stage"), mode_(0), select_(0), menu_(0)
{
	for (int i = 0; i < MODEL_NUM; i++)
	{
		hModel_[i] = -1;
	}

	Table_Reset();
}

//初期化
void Stage::Initialize()
{
	string ModelName[] = {
		"BoxDefault.fbx",
		"BoxBrick.fbx",
		"BoxGrass.fbx",
		"BoxSand.fbx",
		"BoxWater.fbx"
	};
	string name_base = "Assets\\";
	//モデルデータのロード
	for (int i = 0; i < MODEL_NUM; i++)
	{
		hModel_[i] = Model::Load(name_base + ModelName[i]);
		assert(hModel_[i] >= 0);
	}

	for (int x = 0; x < XSIZE; x++)
	{
		for (int z = 0; z < ZSIZE; z++)
		{

			SetBlock(x, z, (BLOCKTYPE)(x % 5));
		}
	}
}

//更新
void Stage::Update()
{
	float w = (float)(Direct3D::scrWidth / 2.0f);//画面幅の半分
	float h = (float)(Direct3D::scrHeight / 2.0f);//画面高さの半分
	float distmin = 999.0f;

	//0ffsetx,yは0
	//minZ =0 maxZ =1

	XMMATRIX vp =
	{
		 w,   0,  0, 0,
		 0,  -h,  0, 0,
		 0,   0,  1, 0,
		 w,   h,  0, 1
	};

	//ビューポート
	XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
	//プロジェクション変換
	XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
	//ビュー変換

	XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
	XMFLOAT3 mousePosFront = Input::GetMousePosition();//マウスポジゲット
	mousePosFront.z = 0.0f;
	XMFLOAT3 mousePosBack = Input::GetMousePosition();
	mousePosBack.z = 1.0f;

	//①　mousePosFrontをベクトルに変換
	XMVECTOR vMousePosF = XMLoadFloat3(&mousePosFront);
	//②　①にinvVP、invProj、invViewをかける
	vMousePosF = XMVector3TransformCoord(vMousePosF, invVP * invProj * invView);
	//③　mousePosBackをベクトルに変換
	XMVECTOR vMousePosB = XMLoadFloat3(&mousePosBack);
	//④　③にinvVP、invProj、invViewをかける
	vMousePosB = XMVector3TransformCoord(vMousePosB, invVP * invProj * invView);




	for (int x = 0; x < XSIZE; x++)
	{
		for (int z = 0; z < ZSIZE; z++)
		{
			for (int y = 0; y < table_[x][z].HEIGHT + 1; y++)
			{

				//⑤　②から④に向かってレイを打つ
				RayCastData data;
				XMStoreFloat4(&data.start, vMousePosF);
				XMStoreFloat4(&data.dir, vMousePosB - vMousePosF);
				Transform trans;
				trans.position_.x = x;
				trans.position_.y = y;
				trans.position_.z = z;
				Model::SetTransform(hModel_[0], trans);

				Model::RayCast(hModel_[0], data);
				//⑥　レイが当たったらブレークポイントで止める
				if (Input::IsKeyDown(DIK_R))//リセット(更地に戻す)
				{
					Table_Reset();
				}
				for (int x = 0; x < XSIZE; x++)
				{
					for (int z = 0; z < ZSIZE; z++)
					{
						table_[x][z].raydist = 999;
					}
				}
				if (data.hit)
				{
					table_[x][z].raydist = data.dist;
					if (Input::IsMouseButtonDown(0))
					{
						//何らかの処理
						if (table_[x][z].raydist < distmin)
						{
							distmin = table_[x][z].raydist;
							switch (mode_)//地形の編集
							{
							case 0:
								table_[x][z].HEIGHT++;
								break;
							case 1:
								if (table_[x][z].HEIGHT > 0)
									table_[x][z].HEIGHT--;
								break;
							case 2:
								switch (select_)//ブロックの種類
								{
								case 0:
									SetBlock(x, z, (BLOCKTYPE)(select_));
									break;
								case 1:
									SetBlock(x, z, (BLOCKTYPE)(select_));
									break;
								case 2:
									SetBlock(x, z, (BLOCKTYPE)(select_));
									break;
								case 3:
									SetBlock(x, z, (BLOCKTYPE)(select_));
									break;
								case 4:
									SetBlock(x, z, (BLOCKTYPE)(select_));
									break;
								default:
									break;
								}
								break;
							default:
								break;
							}
						}

					}
					return;
				}


			}
		}
	}



}

//描画
void Stage::Draw()
{
	for (int x = 0; x < XSIZE; x++)
	{
		for (int z = 0; z < ZSIZE; z++)
		{
			for (int y = 0; y < table_[x][z].HEIGHT + 1; y++)
			{
				int type_ = table_[x][z].type;

				Transform trans;
				trans.position_.x = x;
				trans.position_.z = z;
				trans.position_.y = y;
				Model::SetTransform(hModel_[type_], trans);
				Model::Draw(hModel_[type_]);
			}

		}
	}
}

//開放
void Stage::Release()
{
}





//偽物(何か処理をしたらTRUE、何もしなかったらFALSEを返す)
BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		//ダイアログできた
	case WM_INITDIALOG:
		//ラジオボタンの初期値
		SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);
		//コンボボタンの初期値
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"デフォルト");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"レンガ");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"草");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"土");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"水");

		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_SETCURSEL, 0, 0);

		return TRUE;
	case WM_COMMAND:
		mode_ = LOWORD(wp) - IDC_RADIO_UP;
		select_ = (int)SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0);
	}
	return FALSE;

}



void Stage::Save()
{
	char inData = 0;//高さと種類を入れる文字列
	//inData += table_[15][15].HEIGHT; //高さ
	inData += select_;



	char fileName[MAX_PATH] = "無題.map";  //ファイル名を入れる変数

	//「ファイルを保存」ダイアログの設定
	OPENFILENAME ofn;                         	//名前をつけて保存ダイアログの設定用構造体
	ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
	ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
	ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")        //─┬ファイルの種類
		TEXT("すべてのファイル(*.*)\0*.*\0\0");     //─┘
	ofn.lpstrFile = fileName;               	//ファイル名
	ofn.nMaxFile = MAX_PATH;               	//パスの最大文字数
	ofn.Flags = OFN_OVERWRITEPROMPT;   		//フラグ（同名ファイルが存在したら上書き確認）
	ofn.lpstrDefExt = "map";                  	//デフォルト拡張子



	//「ファイルを保存」ダイアログ
	BOOL selFile;
	selFile = GetSaveFileName(&ofn);
	//aaaaaaa
	//キャンセルしたら中断
	if (selFile == FALSE) return;
	HANDLE hFile;
	hFile = CreateFile(
		fileName,    //ファイル名
		GENERIC_WRITE,  //アクセスモード
		0,
		NULL,
		CREATE_ALWAYS,     //作成方法
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	string data = "";
	//data.length()
	DWORD bytes = 0;
	WriteFile(
		hFile,              //ファイルハンドル
		"ABCEFG",          //保存したい文字列
		12,                  //保存する文字数
		&bytes,             //保存したサイズ
		NULL
	);
	CloseHandle(hFile);
	= CreateFile(
		fileName,                 //ファイル名
		GENERIC_READ,           //アクセスモード（書き込み用）
		0,                      //共有（なし）
		NULL,                   //セキュリティ属性（継承しない）
		OPEN_EXISTING,           //作成方法
		FILE_ATTRIBUTE_NORMAL,  //属性とフラグ（設定なし）
		NULL);                  //拡張属性（なし）

	DWORD fileSize = GetFileSize(hFile, NULL);

	char* data;
	data = new char[fileSize];
	BOOL res = ReadFile(
		hFile,     //ファイルハンドル
		data,      //データを入れる変数
		fileSize,  //読み込むサイズ
		&bytes,  //読み込んだサイズ
		NULL);     //オーバーラップド構造体（今回は使わない）

	//ファイルを開くダイアログ
	BOOL opnFile;
	opnFile = GetOpenFileName(&ofn);

	CloseHandle(hFile);
}


/*
void Stage::Open()
{


	char fileName[MAX_PATH] = "無題.map";  //ファイル名を入れる変数


	//HANDLE res = hFile;

	//dwBytes = 0; //読み込み位置



	//ファイルを開くダイアログ
	BOOL opnFile;
	opnFile = GetOpenFileName(&res);

	CloseHandle(hFile);
}
*/
void Stage::Table_Reset()
{
	//テーブルを初期化
	for (int x = 0; x < XSIZE; x++)
	{
		for (int z = 0; z < ZSIZE; z++)
		{
			SetBlock(x, z, (BLOCKTYPE)(x % 5));
			//table_[x][z].HEIGHT = 1;
			SetBlockHeight(x, z, 0);
		}
	}
}



