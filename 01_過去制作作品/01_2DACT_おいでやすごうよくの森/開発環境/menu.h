//=============================================================================
//
// �L�[�F�ς����� [pausemenu.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _MENU_H_
#define _MENU_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_MENU	(128)	// �w�i�̍ő吔



//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	MENUSTATE_NORMAL = 0,	//�ʏ���
	MENUSTATE_SELECT,			//�I�����
	MENUSTATE_MAX			//��ނ̑���
}MENUSTATE0;


typedef enum
{
	MENUSTATE_NONE = 0,	//�ʏ���
	MENUSTATE_USE,			//�I�����
	MENU_STATE_MAX			//��ނ̑���
}MENUSTATE;


typedef enum
{
	GAMEMODE = 0,	//�R���e�B�j���[
	MENU_RANKING,			//���g���C
	QUIT,			//�N�E�B�b�g
	MENU_MAX		//����
}MENU;


typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�J���[
	MENU menu;
	int nType;			//���
	MENUSTATE0 state;	//�L�[�̏��
	MENUSTATE uState;
	int nCntState;

	bool bUse;			//�g�p���Ă��邩�ǂ���
}PAUSEMENU;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitMenu(void);
void UnInitMenu(void);
void UpdateMenu(void);
void DrawMenu(void);




//void PushPauseMenu(int nNowSelect);
void PushMenu(void);

void SetMenu(D3DXVECTOR3 pos, int nType);
#endif