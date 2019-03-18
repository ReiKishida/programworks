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
#include "force.h"
#include "polygon.h"
#include "input.h"
#include "effect.h"
#include "sound.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_WIDTH (900)	//ポリゴンの幅
#define BG_HEIGHT (700)	//ポリゴンのYの高さ
#define FORCE_RADIUS (40.0f)
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CForce::m_apTexture[MAX_FORCE] = {}; //共有テクスチャのポインタ
CScene2D *CForce::m_apScene2D[MAX_FORCE] = {};	//ポリゴンのポインタ
int CForce::m_aForce[MAX_FORCE] = {};	//フォースのポインタ
int CForce::m_nNumAttack = 0;	//攻のフォース数
int CForce::m_nNumSpread = 0;	//拡のフォース数
int CForce::m_nNumSpeed = 0;	//速のフォース数
int CForce::m_nNumDefense = 0;	//護のフォース数

int CForce::m_nCntDPS = 0;
int CForce::m_nDPS = 0;
int CForce::m_nDamage = 0;
JoyState g_JoyStateForce;

//=============================================================================
//コンストラクタ
//=============================================================================
CForce::CForce() :CScene(7)
{//クリアな値を代入
	m_nCnt = 0;
	m_nNumAttack = 0;	//攻のフォース数
	m_nNumSpread = 0;	//拡のフォース数
	m_nNumSpeed = 0;	//速のフォース数
	m_nNumDefense = 0;	//護のフォース数

}
//=============================================================================
//デストラクタ
//=============================================================================
CForce::~CForce()
{


}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CForce::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/atk.png", &m_apTexture[FORCETYPE_ATTACK]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spr.png", &m_apTexture[FORCETYPE_SPREAD]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spd.png", &m_apTexture[FORCETYPE_SPEED]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/def.png", &m_apTexture[FORCETYPE_DEFENSE]);

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CForce::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_FORCE; nCntTex++)
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
CForce *CForce::Create(D3DXVECTOR3 pos)
{
	//スコアの生成
	CForce *pScore;
	pScore = new CForce;

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
HRESULT CForce::Init(D3DXVECTOR3 pos)
{
	m_nCnt = 0;
	m_nCntDPS = 0;
	//ゲームパッドの状態
	g_JoyStateForce.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	for (int nCntNumber = 0; nCntNumber < MAX_FORCE; nCntNumber++)
	{
		//数字の生成
		m_apScene2D[nCntNumber] = new CScene2D(7);
		if (m_apScene2D[nCntNumber] != NULL)
		{
			m_type[nCntNumber] = FORCETYPE_NONE;

			//初期化処理
			m_apScene2D[nCntNumber]->SetScene2DWidth(FORCE_RADIUS);
			m_apScene2D[nCntNumber]->SetScene2DHeight(FORCE_RADIUS);
			m_apScene2D[nCntNumber]->Init(D3DXVECTOR3(pos.x + (58.0f * nCntNumber), pos.y, pos.z));
			m_apScene2D[nCntNumber]->BindTexture(m_apTexture[FORCETYPE_ATTACK]);

			SetForce(nCntNumber, FORCETYPE_ATTACK);

			//フォースのオブジェクトの設定
			SetObjType(CScene::OBJTYPE_FORCE);

		}
	}



	return S_OK;
}


//=============================================================================
//終了処理
//=============================================================================
void CForce::Uninit(void)
{


	for (int nCntNumber = 0; nCntNumber < MAX_FORCE; nCntNumber++)
	{
		if (m_apScene2D[nCntNumber] != NULL)
		{
			//2Dシーンの終了処理
			m_apScene2D[nCntNumber]->Uninit();
			m_apScene2D[nCntNumber] = NULL;
		}
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CForce::Update(void)
{
	m_nCntDPS++;

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//フォースの位置
	D3DXVECTOR3 posForce[MAX_FORCE];


	for (int nCntForce = 0; nCntForce < MAX_FORCE; nCntForce++)
	{
		posForce[nCntForce] = m_apScene2D[nCntForce]->GetPosScene2D();
	}

	//カウントのリセット
	m_nCnt = m_nCnt % MAX_FORCE;

#if 1
	if (pInputKeyboard->GetKeyboardTrigger(DIK_1) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_B && pJoyState.bConnectionJoypad == true))
	{//攻のフォースの設置
		if (g_JoyStateForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

			SetForce(m_nCnt, FORCETYPE_ATTACK);
			m_nCnt++;
		}
		g_JoyStateForce.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_2) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_A && pJoyState.bConnectionJoypad == true))
	{//拡のフォース設置
		if (g_JoyStateForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

			SetForce(m_nCnt, FORCETYPE_SPREAD);
			m_nCnt++;
		}
		g_JoyStateForce.nJoypadState = INPUT_JOYSTATE_PUSH;
	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_3) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_X && pJoyState.bConnectionJoypad == true))
	{//速のフォース設置
		if (g_JoyStateForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

			SetForce(m_nCnt, FORCETYPE_SPEED);
			m_nCnt++;
		}
		g_JoyStateForce.nJoypadState = INPUT_JOYSTATE_PUSH;
	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_4) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_Y && pJoyState.bConnectionJoypad == true))
	{//護のフォース設置
		if (g_JoyStateForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

			SetForce(m_nCnt, FORCETYPE_DEFENSE);
			m_nCnt++;
		}
		g_JoyStateForce.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
#endif
#if 0
	if (pInputKeyboard->GetKeyboardTrigger(DIK_1) == true )
	{//攻のフォースの設置

		pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

		SetForce(m_nCnt, FORCETYPE_ATTACK);
		m_nCnt++;


	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_2) == true )
	{//拡のフォース設置

		pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

		SetForce(m_nCnt, FORCETYPE_SPREAD);
		m_nCnt++;

	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_3) == true )
	{//速のフォース設置

		pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

		SetForce(m_nCnt, FORCETYPE_SPEED);
		m_nCnt++;

	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_4) == true )
	{//護のフォース設置

		pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

		SetForce(m_nCnt, FORCETYPE_DEFENSE);
		m_nCnt++;

	}
#endif

	if (g_JoyStateForce.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateForce.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateForce.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateForce.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}

	for (int nCntForce = 0; nCntForce < MAX_FORCE; nCntForce++)
	{
		m_apScene2D[nCntForce]->SetPosScene2D(posForce[nCntForce]);
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CForce::Draw(void)
{
	//for (int nCntNumber = 0; nCntNumber < MAX_FORCE; nCntNumber++)
	//{
	//	if (m_apScene2D[nCntNumber] != NULL)
	//	{
	//		m_apScene2D[nCntNumber]->Draw();
	//	}
	//}

}

//=============================================================================
//スコアの設定
//=============================================================================
void CForce::SetForce(int nCntForce, FORCETYPE type)
{
	m_apScene2D[nCntForce]->BindTexture(m_apTexture[type]);

	if (m_type[nCntForce] != type)
	{
		switch (m_type[nCntForce])
		{
		case FORCETYPE_ATTACK:
			if (0 < m_nNumAttack)m_nNumAttack--;
			break;
		case FORCETYPE_SPREAD:
			if (0 < m_nNumSpread)m_nNumSpread--;
			break;
		case FORCETYPE_SPEED:
			if (0 < m_nNumSpeed)m_nNumSpeed--;
			break;
		case FORCETYPE_DEFENSE:
			if (0 < m_nNumDefense)m_nNumDefense--;
			break;
		}

		switch (type)
		{
		case FORCETYPE_ATTACK:
			if (m_nNumAttack < 4)m_nNumAttack++;
			break;
		case FORCETYPE_SPREAD:
			if (m_nNumSpread < 4)m_nNumSpread++;
			break;
		case FORCETYPE_SPEED:
			if (m_nNumSpeed < 4)m_nNumSpeed++;
			break;
		case FORCETYPE_DEFENSE:
			if (m_nNumDefense < 4)m_nNumDefense++;
			break;
		}

		m_type[nCntForce] = type;
	}
}

//=============================================================================
//攻フォースの数の取得
//=============================================================================
int  CForce::GetNumAttack(void)
{
	return m_nNumAttack;
}

//=============================================================================
//拡フォースの数の取得
//=============================================================================
int  CForce::GetNumSpread(void)
{
	return m_nNumSpread;

}

//=============================================================================
//速フォースの数の取得
//=============================================================================
int  CForce::GetNumSpeed(void)
{
	return m_nNumSpeed;

}

//=============================================================================
//護フォースの数の取得
//=============================================================================
int  CForce::GetNumDefense(void)
{
	return m_nNumDefense;

}


//============================================================================================
//DPSチェック
//============================================================================================
void CForce::CheckDamage(int nDamage)
{

	m_nDamage = nDamage;

	if (m_nDamage >= 1)
	{
		m_nDPS = m_nDamage;
	}
	if (m_nDamage <= 0)
	{
		m_nDPS = 0;
	}


}


//============================================================================================
//DPS取得
//============================================================================================
int CForce::GetnCnt()
{
	return m_nCntDPS;
}


