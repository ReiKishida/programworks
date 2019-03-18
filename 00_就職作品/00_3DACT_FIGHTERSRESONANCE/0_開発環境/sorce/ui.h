//=============================================================================
//
// �w�i����[2D �|���S��] [bg.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TEXTURE_UI (8)	//�e�N�X�`����
#define MAX_UI (4)			//UI��
#define MAX_UI_TUTORIAL (8)			//UI��

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CUi : public CScene2D
{
public:
	typedef enum
	{
		UIMODE_NONE = 0,
		UIMODE_TUTORIAL,
		UIMODE_GAME,
		UIMODE_MAX
	}UIMODE;
	typedef enum
	{
		UITYPE_SCORE = 0,			//�X�R�A
		UITYPE_TIME,				//�^�C���̕���
		UITYPE_COLON,				//�R����
		UITYPE_DOT,					//�h�b�g
		UITYPE_CONTROLL_ARCADE,		//����
		UITYPE_CONTROLL_KEYBOARD,	//����
		UITYPE_SKILL,
		UITYPE_END,
		UI_TYP_MAX				//UI�̍ő吔
	}UITYPE;//UI�̎��
	CUi(int nPriority = 7, OBJTYPE type = OBJTYPE_UI);	//�R���X�g���N�^
	~CUi();	//�f�X�g���N�^
	static CUi *Create(UIMODE mode);	//�w�i�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);//�e�N�X�`���̓ǂݍ���
	static void Unload(void);//�e�N�X�`���̔j��

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE_UI]; //���L�e�N�X�`���̃|�C���^
	D3DXCOLOR m_Color;
	int m_nCntColor;	//�J���[�J�E���g�p
	CScene2D *m_pScene2D[MAX_UI];
	CScene2D *m_pScene2D_Tuto[MAX_UI_TUTORIAL];
	UIMODE m_mode;	//UI�̃��[�h
	UITYPE m_Type;	//UI�̃^�C�v
};

#endif