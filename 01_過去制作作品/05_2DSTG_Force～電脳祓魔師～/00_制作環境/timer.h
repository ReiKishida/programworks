//=============================================================================
//
// タイマー処理[2D ポリゴン] [timer.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TIMER_NUMBER (5)

class CTimer : public CScene
{
public:
	typedef enum
	{
		TIMERTYPE_MINUTE = 0,	//分
		TIMERTYPE_TENSECONDS,	//10秒台
		TIMERTYPE_ONESECONDS,	//1秒台
		TIMERTYPE_TENCOMMASECONDS,	//コンマ10秒台
		TIMERTYPE_ONECOMMASECONDS,	//コンマ1秒台

	}TIMETYPE;
	CTimer();	//コンストラクタ
	~CTimer();	//デストラクタ
	static CTimer *Create(D3DXVECTOR3 pos);	//数字の生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetTimer(int nMinutes, int nTenSeconds, int nOneSeconds, int nTenComma, int nOneComma);	//スコアの設定
	static int GetTimer(void);	//スコアの取得
	static void AddSeconds(int nSeconds);	//スコアの加算
private:
	static CNumber *m_apNumber[TIMER_NUMBER];	//数字
	static int m_nTimer;		//タイマー
	static int m_nMinutes;		//分
	static int m_nTenSeconds;	 //10秒台
	static int m_nOneSeconds;	 //10秒台

	static int m_nTenComma;	//コンマ秒
	static int m_nOneComma;	//コンマ秒

	int m_nCntFrame;	//フレーム数カウント用
	int m_nCntColor;	//カラーカウント用
	bool bUse;//カウントするか
};

#endif