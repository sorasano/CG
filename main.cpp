#include <Windows.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <vector>
#include <string>

#include <DirectXMath.h>
using namespace DirectX;

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <DirectXTex.h>

#include "Base/Input.h"

#include <wrl.h>

using namespace Microsoft::WRL;

#include <iostream>
#include <memory>
using namespace std;

#include "Base/WinApp.h"
#include "Base/DirectXCommon.h"

#include "base/DirectXCommon.h"

#include "Sprite.h"
#include "object3D.h"
#include "Model.h"

#include "Player.h"

// ウィンドウプロシージャ
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}
	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}


//windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	//ポインタ
	WinApp* winApp = nullptr;
	//WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	//WindowsAPIの終了処理
	winApp->Finalize();

	//WindowsAPI解放
	//delete winApp;
	//winApp = nullptr;

	// DirectX初期化処理 ここから

	//ポインタ
	DirectXCommon* dxCommon = nullptr;

	////DirectX初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//DirextX解放
	//delete dxCommon;

	////キーボード処理

	//ポインタ
	Input* input = nullptr;

	//入力の初期化
	input = new Input();
	input->Initialize(winApp);

	////入力開放
	//delete input;

	// DirectX初期化処理 ここまで

	//描画初期化処理　ここから

	HRESULT result;
	ID3D12GraphicsCommandList* commandList;

	//3dモデル

	//プレイヤーのモデル
	std::unique_ptr<Model> playerModel_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	//オブジェクト
	std::unique_ptr<Object3D> object3Ds_;

	//プレイヤーのモデル初期化
	Model* newModel = new Model();
	newModel->Initialize(dxCommon, "Player", "Resources/Player/blue.png");
	playerModel_.reset(newModel);

	//プレイヤー初期化
	Player* newPlayer = new Player();
	newPlayer->Initialize(dxCommon, playerModel_.get());
	player_.reset(newPlayer);

	//ビュー変換行列
	XMMATRIX matView;
	XMFLOAT3 eye = { -10, 10, 30 };
	XMFLOAT3 target = { 0, 0, 0 };
	XMFLOAT3 up = { 0, 1, 0 };
	//カメラ初期化
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));


	//射影変換
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//上下画角45度
		(float)window_width / window_height,//アスペクト比(画面横幅/画面立幅)
		0.1f, 1000.0f						//前端、奥端
	);

	float angle = 0.0f; // カメラの回転角


	//アフィン変換情報
	XMFLOAT3 scale_ = { 1,1,1 };
	XMFLOAT3 rotation_ = { 0,0,0 };
	XMFLOAT3 position_ = { 0,0,0 };

	//----スプライト----

	//スプライト
	Sprite* sprite_ = new Sprite;

	//スプライト共通データ生成
	SpriteCommon spriteCommon_;

	//タイトル
	Sprite titleSprite_;

	//スプライト共通データ生成
	spriteCommon_ = sprite_->SpriteCommonCreate(dxCommon->GetDevice(), 1280, 720);

	//スプライトテクスチャ読み込み
	sprite_->SpriteCommonLoadTexture(spriteCommon_, 0, L"Resources/texture.jpg", dxCommon->GetDevice());
	sprite_->SpriteCommonLoadTexture(spriteCommon_, 1, L"Resources/texture2.jpg", dxCommon->GetDevice());
	sprite_->SpriteCommonLoadTexture(spriteCommon_, 2, L"Resources/texture3.jpg", dxCommon->GetDevice());
	sprite_->SpriteCommonLoadTexture(spriteCommon_, 3, L"Resources/testTexture.png", dxCommon->GetDevice());

	//スプライトの生成
	titleSprite_ = titleSprite_.SpriteCreate(dxCommon->GetDevice(), 1280, 720);

	//テクスチャ番号セット
	titleSprite_.SetTexNumber(3);
	//テクスチャサイズ設定
	titleSprite_.SetScale(XMFLOAT2(1280, 720));
	//反映
	titleSprite_.SpriteTransferVertexBuffer(titleSprite_);

	//スプライト用パイプライン生成呼び出し
	PipelineSet spritePipelineSet = sprite_->SpriteCreateGraphicsPipeline(dxCommon->GetDevice());

	//描画初期化処理　ここまで

	// ゲームループ
	while (true) {
		
		//Windowsのメッセージ処理
		if (winApp->processMessage()) {
			//ゲームループを抜ける
			break;
		}

		// DirectX毎フレーム処理 ここから

		//更新処理-ここから

		//射影変換

		if (input->PushKey(DIK_D) || input->PushKey(DIK_A)) {
			if (input->PushKey(DIK_D)) { angle += XMConvertToRadians(1.0f); }
			else if (input->PushKey(DIK_A)) { angle -= XMConvertToRadians(1.0f); }

			//angleラジアンだけY軸回りに回転.半径は-100
			eye.x = -10 * sinf(angle);
			eye.z = -10 * cosf(angle);
			matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

		}

		//ワールド変換

		//平行移動更新

		if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT)) {

			//座標を移動する処理(Z座標)
			if (input->PushKey(DIK_UP)) { position_.y += 0.1f; }
			else if (input->PushKey(DIK_DOWN)) { position_.y -= 0.1f; }
			if (input->PushKey(DIK_RIGHT)) { position_.x -= 0.1f; }
			else if (input->PushKey(DIK_LEFT)) { position_.x += 0.1f; }

		}
		player_->setPosition(position_);

		input->Update();
		player_->Update(matView, matProjection);

		//スプライト更新
		
		//画像変更
		titleSprite_.SetTexNumber(0);

		//スケール変更と更新
		titleSprite_.SetScale(XMFLOAT2(100, 100));
		titleSprite_.SpriteTransferVertexBuffer(titleSprite_);

		//ポジション変更と更新
		titleSprite_.SetPosition(XMFLOAT3(100, 100, 0));
		titleSprite_.SpriteUpdate(titleSprite_, spriteCommon_);

		//更新処理-ここまで

		//描画前処理
		dxCommon->PreDraw();


		player_->Draw();

		////スプライト描画
		////スプライト共通コマンド
		//sprite_->SpriteCommonBeginDraw(dxCommon->GetCommandList(), spriteCommon_);

		//titleSprite_.SpriteDraw(dxCommon->GetCommandList(), titleSprite_, spriteCommon_, dxCommon->GetDevice());

		//commandList = dxCommon->GetCommandList();


		// ４．描画コマンドここまで

		dxCommon->PostDraw();

		// DirectX毎フレーム処理 ここまで

	}

	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");

	return 0;
}
