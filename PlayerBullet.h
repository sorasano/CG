#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Base/DirectXCommon.h"

class PlayerBullet
{
public:
	//シングルトンインスタンス
	PlayerBullet* GetInstance();
	PlayerBullet();
	~PlayerBullet();
	void Initialize(DirectXCommon* dx, Model* model,const XMFLOAT3& position, const XMFLOAT3& rotation);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);
	void Move();
	void OnCollision();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	bool IsDead()const { return isDead_; }
	void SetIsDeadTrue() { isDead_ = true; };
	//ゲッター　セッター　
	XMFLOAT3 GetPosition() { return position_; };
	XMFLOAT3 GetRotation() { return rotation_; };
	XMFLOAT3 GetScale() { return scale_; };
private:
	DirectXCommon* dx;
	Model* model_ = nullptr;
	std::unique_ptr<Object3D> object3d_;
	//アフィン変換情報
	XMFLOAT3 scale_ = { 1,1,1 };
	XMFLOAT3 rotation_ = { 0,0,0 };
	XMFLOAT3 position_ = { 0,0,0 };

	//スピード
	XMFLOAT3 velocity_ = XMFLOAT3{0,0,1};
	float speed = 3.0f;

	//寿命
	static const int32_t lifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = lifeTime;
	//デスフラグ
	bool isDead_ = false;
};

