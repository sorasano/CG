#include "PostEffect.h"

PostEffect::PostEffect() : Sprite()
{
}

void PostEffect::Draw()
{
	//非表示なら処理終了
	if (isInvisible) {
		return;
	}

	//テクスチャの設定コマンド
	spriteManager->SetTextureCommand(textureIndex);

	Update();

	//頂点バッファビューの設定
	spriteManager->directX->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファビュー
	spriteManager->directX->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//描画コマンド
	spriteManager->directX->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
}
