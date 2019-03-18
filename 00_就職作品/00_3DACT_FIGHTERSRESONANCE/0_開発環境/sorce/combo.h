//=============================================================================
//
// �w�i����[2D �|���S��] [bg.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _COMBO_H_
#define _COMBO_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TEXTURE_COMBO (6)	//�e�N�X�`����

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CCombo : public CScene
{
public:
	typedef enum
	{
		COMBOTYPE_S = 0,	//S
		COMBOTYPE_A,		//A
		COMBOTYPE_B,		//B
		COMBOTYPE_C,		//C
		COMBOTYPE_D,		//D
		COMBOTYPE_NONE,	//NONE
		COMBOTYPE_MAX		//�R���{�̍ő吔
	}COMBOTYPE;//UI�̎��
	CCombo(int nPriority = 7, OBJTYPE type = OBJTYPE_COMBO);	//�R���X�g���N�^
	~CCombo();	//�f�X�g���N�^
	static CCombo *Create(void);	//�w�i�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);//�e�N�X�`���̓ǂݍ���
	static void Unload(void);//�e�N�X�`���̔j��

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE_COMBO]; //���L�e�N�X�`���̃|�C���^
	D3DXCOLOR m_Color;
	int m_nCntColor;	//�J���[�J�E���g�p
	CScene2D *m_pScene2D;
	COMBOTYPE m_Type;	//UI�̃^�C�v
	float m_fCombo;		//�R���{��
};

#endif