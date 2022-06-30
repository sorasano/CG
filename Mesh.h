#pragma once

class Mesh
{

public:

	Mesh();
	~Mesh();

	void Init(HRESULT result, ID3D12Device* device, XMFLOAT3 vertices[]);
	void Update();
	void Draw();

};

