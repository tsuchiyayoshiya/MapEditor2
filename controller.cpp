#include "controller.h"
#include"SceneManager.h"
#include "Engine/Fbx.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include"Stage.h"
#include "Engine/Model.h"


int oCam = 89;
int uCam = 0;
controller::controller(GameObject* parent)
	:GameObject(parent, "controller"), hModel_(-1)
{
}

void controller::Initialize()
{
	transform_.position_.z = 7.0f;
	transform_.position_.x = 7.0f;
	transform_.rotate_.x = 45.0f;
}

void controller::Update()
{
	//戦車の現在値をベクトル型に変換
	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	//フレーム移動ベクトル
	XMVECTOR vMove = { 0.0f, 0.0f, 0.1f, 0.0f };
	//transform_.rotate_.y度回転させる行列を作成
	XMMATRIX mRotY;
	mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	//移動ベクトルを変形（戦車と同じ向きに回転）させる
	vMove = XMVector3TransformCoord(vMove, mRotY);

	if (Input::IsKey(DIK_W))
	{
		//移動
		vPos += vMove;

		//現在地をベクトルからいつものtransform_.position_に戻す
		XMStoreFloat3(&transform_.position_, vPos);
	}
	if (Input::IsKey(DIK_S))
	{
		//移動
		vPos -= vMove;

		//現在地をベクトルからいつものtransform_.position_に戻す
		XMStoreFloat3(&transform_.position_, vPos);
	}
	
	//左右移動ベクトル
	XMFLOAT3 rightMove = { 0.1f,0,0 };
	vMove = XMLoadFloat3(&rightMove);
	vMove = XMVector3TransformCoord(vMove, mRotY);
	if (Input::IsKey(DIK_A))
	{
		//移動
		vPos -= vMove;

		//現在地をベクトルからいつものtransform_.position_に戻す
		XMStoreFloat3(&transform_.position_, vPos);
	}
	if (Input::IsKey(DIK_D))
	{
		//移動
		vPos += vMove;

		//現在地をベクトルからいつものtransform_.position_に戻す
		XMStoreFloat3(&transform_.position_, vPos);
	}
	
	XMFLOAT3 camPos = transform_.position_;
	XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
 
    XMVECTOR  vCam = { 0, 0, -10, 0 };
	vCam = XMVector3TransformCoord(vCam, mRotX);
	vCam = XMVector3TransformCoord(vCam, mRotY);
	//XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, vPos + vCam);
	Camera::SetTarget(transform_.position_);
	Camera::SetPosition(camPos);
	


	if (Input::IsKey(DIK_RIGHT))
	{
		transform_.rotate_.y -= 1.0f;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		transform_.rotate_.y += 1.0f;
	}

	
	//上下
	if (Input::IsKey(DIK_UP))
	{
		
		transform_.rotate_.x += 1.5f;
		if (transform_.rotate_.x > oCam)
		{
			transform_.rotate_.x = oCam;
		}
	}
	if (Input::IsKey(DIK_DOWN))
	{
		transform_.rotate_.x -= 1.5f;
		if (transform_.rotate_.x < uCam)
		{
			transform_.rotate_.x = uCam;
		}
	}
	
}

void controller::Draw()
{

}

void controller::Release()
{
}