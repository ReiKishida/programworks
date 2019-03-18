#pragma once
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �Q�[������ [game.h]
// Author :Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
// �Q�[���̏��
typedef enum
{
	GAMESTATE_NONE = 0,	// �������Ă��Ȃ����
	GAMESTATE_GAME1,	// �ʏ���
	GAMESTATE_BOSS_START,// �{�X
	GAMESTATE_BOSS,		// �ʏ���
	GAMESTATE_END,		// �I�����
	GAMESTATE_END2,		// �I�����
	GAMESTATE_MAX,
}GAMESTATE;

typedef enum
{
	GAMESTAGE1 = 0,
	GAMESTAGEBOSS,
	GAMESTAGEMAX
}GAMESTAGE;

//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
GAMESTATE GetGameState(void);
void ReleaseGamePause(void);

void SetPlayerFileData(void);						// �v���C���[�t�@�C���f�[�^
void SetObjectFileData(void);						// �I�u�W�F�N�g�t�@�C���f�[�^
void SetBillboardFileData(char *cFilename);			// �r���{�[�h�̈ʒu�f�[�^
void SetStagePos(char *cFilename);					// �X�e�[�W�̈ʒu�f�[�^
bool GetPause(void);
bool GetPauseView(void);

void PrintStagePos(void);
#endif
