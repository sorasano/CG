#include "Plane.h"

Plane* Plane::GetInstance()
{
	static Plane instance;
	return &instance;
}

Plane::Plane()
{
}

Plane::~Plane()
{
}

void Plane::Initialize(DirectXCommon* dx, Model* model)
{
	//��������󂯎�����l����
	this->dx_ = dx;
	this->model_ = model;

	//�I�u�W�F�N�g������
	Object3D* newObject = new Object3D();
	newObject->Initialize(dx_, model_);
	object3d_.reset(newObject);
}

void Plane::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	//position_.x += 0.02;

	object3d_->SetPosition(position_);
	object3d_->SetRotation(rotation_);
	object3d_->SetScale(scale_);
	//�I�u�W�F�N�g�X�V
	//object3d_->Update();
}

void Plane::Draw()
{
	object3d_->Draw();
}

void Plane::setPosition(XMFLOAT3 pos)
{
	position_ = pos;
}

void Plane::setRotation(XMFLOAT3 rot)
{
	rotation_ = rot;
}

void Plane::setScale(XMFLOAT3 sca)
{
	scale_ = sca;
}
