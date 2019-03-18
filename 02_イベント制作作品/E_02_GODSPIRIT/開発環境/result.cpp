////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���U���g�̏��� [result.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "ranking.h"
#include "gamepad.h"
#include "UI.h"

#include "camera.h"
#include "light.h"
#include "effectUP.h"
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
#include "billboard.h"
#include "BillLife.h"
#include "effect2D.h"
#include "light.h"
#include "MessageWindow.h"
#include "effect3D.h"
#include "game.h"
#include <stdlib.h>
#include <time.h>
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// ���S���W
	RESULTTEXTYPE_GAMECLEAR nType;// ���
	float fWidth;				// ��
	float fHeight;				// ����
	int nNumCnt;				// ������
} ResultInfoGameCrear;
typedef struct
{
	char *pFileName;
}ResultTextureGameCrear;

//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_RESULT_CLEAR	(50)									// ���U���g�N���A
#define SCORE_LEN			(8)
#define TIME_LEN			(3)
#define HIT_LEN				(3)
#define ARROW_LEN				(3)
#define ITEM_LEN			(1)
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexGameCrear(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT_CLEAR] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
RESULTTYPE g_ResultType;								// ���U���g�̃^�C�v
// -------------------------- / �Q�[���N���A /-----------------------------------------------------------------------------------------/
Result g_aResultGameCrear[MAX_RESULT_CLEAR];
ResultInfoGameCrear g_aResultInfoGameCrear[] =
{
	{ D3DXVECTOR3(950, 630, 0.0f), RESULTTEXTYPE_SCORE_MAX ,40,55,SCORE_LEN },
	// ����
	{ D3DXVECTOR3(950, 450, 0.0f), RESULTTEXTYPE_SCORE ,40,55,SCORE_LEN },
	{ D3DXVECTOR3(750, 510, 0.0f), RESULTTEXTYPE_TIME ,40,55,TIME_LEN },
	{ D3DXVECTOR3(750, 570, 0.0f), RESULTTEXTYPE_HIT ,40,55,HIT_LEN },
	// ����
	{ D3DXVECTOR3(480, 630 + 5, 0.0f), RESULTTEXTYPE_TEXT_SCORE_MAX ,400,50,1 },
	{ D3DXVECTOR3(480, 450 + 5, 0.0f), RESULTTEXTYPE_TEXT_SCORE ,400,50,1 },
	{ D3DXVECTOR3(480, 510 + 5, 0.0f), RESULTTEXTYPE_TEXT_TIME ,400,50,1 },
	{ D3DXVECTOR3(480, 570 + 5, 0.0f), RESULTTEXTYPE_TEXT_HIT ,400,50,1 },
	// :
	{ D3DXVECTOR3(630, 450 + 5, 0.0f), RESULTTEXTYPE_TEXT_DOUBLEKORON ,55,55,1 },
	{ D3DXVECTOR3(630, 510 + 5, 0.0f), RESULTTEXTYPE_TEXT_DOUBLEKORON1 ,55,55,1 },
	{ D3DXVECTOR3(630, 570 + 5, 0.0f), RESULTTEXTYPE_TEXT_DOUBLEKORON2 ,55,55,1 },
	{ D3DXVECTOR3(630, 630 + 5, 0.0f), RESULTTEXTYPE_TEXT_DOUBLEKORON3 ,55,55,1 },
	// ������
	{ D3DXVECTOR3(1000, 450 + 5, 0.0f), RESULTTEXTYPE_TEXT_PT ,100,50,1 },
	{ D3DXVECTOR3(800, 510 + 5, 0.0f), RESULTTEXTYPE_TEXT_BYOU ,100,50,1 },
	{ D3DXVECTOR3(830, 570 + 5, 0.0f), RESULTTEXTYPE_TEXT_HITMOJI ,100,50,1 },
	{ D3DXVECTOR3(1000, 630 + 5, 0.0f), RESULTTEXTYPE_TEXT_PT_MAX ,100,50,1 },

};
ResultTextureGameCrear g_aResultTexGameCrear[RESULTTEXTYPE_GAMECLEAR_MAX] =
{
	{ "data/TEXTURE/number000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/number000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/number000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/number000.png" },		// �ǂݍ��ރe�N�X�`��2

	{ "data/TEXTURE/TextSum.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/TextScore.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/TextTime.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/TextHit.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/TextDoubleKoron.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/TextDoubleKoron.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/TextDoubleKoron.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/TextDoubleKoron.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/TextPt.png" },				// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/TextByou.png" },				// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/TextHitMoji.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/TextPt.png" },			// �ǂݍ��ރe�N�X�`��2

};
int g_nResultScoreMax;	// ���U���g�p�̍ő�X�R�A
int g_nResultScore;		// ���U���g�p�̃X�R�A
int g_nResultTime;		// ���U���g�p�^�C��
int g_nResultHit;		// ���U���g�p�ő�q�b�g��

int g_nResultCnt;		// �����J�E���g
int g_nCounterFade;		// �t�F�[�h
int g_nCounterNext;		// ����
int g_nCounterNextAuto;		// ����
int g_nMotionType[4];
//==============================================================================================================================
// �����������i���U���g�j
//==============================================================================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();
	g_nCounterFade = 0;	// �t�F�[�h
	g_nResultCnt = 0;	// �����J�E���g
	g_nCounterNext = 0;// ����
	g_nCounterNextAuto = 0;// ����
	srand((unsigned int)time(0));
	g_nMotionType[0] = rand() % PLAYER_ATTACK_TYPE_MAX;
	g_nMotionType[1] = rand() % PLAYER_ATTACK_TYPE_MAX;
	g_nMotionType[2] = rand() % PLAYER_ATTACK_TYPE_MAX;
	g_nMotionType[3] = rand() % PLAYER_ATTACK_TYPE_MAX;
	if (g_ResultType == RESULTTYPE_CLEAR)
	{
		//�@�X�R�A
		g_nResultScoreMax = g_nResultScore - g_nResultTime * 50 ;
	}
	if (g_ResultType == RESULTTYPE_GAMEOVER)
	{
		//�@�X�R�A
		g_nResultScoreMax = g_nResultScore - g_nResultTime * 50  ;
	}
	if (g_nResultScoreMax <= 0)
	{// �O�ȉ�
		g_nResultScoreMax = 0;
	}
	for (int nTex = 0; nTex < RESULTTEXTYPE_GAMECLEAR_MAX; nTex++)
	{// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			g_aResultTexGameCrear[nTex].pFileName,
			&g_pTextureResult[nTex]);
	}

		// ���̏�����
	for (int nCntResult = 0; nCntResult < MAX_RESULT_CLEAR; nCntResult++)
	{
		g_aResultGameCrear[nCntResult].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���W
		g_aResultGameCrear[nCntResult].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �w�i�̈ړ���
		g_aResultGameCrear[nCntResult].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);// �F
		g_aResultGameCrear[nCntResult].nCounterAnim = 0;						// �J�E���^�[
		g_aResultGameCrear[nCntResult].nPatternAnim = 0;						// �p�^�[��
		g_aResultGameCrear[nCntResult].nCounterFade = 0;						// �t�F�[�h�J�E���^�[
		g_aResultGameCrear[nCntResult].nType = 0;								// �^�C�v
		g_aResultGameCrear[nCntResult].fWidth = 0;								// ��
		g_aResultGameCrear[nCntResult].fHeight = 0;								// �c
		g_aResultGameCrear[nCntResult].bUse = false;							// �g�p����Ă��邩�ǂ���
	}
	MakeVertexGameCrear(pDevice);
	for (int nCnt = 0; nCnt < sizeof g_aResultInfoGameCrear / sizeof(ResultInfoGameCrear); nCnt++)
	{// �Z�b�g
		for (int nCnt2 = 0; nCnt2 < g_aResultInfoGameCrear[nCnt].nNumCnt; nCnt2++)
		{
			SetResult(g_aResultInfoGameCrear[nCnt].pos - D3DXVECTOR3((g_aResultInfoGameCrear[nCnt].fWidth)*nCnt2, 0.0f, 0.0f)
				, g_aResultInfoGameCrear[nCnt].nType, g_aResultInfoGameCrear[nCnt].fWidth, g_aResultInfoGameCrear[nCnt].fHeight);
		}
	}
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
	// UI�̏���������
	InitUI();
	// �G�t�F�N�g�QD�̏���������
	InitEffect2D();
	// ���b�Z�[�W�E�B���h�E�̏���������
	InitMessageWindow();
	// �G�t�F�N�g�RD�̏���������
	InitEffect3D();
	// �G�t�F�N�g�̏���������
	InitEffect();
	//------------------------------/ ���f���̃t�@�C���ǂݍ��� /---------------------------------------------------------------------------//
	SetStagePos("data/stage1.txt");
	GetPlayer()->pos = D3DXVECTOR3(0.0f, 0.0f, 500.0f);
	GetPlayer()->rot.y = 0;
	GetCamera()->nState = CAMERASTATE_BOSS_MOVIE_START;
	GetCamera()->fHeight = -30;
	GetCamera()->posR = GetPlayer()->pos;
	GetCamera()->Destrot.x = D3DX_PI / 4;
	GetCamera()->Destrot.y = D3DX_PI;
	GetCamera()->rot.x = D3DX_PI / 4;
	GetCamera()->rot.y = D3DX_PI;
	SetMessageWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, 550, 0.0f), 900, 300, MESSAGETEX_NONE);
}

//==============================================================================================================================
// �I�������i���U���g�j
//==============================================================================================================================
void UninitResult(void)
{
		// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < MAX_RESULT_CLEAR; nCntTex++)
	{
		if (g_pTextureResult[nCntTex] != NULL)
		{
			g_pTextureResult[nCntTex]->Release();

			g_pTextureResult[nCntTex] = NULL;
		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
	SortRanking(g_nResultScoreMax);
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
	// UI�̏I������
	UninitUI();
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

}
//==============================================================================================================================
// �X�V�����i���U���g�j
//==============================================================================================================================
void UpdateResult(void)
{

	FADE pFADE;
	// �t�F�[�h�̎擾
	pFADE = GetFade();
	Player *pPlayer = GetPlayer();
	if (pFADE == FADE_NONE)
	{
		g_nCounterNextAuto++;
		// ����L�[���������ǂ���
		if (g_nCounterNextAuto % 60 == 0)
		{
			if (g_nCounterNext < 4)
			{// ������6�����܂�
				g_nCounterNext++;
				if (g_nCounterNext >= 4)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);
				}
				else
				{
					PlaySound(SOUND_LABEL_SE_SELECT);
				}
			}
		}
		else if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_J) == true || GetGamePadTrigger(BUTTON_B) == true)
		{
			g_nCounterNext++;
			g_nCounterNextAuto = 0;
			if (g_nCounterNext >= 4)
			{
				PlaySound(SOUND_LABEL_SE_DECIDE);

			}
			else
			{
				PlaySound(SOUND_LABEL_SE_SELECT);
			}
		}


		if (g_nCounterNext >= 5)
		{// �A�łŏI���
			SetFade(MODE_RANKING);
		}
		if (g_nCounterNext >= 4)
		{
			g_nCounterNext = 4;
			g_nCounterFade++;
		}
		if (g_nCounterFade >= 300)
		{// �����Ń����L���O�֐���
			SetFade(MODE_RANKING);
		}
	}
	
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntResult = 0; nCntResult < MAX_RESULT_CLEAR; nCntResult++, pVtx += 4)
	{
		if (g_aResultGameCrear[nCntResult].bUse == true)
		{
			switch (g_nCounterNext)
			{
			case 0:
				break;
			case 1:
				if (g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_SCORE || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_SCORE
					|| g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_DOUBLEKORON || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_PT)
				{
					g_aResultGameCrear[nCntResult].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// �U���̎�ޕ����[�v
					if (pPlayer->bAttackType[nCntAttack] == true)
					{// �U������������
						pPlayer->bAttackType[nCntAttack] = false;								// �U����Ԃ�����
					}
				}
				pPlayer->bAttackType[g_nMotionType[0]] = true;
				break;
			case 2:
				if (g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TIME || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_TIME
					|| g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_DOUBLEKORON1 || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_BYOU)
				{
					g_aResultGameCrear[nCntResult].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// �U���̎�ޕ����[�v
					if (pPlayer->bAttackType[nCntAttack] == true)
					{// �U������������
						pPlayer->bAttackType[nCntAttack] = false;								// �U����Ԃ�����
					}
				}
				pPlayer->bAttackType[g_nMotionType[1]] = true;
				break;
			case 3:
				if (g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_HIT || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_HIT
					|| g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_DOUBLEKORON2 || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_HITMOJI)
				{
					g_aResultGameCrear[nCntResult].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// �U���̎�ޕ����[�v
					if (pPlayer->bAttackType[nCntAttack] == true)
					{// �U������������
						pPlayer->bAttackType[nCntAttack] = false;								// �U����Ԃ�����
					}
				}
				pPlayer->bAttackType[g_nMotionType[2]] = true;
				break;
			case 4:
				if (g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_SCORE_MAX || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_SCORE_MAX
					|| g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_DOUBLEKORON3 || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_PT_MAX)
				{
					g_aResultGameCrear[nCntResult].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// �U���̎�ޕ����[�v
					if (pPlayer->bAttackType[nCntAttack] == true)
					{// �U������������
						pPlayer->bAttackType[nCntAttack] = false;								// �U����Ԃ�����
					}
				}
				pPlayer->bAttackType[g_nMotionType[3]] = true;
	
				break;
			}

		
		}
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(g_aResultGameCrear[nCntResult].fWidth / 2), -(g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;
		pVtx[1].pos = D3DXVECTOR3((g_aResultGameCrear[nCntResult].fWidth / 2), -(g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;
		pVtx[2].pos = D3DXVECTOR3(-(g_aResultGameCrear[nCntResult].fWidth / 2), (g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;
		pVtx[3].pos = D3DXVECTOR3((g_aResultGameCrear[nCntResult].fWidth / 2), (g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;
		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_aResultGameCrear[nCntResult].col;
		pVtx[1].col = g_aResultGameCrear[nCntResult].col;
		pVtx[2].col = g_aResultGameCrear[nCntResult].col;
		pVtx[3].col = g_aResultGameCrear[nCntResult].col;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();
	GetCamera()->fHeight = 20;

	UpdateCamera();
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
	// �O�Ղ̍X�V����
	UpdateOrbit();
	// UI�̍X�V����
	UpdateUI();
	// �r���{�[�h���C�t�̍X�V����
	UpdateBillLife();
	// �G�t�F�N�g�QD�̍X�V����
	UpdateEffect2D();
	// �G�t�F�N�g3D�̍X�V����
	UpdateEffect3D();
	// ���b�Z�[�W�E�B���h�E
	UpdateMessageWindow();

}
//==============================================================================================================================
// �`�揈���i���U���g�j
//==============================================================================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Player *pPlayer = GetPlayer();
	Camera *pCamera = GetCamera();
	D3DVIEWPORT9 viewportDef;
	// �r���[�|�[�g�����擾
	pDevice->GetViewport(&viewportDef);

	// �e��I�u�W�F�N�g�̕`�揈��
	// �J�����̐ݒ�
	SetCamera(0);
	if (GetDrawDebug()[DRAW_DEBUG_LINE] == true)
	{
		// ���̏���������
		DrawLine();
	}

	// ��̕`�揈��
	DrawSky();
	// ���b�V���ǂ̕`�揈��
	DrawMeshWall();
	// ���b�V�����̏���������
	DrawMeshCylinder();
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
	// ���b�Z�[�W�E�B���h�E�̕`�揈��
	DrawMessageWindow();


	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCntResult = 0; nCntResult < MAX_RESULT_CLEAR; nCntResult++)
	{
		if (g_aResultGameCrear[nCntResult].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResult[g_aResultGameCrear[nCntResult].nType]);
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntResult * 4,
				2);
		}
	}
	

}

//==============================================================================================================================
// �ݒ�
//==============================================================================================================================
void SetResult(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight)
{
	VERTEX_2D*pVtx;

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntResult = 0; nCntResult < MAX_RESULT_CLEAR; nCntResult++, pVtx += 4)
	{
		if (g_aResultGameCrear[nCntResult].bUse == false)
		{// �w�i���g�p����Ă��Ȃ�
			g_aResultGameCrear[nCntResult].pos = pos;
			g_aResultGameCrear[nCntResult].nType = nType;
			g_aResultGameCrear[nCntResult].fWidth = fWidth;
			g_aResultGameCrear[nCntResult].fHeight = fHeight;
			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-(g_aResultGameCrear[nCntResult].fWidth / 2), -(g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aResultGameCrear[nCntResult].fWidth / 2), -(g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aResultGameCrear[nCntResult].fWidth / 2), (g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aResultGameCrear[nCntResult].fWidth / 2), (g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;

			switch (g_aResultGameCrear[nCntResult].nType)
			{
				int nData;
			case RESULTTEXTYPE_SCORE_MAX:
				nData = int(powf(10, float(g_nResultCnt)));
				pVtx[0].tex = D3DXVECTOR2((g_nResultScoreMax % (10 * nData) / nData) * 0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_nResultScoreMax % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_nResultScoreMax % (10 * nData) / nData)* 0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_nResultScoreMax % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
				g_nResultCnt++;
				if (g_nResultCnt >= SCORE_LEN)
				{// �������������ݏ�̌W����0�ɂ���
					g_nResultCnt = 0;
				}
				break;
			case RESULTTEXTYPE_SCORE:
				nData = int(powf(10, float(g_nResultCnt)));
				pVtx[0].tex = D3DXVECTOR2((g_nResultScore % (10 * nData) / nData) * 0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_nResultScore % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_nResultScore % (10 * nData) / nData)* 0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_nResultScore % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
				g_nResultCnt++;
				if (g_nResultCnt >= SCORE_LEN)
				{// �������������ݏ�̌W����0�ɂ���
					g_nResultCnt = 0;
				}
				break;
			case RESULTTEXTYPE_TIME:
				nData = int(powf(10, float(g_nResultCnt)));
				pVtx[0].tex = D3DXVECTOR2((g_nResultTime % (10 * nData) / nData) * 0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_nResultTime % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_nResultTime % (10 * nData) / nData)* 0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_nResultTime % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
				g_nResultCnt++;
				if (g_nResultCnt >= TIME_LEN)
				{// �������������ݏ�̌W����0�ɂ���
					g_nResultCnt = 0;
				}
				break;
			case RESULTTEXTYPE_HIT:
				nData = int(powf(10, float(g_nResultCnt)));
				pVtx[0].tex = D3DXVECTOR2((g_nResultHit % (10 * nData) / nData) * 0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_nResultHit % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_nResultHit % (10 * nData) / nData)* 0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_nResultHit % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
				g_nResultCnt++;
				if (g_nResultCnt >= HIT_LEN)
				{// �������������ݏ�̌W����0�ɂ���
					g_nResultCnt = 0;
				}
				break; 
			}
		
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aResultGameCrear[nCntResult].col;
			pVtx[1].col = g_aResultGameCrear[nCntResult].col;
			pVtx[2].col = g_aResultGameCrear[nCntResult].col;
			pVtx[3].col = g_aResultGameCrear[nCntResult].col;

			g_aResultGameCrear[nCntResult].bUse = true;	// �g�p���Ă����Ԃɂ���
			break;
		}
	}
	g_pVtxBuffResult->Unlock();
}
//==============================================================================================================================
// ���U���g�^�C�v�̎擾
//==============================================================================================================================
RESULTTYPE GetResultType(void)
{
	return g_ResultType;
}
//==============================================================================================================================
// ���_���̐ݒ�  �N���A
//==============================================================================================================================
void MakeVertexGameCrear(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RESULT_CLEAR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultGameCrear = 0; nCntResultGameCrear < MAX_RESULT_CLEAR; nCntResultGameCrear++, pVtx += 4)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(g_aResultGameCrear[nCntResultGameCrear].fWidth / 2), -(g_aResultGameCrear[nCntResultGameCrear].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResultGameCrear].pos;
		pVtx[1].pos = D3DXVECTOR3((g_aResultGameCrear[nCntResultGameCrear].fWidth / 2), -(g_aResultGameCrear[nCntResultGameCrear].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResultGameCrear].pos;
		pVtx[2].pos = D3DXVECTOR3(-(g_aResultGameCrear[nCntResultGameCrear].fWidth / 2), (g_aResultGameCrear[nCntResultGameCrear].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResultGameCrear].pos;
		pVtx[3].pos = D3DXVECTOR3((g_aResultGameCrear[nCntResultGameCrear].fWidth / 2), (g_aResultGameCrear[nCntResultGameCrear].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResultGameCrear].pos;
		// rhw�̐ݒ�
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}

		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_aResultGameCrear[nCntResultGameCrear].col;
		pVtx[1].col = g_aResultGameCrear[nCntResultGameCrear].col;
		pVtx[2].col = g_aResultGameCrear[nCntResultGameCrear].col;
		pVtx[3].col = g_aResultGameCrear[nCntResultGameCrear].col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();
}

//==============================================================================================================================
// �X�R�A�̕\�L
//==============================================================================================================================
void GetScore(int nScore)
{
	g_nResultScore = nScore;
}
void GetTime(int nTime)
{
	g_nResultTime = nTime;
}
void GetHit(int nHit)
{
	g_nResultHit = nHit;
}
//==============================================================================================================================
// ���U���g�̎�ނ̐ݒ�
//==============================================================================================================================
void SetResultType(RESULTTYPE type)
{
	g_ResultType = type;
}