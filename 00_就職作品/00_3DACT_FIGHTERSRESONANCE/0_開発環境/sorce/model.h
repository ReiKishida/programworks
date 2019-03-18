//=============================================================================
//
// ���f���\������[3D �|���S��] [model.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PARTS (29)
#define MAX_TEXTURE (2)
//*****************************************************************************
// �V�[��3D�̃N���X�\����
//*****************************************************************************
class CMeshOrbit;
class CCollision;
class CModel
{
public:

	CModel();	//�R���X�g���N�^
	~CModel();	//�f�X�g���N�^
	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	//�G�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetPos(D3DXVECTOR3 pos);	//�ʒu���̑��
	void SetRot(D3DXVECTOR3 rot);	//�p�x���̑��
	void SetMtx(D3DXMATRIX mtx);	//�p�x���̑��
	void CreateOrbit(void);			//�O�Ղ̐���
	void CreateAttackCollision(void);	//�U������̐���
	D3DXMATRIX GetMatrix(void);				//�}�g���b�N�X�̎擾
	void Load(void);						//�g�p����p�[�c�̓ǂݍ���
	void Unload(void);						//�g�p����p�[�c�̔j��
	void SetParent(CModel *pModel);			//�e�q�֌W�̐ݒ�
	void BindModel(LPD3DXMESH mesh, LPD3DXBUFFER buffmat, DWORD nummat,  LPDIRECT3DTEXTURE9 tex);	//�e�N�X�`���̊��蓖��
	void SetColor(D3DXCOLOR col);	//�F���̑��


private:
	 LPDIRECT3DTEXTURE9			 m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	 LPD3DXMESH					 m_pMesh;			//���b�V�����ւ̃|�C���^
	 LPD3DXBUFFER				 m_pBuffMat;		//�}�e���A�����ւ̃|�C���^
	 DWORD						 m_nNumMat;			//�}�e���A���̏��̐�
	 D3DXMATERIAL				*m_pMat;			//�}�e���A���ւ̃|�C���^
	CModel						*m_pParent;			//�e���f���̃|�C���^
	CCollision *m_pCollisionAttack;					// �U�����̓����蔻��̃|�C���^
	int m_nNumParts;								//�p�[�c��
	int m_nMaxParts;								//�p�[�c�̍ő吔
	D3DXMATRIX m_mtxWorld;							//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;								//�ʒu
	D3DXVECTOR3 m_rot;								//����
	static CMeshOrbit *m_pMeshOrbit;				//�O�Ղ̃|�C���^
};

#endif