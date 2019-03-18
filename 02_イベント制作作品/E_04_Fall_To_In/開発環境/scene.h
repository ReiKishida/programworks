#pragma once
//=============================================================================
//
// オブジェクト処理 [scene.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DATA (10000)
#define MAX_PRIORITY_NUM (6)
//====================================
// シーンクラス
//====================================
class CScene
{
public:
	typedef enum
	{// オブジェクトの種類
		OBJTYPE_NONE = 0,		// 何もない
		OBJTYPE_PLAYER,			// プレイヤー
		OBJTYPE_MESHFIELD,		// メッシュフィールド
		OBJTYPE_MESHWALL,		// メッシュウォール
		OBJTYPE_MAP,			// マップ
		OBJTYPE_DEBUG_SPHERE,	// デバッグスフィア
		OBJTYPE_PAINT,			// ペイント
		OBJTYPE_MODEL,			// モデル
		OBJTYPE_SPEEDMETER,		// スピードメーター
		OBJTYPE_TIMER,
		OBJTYPE_OBJMACHINE,
		OBJTYPE_OBJECT,			// オブジェクト
		OBJTYPE_FUELGAUGE,
		OBJTYPE_NUMBER,			// 数字
		OBJTYPE_BULLET,			// 弾
		OBJTYPE_GAUGE,			// ゲージ
		OBJTYPE_EFFECT,			// エフェクト
		OBJTYPE_BG,				// 背景
		OBJTYPE_FRAME,			// フレーム
		OBJTYPE_INFOUI,			// お知らせUI
		OBJTYPE_CALLOUT,		// 吹き出し
		OBJTYPE_POWERCUT,		// 停電
		OBJTYPE_LEGEND,			// 伝説マシン
		OBJTYPE_EFFECT_2D,		// エフェクト２D
		OBJTYPE_ENVIROMENT,		// 植物
		OBJTYPE_PANEL,
		OBJTYPE_SKY,
		OBJTYPE_MAX				// 最大
	}OBJTYPE;
	typedef enum
	{
		PRIORITY_DEBUG_SPHERE = 4,	//
		PRIORITY_OBJMODEL = 3,	// オブジェモデル
		PRIORITY_PLAYER = 3,	// プレイヤー
		PRIORITY_MESHFIELD = 1,	// メッシュフィールド
		PRIORITY_MESHWALL = 4,	// メッシュ壁
		PRIORITY_PAINT = 4,		// ペイント
		PRIORITY_MAP = 5,		// マップ
		PRIORITY_TIMER = 5,		// 時間
		PRIORITY_LINE = 4,		// 線
		PRIORITY_OBJECT = 1,
		PRIORITY_BULLET = 2,	// 弾
		PRIORITY_GAUGE = 5,		// ゲージ
		PRIORITY_EFFECT = 5,	// エフェクト
		PRIORITY_BG = 0,		// 背景
		PRIORITY_FRAME = 3,		// フレーム
		PRIORITY_INFOUI = 1,	// お知らせUI
		PRIORITY_CALLOUT = 5,	// 吹き出し
		PRIORITY_POWERCUT = 4,	// 停電
		PRIPRITY_LEGEND = 3,	// 伝説マシン
		PRIORITY_EFFECT_2D = 5,	// エフェクト２D
		PRIORITY_TITLEROGO = 5,	// ロゴ
		PRIORITY_ENVIROMENT = 3,// 植物
		PRIORITY_PANEL = 5,
		PRIORITY_SKY = 4,
		PRIORITY_INIT = 3,	// 初期用
	}PRIORITY;

	CScene(int nPriority = PRIORITY_INIT);
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void DeathFrag(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void DrawPart(int nIdy);
	void SetObjType(OBJTYPE type);
	OBJTYPE GetObjType(void);
	static int GetNumAll(void);
	static bool OverData(void);
	static CScene *GetScene(const int nPriority);
	CScene *GetNext(void) { return m_apNext; };
	static int GetNumPriority(const int nPriority) { return m_nNumPriority[nPriority]; };
protected:
	void Release(void);
private:
	static CScene *m_apTop[MAX_PRIORITY_NUM];	// 先頭のオブジェクトへのポインタ
	static CScene *m_apCur[MAX_PRIORITY_NUM];	// 現在（最後）のオブジェクトへのポインタ
	CScene *m_apPrev;			// 前のオブジェクトへのポインタ
	CScene *m_apNext;			// 次のオブジェクトへのポインタ
	static int m_nNumAll;						// 総数
	static int m_nNumPriority[MAX_PRIORITY_NUM];// 優先順位ごとの総数

	int m_nID;									// 自分自身の番号
	OBJTYPE m_objType;							// オブジェクトの種類
	bool m_bDeath;								// 死亡フラグ
	int m_nPriority;					// 優先順位

};
#endif