#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Base/DirectXCommon.h"
#define PI 3.14159265359

class Enemy
{
public:
	//�V���O���g���C���X�^���X
	Enemy* GetInstance();
	Enemy();
	~Enemy();
	void Initialize(DirectXCommon* dx, Model* model);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection,XMFLOAT3 playerPosition);
	bool Attack(XMFLOAT3 playerPosition);
	void Move();
	void Draw();
	//�Q�b�^�[�@�Z�b�^�[�@
	XMFLOAT3 GetPosition() { return position_; };
	XMFLOAT3 GetRotation() { return rotation_; };
	XMFLOAT3 GetScale() { return scale_; };
	float GetHp() { return hp_; };
	void SetHp(float hp) { hp_ = hp; };
	void setPosition(XMFLOAT3 pos);
	void setRotation(XMFLOAT3 rot);
	void setScale(XMFLOAT3 sca);
	void HpReset() { hp_ = 150; };
	void SetTitle();
private:
	DirectXCommon* dx_ = nullptr;

	Model* model_ = nullptr;
	std::unique_ptr<Object3D> object3d_;
	//�A�t�B���ϊ����
	XMFLOAT3 scale_ = { 1,1,1};
	XMFLOAT3 rotation_ = { 180,0,0 };
	XMFLOAT3 position_ = { 0,0,50 };

	//phase1�̒e�̃^�C�}�[
	int phase1Timer1_ = 0;
	int phase1Timer2_ = 0;
	bool phase1Flag = false;
	//�v���C���[�̈ʒu��ۑ����Ă����p�̕ϐ�
	XMFLOAT3 playerPosition_;
	float addRotation_;

	//�e�̃^�C�}-
	int bulletTimer_ = 0;

	//�̗�
	float hp_ = 20;
};

