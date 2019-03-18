#pragma once
//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : Okabe Kazuki
//
//=============================================================================
#ifndef _OBJCREATE_H_
#define _OBJCREATE_H_

#include "main.h"
#include "scene.h"
#include "sceneX.h"
//====================================
// オブジェクトクラス
//====================================
class CObjCreate : public CSceneX
{
public:
	CObjCreate();
	~CObjCreate();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//int GetNumObj(void) { return m_nNumObject; }

	static void SetScan(void);			// Xファイル読み込み
	static void SetPrint(void);			// Xファイル書き込み
	static CObjCreate *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type);
protected:
private:
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 角度

	int m_type;				// 種類
	int m_nNumObject;		// 配置数
	bool m_bCamera;

};
//====================================
// フィールド作成クラス
//====================================

#define MAX_FIELD (100)
class CFieldCreate : public CScene
{
public:
	CFieldCreate();
	~CFieldCreate();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetScan(void);			// Xファイル読み込み
	static void SetPrint(void);			// Xファイル書き込み
	static CFieldCreate *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type);
protected:
private:
	static D3DXVECTOR3 m_aMeshPos[MAX_FIELD][MAX_FIELD];	// 頂点の保存場所
	D3DXVECTOR3 m_pos;				// 位置
	D3DXVECTOR3 m_rot;				// 角度
	float m_fLength;				// 長さ
	static int m_type;						// 種類

	static int m_nNumAll;					// メッシュブロック全ての個数
	static int m_nNumBlock[MAX_FIELD];		// メッシュのブロック
	static int m_nCntMesh;					// メッシュの頂点数
	bool m_bCamera;
};

#endif#
