#pragma once
#include "Sprite.h"

class PostEffect :
    public Sprite
{
public:
    PostEffect();

    void Initialize(uint32_t textureNum = UINT32_MAX);
    void Draw();

    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

private:
    //��ʃN���A�J���[
    static const float clearColor[4];

private:
    //�e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texBuff;
    //SRV�p�ŃX�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //�[�x�o�b�t�@
    ComPtr<ID3D12Resource> depthBuff;
    //RTV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

};

