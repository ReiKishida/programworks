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
#include "score.h"
#include "particle.h"
#include "game.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_WIDTH (50.0f)	//ポリゴンの幅
#define PLAYER_HEIGHT (50.0f)	//ポリゴンのYの高さ
#define PLAYER_MOVE (0.8f)	//プレイヤーの移動量
#define SCREEN_LIMIT_MAX_X (1070.0f - PLAYER_WIDTH)	//画面端(右端)
#define SCREEN_LIMIT_MIN_X (210.0f + PLAYER_WIDTH)			//画面(左端)

#define SCREEN_LIMIT_MAX_Y ((SCREEN_HEIGHT + 15.0f) - PLAYER_WIDTH)	//画面(下端)
#define SCREEN_LIMIT_MIN_Y (-15.0f + PLAYER_WIDTH)			//画面(上端)

#define BULLET_LIFE (70)//弾の寿命
#define BULLET_VEROCITY (18.0f)//弾の移動量
#define PATTERN_ANIM (4)	//テクスチャパターン数

#define ANIMATION_SPEED (10)	//アニメーションスピード
#define TEX_X (0.25f)	//テクスチャ座標_X
#define TEX_Y (0.34f)//テクスチャ座標_Y


//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL; //共有テクスチャのポインタ
int CPlayer::m_nLife = 0; //ライフ
int CPlayer::m_nRespawn = 0; //リスポーン時間
D3DXVECTOR3 CPlayer::m_pos = {};//位置情報
D3DXCOLOR CPlayer::m_PlayerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//プレイヤーの色情報
bool CPlayer::m_bUse = true;//使用しているか
CPlayer::PLAYERSTATE CPlayer::m_Pstate = PLAYERSTATE_NONE;//プレイヤーの状態
JoyState g_JoyStatePlayer;
//=============================================================================
//コンストラクタ
//=============================================================================
CPlayer::CPlayer() : CScene2D(6)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//アニメーションカウンター
	m_nCounterAnim = 0;
	m_nTex_Y = 0;
	m_Pstate = PLAYERSTATE_NONE;
	m_nRespawn = 120;
}

//=============================================================================
//デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CPlayer::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &m_pTexture);//プレイヤー

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CPlayer::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}



//=============================================================================
//プレイヤーの生成
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	//プレイヤーのインスタンス生成
	CPlayer *pPlayer;
	pPlayer = new CPlayer;
	if (pPlayer != NULL)
	{
		//ライフ
		pPlayer->m_nLife = 1;
		//プレイヤーの移動量
		pPlayer->m_fPlayerMove = 0.3f;

		//プレイヤーの初期化処理
		pPlayer->Init(pos);

	}

	return pPlayer;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	//生きている状態に
	m_bUse = true;
	m_Pstate = PLAYERSTATE_NORMAL;

	//アニメーションカウンター
	m_nCounterAnim = 0;
	m_nTex_Y = 0;
	m_nRespawn = 120;

	//プレイヤーの幅、高さ設定
	SetScene2DWidth(PLAYER_WIDTH);
	SetScene2DHeight(PLAYER_HEIGHT);

	//テクスチャの割り当て
	BindTexture(m_pTexture);
	SetLife(m_nLife);
	m_PlayerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//ムーブ量初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//2Dシーンの初期化処理
	CScene2D::Init(pos);
	SetTexture(TEX_X, TEX_Y);
	//プレイヤーのオブジェクトの設定
	SetObjType(CScene::OBJTYPE_PLAYER);

	//ゲームパッドの状態
	g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	m_nCntFrame = 0;
	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	//2Dシーンの終了処理
	CScene2D::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void CPlayer::Update(void)
{

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	D3DXVECTOR3 movePlayer; //プレイヤーの移動量
	m_pos = GetPosScene2D();	//プレイヤーの位置情報
	D3DXVECTOR3 rotPlayer = CScene2D::GetRotScene2D();	//プレイヤーの角度情報

	int nNumSpeedForce; //速のフォース数
	int nNumSpreadForce; //拡のフォース数
	int nNumAttackForce;//攻のフォース数
	float fAddMove;//移動量の増加量

	nNumSpeedForce = CForce::GetNumSpeed();	//速のフォース数
	nNumSpreadForce = CForce::GetNumSpread();//拡のフォース数
	nNumAttackForce = CForce::GetNumAttack();//攻のフォース数
	int nNumScene = CScene::GetNumAll();	//オブジェクトの数の取得


	switch (m_Pstate)
	{
	case PLAYERSTATE_NORMAL:
		m_PlayerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_bUse = true;

		m_nAttack = 1 + nNumAttackForce * 2;
		fAddMove = (0.17f * nNumSpeedForce); //移動量の加算量
		m_nSpread = nNumSpreadForce;

		break;


	case PLAYERSTATE_HACK:
		m_nAttack = 5;//攻撃力
		fAddMove = 0.8f; //移動量の加算量
		nNumSpreadForce = 4;//拡散数
		nNumSpeedForce = 3;//速のフォース数
		m_nRespawn--;
		CScore::AddScore(10);
		m_PlayerCol = D3DXCOLOR(0.7f, 0.0f, 1.0f, 1.0f);

		if (m_nRespawn <= 0)
		{
			m_Pstate = PLAYERSTATE_NORMAL;
			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CBg::SetFade(CBg::BGTYPE_PLAYERNORMAL);

			m_nRespawn = 0;
		}
		break;

	case PLAYERSTATE_APPEAR:
		m_PlayerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		m_bUse = true;
		m_nAttack = 1 + nNumAttackForce * 2;
		fAddMove = (0.17f * nNumSpeedForce); //移動量の加算量
		m_nSpread = nNumSpreadForce;
		m_nRespawn--;
		//if (pInputKeyboard->GetKeyboardTrigger(DIK_Y) == true)
		//{//Rキーが押されたとき
		//	CBg::SetFade(CBg::BGTYPE_HACK_PLAYER);
		//	m_nRespawn = 600;
		//}
		if (m_nRespawn <= 0)
		{
			m_Pstate = PLAYERSTATE_NORMAL;
			m_PlayerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			m_nRespawn = 0;
		}
		break;


	case PLAYERSTATE_DEATH:
		m_nRespawn--;//リスポーン時間減少
		fAddMove = 0.0f;
		if (m_nRespawn <= 0)
		{
			m_bUse = true;
			m_Pstate = PLAYERSTATE_APPEAR;
			m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 300.0f, 0.0f);
			m_PlayerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			m_nRespawn = 180;
		}
		break;
	}

	if (m_Pstate == PLAYERSTATE_APPEAR || m_Pstate == PLAYERSTATE_NORMAL || m_Pstate == PLAYERSTATE_HACK)
	{
		//===========================================================================
		//プレイヤーの移動
		//===========================================================================
		//=========================================
		//任意のキー(Aキー)が押されたかどうか    //
		//=========================================
		if (pInputKeyboard->GetKeyboardPress(DIK_A) == true || (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{//Aキーが押されたとき
			m_nTex_Y = 1;//左移動
			if (pInputKeyboard->GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//上キー(Wキー)が押された
				m_move.x += sinf(-D3DX_PI * 0.75f) * (m_fPlayerMove + fAddMove);
				m_move.y += cosf(-D3DX_PI * 0.75f) * (m_fPlayerMove + fAddMove);
			}
			else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//下キー(Sキー)が押された
				m_move.x += sinf(-D3DX_PI * 0.75f) * (m_fPlayerMove + fAddMove);
				m_move.y += cosf(-D3DX_PI * 0.25f) * (m_fPlayerMove + fAddMove);
			}
			else
			{//左キー(Aキー)のみ
				m_move.x += sinf(-D3DX_PI * 0.5f) * (m_fPlayerMove + fAddMove);
			}
		}
		//=========================================
		//任意のキー(Dキー)が押されたかどうか    //
		//=========================================
		else if (pInputKeyboard->GetKeyboardPress(DIK_D) == true || (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{
			m_nTex_Y = 2;//左移動
			if (pInputKeyboard->GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//上キー(Wキー)が押された
				m_move.x += sinf(D3DX_PI * 0.75f) * (m_fPlayerMove + fAddMove);
				m_move.y += cosf(D3DX_PI * 0.75f) * (m_fPlayerMove + fAddMove);

			}
			else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//下キー(Sキー)が押された
				m_move.x += sinf(D3DX_PI * 0.75f) * (m_fPlayerMove + fAddMove);
				m_move.y += cosf(D3DX_PI * 0.25f) * (m_fPlayerMove + fAddMove);

			}
			else
			{//右キー(Dキー)のみ
				m_move.x += sinf(D3DX_PI * 0.5f) * (m_fPlayerMove + fAddMove);
			}
		}
		//=========================================
		//任意のキー(Wキー)が押されたかどうか    //
		//=========================================
		else if (pInputKeyboard->GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{//Wキーが押されたとき
			m_nTex_Y = 0;//前移動
			m_move.y += cosf(D3DX_PI) * (m_fPlayerMove + fAddMove);
		}
		//=========================================
		//任意のキー(Sキー)が押されたかどうか    //
		//=========================================
		else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{//Sキーが押されたとき
			m_nTex_Y = 0;//後ろ移動
			m_move.y += cosf(-D3DX_PI * 0) * (m_fPlayerMove + fAddMove);
		}
		else
		{
			m_nTex_Y = 0;//後ろ移動
		}
		//========================================================================
		//弾の生成
		//========================================================================
		if (pInputKeyboard->GetKeyboardPress(DIK_SPACE) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_RIGHT_SHOULDER && pJoyState.bConnectionJoypad == true))
		{
			m_nCntFrame++;
			if (m_nCntFrame % (16 - (nNumSpeedForce * 2)) == 0)
			{//速のフォース数に応じて発射までの間隔
				pSound->PlaySound(CSound::SOUND_LABEL_SE_SHOT);

				switch (nNumSpreadForce)
				{//拡のフォース数に応じて
				case 0://拡散のフォース数:0
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					break;

				case 1://拡散のフォース数:1
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, -BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);

					break;
				case 2://拡散のフォース数:2
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(4.5f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(-4.5f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, -BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);

					break;

				case 3://拡散のフォース数:3
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(3.5f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(-3.5f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(D3DXVECTOR3(m_pos.x + 15.0f, m_pos.y, 0.0f), D3DXVECTOR3(0.0f, -BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(D3DXVECTOR3(m_pos.x - 15.0f, m_pos.y, 0.0f), D3DXVECTOR3(0.0f, -BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					break;

				case 4://拡散のフォース数:4
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, 14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(3.5f, 14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(-3.5f, 14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(8.0f, 14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(-8.0f, 14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);

					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, -14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(5.0f, -14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(-5.0f, -14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					break;
				}//(閉じ)拡のフォース数に応じて
			}//(閉じ)速のフォース数に応じて発射までの間隔
		}
	}

	if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}


	//位置情報を代入
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;


	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	//=============================================================================
	//敵との当たり判定
	//=============================================================================
	if (mode == CManager::MODE_GAME)
	{//モードがゲームなら
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
					//オブジェクトの種類の取得
					CScene::OBJTYPE type;
					type = pScene2D->GetObjType();
					if (type == CScene::OBJTYPE_ENEMY)
					{//if:オブジェクトの種類が敵なら
						if (m_pos.x >= posScene2D.x - pScene2D->GetWidthScene2D()
							&& m_pos.x <= posScene2D.x + pScene2D->GetWidthScene2D()
							&& m_pos.y >= posScene2D.y - pScene2D->GetHeightScene2D()
							&& m_pos.y <= posScene2D.y + pScene2D->GetHeightScene2D()
							)
						{//範囲内なら
							HitDamage(1);
						}//(閉じ)範囲内なら
					}//(閉じ)if:オブジェクトの種類が敵なら
				}//(閉じ)pScene2Dが空じゃなければ
			}//(閉じ)nNumScene = オブジェクトの総数
		}//(閉じ)優先順位のfor文
	} //(閉じ)モードがゲームなら

	//慣性移動
	m_move.x += (0.0f - m_move.x) * 0.12f;
	m_move.y += (0.0f - m_move.y) * 0.12f;


	//=============================================================================
	//画面端の処理
	//=============================================================================
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
	if (m_pos.y <= SCREEN_LIMIT_MIN_Y)
	{//画面上端
		m_pos.y = SCREEN_LIMIT_MIN_Y;
	}


	m_nCounterAnim++;//アニメーションカウンターの加算
	if (m_nCounterAnim % ANIMATION_SPEED == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % PATTERN_ANIM;//パターンNo.更新

		//テクスチャアニメーション
		CScene2D::SetTextureAnimY(TEX_X, TEX_Y, m_nPatternAnim, m_nTex_Y);
	}

	//位置情報の設定
	CScene2D::SetPosScene2D(m_pos);

	//角度情報の設定
	CScene2D::SetRotScene2D(rotPlayer);

	//色情報の設定
	CScene2D::SetColor(m_PlayerCol);
}

//=============================================================================
//描画処理
//=============================================================================
void CPlayer::Draw(void)
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
void CPlayer::HitDamage(int nValue)
{
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	if (m_Pstate == PLAYERSTATE_NORMAL)
	{
		//パーティクルの生成
		for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
		{
			CParticle::Create(m_pos, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), CParticle::PARITCLETYPE_EXPLOSION, 15);
			CParticle::Create(m_pos, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), CParticle::PARITCLETYPE_EXPLOSION, 15);
		}
		m_bUse = false;
		m_Pstate = PLAYERSTATE_DEATH;
		m_nRespawn = 180;
		CRemaining::DecreseRemaining(-1);
		int nRemaining = CRemaining::GetRemaining();
		if (nRemaining < 0)
		{
			if (pFade == CFade::FADE_NONE)
			{


				CGame::SetGameState(CGame::GAMESTATE_FAILED);
			}
		}

	}
}

//=============================================================================
//使用しているかを取得
//=============================================================================
bool CPlayer::GetbUse()
{
	return m_bUse;
}

//=============================================================================
//プレイヤーの状態取得
//=============================================================================
CPlayer::PLAYERSTATE CPlayer::GetState(void)
{
	return m_Pstate;
}

//=============================================================================
//プレイヤーの状態設定
//=============================================================================
void CPlayer::SetState(PLAYERSTATE state)
{
	m_Pstate = state;
}

//=============================================================================
//プレイヤーの位置取得
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}
