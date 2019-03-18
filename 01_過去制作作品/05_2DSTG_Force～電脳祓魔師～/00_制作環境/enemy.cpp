//=============================================================================
//
// 敵処理[2Dポリゴン] [enemy.cpp]
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
#include "enemy.h"
#include "fade.h"
#include "bg.h"
#include "game.h"
#include "particle.h"
#include "score.h"
#include "sound.h"
#include "timer.h"
#include "log.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STAGE_LIMIT_MAX_X (1040)	//ステージの左端
#define STAGE_LIMIT_MIN_X (240)		//ステージの右端

#define ENEMY_WIDTH (40.0f)			//ポリゴンの幅
#define ENEMY_HEIGHT (45.0f)		//ポリゴンのYの高さ
#define ENEMY_MOVE (1.5f)			//敵の移動量
#define NORMAL_ENEMY_HP (15)		//ノーマル型のHP
#define ATTACK_ENEMY_HP	(25)		//攻撃型の敵のHP
#define SPREAD_ENEMY_HP (25)		//拡散型の敵のHP
#define SPEED_ENEMY_HP (10)			//速度型の敵のHP
#define DEFENSE_ENEMY_HP (80)		//護の敵のHP

#define ENEMY_LENGTH_LONG_R (700.0f)//長さ
#define ENEMY_LENGTH_MIDIUM_R (550.0f)//長さ

#define ENEMY_LENGTH_LONG_L (620.0f)//長さ
#define ENEMY_LENGTH_MIDIUM_L (450.0f)//長さ


#define THROUGH_MOVE_X (3.0f)		//通過速度(X)
#define THROUGH_MOVE_UPPER (1.3f)		//通過速度(上)
#define THROUGH_MOVE_UNDER (1.2f)		//通過速度(下)
#define THROUGH_MOVE_UNDER_2 (1.5f)		//通過速度(下)
#define THROUGH_MOVE_Y_2 (2.0f)		//通過速度(Y)
#define	FAN_MOVE (0.0045f)			//旋回速度
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[MAX_ENEMYTYPE] = {}; //共有テクスチャのポインタ
int  CEnemy::m_nNumEnemy = 0;

//=============================================================================
//コンストラクタ
//=============================================================================
CEnemy::CEnemy() : CScene2D(5)
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CEnemy::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy.png", &m_pTexture[0]);//ノーマルな敵
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_attack.png", &m_pTexture[1]);//攻撃型の敵
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_spread.png", &m_pTexture[2]);//拡散型の敵
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_speed.png", &m_pTexture[3]);//速度型の敵
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_defense.png", &m_pTexture[4]);//防御型の敵

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CEnemy::Unload(void)
{
	for (int nCntEnemyTex = 0; nCntEnemyTex < MAX_ENEMYTYPE; nCntEnemyTex++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCntEnemyTex] != NULL)
		{
			m_pTexture[nCntEnemyTex]->Release();
			m_pTexture[nCntEnemyTex] = NULL;
		}
	}
}




//=============================================================================
//敵の生成
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, ENEMYTYPE type, ENEMYMOVE movetype)
{
	//敵のインスタンス生成
	CEnemy *pEnemy;
	pEnemy = new CEnemy;
	if (pEnemy != NULL)
	{
		pEnemy->m_type = type;
		pEnemy->m_movetype = movetype;
		//ライフの設定
		switch (pEnemy->m_type)
		{
		case ENEMYTYPE_NORMAL:
			pEnemy->m_nLife = NORMAL_ENEMY_HP;
			break;
		case ENEMYTYPE_ATTACK:
			pEnemy->m_nLife = ATTACK_ENEMY_HP;
			break;
		case ENEMYTYPE_SPREAD:
			pEnemy->m_nLife = SPREAD_ENEMY_HP;
			break;
		case ENEMYTYPE_SPEED:
			pEnemy->m_nLife = SPEED_ENEMY_HP;
			break;
		case ENEMYTYPE_DEFENSE:
			pEnemy->m_nLife = DEFENSE_ENEMY_HP;
			break;
		}
		//敵の初期化処理
		pEnemy->Init(pos);
		m_nNumEnemy++;
	}
	return pEnemy;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	//敵の幅、高さ設定
	SetScene2DWidth(ENEMY_WIDTH);
	SetScene2DHeight(ENEMY_HEIGHT);
	//ライフの設定
	SetLife(m_nLife);



	//状態
	m_state = ENEMYSTATE_NORMAL;

	//テクスチャの割り当て
	switch (m_type)
	{
	case ENEMYTYPE_NORMAL:
		BindTexture(m_pTexture[0]);
		break;
	case ENEMYTYPE_ATTACK:
		BindTexture(m_pTexture[1]);
		break;
	case ENEMYTYPE_SPREAD:
		BindTexture(m_pTexture[2]);
		break;
	case ENEMYTYPE_SPEED:
		BindTexture(m_pTexture[3]);
		break;
	case ENEMYTYPE_DEFENSE:
		BindTexture(m_pTexture[4]);
		break;
	}

	//移動量の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//フレーム数のカウント
	m_nCntFrame = 0;

	//2Dシーンの初期化処理
	CScene2D::Init(pos);

	//敵のオブジェクトの設定
	SetObjType(CScene::OBJTYPE_ENEMY);

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	//2Dシーンの終了処理
	CScene2D::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void CEnemy::Update(void)
{

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	m_pos =GetPosScene2D();	//敵の位置情報
	D3DXVECTOR3 rotEnemy = GetRotScene2D();	//敵の角度情報
	D3DXVECTOR3 BulletMove;//弾の移動量
	D3DXVECTOR3 playerPos;//プレイヤーの位置
	CPlayer *pPlayer;//プレイヤー
	pPlayer = CManager::GetPlayer();//プレイヤーの取得
	playerPos = pPlayer->GetPos();//プレイヤーの位置酒取得
	D3DXVECTOR3 posFan = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 50.0f, 0.0f);


	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	//ライフの取得
	m_nLife = GetLife();

	//角度情報
	float fAngle;

	switch (m_state)
	{

	case ENEMYSTATE_NORMAL:
		//============================================================================================
		//										攻 撃 パ タ ー ン									//
		//============================================================================================
#if 1
		switch (m_type)
		{
		case ENEMYTYPE_NONE:
			break;
		case ENEMYTYPE_NORMAL:
			if (mode == CManager::MODE_GAME)
			{
				if (rand() % 80 == 0)
				{
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, -7.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 110, 1);
					m_nCoolDown = 50;
					m_state = ENEMYSTATE_COOLDOWN;
				}
			}
			break;
		case ENEMYTYPE_ATTACK:
			if (rand() % 100 == 0)
			{
				CPlayer *pPlayer;
				pPlayer = CManager::GetPlayer();
				playerPos = pPlayer->GetPos();

				fAngle = atan2f(playerPos.x - m_pos.x, playerPos.y - m_pos.y);

				BulletMove.x = (sinf(fAngle) * 7.0f) * -1;
				BulletMove.y = (cosf(fAngle) * 7.0f) * -1;

				CBullet::Create(m_pos, BulletMove, CBullet::BULLETTYPE_ENEMY_BOMB, 45, 1);
				m_nCoolDown = 85;
				m_state = ENEMYSTATE_COOLDOWN;
			}
			break;
		case ENEMYTYPE_SPREAD:

			if (rand() % 100 == 0)
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
				m_nCoolDown = 100;
				m_state = ENEMYSTATE_COOLDOWN;

			}
			break;
		case ENEMYTYPE_SPEED:
			if (rand() % 150 == 0)
			{
				CBullet::Create(m_pos, D3DXVECTOR3(4.0f, 1.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_R, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(4.0f, 2.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_R, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(-4.0f, 1.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_L, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(-4.0f, 2.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_L, 200, 1);
				m_nCoolDown = 80;
				m_state = ENEMYSTATE_COOLDOWN;

			}

			break;
		case ENEMYTYPE_DEFENSE:
			if (rand() % 200 == 0)
			{
				CBullet::Create(m_pos, D3DXVECTOR3(0.0f, -9.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 60, 1);
				m_nCoolDown = 75;
				m_state = ENEMYSTATE_COOLDOWN;
			}
			break;
		}//(閉じ)攻撃パターンswitch分
#endif
		break;

	case ENEMYSTATE_COOLDOWN:
		m_nCoolDown--;
		if (m_nCoolDown <= 0)
		{
			m_state = ENEMYSTATE_NORMAL;
		}
		break;
	}

	//============================================================================================
	//										移 動 パ タ ー ン									//
	//============================================================================================
	switch (m_movetype)
	{
	case ENEMYMOVE_NONE:
		break;
	case ENEMYMOVE_THROUGH_UPPER:
		m_move.y = THROUGH_MOVE_UPPER;//下へ移動
		break;
	case ENEMYMOVE_THROUGH_UNDER:
		m_move.y = THROUGH_MOVE_UNDER;//下へ移動
		break;
	case ENEMYMOVE_FAN_L_LONG://左回転
		//角度加算
		m_rot.x += FAN_MOVE;
		m_rot.y += FAN_MOVE;

		m_pos = (D3DXVECTOR3(sinf(m_rot.x + D3DX_PI - (D3DX_PI * 0.5f)) * ENEMY_LENGTH_LONG_L + posFan.x
			, cosf(m_rot.y + D3DX_PI - (D3DX_PI *  0.5f)) * ENEMY_LENGTH_LONG_L + posFan.y
			, posFan.z));
		break;

	case ENEMYMOVE_FAN_L_MIDIUM://左回転
							  //角度加算
		m_rot.x += FAN_MOVE;
		m_rot.y += FAN_MOVE;

		m_pos = (D3DXVECTOR3(sinf(m_rot.x + D3DX_PI - (D3DX_PI * 0.5f)) * ENEMY_LENGTH_MIDIUM_L + posFan.x
			, cosf(m_rot.y + D3DX_PI - (D3DX_PI *  0.5f)) * ENEMY_LENGTH_MIDIUM_L + posFan.y
			, posFan.z));
		break;

	case ENEMYMOVE_FAN_R_LONG://右回転
		//角度加算
		m_rot.x -= FAN_MOVE;
		m_rot.y -= FAN_MOVE;

		m_pos = (D3DXVECTOR3(sinf(m_rot.x + D3DX_PI - (D3DX_PI * -0.5f)) * ENEMY_LENGTH_LONG_R + posFan.x
			, cosf(m_rot.y + D3DX_PI - (D3DX_PI *  -0.5f)) * ENEMY_LENGTH_LONG_R + posFan.y
			, posFan.z));
		break;

	case ENEMYMOVE_FAN_R_MIDIUM://右回転
		 //角度加算
		m_rot.x -= FAN_MOVE;
		m_rot.y -= FAN_MOVE;

		m_pos = (D3DXVECTOR3(sinf(m_rot.x + D3DX_PI - (D3DX_PI * -0.5f)) * ENEMY_LENGTH_MIDIUM_R + posFan.x
			, cosf(m_rot.y + D3DX_PI - (D3DX_PI *  -0.5f)) * ENEMY_LENGTH_MIDIUM_R + posFan.y
			, posFan.z));
		break;

	case ENEMYMOVE_STOP_R://画面中央から右にはける
		m_move.y = THROUGH_MOVE_UNDER_2;//下へ移動
		if (m_pos.y > SCREEN_HEIGHT / 2 - 150.0f)
		{
			m_move.y = 0.7f;//下へ移動
			m_move.x = THROUGH_MOVE_X;//右へ移動
		}
		break;

	case ENEMYMOVE_STOP_L://画面中央から左にはける
		m_move.y = THROUGH_MOVE_UNDER_2;//下へ移動
		if (m_pos.y > SCREEN_HEIGHT / 2 - 150.0f)
		{
			m_move.y = 0.7f;//下へ移動
			m_move.x = (THROUGH_MOVE_X * -1);//右へ移動
		}
		break;

	case ENEMYMOVE_STOP:
		m_move.y = THROUGH_MOVE_UNDER_2;//下へ移動
		if (m_pos.y > SCREEN_HEIGHT / 2 - 200.0f)
		{
			m_nCntFrame++;
			if (m_nCntFrame >= 240)
			{
				m_move.y = THROUGH_MOVE_UNDER_2;//下へ移動
			}
			else
			{
				m_move.y = 0.0f;//下へ移動
			}
		}
		break;
	}

	//移動量の代入
	m_pos += m_move;

	//位置情報の設定
	SetPosScene2D(m_pos);
	//角度情報の設定
	SetRotScene2D(rotEnemy);

	//ライフの確認
	CheckLife(m_nLife);


	//============================================================================================
	//										範囲を超えたら										//
	//============================================================================================
	switch (m_movetype)
	{
	case ENEMYMOVE_THROUGH_UPPER:
		if (m_pos.y <= 0)
		{
			Uninit();
		}
		break;
	case ENEMYMOVE_THROUGH_UNDER:
		if (m_pos.y >= SCREEN_HEIGHT + 50.0f)
		{
			Uninit();
		}

		break;
	case ENEMYMOVE_FAN_L_LONG://左回転
		if (m_pos.x <= STAGE_LIMIT_MIN_X - 50.0f)
		{
			Uninit();
		}
		break;

	case ENEMYMOVE_FAN_L_MIDIUM://左回転
		if (m_pos.x <= STAGE_LIMIT_MIN_X - 50.0f)
		{
			Uninit();
		}
		break;

	case ENEMYMOVE_FAN_R_LONG://右回転
		if (m_pos.x >= STAGE_LIMIT_MAX_X + 50.0f)
		{
			Uninit();
		}
		break;

	case ENEMYMOVE_FAN_R_MIDIUM://右回転
		if (m_pos.x >= STAGE_LIMIT_MAX_X + 50.0f)
		{
			Uninit();
		}
		break;

	case ENEMYMOVE_STOP_R:
		if (m_pos.x >= STAGE_LIMIT_MAX_X + 40.0f)
		{
			Uninit();
		}
		else if (m_pos.y >= SCREEN_HEIGHT + 50.0f)
		{
			Uninit();
		}

		break;
	case ENEMYMOVE_STOP_L:

		if (m_pos.x <= STAGE_LIMIT_MIN_X -40.0f)
		{
			Uninit();
		}
		else if (m_pos.y >= SCREEN_HEIGHT + 50.0f)
		{
			Uninit();
		}

		break;
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	//2Dシーンの描画処理
	CScene2D::Draw();
}



//=============================================================================
//位置情報の取得
//=============================================================================
D3DXVECTOR3 CEnemy::GetPosEnemy(void)
{
	return m_pos;
}

//=============================================================================
//ライフの確認
//=============================================================================
void CEnemy::CheckLife(int nLife)
{
	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	if (m_nLife <= 0)
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

		switch (m_type)
		{
		case ENEMYTYPE_NORMAL:
			CScore::AddScore(500);//スコア加算
			CTimer::AddSeconds(3);//秒数加算
   			CLog::Create(D3DXVECTOR3(SCREEN_WIDTH  + 150.0f, SCREEN_HEIGHT / 2 - 120.0f, 0.0f), CLog::LOGTYPE_NORMAL);
			break;
		case ENEMYTYPE_ATTACK:
			CScore::AddScore(600);//スコア加算
			CTimer::AddSeconds(4);//秒数加算
			CLog::Create(D3DXVECTOR3(SCREEN_WIDTH + 150.0f, SCREEN_HEIGHT / 2 - 120.0f, 0.0f), CLog::LOGTYPE_ATTACK);
			break;
		case ENEMYTYPE_SPREAD:
			CScore::AddScore(700);//スコア加算
			CTimer::AddSeconds(4);//秒数加算
			CLog::Create(D3DXVECTOR3(SCREEN_WIDTH + 150.0f, SCREEN_HEIGHT / 2 - 120.0f, 0.0f), CLog::LOGTYPE_SPREAD);

			break;
		case ENEMYTYPE_SPEED:
			CScore::AddScore(700);//スコア加算
			CTimer::AddSeconds(4);//秒数加算
			CLog::Create(D3DXVECTOR3(SCREEN_WIDTH + 150.0f, SCREEN_HEIGHT / 2 - 120.0f, 0.0f), CLog::LOGTYPE_SPEED);

			break;
		case ENEMYTYPE_DEFENSE:
			CScore::AddScore(1000);//スコア加算
			CTimer::AddSeconds(5);//秒数加
			CLog::Create(D3DXVECTOR3(SCREEN_WIDTH + 150.0f, SCREEN_HEIGHT / 2 - 120.0f, 0.0f), CLog::LOGTYPE_DEFENSE);

			break;
		}
		//終了処理
		Uninit();


		pSound->PlaySound(CSound::SOUND_LABEL_SE_ADDTIME);
	}
}

