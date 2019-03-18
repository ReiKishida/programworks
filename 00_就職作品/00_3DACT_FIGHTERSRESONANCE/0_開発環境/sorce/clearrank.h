//=============================================================================
//
// �w�i����[2D �|���S��] [bg.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _CLEARRANK_H_
#define _CLEARRANK_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TEXTURE_RANK (5)	//�e�N�X�`����

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CClearRank : public CScene
{
public:
	typedef enum
	{
		RANKMODE_SCORE = 0,	//�X�R�A
		RANKMODE_TIME,		//�^�C��
		RANKMODE_COMBO,		//�R���{
		RANKMODE_DAMAGE,	//�_���[�W
		RANKMODE_FINAL,		//�ŏI�X�R�A
		RANKMODE_MAX
	}RANKMODE;
	typedef enum
	{
		RANKTYPE_S = 0,	//S
		RANKTYPE_A,		//A
		RANKTYPE_B,		//B
		RANKTYPE_C,		//C
		RANKTYPE_D,		//D
		RANKTYPE_MAX		//�R���{�̍ő吔
	}RANKTYPE;//UI�̎��

	CClearRank(int nPriority = 7, OBJTYPE type = OBJTYPE_COMBO);	//�R���X�g���N�^
	~CClearRank();	//�f�X�g���N�^
	static CClearRank *Create(D3DXVECTOR3 pos,RANKMODE mode);	//�w�i�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);//�e�N�X�`���̓ǂݍ���
	static void Unload(void);//�e�N�X�`���̔j��

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE_RANK]; //���L�e�N�X�`���̃|�C���^
	D3DXCOLOR m_Color;
	D3DXVECTOR3 m_pos;
	int m_nCntColor;	//�J���[�J�E���g�p
	CScene2D *m_pScene2D;
	RANKTYPE m_Type;	//UI�̃^�C�v
	RANKMODE m_mode;	//���[�h
	float m_fCombo;		//�R���{��
	int m_nFinalScore;
};

#endif