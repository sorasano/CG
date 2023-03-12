#pragma once
#include"Base/DirectXCommon.h"
#include<DirectXMath.h>
#include<d3d12.h>
#include<wrl.h>
#include "Vector3.h"
#include"Base/Input.h"

using namespace DirectX;

class Camera
{
public:
	static ID3D12Device* device;

	static void StaticInitialize(ID3D12Device* dev);

	struct ConstBufferCamera {
		XMMATRIX view;
		XMMATRIX projection;
	};

	ConstBufferCamera* constMap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

	XMMATRIX matView;
	XMMATRIX matProjection;
	XMFLOAT3 eye;
	XMFLOAT3 target;
	XMFLOAT3 up;

public:


	void Initialize(const XMFLOAT3& eye, const XMFLOAT3& target, const XMFLOAT3& up, Input* input);

	void UpdateMatrix();

	XMMATRIX GetViewProjection()const { return matView * matProjection; }

	void Update();

	//eyeの更新
	void UpdateEye();

	//targetの更新
	void UpdateTarget();

private:

	Input* input = nullptr;

	float angle = 0.0f; // カメラの回転角

};

