#include "Player.h"

Player* Player::GetInstance()
{
	static Player instance;
	return &instance;
}

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(DirectXCommon* dx,Model* model)
{
	//��������󂯎�����l����
	this->dx_ = dx;
	this->model_ = model;

	//�I�u�W�F�N�g������
	Object3D* newObject = new Object3D();
	newObject->Initialize(dx_, model_);
	object3d_.reset(newObject);
}

void Player::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	//position_.x += 0.02;



	object3d_->SetPosition(position_);
	object3d_->SetRotation(rotation_);
	object3d_->SetScale(scale_);
	//�I�u�W�F�N�g�X�V
	//object3d_->Update();
}

void Player::Draw()
{
	object3d_->Draw();
}

void Player::setPosition(XMFLOAT3 pos)
{ 
	position_ = pos;
}

void Player::setRotation(XMFLOAT3 rot)
{
	rotation_ = rot;
}

void Player::setScale(XMFLOAT3 sca)
{
	scale_ = sca;
}
