//=============================================================================
//
// �Q�[�W����[2D �|���S��] [ui_gauge.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _UIGAUGE_H_
#define _SUIGAUGE_H_

#include "main.h"
#include "scene_gauge.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_GAUGE_TEX (3)
//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
//*****************************************************************************
// �V�[��2D�̃N���X�\����
//*****************************************************************************
class CUiGauge : public CSceneGauge
{
public:
	typedef enum
	{
		GAUGETYPE_NULL = 0,				//��̃Q�[�W
		GAUGETYPE_UNDER ,				//�Q�[�W�̉��n
		GAUGETYPE_GREEN,				//�{�̗�
		GAUGETYPE_RED,					//���̗�
		GAUGETYPE_SKILL,				//�X�^���Q�[�W
		GAUGETYPE_FACE_PLAYER,			//��O��(�v���C���[)
		GAUGETYPE_FACE_BOSS,			//��O��(�{�X)
		GAUGETYPE_MAX					//�Q�[�W�̎�ނ̍ő吔
	}GAUGETYPE;

	CUiGauge::CUiGauge(int nPriority = 7, OBJTYPE type = OBJTYPE_UIGAUGE);	//�R���X�g���N�^
	CUiGauge::~CUiGauge();	//�f�X�g���N�^
	static HRESULT Load(void);									//�e�N�X�`���̓ǂݍ���
	static void Unload(void);									//�e�N�X�`���̔j��
	static CUiGauge *Create(D3DXVECTOR3 pos,D3DXCOLOR col, float fWidth, float fHeight,float fCurrentGauge,float fMaxGauge, GAUGETYPE type);//�Q�[�W�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void AddValue(float fValue);
	void AddValueSlip(float fValue);


private:
	static LPDIRECT3DTEXTURE9		m_apTexture[MAX_GAUGE_TEX];		// �e�N�X�`���ւ̃|�C���^
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