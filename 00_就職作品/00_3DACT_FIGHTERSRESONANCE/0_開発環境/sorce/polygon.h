//=============================================================================
//
// �|���S����������[2D �|���S��] [polygon.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************



class CPolygon
{
public:

	CPolygon();	//�R���X�g���N�^
	~CPolygon();	//�f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	CPolygon *Create(D3DXVECTOR3 pos);//�G�̐���
	void SetTexture(float fTex_X, float fTex_Y);//�e�N�X�`���̐ݒ�
	void SetPolygonTexture(int nType);//�e�N�X�`���̐ݒ�
	void SetPolygon(int nType);	//�̐�
	int GetPolygon();

	void BindTexture(LPDIRECT3DTEXTURE9 tex);//�e�N�X�`���̊��蓖��


	//�|���S���̑傫���̐ݒ�
	void SetPolygonWidth(float fWidth);
	void SetPolygonHeight(float fWidth);

	//�|���S���̈ʒu���̎擾�A�ݒ�
	 D3DXVECTOR3 GetPosPolygon(void);
	void SetPosPolygon(D3DXVECTOR3 pos);

	//�|���S���̊p�x���̐ݒ�A�ݒ�
	D3DXVECTOR3 GetRotPolygon(void);
	void SetRotPolygon(D3DXVECTOR3 rot);

	//�|���S���̒����̎擾�A�ݒ�
	float GetLengthPolygon(void);
	void SetLengthPolygon(float fLength);

	//�|���S���̑傫���̎擾�A�ݒ�
	 float GetWidthPolygon(void);
	 float GetHeightPolygon(void);

	 //�|���S���̐F���̎擾�A�ݒ�
	 D3DXCOLOR GetPolygonColor(void);
	 void SetPolygonColor(D3DXCOLOR col);	//�F�̐ݒ�

	 //���C�t�̐ݒ�
	 void SetLife(int nLife);

	 //�e�N�X�`���A�j���[�V����
	 void SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim);

	 //�e�N�X�`�����ݒ�
	 void SetTexture_All(float fTex_X, float fTex_X_0, float fTex_Y_0, float fTex_Y);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_posPolygon;			// �|���S���̈ʒu
	D3DXVECTOR3				m_rotPolygon;			// �|���S���̉�]
	float m_fLengthPolygon;	//�|���S���̒���
	float m_fAnglePolygon;	//�|���S���̊p�x
	float m_fPolygonWidth;	//�|���S���̕�
	float m_fPolygonHeight;	//�|���S���̍���
	int m_nCntflame;		//�J�E���g
	int m_nNumScene2D;	//2D�|���S����
	static CManager *m_pManager;//�}�l�[�W��
	int m_nLife;//���C�t
	int m_nType;//���

#ifdef _DEBUG
	LPD3DXFONT				m_pFont;			// �t�H���g�ւ̃|�C���^
#endif

};

#endif