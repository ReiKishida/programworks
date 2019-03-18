//=============================================================================
//
// ビルボード処理 [sceneBillboard.h]
// Author : Okabe Kazuki
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"
#include "scene.h"

//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
class CBillboard :public CScene
{
public:
	typedef enum
	{// 頂点の位置
		VTXTYPE_CENTER = 0,	// 中央
		VTXTYPE_BASE,		// 下
		VTXTYPE_LEFTBASE,	// 左下
	}VTXTYPE;
	CBillboard(int nPriority = CScene::PRIORITY_INIT);
	~CBillboard();
	static HRESULT Load(void);	// テクスチャ生成
	static void UnLoad(void);	// テクスチャ破棄

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBillboard *Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);

	D3DXVECTOR3 GetPos(void) { return m_pos; };			// 座標取得
	float GetWidth(void) { return m_fWidth; }			// 幅取得
	float GetHeight(void) { return m_fHeight; }			// 高さ取得
	float GetRot(void) { return m_rot; };				// 回転取得
	D3DXCOLOR GetColor(void) { return m_col; };			// 色取得
	D3DXMATRIX	GetMtx(void) { return m_mtxWorld; };	// マトリックス取得

	void SetPos(const D3DXVECTOR3 pos);									// 位置設定
	void SetSize(const float fWidth, const float fHeight);				// 大きさ設定
	void SetColor(D3DXCOLOR col);										// 色
	void SetRot(const float rot);										// 回転設定
	void SetVtx(void);													// バッファの設定
	void SetVtxType(const VTXTYPE type);
	static void SetZenable(const bool Zenable);
protected:
private:
	static LPDIRECT3DTEXTURE9	m_pTextureThis;		// テクスチャへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;				// 位置
	D3DXCOLOR	m_col;
	D3DXMATRIX	m_mtxWorld;			// ワールドマトリックス
	float		m_rot;				// 向き
	float		m_fWidth;			// 幅
	float		m_fHeight;			// 高さ
	float		m_fAngle;			// 角度
	float		m_fLength;			// 距離
	static bool m_bZenable;			// Zバッファ
	VTXTYPE		m_VtxType;			// 頂点タイプ
};

#endif
#pragma once
