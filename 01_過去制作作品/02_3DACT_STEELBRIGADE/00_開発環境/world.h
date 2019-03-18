//=============================================================================
//
// ����ʏ��� [tips.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _WORLD_H_
#define _WORLD_H_

#include "main.h"
#include "input.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	WORLD_NORMAL = 0,
	WORLD_READING,
	WORLD_DECIDE,
	WORLD_MAX
}WORLD_STATE;

typedef struct
{
	bool bUpdate;
}WORLDSTATE;


typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�J���|
	int nType;			//���
	WORLD_STATE state;
}WORLD;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitWorld(void);
void UnInitWorld(void);
void UpdateWorld(void);
void DrawWorld(void);

#endif


