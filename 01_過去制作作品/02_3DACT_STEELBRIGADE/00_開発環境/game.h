//=============================================================================
//
// �Q�[����� [game.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"



//*****************************************************************************
// �\����
//*****************************************************************************
typedef enum
{
	GAMESTATE_NONE = 0,	//�������Ă��Ȃ����
	GAMESTATE_NORMAL,	//�ʏ���
	GAMESTATE_CLEAR,		//�I�����
	GAMESTATE_FAILED,		//�I�����
	GAMESTATE_END,
	GAMESTATE_MAX		//�ő吔
}GAMESTATE;

typedef struct
{
	bool bPause;
}PAUSECHECK;

typedef struct
{
	bool bPause;
}PAUSECHECK2;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitGame(void);
void UnInitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetGameState(GAMESTATE state);
GAMESTATE *GetGameState(void);
GAMESTATE *GetGameState2(void);

PAUSECHECK *GetPauseCheck(void);
PAUSECHECK2 GetPauseCheck2(void);

void SetInitGame(void);
void SetUninitGame(void);
void SetUpdateGame(void);
void SetDrawGame(void);

void SetGameModel(void);
void SetGameModelWall(void);
void SetGameItem(void);

void SetAreaZero(void);

void SetAreaOne(void);
void SetAreaTwo(void);
void SetAreaThree(void);

#endif

