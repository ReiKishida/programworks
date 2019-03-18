//=============================================================================
//
// �Q�[�W����[2D �|���S��] [ui_gauge.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _COMBOGAUGE_H_
#define _COMBOGAUGE_H_

#include "main.h"
#include "scene_gauge.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �V�[��2D�̃N���X�\����
//*****************************************************************************
class CComboGauge : public CSceneGauge
{
public:
	typedef enum
	{
		GAUGETYPE_UNDER = 0,				//�Q�[�W�̉��n
		GAUGETYPE_COMBO,				//�{�̗�
		GAUGETYPE_MAX					//�Q�[�W�̎�ނ̍ő吔
	}GAUGETYPE;
	CComboGauge::CComboGauge(int nPriority = 7, OBJTYPE type = OBJTYPE_COMBOGAUGE);	//�R���X�g���N�^
	CComboGauge::~CComboGauge();	//�f�X�g���N�^
	static HRESULT Load(void);									//�e�N�X�`���̓ǂݍ���
	static void Unload(void);									//�e�N�X�`���̔j��
	static CComboGauge *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fCurrentGauge, float fMaxGauge, GAUGETYPE type);//�Q�[�W�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	 void AddValueSlip(float fValue);
	 void SetComboGauge(float fValue);
	static float GetComboGauge(void);
	float GetCurrentGauge(void);
	void SetCurrentGauge(float fValue);

	static float GetComboGaugeMax(void);

private:
	static LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;			// �|���S���̈ʒu
	D3DXVECTOR3				m_rot;			// �|���S���̉�]
	D3DXCOLOR			    m_col;			//�|���S���̐F
	static float m_fComboGauge;				//�R���{��
	static float m_fComboGaugeMax;			//�ȑO�̃R���{��

	float m_fWidth;							//�|���S���̕�
	float m_fHeight;						//�|���S���̍���
	float m_fCurrentGauge;					//���݂̒l
	float m_fMaxGauge;						//�Q�[�W�̍ő�l
	float m_fDestValue;						//�ړI�̒l
	float m_fValue;							//����
	float m_fMaxValue;						//�ő吔
	float m_fReminder;
	GAUGETYPE m_type;						//�Q�[�W�̎��
	static int m_nCombo;							//�R���{��
	int m_nCntFrame;						//�t���[���̃J�E���g

};

#endif