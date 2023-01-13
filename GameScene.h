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
	//�����o�֐�
public:
	GameScene();
	~GameScene();
	void Initialize(DirectXCommon* dxCommon, Input* input);
	void Update();
	void Draw();
	//�v���C���[�̒e�ƓG�̓����蔻��
	void EnmeyCollition();

	//�����o�ϐ�
private:
	//�f�o�C�X��input
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	//�v���C���[�̃��f��
	std::unique_ptr<Model> playerModel_;

	//�v���C���[
	std::unique_ptr<Player> player_;

	//�I�u�W�F�N�g
	std::unique_ptr<Object3D> object3Ds_;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> playerBullet_;

	//�e�̃��f��
	std::unique_ptr<Model> bulletModel_;

	//�G�̃��f��
	std::unique_ptr<Model> enemyModel_;

	//�G
	std::unique_ptr<Enemy> enemy_;


	//�X�v���C�g
	Sprite* sprite_ = new Sprite;

	//�X�v���C�g���ʃf�[�^����
	SpriteCommon spriteCommon_;

	//�^�C�g��
	Sprite titleSprite_;

	//�N���A���
	Sprite clearSprite_;

	Sprite hpSprite_[20];

	//�ˉe�ϊ�
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//�㉺��p45�x
		(float)window_width / window_height,//�A�X�y�N�g��(��ʉ���/��ʗ���)
		0.1f, 1000.0f						//�O�[�A���[
	);

	//�r���[�ϊ��s��
	XMMATRIX matView;
	XMFLOAT3 eye = { 0, 20, -20 };
	XMFLOAT3 target = { 0, 10, 0 };
	XMFLOAT3 up = { 0, 1, 0 };

	float angle = 0.0f; // �J�����̉�]�p


	//�v���C���[�A�t�B���ϊ����
	XMFLOAT3 pScale_ = { 1,1,1 };
	XMFLOAT3 pRotation_ = { 0,0,0 };
	XMFLOAT3 pPosition_ = { 0,0,0 };

	float playerSpeed = 0.3;

	//�G�A�t�B���ϊ����
	XMFLOAT3 eScale_ = { 1,1,1 };
	XMFLOAT3 eRotation_ = { 0,0,0 };
	XMFLOAT3 ePosition_ = { 0,0,50 };

	float enemySpeed = 0.3;

	bool eStateX = 0;
	bool eStateY = 0;

	int eHp = 20;

	//�G�ړ�����
	float eMaxX = 30;
	float eMaxY = 15;

	//�V�[�� 0�^�C�g��1�v���C2�N���A
	int scene = 0;

	int keyCoolTimer = 0;
};
