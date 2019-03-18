//=============================================================================
//
// �V�[���Q�[�W����[2D �|���S��] [scenegauge.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _SCENEGAUGE_H_
#define _SCENEGAUGE_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
//*****************************************************************************
// �V�[��2D�̃N���X�\����
//*****************************************************************************
class CSceneGauge : public CScene
{
public:
	CSceneGauge::CSceneGauge(int nPriority = 7, OBJTYPE type = OBJTYPE_SCENEGAUGE);	//�R���X�g���N�^
	CSceneGauge::~CSceneGauge();	//�f�X�g���N�^
	static CSceneGauge *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);//�G�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//�e�N�X�`���̊��蓖��
	void SetTexture(float fTex_X, float fTex_Y);//�e�N�X�`���̐ݒ�
	void SetColor(D3DXCOLOR col);	//�F�̐ݒ�
	void SlipGauge(void);			//�Q�[�W����

	//�e�N�X�`���A�j���[�V����
	void SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim);
	void SetTextureAnimY(float fTex_X, float fTex_Y, int nPatternAnim, int nTex_Y);

	//�|���S���̑傫���̎擾�A�ݒ�
	void SetGaugeWidth(float fWidth);
	void SetGaugeHeight(float fWidth);

	//�|���S���̈ʒu���̎擾�A�ݒ�
	D3DXVECTOR3 GetPosGauge(void);
	void SetPosGauge(D3DXVECTOR3 pos);

	//�|���S���̑傫���̎擾�A�ݒ�
	float GetWidthGauge(void);
	float GetHeightGauge(void);

	//���ݒl�̐ݒ�
	void SetCurrentValue(float fCurrentValue);

	//�ő�l�̐ݒ�
	void SetMaxValue(float fMaxValue);



protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_posGauge;		// �|���S���̈ʒu
	D3DXVECTOR3				m_rotGauge;		// �|���S���̉�]
	float m_fLengthGauge;					//�|���S���̒���
	float m_fAngleGauge;					//�|���S���̊p�x
	float m_fGaugeWidth;					//�|���S���̕�
	float m_fGaugeHeight;					//�|���S���̍���
	int m_nCntflame;						//�J�E���g
	float m_fCurrentValue;					//���݂̒l
	float m_fMaxValue;						//�Q�[�W�̍ő�l
	bool  m_bDamage;						//�_���[�W���
};

#endif