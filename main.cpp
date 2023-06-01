#include <string>
#include <sstream>
#include <iomanip>
#include "GameScene.h"
#include "PostEffect.h"

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

	// DirectX初期化処理
	//ポインタ
	DirectXCommon* dxCommon = nullptr;
	////DirectX初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	////キーボード処理
	//ポインタ
	Input* input = nullptr;
	//入力の初期化
	input = new Input();
	input->Initialize(winApp);

	//テクスチャマネージャーの初期化
	Texture::Initialize(dxCommon->GetDevice());
	//スプライト共通部の初期化
	SpriteManager* spriteManager = nullptr;
	spriteManager = SpriteManager::GetInstance();
	spriteManager->Initialize(dxCommon, WinApp::winW, WinApp::winH);

	//fbxLoadr汎用初期化
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

	//ゲームシーン
	GameScene* gameScene = nullptr;
	gameScene = new GameScene();
	gameScene->Initialize(dxCommon, input);

	//ポストエフェクト
	PostEffect* postEffect = nullptr;
	//ポストエフェクト用テクスチャ読み込み
	uint32_t postEffectTexture = Texture::LoadTexture(L"Resources/white1x1.png");
	//ポストエフェクト初期化
	postEffect = new PostEffect();
	postEffect->Initialize(postEffectTexture);
	postEffect->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	postEffect->SetPos(XMFLOAT2(WinApp::winW / 2, WinApp::winH / 2));
	postEffect->SetSize(XMFLOAT2(100,100));
	postEffect->Update();

	//描画初期化処理　ここまで

	// ゲームループ
	while (true) {

		//Windowsのメッセージ処理
		if (winApp->processMessage()) {
			//ゲームループを抜ける
			break;
		}

		//キー
		input->Update();

		//更新
		gameScene->Update();

		//描画前処理
		dxCommon->PreDraw();

		//ポストエフェクト
		SpriteManager::GetInstance()->beginDraw();
		postEffect->Draw();

		//ゲームシーン描画
		//gameScene->Draw();

		//描画後処理
		dxCommon->PostDraw();

	}

	//WindowsAPI解放
	delete winApp;
	winApp = nullptr;

	//DirextX解放
	delete dxCommon;

	//入力開放
	delete input;

	//gamescene解放
	delete gameScene;

	//ポストエフェクト解放
	delete postEffect;

	//fbxLoader解放
	FbxLoader::GetInstance()->Finalize();

	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");

	return 0;
}
