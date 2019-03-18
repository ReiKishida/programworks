////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �Q�[������ [game.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// �C���N���[�h�֐�
#include <string.h>		// �X�g�����O�֐�
#include <stdlib.h>		// �����h�֐�
#include "main.h"
#include "game.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "player.h"
#include "fade.h"
#include "time.h"
#include "gamepad.h"
#include "light.h"
#include "line.h"
#include "meshWall.h"
#include "meshField.h"
#include "model.h"
#include "orbit.h"
#include "shadow.h"
#include "effect.h"
#include "pause.h"
#include "time.h"
#include "sky.h"
#include "effect2D.h"
#include "sound.h"
#include "spGage.h"
#include "time.h"
#include "UI.h"
#include "effectUP.h"
#include "MessageWindow.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_CHAR	(256)
//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
GAMESTATE g_gameState = GAMESTATE_START;	// �Q�[���̏��
int g_nCounterGameState;				// ��ԊǗ��J�E���^�[
bool g_bPause;							// �|�[�Y�����ǂ���
bool g_bPauseView;						// �|�[�Y��\��
bool g_bGameState[GAMESTAGEMAX];		// �Q�[���؂�ւ��
int g_nMessageWindow = 0;
bool g_bMessage[MESSAGETEX_MAX];
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitGame(void)
{
	Player *pPlayer = GetPlayer();
	// �e��I�u�W�F�N�g�̏���������
	g_bPause = false;			// �|�[�Y���Ȃ�
	g_bPauseView = false;		// �|�[�Y�������Ȃ�
	for (int nCntState = 0; nCntState < GAMESTAGEMAX; nCntState++)
	{
		g_bGameState[nCntState] = false;
	}
	g_gameState = GAMESTATE_START;
	g_nCounterGameState = 0;
	g_nMessageWindow = 0;
	for (int nCnt = 0; nCnt < MESSAGETEX_MAX; nCnt++)
	{
		g_bMessage[nCnt] = false;
	}
	// �J�����̏���������
	InitCamera();
	// ���̏���������
	InitLine();
	// ���C�g�̏���������
	InitLight();
	// �G�t�F�N�g�̏���������
	InitEffect2D();

	// �e�̏���������
	InitShadow();
	// �v���C���[�̏���������
	InitPlayer();
	// ���b�V�����̏���������
	InitMeshField();
	// ���b�V���ǂ̏���������
	InitMeshWall();
	// ���f���̏���������
	InitModel();
	// �t�@�C���ǂݍ���
	SetStagePos("data/stage1.txt");
	// �O�Ղ̏���������
	InitOrbit();
	// ��̐���
	InitSky();
	// SP
	InitSpGage();
	// ���b�Z�[�W�E�B���h�E
	InitMessageWindow();
	// �^�C��
	InitTime();
	SetPlayer(0, D3DXVECTOR3(0.0f, 600, 0));
	GetCamera()->posR = D3DXVECTOR3(0.0f, 500, 0);
	GetCamera()->posRDest = D3DXVECTOR3(0.0f, 500, 0);
	// �|�[�Y�̏���������
	InitPause();
	// �G�t�F�N�g
	InitEffect();
	InitEffectUP();
	InitUI();
	SetMessageWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0.0f), 600, 500, MESSAGETEX_TITLE);
	g_bMessage[g_nMessageWindow] = true;
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitGame(void)
{
	// �e��I�u�W�F�N�g�̏I������
#if 1
	// �J�����̏I������
	UninitCamera();
	// ���C���̏I������
	UninitLine();
	// ���C�g�̏I������
	UninitLight();
	// �e�̏I������
	UninitShadow();
	// �O�Ղ̏I������
	UninitOrbit();
	// �|�[�Y�̏I������
	UninitPause();
	// �G�t�F�N�g
	UninitEffect();
	UninitEffect2D();
	UninitEffectUP();
	// ��
	UninitSky();
	// SP
	UninitSpGage();
	// �^�C��
	UninitTime();
	// ���b�Z�[�W�E�B���h�E
	UninitMessageWindow();
	UninitUI();
	g_gameState = GAMESTATE_START;
	g_bPause = false;
	g_bPauseView = false;
#endif
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateGame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Player *pPlayer = GetPlayer();
	Camera *pCamera = GetCamera();
	// �|�[�Y����
	if (GetKeyboardTrigger(DIK_P) == true)
	{// �L�[�{�[�h
		if (g_bPauseView == true)
		{
			g_bPauseView = false;
		}
		PlaySound(SOUND_LABEL_PAUSE000);
		SetPause(true, 0);
		g_bPause = g_bPause ? false : true;
	}

	// �|�[�Y�����ǂ���
	if (g_bPause == true)
	{
		if (GetKeyboardTrigger(DIK_V) == true)
		{// �|�[�Y��ʏ������ǂ���
			g_bPauseView = g_bPauseView ? false : true;
		}

	}

	if (g_bPause == false)
	{
		// �e��I�u�W�F�N�g�̍X�V����
		// �J�����̍X�V����
		UpdateCamera();
		// ���̍X�V����
		UpdateLine();
		// �O�Ղ̕`��
		UpdateOrbit();
		// ���C�g�̍X�V����
		UpdateLight();

		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			// �v���C���[�̍X�V����
			UpdatePlayer(nCnt);
		}
		// �e�̍X�V����
		UpdateShadow();
		// ���b�V�����̍X�V����
		UpdateMeshField();
		// ���b�V���ǂ̍X�V����
		UpdateMeshWall();
		// ���f���̍X�V����
		UpdateModel();
		// ��
		UpdateSky();
		// �G�t�F�N�g
		UpdateEffect();
		UpdateEffect2D();
		UpdateEffectUP();
		// SP
		UpdateSpGage();
		// ���b�Z�[�W�E�B���h�E
		UpdateMessageWindow();
		UpdateUI();
	}

	if (g_bPause == false)
	{
		switch (g_gameState)
		{
		case GAMESTATE_NONE:
			break;
		case GAMESTATE_START:
			if (g_bMessage[g_nMessageWindow])
			{
				if (g_nMessageWindow < 4)
				{
					if (GetKeyboardTrigger(DIK_S) || GetGamePadTrigger(0, BUTTON_B))
					{
						DeleteMessageWndow();
						g_bMessage[g_nMessageWindow] = false;
						g_nMessageWindow++;
						if (g_nMessageWindow >= 4)
						{
							g_nMessageWindow = 4 - 1;
							g_gameState = GAMESTATE_GAME;
							SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2 - 150, 600, 0.0f), D3DXCOLOR(1, 1, 1, 1), UITEX_L, 200 * 0.7f, 150 * 0.7f);
							SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2 + 150, 600, 0.0f), D3DXCOLOR(1, 1, 1, 1), UITEX_R, 200 * 0.7f, 150 * 0.7f);
							SetMessageWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), 400*1.2f, 300 * 1.2f, MESSAGETEX_KAERU);

						}
						else
						{
							if (g_nMessageWindow == MESSAGETEX_TEIJI)
							{
								PlaySound(SOUND_RESULT_TEIKI);
								SetMessageWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), 500, 300, MESSAGETEX_TEIJI);
							}
							if (g_nMessageWindow == MESSAGETEX_KAEROU)
							{
								PlaySound(SOUND_RESULT_SOKKOU);
								SetMessageWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), 400, 300, MESSAGETEX_KAEROU);
							}
							if (g_nMessageWindow == MESSAGETEX_SOUSA)
							{
								SetMessageWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0.0f), 700, 400, MESSAGETEX_SOUSA);
							}
						}
						g_bMessage[g_nMessageWindow] = true;
					}
				}
			}
			break;
		case GAMESTATE_GAME:
			if (g_bPause == false)
			{
				// �^�C��
				UpdateTime();
			}
			break;
		case GAMESTATE_END:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 250)
			{
				g_gameState = GAMESTATE_NONE;
				if (GetFade() == FADE_NONE)
				{
					// ��ʃ��[�h�̐ݒ�
					SetFade(MODE_RESULT);
				}
			}
			break;
		}

	}
	else
	{
		if (g_bPauseView == false)
		{
			UpdatePause(&g_bPause);		// �|�[�Y
		}
		else
		{
			// �J�����̍X�V����
			UpdateCamera();
		}
	}
	if (GetKeyboardTrigger(DIK_F9))
	{
		UninitLine();
		UninitModel();
		InitLine();
		SetObjectFileData();
		InitModel();
		InitMeshField();
		InitMeshWall();
		// �t�@�C���ǂݍ���
		SetStagePos("data/stage1.txt");
	}
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		InitModel();
		InitMeshField();
		InitMeshWall();
		// �t�@�C���ǂݍ���
		SetStagePos("data/stage1.txt");

	}
}
//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawGame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Player *pPlayer = GetPlayer();
	Camera *pCamera = GetCamera();
	D3DVIEWPORT9 viewportDef;

	// �r���[�|�[�g�����擾
	pDevice->GetViewport(&viewportDef);

	// �e��I�u�W�F�N�g�̕`�揈��
#if 1
	// �J�����̐ݒ�
	SetCamera(0);
	if (GetDrawDebug()[DRAW_DEBUG_LINE] == true)
	{
		// ���̏���������
		DrawLine();
	}
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
			// �v���C���[�̕`�揈��
		DrawPlayer(nCnt, 0);
	}
	// ��
	DrawSky();
	// ���b�V�����̕`�揈��
	DrawMeshField();
	// ���f���̕`�揈��
	DrawModel();
	// ���b�V���ǂ̕`�揈��
	DrawMeshWall();
	DrawEffectUP();
	// �e�̕`�揈��
	DrawShadow();
	// �O�Ղ̕`��
	DrawOrbit();
	// �G�t�F�N�g
	DrawEffect();
	if (g_bPauseView == false)
	{// UI
		DrawEffect2D();
		// SP
		DrawSpGage();
		// �^�C��
		DrawTime();
		// ���b�Z�[�W�E�B���h�E
		DrawMessageWindow();
		DrawUI();
	}
#endif

	if (g_bPause == true)
	{// �|�[�Y
		if (g_bPauseView == false)
		{// �|�[�Y�\������\����
		 // �J�����̐ݒ�
			SetCamera(0);
			DrawPause();
		}
	}
	// �r���[�|�[�g�������Ƃɖ߂�
	pDevice->SetViewport(&viewportDef);


}
//==============================================================================================================================
// �e��I�u�W�F�N�g����
//==============================================================================================================================
void SetPlayerFileData(int nIdy, char *cFilename)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PlayerData *pPlayerData = GetPlayerData();
	Player *pPlayer = GetPlayer();
	FILE *pFile;
	char cLine[MAX_CHAR];	// �s�S�ēǂݍ���
	char cData[MAX_CHAR];	// ��������
	pFile = fopen(cFilename, "r");
	int nLoop = 0;							// ���[�v���邩�ǂ���[0:���[�v���Ȃ� / 1:���[�v����]
	int nOrbit = 0;							// �O�Ղ̐�
	int nOrbitLength = 0;					// �O�Ղ̒���
	int nIdyOrbit = 0;						// �O�Ղ̐e
	ORBITTYPE nOrbitType;					// �O�Ղ̃^�C�v
	int nCollisionIdy = 0;										// �U������̐e
	D3DXVECTOR3 CollisionOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �U������̃I�t�Z�b�g
	float fCollisionRadius = 0;									// �U������̔��a
	int nCollisionStart = 0;									// �U������̃X�^�[�g
	int nCollisionEnd = 0;										// �U������̏I���
	D3DXVECTOR3 OrbitStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �O�Վn��
	D3DXVECTOR3 OrbitEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �O�ՏI���
	int nDamageIdx = 0;											// �_���[�W�ԍ�
	D3DXVECTOR3 DamageOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �_���[�W�I�t�Z�b�g
	float fDamageRadius = 0;									// �_���[�W���a
	int nStopTime = 0;											// �X�g�b�v
	for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
	{// ����̏�����
		pPlayerData[nIdy].ColAttack[nCollision].bUse = false;
	}

	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);
			if (strcmp(cData, "NUM_MODEL") == 0)		// ���f���̐�
			{// �g���ĂȂ�����ǂ�����
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayerData[nIdy].nNumParts);
			}
			// ���f���̃t�@�C���ǂݍ���
			if (strcmp(cData, "MODEL_FILENAME") == 0)
			{
				for (int nCntModel = 0; nCntModel < pPlayerData[nIdy].nNumParts; nCntModel++)
				{
					sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);
					pPlayerData[nIdy].aModel[nCntModel].pFileName = &cData[0];

					// X�t�@�C���̓ǂݍ���
					D3DXLoadMeshFromX(pPlayerData[nIdy].aModel[nCntModel].pFileName,
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&pPlayerData[nIdy].aModel[nCntModel].pBuffMat,
						NULL,
						&pPlayerData[nIdy].aModel[nCntModel].nNumMat,
						&pPlayerData[nIdy].aModel[nCntModel].pMesh);
					D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^
														// �}�e���A���f�[�^�ւ̃|�C���^���擾
					pMat = (D3DXMATERIAL*)pPlayerData[nIdy].aModel[nCntModel].pBuffMat->GetBufferPointer();
					// �e�N�X�`���̔z��ݒ�
					pPlayerData[nIdy].aModel[nCntModel].pTextuer = new LPDIRECT3DTEXTURE9[(int)pPlayerData[nIdy].aModel[nCntModel].nNumMat];
					for (int nCntPlayerTex = 0; nCntPlayerTex < (int)pPlayerData[nIdy].aModel[nCntModel].nNumMat; nCntPlayerTex++)
					{
						pPlayerData[nIdy].aModel[nCntModel].pTextuer[nCntPlayerTex] = NULL;
						if (pMat[nCntPlayerTex].pTextureFilename != NULL)
						{
							// �e�N�X�`���̐ݒ�
							D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
								pMat[nCntPlayerTex].pTextureFilename,	// �t�@�C���̖��O
								&pPlayerData[nIdy].aModel[nCntModel].pTextuer[nCntPlayerTex]);		// �e�N�X�`���ւ̃|�C���^
																										// �e�N�X�`���̐ݒ�
						}
					}
					fgets(&cLine[0], MAX_CHAR, pFile);
				}
			}
			// �v���C���[�̃I�t�Z�b�g���
			if (strcmp(cData, "CHARACTERSET") == 0)
			{
				int nCntPlayerModel = 0;	// ���f���ԍ�
				while (strcmp(cData, "END_CHARACTERSET") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "MOVE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayerData[nIdy].MoveSpeed);
					}
					else if (strcmp(cData, "JUMP") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayerData[nIdy].JumpSpeed);
					}
					else if (strcmp(cData, "RADIUS") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayerData[nIdy].fRadiusWidth);
					}
					else if (strcmp(cData, "HEIGHT") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayerData[nIdy].fRadiusHeight);
					}
					else if (strcmp(cData, "LIFE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayerData[nIdy].nInitLife);
					}
					else if (strcmp(cData, "NUM_PARTS") == 0)		// ���f���̐�
					{// ���f����
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayerData[nIdy].nNumParts);
					}
					else if (strcmp(cData, "BODY_COLLISION") == 0)		// ���f���̐�
					{// ���f����
						sscanf(&cLine[0], "%s %s %d %f %f %f %f", &cData[0], &cData[0], &nDamageIdx, &DamageOffset.x, &DamageOffset.y, &DamageOffset.z, &fDamageRadius);
						SetBodyCollisionPlayer(nIdy, nDamageIdx, DamageOffset, fDamageRadius);
					}
					if (strcmp(cData, "PARTSSET") == 0)
					{
						while (strcmp(cData, "END_PARTSSET") != 0)
						{
							fgets(&cLine[0], MAX_CHAR, pFile);
							sscanf(&cLine[0], "%s", &cData);
							if (strcmp(cData, "INDEX") == 0)
							{
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayerData[nIdy].aModel[nCntPlayerModel].nIdxModel);
							}
							else if (strcmp(cData, "PARENT") == 0)
							{
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayerData[nIdy].aModel[nCntPlayerModel].nIdxModelPareant);
							}
							else if (strcmp(cData, "POS") == 0)
							{
								sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pPlayerData[nIdy].aModel[nCntPlayerModel].pos.x, &pPlayerData[nIdy].aModel[nCntPlayerModel].pos.y, &pPlayerData[nIdy].aModel[nCntPlayerModel].pos.z);
								sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pPlayerData[nIdy].aModel[nCntPlayerModel].Initpos.x, &pPlayerData[nIdy].aModel[nCntPlayerModel].Initpos.y, &pPlayerData[nIdy].aModel[nCntPlayerModel].Initpos.z);
							}
							else if (strcmp(cData, "ROT") == 0)
							{
								sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pPlayerData[nIdy].aModel[nCntPlayerModel].rot.x, &pPlayerData[nIdy].aModel[nCntPlayerModel].rot.y, &pPlayerData[nIdy].aModel[nCntPlayerModel].rot.z);
							}
						}
						nCntPlayerModel++;
					}

				}
			}
			// �v���C���[�̃��[�V�����ݒ�
			if (strcmp(cData, "MOTIONSET") == 0)
			{
				int nCntKey = 0;	// �L�[��
				while (strcmp(cData, "END_MOTIONSET") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "LOOP") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nLoop);
						if (nLoop == 0)
						{// ���[�v���Ȃ�
							pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].bLoop = false;
						}
						else if (nLoop == 1)
						{// ���[�v����
							pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].bLoop = true;
						}
					}
					else if (strcmp(cData, "NUM_KEY") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].nNumKey);
					}
					else if (strcmp(cData, "SETORBIT") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d %f %f %f %f %f %f %d", &cData[0], &cData[0], &nIdyOrbit, &nOrbitLength, &OrbitStart.x, &OrbitStart.y, &OrbitStart.z, &OrbitEnd.x, &OrbitEnd.y, &OrbitEnd.z, &nOrbitType);
						if (pPlayerData[nIdy].orbit[pPlayerData[nIdy].nNumMotion][nOrbit].motion == PlayerMotionType(pPlayerData[nIdy].nNumMotion))
						{
							nOrbit++;
						}
						else
						{
							nOrbit = 0;
						}
						pPlayerData[nIdy].orbit[pPlayerData[nIdy].nNumMotion][nOrbit].nIdyOrbit = nIdyOrbit;				// �O�Ղ̐�
						pPlayerData[nIdy].orbit[pPlayerData[nIdy].nNumMotion][nOrbit].nOrbitLength = nOrbitLength;		// �O�Ղ̒���
						pPlayerData[nIdy].orbit[pPlayerData[nIdy].nNumMotion][nOrbit].OrbitStart = OrbitStart;			// ���_
						pPlayerData[nIdy].orbit[pPlayerData[nIdy].nNumMotion][nOrbit].OrbitEnd = OrbitEnd;				// �I�_
						pPlayerData[nIdy].orbit[pPlayerData[nIdy].nNumMotion][nOrbit].nType = nOrbitType;					// �O�Ղ̃^�C�v
						pPlayerData[nIdy].orbit[pPlayerData[nIdy].nNumMotion][nOrbit].motion = PlayerMotionType(pPlayerData[nIdy].nNumMotion);	// ���[�V����

					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %d %d %d", &cData[0], &cData[0], &nCollisionIdy, &CollisionOffset.x, &CollisionOffset.y, &CollisionOffset.z, &fCollisionRadius, &nCollisionStart, &nCollisionEnd, &nStopTime);
						SetAttackCollisionPlayer(nIdy, nCollisionIdy, CollisionOffset, fCollisionRadius, nCollisionStart, nCollisionEnd, nStopTime, PlayerMotionType(pPlayerData[nIdy].nNumMotion));
					}
					else if (strcmp(cData, "ATTACK_POWER") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].fAttackPower);
					}
					else if (strcmp(cData, "COLLISION_DAMAGE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].fDamageH, &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].fDamageV);
					}
					if (strcmp(cData, "KEYSET") == 0)
					{
						int nCntParts = 0;
						while (strcmp(cData, "END_KEYSET") != 0)
						{
							fgets(&cLine[0], MAX_CHAR, pFile);
							sscanf(&cLine[0], "%s", &cData);
							if (strcmp(cData, "FRAME") == 0)
							{
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].aKey[nCntKey].nFrame);
							}
							else if (strcmp(cData, "KEY") == 0)
							{
								while (strcmp(cData, "END_KEY") != 0)
								{
									fgets(&cLine[0], MAX_CHAR, pFile);
									sscanf(&cLine[0], "%s", &cData);
									if (strcmp(cData, "POS") == 0)
									{
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].aKey[nCntKey].aPartsKey[nCntParts].pos.x, &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].aKey[nCntKey].aPartsKey[nCntParts].pos.y, &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].aKey[nCntKey].aPartsKey[nCntParts].pos.z);
									}
									else if (strcmp(cData, "ROT") == 0)
									{
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].aKey[nCntKey].aPartsKey[nCntParts].rot.x, &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].aKey[nCntKey].aPartsKey[nCntParts].rot.y, &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].aKey[nCntKey].aPartsKey[nCntParts].rot.z);
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayerData[nIdy].aModel[nCntParts].rot.x, &pPlayerData[nIdy].aModel[nCntParts].rot.y, &pPlayerData[nIdy].aModel[nCntParts].rot.z);
									}
								}
								nCntParts++;// ���̃p�[�c��
							}
						}
						nCntKey++;	// ���̃L�[��
					}
				}
				pPlayerData[nIdy].nNumMotion++; // ���̃��[�V������
			}
		}

	}

	fclose(pFile);

}
//==============================================================================================================================
// �e��I�u�W�F�N�g����
//==============================================================================================================================
void SetObjectFileData(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	char cLine[MAX_CHAR];
	char cData[MAX_CHAR];
	int nNumModel;
	pFile = fopen("data/model.txt", "r");
	MeshField *pMeshField = GetMeshField();
	MeshFieldData  *pMeshFieldData = GetMeshFieldData();
	MeshWall *pMeshWall = GetMeshWall();
	MeshWallData *pMeshWallData = GetMeshWallData();
	Model *pModel = GetModel();
	ModelData *pModelData = GetModelData();
	ModelData *pModelData2 = GetModelData();
	for (int nCntMat = 0; nCntMat < MAX_MODEL_TYPE; nCntMat++)
	{
		for (int nCntCollision = 0; nCntCollision < MAX_MODEL_COLLISION; nCntCollision++)
		{
			pModelData[nCntMat].bCollison[nCntCollision] = false;
		}
		pModelData[nCntMat].bCatch = false;// ���ĂȂ�
		pModelData[nCntMat].bGetInfo = false;// ���擾�ł��Ȃ�
	}
	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&cLine[0], "%s", &cData);

			if (strcmp(cData, "NUM_TEXTURE") == 0)
			{// �g���ĂȂ�����ǂ�����
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pMeshFieldData[0].nNumTex);
			}
			if (strcmp(cData, "TEXTURE_FILENAME") == 0)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);
				pMeshFieldData->pFileName = &cData[0];

				D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
					pMeshFieldData->pFileName,							// �t�@�C���̖��O
					&pMeshFieldData->pTexture);							// �e�N�X�`���ւ̃|�C���^
				pMeshFieldData++;
			}
			if (strcmp(cData, "NUM_WALL") == 0)
			{// �g���ĂȂ�����ǂ�����
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pMeshWallData[0].nNumTex);
			}
			if (strcmp(cData, "WALL_FILENAME") == 0)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);
				pMeshWallData->pFileName = &cData[0];

				D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
					pMeshWallData->pFileName,						// �t�@�C���̖��O
					&pMeshWallData->pTexture);						// �e�N�X�`���ւ̃|�C���^
				pMeshWallData++;
			}

			if (strcmp(cData, "NUM_MODEL") == 0)
			{// �g���ĂȂ�����ǂ�����
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nNumModel);
			}

			if (strcmp(cData, "MODEL_FILENAME") == 0)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);
				pModelData->pFileName = &cData[0];

				// ���f���f�[�^�̓ǂݍ���
				pModelData->pMesh = NULL;
				pModelData->pBuffMat = NULL;
				pModelData->nNumMat = 0;
				for (int nCnt = 0; nCnt < MAX_MODEL_COLLISION; nCnt++)
				{
					pModelData->CollisionPos[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					pModelData->fRadiusCollision[nCnt] = 0;
					pModelData->bCollison[nCnt] = false;
				}
				// X�t�@�C���̓ǂݍ���
				D3DXLoadMeshFromX(pModelData->pFileName,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&pModelData->pBuffMat,
					NULL,
					&pModelData->nNumMat,
					&pModelData->pMesh);
				D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^

													// �}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)pModelData->pBuffMat->GetBufferPointer();
				// �e�N�X�`���̔z��ݒ�
				pModelData->pTextuer = new LPDIRECT3DTEXTURE9[(int)pModelData->nNumMat];

				// �e�N�X�`���̓ǂݍ���
				for (int nCntMatTex = 0; nCntMatTex < (int)pModelData->nNumMat; nCntMatTex++)
				{
					pModelData->pTextuer[nCntMatTex] = NULL;

					if (pMat[nCntMatTex].pTextureFilename != NULL)
					{
						// �e�N�X�`���̐ݒ�
						D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
							pMat[nCntMatTex].pTextureFilename,	// �t�@�C���̖��O
							&pModelData->pTextuer[nCntMatTex]);		// �e�N�X�`���ւ̃|�C���^


					}
				}
				pModelData++;
			}
			if (strcmp(cData, "SETCATCH") == 0)
			{
				int nType = 0;
				int nCatch = 0;
				while (strcmp(cData, "ENDSETCATCH") != 0)
				{
					fgets(&cLine[0], 256, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "CATCH") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d", &cData[0], &cData[0], &nType, &nCatch);
						if (nCatch == 1)
						{
							pModelData2[nType].bCatch = true;
						}
					}
				}
			}
			if (strcmp(cData, "SETGETINFO") == 0)
			{// ���擾
				int nType = 0;
				int nGetInfo = 0;
				while (strcmp(cData, "ENDSETGETINFO") != 0)
				{
					fgets(&cLine[0], 256, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "GETINFO") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d", &cData[0], &cData[0], &nType, &nGetInfo);
						if (nGetInfo == 1)
						{
							pModelData2[nType].bGetInfo = true;
						}
					}
				}
			}
			if (strcmp(cData, "SETCOLLISION") == 0)
			{
				int nType = 0;
				D3DXVECTOR3 ofset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				float fRadius = 0;
				float fWidth = 0;
				float fDepth = 0;
				float fHeight = 0;
				while (strcmp(cData, "ENDSETCOLLISION") != 0)
				{
					fgets(&cLine[0], 256, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "COLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %f %f %f", &cData[0], &cData[0], &nType, &ofset.x, &ofset.y, &ofset.z, &fRadius, &fWidth, &fDepth, &fHeight);
						SetCollisionModel(ofset, fRadius, fWidth, fDepth, fHeight, MODELTYPE(nType));
					}
				}
			}
		}
		fclose(pFile);
	}
}
//==============================================================================================================================
// �X�e�[�W�ݒu
//==============================================================================================================================
void SetStagePos(char *cFilename)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	char cLine[MAX_CHAR];
	char cData[MAX_CHAR];
	pFile = fopen(cFilename, "r");
	MeshField *pMeshField = GetMeshField();
	MeshWall *pMeshWall = GetMeshWall();
	Model *pModel = GetModel();
	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&cLine[0], "%s", &cData);
			if (strcmp(cData, "WALLSET") == 0)
			{
				while (strcmp(cData, "END_WALLSET") != 0)
				{
					fgets(&cLine[0], 256, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TEXTYPE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pMeshWall->nType);
					}
					else if (strcmp(cData, "POS") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pMeshWall->pos.x, &pMeshWall->pos.y, &pMeshWall->pos.z);
					}
					else if (strcmp(cData, "ROT") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pMeshWall->rot.x, &pMeshWall->rot.y, &pMeshWall->rot.z);

						if (pMeshWall->rot.y == 90.0f)
						{
							pMeshWall->rot.y = D3DX_PI / 2;
						}
						if (pMeshWall->rot.y == 180.0f)
						{
							pMeshWall->rot.y = D3DX_PI;
						}
						if (pMeshWall->rot.y == -90.0f)
						{
							pMeshWall->rot.y = -D3DX_PI / 2;
						}
					}
					else if (strcmp(cData, "BLOCK") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d", &cData[0], &cData[0], &pMeshWall->nBlock_X, &pMeshWall->nBlock_Y);
					}
					else if (strcmp(cData, "SIZE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pMeshWall->fWidth, &pMeshWall->fHeight);
					}
					else if (strcmp(cData, "CULL") == 0)
					{
						int nCull = 0;
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nCull);
						if (nCull == 0)
						{// �J�����O����
							pMeshWall->bCull = false;
						}
						else
						{// �J�����O���Ȃ�
							pMeshWall->bCull = true;
						}
					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						int nCollision = 0;
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nCollision);
						if (nCollision == 0)
						{// �J�����O����
							pMeshWall->bCollision = false;
						}
						else
						{// �J�����O���Ȃ�
							pMeshWall->bCollision = true;
						}
					}
				}
				SetMeshWall(pMeshWall->pos, pMeshWall->rot, pMeshWall->nType, pMeshWall->nBlock_X, pMeshWall->nBlock_Y, pMeshWall->fWidth, pMeshWall->fHeight, pMeshWall->bCull);
				pMeshWall++;
			}
			if (strcmp(cData, "FIELDSET") == 0)
			{
				while (strcmp(cData, "END_FIELDSET") != 0)
				{
					fgets(&cLine[0], 256, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TEXTYPE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pMeshField->nType);
					}
					else if (strcmp(cData, "POS") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pMeshField->pos.x, &pMeshField->pos.y, &pMeshField->pos.z);
					}
					else if (strcmp(cData, "ROT") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pMeshField->rot.x, &pMeshField->rot.y, &pMeshField->rot.z);
						pMeshField->rot.x /= 57.3248408f;
						pMeshField->rot.y /= 57.3248408f;
						pMeshField->rot.z /= 57.3248408f;
						if (pMeshField->rot.x == 180.0f)
						{
							pMeshField->rot.x = D3DX_PI;
						}
					}
					else if (strcmp(cData, "BLOCK") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d", &cData[0], &cData[0], &pMeshField->nBlock_X, &pMeshField->nBlock_Z);
					}
					else if (strcmp(cData, "SIZE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pMeshField->fWidth, &pMeshField->fDepth);
					}
					else if (strcmp(cData, "SLOPE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pMeshField->fSlopeSin, &pMeshField->fSlopeCos);
					}
					else if (strcmp(cData, "CULL") == 0)
					{
						int nCull = 0;
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nCull);
						if (nCull == 0)
						{// �J�����O����
							pMeshField->bCull = false;
						}
						else
						{// �J�����O���Ȃ�
							pMeshField->bCull = true;
						}
					}
					else if (strcmp(cData, "MOVE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pMeshField->fTexmove.x, &pMeshField->fTexmove.y);
					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						int nCollison = 0;
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nCollison);
						if (nCollison == 0)
						{// ����Ȃ�
							pMeshField->bCollision = false;
						}
						else
						{// ���肠��
							pMeshField->bCollision = true;
						}
					}
				}
				SetMeshField(pMeshField->pos, pMeshField->rot, pMeshField->nType, pMeshField->nBlock_X, pMeshField->nBlock_Z, pMeshField->fWidth, pMeshField->fDepth, pMeshField->fSlopeSin, pMeshField->bCull);
				pMeshField++;
			}

			if (strcmp(cData, "SETMODEL") == 0)
			{
				while (strcmp(cData, "ENDSETMODEL") != 0)
				{
					fgets(&cLine[0], 256, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "POS") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pModel->pos.x, &pModel->pos.y, &pModel->pos.z);
					}
					else if (strcmp(cData, "RANDOMPOS") == 0)
					{
						D3DXVECTOR3 randomPos;
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &randomPos.x, &randomPos.y, &randomPos.z);
						pModel->pos += D3DXVECTOR3((sinf(float(rand() % 624) / 100.0f)*randomPos.x), (cosf(float(rand() % 624) / 100.0f)*randomPos.y), (cosf(float(rand() % 624) / 100.0f)*randomPos.z));
					}
					else if (strcmp(cData, "ROT") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pModel->rot.x, &pModel->rot.y, &pModel->rot.z);
						pModel->rot.x /= 57.3248408f;
						pModel->rot.y /= 57.3248408f;
						pModel->rot.z /= 57.3248408f;
					}
					else if (strcmp(cData, "TYPE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pModel->nType);
					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						int nCollision = 0;
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nCollision);
						if (nCollision == 0)
						{// ����Ȃ�
							pModel->bCollision = false;
						}
						else
						{// ���肠��
							pModel->bCollision = true;
						}
					}
				}
				SetModel(pModel->pos, pModel->rot, pModel->nType);
				pModel++;
			}
		}
		fclose(pFile);
	}
}
//==============================================================================================================================
// ���z�u
//==============================================================================================================================
void PrintStagePos(void)
{
	FILE *pFileQ;						// �t�@�C���^�̃|�C���^�ϐ�
	Model *pModel = GetModel();
	int nCollision = 0;
	pFileQ = fopen("data/�X�e�[�W���.txt", "w");	// ���̓ǂݍ���
	if (pFileQ != NULL)
	{// �t�@�C�����J������
		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
		{
			if (pModel->bUse == true)
			{
				fprintf(pFileQ, "SETMODEL\n");
				fprintf(pFileQ, "	POS = %.1f %.1f %.1f\n", pModel->pos.x, pModel->pos.y, pModel->pos.z);
				fprintf(pFileQ, "	ROT = %.1f %.1f %.1f\n", pModel->rot.x, pModel->rot.y, pModel->rot.z);
				fprintf(pFileQ, "	TYPE = %d\n", pModel->nType);
				if (pModel->bCollision == false)
				{
					nCollision = 0;
				}
				else
				{
					nCollision = 1;
				}
				fprintf(pFileQ, "	COLLISION = %d\n", nCollision);
				fprintf(pFileQ, "ENDSETMODEL\n");
			}
		}
		fclose(pFileQ);
	}
}
//==============================================================================================================================
// �Q�[���̏�Ԃ̐ݒ�
//==============================================================================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}
//==============================================================================================================================
// �Q�[���̏�Ԃ̎擾
//==============================================================================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
int GetGameStateCounter(void)
{
	return g_nCounterGameState;
}
//==============================================================================================================================
// �|�[�Y��Ԃ�ς���
//==============================================================================================================================
void ReleaseGamePause(void)
{
	g_bPause = g_bPause ? false : true;
}
//==============================================================================================================================
// �|�[�Y�擾
//==============================================================================================================================
bool GetPause(void)
{
	return g_bPause;
}
bool GetPauseView(void)
{
	return g_bPauseView;
}

