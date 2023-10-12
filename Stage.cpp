#include "Stage.h"
#include "Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Direct3D.h"
#include"resource.h"
#include"Engine/Fbx.h"
//�R���X�g���N�^
Stage::Stage(GameObject* parent)
	: GameObject(parent, "Stage"), mode_(0), select_(0), menu_(0)
{
	for (int i = 0; i < MODEL_NUM; i++)
	{
		hModel_[i] = -1;
	}

	Table_Reset();
}

//������
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
	//���f���f�[�^�̃��[�h
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

//�X�V
void Stage::Update()
{
	float w = (float)(Direct3D::scrWidth / 2.0f);//��ʕ��̔���
	float h = (float)(Direct3D::scrHeight / 2.0f);//��ʍ����̔���
	float distmin = 999.0f;

	//0ffsetx,y��0
	//minZ =0 maxZ =1

	XMMATRIX vp =
	{
		 w,   0,  0, 0,
		 0,  -h,  0, 0,
		 0,   0,  1, 0,
		 w,   h,  0, 1
	};

	//�r���[�|�[�g
	XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
	//�v���W�F�N�V�����ϊ�
	XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
	//�r���[�ϊ�

	XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
	XMFLOAT3 mousePosFront = Input::GetMousePosition();//�}�E�X�|�W�Q�b�g
	mousePosFront.z = 0.0f;
	XMFLOAT3 mousePosBack = Input::GetMousePosition();
	mousePosBack.z = 1.0f;

	//�@�@mousePosFront���x�N�g���ɕϊ�
	XMVECTOR vMousePosF = XMLoadFloat3(&mousePosFront);
	//�A�@�@��invVP�AinvProj�AinvView��������
	vMousePosF = XMVector3TransformCoord(vMousePosF, invVP * invProj * invView);
	//�B�@mousePosBack���x�N�g���ɕϊ�
	XMVECTOR vMousePosB = XMLoadFloat3(&mousePosBack);
	//�C�@�B��invVP�AinvProj�AinvView��������
	vMousePosB = XMVector3TransformCoord(vMousePosB, invVP * invProj * invView);




	for (int x = 0; x < XSIZE; x++)
	{
		for (int z = 0; z < ZSIZE; z++)
		{
			for (int y = 0; y < table_[x][z].HEGHT + 1; y++)
			{

				//�D�@�A����C�Ɍ������ă��C��ł�
				RayCastData data;
				XMStoreFloat4(&data.start, vMousePosF);
				XMStoreFloat4(&data.dir, vMousePosB - vMousePosF);
				Transform trans;
				trans.position_.x = x;
				trans.position_.y = y;
				trans.position_.z = z;
				Model::SetTransform(hModel_[0], trans);

				Model::RayCast(hModel_[0], data);
				//�E�@���C������������u���[�N�|�C���g�Ŏ~�߂�
				if (Input::IsKeyDown(DIK_R))//���Z�b�g(�X�n�ɖ߂�)
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
						//���炩�̏���
						if (table_[x][z].raydist < distmin)
						{
							distmin = table_[x][z].raydist;
							switch (mode_)//�n�`�̕ҏW
							{
							case 0:
								table_[x][z].HEGHT++;
								break;
							case 1:
								if (table_[x][z].HEGHT > 0)
									table_[x][z].HEGHT--;
								break;
							case 2:
								switch (select_)//�u���b�N�̎��
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

//�`��
void Stage::Draw()
{
	for (int x = 0; x < XSIZE; x++)
	{
		for (int z = 0; z < ZSIZE; z++)
		{
			for (int y = 0; y < table_[x][z].HEGHT + 1; y++)
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

//�J��
void Stage::Release()
{
}





//�U��(����������������TRUE�A�������Ȃ�������FALSE��Ԃ�)
BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		//�_�C�A���O�ł���
	case WM_INITDIALOG:
		//���W�I�{�^���̏����l
		SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);
		//�R���{�{�^���̏����l
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"�f�t�H���g");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"�����K");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"��");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"�y");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"��");

		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_SETCURSEL, 0, 0);

		return TRUE;
	case WM_COMMAND:
		mode_ = LOWORD(wp) - IDC_RADIO_UP;
		select_ = (int)SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0);
	}
	return FALSE;
	//LOWORD(wp)
}

/*
//���j���[�p�̃v�����[����
BOOL CALLBACK DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		SendMessage(GetDlgItem(hDlg, ID_MENU_SAVE), CB_GETCURSEL, ���A, ���B)
	}
	return FALSE;
}
*/

void Stage::Table_Reset()
{
	//�e�[�u����������
	for (int x = 0; x < XSIZE; x++)
	{
		for (int z = 0; z < ZSIZE; z++)
		{
			SetBlock(x, z, (BLOCKTYPE)(x % 5));
			//table_[x][z].HEGHT = 1;
			SetBlockHeght(x, z, 0);
		}
	}
}

void Stage::Save()
{
	char fileName[MAX_PATH] = "����.map";

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.map)\0*.map\0")
		TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = "map";

	//�t�@�C����ۑ��@�_�C�A���O
	BOOL selFile;
	selFile = GetSaveFileName(&ofn);

	//�L�����Z�������璆�f
	if (selFile == FALSE) return;


	/*
	HANDLE hFile;        //�t�@�C���̃n���h��
	hFile = CreateFile(
		"dataFile.txt",                 //�t�@�C����
		GENERIC_WRITE,           //�A�N�Z�X���[�h�i�������ݗp�j
		0,                      //���L�i�Ȃ��j
		NULL,                   //�Z�L�����e�B�����i�p�����Ȃ��j
		CREATE_ALWAYS,           //�쐬���@
		FILE_ATTRIBUTE_NORMAL,  //�����ƃt���O�i�ݒ�Ȃ��j
		NULL);                  //�g�������i�Ȃ��j
	//���s�����Ƃ�
	if (hFile == INVALID_HANDLE_VALUE)return;
	
	string writestr;
	writestr = ;

	DWORD dwBytes = 0;  //�������݈ʒu
	WriteFile(
		hFile,                   //�t�@�C���n���h��
		writestr,                  //�ۑ�����f�[�^�i������j
		(DWORD)strlen(writestr),   //�������ޕ�����
		&dwBytes,                //�������񂾃T�C�Y������ϐ�
		NULL);                   //�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j


	CloseHandle(hFile);
	*/
}