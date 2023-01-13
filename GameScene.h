#include "Base/DirectXCommon.h"
#include "Base/input.h"
#include "DirectXTex.h"
#include "object3D.h"
#include "list"
#include "memory"
#include "Model.h"
#include "Object3D.h"
#include "Player.h"
#include "Sprite.h"
#include "PlayerBullet.h"
#include "Enemy.h"

class GameScene
{
	//メンバ関数
public:
	GameScene();
	~GameScene();
	void Initialize(DirectXCommon* dxCommon, Input* input);
	void Update();
	void Draw();
	//プレイヤーの弾と敵の当たり判定
	void EnmeyCollition();

	//メンバ変数
private:
	//デバイスとinput
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	//プレイヤーのモデル
	std::unique_ptr<Model> playerModel_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	//オブジェクト
	std::unique_ptr<Object3D> object3Ds_;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> playerBullet_;

	//弾のモデル
	std::unique_ptr<Model> bulletModel_;

	//敵のモデル
	std::unique_ptr<Model> enemyModel_;

	//敵
	std::unique_ptr<Enemy> enemy_;


	//スプライト
	Sprite* sprite_ = new Sprite;

	//スプライト共通データ生成
	SpriteCommon spriteCommon_;

	//タイトル
	Sprite titleSprite_;

	//クリア画面
	Sprite clearSprite_;

	Sprite hpSprite_[20];

	//射影変換
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//上下画角45度
		(float)window_width / window_height,//アスペクト比(画面横幅/画面立幅)
		0.1f, 1000.0f						//前端、奥端
	);

	//ビュー変換行列
	XMMATRIX matView;
	XMFLOAT3 eye = { 0, 20, -20 };
	XMFLOAT3 target = { 0, 10, 0 };
	XMFLOAT3 up = { 0, 1, 0 };

	float angle = 0.0f; // カメラの回転角


	//プレイヤーアフィン変換情報
	XMFLOAT3 pScale_ = { 1,1,1 };
	XMFLOAT3 pRotation_ = { 0,0,0 };
	XMFLOAT3 pPosition_ = { 0,0,0 };

	float playerSpeed = 0.3;

	//敵アフィン変換情報
	XMFLOAT3 eScale_ = { 1,1,1 };
	XMFLOAT3 eRotation_ = { 0,0,0 };
	XMFLOAT3 ePosition_ = { 0,0,50 };

	float enemySpeed = 0.3;

	bool eStateX = 0;
	bool eStateY = 0;

	int eHp = 20;

	//敵移動制限
	float eMaxX = 30;
	float eMaxY = 15;

	//シーン 0タイトル1プレイ2クリア
	int scene = 0;

	int keyCoolTimer = 0;
};
