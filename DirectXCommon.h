#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include"WinApp.h"
#include<vector>

//DirectX���
class DirectXCommon
{
public:
	//����������
	void Initialize(WinApp* winApp);

	//�`��O����
	void PreDraw();

	//�`��㏈��
	void PostDraw();

	//�f�o�C�X�擾
	ID3D12Device* GetDevice() const { return device.Get(); }

	HRESULT GetResult() { return result.Get(); }

private:
	//�f�o�C�X�̐���
	void InitializeDevice();
	//�R�}���h�֘A�̏�����
	void InitializeCommand();
	//�X���b�v�`�F�[���̏�����
	void InitializeSwapchain();
	//�����_�[�^�[�Q�b�g�r���[�̏�����
	void InitializeRenderTargetView();
	//�[�x�o�b�t�@�̏�����
	void InitializeDepthBuffer();
	//�t�F���X�̏�����
	void InitializeFence();


private:

	HRESULT result;
	//DirextX12�f�o�C�X
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	//DXGI�t�@�N�g���[
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;

	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	D3D12_RESOURCE_BARRIER barrierDesc{};
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	//�o�b�N�o�b�t�@
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

private:
	//WindowsAPI
	WinApp* winApp_ = nullptr;

};

