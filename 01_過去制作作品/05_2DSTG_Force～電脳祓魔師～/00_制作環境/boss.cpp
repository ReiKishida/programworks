//=============================================================================
//
// プレイヤー処理[2Dポリゴン] [player.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "effect.h"
#include "sound.h"
#include "force.h"
#include "followforce.h"
#include "explosion.h"
#include "fade.h"
#include "remaining.h"
#include "bg.h"
#include "boss.h"
#include "particle.h"
#include "score.h"
#include "game.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BOSS_WIDTH (50.0f)	//ポリゴンの幅
#define BOSS_HEIGHT (80.0f)	//ポリゴンのYの高さ
#define SCREEN_LIMIT_MAX_X (1050.0f - BOSS_WIDTH)	//画面端(右端)
#define SCREEN_LIMIT_MIN_X (250.0f + BOSS_WIDTH)			//画面(左端)

#define SCREEN_LIMIT_MAX_Y ((SCREEN_HEIGHT + 15.0f) - BOSS_WIDTH)	//画面(下端)
#define SCREEN_LIMIT_MIN_Y (-15.0f + BOSS_WIDTH)			//画面(上端)

#define BULLET_LIFE (70)//弾の寿命

#define TEX_X (0.5f)	//テクスチャ座標_X
#define TEX_Y (1.0f)//テクスチャ座標_Y

#define BOSS_LIFE (300)

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBoss::m_pTexture = NULL; //共有テクスチャのポインタ
//int CBoss::m_nLife = 0; //ライフ
int CBoss::m_nHackTime = 0; //ハックタイム
int CBoss::m_nMoveTime = 0;//移動継続時間
int CBoss::m_nCntWait = 0; //待機時間
int CBoss::m_MoveState = 0;	//移動状態
int CBoss::m_nCntCoolTime = 0;	//クールタイム
float CBoss::m_fAngle = 0.0f;//角度

int CBoss::m_nMovePattern = 0;//移動パターン
bool CBoss::m_bWait = false;//待機しているか
D3DXVECTOR3 CBoss::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置情報
D3DXVECTOR3 CBoss::m_posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置情報
D3DXVECTOR3 CBoss::m_Destpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置情報
D3DXVECTOR3 CBoss::m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
bool CBoss::m_bUse = true;
bool CBoss::m_bSwitch = true;
CBoss::BOSSSTATE CBoss::m_state = BOSSSTATE_NONE;

//=============================================================================
//コンストラクタ
//=============================================================================
CBoss::CBoss() : CScene2D(5)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//アニメーションカウンター
	m_nCounterAnim = 0;
	m_nTex_Y = 0;
	m_state = BOSSSTATE_NONE;
}

//=============================================================================
//デストラクタ
//=============================================================================
CBoss::~CBoss()
{

}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CBoss::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CBoss::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}



//=============================================================================
//ボスの生成
//=============================================================================
CBoss *CBoss::Create(D3DXVECTOR3 pos)
{
	//ボスのインスタンス生成
	CBoss *pBoss;
	pBoss = new CBoss;
	if (pBoss != NULL)
	{
		//ライフ
		pBoss->m_nLife = BOSS_LIFE;
		//ボスの移動量
		pBoss->m_fBossMove = 0.5f;

		//ボスの初期化処理
		pBoss->Init(pos);

	}

	return pBoss;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CBoss::Init(D3DXVECTOR3 pos)
{
	//生きている状態に
	m_bUse = true;
	m_bWait = false;
	m_bSwitch = true;
	m_state = BOSSSTATE_NORMAL;

	//アニメーションカウンター
	m_nCounterAnim = 0;
	//テクスチャの初期化
	m_nTex_Y = 0;
	//角度情報の初期化
	m_fAngle = 0.0f;
	//移動情報の初期化
	m_MoveState = 0;
	m_nMovePattern = 0;


	//ボスの幅、高さ設定
	SetScene2DWidth(BOSS_WIDTH);
	SetScene2DHeight(BOSS_HEIGHT);

	//テクスチャの割り当て
	BindTexture(m_pTexture);
	m_nLife = BOSS_LIFE;
	SetLife(m_nLife);

	//移動量初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//2Dシーンの初期化処理
	CScene2D::Init(pos);
	SetTexture(TEX_X, TEX_Y);
	//ボスのオブジェクトの設定
	SetObjType(CScene::OBJTYPE_BOSS);

	m_nCntFlame = 0;
	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CBoss::Uninit(void)
{
	//2Dシーンの終了処理
	CScene2D::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void CBoss::Update(void)
{


	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	D3DXVECTOR3 movePlayer; //ボスの移動量
	m_pos = GetPosScene2D();	//ボスの位置情報
	D3DXVECTOR3 rotPlayer = GetRotScene2D();	//ボスの角度情報

	m_nLife = GetLife();//ライフの取得
	switch (m_state)
	{
	case BOSSSTATE_NORMAL:
		switch (m_nMovePattern)
		{
		case 0:
			MovePattern0();
			break;
		case 1:
			MovePattern1();
			break;
		case 2:
			MovePattern2();
			break;
		}

		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));



		break;


	case BOSSSTATE_HACK:
		m_nHackTime--;
		SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

		if (m_nHackTime <= 0)
		{
			m_state = BOSSSTATE_NORMAL;
			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CBg::SetFade(CBg::BGTYPE_BOSSNORMAL);

			m_nHackTime = 0;
		}
		break;

	case BOSSSTATE_APPEAR:
		break;

	case BOSSSTATE_DEATH:

		break;
	}



	//位置情報を代入
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;


	//慣性移動
	m_move.x += (0.0f - m_move.x) * 0.12f;
	m_move.y += (0.0f - m_move.y) * 0.12f;

	//画面端の処理
	if (m_pos.x >= SCREEN_LIMIT_MAX_X)
	{//画面右端
		m_pos.x = SCREEN_LIMIT_MAX_X;
	}
	if (m_pos.x <= SCREEN_LIMIT_MIN_X)
	{//画面左端
		m_pos.x = SCREEN_LIMIT_MIN_X;
	}
	if (m_pos.y >= SCREEN_LIMIT_MAX_Y)
	{//画面下端
		m_pos.y = SCREEN_LIMIT_MAX_Y;
	}
	//if (m_pos.y <= SCREEN_LIMIT_MIN_Y)
	//{//画面上端
	//	m_pos.y = SCREEN_LIMIT_MIN_Y;
	//}


	//位置情報の設定
	CScene2D::SetPosScene2D(m_pos);

	//角度情報の設定
	CScene2D::SetRotScene2D(rotPlayer);

	HitDamage(m_nLife);

}

//=============================================================================
//描画処理
//=============================================================================
void CBoss::Draw(void)
{
	if (m_bUse == true)
	{
		//2Dシーンの描画処理
		CScene2D::Draw();
	}
}
//=============================================================================
//ダメージ処理
//=============================================================================
void CBoss::HitDamage(int nValue)
{
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();
	//m_nLife = GetLife();


	if (nValue <= 0)
	{
		//パーティクルの生成
		for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
		{
			CParticle::Create(m_pos, D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f), CParticle::PARITCLETYPE_EXPLOSION, 80);
		}
		for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
		{
			CParticle::Create(m_pos, D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f), CParticle::PARITCLETYPE_EXPLOSION_R, 80);
		}
		//終了処理
		Uninit();
		CScore::AddScore(5000);//スコア加算
		CGame::SetGameState(CGame::GAMESTATE_CLEAR);

	}


}


//=============================================================================
//ボスの状態取得
//=============================================================================
CBoss::BOSSSTATE CBoss::GetState(void)
{
	return m_state;
}
//=============================================================================
//ボスの状態設定
//=============================================================================
void CBoss::SetState(BOSSSTATE state)
{
	m_state = state;
}

//=============================================================================
//移動パターン(1:出現時)
//=============================================================================
void CBoss::MovePattern0(void)
{
	m_pos.y += 1.5f;

	if (m_pos.y >= 120.0f)
	{
		//MoveInit(SCREEN_WIDTH / 2 - 250.0f, 120.0f, 1);
		m_nMovePattern = 1;
	}
	m_nCntCoolTime--;
	if (m_nCntCoolTime <= 0)
	{
		m_nCntCoolTime = 0;
	}
}

//=============================================================================
//移動パターン(2:左右移動)
//=============================================================================
void CBoss::MovePattern1(void)
{
	if (m_bSwitch == true)
	{
		m_move.x = 1.5f;
	}
	if (m_bSwitch == false)
	{
		m_move.x = -1.5f;
	}

	if (m_bWait == false)
	{
		m_pos.x += m_move.x;

		if (m_pos.x >= SCREEN_LIMIT_MAX_X)
		{

			m_bSwitch = false;

		}
		else if (m_pos.x <= SCREEN_LIMIT_MIN_X)
		{
			m_bSwitch = true;
		}
	}

	if (m_nLife < 220)
	{
		//攻撃パターン2
		AttackPattern1();
	}
	else
	{
		//攻撃パターン1
		AttackPattern0();
	}

	//クールタイム減少
	m_nCntCoolTime--;
	if (m_nCntCoolTime <= 0)
	{
		m_nCntCoolTime = 0;
	}


	if (m_nLife <= 100)
	{
		MoveInit(SCREEN_WIDTH / 2 - 250.0f, 120.0f, 1);
		m_nMovePattern = 2;
	}


}

//=============================================================================
//移動パターン(3:三角移動)
//=============================================================================
void CBoss::MovePattern2(void)
{
	float fAngle;//角度計算用
	m_fAngle += 1.0;

	fAngle = sinf(m_fAngle * D3DX_PI / 180);


	float fMove_X = m_posold.x + fAngle * m_Destpos.x;
	float fMove_Y = m_posold.y + fAngle * m_Destpos.y;


	m_pos.x = fMove_X;
	m_pos.y = fMove_Y;


	if (m_fAngle == 90.0f)
	{

		if (m_MoveState == 0)
		{
			MoveInit(SCREEN_WIDTH/2 -250.0f, 120.0f, 1);
		}
		else if (m_MoveState == 1)
		{
			MoveInit(SCREEN_WIDTH / 2, 300.0f, 2);
		}
		else
		{
			MoveInit(SCREEN_WIDTH / 2 + 250.0f, 120.0f, 0);
		}
	}

	//攻撃パターン3
	AttackPattern2();

	//クールタイム減少
	m_nCntCoolTime--;
	if (m_nCntCoolTime <= 0)
	{
		m_nCntCoolTime = 0;
	}


}

//=============================================================================
//攻撃パターン(1:通常攻撃、レーザー)
//=============================================================================
void CBoss::AttackPattern0(void)
{
	D3DXVECTOR3 BulletMove;//弾の移動量
	int nAttackType;//攻撃パターン
	D3DXVECTOR3 playerPos;//プレイヤーの位置
	float fAngle;//角度情報

	if (m_nCntCoolTime <= 0)
	{
		nAttackType = rand() % 2;

		switch (nAttackType)
		{
		case 0:
			if (rand() % 50 == 0)
			{
				CPlayer *pPlayer;
				pPlayer = CManager::GetPlayer();
				playerPos = pPlayer->GetPos();

				fAngle = atan2f(playerPos.x - m_pos.x, playerPos.y - m_pos.y);

				BulletMove.x = (sinf(fAngle) * 7.0f) * -1;
				BulletMove.y = (cosf(fAngle) * 7.0f) * -1;

				CBullet::Create(m_pos, BulletMove, CBullet::BULLETTYPE_ENEMY_NORMAL, 120, 1);
				m_nCntCoolTime = 15;
			}
			break;

		case 1:
			if (rand() % 100 == 0)
			{
				CBullet::Create(m_pos, D3DXVECTOR3(4.0f, 1.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_R, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(4.0f, 2.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_R, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(-4.0f, 1.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_L, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(-4.0f, 2.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_L, 200, 1);
				m_nCntCoolTime = 180;

			}
			break;
		}
	}
}


//=============================================================================
//攻撃パターン(2:通常攻撃、拡散弾)
//=============================================================================
void CBoss::AttackPattern1(void)
{
	D3DXVECTOR3 BulletMove;//弾の移動量
	int nAttackType;//攻撃パターン
	D3DXVECTOR3 playerPos;//プレイヤーの位置
	float fAngle;//角度情報


	if (m_nCntCoolTime <= 0)
	{

			nAttackType = rand() % 2;

			switch (nAttackType)
			{
			case 0:
				if (rand() % 50 == 0)
				{
					CPlayer *pPlayer;
					pPlayer = CManager::GetPlayer();
					playerPos = pPlayer->GetPos();

					fAngle = atan2f(playerPos.x - m_pos.x, playerPos.y - m_pos.y);

					BulletMove.x = (sinf(fAngle) * 7.0f) * -1;
					BulletMove.y = (cosf(fAngle) * 7.0f) * -1;

					CBullet::Create(m_pos, BulletMove, CBullet::BULLETTYPE_ENEMY_NORMAL, 120, 1);
					m_nCntCoolTime = 15;
				}
				break;

			case 1:
				if (rand() % 150 == 0)
				{
					int nNumloop = 0;
					float fRad = atan2f(m_pos.y - playerPos.y, m_pos.x - playerPos.x);
					while (1)
					{

						float fSpeed = 7.0f;
						float fRadius = fRad + nNumloop*((360 / 20)* D3DX_PI / 180);

						BulletMove.x = sinf(fRadius) * 7.0f;
						BulletMove.y = cosf(fRadius) * 7.0f;


						CBullet::Create(m_pos, BulletMove, CBullet::BULLETTYPE_ENEMY_NORMAL, 120, 1);
						++nNumloop;
						if (nNumloop == 20)
						{
							break;
						}
					}
					m_nCntCoolTime = 180;

				}
				break;
			}

	}
}


//=============================================================================
//攻撃パターン(3:全て)
//=============================================================================
void CBoss::AttackPattern2(void)
{
	D3DXVECTOR3 BulletMove;//弾の移動量
	int nAttackType;//攻撃パターン
	D3DXVECTOR3 playerPos;//プレイヤーの位置
	float fAngle;//角度情報


	if (m_nCntCoolTime <= 0)
	{

		nAttackType = rand() % 3;

		switch (nAttackType)
		{
		case 0:
			if (rand() % 50 == 0)
			{
				CPlayer *pPlayer;
				pPlayer = CManager::GetPlayer();
				playerPos = pPlayer->GetPos();

				fAngle = atan2f(playerPos.x - m_pos.x, playerPos.y - m_pos.y);

				BulletMove.x = (sinf(fAngle) * 7.0f) * -1;
				BulletMove.y = (cosf(fAngle) * 7.0f) * -1;

				CBullet::Create(m_pos, BulletMove, CBullet::BULLETTYPE_ENEMY_NORMAL, 120, 1);
				m_nCntCoolTime = 15;
			}
			break;

		case 1:
			if (rand() % 150 == 0)
			{//拡散弾
				int nNumloop = 0;
				float fRad = atan2f(m_pos.y - playerPos.y, m_pos.x - playerPos.x);
				while (1)
				{

					float fSpeed = 7.0f;
					float fRadius = fRad + nNumloop*((360 / 20)* D3DX_PI / 180);

					BulletMove.x = sinf(fRadius) * 7.0f;
					BulletMove.y = cosf(fRadius) * 7.0f;


					CBullet::Create(m_pos, BulletMove, CBullet::BULLETTYPE_ENEMY_NORMAL, 120, 1);
					++nNumloop;
					if (nNumloop == 20)
					{
						break;
					}
				}
				m_nCntCoolTime = 180;

			}
			break;

		case 2:
			if (rand() % 100 == 0)
			{//ホーミング
				CBullet::Create(m_pos, D3DXVECTOR3(4.0f, 1.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_R, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(4.0f, 1.5f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_R, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(4.0f, 2.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_R, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(-4.0f, 1.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_L, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(-4.0f, 1.5f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_L, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(-4.0f, 2.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_L, 200, 1);
				m_nCntCoolTime = 180;

			}
			break;
		}

	}
}


//=============================================================================
//移動(目的地の設定)
//=============================================================================
void CBoss::MoveInit(float dest_x, float dest_y, int state)
{
	//過去の位置代入
	m_posold.x = m_pos.x;
	m_posold.y = m_pos.y;

	//目的の位置
	m_Destpos.x = dest_x - m_pos.x;
	m_Destpos.y = dest_y - m_pos.y;

	m_fAngle = 0;

	m_MoveState = state;
}
