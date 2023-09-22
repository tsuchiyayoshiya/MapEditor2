#include "TestScene.h"
#include "controller.h"
#include"SceneManager.h"
#include "Engine/Fbx.h"
#include "Engine/Input.h"
#include"Stage.h"
TestScene::TestScene(GameObject* parent)
	     :GameObject(parent, "TestScene")
{
}

void TestScene::Initialize()
{
	Instantiate<Stage>(this);
	Instantiate<controller>(this);
}

void TestScene::Update()
{
	
}

void TestScene::Draw()
{
	
}

void TestScene::Release()
{
}