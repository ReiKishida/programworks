#pragma once
//=============================================================================
//
// 作成ツールの処理 [create.h]
// Author : Okabe Kazuki
//
//=============================================================================
#ifndef _CREATE_H_
#define _CREATE_H_

#include "main.h"
class CLight;
class CCamera;
class CScene3D;
class CMeshField;
class CPlayer;
class CObjCreate;
class CFieldCreate;
class CLine;

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//====================================
// ゲームクラス
//====================================
class CCreate
{
public:
	typedef enum
	{
		CREATEMODE_OBJECT = 0,
		CREATEMODE_FIELD,
		CREATEMODE_MAX
	}CREATEMODE;
	CCreate();
	~CCreate();
	static CCreate *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CLight *GetLight(void) { return m_pLight; }						// ライト
	static CCamera *GetCamera(void) {return m_pCamera;}					// カメラ
	static CScene3D *GetScene3D(void) { return m_pScene3D; }			// 3Dポリゴン
	static CMeshField *GetMeshField(void) { return m_pMeshField; }			// メッシュフィールド
	static CObjCreate *GetObjCreate(void) { return m_pObjCreate; }
	static CFieldCreate *GetFieldCreate(void) { return m_pFieldCreate; }	// オブジェクト作成
	static CLine *GetLine(void) { return m_pLine; }							// ライン
	static int GetMainPlayerID(void) { return m_nMainPlayerID; }			// メインのプレイヤー番号
	static void SetMainPlayerID(int nID) { m_nMainPlayerID = nID; }


	static bool GetPause(void);					// ポーズ取得
	static void SetPause(bool bPause);			// ポーズ設定

	static CREATEMODE  GetCreateMode(void) { return m_mode; }

private:
	static CLight *m_pLight;					// ライト
	static CCamera *m_pCamera;					// カメラ
	static CScene3D *m_pScene3D;				// 3Dポリゴン
	static CMeshField *m_pMeshField;			// メッシュフィールド
	static int m_nMainPlayerID;					// メインのプレイヤー番号
	static CPlayer *m_pPlayer;					// プレイヤー
	static CObjCreate *m_pObjCreate;
	static CFieldCreate *m_pFieldCreate;
	static CLine *m_pLine;						// ライン
	static bool m_bPause;						// ポーズ
	static CREATEMODE m_mode;							// 作成する種類

};
#endif