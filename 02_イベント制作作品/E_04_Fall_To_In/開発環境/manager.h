#pragma once
//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputGamePad;
class CSound;
class CTexture;
class CDebugProc;
class CTitle;
class CGame;
class CResult;
class CCamera;
class CCreate;
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//====================================
// マネージャクラス
//====================================
class CManager
{
public:
	typedef enum
	{// モードの種類
		MODE_TITLE = 0,	// タイトル
		MODE_GAME,		// ゲーム
		MODE_CREATE,	// クリエイト
		MODE_RESULT,	// リザルト
		MODE_MAX,
	}MODE;

	CManager();
	~CManager();
	static void Load(void);
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SaveAngle(float *pAngle);
	static void SaveRot(D3DXVECTOR3 *rot);
	static int PowInt(const int nNum, const int nCnt);	// int型の累乗計算
	static float GetRandomPI(void);					// ランダム関数
	static float GetRandBetWeen(const float fMin,const float fMax);	// ランダム関数
	static int GetIntRandBetWeen(const int nMin, const int nMax);	// ランダム関数
	static void SaveMinMax(float *pSave,const float Min,const float Max);
	static MODE GetMode(void);						// モード
	static CCamera *GetCameraMode(void);				// カメラ取得

	static CRenderer *GetRenderer(void);			// レンダラーの取得
	static CInputKeyboard *GetInputKeyboard(void);	// キーボードの取得
	static CInputMouse *GetInputMouse(void);		// マウスの取得
	static CInputGamePad *GetInputGamePad(void);	// ゲームパッドの取得
	static CSound *GetSound(void);					// サウンド
	static CTexture *GetTexture(void);				// テクスチャ

	static HRESULT SetMode(MODE mode);				// モード
private:
	static CRenderer *m_pRenderer;				// レンダラー
	static CInputKeyboard *m_pInputKeyboard;	// キーボード
	static CInputMouse *m_pInputMouse;			// マウス
	static CInputGamePad *m_pInputGamePad;		// ゲームパッド
	static CSound *m_pSound;					// 音
	static CTexture *m_pTexture;				// テクスチャ
	static CDebugProc *m_pDebugProc;			// デバッグ
	static CTitle *m_pTitle;					// タイトル
	static CGame *m_pGame;						// ゲーム
	static CCreate *m_pCreate;					// クリエイト
	static CResult *m_pResult;					// リザルト
	static MODE m_mode;							// モード
};
#endif