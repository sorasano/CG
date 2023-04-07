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

#include "Sphere.h"
#include "Plane.h"

#include "Collision.h"
#include <sstream>
#include <iomanip>

static double PI = 3.14159265359;

#include "Camera.h"
#include "ParticleManager.h"

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

	//オブジェクト
	std::unique_ptr<Object3D> object3Ds_;

	//----------球----------

	//球オブジェクト
	std::unique_ptr<Model> sphereModel_;
	std::unique_ptr<Sphere> sphere_;

	//球オブジェクトのモデル初期化
	Model* newSphereModel = new Model();
	newSphereModel->Initialize(dxCommon, "sphere", "Resources/sphere/white1×1.png");
	sphereModel_.reset(newSphereModel);
	
	//球オブジェクト初期化
	Sphere* newSphere = new Sphere();
	newSphere->Initialize(dxCommon, sphereModel_.get());
	sphere_.reset(newSphere);

	std::unique_ptr<Model> sphereRedModel_;
	std::unique_ptr<Sphere> sphereRed_;

	//球オブジェクトのモデル初期化
	Model* newSphereRedModel = new Model();
	newSphereRedModel->Initialize(dxCommon, "sphere", "Resources/sphere/red1x1.png");
	sphereRedModel_.reset(newSphereRedModel);

	//球オブジェクト初期化
	Sphere* newRedSphere = new Sphere();
	newRedSphere->Initialize(dxCommon, sphereRedModel_.get());
	sphereRed_.reset(newRedSphere);

	//---------地面-----------

	////地面オブジェクト
	//std::unique_ptr<Model> planeModel_;
	//std::unique_ptr<Plane> plane_;

	////地面オブジェクトのモデル初期化
	//Model* newPlaneModel = new Model();
	//newPlaneModel->Initialize(dxCommon, "plane", "Resources/plane/white1×1.png");
	//planeModel_.reset(newPlaneModel);

	////地面オブジェクト初期化
	//Plane* newPlane = new Plane();
	//newPlane->Initialize(dxCommon,planeModel_.get());
	//plane_.reset(newPlane);


	//------------プレイヤー----------

	//プレイヤーのモデル
	//std::unique_ptr<Model> playerModel_;

	////プレイヤー
	//std::unique_ptr<Player> player_;

	////プレイヤーのモデル初期化
	//Model* newModel = new Model();
	//newModel->Initialize(dxCommon, "Player", "Resources/Player/blue.png");
	//playerModel_.reset(newModel);

	////プレイヤー初期化
	//Player* newPlayer = new Player();
	//newPlayer->Initialize(dxCommon, playerModel_.get());
	//player_.reset(newPlayer);

	//当たり判定

	////球
	sphere_->sphereCol.radius = 1;
	sphere_->sphereCol.center = XMVECTOR{ sphere_->GetPosition().x, sphere_->GetPosition().y, sphere_->GetPosition().z,1 };

	////平面


	bool hit;

	//ビュー変換行列

	XMFLOAT3 eye = { -10, 1, 50 };
	XMFLOAT3 target = { 0, 0, 0 };
	XMFLOAT3 up = { 0, 1, 0 };
	//カメラ
	//カメラ初期化
	Camera* camera{};
	camera = new Camera;
	camera->StaticInitialize(dxCommon->GetDevice());
	camera->Initialize(eye, target, up, input);


	//アフィン変換情報
	XMFLOAT3 scale_ = { 1,1,1 };
	XMFLOAT3 rotation_ = { 0,0,0 };
	XMFLOAT3 position_ = { 0,1,0 };

	bool state = 0;

	//----スプライト----


	//スプライト
	Sprite* sprite_ = new Sprite;
	//スプライト共通データ生成
	SpriteCommon spriteCommon_;

	//スプライト共通データ生成
	spriteCommon_ = sprite_->SpriteCommonCreate(dxCommon->GetDevice(), 1280, 720);

	//スプライトテクスチャ読み込み
	sprite_->SpriteCommonLoadTexture(spriteCommon_, 0, L"Resources/texture.jpg", dxCommon->GetDevice());
	sprite_->SpriteCommonLoadTexture(spriteCommon_, 1, L"Resources/reimu.png", dxCommon->GetDevice());

	//スプライトの生成
	Sprite testSprite1_;
	testSprite1_ = testSprite1_.SpriteCreate(dxCommon->GetDevice(), 64, 64);

	//テクスチャ番号セット
	testSprite1_.SetTexNumber(0);
	//テクスチャサイズ設定
	testSprite1_.SetScale(XMFLOAT2(128, 128));
	//ポジション変更と更新
	testSprite1_.SetPosition(XMFLOAT3(0, 0, 0));
	testSprite1_.SpriteUpdate(testSprite1_,spriteCommon_);
	//反映
	testSprite1_.SpriteTransferVertexBuffer(testSprite1_);


	//スプライトの生成
	Sprite testSprite2_;
	testSprite2_ = testSprite2_.SpriteCreate(dxCommon->GetDevice(),64, 64);

	//テクスチャ番号セット
	testSprite2_.SetTexNumber(1);
	//テクスチャサイズ設定
	testSprite2_.SetScale(XMFLOAT2(64, 64));
	//ポジション変更と更新
	testSprite2_.SetPosition(XMFLOAT3(600,0,0));
	testSprite2_.SpriteUpdate(testSprite2_, spriteCommon_);
	//反映
	testSprite2_.SpriteTransferVertexBuffer(testSprite2_);

	//スプライト用パイプライン生成呼び出し
	PipelineSet spritePipelineSet = sprite_->SpriteCreateGraphicsPipeline(dxCommon->GetDevice());


	//パーティクル1

	// パーティクル静的初期化
	ParticleManager::StaticInitialize(dxCommon, WinApp::winW, WinApp::winH);


	ParticleManager* particle1 = new ParticleManager();
	//パーティクル生成
	particle1->Initialize("Resources/effect1.png");

	for (int i = 0; i < 100; i++) {
		//X,Y,Zすべて[-5.0f,+5.0f]でランダムに分布
		const float md_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;

		//X,Y,Zすべて[-0.05f,+0.05f]でランダムに分布
		const float md_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;

		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		//追加
		particle1->Add(600, pos, vel, acc);

	}

	particle1->Update();


	ParticleManager* particle2 = new ParticleManager();
	//パーティクル生成
	particle2->Initialize("Resources/effect2.png");

	for (int i = 0; i < 100; i++) {
		//X,Y,Zすべて[-5.0f,+5.0f]でランダムに分布
		const float md_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;

		//X,Y,Zすべて[-0.05f,+0.05f]でランダムに分布
		const float md_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;

		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		//追加
		particle2->Add(600, pos, vel, acc);

	}

	particle2->Update();

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

		//カメラ更新
		camera->Update();

		//ワールド変換

		//平行移動更新

		if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT)) {

			//座標を移動する処理(Z座標)
			if (input->PushKey(DIK_UP)) { position_.y += 0.1f; }
			else if (input->PushKey(DIK_DOWN)) { position_.y -= 0.1f; }
			if (input->PushKey(DIK_RIGHT)) { position_.x -= 0.1f; }
			else if (input->PushKey(DIK_LEFT)) { position_.x += 0.1f; }

		}

		input->Update();

		sphere_->setPosition(position_);
		sphere_->setRotation(XMFLOAT3(0, 0, 0));
	/*	sphere_->setScale(XMFLOAT3(1, 100, 100));*/


		sphere_->Update(camera->matView, camera->matProjection);

		sphereRed_->setPosition(XMFLOAT3(position_.x + 3, position_.y, position_.z));
		sphereRed_->Update(camera->matView, camera->matProjection);

		//plane_->Update(matView, matProjection);

		sphere_->sphereCol.center = XMVECTOR{sphere_->GetPosition().x, sphere_->GetPosition().y, sphere_->GetPosition().z,1};

		//hit = Collision::CheckSphere2Plane(sphere_->sphereCol,plane_->planeCol);
		
		//パーティクル
		particle1->Update();
		particle2->Update();

		//更新処理-ここまで

		//描画前処理
		dxCommon->PreDraw();

		sphere_->Draw();
		sphereRed_->Draw();

		//plane_->Draw();

		//スプライト描画
		//スプライト共通コマンド
		sprite_->SpriteCommonBeginDraw(dxCommon->GetCommandList(), spriteCommon_);

		testSprite1_.SpriteDraw(dxCommon->GetCommandList(),testSprite1_, spriteCommon_, dxCommon->GetDevice());

		testSprite2_.SpriteDraw(dxCommon->GetCommandList(), testSprite2_, spriteCommon_, dxCommon->GetDevice());

		commandList = dxCommon->GetCommandList();

		//パーティクル
		particle1->Draw();
		particle2->Draw();

		// ４．描画コマンドここまで

		dxCommon->PostDraw();

		// DirectX毎フレーム処理 ここまで

	}

	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");

	return 0;
}
