#pragma once
//=============================================================================
//
// �QD�I�u�W�F�N�g���� [scene.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

//====================================
// 2D�I�u�W�F�N�g�N���X
//====================================
class CScene2D : public CScene
{
public:
	typedef enum
	{// ���_�̈ʒu
		VTXTYPE_CENTER = 0,	// ����
		VTXTYPE_BASE,		// ��
		VTXTYPE_LEFT,		// ��
		VTXTYPE_RIGHT,		// �E
		VTXTYPE_MAX,
	}VTXTYPE;

	CScene2D(int nPriority = CScene::PRIORITY_INIT);
	~CScene2D();
	static HRESULT Load(void);	// �e�N�X�`������
	static void UnLoad(void);	// �e�N�X�`���j��

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CScene2D *Create(const D3DXVECTOR3 pos, const float fWidth,const float fHeight);

	void SetPos(D3DXVECTOR3 pos);
	void SetInitPos(D3DXVECTOR3 pos);
	void SetSize(float fWidth, float fHeight);	// �傫���ݒ�
	void SetRot(float fRot);							// ��]�ݒ�
	void SetColor(D3DXCOLOR col);						// �F
	void SetVtx(void);							// ���_�ݒ�
	void SetPattern(const D3DXVECTOR2 uv, const int nPatternX, const int nPatternY, const bool bTurn = false);
	void SetTex(D3DXVECTOR2 tex);
	void SetVtxType(VTXTYPE VtxType);
	void SetUse(const bool bUse);
	void SetStamp(const float fWidth, const float fHeight);
	D3DXVECTOR3 GetPos(void) { return m_pos; }			//���W
	D3DXVECTOR3 GetInitPos(void) { return m_Initpos; };	// �������W
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; };	// �O����W�擾
	float GetWidth(void) { return m_fWidth; }			// ���擾
	float GetHeight(void) { return m_fHeight; }			// �����擾
	float GetRot(void) {return m_fRot;};				// ��]�擾
	D3DXCOLOR GetColor(void) {return m_col;};			// �F�擾
	D3DXVECTOR2 GetTex(void) { return m_tex; };
	LPDIRECT3DVERTEXBUFFER9 GetBuff(void) { return m_pVtxBuff; };	// ���_�擾
	bool GetUse(void) { return m_bUse; };
protected:
private:
	static LPDIRECT3DTEXTURE9	m_pTextureThis;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;			// �ʒu
	D3DXVECTOR3				m_Initpos;			// �ʒu
	D3DXVECTOR3				m_posOld;		// �ʒu
	D3DXCOLOR				m_col;			// �F
	float					m_fWidth;		// ��
	float					m_fHeight;		// ����
	D3DXVECTOR2				m_tex;			// �e�N�X�`�����Z���W
	float					m_fAngle[2];	// �p�x
	float					m_fLength[2];	// ����
	float					m_fRot;			// ��]��
	VTXTYPE					m_VtxType;		// ���_�^�C�v
	bool					m_bUse;			// �g�p����Ă��邩�ǂ���
};
#endif