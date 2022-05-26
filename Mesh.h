#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Mesh {

public:

	XMFLOAT3 pos; // xyz���W
	XMFLOAT2 uv;  // uv���W

public:

	Mesh(XMFLOAT3 pos,XMFLOAT2 uv);
	~Mesh();

	void Init(HRESULT result, ID3D12Device* device,Mesh Box[4]);
	void Update();
	void Draw();

};