#pragma once

#include <list>
#include <string>
#include "Transform.h"
#include "Direct3D.h"
#include "SpereCollider.h"

using std::string;
using std::list;

class SphereCollider;

class GameObject
{
	bool dead_;
	int position_;
protected:
	list<GameObject*>	childList_;
	Transform			transform_;
	GameObject*         pParent_;
	string				objectName_;
	SpereCollider* pCollider_;

public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	~GameObject();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;
	
	void UpdateSub();
	void DrawSub();
	void ReleaseSub();

	bool IsDead();
	void KillMe();
	GameObject* FindChildObject(string _objName);
	GameObject* GetRootjob();
	GameObject* FindObject(string _objName);
	void AddCollider(SpereCollider* pCollider);
	void Collision(GameObject* pTarget);
	void RoundRobin(GameObject* pTarget);
	virtual void OnCollision(GameObject* pTarget){};
	std::string GetObjectName();

	template <class T>
	GameObject* Instantiate(GameObject* parent)
	{
		T* pObject;
		pObject = new T(parent);
		pObject->Initialize();
		childList_.push_back(pObject);
		return pObject;
	}
	//フラグ
	struct OBJECT_STATE
	{
		unsigned initialized : 1;	//初期化済みか
		unsigned entered : 1;		//更新するか
		unsigned visible : 1;		//描画するか
		unsigned dead : 1;	//削除するか
		
	};
	OBJECT_STATE state_;

	void SetPosition(XMFLOAT3 position);
};