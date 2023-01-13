#include "PlayerBullet.h"
#include <cassert>

PlayerBullet* PlayerBullet::GetInstance()
{
	static PlayerBullet instance;
	return &instance;
}

PlayerBullet::PlayerBullet()
{
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initialize(DirectXCommon* dx, Model* model, const XMFLOAT3& position, const XMFLOAT3& rotation)
{
	assert(model);

	//ˆø”‚©‚çó‚¯æ‚Á‚½’l‚ğ•Ï”‚É‘ã“ü
	this->dx = dx;
	this->model_ = model;
	this->position_ = position;
	this->rotation_ = rotation;

	Object3D* newObject = new Object3D();
	newObject->Initialize(dx, model_);
	object3d_.reset(newObject);

	object3d_->setPosition(position_);
	object3d_->setRotation(rotation_);
	object3d_->setScale(scale_);
}

void PlayerBullet::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	//’e‚ÌˆÚ“®
	Move();

	object3d_->setPosition(position_);
	object3d_->setRotation(rotation_);
	object3d_->setScale(scale_);

	object3d_->Update(matView, matProjection);

	//ˆê’èŠÔŒo‰ß‚Å’eíœ
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Move()
{
	//’e‚ÌˆÚ“®
	position_.x += velocity_.x * speed;
	position_.y += velocity_.y * speed;
	position_.z += velocity_.z * speed;
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

void PlayerBullet::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object3d_->Draw(model_->vbView, model_->ibView);
}
