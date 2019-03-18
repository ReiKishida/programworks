//=============================================================================
//
// タイマーの処理[2Dポリゴン] [timer.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================5
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"
#include "number.h"
#include "timer.h"
#include "fade.h"
#include "game.h"
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CNumber *CTimer::m_apNumber[TIMER_NUMBER] = {};	//数字のポインタ
int CTimer::m_nTimer = 0;	//タイマー
int CTimer::m_nMinutes = 0;	//分
int CTimer::m_nTenSeconds = 0;	//10秒台
int CTimer::m_nOneSeconds = 0;	//1秒台
int CTimer::m_nTenComma = 0;	//コンマ秒1秒台
int CTimer::m_nOneComma = 0;	//コンマ秒10秒台
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MINUTES_WIDTH (30.0f)
#define MINUTES_HEIGHT (50.0f)

#define SECONDS_WIDTH (20.0f)
#define SECONDS_HEIGHT (40.0f)

#define COMMA_WIDTH (15.0f)
#define COMMA_HEIGHT (25.0f)

//=============================================================================
//コンストラクタ
//=============================================================================
CTimer::CTimer() :CScene(7)
{//クリアな値を代入
	m_nMinutes = 0;
	m_nTenSeconds = 0;
	m_nOneSeconds = 0;
	m_nTenComma = 0;
	m_nOneComma = 0;
	m_nCntFrame = 0;
}
//=============================================================================
//デストラクタ
//=============================================================================
CTimer::~CTimer()
{
	bUse = true;

}



//=============================================================================
//スコアの生成
//=============================================================================
CTimer *CTimer::Create(D3DXVECTOR3 pos)
{
	//スコアの生成
	CTimer *pScore;
	pScore = new CTimer;

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
HRESULT CTimer::Init(D3DXVECTOR3 pos)
{
	//float fSpace;
	m_nCntFrame = 0;
	bUse = true;

	for (int nCntNumber = 0; nCntNumber < TIMER_NUMBER; nCntNumber++)
	{
		//数字の生成
		m_apNumber[nCntNumber] = new CNumber;
		if (m_apNumber[nCntNumber] != NULL)
		{
			switch (nCntNumber)
			{
			case TIMERTYPE_MINUTE:
				//分
				m_apNumber[0]->SetNumberWidth(MINUTES_WIDTH);
				m_apNumber[0]->SetNumberHeight(MINUTES_HEIGHT);
				m_apNumber[0]->Init(D3DXVECTOR3(pos.x, pos.y, pos.z));
				break;
			case TIMERTYPE_TENSECONDS:
				//10秒台
				m_apNumber[1]->SetNumberWidth(SECONDS_WIDTH);
				m_apNumber[1]->SetNumberHeight(SECONDS_HEIGHT);
				m_apNumber[1]->Init(D3DXVECTOR3(pos.x + 60.0f, pos.y + 5.0f, pos.z));
				break;
			case TIMERTYPE_ONESECONDS:
				//1秒台
				m_apNumber[2]->SetNumberWidth(SECONDS_WIDTH);
				m_apNumber[2]->SetNumberHeight(SECONDS_HEIGHT);
				m_apNumber[2]->Init(D3DXVECTOR3(pos.x + 90.0f, pos.y + 5.0f, pos.z));
				break;
			case TIMERTYPE_TENCOMMASECONDS:
				//10コンマ秒
				m_apNumber[3]->SetNumberWidth(COMMA_WIDTH);
				m_apNumber[3]->SetNumberHeight(COMMA_HEIGHT);
				m_apNumber[3]->Init(D3DXVECTOR3(pos.x + 140.0f, pos.y + 15.0f, pos.z));
				break;
			case TIMERTYPE_ONECOMMASECONDS:
				//1コンマ秒
				m_apNumber[4]->SetNumberWidth(COMMA_WIDTH);
				m_apNumber[4]->SetNumberHeight(COMMA_HEIGHT);
				m_apNumber[4]->Init(D3DXVECTOR3(pos.x + 160.0f, pos.y + 15.0f, pos.z));
				break;
			}




			//====================================
			//タイマーの設定
			//====================================
			SetTimer(1, 0, 3, 0, 0);

			//スコアのオブジェクトの設定
			SetObjType(CScene::OBJTYPE_SCORE);
		}
	}

	return S_OK;
}


//=============================================================================
//終了処理
//=============================================================================
void CTimer::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < TIMER_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			//2Dシーンの終了処理
			m_apNumber[nCntNumber]->Uninit();
			m_apNumber[nCntNumber] = NULL;
		}
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CTimer::Update(void)
{
	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();
	//カウント用フレームの加算
	m_nCntFrame++;

	if (bUse == true)
	{
		m_nOneComma--;
	}
	//１０台コンマ秒を減少
	if (m_nCntFrame % 6 == 0)
	{//6フレームで更新
			m_nTenComma--;
	}

		if (m_nOneComma < 0)
		{//コンマ秒

			if (m_nMinutes <= 0 && m_nTenSeconds <= 0 && m_nOneSeconds <= 0 && m_nTenComma <= 0 && m_nOneComma < 0)
			{
				m_nOneComma = 0;
				bUse = false;
				if (pFade == CFade::FADE_NONE)
				{
					CGame::SetGameState(CGame::GAMESTATE_FAILED);
					//CFade::SetFade(CManager::MODE_RESULT);

				}
			}
			else
			{
				m_nOneComma = 9;

			}
		}
		//1台コンマ秒を減少
		if (m_nTenComma < 0)
		{//コンマ秒
			if (m_nMinutes <= 0 && m_nTenSeconds <= 0 && m_nOneSeconds <= 0 && m_nTenComma <= 0)
			{
				m_nTenComma = 0;
				m_nOneComma = 0;
			}
			else
			{
				m_nTenComma = 9;
				m_nOneSeconds--;

			}
		}

		if (m_nOneSeconds < 0)
		{//秒
			if (m_nMinutes <= 0 && m_nTenSeconds <= 0 && m_nOneSeconds <= 0)
			{//1分と10秒台を切っているとき
				m_nOneSeconds = 0;
			}
			else
			{
				m_nOneSeconds = 9;
				m_nTenSeconds--;

			}
		}
		if (m_nTenSeconds < 0)
		{//秒
			if (m_nMinutes <= 0 && m_nTenSeconds <= 0)
			{//1分を切っているとき
				m_nTenSeconds = 0;
			}
			else
			{
				m_nTenSeconds = 5;
				m_nMinutes--;

			}

		}
		if (m_nMinutes >= 1)
		{//残り1分
			m_apNumber[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apNumber[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apNumber[2]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apNumber[3]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apNumber[4]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		}
		if (m_nMinutes <= 0)
		{//残り1分
			m_nMinutes = 0;

			m_apNumber[0]->SetColor(D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));
			m_apNumber[1]->SetColor(D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));
			m_apNumber[2]->SetColor(D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));
			m_apNumber[3]->SetColor(D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));
			m_apNumber[4]->SetColor(D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));

		}
		if (m_nMinutes <= 0 && m_nTenSeconds < 3)
		{//残り30秒
			m_nCntColor++;
			m_nCntColor = m_nCntColor % 18;
			if (m_nCntColor < 9)
			{
				m_apNumber[0]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				m_apNumber[1]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				m_apNumber[2]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				m_apNumber[3]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				m_apNumber[4]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			if (m_nCntColor >= 9)
			{
				m_apNumber[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apNumber[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apNumber[2]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apNumber[3]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apNumber[4]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}

	//=======================================================
	//番号のテクスチャ設定
	//=======================================================
	//分
	m_apNumber[0]->SetNumber(m_nMinutes);

	//秒
	m_apNumber[1]->SetNumber(m_nTenSeconds);
	m_apNumber[2]->SetNumber(m_nOneSeconds);

	//コンマ秒
	m_apNumber[3]->SetNumber(m_nTenComma);
	m_apNumber[4]->SetNumber(m_nOneComma);

}

//=============================================================================
//描画処理
//=============================================================================
void CTimer::Draw(void)
{
	for (int nCntNumber = 0; nCntNumber < TIMER_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->Draw();
		}
	}
}

//=============================================================================
//スコアの設定
//=============================================================================
void CTimer::SetTimer(int nMinutes, int nTenSeconds, int nOneSeconds, int nTenComma, int nOneComma)
{
	m_nMinutes = nMinutes;
	m_nTenSeconds = nTenSeconds;
	m_nOneSeconds = nOneSeconds;
	m_nTenComma = nTenComma;
	m_nOneComma = nOneComma;

}

//=============================================================================
//スコアの取得
//=============================================================================
int CTimer::GetTimer(void)
{
	int nMinutes = m_nMinutes;
	int nTenSeconds = m_nTenSeconds;
	int nOneSeconds = m_nOneSeconds;
	int nTenComma = m_nTenComma;
	int nOneComma = m_nOneComma;

	nMinutes = nMinutes * 10000;
	nTenSeconds = nTenSeconds * 1000;
	nOneSeconds = nOneSeconds * 100;
	nTenComma = nTenComma * 10;
	nOneComma = nOneComma;

	m_nTimer = nMinutes + nTenSeconds + nOneSeconds + nTenComma + nOneComma;

	return m_nTimer;
}

//=============================================================================
//スコアの加算
//=============================================================================
void CTimer::AddSeconds(int nSeconds)
{
	int nOneSecondsAmari;
	int nTenSecondsAmari;

	//スコアの加算
	m_nOneSeconds += nSeconds;


	if (m_nOneSeconds >= 10)
	{
		m_nTenSeconds++;
		nOneSecondsAmari = m_nOneSeconds % 10;
		m_nOneSeconds = 0;
		m_nOneSeconds = m_nOneSeconds + nOneSecondsAmari;
	}



	if (m_nTenSeconds >= 6)
	{
		m_nMinutes++;
		nTenSecondsAmari = m_nTenSeconds % 6;

		m_nTenSeconds = 0;

		m_nTenSeconds = m_nTenSeconds + nTenSecondsAmari;

	}



}

