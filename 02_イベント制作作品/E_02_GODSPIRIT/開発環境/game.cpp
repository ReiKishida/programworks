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
#include "effectUP.h"
#include "input.h"
#include "gamepad.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include "enemy.h"
#include "effect.h"
#include "meshField.h"
#include "meshWall.h"
#include "meshCylinder.h"
#include "sky.h"
#include "pause.h"
#include "fade.h"
#include "item.h"
#include "line.h"
#include "orbit.h"
#include "time.h"
#include "score.h"
#include "billboard.h"
#include "hit.h"
#include "UI.h"
#include "life.h"
#include "meshFar.h"
#include "BillLife.h"
#include "effect2D.h"
#include "result.h"
#include "light.h"
#include "MessageWindow.h"
#include "effect3D.h"
#include "spirit.h"
#include "meshDome.h"
#include "meshDome2.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_CHAR	(256)
//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
GAMESTATE g_gameState = GAMESTATE_GAME1;	// �Q�[���̏��
int g_nCounterGameState;				// ��ԊǗ��J�E���^�[
bool g_bPause;							// �|�[�Y�����ǂ���
bool g_bPauseView;						// �|�[�Y��\��
bool g_bGameState[GAMESTAGEMAX];		// �Q�[���؂�ւ��
bool g_bOpenBoss;
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitGame(void)
{
	// �e��I�u�W�F�N�g�̏���������
	g_bPause = false;	// �|�[�Y���Ȃ�
	g_bPauseView = false;// �|�[�Y�������Ȃ�
	for (int nCntState = 0; nCntState < GAMESTAGEMAX; nCntState++)
	{
		g_bGameState[nCntState] = false;
	}
	g_gameState = GAMESTATE_GAME1;
//	g_bGameState[GAMESTAGE2] = true;
	g_bOpenBoss = false;
	g_nCounterGameState = 0;
#if 1
	// �J�����̏���������
	InitCamera();
	// ���̏���������
	InitLine();
	// ���C�g�̏���������
	InitLight();
	// ��̏���������
	InitSky();
	// �������G�t�F�N�g�̏���������
	InitEffectUP();
	// ���b�V�����̏���������
	InitMeshField();
	// ���b�V���ǂ̏���������
	InitMeshWall();
	// ���b�V�����̏���������
	InitMeshCylinder();
	// �e�̏���������
	InitShadow();
	// �v���C���[�̏���������
	InitPlayer();
	// ���f���̏���������
	InitModel();
	// �r���{�[�h�̏���������
	InitBillboard();
	// �r���{�[�h���C�t�̏���������
	InitBillLife();
	// �G�̏���������
	InitEnemy();
	// �A�C�e���̏���������
	InitItem();
	// �|�[�Y�̏���������
	InitPause();
	// ���Ԃ̏���������
	InitTime();
	// �X�R�A�̏���������
	InitScore();
	// �q�b�g�̏���������
	InitHit();
	// UI�̏���������
	InitUI();
	// ���C�t�̏���������
	InitLife();
	// �X�s���b�g�̏���������
	InitSpirit();
	// �~�̏���������
	InitMeshFar();
	// �G�t�F�N�g�QD�̏���������
	InitEffect2D();
	// ���b�Z�[�W�E�B���h�E�̏���������
	InitMessageWindow();
	// �G�t�F�N�g�RD�̏���������
	InitEffect3D();
	// �G�t�F�N�g�̏���������
	InitEffect();
	// ���b�V���h�[���̏���������
	InitMeshDome();
	InitMeshDome2();
	//------------------------------/ ���f���̃t�@�C���ǂݍ��� /---------------------------------------------------------------------------//
	if (g_gameState == GAMESTATE_GAME1)
	{
		SetStagePos("data/stage1.txt");
		// �r���{�[�h�̈ʒu
		SetBillboardFileData("data/BillboardStage1.txt");
	}
	if (g_gameState == GAMESTATE_BOSS)
	{
		SetStagePos("data/stageBoss.txt");
	}
#endif

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
	// ��̏I������
	UninitSky();
	// �������G�t�F�N�g�̏I������
	UninitEffectUP();
	// ���b�V�����̏I������
	UninitMeshCylinder();
	// �r���{�[�h�̏I������
	UninitBillboard();
	// �e�̏I������
	UninitShadow();
	// �A�C�e���̏I������
	UninitItem();
	// �|�[�Y�̏I������
	UninitPause();
	// ���Ԃ̏I������
	UninitTime();
	// �X�R�A�̏I������
	UninitScore();
	// �q�b�g�̏I������
	UninitHit();
	// UI�̏I������
	UninitUI();
	// ���C�t�̏I������
	UninitLife();
	// �X�s���b�g�̏I������
	UninitSpirit();
	// �~�̏I������
	UninitMeshFar();
	// �r���{�[�h���C�t�̏I������
	UninitBillLife();
	// �G�t�F�N�g�QD�̏I������
	UninitEffect2D();
	// ���b�Z�[�W�E�B���h�E�̏I������
	UninitMessageWindow();
	// �G�t�F�N�g�̏I������
	UninitEffect();
	// �G�t�F�N�g3D�̏I������
	UninitEffect3D();
	// ���b�V���h�[���̏I������
	UninitMeshDome();
	UninitMeshDome2();
	g_gameState = GAMESTATE_GAME1;
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
	Model *pModel = GetModel();
#if _DEBUG
	if (GetKeyboardTrigger(DIK_0) == true)
	{
		DeleteLine(LINETYPE_MODEL);
		InitModel();
		InitMeshField();
		InitMeshWall();
		InitBillboard();
		SetStagePos("data/stage1.txt");
		SetBillboardFileData("data/BillboardStage1.txt");

	}
	if (GetKeyboardTrigger(DIK_F9) == true)
	{
		PrintStagePos();
	}
	if (GetKeyboardTrigger(DIK_F5) == true)
	{
		g_bPauseView = true;
	}
#endif
	if (GetKeyboardTrigger(DIK_P) == true||GetGamePadTrigger(BUTTON_START)==true)
	{
		if (g_bPauseView == true)
		{
			g_bPauseView = false;
		}
		PlaySound(SOUND_LABEL_SE_PAUSE);
		g_bPause = g_bPause ? false : true;
	}
	if (g_bPause == true)
	{
		if (GetKeyboardTrigger(DIK_V) == true || GetGamePadTrigger(BUTTON_A) == true)
		{
			g_bPauseView = g_bPauseView ? false : true;
		}
	}
	if (g_bPause == false)
	{
		// �e��I�u�W�F�N�g�̍X�V����
		// �J�����̍X�V����
		UpdateCamera();
		if (pCamera->nState != CAMERASTATE_BOSS_MOVIE)
		{
			// ���̍X�V����
			UpdateLine();
			// ���C�g�̍X�V����
			UpdateLight();
			// ��̍X�V����
			UpdateSky();
			// �G�t�F�N�g�̍X�V����
			UpdateEffect();
			// �������G�t�F�N�g�̍X�V����
			UpdateEffectUP();
			// ���b�V�����̍X�V����
			UpdateMeshField();
			// ���b�V���ǂ̍X�V����
			UpdateMeshWall();
			// ���b�V�����̏���������
			UpdateMeshCylinder();
			// �v���C���[�̍X�V����
			UpdatePlayer(0);
			// ���f���̍X�V����
			UpdateModel();
			// �r���{�[�h�̍X�V����
			UpdateBillboard();
			// �e�̍X�V����
			UpdateShadow();
			// �G�̍X�V����
			UpdateEnemy();
			// �A�C�e���̍X�V����
			UpdateItem();
			// ���Ԃ̍X�V����
			UpdateTime();
			// �X�R�A�̍X�V����
			UpdateScore();
			// �O�Ղ̍X�V����
			UpdateOrbit();
			// �q�b�g�̍X�V����
			UpdateHit();
			// UI�̍X�V����
			UpdateUI();
			// ���C�t�̍X�V����
			UpdateLife();
			// �X�s���b�g�̍X�V����
			UpdateSpirit();
			// �~�̍X�V����
			UpdateMeshFar();
			// �r���{�[�h���C�t�̍X�V����
			UpdateBillLife();
			// �G�t�F�N�g�QD�̍X�V����
			UpdateEffect2D();
			// �G�t�F�N�g3D�̍X�V����
			UpdateEffect3D();
			// ���b�V���h�[���̍X�V����
			UpdateMeshDome();
			UpdateMeshDome2();
		}
		// ���b�Z�[�W�E�B���h�E�̍X�V����
		UpdateMessageWindow();

		switch (g_gameState)
		{
		case GAMESTATE_GAME1:
			if (g_bOpenBoss == false)
			{// �Ƃт炵�܂��Ă�
			}
			if (GetFade() == FADE_NONE)
			{
				if (pPlayer->pos.x <= -3500 && pPlayer->pos.x >= -4000)
				{// �{�X�ɍs��
					if (pPlayer->pos.z <= -2070)
					{
						SetFadeNone();
						g_bGameState[GAMESTAGEBOSS] = true;
					}
				}
			}
			if (g_bGameState[GAMESTAGEBOSS] == true)
			{
				if (GetFade() == FADE_IN)
				{
					UninitBillLife();
					InitBillLife();
					InitEnemy();
					InitModel();
					InitMeshField();
					InitMeshWall();
					SetStagePos("data/stageBoss.txt");
					g_gameState = GAMESTATE_BOSS_START;
				}
			}
			break;

		case GAMESTATE_BOSS_START:

			g_gameState = GAMESTATE_BOSS;

			break;
		case GAMESTATE_BOSS:
			if (g_bGameState[GAMESTAGEBOSS] == true)
			{// �{�X�ɂȂ�����
				StopSound(SOUND_LABEL_GAMEBGM1);
				PlaySound(SOUND_LABEL_BOSSBGM);
				g_bGameState[GAMESTAGEBOSS] = false;
				pCamera->nState = CAMERASTATE_BOSS_MOVIE;
				pCamera->nStateCounter = 20;
				pCamera->rot.x = 0.8f;
				pCamera->rot.y = 0.0f;
				pCamera->rot.z = 0.99f;
				pCamera->posV = D3DXVECTOR3(-3740.0f, 400.0f, -2700);
			}
			if (GetFade() == FADE_NONE)
			{
				for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
				{
					if (pModel[nCntModel].nType == MODELTYPE_BOSSSAKU)
					{
						pModel[nCntModel].pos.y -= 6;
						if (pModel[nCntModel].pos.y <= pModel[nCntModel].Initpos.y)
						{
							pModel[nCntModel].pos.y = pModel[nCntModel].Initpos.y;
						}
					}
				}
			}
			break;
		case GAMESTATE_END:
			g_nCounterGameState++;
			if (g_nCounterGameState == 120)
			{
				if (GetResultType() == RESULTTYPE_CLEAR)
				{
					pCamera->fLength = 250;
					pCamera->fHeight = 200;
					pCamera->rot.y = pPlayer->rot.y;
					pCamera->posR.x = pPlayer->pos.x + sinf(D3DX_PI / 2 + pCamera->rot.y) * pCamera->AttentionPos.x + sinf(pPlayer->rot.y - D3DX_PI) *pCamera->fLengthR;
					pCamera->posR.y = pPlayer->pos.y + cosf(pPlayer->rot.x) *pCamera->fHeightR;
					pCamera->posR.z = pPlayer->pos.z + cosf(D3DX_PI / 2 + pCamera->rot.y)*pCamera->AttentionPos.x + cosf(pPlayer->rot.y - D3DX_PI) * pCamera->fLengthR;
					pCamera->nState = CAMERASTATE_NONE;
				}
				g_gameState = GAMESTATE_END2;
			}			
			break;
		case GAMESTATE_END2:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 250)
			{
				g_gameState = GAMESTATE_NONE;
				// ��ʃ��[�h�̐ݒ�
				SetFade(MODE_RESULT);
			}
			break;
		}
	}
	else
	{
		// �J�����̍X�V����
		UpdateCamera();
		if (g_bPauseView == false)
		{
			UpdatePause();		// �|�[�Y
		}

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

	// ��̕`�揈��
//	DrawSky();
// ���b�V���h�[���̍X�V����
	DrawMeshDome();
	DrawMeshDome2();
	// ���b�V���ǂ̕`�揈��
	DrawMeshWall();
	// �~�̕`�揈��
	DrawMeshFar();
	// ���b�V�����̏���������
//	DrawMeshCylinder();
	// �v���C���[�̕`�揈��
	DrawPlayer(0);
	// ���f���̕`�揈��
	DrawModel();
	// ���b�V�����̕`�揈��
	DrawMeshField();
	// �e�̕`�揈��
	DrawShadow();
	// �������G�t�F�N�g�̕`�揈��
	DrawEffectUP();
	// �r���{�[�h�̕`�揈��
	DrawBillboard();
	// �G�̕`�揈��
	DrawEnemy();
	// �G�t�F�N�g�̕`�揈��
	DrawEffect();
	// �O�Ղ̕`�揈��
	DrawOrbit();
	// �A�C�e���̕`�揈��
	DrawItem();
	// �G�t�F�N�g#D�̕`�揈��
	DrawEffect3D();

	if (pPlayer->state != PLAYERSTATE_DEATH&&pCamera->nState != CAMERASTATE_BOSS_MOVIE_START)
	{
		if (g_bPauseView == false)
		{// �|�[�Y�\������\�����@���S���[�r�[��
		 // �r���{�[�h���C�t�̕`�揈��
			DrawBillLife();
			// ���Ԃ̕`�揈��
			DrawTime();
			// �X�R�A�̕`�揈��
			DrawScore();
			// �q�b�g�̕`�揈��
			DrawHit();
			// ���C�t�̕`�揈��
			DrawLife();
			// �X�s���b�g�̕`�揈��
			DrawSpirit();
			// �G�t�F�N�g2D�̕`�揈��
			DrawEffect2D();

		}
	}

	if (g_bPauseView == false && pCamera->nState != CAMERASTATE_BOSS_MOVIE_START)
	{
		// UI�̕`�揈��
		DrawUI();
		if (pPlayer->state != PLAYERSTATE_DEATH)
		{
			// �v���C���[�̕`�揈��
			SetCamera(1);
			// �y�o�b�t�@�̃r���[�|�[�g�̃G���A���N���A
			pDevice->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
			// ���C�g�e���󂯂Ȃ�
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			DrawPlayer(0);
			// ���C�g�e���󂯂Ȃ�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			// �r���[�|�[�g�������Ƃɖ߂�
			pDevice->SetViewport(&viewportDef);
		}
		DrawUI();
	}
	// ���b�Z�[�W�E�B���h�E�̕`�揈��
	DrawMessageWindow();
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
void SetPlayerFileData(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Player *pPlayer = GetPlayer();
	FILE *pFile;
	char cLine[MAX_CHAR];	// �s�S�ēǂݍ���
	char cData[MAX_CHAR];	// ��������
	pFile = fopen("data/player.txt", "r");
	pPlayer->nNumMotion = 0;				// ���[�V�����ԍ����Z�b�g
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
	D3DXVECTOR3 OrbitStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 OrbitEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nDamageIdx = 0;
	D3DXVECTOR3 DamageOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fDamageRadius = 0;
	int nCntWeapon = 0;
	for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
	{// ����̏��
		for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
		{// ����̏�����
			pPlayer->CollisionAttack[nCollision][nCntWeapon].bUse = false;
		}
	}
	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);
			if (strcmp(cData, "NUM_MODEL") == 0)		// ���f���̐�
			{// �g���ĂȂ�����ǂ�����
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->nNumParts);
			}
			if (strcmp(cData, "ALLMODEL_FILENAME") == 0)
			{
				int nCntModel = 0;	// ���f���ԍ�
				while (strcmp(cData, "END_ALLMODEL_FILENAME") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					// ���f���̃t�@�C���ǂݍ���
					if (strcmp(cData, "MODEL_FILENAME") == 0)
					{
						sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);
						pPlayer->aModel[nCntModel][nCntWeapon].pFileName = &cData[0];

						// X�t�@�C���̓ǂݍ���
						D3DXLoadMeshFromX(pPlayer->aModel[nCntModel][nCntWeapon].pFileName,
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,
							&pPlayer->aModel[nCntModel][nCntWeapon].pBuffMat,
							NULL,
							&pPlayer->aModel[nCntModel][nCntWeapon].nNumMat,
							&pPlayer->aModel[nCntModel][nCntWeapon].pMesh);
						D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^
						// �}�e���A���f�[�^�ւ̃|�C���^���擾
						pMat = (D3DXMATERIAL*)pPlayer->aModel[nCntModel][nCntWeapon].pBuffMat->GetBufferPointer();
						// �e�N�X�`���̔z��ݒ�
						pPlayer->aModel[nCntModel][nCntWeapon].pTextuer = new LPDIRECT3DTEXTURE9[(int)pPlayer->aModel[nCntModel][nCntWeapon].nNumMat];
						for (int nCntPlayerTex = 0; nCntPlayerTex < (int)pPlayer->aModel[nCntModel][nCntWeapon].nNumMat; nCntPlayerTex++)
						{
							pPlayer->aModel[nCntModel][nCntWeapon].pTextuer[nCntPlayerTex] = NULL;
							if (pMat[nCntPlayerTex].pTextureFilename != NULL)
							{
								// �e�N�X�`���̐ݒ�
								D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
									pMat[nCntPlayerTex].pTextureFilename,	// �t�@�C���̖��O
									&pPlayer->aModel[nCntModel][nCntWeapon].pTextuer[nCntPlayerTex]);		// �e�N�X�`���ւ̃|�C���^
							}
						}
						nCntModel++;
					}
				}
				nCntWeapon++;

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
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayer->MoveSpeed);
					}
					else if (strcmp(cData, "JUMP") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayer->JumpSpeed);
					}
					else if (strcmp(cData, "RADIUS") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayer->fRadiusWidth);
					}
					else if (strcmp(cData, "HEIGHT") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayer->fRadiusHeight);
					}
					else if (strcmp(cData, "LIFE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->nInitLife);
					}
					else if (strcmp(cData, "SPIRIT") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->nInitSpirit);
					}
					else if (strcmp(cData, "NUM_PARTS") == 0)		// ���f���̐�
					{// ���f����
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->nNumParts);
					}
					else if (strcmp(cData, "BODY_COLLISION") == 0)		// ���f���̐�
					{// ���f����
						sscanf(&cLine[0], "%s %s %d %f %f %f %f", &cData[0], &cData[0], &nDamageIdx,&DamageOffset.x, &DamageOffset.y, &DamageOffset.z,&fDamageRadius);
						SetBodyCollisionPlayer(nDamageIdx, DamageOffset, fDamageRadius);
					}
					if (strcmp(cData, "PARTSSET") == 0)
					{
						while (strcmp(cData, "END_PARTSSET") != 0)
						{
							fgets(&cLine[0], MAX_CHAR, pFile);
							sscanf(&cLine[0], "%s", &cData);
							if (strcmp(cData, "INDEX") == 0)
							{
								for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
								{
									sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aModel[nCntPlayerModel][nCntWeapon].nIdxModel);
								}
							}
							else if (strcmp(cData, "PARENT") == 0)
							{
								for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
								{
									sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aModel[nCntPlayerModel][nCntWeapon].nIdxModelPareant);
								}
							}
							else if (strcmp(cData, "POS") == 0)
							{
								for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
								{
									sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pPlayer->aModel[nCntPlayerModel][nCntWeapon].pos.x, &pPlayer->aModel[nCntPlayerModel][nCntWeapon].pos.y, &pPlayer->aModel[nCntPlayerModel][nCntWeapon].pos.z);
									sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pPlayer->aModel[nCntPlayerModel][nCntWeapon].Initpos.x, &pPlayer->aModel[nCntPlayerModel][nCntWeapon].Initpos.y, &pPlayer->aModel[nCntPlayerModel][nCntWeapon].Initpos.z);
								}
							}
							else if (strcmp(cData, "ROT") == 0)
							{
								for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
								{
									sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pPlayer->aModel[nCntPlayerModel][nCntWeapon].rot.x, &pPlayer->aModel[nCntPlayerModel][nCntWeapon].rot.y, &pPlayer->aModel[nCntPlayerModel][nCntWeapon].rot.z);
								}
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
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].bLoop = false;
						}
						else if (nLoop == 1)
						{// ���[�v����
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].bLoop = true;
						}
					}
					else if (strcmp(cData, "NUM_KEY") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].nNumKey);
					}
					else if (strcmp(cData, "SETORBIT") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d %f %f %f %f %f %f %d", &cData[0], &cData[0], &nIdyOrbit, &nOrbitLength, &OrbitStart.x, &OrbitStart.y, &OrbitStart.z, &OrbitEnd.x, &OrbitEnd.y, &OrbitEnd.z, &nOrbitType);
						// �O�Րݒ�
						SetOrbit(nIdyOrbit, nOrbitLength, D3DXVECTOR3(OrbitStart.x, OrbitStart.y, OrbitStart.z), D3DXVECTOR3(OrbitEnd.x, OrbitEnd.y, OrbitEnd.z), nOrbitType, PlayerMotionType(pPlayer->nNumMotion),PLAYERWEAPON_HAND);
						nOrbit++;
					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %d %d", &cData[0], &cData[0], &nCollisionIdy, &CollisionOffset.x, &CollisionOffset.y, &CollisionOffset.z,&fCollisionRadius, &nCollisionStart, &nCollisionEnd);
						SetAttackCollisionPlayer(nCollisionIdy, CollisionOffset,fCollisionRadius, nCollisionStart, nCollisionEnd,PlayerMotionType(pPlayer->nNumMotion), PLAYERWEAPON_HAND);
					}
					else if (strcmp(cData, "ATTACK_POWER") == 0)
					{
						sscanf(&cLine[0], "%s %s %hhd", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].nAttackPower);
					}
					else if (strcmp(cData, "COLLISION_DAMAGE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].fDamageHorizontal, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].fDamageVertical);
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
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].aKey[nCntKey].nFrame);
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
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].aKey[nCntKey].aPartsKey[nCntParts].pos.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].aKey[nCntKey].aPartsKey[nCntParts].pos.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].aKey[nCntKey].aPartsKey[nCntParts].pos.z);
									}
									else if (strcmp(cData, "ROT") == 0)
									{
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].aKey[nCntKey].aPartsKey[nCntParts].rot.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].aKey[nCntKey].aPartsKey[nCntParts].rot.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].aKey[nCntKey].aPartsKey[nCntParts].rot.z);
									}
								}
								nCntParts++;// ���̃p�[�c��
							}
						}
						nCntKey++;	// ���̃L�[��
					}
				}
				pPlayer->nNumMotion++; // ���̃��[�V������
			}
		}

	}
	fclose(pFile);
//-------------------------------------/ �����[�V�����̓ǂݍ��� /--------------------------------------------------------//
	FILE *pFile2;
	pFile2 = fopen("data/TEXT/player_sord.txt", "r");
	pPlayer->nNumMotion = 0;				// ���[�V�����ԍ����Z�b�g

	if (pFile2 != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);
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
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].bLoop = false;
						}
						else if (nLoop == 1)
						{// ���[�v����
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].bLoop = true;
						}
					}
					else if (strcmp(cData, "NUM_KEY") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].nNumKey);
					}
					else if (strcmp(cData, "SETORBIT") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d %f %f %f %f %f %f %d", &cData[0], &cData[0], &nIdyOrbit, &nOrbitLength, &OrbitStart.x, &OrbitStart.y, &OrbitStart.z, &OrbitEnd.x, &OrbitEnd.y, &OrbitEnd.z, &nOrbitType);
						// �O�Րݒ�
						SetOrbit(nIdyOrbit, nOrbitLength, D3DXVECTOR3(OrbitStart.x, OrbitStart.y, OrbitStart.z), D3DXVECTOR3(OrbitEnd.x, OrbitEnd.y, OrbitEnd.z), nOrbitType, PlayerMotionType(pPlayer->nNumMotion), PLAYERWEAPON_SORD);
					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %d %d", &cData[0], &cData[0], &nCollisionIdy, &CollisionOffset.x, &CollisionOffset.y, &CollisionOffset.z, &fCollisionRadius, &nCollisionStart, &nCollisionEnd);
						SetAttackCollisionPlayer(nCollisionIdy, CollisionOffset, fCollisionRadius, nCollisionStart, nCollisionEnd, PlayerMotionType(pPlayer->nNumMotion), PLAYERWEAPON_SORD);
					}
					else if (strcmp(cData, "ATTACK_POWER") == 0)
					{
						sscanf(&cLine[0], "%s %s %hhd", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].nAttackPower);
					}
					else if (strcmp(cData, "COLLISION_DAMAGE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].fDamageHorizontal, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].fDamageVertical);
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
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].aKey[nCntKey].nFrame);
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
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].aKey[nCntKey].aPartsKey[nCntParts].pos.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].aKey[nCntKey].aPartsKey[nCntParts].pos.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].aKey[nCntKey].aPartsKey[nCntParts].pos.z);
									}
									else if (strcmp(cData, "ROT") == 0)
									{
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].aKey[nCntKey].aPartsKey[nCntParts].rot.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].aKey[nCntKey].aPartsKey[nCntParts].rot.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].aKey[nCntKey].aPartsKey[nCntParts].rot.z);
									}
								}
								nCntParts++;// ���̃p�[�c��
							}
						}
						nCntKey++;	// ���̃L�[��
					}
				}
				pPlayer->nNumMotion++; // ���̃��[�V������
			}
		}
	}
	fclose(pFile2);
//-------------------------------------/ �����[�V�����̓ǂݍ��� /--------------------------------------------------------//
	FILE *pFile3;
	pFile3 = fopen("data/TEXT/player_lance.txt", "r");
	pPlayer->nNumMotion = 0;				// ���[�V�����ԍ����Z�b�g

	if (pFile3 != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);
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
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].bLoop = false;
						}
						else if (nLoop == 1)
						{// ���[�v����
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].bLoop = true;
						}
					}
					else if (strcmp(cData, "NUM_KEY") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].nNumKey);
					}
					else if (strcmp(cData, "SETORBIT") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d %f %f %f %f %f %f %d", &cData[0], &cData[0], &nIdyOrbit, &nOrbitLength, &OrbitStart.x, &OrbitStart.y, &OrbitStart.z, &OrbitEnd.x, &OrbitEnd.y, &OrbitEnd.z, &nOrbitType);
						// �O�Րݒ�
						SetOrbit(nIdyOrbit, nOrbitLength, D3DXVECTOR3(OrbitStart.x, OrbitStart.y, OrbitStart.z), D3DXVECTOR3(OrbitEnd.x, OrbitEnd.y, OrbitEnd.z), nOrbitType, PlayerMotionType(pPlayer->nNumMotion), PLAYERWEAPON_LANCE);
					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %d %d", &cData[0], &cData[0], &nCollisionIdy, &CollisionOffset.x, &CollisionOffset.y, &CollisionOffset.z, &fCollisionRadius, &nCollisionStart, &nCollisionEnd);
						SetAttackCollisionPlayer(nCollisionIdy, CollisionOffset, fCollisionRadius, nCollisionStart, nCollisionEnd, PlayerMotionType(pPlayer->nNumMotion), PLAYERWEAPON_LANCE);
					}
					else if (strcmp(cData, "ATTACK_POWER") == 0)
					{
						sscanf(&cLine[0], "%s %s %hhd", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].nAttackPower);
					}
					else if (strcmp(cData, "COLLISION_DAMAGE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].fDamageHorizontal, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].fDamageVertical);
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
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].aKey[nCntKey].nFrame);
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
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].aKey[nCntKey].aPartsKey[nCntParts].pos.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].aKey[nCntKey].aPartsKey[nCntParts].pos.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].aKey[nCntKey].aPartsKey[nCntParts].pos.z);
									}
									else if (strcmp(cData, "ROT") == 0)
									{
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].aKey[nCntKey].aPartsKey[nCntParts].rot.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].aKey[nCntKey].aPartsKey[nCntParts].rot.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].aKey[nCntKey].aPartsKey[nCntParts].rot.z);
									}
								}
								nCntParts++;// ���̃p�[�c��
							}
						}
						nCntKey++;	// ���̃L�[��
					}
				}
				pPlayer->nNumMotion++; // ���̃��[�V������
			}
		}
	}
	fclose(pFile3);
	//-------------------------------------/ ���ƃ��[�V�����̓ǂݍ��� /--------------------------------------------------------//
	FILE *pFile4;
	pFile4 = fopen("data/TEXT/player_hammer.txt", "r");
	pPlayer->nNumMotion = 0;				// ���[�V�����ԍ����Z�b�g

	if (pFile4 != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);
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
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].bLoop = false;
						}
						else if (nLoop == 1)
						{// ���[�v����
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].bLoop = true;
						}
					}
					else if (strcmp(cData, "NUM_KEY") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].nNumKey);
					}
					else if (strcmp(cData, "SETORBIT") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d %f %f %f %f %f %f %d", &cData[0], &cData[0], &nIdyOrbit, &nOrbitLength, &OrbitStart.x, &OrbitStart.y, &OrbitStart.z, &OrbitEnd.x, &OrbitEnd.y, &OrbitEnd.z, &nOrbitType);
						// �O�Րݒ�
						SetOrbit(nIdyOrbit, nOrbitLength, D3DXVECTOR3(OrbitStart.x, OrbitStart.y, OrbitStart.z), D3DXVECTOR3(OrbitEnd.x, OrbitEnd.y, OrbitEnd.z), nOrbitType, PlayerMotionType(pPlayer->nNumMotion), PLAYERWEAPON_HAMMER);
					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %d %d", &cData[0], &cData[0], &nCollisionIdy, &CollisionOffset.x, &CollisionOffset.y, &CollisionOffset.z, &fCollisionRadius, &nCollisionStart, &nCollisionEnd);
						SetAttackCollisionPlayer(nCollisionIdy, CollisionOffset, fCollisionRadius, nCollisionStart, nCollisionEnd, PlayerMotionType(pPlayer->nNumMotion), PLAYERWEAPON_HAMMER);
					}
					else if (strcmp(cData, "ATTACK_POWER") == 0)
					{
						sscanf(&cLine[0], "%s %s %hhd", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].nAttackPower);
					}
					else if (strcmp(cData, "COLLISION_DAMAGE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].fDamageHorizontal, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].fDamageVertical);
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
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].aKey[nCntKey].nFrame);
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
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].aKey[nCntKey].aPartsKey[nCntParts].pos.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].aKey[nCntKey].aPartsKey[nCntParts].pos.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].aKey[nCntKey].aPartsKey[nCntParts].pos.z);
									}
									else if (strcmp(cData, "ROT") == 0)
									{
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].aKey[nCntKey].aPartsKey[nCntParts].rot.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].aKey[nCntKey].aPartsKey[nCntParts].rot.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].aKey[nCntKey].aPartsKey[nCntParts].rot.z);
									}
								}
								nCntParts++;// ���̃p�[�c��
							}
						}
						nCntKey++;	// ���̃L�[��
					}
				}
				pPlayer->nNumMotion++; // ���̃��[�V������
			}
		}
	}
	fclose(pFile4);

}
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
	for (int nCntMat = 0; nCntMat < MAX_MODEL_TYPE; nCntMat++)
	{
		for (int nCntCollision = 0; nCntCollision < MAX_MODEL_COLLISION; nCntCollision++)
		{
			pModelData[nCntMat].bCollison[nCntCollision] = false;
		}
	}
	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL )
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
			if (strcmp(cData, "SETCOLLISION") == 0)
			{
				int nType = 0;
				D3DXVECTOR3 ofset = D3DXVECTOR3(0.0f,0.0f,0.0f);
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
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %f %f %f", &cData[0], &cData[0], &nType,&ofset.x,&ofset.y,&ofset.z,&fRadius,&fWidth,&fDepth,&fHeight);
						SetCollisionModel(ofset, fRadius,fWidth,fDepth,fHeight,MODELTYPE(nType));				
					}
				}
			}
		}
		fclose(pFile);
	}
}
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
						//pMeshField->rot.x /= 57.3248408f;
						//pMeshField->rot.y /= 57.3248408f;
						//pMeshField->rot.z /= 57.3248408f;
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
void SetBillboardFileData(char *cFilename)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	char cLine[MAX_CHAR];
	char cData[MAX_CHAR];
	pFile = fopen(cFilename, "r");
	Billboard *pBillboard = GetBillboard();
	BillboardData *pBillboardData = GetBillboardData();

	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&cLine[0], "%s", &cData);

			if (strcmp(cData, "NUM_TEXTURE") == 0)
			{// �g���ĂȂ�����ǂ�����
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pBillboardData[0].nNumTex);
			}
			if (strcmp(cData, "TEXTURE_FILENAME") == 0)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);
				pBillboardData->pFileName = &cData[0];

				D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
					pBillboardData->pFileName,		// �t�@�C���̖��O
					&pBillboardData->pTexture);		// �e�N�X�`���ւ̃|�C���^
				pBillboardData++;
			}
			if (strcmp(cData, "BILLBOARDSET") == 0)
			{
				while (strcmp(cData, "END_BILLBOARDSET") != 0)
				{
					fgets(&cLine[0], 256, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TEXTYPE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pBillboard->nType);
					}
					else if (strcmp(cData, "POS") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pBillboard->pos.x, &pBillboard->pos.y, &pBillboard->pos.z);
					}
					else if (strcmp(cData, "ROT") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pBillboard->rot.x, &pBillboard->rot.y, &pBillboard->rot.z);
						pBillboard->rot.x /= 57.3248408f;
						pBillboard->rot.y /= 57.3248408f;
						pBillboard->rot.z /= 57.3248408f;
					}
					else if (strcmp(cData, "SIZE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pBillboard->fWidth, &pBillboard->fHeight);
					}
				}
				SetBillboard(pBillboard->pos, pBillboard->rot, pBillboard->nType, pBillboard->fWidth, pBillboard->fHeight);
				pBillboard++;
			}
		}
	}
}
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