#include "transform.h"

Transform::Transform():
    matTranslate_(XMMatrixIdentity()),
    matRotate_(XMMatrixIdentity()),
    matScale_(XMMatrixIdentity()),
    position_(XMFLOAT3(0, 0, 0)),
    rotate_(XMFLOAT3(0, 0, 0)),
    scale_(XMFLOAT3(1, 1, 1))
{

}

Transform::~Transform()
{

}

void Transform::Calclation()
{
	//各行列の計算
    matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);

    //回転
    XMMATRIX rotateX, rotateY, rotateZ;
    rotateX = XMMatrixRotationX(XMConvertToRadians(rotate_.x));
    rotateY = XMMatrixRotationY(XMConvertToRadians(rotate_.y));
    rotateZ = XMMatrixRotationZ(XMConvertToRadians(rotate_.z));
    matRotate_ = rotateX * rotateY * rotateZ;

    //拡大縮小
    matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
}

XMMATRIX Transform::GetWorldMatrix()
{
    if (pParent_ != nullptr)
    {  
        //親があったら
        return matScale_ * matRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
    }
    else  
    {
        //ワールド行列を所得
        return matScale_ * matRotate_ * matTranslate_;
    }
}

XMMATRIX Transform::GetNormalMatrix()
{
    return matRotate_ * XMMatrixInverse(nullptr, matScale_);
}
