////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���b�Z�[�W�E�B���h�E���� [message.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "main.h"
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef enum
{
	MESSAGETEX_TITLE = 0,		// �^�C�g��
	MESSAGETEX_TEIJI,		// �^�C�g��
	MESSAGETEX_KAEROU,		// �^�C�g��
	MESSAGETEX_SOUSA,		// �^�C�g��
	MESSAGETEX_KAERU ,		// �^�C�g��
	MESSAGETEX_MAX,
}MESSAGETEX;
typedef enum
{
	MESSAGESTATE_NONE = 0,
	MESSAGESTATE_SET,
	MESSAGESTATE_DELETE,
	MESSAGESTATE_MAX,
}MESSAGESTATE;
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXCOLOR col;			// �F
	MESSAGETEX nType;			// ���
	MESSAGESTATE nState;	// ���
	float fWidth;			// ��
	float fHeight;			// ����
	float fDiffWidth;		// ��
	float fDiffHeight;		// ����
	bool bUse;				// �g�p����Ă��邩�ǂ���
}MessageWindow;
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitMessageWindow(void);
void UninitMessageWindow(void);
void UpdateMessageWindow(void);
void DrawMessageWindow(void);
void SetMessageWindow(D3DXVECTOR3 pos, float DiffWidth, float DiffHigeht, MESSAGETEX nType);
void DeleteMessageWndow(void);
MessageWindow *GetMessageWindow(void);
#endif
