////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �^�C�g���̏��� [title.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _TITLE_H_
#define _TITLE_H_

//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef enum
{
	TITLETYPE_LOGOBG = 0,		// ���S�w�i
	TITLETYPE_LOGO,			// ���S
	TITLETYPE_PRESSENTER,	// PressEnter
	TITLETYPE_MAX			// �ő�
}TITLETYPE;
typedef enum
{
	TITLEFADE_NONE = 0,	// �����Ȃ�
	TITLEFADE_FADEIN,	// �t�F�[�h�C��
	TITLEFADE_FADEOUT	// �t�F�[�h�A�E�g
}TITLEFADE;
typedef enum
{
	TITLESTATE_NONE=0,		// �ʏ�
	TITLESTATE_END			// �I���
}TITLESTATE;
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 Initpos;// �����ʒu
	D3DXCOLOR col;		// �F
	D3DXVECTOR3 move;	// �ړ� 
	TITLETYPE nType;	// ���
	int nCounterAnim;	// �A�j���[�V�����J�E���^�[
	int nPatternAnim;	// �A�j���[�V�����p�^�[��
	int nCounterFade;	// �t�F�[�h�J�E���^
	float fWidth;		// ��
	float fHeight;		// ����
	TITLEFADE nFade;	// �t�F�[�h
	bool bUse;			// �g�p���Ă��邩�ǂ���
} Title;
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
void SetTitle(D3DXVECTOR3 pos,TITLETYPE nType,float fWidth,float fHeight);

#endif // !_TITLE_H_

