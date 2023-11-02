#include <Windows.h>
#include <stdlib.h>
#include"Engine/Direct3D.h"
#include"Engine/Camera.h"
#include"Engine/Input.h"
#include"Engine/RootJob.h"
#include"Engine/GameObject.h"
#include"Engine/Model.h"
#include<DirectXCollision.h>

#include"resource.h"
#include"Stage.h"

#pragma comment(lib, "winmm.lib")
//�萔�錾
const char* WIN_GAME_NAME = "�T���v���Q�[��";
const char* WIN_CLASS_NAME = "SampleGame";  //�E�B���h�E�N���X��
const int WINDOW_WIDTH = 800;  //�E�B���h�E�̕�
const int WINDOW_HEIGHT = 600; //�E�B���h�E�̍���
float R;

RootJob* pRootJob = nullptr;

//Stage* pStage = nullptr;

//�v���g�^�C�v�錾(�E�B���h�E�v���V�[�W��)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);


//�G���g���[�|�C���g�@�v���O�����̃X�^�[�g�n�_
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
//HINSTANCE�@�C���X�^���X�n���h��
{
	XMVECTOR beginP = XMVectorSet(1, 5, 1, 0);
	XMVECTOR dirVec = XMVectorSet(0, -1, 0, 0);
	XMVECTOR P1 = XMVectorSet(0, 0, 0, 0);
	XMVECTOR P2 = XMVectorSet(0, 0, 3, 0);
	XMVECTOR P3 = XMVectorSet(3, 0, 0, 0);
	float dist;
	bool result = TriangleTests::Intersects(beginP, dirVec, P1, P2, P3, dist);
	int a;

	HRESULT hr;
	//�E�B���h�E�N���X�i�݌v�}�j���쐬
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);             //���̍\���̂̃T�C�Y
	wc.hInstance = hInstance;                   //�C���X�^���X�n���h��
	wc.lpszClassName = WIN_CLASS_NAME;            //�E�B���h�E�N���X��
	wc.lpfnWndProc = WndProc;                   //�E�B���h�E�v���V�[�W��
	wc.style = CS_VREDRAW | CS_HREDRAW;         //�X�^�C���i�f�t�H���g�j
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //�A�C�R��
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   //�������A�C�R��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //�}�E�X�J�[�\��
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); //���j���[�Ȃ�
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH); //�w�i�i���j
	RegisterClassEx(&wc);  //�N���X��o�^

	//�E�B���h�E�T�C�Y�̌v�Z
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, TRUE);//FALSE ���j���[�̗L���@����ꍇtrue
	int winW = winRect.right - winRect.left;     //�E�B���h�E��
	int winH = winRect.bottom - winRect.top;     //�E�B���h�E����


	//�E�B���h�E���쐬
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,         //�E�B���h�E�N���X��
		WIN_GAME_NAME,     //�^�C�g���o�[�ɕ\��������e
		WS_OVERLAPPEDWINDOW, //�X�^�C���i���ʂ̃E�B���h�E�j
		CW_USEDEFAULT,       //�\���ʒu���i���܂����j
		CW_USEDEFAULT,       //�\���ʒu��i���܂����j
		winW,                 //�E�B���h�E��
		winH,                 //�E�B���h�E����
		NULL,                //�e�E�C���h�E�i�Ȃ��j
		NULL,                //���j���[�i�Ȃ��j
		hInstance,           //�C���X�^���X
		NULL                 //�p�����[�^�i�Ȃ��j
	);

	//�E�B���h�E��\��
	ShowWindow(hWnd, nCmdShow);

	//Direct3D������
	Direct3D::Initialize(winW, winH, hWnd);

	Fbx* pFbx = new Fbx;
	//pFbx->Load("Assets/BoxBrick.fbx");
	//RayCastData data;
	//data.

	Camera::Initialize();
	Camera::SetTarget(XMFLOAT3(0, 0, 0));
	//DirectInput�̏�����
	Input::Initialize(hWnd);

	//RootJob
	pRootJob = new RootJob();
	pRootJob->Initialize();
	
	//pStage
	//pStage = new Stage();
	//pStage->fLoad();
	

	HWND hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)DialogProc);
	//MAKEINTRESOURCE  IDD_DIALOG1�̔ԍ������߂�


  //���b�Z�[�W���[�v�i�����N����̂�҂j

	//int a = 0;

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//���b�Z�[�W����
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))//PeekMessage ���b�Z�[�W���W�߂�
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//���b�Z�[�W�Ȃ�
		else
		{
			timeBeginPeriod(1);//1�~���b�P�ʂ�(���x���オ��)
			static DWORD countFps = 0;

			static DWORD startTime = timeGetTime();
			DWORD nowTime = timeGetTime();

			static DWORD lastUpdateTime = nowTime;

			if (nowTime - startTime >= 1000)//if���̏������d��(���܂�1000�𒴂���)
			{
				char str[16];
				wsprintf(str, "%u", countFps);
				SetWindowText(hWnd, str);

				countFps = 0;
				startTime = nowTime;

			}

			if ((nowTime - lastUpdateTime) * 60 <= 1000)//���݂̎��Ԃ��O��̎��Ԃ��牽�b�o������
			{
				continue;
			}
			lastUpdateTime = nowTime;
			countFps++;

			timeEndPeriod(1);
			//�Q�[���̏���
			Camera::Update();


			//���͏��̍X�V
			Input::Update();
			pRootJob->UpdateSub();



			//�`�揈��
			Direct3D::BeginDraw();
			pRootJob->DrawSub();


			//�Z�[�u�f�[�^�̃��[�h
			//Stage::Load();
			 //Stage:: fLoad();

			Direct3D::EndDraw();



		}
	}
	Model::Release();
	pRootJob->ReleaseSub();


	Input::Release();
	Direct3D::Release();

	return 0;
}

//�v���V�[�W���̓N���X�̃����o�Ɋ܂܂�Ȃ�
//�E�B���h�E�v���V�[�W���i�������������ɂ�΂��֐��j
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int a = 0;

	switch (msg)
	{
	case WM_MOUSEMOVE:
		Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);  //�E�B���h�E������ꂽ��v���O�����I�� ������Ȃ����ƃv���O�������I�����Ȃ�
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_MENU_NEW:
			a++;
			break;
		case ID_MENU_OPEN:
			((Stage*)pRootJob->FindObject("Stage"))->Save();
			break;
		case ID_MENU_SAVE:
			((Stage*)pRootJob->FindObject("Stage"))->Save();
			return 0;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);//�E�B���h�E�̊g�k�Ȃǂ̃f�t�H���g�̓���
}

//�_�C�A���O�v���V�[�W��(����������������TRUE�A�������Ȃ�������FALSE��Ԃ�)
BOOL CALLBACK DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	Stage* pStage = (Stage*)pRootJob->FindObject("Stage");
	return pStage->DialogProc(hDlg, msg, wp, lp);
	//LOWORD(wp)

}