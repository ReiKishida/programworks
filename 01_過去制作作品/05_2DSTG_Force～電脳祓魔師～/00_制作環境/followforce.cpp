//=============================================================================
//
// UIの処理[2Dポリゴン] [ui.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"
#include "score.h"
#include "number.h"
#include "followforce.h"
#include "polygon.h"
#include "input.h"
#include "effect.h"
#include "game.h"
#include "tutorial.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FORCE_WIDTH (150)	//ポリゴンの幅
#define FORCE_HEIGHT (150)	//ポリゴンのYの高さ
#define FORCE_RADIUS (67.0f)//フォースの半径
#define FORCE_ROTATE (0.1f)//フォースの回転量
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CFollowForce::m_apTexture[MAX_FOLLOWFORCE] = {}; //共有テクスチャのポインタ
CScene2D *CFollowForce::m_apPolygon[MAX_FOLLOWFORCE] = {};	//ポリゴンのポインタ
int CFollowForce::m_nNumAttack = 0;	//攻のフォース数
int CFollowForce::m_nNumSpread = 0;	//拡のフォース数
int CFollowForce::m_nNumSpeed = 0;	//速のフォース数
int CFollowForce::m_nNumDefense = 0;	//護のフォース数
bool CFollowForce::m_bUse = true;//使用しているか
JoyState g_JoyStateFollowForce;


//=============================================================================
//コンストラクタ
//=============================================================================
CFollowForce::CFollowForce() : CScene(5)
{//クリアな値を代入
	m_nCnt = 0;
	m_ForceRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCntNumber = 0; nCntNumber < MAX_FOLLOWFORCE; nCntNumber++)
	{
		fLengthForce[nCntNumber] = 0.0f;
	}
	fAngleForce = 0.0f;
	m_bUse = true;
}
//=============================================================================
//デストラクタ
//=============================================================================
CFollowForce::~CFollowForce()
{


}
//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CFollowForce::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/atk.png", &m_apTexture[FOLLOWFORCETYPE_ATTACK]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spr.png", &m_apTexture[FOLLOWFORCETYPE_SPREAD]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spd.png", &m_apTexture[FOLLOWFORCETYPE_SPEED]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/def.png", &m_apTexture[FOLLOWFORCETYPE_DEFENSE]);

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CFollowForce::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_FOLLOWFORCE; nCntTex++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}



//=============================================================================
//スコアの生成
//=============================================================================
CFollowForce *CFollowForce::Create(D3DXVECTOR3 pos)
{
	//スコアの生成
	CFollowForce *pScore;
	pScore = new CFollowForce;

	if (pScore != NULL)
	{
		//スコアの初期化処理
		pScore->Init(pos);
	}

	return pScore;



}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CFollowForce::Init(D3DXVECTOR3 pos)
{
	m_nCnt = 0;
	m_ForceRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bUse = true;
	//ゲームパッドの状態
	g_JoyStateFollowForce.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	for (int nCntScene2D = 0; nCntScene2D < MAX_FOLLOWFORCE; nCntScene2D++)
	{
		m_aForceStruct[nCntScene2D].type = FOLLOWFORCETYPE_NONE;

		m_aForceStruct[nCntScene2D].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//2Dポリゴンの生成
		m_apPolygon[nCntScene2D] = new CScene2D;
		if (m_apPolygon[nCntScene2D] != NULL)
		{
			//背景の幅、高さ設定
			m_apPolygon[nCntScene2D]->SetScene2DWidth(34.0f);
			m_apPolygon[nCntScene2D]->SetScene2DHeight(34.0f);


			//テクスチャの割り当て
			m_apPolygon[nCntScene2D]->BindTexture(m_apTexture[FOLLOWFORCETYPE_ATTACK]);

			//初期化処理
			m_apPolygon[nCntScene2D]->Init(D3DXVECTOR3(sinf(m_ForceRot.x + D3DX_PI - ((D3DX_PI * 2) / MAX_FOLLOWFORCE) * nCntScene2D) * FORCE_RADIUS + pos.x
												     , cosf(m_ForceRot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_FOLLOWFORCE) * nCntScene2D) * FORCE_RADIUS + pos.y
													 , pos.z));

			//テクスチャ座標設定
			m_apPolygon[nCntScene2D]->SetTexture(1.0f, 1.0f);

			//フォースの設定
			SetForce(nCntScene2D, FOLLOWFORCETYPE_ATTACK);

			//オブジェクトのタイプ設定
			m_apPolygon[nCntScene2D]->SetObjType(CScene::OBJTYPE_FORCE);


		}
	}
	return S_OK;
}


//=============================================================================
//終了処理
//=============================================================================
void CFollowForce::Uninit(void)
{



	for (int nCntNumber = 0; nCntNumber < MAX_FOLLOWFORCE; nCntNumber++)
	{
		if (m_apPolygon[nCntNumber] != NULL)
		{
			//2Dシーンの終了処理
			m_apPolygon[nCntNumber]->Uninit();
			m_apPolygon[nCntNumber] = NULL;
		}
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CFollowForce::Update(void)
{
	CManager::MODE mode;
	mode = CManager::GetMode();

#if 1
	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();



	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);




	//プレイヤーの取得
	CPlayer *pPlayer;
	D3DXVECTOR3 PosPlayer;
	if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		PosPlayer = pPlayer->GetPosScene2D();

	}
	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		PosPlayer = pPlayer->GetPosScene2D();
	}

	D3DXVECTOR3 posForce[MAX_FOLLOWFORCE];
	m_bUse = CPlayer::GetbUse();
	CPlayer::PLAYERSTATE PlayerState = CPlayer::GetState();

	//エフェクトの色設定
	for (int nCntForce = 0; nCntForce < MAX_FOLLOWFORCE; nCntForce++)
	{
		switch (m_aForceStruct[nCntForce].type)
		{
		case FOLLOWFORCETYPE_ATTACK:
			m_aForceStruct[nCntForce].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		case FOLLOWFORCETYPE_SPREAD:
			m_aForceStruct[nCntForce].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			break;
		case FOLLOWFORCETYPE_SPEED:
			m_aForceStruct[nCntForce].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		case FOLLOWFORCETYPE_DEFENSE:
			m_aForceStruct[nCntForce].col = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f);
			break;
		}

		//フォースの位置取得
		posForce[nCntForce] = m_apPolygon[nCntForce]->GetPosScene2D();

		if (PlayerState == CPlayer::PLAYERSTATE_APPEAR || PlayerState == CPlayer::PLAYERSTATE_NORMAL)
		{
			//エフェクトの生成
			CEffect::Create(posForce[nCntForce], m_aForceStruct[nCntForce].col, 24.0f, 24.0f, 7, CEffect::EFFECTTYPE_BULLET);
		}
		else if (PlayerState == CPlayer::PLAYERSTATE_HACK)
		{
			//エフェクトの生成
			CEffect::Create(posForce[0], D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 24.0f, 24.0f, 7, CEffect::EFFECTTYPE_BULLET);
			CEffect::Create(posForce[1], D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 24.0f, 24.0f, 7, CEffect::EFFECTTYPE_BULLET);
			CEffect::Create(posForce[2], D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 24.0f, 24.0f, 7, CEffect::EFFECTTYPE_BULLET);
			CEffect::Create(posForce[3], D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f), 24.0f, 24.0f, 7, CEffect::EFFECTTYPE_BULLET);

		}
		if (m_bUse == true)
		{
			//プレイヤーの位置代入
			posForce[nCntForce] = (D3DXVECTOR3(sinf(m_ForceRot.x + D3DX_PI - ((D3DX_PI * 2) / MAX_FOLLOWFORCE) * nCntForce) * FORCE_RADIUS + PosPlayer.x
				, cosf(m_ForceRot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_FOLLOWFORCE) * nCntForce) * FORCE_RADIUS + PosPlayer.y
				, PosPlayer.z));
		}
		else
		{
			posForce[nCntForce] = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);
		}
	}
	float fForceRotate = 0.1f + (0.014f * m_nNumDefense);
	//フォースの回転
	m_ForceRot.x += fForceRotate;
	m_ForceRot.y += fForceRotate;

	//カウントのリセット
	m_nCnt = m_nCnt % MAX_FOLLOWFORCE;
#if 1
	if (pInputKeyboard->GetKeyboardTrigger(DIK_1) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_B && pJoyState.bConnectionJoypad == true))
	{//攻のフォースの設置
		if (g_JoyStateFollowForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			SetForce(m_nCnt, FOLLOWFORCETYPE_ATTACK);
			m_nCnt++;
		}
		g_JoyStateFollowForce.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_2) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_A && pJoyState.bConnectionJoypad == true))
	{//拡のフォース設置
		if (g_JoyStateFollowForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			SetForce(m_nCnt, FOLLOWFORCETYPE_SPREAD);
			m_nCnt++;
		}
		g_JoyStateFollowForce.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_3) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_X && pJoyState.bConnectionJoypad == true))
	{//速のフォース設置
		if (g_JoyStateFollowForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			SetForce(m_nCnt, FOLLOWFORCETYPE_SPEED);
			m_nCnt++;
		}
		g_JoyStateFollowForce.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_4) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_Y && pJoyState.bConnectionJoypad == true))
	{//護のフォース設置
		if (g_JoyStateFollowForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			SetForce(m_nCnt, FOLLOWFORCETYPE_DEFENSE);
			m_nCnt++;
		}
		g_JoyStateFollowForce.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
#endif
#if 0
	if (pInputKeyboard->GetKeyboardTrigger(DIK_1) == true )
	{//攻のフォースの設置
		SetForce(m_nCnt, FOLLOWFORCETYPE_ATTACK);
		m_nCnt++;
	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_2) == true )
	{//拡のフォース設置

		SetForce(m_nCnt, FOLLOWFORCETYPE_SPREAD);
		m_nCnt++;
	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_3) == true )
	{//速のフォース設置
		SetForce(m_nCnt, FOLLOWFORCETYPE_SPEED);
		m_nCnt++;
	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_4) == true )
	{//護のフォース設置
		SetForce(m_nCnt, FOLLOWFORCETYPE_DEFENSE);
		m_nCnt++;
	}
#endif
	if (g_JoyStateFollowForce.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateFollowForce.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateFollowForce.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateFollowForce.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
	for (int nCntForce = 0; nCntForce < MAX_FOLLOWFORCE; nCntForce++)
	{
		m_apPolygon[nCntForce]->SetPosScene2D(posForce[nCntForce]);
	}
#endif
}

//=============================================================================
//描画処理
//=============================================================================
void CFollowForce::Draw(void)
{
	//if (m_bUse == true)
	//{
	//	for (int nCntNumber = 0; nCntNumber < MAX_FOLLOWFORCE; nCntNumber++)
	//	{
	//		if (m_apPolygon[nCntNumber] != NULL)
	//		{
	//			m_apPolygon[nCntNumber]->Draw();
	//		}
	//	}
	//}

}

//=============================================================================
//スコアの設定
//=============================================================================
void CFollowForce::SetForce(int nCntForce, FOLLOWFORCETYPE type)
{
	m_apPolygon[nCntForce]->BindTexture(m_apTexture[type]);

	if (m_aForceStruct[nCntForce].type != type)
	{
		switch (m_aForceStruct[nCntForce].type)
		{
		case FOLLOWFORCETYPE_ATTACK://攻のフォース
			if (0 < m_nNumAttack)m_nNumAttack--;
			break;
		case FOLLOWFORCETYPE_SPREAD://拡のフォース
			if (0 < m_nNumSpread)m_nNumSpread--;
			break;
		case FOLLOWFORCETYPE_SPEED://速のフォース
			if (0 < m_nNumSpeed)m_nNumSpeed--;
			break;
		case FOLLOWFORCETYPE_DEFENSE://護のフォース
			if (0 < m_nNumDefense)m_nNumDefense--;
			break;
		}

		switch (type)
		{
		case FOLLOWFORCETYPE_ATTACK://攻のフォース
			if (m_nNumAttack < 4)m_nNumAttack++;
			break;
		case FOLLOWFORCETYPE_SPREAD://拡のフォース
			if (m_nNumSpread < 4)m_nNumSpread++;
			break;
		case FOLLOWFORCETYPE_SPEED://速のフォース
			if (m_nNumSpeed < 4)m_nNumSpeed++;
			break;
		case FOLLOWFORCETYPE_DEFENSE://護のフォース
			if (m_nNumDefense < 4)m_nNumDefense++;
			break;
		}

		m_aForceStruct[nCntForce].type = type;
		m_apPolygon[nCntForce]->SetForce(type);
	}





}

