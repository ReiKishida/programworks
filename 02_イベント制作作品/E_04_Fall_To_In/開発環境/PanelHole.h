#pragma once
//=============================================================================
//
// ぱねるの処理 [Event.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _PANELHOLE_H_
#define _PANELHOLE_H_

#include "main.h"
#include "scene3D.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


//====================================
// 停電クラス
//====================================
class CPanelHole :CScene3D
{
public:

	typedef enum
	{
		TYPE_SPACE_NONE = 0,
		TYPE_SPACE_1,
		TYPE_SPACE_2,
		TYPE_SPACE_3,
		TYPE_SPACE_4,

		TYPE_EARTH_NONE ,
		TYPE_EARTH_1,
		TYPE_EARTH_2,
		TYPE_EARTH_3,
		TYPE_EARTH_4,


		TYPE_HELL_NONE,
		TYPE_HELL_1,
		TYPE_HELL_2,
		TYPE_HELL_3,
		TYPE_HELL_4,
		TYPE_MAX
	}TYPE;
	CPanelHole();
	~CPanelHole();
	static HRESULT Load(void);
	static void UnLoad(void);

	static CPanelHole *Create(D3DXVECTOR3 pos, float fWidth, float fDepth, TYPE type,int nNum);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPlayer::MOTION_TYPE m_aMotionType[TYPE_MAX];
	static int m_nBreakId;
	 int m_nCounter;
	int Collision(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld , const float fWidth, int nMotion);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];
	TYPE m_type;
	int m_nNumber;
	bool m_bBreak;
	D3DXVECTOR3 m_move;
};

class CPanel
{
public:
	typedef enum
	{
		STAGE_SPACE = 0,
		STAGE_EARTH,
		STAGE_HELL,
		STAGE_MAX,
	}STAGE;

	CPanel();
	~CPanel();

	static void SetPanel(const D3DXVECTOR3 pos,const int nHole, const STAGE stage);
private:
	static int m_nNumAll;
};
#endif