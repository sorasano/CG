#include "PostEffect.h"

PostEffect::PostEffect() : Sprite()
{
}

void PostEffect::Draw()
{
	//��\���Ȃ珈���I��
	if (isInvisible) {
		return;
	}

	//�e�N�X�`���̐ݒ�R�}���h
	spriteManager->SetTextureCommand(textureIndex);

	Update();

	//���_�o�b�t�@�r���[�̐ݒ�
	spriteManager->directX->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//�萔�o�b�t�@�r���[
	spriteManager->directX->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//�`��R�}���h
	spriteManager->directX->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
}
