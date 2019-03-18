////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �RD�|���S������ [scene3D.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"
//******************************************************************************************************************************
// 3D�|���S���̃N���X
//******************************************************************************************************************************
class CScene3D : public CScene
{
public:

	CScene3D(int nPriority = CScene::PRIORITY_INIT);
	~CScene3D();
	static HRESULT Load(void);	// �e�N�X�`������
	static void UnLoad(void);	// �e�N�X�`���j��

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CScene3D *Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth);

	D3DXVECTOR3 GetPos(void) { return m_pos; };			// ���W�擾
	D3DXVECTOR3 GetMove(void) { return m_move; };		// �ړ��ʎ擾
	float GetWidth(void) { return m_fWidth; }			// ���擾
	float GetHeight(void) { return m_fHeight; }			// �����擾
	float GetDepth(void) { return m_fDepth; }			// ���s���擾
	D3DXVECTOR3 GetRot(void) { return m_rot; };			// ��]�擾
	D3DXMATRIX	GetMtx(void) { return m_mtxWorld; };	// �}�g���b�N�X�擾
	D3DXCOLOR GetColor(void) { return m_col; };
	void SetPos(const D3DXVECTOR3 pos);
	void SetMove(const D3DXVECTOR3 move);
	void SetSize(const float fWidth, const float fHeight, const float fDepth);	// �傫���ݒ�
	void SetRot(const D3DXVECTOR3 fRot);	// ��]�ݒ�
	void SetColor(const D3DXCOLOR col); // �F�ݒ�
	void SetVtx(void);
	void Collision(const D3DXVECTOR3 pos);

private:
	static LPDIRECT3DTEXTURE9	m_pTextureThis;		// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_aVtxPos[4];
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_move;	// �ړ���
	D3DXVECTOR3 m_rot;	// ����
	D3DXCOLOR	m_col;	// �F
	D3DXMATRIX	m_mtxWorld;// ���[���h�}�g���b�N�X
	float		m_fWidth;		// ��
	float		m_fHeight;		// ����
	float		m_fDepth;		// ���s��
	float		m_fAngle;	// �p�x
	float		m_fLength;	// ����
};
#endif
