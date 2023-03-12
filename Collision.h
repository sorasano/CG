#pragma once

#include "CollisionPrimitive.h"

class Collision
{
public:

	//���ƕ��ʂ̓����蔻��
	static bool CheckSphere2Plane(const SphereCol& sphere,const PlaneCol&plane, DirectX::XMVECTOR*inter = nullptr);

	//�_�ƎO�p�`�̍ŋߐړ_�����߂�
	static void ClosesPtPoint2Triangle(const DirectX::XMVECTOR& point,const Triangle&triangle, DirectX::XMVECTOR* closest);

	//���Ɩ@���t���O�p�`�̓����蔻��
	static bool CheckSphere2Triangle(const SphereCol& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr);

};

