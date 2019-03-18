#pragma once
//=============================================================================
//
// �t�F�[�h�̏��� [fade.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "manager.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//====================================
// �t�F�[�h�N���X
//====================================
class CFade
{
public:
	typedef enum
	{
		FADE_NONE = 0,		// �������Ă��Ȃ����
		FADE_IN,			// �t�F�[�h�C�����
		FADE_OUT,			// �t�F�[�h�A�E�g���
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
	static HRESULT Load(void);	// �e�N�X�`������
	static void UnLoad(void);	// �e�N�X�`���j��
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
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	static TYPE m_type;
	static D3DXVECTOR3		m_pos;			// ���W
	static D3DXCOLOR		m_col;			// �F
	static float m_fWidth;					// ��
	static float m_fHeight;					// ����
	static D3DXVECTOR2 m_uv;
	static CManager::MODE m_modeNext;	// ���̃��[�h
	static FADE m_fade;					// �t�F�[�h
};
#endif