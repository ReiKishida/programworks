//=============================================================================
//
// ゲームの処理 [game.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
class CPlayer;
class CLight;
class CCamera;

//====================================
// ゲームクラス
//====================================
class CGame
{
public:
	typedef enum
	{// ステータス
		STATE_NONE=0,
		STATE_START,
		STATE_END,
		STATE_MAX,
	}STATE;
	typedef enum
	{
		STAGE_SPACE=0,
		STAGE_EARTH,
		STAGE_HELL,
		STAGE_MAX,
	}STAGE;
	CGame();
	~CGame();
	static CGame *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CLight *GetLight(void) { return m_pLight; }						// ライト
	static CCamera *GetCamera(void) { return m_pCamera; }					// カメラ
	static CPlayer *GetPlayer(void) { return m_pPlayer; }	// プレイヤー
	static int m_WinId;

	static bool GetPause(void);					// ポーズ取得
	static void SetPause(bool bPause);			// ポーズ設定
	static STATE GetState(void);				// ステータス
	static void SetState(const STATE state);	// ステータス
	static STAGE GetStage(void) {return m_Stage;};
private:
	static CLight *m_pLight;					// ライト
	static CCamera *m_pCamera;					// カメラ
	static CPlayer *m_pPlayer;		// プレイヤー

	static bool m_bPause;						// ポーズ
	static STATE m_State;
	static int m_nStateCounter;					// ステータスカウンター
	static STAGE m_Stage;
	static bool m_bStage[STAGE_MAX];
	static int m_nPanelCounter;
};
#endif