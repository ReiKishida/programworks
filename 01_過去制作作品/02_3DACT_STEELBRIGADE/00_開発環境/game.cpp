//=============================================================================
//
// �Q�[����� [game.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "game.h"

#include "fade.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "meshfield.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "smoke.h"
#include "enemy.h"
#include "score.h"
#include "ranking.h"
#include "life.h"
#include "pausemenu.h"
#include "pause.h"
#include "scoreitem.h"
#include "boost.h"
#include "motion.h"
#include "inputx.h"
#include "reticle.h"
#include "damage.h"
#include "danger.h"
#include "hit.h"
#include "model.h"
#include "result.h"
#include "modelwall.h"
#include "gate.h"
#include "pedestal.h"
#include "canon.h"
//*****************************************************************************
// �O���[�o���֐�
//*****************************************************************************
GAMESTATE g_GameState;	//�Q�[���̏��
GAMESTATE g_GameState2;

int g_nCounterGameState;
PAUSECHECK g_bPause;		//�|�[�Y����
PAUSECHECK2 g_GameScroll;   //�X�N���[���|�[�Y
JoyState g_JoyStateGame;

//=============================================================================
// �Q�[����ʂ̏���������
//=============================================================================
void InitGame(void)
{
	//�Q�[���p�b�h�̏��
	g_JoyStateGame.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	CAMERA pCamera;
	pCamera = *GetCamera();

	WALL *pWall;
	pWall = GetModelWall();

	GATE *pGate;
	pGate = GetGate();

	//�Q�[���Ŏg�����̏�����
	SetInitGame();

	//�X�R�A������
	InitScore();

	//���C�t������
	 InitLife();

	//�u�[�X�g�Q�[�W������
	 InitBoost();



	 //�|�[�Y�֘A������
	 InitPauseMenu();
	 InitPause();

	 //�X�R�A���Z������
	 InitScoreItem();

	 //���[�V�����̏�����
	InitMotion();

	 //���f��(��)
	 InitModelWall();

	 //�叉����
	 InitGate();

	 //���������
	 InitPedestal();

	 //�C�䏉����
	 InitCanon();

	 //���e�B�N��������
	 InitReticle();

	 //�_���[�W�R�[��������
	 InitDamage();

	 //�x��������
	 InitDanger();

	 //�q�b�g�R�[��������
	 InitItem();

	 //���f��������
	 InitModel();

	 //�ݒu�n����
	 SetGameModelWall();

	 SetAreaZero();
	 SetAreaOne();
	 SetAreaTwo();
	 SetAreaThree();



	g_bPause.bPause = false;
	g_GameScroll.bPause = false;
	g_GameState = GAMESTATE_NORMAL;	//�ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;


}

//=============================================================================
// �Q�[����ʂ̏I������
//=============================================================================
void UnInitGame(void)
{
	//�Q�[���Ŏg�����̏I��
	SetUninitGame();

	//�X�R�A�I��
	UninitScore();

	//�|�[�Y���j���[�I��
	UnInitPauseMenu();

	//�|�[�Y�I��
	UnInitPause();

	//���C�t�I��
	UninitLife();

	//�X�R�A���Z�A�C�e���I��
	UninitScoreItem();
	//�u�[�X�g�Q�[�W�I��
	UninitBoost();

	//���[�V�����̏I��
	UninitMotion();

	//�G���[�V�����I��
	//UninitEnemyMotion();

	//�ǃ��f���I��
	UninitModelWall();

	//��I��
	UninitGate();

	//����I��
	UninitPedestal();

	//�C��I��
	UninitCanon();

	//���e�B�N���I��
	UnInitReticle();

	//�_���[�W�R�[���I��
	UninitDamage();

	//�x���I��
	UninitDanger();

	//�q�b�g�R�[���I��
	UninitItem();

	//���f���I��
	UninitModel();

}


//=============================================================================
// �Q�[����ʂ̍X�V����
//=============================================================================
void UpdateGame(void)
{

	//�Q�[���p�b�h��Ԏ擾
	JoyState pJoyState = GetJoystate();


	//���
	XINPUT_STATE state;
	XInputGetState(0, &state);

	if (GetKeyboardTrigger(DIK_P) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
	{//�|�[�YON/OFF
		if (g_JoyStateGame.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			g_bPause.bPause = g_bPause.bPause ? false : true;
		}
		g_JoyStateGame.nJoypadState = INPUT_JOYSTATE_PUSH;

	}




		if (g_bPause.bPause == false && g_GameScroll.bPause == false)
		{

			//�Q�[���Ŏg�����̍X�V����
			SetUpdateGame();

			//�X�R�A�X�V����
			UpdateScore();
			UpdateLife();	//���C�t�X�V
			//UpdateItem();
			UpdateScoreItem();

			//�u�[�X�g�Q�[�W�X�V
			UpdateBoost();

			//���[�V�����̍X�V
			UpdateMotion();
			//UpdateEnemyMotion();

			//�ǃ��f���X�V
			UpdateModelWall();

			//��X�V
			UpdateGate();

			//����X�V
			UpdatePedestal();

			//�C��X�V
			UpdateCanon();

			//���e�B�N���X�V
			UpdateReticle();

			//�_���[�W�R�[���X�V
			UpdateDamage();

			//�x���X�V
			UpdateDanger();

			//�q�b�g�R�[���X�V
			UpdateItem();

			//���f���X�V
			UpdateModel();

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
					SetRanking(GetScore());
					SetResult(GetScore());
					//���[�h�ݒ�
					SetFade(MODE_RESULT);
					//SetFade(MODE_RANKING);
				}
				break;

			case GAMESTATE_FAILED:
				g_nCounterGameState++;
				if (g_nCounterGameState >= 60)
				{
					g_GameState2 = GAMESTATE_FAILED;
					g_GameState = GAMESTATE_NONE;
					SetRanking(GetScore());
					SetResult(GetScore());
					//���[�h�ݒ�
					SetFade(MODE_RESULT);
					//SetFade(MODE_RANKING);
				}
				break;
			}


		}


		if (g_bPause.bPause == true)
		{
			UpdatePause();
			UpdatePauseMenu();


		}

		if (g_JoyStateGame.nJoypadState == INPUT_JOYSTATE_PUSH)
		{
			g_JoyStateGame.nJoypadState = INPUT_JOYSTATE_NONE;
		}
		else if (g_JoyStateGame.nJoypadState == INPUT_JOYSTATE_NONE)
		{
			g_JoyStateGame.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
		}


	}


//=============================================================================
// �Q�[����ʂ̕`�揈��
//=============================================================================
void DrawGame(void)
{
	SetDrawGame();

	DrawScoreItem();
	//�u�[�X�g�Q�[�W�`��
	DrawBoost();

	//���C�t�`��
	DrawLife();

	//�ǃ��f���`��
	DrawModelWall();
	//��`��
	DrawGate();

	//����`��
	DrawPedestal();

	//�C��`��
	DrawCanon();
	//�G�t�F�N�g�`�揈��
	DrawEffect();
	//�p�[�e�B�N���`�揈��
	DrawParticle();
	//���̍X�V����
	DrawSmoke();

	//�_���[�W�R�[���`�揈��
	DrawDamage();

	//�x���`�揈��
	DrawDanger();

	//�q�b�g�`��
	DrawItem();

	//���f���`��
	DrawModel();


	DrawReticle();
	DrawScore();

		if (g_bPause.bPause == true)
		{
			DrawPause();
			DrawPauseMenu();
		}
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



//=============================================================================
// �Q�[�����2�̎擾
//=============================================================================
PAUSECHECK2 GetPauseCheck2(void)
{
	return g_GameScroll;
}

//=============================================================================
// �Q�[���������ݒ�
//=============================================================================
void SetInitGame(void)
{
	// �J�����̏���������
	InitCamera();

	// ���C�g�̏���������
	InitLight();

	//���b�V���t�B�[���h�̏���������
	InitMeshField();

	//�e�̏�����
	InitShadow();


	//�e�̏�����
	InitBullet();

	//�G�t�F�N�g����������
	InitEffect();

	//�p�[�e�B�N������������
	InitParticle();

	//�v���C���[�̏���������
	InitPlayer();

	//���̏���������
	InitSmoke();

	InitEnemy();

}


//=============================================================================
// �Q�[���I���ݒ�
//=============================================================================
void SetUninitGame(void)
{
	// �J�����̏I������
	UninitCamera();

	// ���C�g�̏I������
	UninitLight();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();

	//�e�̏I������
	UninitShadow();


	//�e�̏I������
	UninitBullet();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//�v���C���[�̏I������
	UninitPlayer();

	//���̏I������
	UninitSmoke();

	UninitEnemy();


}

//=============================================================================
// �Q�[���X�V�ݒ�
//=============================================================================
void SetUpdateGame(void)
{
	// �J�����̍X�V����
	UpdateCamera();

	// ���C�g�̍X�V����
	UpdateLight();

	//���b�V���t�B�[���h�̍X�V����
	UpdateMeshField();

	//�e�̍X�V����
	UpdateBullet();

	//�e�̍X�V����
	UpdateShadow();

	//�G�t�F�N�g�X�V����
	UpdateEffect();

	//�p�[�e�B�N���X�V����
	UpdateParticle();

	//�v���C���[�̍X�V����
	UpdatePlayer();

	//���̍X�V����
	UpdateSmoke();

	UpdateEnemy();
}

//=============================================================================
// �Q�[���`��ݒ�
//=============================================================================
void SetDrawGame(void)
{
	// �J�����̐ݒ�
	SetCamera();
	//���b�V���t�B�[���h�̍X�V����
	DrawMeshField();

	//�e�̕`�揈��
	DrawShadow();


	//�v���C���[�̕`�揈��
	DrawPlayer();

	DrawEnemy();

	//�e�̕`��
	DrawBullet();
}


//=============================================================================
// ���f���E�H�[���ݒu
//=============================================================================
void SetGameModelWall(void)
{
	//���E�[
	SetModelWall(D3DXVECTOR3(-100.0f, 0.0f, -1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),0);
	SetModelWall(D3DXVECTOR3(1500.0f, 0.0f, -1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),0);

	//�㉺�[
	SetModelWall(D3DXVECTOR3(680.0f, -10.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
	SetModelWall(D3DXVECTOR3(680.0f, 0.0f, -3900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);

	//��
	SetModelWall(D3DXVECTOR3(560.0f, 0.0f, -3780.0f), D3DXVECTOR3(0.0f, 0.0f , 0.0f), 4);



	//�v���C���[�̌��
	SetModelWall(D3DXVECTOR3(250.0f, 0.0f, -3260.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2);
	SetModelWall(D3DXVECTOR3(720.0f, 140.0f, -3260.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3);
	SetGate(D3DXVECTOR3(720.0f, 0.0f, -3230.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
	SetModelWall(D3DXVECTOR3(1200.0f, 0.0f, -3260.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2);


	//��1�Q�[�g
	SetModelWall(D3DXVECTOR3(250.0f, 0.0f, -2000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2);
	SetModelWall(D3DXVECTOR3(720.0f, 140.0f, -2000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3);
	SetGate(D3DXVECTOR3(720.0f, 0.0f, -2000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),5);
	SetModelWall(D3DXVECTOR3(1200.0f, 0.0f, -2000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2);


	//��2�Q�[�g
	SetModelWall(D3DXVECTOR3(250.0f, 0.0f, -900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2);
	SetModelWall(D3DXVECTOR3(720.0f, 140.0f, -900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3);
	SetGate(D3DXVECTOR3(720.0f, 0.0f, -900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 6);
	SetModelWall(D3DXVECTOR3(1200.0f, 0.0f, -900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2);



}


//=============================================================================
// �G���A0�I�u�W�F�N�g�ݒu
//=============================================================================
void SetAreaZero(void)
{
	//�X�R�A�A�C�e���ݒu
	SetScoreItem(D3DXVECTOR3(20.0f, 20.0f, -3730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//�X�^�[�g�n�_
	SetScoreItem(D3DXVECTOR3(80.0f, 20.0f, -3730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//�X�^�[�g�n�_
	SetScoreItem(D3DXVECTOR3(140.0f, 20.0f, -3730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//�X�^�[�g�n�_

}

//=============================================================================
// �G���A�P�I�u�W�F�N�g�ݒu
//=============================================================================
void SetAreaOne(void)
{
	//���I
	SetModel(D3DXVECTOR3(320, 0.0f, -3050.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	SetModel(D3DXVECTOR3(1200, 0.0f, -2200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetModel(D3DXVECTOR3(1200, 0.0f, -3050.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	//�X�R�A�A�C�e���ݒu
	SetScoreItem(D3DXVECTOR3(180.0f, 20.0f, -2900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//�X�^�[�g�n�_
	SetScoreItem(D3DXVECTOR3(1400.0f, 20.0f, -2200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//�X�^�[�g�n�_
	SetScoreItem(D3DXVECTOR3(20.0f, 20.0f, -3100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//�X�^�[�g�n�_
	SetScoreItem(D3DXVECTOR3(1400.0f, 20.0f, -3100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//�X�^�[�g�n�_

	SetEnemy(D3DXVECTOR3(750.0f, 0.0f, -2600.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 3);
}


//=============================================================================
// �G���A2�I�u�W�F�N�g�ݒu
//=============================================================================
void SetAreaTwo(void)
{

	//�X�R�A�A�C�e���ݒu
	SetScoreItem(D3DXVECTOR3(600.0f, 20.0f, -1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetScoreItem(D3DXVECTOR3(920.0f, 20.0f, -1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetScoreItem(D3DXVECTOR3(40.0f, 20.0f, -1500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));


	//���I
	SetModel(D3DXVECTOR3(800, 0.0f, -1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetModel(D3DXVECTOR3(700, 0.0f, -1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	//�C��
	SetPedestal(D3DXVECTOR3(500.0f, 0.0f, -1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetCanon(D3DXVECTOR3(500.0f, 80.0f, -1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 3);
	SetPedestal(D3DXVECTOR3(900.0f, 0.0f, -1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetCanon(D3DXVECTOR3(900.0f, 80.0f, -1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 3);
	SetEnemy(D3DXVECTOR3(750.0f, 0.0f, -1180.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 4);

}

//=============================================================================
// �G���A3�I�u�W�F�N�g�ݒu
//=============================================================================
void SetAreaThree(void)
{
	SetScoreItem(D3DXVECTOR3(20.0f, 20.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//�X�^�[�g�n�_
	SetScoreItem(D3DXVECTOR3(1200.0f, 20.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//�X�^�[�g�n�_

	 //�C��
	SetPedestal(D3DXVECTOR3(300.0f, 0.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetCanon(D3DXVECTOR3(300.0f, 80.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 3);

	SetPedestal(D3DXVECTOR3(1000.0f, 0.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetCanon(D3DXVECTOR3(1000.0f, 80.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 3);

	SetPedestal(D3DXVECTOR3(700.0f, 0.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetCanon(D3DXVECTOR3(700.0f, 80.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 5);

	//�G
	SetEnemy(D3DXVECTOR3(750.0f, 0.0f, -70.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 3);

}