#pragma once
//=============================================================================
//
// ���m�点UI�̏��� [infoUI.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _INFOUI_H_
#define _INFOUI_H_

#include "scene2D.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_INFOUI (30)
//====================================
// ���m�点UI�N���X
//====================================
class CInfoUI : public CScene2D
{
public:
	typedef enum
	{
		TYPE_TITLE_LOGO = 0,		// �^�C�g�����S
		TYPE_SHUTU,					// �^�C�g���f��[6]
		TYPE_WIN,					// �^�C�g���f��[6]
		TYPE_LOOSE,					// �^�C�g���f��[6]
		TYPE_DRAW,					// �^�C�g���f��[6]
		TYPE_MAX
	}TYPE;

	CInfoUI(int nPriority = 5);
	~CInfoUI();
	static HRESULT Load(void);	// �e�N�X�`������
	static void UnLoad(void);	// �e�N�X�`���j��
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
	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];	// �e�N�X�`��
	D3DXVECTOR3 m_DestPos;
	TYPE type;
	int m_nCounter;			// �J�E���^�[
};

//====================================
// �^�C�g�����S
//====================================
class CTitleLogo :public CInfoUI
{
public:
	typedef enum
	{
		LOGOTYPE_1 = 0,	// �͂Ԃ炵
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