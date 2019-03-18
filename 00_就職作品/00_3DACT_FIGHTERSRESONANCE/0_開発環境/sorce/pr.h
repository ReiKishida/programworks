//=============================================================================
//
// �V�[��3D����[3D �|���S��] [scene3D.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _PR_H_
#define _PR_H_

#include "main.h"
#include "scene3D.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TEXTURE_PR (7)
//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
//*****************************************************************************
// �V�[��3D�̃N���X�\����
//*****************************************************************************
class CPr : public CScene3D
{
public:
	typedef enum
	{
		PRTYPE_BOARD_STREAM = 0,			//�d���f���i����镶���j
		PRTYPE_BOARD_SWITCH_0,		//�d���f���i�؂芷��1�j
		PRTYPE_BOARD_SWITCH_1,		//�d���f���i�؂芷��2�j
		PRTYPE_BOARD_ADVERTISE_0,	//�L���i0�j
		PRTYPE_BOARD_ADVERTISE_1,	//�L���i1�j
		PRTYPE_BOARD_ADVERTISE_2,	//�L���i2�j
		PRTYPE_BOARD_ADVERTISE_3,	//�L���i2�j
		PRTYPE_MAX
	}PRTYPE;
	static HRESULT Load(void);
	static void Unload(void);
	CPr::CPr(int nPriority = 4, OBJTYPE type = OBJTYPE_PR);	//�R���X�g���N�^
	CPr::~CPr();	//�f�X�g���N�^
	static CPr *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, PRTYPE type);//�G�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��


private:
	static LPDIRECT3DTEXTURE9	m_apTexture[MAX_TEXTURE_PR];		// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3				m_pos;			// �|���S���̈ʒu
	D3DXVECTOR3				m_rot;			// �|���S���̉�]
	D3DXVECTOR2 m_aPr[4];	//�w�i�̍ő吔
	D3DXCOLOR m_col;		//�F���
	float m_fWidth;			//��
	float m_fHeight;		//����
	PRTYPE m_type;
	int m_nCntFrame;
	int m_nRand;
};

#endif