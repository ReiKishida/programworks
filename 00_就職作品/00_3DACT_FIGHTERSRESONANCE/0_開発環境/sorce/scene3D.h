//=============================================================================
//
// �V�[��3D����[3D �|���S��] [scene3D.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
//*****************************************************************************
// �V�[��3D�̃N���X�\����
//*****************************************************************************
class CScene3D : public CScene
{
public:

	CScene3D::CScene3D(int nPriority = 3, OBJTYPE type = OBJTYPE_SCENE3D);	//�R���X�g���N�^
	CScene3D::~CScene3D();	//�f�X�g���N�^
	static CScene3D *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);//�G�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	int GetNum2DSceneAll(void);	//2D�V�[�����̎擾
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//�e�N�X�`���̊��蓖��
	void SetColor(D3DXCOLOR col);	//�F�̐ݒ�
	void SetPriority(int nPriority);//�D�揇��

	//�|���S���̈ʒu���̎擾�A�ݒ�
	D3DXVECTOR3 GetPosPolygon(void);
	void SetPosPolygon(D3DXVECTOR3 pos);

	//�|���S���̊p�x���̎擾�A�ݒ�
	D3DXVECTOR3 GetRotPolygon(void);
	void SetRotPolygon(D3DXVECTOR3 pos);

	void SetWidth(float fWidth);
	void SetHeight(float fHeight);

	//�w�i�X�N���[��
	void ScrollPolygon(D3DXVECTOR2 Scroll_0, D3DXVECTOR2 Scroll_1, D3DXVECTOR2 Scroll_2, D3DXVECTOR2 Scroll_3);
	void SetTexture_All(float fTex_X, float fTex_X_0, float fTex_Y_0, float fTex_Y);//�e�N�X�`���̐ݒ�


private:
	LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorldPolygon;			//�|���S���̃}�g���b�N�X���
	D3DXVECTOR3				m_posPolygon;			// �|���S���̈ʒu
	D3DXVECTOR3				m_rotPolygon;			// �|���S���̉�]
	static CManager *m_pManager;//�}�l�[�W��
	int m_nCntflame;		//�J�E���g
	int m_nNumScene2D;	//2D�|���S����
	int m_nPriority;//�D��x
	float m_fWidth;	//�|���S���̕�
	float m_fHeight; //�|���S���̉��s��
	float m_fnor; //�|���S���̖@��
};

#endif