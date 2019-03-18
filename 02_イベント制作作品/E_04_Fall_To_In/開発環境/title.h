#pragma once
//=============================================================================
//
// タイトルの処理 [title.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _TIITLE_H_
#define _TIITLE_H_

#include "main.h"
class CLight;
class CCamera;
class CScene3D;
class CInfoUI;
class CPlayer;
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//====================================
// タイトルクラス
//====================================
class CTitle
{
public:
	CTitle();
	~CTitle();
	static CTitle *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLight *GetLight(void) { return m_pLight; }						// ライト
	static CCamera *GetCamera(void) { return m_pCamera; }					// カメラ

	static int GetNext(void){ return m_nCntNext; }
	static bool GetTitlePush(void) { return m_bPush; }
	static bool GetSkip(void) { return m_bSkip; }

	static void SetSkip(bool skip);

private:
	static CLight *m_pLight;					// ライト
	static CCamera *m_pCamera;					// カメラ
	static CPlayer *m_pPlayer;		// プレイヤー
	static CPlayer *m_pPlayer2;		// プレイヤー

	static int m_nCntNext;	// セレクトまで行くまでのカウンター
	static bool m_bPush;	// ボタンが押されたかどうか
	static bool m_bSkip;	// スキップされてるかどうか

	static int m_nCntAppear;		// タイトル素材の出現カウンター
};
#endif