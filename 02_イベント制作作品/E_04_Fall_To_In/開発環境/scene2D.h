#pragma once
//=============================================================================
//
// ２Dオブジェクト処理 [scene.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

//====================================
// 2Dオブジェクトクラス
//====================================
class CScene2D : public CScene
{
public:
	typedef enum
	{// 頂点の位置
		VTXTYPE_CENTER = 0,	// 中央
		VTXTYPE_BASE,		// 下
		VTXTYPE_LEFT,		// 左
		VTXTYPE_RIGHT,		// 右
		VTXTYPE_MAX,
	}VTXTYPE;

	CScene2D(int nPriority = CScene::PRIORITY_INIT);
	~CScene2D();
	static HRESULT Load(void);	// テクスチャ生成
	static void UnLoad(void);	// テクスチャ破棄

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CScene2D *Create(const D3DXVECTOR3 pos, const float fWidth,const float fHeight);

	void SetPos(D3DXVECTOR3 pos);
	void SetInitPos(D3DXVECTOR3 pos);
	void SetSize(float fWidth, float fHeight);	// 大きさ設定
	void SetRot(float fRot);							// 回転設定
	void SetColor(D3DXCOLOR col);						// 色
	void SetVtx(void);							// 頂点設定
	void SetPattern(const D3DXVECTOR2 uv, const int nPatternX, const int nPatternY, const bool bTurn = false);
	void SetTex(D3DXVECTOR2 tex);
	void SetVtxType(VTXTYPE VtxType);
	void SetUse(const bool bUse);
	void SetStamp(const float fWidth, const float fHeight);
	D3DXVECTOR3 GetPos(void) { return m_pos; }			//座標
	D3DXVECTOR3 GetInitPos(void) { return m_Initpos; };	// 初期座標
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; };	// 前回座標取得
	float GetWidth(void) { return m_fWidth; }			// 幅取得
	float GetHeight(void) { return m_fHeight; }			// 高さ取得
	float GetRot(void) {return m_fRot;};				// 回転取得
	D3DXCOLOR GetColor(void) {return m_col;};			// 色取得
	D3DXVECTOR2 GetTex(void) { return m_tex; };
	LPDIRECT3DVERTEXBUFFER9 GetBuff(void) { return m_pVtxBuff; };	// 頂点取得
	bool GetUse(void) { return m_bUse; };
protected:
private:
	static LPDIRECT3DTEXTURE9	m_pTextureThis;		// テクスチャへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;			// 位置
	D3DXVECTOR3				m_Initpos;			// 位置
	D3DXVECTOR3				m_posOld;		// 位置
	D3DXCOLOR				m_col;			// 色
	float					m_fWidth;		// 幅
	float					m_fHeight;		// 高さ
	D3DXVECTOR2				m_tex;			// テクスチャ加算座標
	float					m_fAngle[2];	// 角度
	float					m_fLength[2];	// 距離
	float					m_fRot;			// 回転量
	VTXTYPE					m_VtxType;		// 頂点タイプ
	bool					m_bUse;			// 使用されているかどうか
};
#endif