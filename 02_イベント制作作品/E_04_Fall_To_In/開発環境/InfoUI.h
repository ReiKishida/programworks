#pragma once
//=============================================================================
//
// お知らせUIの処理 [infoUI.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _INFOUI_H_
#define _INFOUI_H_

#include "scene2D.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_INFOUI (30)
//====================================
// お知らせUIクラス
//====================================
class CInfoUI : public CScene2D
{
public:
	typedef enum
	{
		TYPE_TITLE_LOGO = 0,		// タイトルロゴ
		TYPE_SHUTU,					// タイトル素材[6]
		TYPE_WIN,					// タイトル素材[6]
		TYPE_LOOSE,					// タイトル素材[6]
		TYPE_DRAW,					// タイトル素材[6]
		TYPE_MAX
	}TYPE;

	CInfoUI(int nPriority = 5);
	~CInfoUI();
	static HRESULT Load(void);	// テクスチャ生成
	static void UnLoad(void);	// テクスチャ破棄
	static CInfoUI *Create(const D3DXVECTOR3 pos,const D3DXVECTOR3 Destpos ,const float fRot,const float fWidth,const float fHeight,const TYPE type);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetDestPos(const D3DXVECTOR3 DestPos) { m_DestPos = DestPos; };
	D3DXVECTOR3 GetDestPos(void) { return m_DestPos; };
	void SetCounter(const int count) { m_nCounter = count; };
	int GetCounter(void) { return m_nCounter; };
private:

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];	// テクスチャ
	D3DXVECTOR3 m_DestPos;
	TYPE type;
	int m_nCounter;			// カウンター
};

//====================================
// タイトルロゴ
//====================================
class CTitleLogo :public CInfoUI
{
public:
	typedef enum
	{
		LOGOTYPE_1 = 0,	// はぶらし
		LOGOTYPE_MAX,
	}LOGOTYPE;
	CTitleLogo();
	~CTitleLogo();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTitleLogo *Create(const LOGOTYPE type);
private:
	LOGOTYPE m_type;

};


#endif