#pragma once
#include "Sprite.h"

class PostEffect :
    public Sprite
{
public:
    PostEffect();

    void Initialize(uint32_t textureNum = UINT32_MAX);
    void Draw();

private:
    //�e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texBuff;
    //SRV�p�ŃX�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};

