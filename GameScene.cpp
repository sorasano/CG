#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	this->dxCommon_ = dxCommon;
	this->input_ = input;

	//�v���C���[�̃��f��������
	Model* newModel = new Model();
	newModel->Initialize(dxCommon_, "Player", "Resources/Player/blue.png");
	playerModel_.reset(newModel);

	//�v���C���[������
	Player* newPlayer = new Player();
	newPlayer->Initialize(dxCommon, playerModel_.get());
	player_.reset(newPlayer);

	//�e�̃��f���̏�����
	Model* newBulletModel = new Model();
	newBulletModel->Initialize(dxCommon_, "bullet", "Resources/bullet/black.png");
	bulletModel_.reset(newBulletModel);

	//�G�̃��f��������
	Model* newEnemyModel = new Model();
	newEnemyModel->Initialize(dxCommon_, "Player", "Resources/Player/red.png");
	enemyModel_.reset(newEnemyModel);

	//�G������
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(dxCommon_, enemyModel_.get());
	enemy_.reset(newEnemy);

	//�J����������
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));


	//----�X�v���C�g----

	//�X�v���C�g���ʃf�[�^����
	spriteCommon_ = sprite_->SpriteCommonCreate(dxCommon_->GetDevice(), 1280, 720);

	//�X�v���C�g�e�N�X�`���ǂݍ���
	sprite_->SpriteCommonLoadTexture(spriteCommon_, 0, L"Resources/title.png", dxCommon_->GetDevice());
	sprite_->SpriteCommonLoadTexture(spriteCommon_, 1, L"Resources/clear.png", dxCommon_->GetDevice());
	sprite_->SpriteCommonLoadTexture(spriteCommon_, 2, L"Resources/player/red.png", dxCommon_->GetDevice());

	//�X�v���C�g�̐���
	titleSprite_ = titleSprite_.SpriteCreate(dxCommon_->GetDevice(), 1280, 720);

	//�e�N�X�`���ԍ��Z�b�g
	titleSprite_.SetTexNumber(0);
	//�e�N�X�`���T�C�Y�ݒ�
	titleSprite_.SetScale(XMFLOAT2(1280, 720));
	//���f
	titleSprite_.SpriteTransferVertexBuffer(titleSprite_);

	//�X�v���C�g�̐���
	clearSprite_ = clearSprite_.SpriteCreate(dxCommon_->GetDevice(), 1280, 720);

	//�e�N�X�`���ԍ��Z�b�g
	clearSprite_.SetTexNumber(1);
	//�e�N�X�`���T�C�Y�ݒ�
	clearSprite_.SetScale(XMFLOAT2(1280, 720));
	//���f
	clearSprite_.SpriteTransferVertexBuffer(clearSprite_);

	for (int i = 0; i < 20; i++) {

		hpSprite_[i].SetTexNumber(2);

		//�X�v���C�g�̐���
		hpSprite_[i] = hpSprite_[i].SpriteCreate(dxCommon_->GetDevice(), 32, 32);

		//�e�N�X�`���ԍ��Z�b�g
		hpSprite_[i].SetTexNumber(2);
		//�e�N�X�`���T�C�Y�ݒ�
		hpSprite_[i].SetScale(XMFLOAT2(32, 32));
		//���f
		hpSprite_[i].SpriteTransferVertexBuffer(hpSprite_[i]);

		//�|�W�V�����ύX�ƍX�V
		hpSprite_[i].SetPosition(XMFLOAT3(0 + (64 * i), 16, 0));
		hpSprite_[i].SpriteUpdate(hpSprite_[i], spriteCommon_);
	}

	//�X�v���C�g�p�p�C�v���C�������Ăяo��
	PipelineSet spritePipelineSet = sprite_->SpriteCreateGraphicsPipeline(dxCommon_->GetDevice());

}

void GameScene::Update()
{

	//---�V�[���؂�ւ�---

	keyCoolTimer--;

	if (scene == 0 && keyCoolTimer <= 0) {
		if (input_->PushKey(DIK_SPACE)) {
			scene = 1;
		}
	}
	else if (scene == 1) {

		if (enemy_->GetHp() <= 0) {
			scene = 2;
			keyCoolTimer = 50;
		}

	}
	else if (scene == 2 && keyCoolTimer <= 0) {
		if (input_->PushKey(DIK_SPACE)) {
			scene = 0;
			keyCoolTimer = 50;
			enemy_->SetHp(20);
			pPosition_ = XMFLOAT3(0, 0, 0);
			ePosition_ = XMFLOAT3(0, 0, 50);

			//eye = { 0, 20, -20 };
			//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

		}
	}

	eHp = enemy_->GetHp();

	//----�J����----

	//if (input_->PushKey(DIK_RIGHT) || input_->PushKey(DIK_LEFT)) {
	//	if (input_->PushKey(DIK_RIGHT)) { angle += XMConvertToRadians(1.0f); }
	//	else if (input_->PushKey(DIK_LEFT)) { angle -= XMConvertToRadians(1.0f); }

	//	//angle���W�A������Y�����ɉ�].���a��-100
	//	eye.x = -10 * sinf(angle);
	//	eye.z = -10 * cosf(angle) - 20;
	//	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//}

	//------���@-----

	//���[���h�ϊ�

	//���s�ړ��X�V

	if (input_->PushKey(DIK_W) || input_->PushKey(DIK_S) || input_->PushKey(DIK_D) || input_->PushKey(DIK_A)) {

		//���W���ړ����鏈��(Z���W)
		if (input_->PushKey(DIK_W)) { pPosition_.y += playerSpeed; }
		else if (input_->PushKey(DIK_DOWN) || input_->PushKey(DIK_S)) { pPosition_.y -= playerSpeed; }
		if (input_->PushKey(DIK_RIGHT) || input_->PushKey(DIK_D)) { pPosition_.x += playerSpeed; }
		else if (input_->PushKey(DIK_LEFT) || input_->PushKey(DIK_A)) { pPosition_.x -= playerSpeed; }

	}

	player_->setPosition(pPosition_);

	//----�e----

	//�v���C���[����󂯎�����t���O�Œe����
	if (input_->PushKey(DIK_SPACE))
	{
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(dxCommon_, bulletModel_.get(), player_->GetPosition(), player_->GetRotation());
		playerBullet_.push_back(std::move(newBullet));
	}
	//�v���C���[�̒e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullet_)
	{
		bullet->Update(matView, matProjection);
	}
	//�f�X�t���O�̗������e���폜
	playerBullet_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{return bullet->IsDead(); }
	);

	//-----�G-----

	//���W���ړ����鏈��

	if (eStateX) { ePosition_.x += enemySpeed; }
	else { ePosition_.x -= enemySpeed; }

	if (eStateY) { ePosition_.y += enemySpeed; }
	else { ePosition_.y -= enemySpeed; }

	if (ePosition_.x >= eMaxX) {
		eStateX = 0;
	}
	else if (ePosition_.x <= -eMaxX) {
		eStateX = 1;
	}

	if (ePosition_.y >= eMaxY) {
		eStateY = 0;
	}
	else if (ePosition_.y <= -eMaxY) {
		eStateY = 1;
	}

	enemy_->setPosition(ePosition_);

	//�G�����蔻��
	EnmeyCollition();

	input_->Update();
	player_->Update(matView, matProjection);
	enemy_->Update(matView, matProjection, player_->GetPosition());

}

void GameScene::Draw()
{
	player_->Draw();
	enemy_->Draw();

	//�v���C���[�̒e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullet_)
	{
		bullet->Draw(dxCommon_->GetCommandList());
	}
	//�X�v���C�g���ʃR�}���h
	sprite_->SpriteCommonBeginDraw(dxCommon_->GetCommandList(), spriteCommon_);

	if (scene == 0) {
		titleSprite_.SpriteDraw(dxCommon_->GetCommandList(), titleSprite_, spriteCommon_, dxCommon_->GetDevice());
	}
	else if (scene == 1) {

		for (int i = 0; i < eHp; i++) {
			hpSprite_[i].SpriteDraw(dxCommon_->GetCommandList(), hpSprite_[i], spriteCommon_, dxCommon_->GetDevice());
		}

	}
	else if (scene == 2) {
		clearSprite_.SpriteDraw(dxCommon_->GetCommandList(), clearSprite_, spriteCommon_, dxCommon_->GetDevice());
	}

}

void GameScene::EnmeyCollition()
{
	//�G�ɒe������������e���� �GHp�ւ炷
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullet_)
	{

		XMFLOAT3 ePos = enemy_->GetPosition();

		float eSize = 2;

		float ePosX1 = ePos.x - (eSize / 2);
		float ePosX2 = ePos.x + (eSize / 2);

		float ePosY1 = ePos.y - (eSize / 2);
		float ePosY2 = ePos.y + (eSize / 2);

		float ePosZ1 = ePos.z - (eSize / 2);
		float ePosZ2 = ePos.z + (eSize / 2);

		XMFLOAT3 bPos = bullet->GetPosition();

		float bSize = 1;

		float bPosX1 = bPos.x - (bSize / 2);
		float bPosX2 = bPos.x + (bSize / 2);

		float bPosY1 = bPos.y - (bSize / 2);
		float bPosY2 = bPos.y + (bSize / 2);

		float bPosZ1 = bPos.z - (bSize / 2);
		float bPosZ2 = bPos.z + (bSize / 2);


		if (ePosX1 < bPosX2 && bPosX1 < ePosX2) {
			if (ePosY1 < bPosY2 && bPosY1 < ePosY2) {
				if (ePosZ1 < bPosZ2 && bPosZ1 < ePosZ2) {

					enemy_->SetHp(enemy_->GetHp() - 1);
					bullet->SetIsDeadTrue();

				}
			}
		}

	}
}