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

	std::unique_ptr<Model> sphereRedModel_;
	std::unique_ptr<Sphere> sphereRed_;


	//球オブジェクトのモデル初期化
	Model* newSphereModel = new Model();
	newSphereModel->Initialize(dxCommon, "sphere", "Resources/sphere/white1×1.png");
	sphereModel_.reset(newSphereModel);
	
	//球オブジェクト初期化
	Sphere* newSphere = new Sphere();
	newSphere->Initialize(dxCommon, sphereModel_.get());
	sphere_.reset(newSphere);



	//球オブジェクトのモデル初期化
	Model* newSphereRedModel = new Model();
	newSphereRedModel->Initialize(dxCommon, "sphere", "Resources/sphere/red1x1.png");
	sphereRedModel_.reset(newSphereRedModel);

	//球オブジェクト初期化
	Sphere* newRedSphere = new Sphere();
	newRedSphere->Initialize(dxCommon, sphereRedModel_.get());
	sphereRed_.reset(newRedSphere);

	//---------地面-----------

	//地面オブジェクト
	std::unique_ptr<Model> planeModel_;
	std::unique_ptr<Plane> plane_;

	//地面オブジェクトのモデル初期化
	Model* newPlaneModel = new Model();
	newPlaneModel->Initialize(dxCommon, "plane", "Resources/plane/white1×1.png");
	planeModel_.reset(newPlaneModel);

	//地面オブジェクト初期化
	Plane* newPlane = new Plane();
	newPlane->Initialize(dxCommon,planeModel_.get());
	plane_.reset(newPlane);


	//------------プレイヤー----------

	//プレイヤーのモデル
	std::unique_ptr<Model> playerModel_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	//プレイヤーのモデル初期化
	Model* newModel = new Model();
	newModel->Initialize(dxCommon, "Player", "Resources/Player/blue.png");
	playerModel_.reset(newModel);

	//プレイヤー初期化
	Player* newPlayer = new Player();
	newPlayer->Initialize(dxCommon, playerModel_.get());
	player_.reset(newPlayer);

	//当たり判定

	////球
	sphere_->sphereCol.radius = 1;
	sphere_->sphereCol.center = XMVECTOR{ sphere_->GetPosition().x, sphere_->GetPosition().y, sphere_->GetPosition().z,1 };

	////平面


	bool hit;

	//ビュー変換行列
	XMMATRIX matView;
	XMFLOAT3 eye = { -10, 1, 50 };
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
	XMFLOAT3 position_ = { 0,10,0 };

	bool state = 0;

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
			//eye.z = -10 * cosf(angle);
			matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

		}

		if (input->PushKey(DIK_W)) {
			eye.y += 0.1;
			matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		}		
		if (input->PushKey(DIK_S)) {
			eye.y -= 0.1;
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

		//自動移動
		if (state == false) {
			position_.y -= 0.1f;

			if (position_.y <= -5) {
				state = true;
			}

		}
		else {
			position_.y += 0.1f;

			if (position_.y >= +5) {
				state = false;
			}
		}

		input->Update();

		sphere_->setPosition(position_);
		sphere_->Update(matView, matProjection);

		sphereRed_->setPosition(position_);
		sphereRed_->Update(matView, matProjection);

		plane_->Update(matView, matProjection);

		sphere_->sphereCol.center = XMVECTOR{sphere_->GetPosition().x, sphere_->GetPosition().y, sphere_->GetPosition().z,1};

		hit = Collision::CheckSphere2Plane(sphere_->sphereCol,plane_->planeCol);
		
		//更新処理-ここまで

		//描画前処理
		dxCommon->PreDraw();

		if (hit) {
			sphereRed_->Draw();
		}
		else {
			sphere_->Draw();
		}

		plane_->Draw();

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
