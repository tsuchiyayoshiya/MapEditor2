#include "GameObject.h"
#include"SpereCollider.h"

GameObject::GameObject()
	:pParent_(nullptr), dead_(false)
{
}

GameObject::GameObject(GameObject* parent, const std::string& name)
	:pParent_(parent), objectName_(name), dead_(false)
{
	if (parent != nullptr)
		this->transform_.pParent_ = &(parent->transform_);
}

GameObject::~GameObject()
{
}

void GameObject::DrawSub()
{
	Draw();

	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
	{
		(*itr)->DrawSub();
	}
}

void GameObject::UpdateSub()
{
	Update();

	RoundRobin(GetRootjob());
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
	{
		(*itr)->UpdateSub();
	}

	for (auto itr = childList_.begin(); itr != childList_.end();)
	{
		if ((*itr)->dead_ == true)
		{
			(*itr)->ReleaseSub();
			SAFE_DELETE(*itr);//�������g������
			itr = childList_.erase(itr);//���X�g������폜
		}
		else
		{
			itr++;
		}
	}
}

void GameObject::ReleaseSub()
{
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
	{
		(*itr)->ReleaseSub();//*itr�̃����[�X���Ă�
		SAFE_DELETE(*itr);//*itr���̂�����
	}
	Release();
}

bool GameObject::IsDead()
{
	return false;
}

void GameObject::KillMe()
{
	dead_ = true;
}

void GameObject::SetPosition(XMFLOAT3 position)
{
	transform_.position_ = position;
}

GameObject* GameObject::FindChildObject(string _objName)
{
	if (_objName == this->objectName_)
	{
		return(this);

	}
	else
	{
		for (auto itr : childList_)
		{
			GameObject* obj = itr->FindChildObject(_objName);
				if (obj != nullptr)
				{
					return obj;
				}
		}
	}
	return nullptr;
}

GameObject* GameObject::GetRootjob()
{
	if(pParent_==nullptr)
		return this;

	return pParent_->GetRootjob();
}

GameObject* GameObject::FindObject(string _objName)
{
	return GetRootjob()->FindChildObject(_objName);
}

void GameObject::AddCollider(SpereCollider* pCollider)
{
	pCollider_ = pCollider;
}

void GameObject::Collision(GameObject* pTarget)
{
	//�������m�̓����蔻��͂��Ȃ�
	if (pTarget == this || pTarget->pCollider_ == nullptr)
	{
		return;
	}

	if (pTarget->pCollider_ == nullptr)
	{
		return;
	}
	XMVECTOR v{transform_.position_.x - pTarget->transform_.position_.x,
			   transform_.position_.y - pTarget->transform_.position_.y, 
	           transform_.position_.z - pTarget->transform_.position_.z, 
	           0};
	
	//XMVECTOR dist = XMVector3Dot(v, v);
	float dist = (transform_.position_.x - pTarget->transform_.position_.x) *
		(transform_.position_.x - pTarget->transform_.position_.x) +
		(transform_.position_.y - pTarget->transform_.position_.y) *
		(transform_.position_.y - pTarget->transform_.position_.y) +
		(transform_.position_.z - pTarget->transform_.position_.z) *
		(transform_.position_.z - pTarget->transform_.position_.z);
	float rDist = (this->pCollider_->GetRadius() + this->pCollider_->GetRadius()) * (this->pCollider_->GetRadius() + this->pCollider_->GetRadius());

	//�����ƃ^�[�Q�b�g�̋����@<= R1+R2�Ȃ�
	//�����A�����̃R���C�_�[�ƃ^�[�Q�b�g���Ԃ�������
    //onCollision(pTarget)���Ăяo��
	if (dist <= rDist)
	{
		OnCollision(pTarget);
		
	}
}

void GameObject::RoundRobin(GameObject* pTarget)
{
	if (pCollider_ == nullptr)
	{
		return;
	}
	if (pTarget->pCollider_ != nullptr)
	{
		Collision(pTarget);
	}
	for (auto itr =pTarget->childList_.begin();itr!=pTarget->childList_.end();itr++)
	{
		RoundRobin(*itr);
	}
}

std::string GameObject::GetObjectName()
{
	return objectName_;
}


