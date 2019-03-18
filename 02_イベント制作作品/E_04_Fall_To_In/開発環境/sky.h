#pragma once
//=============================================================================
//
// イベントの処理 [Event.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _SKY_H_
#define _SKY_H_

#include "main.h"
#include "scene3DSphere.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SKY (5)

//====================================
// 停電クラス
//====================================
class CSky :CScene3DSphere
{
public:
	typedef enum
	{
		TYPE_EARTH=0,
		TYPE_UTHU,
		TYPE_EARTH_1,
		TYPE_EARTH_2,
		TYPE_MOON,
		TYPE_MAX
	}TYPE;
	CSky();
	~CSky();
	static HRESULT Load(void);
	static void UnLoad(void);

	static CSky *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const float fHeight,
		const int nCutH, const int nCutV, const D3DXCOLOR col, CSky::TYPE type,const int nDirection = 0);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];
	TYPE m_type;
};


#endif