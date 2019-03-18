#pragma once
//=============================================================================
//
// フェードの処理 [fade.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "manager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//====================================
// フェードクラス
//====================================
class CFade
{
public:
	typedef enum
	{
		FADE_NONE = 0,		// 何もしていない状態
		FADE_IN,			// フェードイン状態
		FADE_OUT,			// フェードアウト状態
		FADE_MAX
	} FADE;
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_WIPE,
		TYPE_CLOSSFADE,
		TYPE_MAX,
	}TYPE;
	CFade();
	~CFade();
	static HRESULT Load(void);	// テクスチャ生成
	static void UnLoad(void);	// テクスチャ破棄
	static CFade *Create(CManager::MODE modeNext);

	HRESULT Init(CManager::MODE modeNext);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetFade(CManager::MODE modeNext);
	static void SetWipe(CManager::MODE modeNext);
	static void SetCloss(void);
	static FADE GetFade(void);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	static TYPE m_type;
	static D3DXVECTOR3		m_pos;			// 座標
	static D3DXCOLOR		m_col;			// 色
	static float m_fWidth;					// 幅
	static float m_fHeight;					// 高さ
	static D3DXVECTOR2 m_uv;
	static CManager::MODE m_modeNext;	// 次のモード
	static FADE m_fade;					// フェード
};
#endif