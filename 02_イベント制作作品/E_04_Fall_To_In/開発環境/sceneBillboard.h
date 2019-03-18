//=============================================================================
//
// �r���{�[�h���� [sceneBillboard.h]
// Author : Okabe Kazuki
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"
#include "scene.h"

//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
class CBillboard :public CScene
{
public:
	typedef enum
	{// ���_�̈ʒu
		VTXTYPE_CENTER = 0,	// ����
		VTXTYPE_BASE,		// ��
		VTXTYPE_LEFTBASE,	// ����
	}VTXTYPE;
	CBillboard(int nPriority = CScene::PRIORITY_INIT);
	~CBillboard();
	static HRESULT Load(void);	// �e�N�X�`������
	static void UnLoad(void);	// �e�N�X�`���j��

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBillboard *Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);

	D3DXVECTOR3 GetPos(void) { return m_pos; };			// ���W�擾
	float GetWidth(void) { return m_fWidth; }			// ���擾
	float GetHeight(void) { return m_fHeight; }			// �����擾
	float GetRot(void) { return m_rot; };				// ��]�擾
	D3DXCOLOR GetColor(void) { return m_col; };			// �F�擾
	D3DXMATRIX	GetMtx(void) { return m_mtxWorld; };	// �}�g���b�N�X�擾

	void SetPos(const D3DXVECTOR3 pos);									// �ʒu�ݒ�
	void SetSize(const float fWidth, const float fHeight);				// �傫���ݒ�
	void SetColor(D3DXCOLOR col);										// �F
	void SetRot(const float rot);										// ��]�ݒ�
	void SetVtx(void);													// �o�b�t�@�̐ݒ�
	void SetVtxType(const VTXTYPE type);
	static void SetZenable(const bool Zenable);
protected:
private:
	static LPDIRECT3DTEXTURE9	m_pTextureThis;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;				// �ʒu
	D3DXCOLOR	m_col;
	D3DXMATRIX	m_mtxWorld;			// ���[���h�}�g���b�N�X
	float		m_rot;				// ����
	float		m_fWidth;			// ��
	float		m_fHeight;			// ����
	float		m_fAngle;			// �p�x
	float		m_fLength;			// ����
	static bool m_bZenable;			// Z�o�b�t�@
	VTXTYPE		m_VtxType;			// ���_�^�C�v
};

#endif
#pragma once
