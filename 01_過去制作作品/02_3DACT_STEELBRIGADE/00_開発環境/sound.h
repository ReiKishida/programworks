//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : REI KISHIDA
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
	SOUND_LABEL_TITLE000 = 0,		// BGM0
	SOUND_LABEL_GAME000,			// BGM1
	SOUND_LABEL_RANKING,			// BGM2
	SOUND_LABEL_TUTORIAL000,		// BGM4
	SOUND_LABEL_WORLD,				// ���[���h
	SOUND_LABEL_BR,				// �u���[�t�B���O
	SOUND_LABEL_SE_SHOT,			// �ˌ���
	SOUND_LABEL_SE_EXPLOSION,			// ����
	SOUND_LABEL_SE_QUICK,			// �N�C�b�N��
	SOUND_LABEL_SE_LAND,			// ���n��
	SOUND_LABEL_SE_STEAM,			// �X�`�[��
	SOUND_LABEL_SE_WALK,			// ���s
	SOUND_LABEL_SE_BIRI,			//�r���r��
	SOUND_LABEL_SE_SETUP,			//�Z�b�g�A�b�v
	SOUND_LABEL_SE_DECIDE,			// ���艹
	SOUND_LABEL_MAX,
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
