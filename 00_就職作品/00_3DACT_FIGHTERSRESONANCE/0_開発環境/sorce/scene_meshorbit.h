//=============================================================================
//
// �V�[��3D����[3D �|���S��] [scene3D.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _SCENEMESHORBIT_H_
#define _SCENEMESHORBIT_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_VERTEX (100)
//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
//*****************************************************************************
// �V�[��3D�̃N���X�\����
//*****************************************************************************
class CMeshOrbit : public CScene
{
public:
	CMeshOrbit::CMeshOrbit(int nPriority = 4, OBJTYPE type = OBJTYPE_SCENEORBIT);	//�R���X�g���N�^
	CMeshOrbit::~CMeshOrbit();	//�f�X�g���N�^
	static CMeshOrbit *Create();//�G�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//�e�N�X�`���̊��蓖��
	void SetColor(D3DXCOLOR col);	//�F�̐ݒ�
	void CreateOrbit(void);	//�O�Ղ̐���

	//�|���S���̈ʒu���̎擾�A�ݒ�
	D3DXVECTOR3 GetPosPolygon(void);
	void SetPosPolygon(D3DXVECTOR3 pos);

	//�|���S���̊p�x���̎擾�A�ݒ�
	D3DXVECTOR3 GetRotPolygon(void);
	void SetRotPolygon(D3DXVECTOR3 pos);

	void SetMtxParent(D3DXMATRIX *pMtx);	//�e�̃}�g���b�N�X�ւ̃|�C���^
private:
	static LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;			//�|���S���̃}�g���b�N�X���
	D3DXVECTOR3				m_posPolygon;		// �|���S���̈ʒu
	D3DXVECTOR3				m_rotPolygon;		// �|���S���̉�]
	int m_nCntflame;							//�J�E���g
	int m_nNumScene3D;							//3D�|���S����
	int m_nPriority;							//�D��x
	float m_fWidth;								//�|���S���̕�
	float m_fHeight;							//�|���S���̉��s��
	float m_fnor;								//�|���S���̖@��
	D3DXMATRIX *m_pMtxParent;					//�|���S���̃}�g���b�N�X���
	D3DXVECTOR3 m_aPosVertex[MAX_VERTEX];		//�Z�o�������_���W
	D3DXVECTOR3 m_aOffSet[2];					//�I�t�Z�b�g
};

#endif