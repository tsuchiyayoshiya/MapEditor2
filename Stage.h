#pragma once
#include "Engine/GameObject.h"

#include<Windows.h>
namespace {
	const int MODEL_NUM{ 5 };
	const int XSIZE{ 15 };
	const int ZSIZE{ 15 };
	enum BLOCKTYPE
	{
		DEFAULT, BRICK, GRASS, SAND, WATER
	};
}

struct {
	BLOCKTYPE type;
	int HEGHT = 0;
	float raydist;
	bool IsRay;

}table_[XSIZE][ZSIZE];

//�����V�[�����Ǘ�����N���X
class Stage : public GameObject
{
	int hModel_[MODEL_NUM];    //���f���ԍ�
	//int table_[XSIZE][ZSIZE];
	int mode_;//0:�グ��  1:������  2:��ނ�ς��� 
	int select_;//���
	int menu_; //���j���[�̎�� 0:�쐬 1:�J��,2:�ۑ�
	XMFLOAT3 keepPos;


public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	Stage(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
	void Table_Reset();
	void SetBlock(int _x, int _z, BLOCKTYPE _type) { table_[_x][_z].type = _type; };
	void SetBlockHeght(int _x, int _z, int _height) { table_[_x][_z].HEGHT = _height; };
	void Save();

};