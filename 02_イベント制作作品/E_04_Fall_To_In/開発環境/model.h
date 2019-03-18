//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// ���f������ [model.h]
// Author:�� �a��
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#ifndef _MODEL_H_
#define _MODEL_H_

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �C���N���[�h�t�@�C��
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#include "main.h"
#include "scene.h"
#include "motion.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �}�N����`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MAX_MODEL_TEX		(20)		// �e�N�X�`���̍ő吔

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �N���X��`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
class CModel
{ // ���f���N���X
public:
	//+*++*++*++*++*++*++*++*++*++*++*
	// �v���g�^�C�v�錾
	//+*++*++*++*++*++*++*++*++*++*++*
	CModel();
	~CModel();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(D3DXMATRIX mtxParent);
	void Draw(D3DXMATRIX mtxParent,D3DXVECTOR3 posMotion,D3DXVECTOR3 rotMotion);
	void Draw(D3DXMATRIX mtxParent, D3DXVECTOR3 posMotion, D3DXVECTOR3 rotMotion, CMotionData::Model model);

	// �e��ݒ�E�擾�֐�
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }										// �ʒu�ݒ�
	D3DXVECTOR3 GetPos(void) { return m_pos; }												// �ʒu�擾
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }										// ��]�ݒ�
	D3DXVECTOR3 GetRot(void) { return m_rot; }												// ��]�擾
	void SetMtxWorld(const D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }					// ���[���h�}�g���b�N�X�̐ݒ�
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }										// ���[���h�}�g���b�N�X�̎擾
	void SetParent(CModel *parent) { m_pParent = parent; }									// �e���f���ւ̃|�C���^�̐ݒ�
	void SetMesh(const LPD3DXMESH mesh) { m_pMesh = mesh; }									// ���b�V���ݒ�
	void SetBuffMat(const LPD3DXBUFFER buffMat) { m_pBuffMat = buffMat; }					// �}�e���A���ݒ�
	void SetNumMat(const DWORD numMat) { m_nNumMat = numMat; }								// �}�e���A�����̐��ݒ�
	void SetTexture(const LPDIRECT3DTEXTURE9 tex, const int nId) { m_pTexture[nId] = tex; }	// �e�N�X�`���ݒ�
	void BindModel(const LPD3DXMESH mesh, const LPD3DXBUFFER buffMat, const DWORD numMat);

	// �ÓI�����o�֐�
	static CModel *Create(const D3DXVECTOR3 pos);

private:
	//+*++*++*++*++*++*++*++*++*++*++*
	// �ϐ��錾
	//+*++*++*++*++*++*++*++*++*++*++*
	D3DXVECTOR3			m_pos;				// �ʒu�i���S���W�j
	D3DXVECTOR3			m_rot;				// ����
	D3DXMATRIX			m_mtxWorld;			// ���[���h�}�g���b�N�X
	CModel				*m_pParent;			// �e���f���ւ̃|�C���^
	LPD3DXMESH			m_pMesh;			// ���b�V���i���_�̏W�܂�j���ւ̃|�C���^
	LPD3DXBUFFER		m_pBuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD				m_nNumMat;			// �}�e���A�����̐�
	LPDIRECT3DTEXTURE9	m_pTexture[MAX_MODEL_TEX] = {};
};

#endif
