#include "PostEffect.h"

PostEffect::PostEffect() : Sprite()
{
}

void PostEffect::Initialize(uint32_t textureNum)
{
	HRESULT result;
	spriteManager = SpriteManager::GetInstance();
	ComPtr<ID3D12Device> dev = spriteManager->directX->GetDevice();

	//���N���X�Ƃ��Ă̏�����
	Sprite::Initialize(textureNum);

	//�e�N�X�`�����\�[�X�ݒ�
	D3D12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		window_width,
		(UINT)window_height,1,0,1,0,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	CD3DX12_HEAP_PROPERTIES texheapProps = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	//�e�N�X�`���o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&texheapProps,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff));
	assert(SUCCEEDED(result));

	{//�e�N�X�`����ԃN���A
		//��f��(1280*720 = 921600�s�N�Z��)
		const UINT pixelCount = window_width * window_height;
		//�摜1�s���̃T�C�Y
		const UINT rowPitch = sizeof(UINT) * window_width;
		//�摜�S�̂̃f�[�^�T�C�Y
		const UINT depthPitch = rowPitch * window_height;
		//�摜�C���[�W
		UINT* img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++) { img[i] = 0xff0000ff; }

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(0,nullptr,img,rowPitch,depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}

	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = dev->CreateDescriptorHeap(&srvDescHeapDesc,IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v��SRV�쐬
	dev->CreateShaderResourceView(texBuff.Get(),
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);
}

void PostEffect::Draw()
{

	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	spriteManager->directX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//��\���Ȃ珈���I��
	if (isInvisible) {
		return;
	}

	//�e�N�X�`���̐ݒ�R�}���h
	spriteManager->PostEffectSetTextureCommand(textureIndex);
	//SRV�̃n���h�������[�g�p�����[�^1�Ԃɐݒ�
	spriteManager->directX->GetCommandList()->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	Update();

	//���_�o�b�t�@�r���[�̐ݒ�
	spriteManager->directX->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//�萔�o�b�t�@�r���[
	spriteManager->directX->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//�`��R�}���h
	spriteManager->directX->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);

}
