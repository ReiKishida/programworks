////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UI���� [UI.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _UI_H_
#define _UI_H_

#include "main.h"
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef enum
{
	UITEX_BG = 0,			// �w�i
	UITEX_BG1,				// �w�i
	UITEX_RESULT,			// ���ʍ���
	UITEX_GAMECLEAR,		// �Q�[���N���A
	UITEX_GAMEOVER,			// �Q�[���I�[�o�[
	UITEX_BOTTON,
	UITEX_LANCE,
	UITEX_SORD,
	UITEX_HAMMER,
	UITEX_PUNCH,
	UITEX_MAX,				// �ő吔
}UITEX;
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXCOLOR col;			// �F
	UITEX nType;			// ���
	float fWidth;			// ��
	float fHeight;			// ����
	bool bUse;				// �g�p����Ă��邩�ǂ���
}UI;
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitUI(void);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);
void SetUI(D3DXVECTOR3 pos, D3DXCOLOR col, UITEX nType, float fWidth, float fHeight);
UI *GetUI(void);
#endif
