//=============================================================================
//
// �Q�[�W����[2D �|���S��] [ui_gauge.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _UIGAUGE_H_
#define _SUIGAUGE_H_

#include "main.h"
#include "scene_gauge_bill.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
//*****************************************************************************
// �V�[��2D�̃N���X�\����
//*****************************************************************************
class CUiGaugeBillBoard : public CSceneGaugeBillBoard
{
public:
	typedef enum
	{
		GAUGETYPE_UI = 0,				//�v���C���[�̊�O��
		GAUGETYPE_UNDER,				//�Q�[�W�̉��n
		GAUGETYPE_GREEN,				//�{�̗�
		GAUGETYPE_RED,					//���̗�
		GAUGETYPE_SKILL,					//�X�^���Q�[�W
		GAUGETYPE_MAX					//�Q�[�W�̎�ނ̍ő吔
	}GAUGETYPE;
	CUiGaugeBillBoard::CUiGaugeBillBoard(int nPriority = 3, OBJTYPE type = OBJTYPE_UIGAUGEBILLBOARD);	//�R���X�g���N�^
	CUiGaugeBillBoard::~CUiGaugeBillBoard();	//�f�X�g���N�^
	static HRESULT Load(void);									//�e�N�X�`���̓ǂݍ���
	static void Unload(void);									//�e�N�X�`���̔j��
	static CUiGaugeBillBoard *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fCurrentGauge, float fMaxGauge, GAUGETYPE type);//�Q�[�W�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void AddValue(float fValue);
	void AddValueSlip(float fValue);


private:
	static LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;			// �|���S���̈ʒu
	D3DXVECTOR3				m_rot;			// �|���S���̉�]
	D3DXCOLOR			    m_col;			//�|���S���̐F
	float m_fWidth;							//�|���S���̕�
	float m_fHeight;						//�|���S���̍���
	float m_fCurrentGauge;					//���݂̒l
	float m_fMaxGauge;						//�Q�[�W�̍ő�l
	float m_fDestValue;						//�ړI�̒l
	float m_fValue;							//���݂̎Z�o�����l
	float m_fValueOld;						//�ߋ��̎Z�o�����l
	GAUGETYPE m_type;						//�Q�[�W�̎��
	int m_nCntFrame;						//�t���[���̃J�E���g
};

#endif