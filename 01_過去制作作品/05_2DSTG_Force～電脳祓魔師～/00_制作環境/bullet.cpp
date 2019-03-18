//=============================================================================
//
// 弾の処理[2Dポリゴン] [bullet.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "Bullet.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"
#include "score.h"
#include "force.h"
#include "polygon.h"
#include "game.h"
#include "boss.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STAGE_LIMIT_MAX_X (1040)	//ステージの左端
#define STAGE_LIMIT_MIN_X (240)		//ステージの右端
#define STAGE_LIMIT_MAX_Y (720)		//ステージの下端
#define STAGE_LIMIT_MIN_Y (0)		//ステージの上端

#define BULLET_WIDTH (10.0f)		//弾の幅
#define BULLET_HEIGHT (10.0f)		//弾の高さ

#define LIMIT_HORMING (85)			//ホーミング時間
//*****************************************************************************
// グローバル変数
//*****************************************************************************
bool g_bDeleteBullet = false;

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//=============================================================================
//コンストラクタ
//=============================================================================
CBullet::CBullet() : CScene2D(5)
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CBullet::~CBullet()
{

}
//=============================================================================
//テクスチャのロード
//=============================================================================
HRESULT CBullet::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &m_pTexture);

	return S_OK;

}
//=============================================================================
//テクスチャの破棄
//=============================================================================
void CBullet::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
//弾の生成
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type, int nLife, int nAttack)
{
	//弾の生成
	CBullet *pBullet;
	pBullet = new CBullet;





	if (pBullet != NULL)
	{
		//弾の種類
		pBullet->m_type = type;
		//弾の移動量設定
		pBullet->m_BulletMove = move;
		pBullet->m_nCntFrame = 0;
		pBullet->m_nLife = nLife;
		pBullet->m_nAttack = nAttack;
		//弾の初期化
		pBullet->Init(pos);

	}

	return pBullet;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos)
{


	//テクスチャの割り当て
	BindTexture(m_pTexture);

	//ポリゴンの幅、高さ設定
	SetScene2DWidth(BULLET_WIDTH);
	SetScene2DHeight(BULLET_HEIGHT);
	m_nCntFrame = 0;
	//2Dシーンの初期化処理
	CScene2D::Init(pos);

	//弾のオブジェクトの設定
	SetObjType(CScene::OBJTYPE_BULLET);

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	//2Dシーンの終了処理
	CScene2D::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void CBullet::Update(void)
{
	CPlayer *pPlayer;//プレイヤーのポインタ
	pPlayer = CGame::GetPlayer();//プレイヤーの取得
	D3DXVECTOR3 playerPos;//プレイヤーの位置
	playerPos = pPlayer->GetPos();//プレイヤーの位置酒取得

	CBoss *pBoss;
	pBoss = CGame::GetBoss();


	//弾の情報
	D3DXVECTOR3 posBullet = GetPosScene2D();	//弾の位置情報
	D3DXVECTOR3 rotBullet = GetRotScene2D();	//弾の角度情報
	D3DXVECTOR3 DestPos;	//目的の位置
	D3DXVECTOR3 BulletMove;	//弾の移動量
	DestPos = playerPos - posBullet;

	float fAngle;
	float fShotAngle = D3DX_PI + (D3DX_PI / 2);
	int nNumScene = CScene::GetNumAll();	//オブジェクトの数の取得

	//フォースの数の取得
	int nNumAttackForce = CForce::GetNumAttack();
	int nNumSpreadForce = CForce::GetNumSpread();
	int nNumSpeedForce = CForce::GetNumSpeed();
	int nNumDefenseForce = CForce::GetNumDefense();

	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	//プレイヤーの状態
	CPlayer::PLAYERSTATE state = CPlayer::GetState();


	m_nCntFrame++;

	switch (m_type)
	{
	case BULLETTYPE_PLAYER:
		if (state == CPlayer::PLAYERSTATE_NORMAL || state == CPlayer::PLAYERSTATE_APPEAR)
		{
			CEffect::Create(posBullet, D3DXCOLOR((0.25f * nNumAttackForce) + (0.25f * nNumDefenseForce), (0.25f * nNumSpreadForce) + (0.15f * nNumDefenseForce), (0.25f * nNumSpeedForce), 1.0f), 20.0f, 20.0f, 15, CEffect::EFFECTTYPE_BULLET);
		}
		else if (state == CPlayer::PLAYERSTATE_HACK)
		{
			CEffect::Create(posBullet, D3DXCOLOR(((rand() % 10) * 0.1f), ((rand() % 10) * 0.1f), ((rand() % 10) * 0.1f), 1.0f), ((rand() % 20 + 10) * 1.0f), ((rand() % 20 + 10) * 1.0f), 10, CEffect::EFFECTTYPE_BULLET);
		}
		break;
	case BULLETTYPE_ENEMY_NORMAL:
		CEffect::Create(posBullet, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f), 20.0f, 20.0f, 15, CEffect::EFFECTTYPE_BULLET);
		break;
	case BULLETTYPE_ENEMY_BOMB:
		CEffect::Create(posBullet, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20.0f, 20.0f, 20, CEffect::EFFECTTYPE_BULLET);
		break;
	case BULLETTYPE_ENEMY_LASER:
		CEffect::Create(posBullet, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 15.0f, 15.0f, 40, CEffect::EFFECTTYPE_BULLET);



		break;
	case BULLETTYPE_ENEMY_LASER_L:
		CEffect::Create(posBullet, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 15.0f, 15.0f, 120, CEffect::EFFECTTYPE_BULLET);
		if (m_nLife > LIMIT_HORMING)
		{
			if (m_nCntFrame % 5 == 0)
			{
				DestPos = playerPos - posBullet;//目的の位置

				fAngle = atan2f(DestPos.x, DestPos.y);//角度
				//一定量の角度を超えたら
				if (fAngle > D3DX_PI)
				{
					fAngle -= D3DX_PI * 2;
				}
				if (fAngle < -D3DX_PI)
				{
					fAngle += D3DX_PI * 2;
				}
				//移動量の設定
				BulletMove.x = sinf(fAngle);
				BulletMove.y = cosf(fAngle);
				BulletMove.z = 0.0f;

				//位置情報更新
				m_BulletMove.x += BulletMove.x * -1.0f;
				m_BulletMove.y -= BulletMove.y * 0.6f;
			}
		}
		break;
	case BULLETTYPE_ENEMY_LASER_R:
		CEffect::Create(posBullet, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 15.0f, 15.0f, 120, CEffect::EFFECTTYPE_BULLET);
		if (m_nLife > LIMIT_HORMING)
		{
			if (m_nCntFrame % 5 == 0)
			{
				DestPos = playerPos - posBullet;//目的の位置

				fAngle = atan2f(DestPos.x, DestPos.y);
				if (fAngle > D3DX_PI)
				{
					fAngle -= D3DX_PI * 2;
				}
				if (fAngle < -D3DX_PI)
				{
					fAngle += D3DX_PI * 2;
				}
				BulletMove.x = sinf(fAngle);
				BulletMove.y = cosf(fAngle);
				BulletMove.z = 0.0f;

				//位置情報更新
				m_BulletMove.x -= BulletMove.x * 1.0f;
				m_BulletMove.y -= BulletMove.y * 0.6f;
			}
		}
		break;

	}


	//位置情報を更新
	posBullet.y -= m_BulletMove.y;
	posBullet.x -= m_BulletMove.x;



	//位置情報の設定
	CScene2D::SetPosScene2D(posBullet);

	//角度情報の設定
	CScene2D::SetRotScene2D(rotBullet);



	//ライフの減少
	m_nLife--;
	if (m_nLife <= 0)
	{
		if (m_type == BULLETTYPE_ENEMY_BOMB)
		{
			Create(posBullet, D3DXVECTOR3(0.0f, 5.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);
			Create(posBullet, D3DXVECTOR3(4.0f, 5.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);
			Create(posBullet, D3DXVECTOR3(-4.0f, 5.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);

			Create(posBullet, D3DXVECTOR3(4.0f, 0.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);
			Create(posBullet, D3DXVECTOR3(-4.0f, 0.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);


			Create(posBullet, D3DXVECTOR3(0.0f, -5.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);
			Create(posBullet, D3DXVECTOR3(4.0f, -5.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);
			Create(posBullet, D3DXVECTOR3(-4.0f, -5.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);


			//爆発の生成
			CExplosion::Create(posBullet, 50.0f);
			//終了処理
			Uninit();

		}
		else
		{
			//終了処理
			Uninit();

			//爆発の生成
			CExplosion::Create(posBullet, 50.0f);
		}
	}
	else if (posBullet.y <= STAGE_LIMIT_MIN_Y || posBullet.y >= STAGE_LIMIT_MAX_Y
		|| posBullet.x <= STAGE_LIMIT_MIN_X || posBullet.x >= STAGE_LIMIT_MAX_X)
	{
		//終了処理
		Uninit();
	}
	else
	{///////////////////////////////////当たり判定///////////////////////////////////
		for (int nCntPriority = 0; nCntPriority < PRIORITY_NUMBER; nCntPriority++)
		{//優先順位のfor文
			for (int nCntScene = 0; nCntScene < nNumScene; nCntScene++)
			{//nNumScene = オブジェクトの総数
			 //シーンの取得
				CScene2D *pScene2D;
				pScene2D = (CScene2D*)GetScene(nCntPriority, nCntScene);

				if (pScene2D != NULL)
				{//pScene2Dが空じゃなければ
				 //ポリゴンの情報
					D3DXVECTOR3 posScene2D = pScene2D->GetPosScene2D();	//敵の位置情報

					int nForceType = pScene2D->GetForce();

					//オブジェクトの種類の取得
					CScene::OBJTYPE type;
					type = pScene2D->GetObjType();

					if (m_type == BULLETTYPE_PLAYER)
					{//プレイヤーの弾なら
						if (type == CScene::OBJTYPE_ENEMY)
						{//if:オブジェクトの種類が敵なら
							if (posBullet.x >= posScene2D.x - pScene2D->GetWidthScene2D()
								&& posBullet.x <= posScene2D.x + pScene2D->GetWidthScene2D()
								&& posBullet.y >= posScene2D.y - pScene2D->GetHeightScene2D()
								&& posBullet.y <= posScene2D.y + pScene2D->GetHeightScene2D()
								)
							{//範囲内なら
								pScene2D->HitLife(m_nAttack);
								//pEnemy->HitEnemy(m_nAttack);
								//スコア加算
								CScore::AddScore(100);

								//終了処理
								Uninit();
								break;
							}//(閉じ)if:範囲内なら
						}//(閉じ)if:オブジェクトの種類が敵なら
						if (type == CScene::OBJTYPE_BOSS)
						{//if:オブジェクトの種類が敵なら
							if (posBullet.x >= posScene2D.x - pScene2D->GetWidthScene2D()
								&& posBullet.x <= posScene2D.x + pScene2D->GetWidthScene2D()
								&& posBullet.y >= posScene2D.y - pScene2D->GetHeightScene2D()
								&& posBullet.y <= posScene2D.y + pScene2D->GetHeightScene2D()
								)
							{//範囲内なら
								//pBoss->HitDamage(m_nAttack);
								pScene2D->HitLife(m_nAttack);

								//スコア加算
								CScore::AddScore(100);

								//終了処理
								Uninit();
								break;
							}//(閉じ)if:範囲内なら
						}//(閉じ)if:オブジェクトの種類が敵なら
					}//(閉じ)if:プレイヤーの弾なら
					if (m_type == BULLETTYPE_ENEMY_NORMAL	||
						m_type == BULLETTYPE_ENEMY_BOMB		||
						m_type == BULLETTYPE_ENEMY_HORMING	||
						m_type == BULLETTYPE_ENEMY_LASER_R	||
						m_type == BULLETTYPE_ENEMY_LASER_L)
					{//敵の弾なら
						if (type == CScene::OBJTYPE_PLAYER)
						{//オブジェクトの種類がプレイヤーなら
							CPlayer::PLAYERSTATE state = CPlayer::GetState();
							if (state == CPlayer::PLAYERSTATE_NORMAL)
							{//プレイヤーの状態がノーマルなら
								if (posBullet.x >= (posScene2D.x - pScene2D->GetWidthScene2D()) + 2.0f
									&& posBullet.x <= (posScene2D.x + pScene2D->GetWidthScene2D()) -2.0f
									&& posBullet.y >= (posScene2D.y - pScene2D->GetHeightScene2D())+ 2.0f
									&& posBullet.y <= (posScene2D.y + pScene2D->GetHeightScene2D()) -2.0f
									)
								{//if:範囲内なら
								 //爆発の生成
									//CExplosion::Create(posBullet, 50.0f);
									//終了処理
									Uninit();
									pPlayer->HitDamage(1);
									break;
								}//(閉じ)if:範囲内なら
							}//(閉じ)プレイヤーの状態がノーマルなら
						}//(閉じ)if:オブジェクトの種類がプレイヤーなら
						if (type == CScene::OBJTYPE_FORCE)
						{//オブジェクトの種類がフォースなら
							if (nForceType == 3)
							{//フォースタイプが「護」なら
								if (posBullet.x >= posScene2D.x - pScene2D->GetWidthScene2D()
									&& posBullet.x <= posScene2D.x + pScene2D->GetWidthScene2D()
									&& posBullet.y >= posScene2D.y - pScene2D->GetHeightScene2D()
									&& posBullet.y <= posScene2D.y + pScene2D->GetHeightScene2D()
									)
								{//範囲内なら
								 //終了処理
									Uninit();
									break;
								}//(閉じ)範囲内なら
							}//(閉じ)フォースタイプが「護」なら
						}//(閉じ)オブジェクトの種類がフォースなら
					}//(閉じ)弾が敵なら
				}//(閉じ)nullじゃないとき
			}//(閉じ)シーンのfor文
		}//(閉じ)優先順位のfor文
	}//(閉じ)else
}

//=============================================================================
//描画処理
//=============================================================================
void CBullet::Draw(void)
{
	//2Dシーンの描画処理
	CScene2D::Draw();
}
