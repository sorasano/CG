#include "Enemy.h"

Enemy* Enemy::GetInstance()
{
    static Enemy instance;
    return &instance;
}

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(DirectXCommon* dx, Model* model)
{
	//引数から受け取った値を代入
	this->dx_ = dx;
	this->model_ = model;

	//オブジェクト初期化
	Object3D* newObject = new Object3D();
	newObject->Initialize(dx_, model_);
	object3d_.reset(newObject);
}

void Enemy::Update(XMMATRIX& matView, XMMATRIX& matProjection, XMFLOAT3 playerPosition)
{
	Move();

	addRotation_ = atan2(playerPosition.x, playerPosition.z);
	rotation_.y = (addRotation_);

	object3d_->setScale(scale_);
	object3d_->setRotation(rotation_);
	object3d_->setPosition(position_);
	//オブジェクト更新
	object3d_->Update(matView, matProjection);
}

bool Enemy::Attack(XMFLOAT3 playerPosition)
{
	if (phase1Flag == false)
	{
		phase1Timer1_++;
		//プレイヤーの座標保存
		if (phase1Timer1_ > 200)
		{
			phase1Flag = true;
		}
	}
	if (phase1Flag == true)
	{
		phase1Timer2_++;
		if (phase1Timer2_ % 20 == 0)
		{
			return true;
		}
		if (phase1Timer2_ > 61)
		{
			phase1Timer1_ = 0;
			phase1Timer2_ = 0;
			phase1Flag = false;
		}
	}
	/*addRotation_ = atan2(playerPosition_.x, playerPosition_.z);
	rotation_.y = (addRotation_ + PI / 2);*/

    return false;
}

void Enemy::Move()
{
}

void Enemy::Draw()
{
	object3d_->Draw(model_->vbView, model_->ibView);
}

void Enemy::setPosition(XMFLOAT3 pos)
{
	position_ = pos;
}

void Enemy::setRotation(XMFLOAT3 rot)
{
	rotation_ = rot;
}

void Enemy::setScale(XMFLOAT3 sca)
{
	scale_ = sca;
}

void Enemy::SetTitle()
{
	phase1Timer1_ = 0;
	phase1Timer2_ = 0;
	phase1Flag = false;
	bulletTimer_ = 0;
}
