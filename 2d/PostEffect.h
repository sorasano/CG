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
    //テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;
    //SRV用でスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};

