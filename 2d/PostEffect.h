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
    //画面クリアカラー
    static const float clearColor[4];

private:
    //テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;
    //SRV用でスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //深度バッファ
    ComPtr<ID3D12Resource> depthBuff;
    //RTV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

};

