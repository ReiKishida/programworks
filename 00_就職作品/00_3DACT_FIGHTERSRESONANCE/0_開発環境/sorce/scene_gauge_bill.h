//=============================================================================
//
// �V�[���Q�[�W����[2D �|���S��] [scenegauge.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _SCENEGAUGEBILL_H_
#define _SCENEGAUGEBILL_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
//*****************************************************************************
// �V�[��2D�̃N���X�\����
//*****************************************************************************
class CSceneGaugeBillBoard : public CScene
{
public:
	CSceneGaugeBillBoard::CSceneGaugeBillBoard(int nPriority = 3, OBJTYPE type = OBJTYPE_SCENEGAUGE);	//�R���X�g���N�^
	CSceneGaugeBillBoard::~CSceneGaugeBillBoard();	//�f�X�g���N�^
	static CSceneGaugeBillBoard *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);//�G�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//�e�N�X�`���̊��蓖��
	void SetColor(D3DXCOLOR col);	//�F�̐ݒ�


	//�|���S���̑傫���̎擾�A�ݒ�
	void SetGaugeWidth(float fWidth);
	void SetGaugeHeight(float fWidth);


	//���ݒl�̐ݒ�
	void SetCurrentValue(float fCurrentValue);

	//�ő�l�̐ݒ�
	void SetMaxValue(float fMaxValue);

	//�ʒu���̑��
	void SetPosGauge(D3DXVECTOR3 pos);


protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;		// �|���S���̈ʒu
	D3DXMATRIX              m_mtxWorld;		//�}�g���b�N�X���
	float m_fLengthGauge;					//�|���S���̒���
	float m_fAngleGauge;					//�|���S���̊p�x
	float m_fWidth;					//�|���S���̕�
	float m_fHeight;					//�|���S���̍���
	int m_nCntflame;						//�J�E���g
	float m_fCurrentValue;					//���݂̒l
	float m_fMaxValue;						//�Q�[�W�̍ő�l
	bool  m_bDamage;						//�_���[�W���
};

#endif