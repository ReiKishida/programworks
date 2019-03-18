//=============================================================================
//
// �T�E���h���� [sound.h]
// Author :Kishida Rei
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*************************************
// �T�E���h�t�@�C��
//*************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM_RESULT,		// ���U���g
	SOUND_LABEL_BGM_RANKING,	// �����L���O
	SOUND_LABEL_SE_CHARGE,		// �`���[�W
	SOUND_LABEL_SE_HIT000,		// �q�b�g��0
	SOUND_LABEL_SE_HIT001,		// �q�b�g��1
	SOUND_LABEL_SE_SELECT,		//�I����
	SOUND_LABEL_SE_DECIDE,		// ���艹
	SOUND_LABEL_SE_COIN,		// �R�C��
	SOUND_LABEL_SE_JUMP,		// �W�����v
	SOUND_LABEL_SE_DRINK,		// ����
	SOUND_LABEL_SE_PEOPLE,		// ����
	SOUND_LABEL_SE_PEOPLE2,		// ����
	SOUND_LABEL_SE_TRUMPET,		// �g�����y�b�g
	SOUND_LABEL_SE_MEKURI,		// �߂���
	SOUND_LABEL_SE_DRAMA,		// �߂���
	SOUND_LABEL_MAX
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
