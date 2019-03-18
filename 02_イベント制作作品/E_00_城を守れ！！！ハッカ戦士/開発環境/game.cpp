//=============================================================================
//
// �Q�[����� [game.cpp]
// Author : KishidaRei
//
//=============================================================================
#include "main.h"
#include "game.h"
#include "fade.h"
#include "input.h"
#include "pause.h"
#include "pausemenu.h"
#include "sound.h"
#include "bg.h"
#include "player.h"
#include "castle.h"
#include "chargebar.h"
#include "enemy.h"
#include "bullet.h"
#include "enemy.h"
#include "stamina.h"
#include "castlelife.h"
#include "life.h"
#include "explosion.h"
#include "timer.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���֐�
//*****************************************************************************
GAMESTATE g_GameState;	//�Q�[���̏��
GAMESTATE g_GameState2;
int g_nCounterGameState;
PAUSECHECK g_bPause;		//�|�[�Y����


//=============================================================================
// �Q�[����ʂ̏���������
//=============================================================================
void InitGame(void)
{


	//�w�i������
	InitBG();

	//�v���C���[������
	InitPlayer();

	//��̏�����
	InitCastle();
	
	

	//�e������
	InitBullet();




	//�G�̏�����
	InitEnemy();

	InitLife();

	InitExplosion();

	InitTimer();


	SetEnemy(D3DXVECTOR3(1000, 540, 0.0f), 0, 10);
	SetEnemy(D3DXVECTOR3(1200, 540, 0.0f), 1, 10);
	SetEnemy(D3DXVECTOR3(1400, 540, 0.0f), 2, 10);
	SetEnemy(D3DXVECTOR3(1600, 540, 0.0f), 0, 10);
	SetEnemy(D3DXVECTOR3(1800, 540, 0.0f), 1, 10);
	SetEnemy(D3DXVECTOR3(2000, 540, 0.0f), 2, 10);

	SetEnemy(D3DXVECTOR3(2200, 540, 0.0f), 0, 10);
	SetEnemy(D3DXVECTOR3(2400, 540, 0.0f), 1, 10);
	SetEnemy(D3DXVECTOR3(2600, 540, 0.0f), 2, 10);
	SetEnemy(D3DXVECTOR3(2800, 540, 0.0f), 0, 10);
	SetEnemy(D3DXVECTOR3(3000, 540, 0.0f), 1, 10);
	SetEnemy(D3DXVECTOR3(3200, 540, 0.0f), 2, 10);

	//SetEnemy(D3DXVECTOR3(800, 540, 0.0f), 1, 3);

	//SetEnemy(D3DXVECTOR3(600, 540, 0.0f), 2, 3);

	g_bPause.bPause = false;
	g_GameState = GAMESTATE_NORMAL;	//�ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;

}

//=============================================================================
// �Q�[����ʂ̏I������
//=============================================================================
void UnInitGame(void)
{

	//�w�i�̏I��
	UnInitBG();

	//�v���C���[�I��
	UnInitPlayer();

	//�e�I��
	UnInitBullet();

	//�G�̏I��
	UnInitEnemy();

	//��̏I��
	UnInitCastle();

	UninitLife();

	UnInitExplosion();

	UninitTimer();

}


//=============================================================================
// �Q�[����ʂ̍X�V����
//=============================================================================
void UpdateGame(void)
{

	if (GetKeyboardTrigger(DIK_P) == true)
	{//�|�[�YON/OFF
		g_bPause.bPause = g_bPause.bPause ? false : true;
	}

	if (g_bPause.bPause == false)
	{
		//�w�i�̍X�V
		UpdateBG();

		//�v���C���[�I��
		UpdatePlayer();

		//��̍X�V
		UpdateCastle();

		
		
		//�e�X�V
		UpdateBullet();


		//�G�̍X�V
		UpdateEnemy();

		UpdateLife();

		UpdateExplosion();


		
		UpdateTimer();

		switch (g_GameState)
		{
		case GAMESTATE_NORMAL:
			break;

		case GAMESTATE_CLEAR:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 60)
			{
				g_GameState2 = GAMESTATE_CLEAR;
				g_GameState = GAMESTATE_NONE;
				


				SetFade(MODE_RESULT);



			}
			break;

		case GAMESTATE_FAILED:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 60)
			{
				g_GameState2 = GAMESTATE_FAILED;
				g_GameState = GAMESTATE_NONE;





				SetFade(MODE_RESULT);

			}
			break;
		}


		

	}

	if (g_bPause.bPause == false)
	{
		

	}

}


//=============================================================================
// �Q�[����ʂ̕`�揈��
//=============================================================================
void DrawGame(void)
{
	//�w�i�̕`��
	DrawBG();

	//�v���C���[�̕`��
	DrawPlayer();

	//��̕`��
	DrawCastle();

	
	
	DrawTimer();



	

	//�e�X�V
	DrawBullet();


	//�G�̕`��
	DrawEnemy();

	DrawLife();

	DrawExplosion();
}


//=============================================================================
// ���[�h�ݒ菈��
//=============================================================================
void SetGameState(GAMESTATE state)
{
	g_GameState = state;
	g_nCounterGameState = 0;
}

//=============================================================================
// �Q�[����Ԃ̎擾
//=============================================================================
GAMESTATE *GetGameState(void)
{
	return &g_GameState;
}

GAMESTATE *GetGameState2(void)
{
	return &g_GameState2;
}


//=============================================================================
// �Q�[����Ԃ̎擾
//=============================================================================
PAUSECHECK *GetPauseCheck(void)
{
	return &g_bPause;
}
