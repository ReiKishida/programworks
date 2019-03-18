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
#include "log.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_WIDTH (900)	//ポリゴンの幅
#define BG_HEIGHT (700)	//ポリゴンのYの高さ
#define LOG_WIDTH (210.0f)	//ログの幅
#define LOG_HEIGHT (70.0f)	//ログの高さ
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CLog::m_apTexture[MAX_LOGTEXTURE_NUM] = {}; //共有テクスチャのポインタ
int CLog::m_nCntLog = 0;	//ログの数
int CLog::m_nCntLogOld = 0;	//過去のログの数
//=============================================================================
//コンストラクタ
//=============================================================================
CLog::CLog() : CScene2D(7)
{//クリアな値を代入

}
//=============================================================================
//デストラクタ
//=============================================================================
CLog::~CLog()
{
	m_nCntLog = 0;

}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CLog::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/normalenemy_d.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/attackenemy_d.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spreadenemy_d.png", &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/speedenemy_d.png", &m_apTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/defenceenemy_d.png", &m_apTexture[4]);

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CLog::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_LOGTEXTURE_NUM; nCntTex++)
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
CLog *CLog::Create(D3DXVECTOR3 pos, LOGTYPE type)
{
	//スコアの生成
	CLog *pLog;
	pLog = new CLog;

	if (pLog != NULL)
	{
		m_nCntLog++;
		pLog->m_type = type;
		//pLog->m_MoveState = 0;
		//スコアの初期化処理
		pLog->Init(pos);
		m_nCntLogOld = m_nCntLog;
		m_nCntLog = m_nCntLog % MAX_LOG_NUM;


	}

	return pLog;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CLog::Init(D3DXVECTOR3 pos)
{


	//プレイヤーの幅、高さ設定
	SetScene2DWidth(LOG_WIDTH);
	SetScene2DHeight(LOG_HEIGHT);

	switch (m_type)
	{
	case LOGTYPE_NORMAL:
		BindTexture(m_apTexture[LOGTYPE_NORMAL]);
		break;
	case LOGTYPE_ATTACK:
		BindTexture(m_apTexture[LOGTYPE_ATTACK]);
		break;
	case LOGTYPE_SPREAD:
		BindTexture(m_apTexture[LOGTYPE_SPREAD]);
		break;
	case LOGTYPE_SPEED:
		BindTexture(m_apTexture[LOGTYPE_SPEED]);
		break;
	case LOGTYPE_DEFENSE:
		BindTexture(m_apTexture[LOGTYPE_DEFENSE]);
		break;
	}

	//初期化位置
	D3DXVECTOR3 InitPos;


	//2Dシーンの初期化処理
	CScene2D::Init(D3DXVECTOR3(pos.x, pos.y, pos.z));

	//位置情報の取得
	m_pos = GetPosScene2D();
	MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 - 120.0f, 1);

	SetTexture(TEX_X, TEX_Y);
	//プレイヤーのオブジェクトの設定
	SetObjType(CScene::OBJTYPE_LOG);

	return S_OK;

}


//=============================================================================
//終了処理
//=============================================================================
void CLog::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void CLog::Update(void)
{

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();





	//位置情報の取得
	m_pos = GetPosScene2D();
	//色情報の取得
	m_col = GetColor();

	m_col.a -= 0.0015f;

	MoveLog();

	//色情報の設定
	SetColor(m_col);
	//位置情報の設定
	SetPosScene2D(m_pos);
	if (m_pos.y >= SCREEN_HEIGHT / 2 + 360.0f)
	{
		Uninit();
	}

}

//=============================================================================
//描画処理
//=============================================================================
void CLog::Draw(void)
{
	//2Dシーンの描画処理
	CScene2D::Draw();
}

//=============================================================================
//ログの移動
//=============================================================================
void CLog::MoveLog(void)
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
		switch (m_MoveState)
		{
		case 0:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 - 60.0f, 1);
			break;
		case 1:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 , 2);
			break;
		case 2:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 + 120.0f, 3);
			break;
		case 3:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 + 180.0f, 4);
			break;
		case 4:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 + 240.0f, 5);
			break;
		case 5:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 + 300.0f, 6);
			break;
		case 6:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 + 360.0f, 7);
			break;
		case 7:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 + 440.0f, 8);
			break;
		}

	}

}

//=============================================================================
//ログの移動情報の代入
//=============================================================================
void CLog::MoveInit(float dest_x, float dest_y, int state)
{
	//過去の位置代入
	m_posold.x = m_pos.x;
	m_posold.y = m_pos.y;

	//目的の位置
	m_Destpos.x = dest_x - m_pos.x;
	m_Destpos.y = dest_y - m_pos.y;


	m_fAngle = 0.0f;

	m_MoveState = state;
}
