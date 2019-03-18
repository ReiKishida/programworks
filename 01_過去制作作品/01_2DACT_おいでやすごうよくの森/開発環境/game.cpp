//=============================================================================
//
// �Q�[����� [game.cpp]
// Author : KishidaRei
//
//=============================================================================
#include "main.h"
#include "game.h"
#include "bg.h"
#include "player.h"
#include "score.h"
#include "effect.h"
#include "timer.h"
#include "input.h"
#include "pause.h"
#include "pausemenu.h"
#include "fade.h"
#include "block.h"
#include "item.h"
#include "shoot.h"
#include "character.h"
#include "itemcounter.h"
#include "vodkacounter.h"
#include "satsucounter.h"
#include "areycounter.h"
#include "ranking.h"
#include "highscore.h"
#include "gamehighscore.h"
#include "resultitem.h"
#include "resultarey.h"
#include "resultitemall.h"
#include "menuranking.h"
#include "shadow.h"
#include "fg.h"
#include "fg2.h"
#include "sound.h"
#include "popscore.h"
#include "fever.h"

//*****************************************************************************
// �O���[�o���֐�
//*****************************************************************************
GAMESTATE g_GameState;	//�Q�[���̏��
//GAMESTATE g_GameState2;
int g_nCounterGameState;
PAUSECHECK g_bPause;		//�|�[�Y����
PAUSECHECK2 g_GameScroll;   //�X�N���[���|�[�Y
//=============================================================================
// �Q�[����ʂ̏���������
//=============================================================================
void InitGame(void)
{

	//�w�i������
	InitBG();

	
	//̃u���b�N�̏�����
	InitBlock();

	//�e������
	//InitShadow();

	//�J�E���^�[������//
	InitItemCounter();
	InitVodkaCounter();
	InitSatsuCounter();
	InitAreyCounter();
	

	//�A�C�e��������
	InitItem();

	//�_��
	InitPopScore();

	//�L�����N�^�[������
	InitCharacter();

	//�G�t�F�N�g�̏�����
	InitEffect();


	//���ˈʒu�̏�����
	InitShoot();


	//�^�C�}�[�̏�����
	InitTimer();
	
	////�Q�[�����n�C�X�R�A
	InitGameHighScore();


	//�|�[�Y�̏�����
	InitPause();
		
	//���j���[�̏�����
	InitPauseMenu();

	//�O�i�̏�����
	InitFg();
	InitFg2();
	
	//�t�B�[�o�[������
	InitFever();

	SetFg(D3DXVECTOR3(270, 660, 0.0f), 0, 85, 150);
	SetFg(D3DXVECTOR3(810, 660, 0.0f), 1, 80, 150);
	SetFg(D3DXVECTOR3(1100, 650, 0.0f), 0, 65, 150);

	//��
	SetFg(D3DXVECTOR3(100, 640, 0.0f), 2, 50, 25);
	SetFg(D3DXVECTOR3(640, 630, 0.0f), 2, 60, 30);
	SetFg(D3DXVECTOR3(450, 630, 0.0f), 2, 50, 30);
	SetFg(D3DXVECTOR3(600, 650, 0.0f), 2, 40, 20);



	SetFg2(D3DXVECTOR3(SCREEN_WIDTH / 2, 590, 0.0f), 0, 50, 480);
	SetFg2(D3DXVECTOR3(-10, 645, 0.0f), 4, 250, 120);
	SetFg2(D3DXVECTOR3(SCREEN_WIDTH + 10, 645, 0.0f), 5, 250, 120);

	//SetFg2(D3DXVECTOR3(SCREEN_WIDTH / 2, 660, 0.0f), 2, SCREEN_HEIGHT -100, SCREEN_WIDTH / 2 + 20);


	


	//�u���b�N�ݒu(��Q���n)
	SetBlockType000();

	//�u���b�N�ݒu(�n��)
	SetBlockType001();

	//�u���b�N�ݒu(�ړ���)
	SetBlockType002();

	//�u���b�N�ݒu(�o�l��)
	SetBlockType003();

	SetBlockType004();

	//�X�R�A�̏�����
	InitScore();

	//�A�C�e���ݒu
	SetItem();

	//�v���C���[�̏�����
	InitPlayer();

	
	//���ˈʒu�̐ݒu
	SetShoot(D3DXVECTOR3(SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 - 220 , 0.0f));


	
		//�L�����N�^�[�z�u
		//SetCharacter(D3DXVECTOR3(1300, 550, 0.0f), 0);
	

	g_bPause.bPause = false;
	g_GameScroll.bPause = false;
	g_GameState = GAMESTATE_NORMAL;	//�ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;

	//PlaySound(SOUND_LABEL_SE_TRUMPET);
}

//=============================================================================
// �Q�[����ʂ̏I������
//=============================================================================
void UnInitGame(void)
{
	//�w�i�I��
	UnInitBG();


	//�A�C�e��������
	UnInitItem();

	//���ˈʒu�̏I��
	UnInitShoot();

	//�u���b�N�̏I��
	UnInitBlock();

	//�e������
	//UnInitShadow();


	//�L�����N�^�[�I��
	UnInitCharacter();

	


	//�G�t�F�N�g�̏I��
	UninitEffect();
	
	



	//�^�C�}�[�̏I��
	UninitTimer();
	

	//�|�[�Y�̏I��
	UnInitPause();

	
	//���j���[�̏I��
	UnInitPauseMenu();

	
	//�X�R�A�̏I��
	UninitScore();

	//�J�E���^�[�I��
	UninitItemCounter();

	//�J�E���^�[//
	UninitVodkaCounter();

	UninitSatsuCounter();

	UninitAreyCounter();

	//�Q�[�����n�C�X�R�A
	UnInitGameHighScore();

	//�v���C���[�̏I��
	UnInitPlayer();

	//�O�i�̏I��
	UnInitFg();
	UnInitFg2();
	
	//�_��
	UnInitPopScore();

	//�t�B�[�o�[������
	UnInitFever();
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

	//if (GetKeyboardTrigger(DIK_O) == true)
	//{//�|�[�YON/OFF
	//	g_GameScroll.bPause = g_GameScroll.bPause ? false : true;
	//}


	if (g_bPause.bPause == false || g_GameScroll.bPause == true )
	{
		//�u���b�N�̍X�V
		UpdateBlock();

		
	}
		

		if (g_bPause.bPause == false && g_GameScroll.bPause == false)
		{

			//�u���b�N�̍X�V
			//UpdateBlock();

			//�w�i�X�V
			UpdateBG();


			//�e�X�V
			//UpdateShadow();

			//�v���C���[�̍X�V
			UpdatePlayer();

			//�J�E���^�[�X�V
			UpdateItemCounter();

			//�E�H�b�J�X�V
			UpdateVodkaCounter();

			//�D�X�V
			UpdateSatsuCounter();

			UpdateAreyCounter();


			//�A�C�e���X�V
			UpdateItem();

			//���ˈʒu�̍X�V
			UpdateShoot();

			//�L�����N�^�[�X�V
			UpdateCharacter();

			////�Q�[�����n�C�X�R�A
			UpdateGameHighScore();

			//�O�i�̍X�V
			UpdateFg();
			UpdateFg2();
			


			//�^�C�}�[�̍X�V
			UpdateTimer();


			//�X�R�A�̍X�V
			UpdateScore();

			//�G�t�F�N�g�̍X�V
			UpdateEffect();

			//�_��
			UpdatePopScore();

			//�t�B�[�o�[������
			UpdateFever();

			switch (g_GameState)
			{
			case GAMESTATE_NORMAL:
				break;

			case GAMESTATE_END:
				g_nCounterGameState++;
				if (g_nCounterGameState >= 10)
				{
					g_GameState = GAMESTATE_NONE;

					////////////////////
					//�n�C�X�R�A�ɑ��//
					////////////////////
					SetGameHighScore(GetScore());
					SetRanking(GetScore());
					SetMenuRanking(GetScore());
					SetHighScore(GetScore());


					//////////////////
					//�A�C�e���̐��Z//
					//////////////////
					SetResultCoin(GetItemCounter());
					SetResultArey(GetAreyCounter());
					SetResultVodka(GetVodkaCounter());
					SetResultSatsu(GetSatsuCounter());

					SetResultItemAll(GetItemCounter(), GetVodkaCounter(), GetSatsuCounter(), GetAreyCounter());


					//���[�h�ݒ�
					//SetMode(MODE_RESULT);
					SetFade(MODE_RESULT);

				}
				
				break;
			}

			
		}


		if (g_bPause.bPause == true)
		{
			UpdatePause();
			UpdatePauseMenu();


		}

	}


//=============================================================================
// �Q�[����ʂ̕`�揈��
//=============================================================================
void DrawGame(void)
{
		//�w�i�`��
		DrawBG();

		//�t�B�[�o�[������
		DrawFever();

		//�u���b�N�̕`��
		DrawBlock();


		//�L�����N�^�[�`��
		DrawCharacter();


		//���ˈʒu�̕`��
		DrawShoot();


		//�e�X�V
		//DrawShadow();
	

		DrawFg2();

		//�G�t�F�N�g�̕`��
		DrawEffect();

		//�v���C���[�̕`��
		DrawPlayer();
		

		//�A�C�e���̕`��
		DrawItem();

		//�_��
		DrawPopScore();

		//�O�i�̕`��
		DrawFg();

		//�^�C�}�[�̕`��
		DrawTimer();

	
		//�X�R�A�̕`��
		DrawScore();

		

		//�Q�[�����n�C�X�R�A
		DrawGameHighScore();

		//�J�E���^�[�`��
		DrawItemCounter();

		//�E�H�b�J�J�E���^�[
		DrawVodkaCounter();

		//�D�J�E���^�[
		DrawSatsuCounter();

		//�S�A���C�J�E���^�[
		DrawAreyCounter();

	


		if (g_bPause.bPause == true)
		{
			DrawPause();
			DrawPauseMenu();
		}
}


//=============================================================================
// Type000�ݒu
//=============================================================================

void SetBlockType000()
{

	SetBlock(D3DXVECTOR3(280, 410, 0), 120, 80, 0, BLOCKSTATE_NORMAL, 0.0f, 0.0f);
	
	SetBlock(D3DXVECTOR3(90, 350, 0), 120.0f, 60.0f, 2, BLOCKSTATE_MOVE_Y, 0.0f, -2.0f);



	SetBlock(D3DXVECTOR3(910, 410, 0), 120, 80, 0, BLOCKSTATE_NORMAL, 0.0f, 0.0f);

	SetBlock(D3DXVECTOR3(1090, 350, 0), 120.0f, 60.0f, 3, BLOCKSTATE_MOVE_Y, 0.0f, 2.0f);


	//SetBlock(D3DXVECTOR3(940, 200, 0), 100, 50, 2, BLOCKSTATE_MOVE_X, -1.5f, 0.0f);

	
}

//=============================================================================
// Type001�ݒu
//=============================================================================
void SetBlockType001()
{
	
	SetBlock(D3DXVECTOR3(0, 580, 0), SCREEN_WIDTH, 150.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f);

	

	
		//SetBlock(D3DXVECTOR3(SCREEN_WIDTH / 2 - 400, -250, 0), 300.0f, 50.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f);
		//SetBlock(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200, -250, 0), 300.0f, 50.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f);

}

//=============================================================================
// Type002�ݒu
//=============================================================================
void SetBlockType002()
{
	// X���ړ���
	//SetBlock(D3DXVECTOR3(790, 480, 0), 100.0f, 40.0f, 2, BLOCKSTATE_MOVE_X, 1.4f, 0.0f);


	// Y���ړ���
	//SetBlock(D3DXVECTOR3(1100, 420, 0), 100.0f, 40.0f, 2, BLOCKSTATE_MOVE_Y, 0.0f, 0.0f);


	//SetBlock(D3DXVECTOR3(150, 600, 0), 150.0f, 40.0f, 2, BLOCKSTATE_MOVE_XY, 1.0f, -1.0f);


}

//=============================================================================
// Type003�ݒu
//=============================================================================

void SetBlockType003()
{

	//SetBlock(D3DXVECTOR3(20, 630, 0), 50.0f, 50.0f, 3, BLOCKSTATE_SPRING, 0.0f, 0.0f);
	//SetBlock(D3DXVECTOR3(1235, 630, 0), 50.0f, 50.0f, 3, BLOCKSTATE_SPRING, 0.0f, 0.0f);

}


//=============================================================================
// Type004�ݒu
//=============================================================================

void SetBlockType004()
{

	//SetBlock(D3DXVECTOR3(400, 450, 0), 60.0f, 60.0f, 0, BLOCKSTATE_CRASH, 0.0f, 0.0f);

}

//=============================================================================
// �A�C�e���̐ݒu
//=============================================================================
void SetItem()
{

	SetItem(D3DXVECTOR3(30, 40, 0.0f), ITEMTYPE_H_COIN, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetItem(D3DXVECTOR3(220, 40, 0.0f), ITEMTYPE_H_VODKA, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetItem(D3DXVECTOR3(400, 40, 0.0f), ITEMTYPE_H_PAPER, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetItem(D3DXVECTOR3(600, 40, 0.0f), ITEMTYPE_H_AREY, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

}


//=============================================================================
// ���[�h�ݒ菈��
//=============================================================================
void SetGameState(GAMESTATE state)
{
	g_GameState = state;
	g_nCounterGameState = 10;
}

//=============================================================================
// �Q�[����Ԃ̎擾
//=============================================================================
GAMESTATE *GetGameState(void)
{
	return &g_GameState;
}

//GAMESTATE *GetGameState2(void)
//{
//	return &g_GameState2;
//}


//=============================================================================
// �Q�[����Ԃ̎擾
//=============================================================================
PAUSECHECK *GetPauseCheck(void)
{
	return &g_bPause;
}



//=============================================================================
// �Q�[�����2�̎擾
//=============================================================================
PAUSECHECK2 GetPauseCheck2(void)
{
	return g_GameScroll;
}

