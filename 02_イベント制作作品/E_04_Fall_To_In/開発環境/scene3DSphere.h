//=============================================================================
//
// 3D���b�V���X�t�B�A�̏��� [scene3DSphere.h]
// Author : �T�c ����
//
//=============================================================================
#ifndef _SCENE3DSPHERE_H_
#define _SCENE3DSPHERE_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CScene3DSphere : public CScene
{// 3D���b�V���X�t�B�A�N���X(CScene�̔h���N���X)
public:
	CScene3DSphere(int nPriority = CScene::PRIORITY_INIT);
	~CScene3DSphere();
	static CScene3DSphere *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const float fHeight, const int nCutH, const int nCutV, const D3DXCOLOR col, const int nDirection = 0);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void MakeVertex(void);
	void SetAll(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const float fHeight, const int nCutH, const int nCutV, const D3DXCOLOR col, const int nDirection);
	void SetPosition(const D3DXVECTOR3 pos);
	void SetTexMove(const D3DXVECTOR2 tex);
	D3DXVECTOR3 GetPosition(void) { return m_pos; };
	D3DXMATRIX GetMtx(void) { return m_mtxWorld; };
	D3DXVECTOR2 GetTexMove(void) { return m_texMove; };
private:
	LPDIRECT3DTEXTURE9			m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffLid;	// ���_�o�b�t�@�ւ̃|�C���^[�W]
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX					m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3					m_pos;			// �ʒu
	D3DXVECTOR3					m_rot;			// ����
	D3DXCOLOR					m_col;			// �J���[
	D3DXVECTOR2					m_texMove;		// �e�N�X�`���ړ�
	float						m_fRadius;		// ���a
	float						m_fHeight;		// ����
	int							m_nCutH;		// �����̕�����
	int							m_nCutV;		// �����̕�����
	int							m_nDirection;	// ����[ �\ = 0, �� = 1 ]
	int							m_nNumVtx;		// ���_��
	int							m_nNumPolygon;	// �|���S����
	int							m_nNumIdx;		// �C���f�b�N�X��
};

#endif
