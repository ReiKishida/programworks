//=============================================================================
//
// 背景の処理[2Dポリゴン] [bg.cpp]
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
#include "explosion.h"
#include "bg.h"
#include "force.h"
#include "boss.h"
#include "cutin.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_WIDTH (900)	//ポリゴンの幅
#define BG_HEIGHT (1400)	//ポリゴンのYの高さ
#define TEX_X (1.0f)	//テクスチャの横幅
#define TEX_Y (1.0f)	//テクスチャの縦幅
#define PATTERN_ANIM (11)	//テクスチャパターン数

#define ANIMATION_SPEED (8)	//アニメーションスピード

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CScene2D *CBg::m_apScene2D[MAX_BG] = {};
LPDIRECT3DTEXTURE9 CBg::m_pTexture[MAX_TEXTURE] = {}; //共有テクスチャのポインタ
D3DXCOLOR CBg::m_col[MAX_BG] = {}; //カラー代入用
CBg::BGTYPE CBg::m_type = BGTYPE_PLAYERNORMAL;
CBg::BGFADE CBg::m_BgFade = BGFADE_NONE;
CBg::BGTYPE CBg::m_typeNext = BGTYPE_PLAYERNORMAL;

int CBg::m_nPatternAnim = 0;
int CBg::m_nCounterAnim = 0;
int CBg::m_nTex_Y = 1;

//=============================================================================
//コンストラクタ
//=============================================================================
CBg::CBg(int nPriority) : CScene(nPriority)
{//クリアな値を代入
	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		m_aBg[nCntBg] = D3DXVECTOR2(0.0f, 0.0f);
		m_col[nCntBg] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	m_nCntFrame = 0;
	m_nPatternAnim = 10;	//アニメーションパターン
	m_nCounterAnim = 0;//アニメーションカウンター
	m_nTex_Y = 1;

}
//=============================================================================
//デストラクタ
//=============================================================================
CBg::~CBg()
{


}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CBg::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();


	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg001.png", &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg002.png", &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg001_2.png", &m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg_alter0_1.png", &m_pTexture[4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg_alter1.png", &m_pTexture[5]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg_alter2.png", &m_pTexture[6]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/fade3.png", &m_pTexture[7]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/fade4.png", &m_pTexture[8]);

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CBg::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}


//=============================================================================
//背景の生成
//=============================================================================
CBg *CBg::Create(D3DXVECTOR3 pos)
{
	//背景の生成
	CBg *pBg;
	pBg = new CBg;

	if (pBg != NULL)
	{
		m_type = BGTYPE_PLAYERNORMAL;
		//背景の初期化処理
		pBg->Init(pos);
	}
	return pBg;



}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CBg::Init(D3DXVECTOR3 pos)
{
	//優先順位
	int nPriority;
	m_nCntFrame = 0;//フレームカウント用
	m_nPatternAnim = 10;	//アニメーションパターン
	m_nCounterAnim = 0;//アニメーションカウンター
	m_nTex_Y = 1;//テクスチャ座標

	for (int nCntBg = 0; nCntBg < 3; nCntBg++)
	{
		m_col[nCntBg] = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		m_aBg[nCntBg] = D3DXVECTOR2(0.0f, 0.0f);
	}

	//透明にしておく
	m_typeNext = BGTYPE_PLAYERNORMAL;
	m_BgFade = BGFADE_IN;
	m_col[3] = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f);



	for (int nCntScene2D = 0; nCntScene2D < MAX_BG; nCntScene2D++)
	{//3枚目の背景をプレイヤーのより上へ
		switch (nCntScene2D)
		{
		case 0:
			nPriority = 0;
			break;
		case 1:
			nPriority = 1;
			break;
		case 2:
			nPriority = 6;
			break;
		case 3:
			nPriority = 4;
			break;
		}

		//2Dポリゴンの生成
		m_apScene2D[nCntScene2D] = new CScene2D(nPriority);
		if (m_apScene2D[nCntScene2D] != NULL)
		{
			if (nCntScene2D == 3)
			{
				//背景の幅、高さ設定
				m_apScene2D[3]->SetScene2DWidth(1280.0f);
				m_apScene2D[3]->SetScene2DHeight(720.0f);

				//初期化処理
				m_apScene2D[3]->Init(pos);

				m_apScene2D[3]->SetColor(m_col[3]);

				//テクスチャの割り当て
				m_apScene2D[3]->BindTexture(m_pTexture[7]);
				//背景のオブジェクトの設定
				m_apScene2D[3]->SetObjType(CScene::OBJTYPE_BG);
			}
			else
			{
				//背景の幅、高さ設定
				m_apScene2D[nCntScene2D]->SetScene2DWidth(BG_WIDTH);
				m_apScene2D[nCntScene2D]->SetScene2DHeight(BG_HEIGHT);

				//初期化処理
				m_apScene2D[nCntScene2D]->Init(pos);

				m_apScene2D[nCntScene2D]->SetColor(m_col[nCntScene2D]);

				//テクスチャの割り当て
				m_apScene2D[nCntScene2D]->BindTexture(m_pTexture[nCntScene2D]);
				//背景のオブジェクトの設定
				m_apScene2D[nCntScene2D]->SetObjType(CScene::OBJTYPE_BG);
			}
		}
	}

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CBg::Uninit(void)
{
	for (int nCntScene2D = 0; nCntScene2D < MAX_BG; nCntScene2D++)
	{
		if (m_apScene2D[nCntScene2D] != NULL)
		{
			//2Dシーンの終了処理
			m_apScene2D[nCntScene2D]->Uninit();
			m_apScene2D[nCntScene2D] = NULL;
		}
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CBg::Update(void)
{
	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	D3DXVECTOR3 posBg[MAX_BG];
	D3DXVECTOR3 rotBg[MAX_BG];

	int nNumSpeedForce = CForce::GetNumSpeed();	//速のフォース数

	CPlayer::PLAYERSTATE state = CPlayer::GetState();

	m_nCntFrame++;
	float fCol = m_nCntFrame * 0.00007f;
	float fCol2 = m_nCntFrame * 0.00004f;
	if (m_type == BGTYPE_HACK_PLAYER)
	{
		m_col[0] = D3DXCOLOR(0.0f, 0.6f, 0.0f, 0.3f);
		m_col[1] = D3DXCOLOR(0.0f, 0.6f, 0.0f, 0.3f);
		m_col[2] = D3DXCOLOR(0.0f, 0.6f, 0.0f, 0.3f);

		m_apScene2D[0]->BindTexture(m_pTexture[4]);
		m_apScene2D[1]->BindTexture(m_pTexture[2]);
		m_apScene2D[2]->BindTexture(m_pTexture[6]);

	}
	else if (m_type == BGTYPE_HACK_ENEMY)
	{
		m_col[0] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);
		m_col[1] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);
		m_col[2] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);

		m_apScene2D[0]->BindTexture(m_pTexture[0]);
		m_apScene2D[1]->BindTexture(m_pTexture[3]);
		m_apScene2D[2]->BindTexture(m_pTexture[6]);

	}
	else
	{
		m_col[0] = D3DXCOLOR(0.0f + fCol, 1.0f - fCol, 1.0f - fCol, 0.4f);
		m_col[1] = D3DXCOLOR(0.0f + fCol, 1.0f - fCol, 1.0f - fCol, 0.4f);
		m_col[2] = D3DXCOLOR(0.0f + fCol, 1.0f - fCol, 1.0f - fCol, 0.4f);
		m_apScene2D[0]->BindTexture(m_pTexture[0]);
		m_apScene2D[1]->BindTexture(m_pTexture[1]);
		m_apScene2D[2]->BindTexture(m_pTexture[2]);


	}


	for (int nCntScene2D = 0; nCntScene2D < 3; nCntScene2D++)
	{
		if (m_apScene2D[nCntScene2D] != NULL)
		{
			posBg[nCntScene2D] = m_apScene2D[nCntScene2D]->GetPosScene2D();	//プレイヤーの位置情報
			rotBg[nCntScene2D] = m_apScene2D[nCntScene2D]->GetRotScene2D();	//プレイヤーの角度情報

		   //背景スクロール
			D3DXVECTOR2 BgScroll[4];	//背景スクロール用


			//値の代入
			if (nCntScene2D == 0)
			{//1枚目のスクロール
				m_aBg[0].y += 0.001f;
				if (state == CPlayer::PLAYERSTATE_NORMAL || state == CPlayer::PLAYERSTATE_APPEAR || state == CPlayer::PLAYERSTATE_HACK)
				{
					if (pInputKeyboard->GetKeyboardPress(DIK_A) == true)
					{//Aキーが押されたとき
						m_aBg[0].x += 0.001f + (nNumSpeedForce * 0.0007f);
					}
					else if (pInputKeyboard->GetKeyboardPress(DIK_D) == true)
					{
						m_aBg[0].x -= 0.001f + (nNumSpeedForce * 0.0007f);

					}
				}
				BgScroll[0] = D3DXVECTOR2(m_aBg[0].x, m_aBg[0].y - 0.1f);
				BgScroll[1] = D3DXVECTOR2(m_aBg[0].x - TEX_X, m_aBg[0].y - 0.1f);
				BgScroll[2] = D3DXVECTOR2(m_aBg[0].x, m_aBg[0].y - TEX_Y);
				BgScroll[3] = D3DXVECTOR2(m_aBg[0].x - TEX_X, m_aBg[0].y - TEX_Y);

			}
			else if (nCntScene2D == 1)
			{//2枚目のスクロール
				m_aBg[1].y += 0.0015f;

				if (state == CPlayer::PLAYERSTATE_NORMAL || state == CPlayer::PLAYERSTATE_APPEAR || state == CPlayer::PLAYERSTATE_HACK)
				{
					if (pInputKeyboard->GetKeyboardPress(DIK_A) == true)
					{//Aキーが押されたとき
						m_aBg[1].x += 0.0015f + (nNumSpeedForce * 0.0007f);
					}
					else if (pInputKeyboard->GetKeyboardPress(DIK_D) == true)
					{
						m_aBg[1].x -= 0.0015f + (nNumSpeedForce * 0.0007f);

					}
				}
				BgScroll[0] = D3DXVECTOR2(m_aBg[1].x, m_aBg[1].y - 0.1f);
				BgScroll[1] = D3DXVECTOR2(m_aBg[1].x - TEX_X, m_aBg[1].y - 0.1f);
				BgScroll[2] = D3DXVECTOR2(m_aBg[1].x, m_aBg[1].y - TEX_Y);
				BgScroll[3] = D3DXVECTOR2(m_aBg[1].x - TEX_X, m_aBg[1].y - TEX_Y);

			}
			else if (nCntScene2D == 2)
			{//2枚目のスクロール
				m_aBg[2].y += 0.002f;

				if (state == CPlayer::PLAYERSTATE_NORMAL || state == CPlayer::PLAYERSTATE_APPEAR || state == CPlayer::PLAYERSTATE_HACK)
				{
					if (pInputKeyboard->GetKeyboardPress(DIK_A) == true)
					{//Aキーが押されたとき
						m_aBg[2].x += 0.0015f + (nNumSpeedForce * 0.0007f);
					}
					else if (pInputKeyboard->GetKeyboardPress(DIK_D) == true)
					{
						m_aBg[2].x -= 0.0015f + (nNumSpeedForce * 0.0007f);

					}

				}
				BgScroll[0] = D3DXVECTOR2(m_aBg[2].x, m_aBg[2].y - 0.1f);
				BgScroll[1] = D3DXVECTOR2(m_aBg[2].x - TEX_X, m_aBg[2].y - 0.1f);
				BgScroll[2] = D3DXVECTOR2(m_aBg[2].x, m_aBg[2].y - TEX_Y);
				BgScroll[3] = D3DXVECTOR2(m_aBg[2].x - TEX_X, m_aBg[2].y - TEX_Y);

			}
			else if (nCntScene2D == 3)
			{//3枚目のスクロール
				m_aBg[3].y += 0.0f;

				if (state == CPlayer::PLAYERSTATE_NORMAL || state == CPlayer::PLAYERSTATE_APPEAR || state == CPlayer::PLAYERSTATE_HACK)
				{
					BgScroll[0] = D3DXVECTOR2(m_aBg[3].x, m_aBg[3].y - 0.1f);
					BgScroll[1] = D3DXVECTOR2(m_aBg[3].x - TEX_X, m_aBg[3].y - 0.1f);
					BgScroll[2] = D3DXVECTOR2(m_aBg[3].x, m_aBg[3].y - TEX_Y);
					BgScroll[3] = D3DXVECTOR2(m_aBg[3].x - TEX_X, m_aBg[3].y - TEX_Y);
				}
			}

			//位置情報の設定
			m_apScene2D[nCntScene2D]->SetPosScene2D(posBg[nCntScene2D]);

			//背景のスクロール
			m_apScene2D[nCntScene2D]->ScrollBg(BgScroll[0], BgScroll[1], BgScroll[2], BgScroll[3]);
			m_apScene2D[nCntScene2D]->SetColor(m_col[nCntScene2D]);

		}
	}

	//===========================================================================
	//フェードの処理
	//===========================================================================
	CPlayer::PLAYERSTATE pState;
	pState = CPlayer::GetState();
	if (m_apScene2D[3] != NULL)
	{

		if (m_BgFade != BGFADE_NONE)
		{
			if (m_BgFade == BGFADE_IN)
			{
				SetTexture(0.8f, 1.0f, 0.5f, 1.0f);



				m_nCounterAnim++;

				if (m_nCounterAnim % ANIMATION_SPEED == 0)
				{
					m_nPatternAnim = (m_nPatternAnim - 1) % PATTERN_ANIM;//パターンNo.更新
					if (m_nPatternAnim == 4)
					{
						m_nTex_Y--;
					}
					if (m_nPatternAnim == 0)
					{
						m_BgFade = BGFADE_NONE;//何もしていない状態
						m_nPatternAnim = 0;
					}
				}

			}
			else if (m_BgFade == BGFADE_OUT)
			{
				SetTexture(0.0f, 0.2f, 0.0f, 0.5f);

				m_nCounterAnim++;
				if (m_nCounterAnim % ANIMATION_SPEED == 0)
				{
					m_nPatternAnim = (m_nPatternAnim + 1) % PATTERN_ANIM;//パターンNo.更新
					if (m_nPatternAnim == 1)
					{
						switch (m_typeNext)
						{
						case BGTYPE_HACK_PLAYER:
							CCutin::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), CCutin::CUTIN_TYPE_CUT_PLAYER);
							break;
						case BGTYPE_HACK_ENEMY:
							CCutin::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2, 0.0f), CCutin::CUTIN_TYPE_CUT_ENEMY);
							break;
						}
					}
					if (m_nPatternAnim == 5)
					{
						if (m_typeNext == BGTYPE_HACK_PLAYER || m_typeNext == BGTYPE_HACK_ENEMY)
						{
							CCutin::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, SCREEN_HEIGHT / 2 - 280.0f, 0.0f), CCutin::CUTIN_TYPE_RYOU);
						}
						m_nTex_Y++;
					}
					if (m_nPatternAnim == 6)
					{
						if (m_typeNext == BGTYPE_HACK_PLAYER || m_typeNext == BGTYPE_HACK_ENEMY)
						{
							CCutin::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 - 280.0f, 0.0f), CCutin::CUTIN_TYPE_IKI);
						}
					}
					if (m_nPatternAnim == 7)
					{
						if (m_typeNext == BGTYPE_HACK_PLAYER || m_typeNext == BGTYPE_HACK_ENEMY)
						{
							CCutin::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, SCREEN_HEIGHT / 2 + 280.0f, 0.0f), CCutin::CUTIN_TYPE_TEN);
						}
					}
					if (m_nPatternAnim == 8)
					{
						if (m_typeNext == BGTYPE_HACK_PLAYER || m_typeNext == BGTYPE_HACK_ENEMY)
						{//
							CCutin::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 + 280.0f, 0.0f), CCutin::CUTIN_TYPE_KAI);
						}
					}
					if (m_nPatternAnim == 9)
					{
						CCutin::NextAnim();
					}
					if (m_nPatternAnim == 10)
					{
						m_BgFade = BGFADE_IN;//何もしていない状態
						m_nPatternAnim = 9;
						m_type = m_typeNext;
						switch (m_type)
						{
						case BGTYPE_PLAYERNORMAL:
							CPlayer::SetState(CPlayer::PLAYERSTATE_NORMAL);
							break;
						case BGTYPE_HACK_PLAYER:
								CPlayer::SetState(CPlayer::PLAYERSTATE_HACK);
								CBoss::SetState(CBoss::BOSSSTATE_NORMAL);
							break;
						case BGTYPE_HACK_ENEMY:
							CPlayer::SetState(CPlayer::PLAYERSTATE_NORMAL);
							CBoss::SetState(CBoss::BOSSSTATE_HACK);
							break;
						}
					}
				}
			}
		}
		//テクスチャ設定
		m_apScene2D[3]->SetTextureAnimY(0.2f, 0.5f, m_nPatternAnim, m_nTex_Y);

		//位置情報の設定
		m_apScene2D[3]->SetPosScene2D(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CBg::Draw(void)
{

}

//=============================================================================
// フェードの設定
//=============================================================================
void CBg::SetBG(BGTYPE type)
{
	m_type = type;
	if (type == BGTYPE_HACK_PLAYER)
	{
		CPlayer::SetState(CPlayer::PLAYERSTATE_HACK);
	}
	if (type == BGTYPE_PLAYERNORMAL)
	{
		CPlayer::SetState(CPlayer::PLAYERSTATE_NORMAL);
	}
}


//=============================================================================
// 背景の状態
//=============================================================================
CBg::BGTYPE CBg::GetBG(void)
{
	return m_type;
}

//=============================================================================
// 背景フェードの設定
//=============================================================================
 void CBg::SetFade(BGTYPE modeNext)
 {
	 m_col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	 m_apScene2D[3]->SetColor(m_col[3]);
	 m_BgFade = BGFADE_OUT;
	 if (modeNext == BGTYPE_HACK_PLAYER)
	 {
		 m_apScene2D[3]->BindTexture(m_pTexture[7]);
		 m_typeNext = modeNext;

	 }
	 else if (modeNext == BGTYPE_PLAYERNORMAL)
	 {
		 m_apScene2D[3]->BindTexture(m_pTexture[7]);
		 m_typeNext = modeNext;

	 }
	 else if (modeNext == BGTYPE_HACK_ENEMY)
	 {
		 m_apScene2D[3]->BindTexture(m_pTexture[8]);
		 m_typeNext = modeNext;

	 }
	 else if (modeNext == BGTYPE_BOSSNORMAL)
	 {
		 m_apScene2D[3]->BindTexture(m_pTexture[8]);
		 m_typeNext = modeNext;

	 }
 }

 //=============================================================================
 // 背景フェードの取得
 //=============================================================================
 CBg::BGFADE CBg::GetBgFade(void)
 {
	 return m_BgFade;
 }

 //=============================================================================
 // 背景テクスチャの設定
 //=============================================================================
 void CBg::SetTexture(float fTex_X_0, float fTex_X_1, float fTex_Y_0, float fTex_Y_1)
 {

	 m_apScene2D[3]->SetTexture_All(fTex_X_1, fTex_X_0, fTex_Y_1, fTex_Y_0);

 }
