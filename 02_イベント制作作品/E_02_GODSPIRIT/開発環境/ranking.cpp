////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����L���O���� [ranking.cpp]
// Author : RyotaKinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "effect.h"
#include "sound.h"
#include "gamepad.h"
#include "game.h"

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
#include "meshDome.h"
#include "meshDome2.h"

//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// �u���b�N�̒��S���W
	D3DXCOLOR  col;			// �ړ���
	RANKINGTYPE nType;			// �u���b�N�̎��
	float fWidth;				// �u���b�N�̕�
	float fHeight;				// �u���b�N�̍���
	int nNumCnt;				// ������
} RankingInfo;
typedef struct
{
	char *pFileName;
}RankingTexture;
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_RANKING (5)
#define MAX_RANKING_POLYGON	(51)
#define AUTO_NEXT_TIME		(300)		// ������ʐ���
#define FLASHTIME			(8)			// �_�ŕp�x /2
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRanking[RANKINGTYPE_MAX] = {};							// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;									// ���_�o�b�t�@�ւ̃|�C���^
static int				g_nRanking[MAX_RANKING] = {	500000,400000,300000,250000,100000};	// �X�R�A
int						nCounterFade;												// �t�F�[�h�J�E���^�|
int						g_nScoreNow;												// ���̃X�R�A
int						nCountScore;												// �X�R�A�̓_��
bool					bHeightScore;
int g_nRankCnt;
int g_nLenRank;
int nAnimCounter;
Ranking g_aRanking[MAX_RANKING_POLYGON];
RankingInfo g_aRankingInfo[] =
{

	{ D3DXVECTOR3(SCREEN_WIDTH / 2 ,100 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCOREBG1,SCREEN_WIDTH - 200, 0 ,1 },
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,250 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCOREBG2,SCREEN_WIDTH - 200, 0 ,1 },
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,400 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCOREBG3,SCREEN_WIDTH - 100, 0 ,1},
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,540 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCOREBG4,SCREEN_WIDTH, 0 ,1 },
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,650 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCOREBG5,SCREEN_WIDTH - 300, 0 ,1 },

	{ D3DXVECTOR3(950,100 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORE1,100, 120 ,8 },
	{ D3DXVECTOR3(900,230 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORE2,90, 100 ,8 },
	{ D3DXVECTOR3(850,360 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORE3,70, 90 ,8 },
	{ D3DXVECTOR3(800,490 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORE4,60, 80 ,8 },
	{ D3DXVECTOR3(800,620 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORE5,60, 80 ,8},

	{ D3DXVECTOR3(100,100 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORENUM1,120, 120 ,1 },
	{ D3DXVECTOR3(100,230 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORENUM2,110, 100 ,1 },
	{ D3DXVECTOR3(100,360 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORENUM3,90, 90 ,1 },
	{ D3DXVECTOR3(100,490 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORENUM4,80, 80 ,1 },
	{ D3DXVECTOR3(100,620 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORENUM5,80, 80 ,1 },
};
RankingTexture g_aRankingTex[RANKINGTYPE_MAX] = 
{
	{ "data/TEXTURE/sori.png" },	// �ǂݍ��ރe�N�X�`���t�@�C����
	{ "data/TEXTURE/sori.png" },	// �ǂݍ��ރe�N�X�`���t�@�C����
	{ "data/TEXTURE/sori.png" },	// �ǂݍ��ރe�N�X�`���t�@�C����
	{ "data/TEXTURE/sori.png" },	// �ǂݍ��ރe�N�X�`���t�@�C����
	{ "data/TEXTURE/sori.png" },	// �ǂݍ��ރe�N�X�`���t�@�C����

	{ "data/TEXTURE/number000.png" },	// �ǂݍ��ރe�N�X�`���t�@�C����
	{ "data/TEXTURE/number000.png" },	// �ǂݍ��ރe�N�X�`���t�@�C����
	{ "data/TEXTURE/number000.png" },	// �ǂݍ��ރe�N�X�`���t�@�C����
	{ "data/TEXTURE/number000.png" },	// �ǂݍ��ރe�N�X�`���t�@�C����
	{ "data/TEXTURE/number000.png" },	// �ǂݍ��ރe�N�X�`���t�@�C����

	{ "data/TEXTURE/rank1.png" },	// �ǂݍ��ރe�N�X�`���t�@�C����
	{ "data/TEXTURE/rank2.png" },	// �ǂݍ��ރe�N�X�`���t�@�C����
	{ "data/TEXTURE/rank3.png" },	// �ǂݍ��ރe�N�X�`���t�@�C����
	{ "data/TEXTURE/rank4.png" },	// �ǂݍ��ރe�N�X�`���t�@�C����
	{ "data/TEXTURE/rank5.png" },	// �ǂݍ��ރe�N�X�`���t�@�C����

};
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitRanking(void)
{
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

	SetStagePos("data/stageTitle.txt");
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nTex = 0; nTex < RANKINGTYPE_MAX; nTex++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
			g_aRankingTex[nTex].pFileName,		// �t�@�C���̖��O
			&g_pTextureRanking[nTex]);			// �e�N�X�`���ւ̃|�C���^
	}
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING_POLYGON; nCntRanking++)
	{
		g_aRanking[nCntRanking].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRanking[nCntRanking].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRanking[nCntRanking].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aRanking[nCntRanking].nType = RANKINGTYPE_SCOREBG1;
		g_aRanking[nCntRanking].fWidth = 0;
		g_aRanking[nCntRanking].fHeight = 0;
		g_aRanking[nCntRanking].bUse = false;
	}
	nCounterFade = 0;
	nCountScore = 0;
	g_nRankCnt = 0;		// �����̗ݏ�\��
	g_nLenRank = 0;		// �T���邽��
	nAnimCounter = 0;	// �����L���O�A�j���[�V����
	// ���_���̍쐬
	MakeVertexRanking(pDevice);

	for (int nCnt = 0; nCnt < sizeof g_aRankingInfo / sizeof(RankingInfo); nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < g_aRankingInfo[nCnt].nNumCnt; nCnt2++)
		{
			SetRanking(g_aRankingInfo[nCnt].pos - D3DXVECTOR3(g_aRankingInfo[nCnt].fWidth * nCnt2, 0, 0.0f), g_aRankingInfo[nCnt].col, g_aRankingInfo[nCnt].nType, g_aRankingInfo[nCnt].fWidth, g_aRankingInfo[nCnt].fHeight);
		}
	}

}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitRanking(void)
{
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

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < RANKINGTYPE_MAX; nCnt++)
	{
		if (g_pTextureRanking[nCnt] != NULL)
		{
			g_pTextureRanking[nCnt]->Release();
			g_pTextureRanking[nCnt] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateRanking(void)
{
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
	// �r���{�[�h���C�t�̍X�V����
	UpdateBillLife();
	// �G�t�F�N�g�QD�̍X�V����
	UpdateEffect2D();
	// �G�t�F�N�g3D�̍X�V����
	UpdateEffect3D();
	// ���b�Z�[�W�E�B���h�E
	UpdateMessageWindow();
	// ���b�V���h�[���̍X�V����
	UpdateMeshDome();
	UpdateMeshDome2();

	FADE pFADE;
	// �t�F�[�h�̎擾
	pFADE = GetFade();

	if (pFADE == FADE_NONE)
	{
		for (int nCntKey = 0; nCntKey < BUTTON_MAX; nCntKey++)
		{
			// ����L�[���������ǂ���
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(nCntKey) == true)
			{
				// ���[�h�ݒ�
				SetFade(MODE_TITLE);
				bHeightScore = false;
			}
		}
		nCounterFade++;
		if (nCounterFade % AUTO_NEXT_TIME == 0)
		{// �����Ń^�C�g���ɐ���
			SetFade(MODE_TITLE);
			bHeightScore = false;
		}
	}
//-----------------------------------/ �����L���O�F�_�� /--------------------------------------------------------//

	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
	nCountScore++;
	nCountScore = nCountScore % FLASHTIME;
	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * RANKINGTYPE_SCORE1;
	for (int nCntRanking = 0; nCntRanking < 5; nCntRanking++)
	{
		if (g_aRanking[nCntRanking].bUse == true)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++, pVtx += 4)
			{
				if (g_nRanking[nCntRanking] == g_nScoreNow)
				{
					if (nCountScore < FLASHTIME / 2)
					{
						g_aRanking[nCntRanking].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
					}
					else if (nCountScore >= FLASHTIME / 2)
					{
						g_aRanking[nCntRanking].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.3f);
					}
					// ���_�J���[�̐ݒ�
					pVtx[0].col = g_aRanking[nCntRanking].col;
					pVtx[1].col = g_aRanking[nCntRanking].col;
					pVtx[2].col = g_aRanking[nCntRanking].col;
					pVtx[3].col = g_aRanking[nCntRanking].col;

				}
			}

		}
	}
		
	
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();

//-----------------------------------/ �����L���O�̈ړ� /--------------------------------------------------------//
	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);
	nAnimCounter++;

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING_POLYGON; nCntRanking++, pVtx += 4)
	{
		if (g_aRanking[nCntRanking].bUse == true)
		{
			g_aRanking[nCntRanking].pos += g_aRanking[nCntRanking].move;

			if (nAnimCounter >= 0)
			{// �P��
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE1)
				{// �X�R�A�ړ�
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 100) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCOREBG1)
				{// �X�R�A�w�i�ړ�
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 100) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORENUM1)
				{// ����
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 100) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
			}
			if (nAnimCounter >= 20)
			{// �Q��
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE2 )
				{// �X�R�A�ړ�
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 230) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCOREBG2)
				{// �X�R�A�w�i�ړ�
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 250) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORENUM2)
				{// ����
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 230) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
			}
			if (nAnimCounter >= 40)
			{// �R��
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE3 )
				{// �X�R�A�ړ�
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 360) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCOREBG3)
				{// �X�R�A�w�i�ړ�
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 400) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORENUM3)
				{// ����
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 360) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
			}
			if (nAnimCounter >= 60)
			{// �S��
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE4)
				{// �X�R�A�ړ�
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 490) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCOREBG4)
				{// �X�R�A�w�i�ړ�
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 540) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORENUM4)
				{// ����
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 490) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
			}
			if (nAnimCounter >= 80)
			{// �T��
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE5)
				{// �X�R�A�ړ�
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 620) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280) g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCOREBG5)
				{// �X�R�A�w�i�ړ�
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 640) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORENUM5)
				{// ����
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 620) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-(g_aRanking[nCntRanking].fWidth / 2), -(g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aRanking[nCntRanking].fWidth / 2), -(g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aRanking[nCntRanking].fWidth / 2), (g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aRanking[nCntRanking].fWidth / 2), (g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();
}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawRanking(void)
{
	SetCamera(0);
	if (GetDrawDebug()[DRAW_DEBUG_LINE] == true)
	{
		DrawLine();
	}	
	// ���b�V���h�[���̍X�V����
	DrawMeshDome();
	DrawMeshDome2();

	// ��̕`�揈��
	//DrawSky();
	// ���b�V���ǂ̕`�揈��
	DrawMeshWall();
	// ���b�V�����̏���������
	//DrawMeshCylinder();
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

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCnt = 0; nCnt < MAX_RANKING_POLYGON;nCnt++)
	{
		if (g_aRanking[nCnt].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureRanking[g_aRanking[nCnt].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nCnt,
				2);
		}
	}
}

//==============================================================================================================================
// ���_���̍쐬
//==============================================================================================================================
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKING_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);
//-----------------------------------/ �����L���O /--------------------------------------------------------//
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING_POLYGON ; nCntRanking++, pVtx += 4)
	{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-(g_aRanking[nCntRanking].fWidth / 2), -(g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aRanking[nCntRanking].fWidth / 2), -(g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aRanking[nCntRanking].fWidth / 2), (g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aRanking[nCntRanking].fWidth / 2), (g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			// rhw�̐ݒ�
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				pVtx[nCnt].rhw = 1.0f;
			}
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();

}

//==============================================================================================================================
// �����L���O�̓���ւ�菈��
//==============================================================================================================================
void SetRanking(D3DXVECTOR3 pos, D3DXCOLOR  col, RANKINGTYPE nType, float fWidth, float fHeight)
{
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);
	//-----------------------------------/ �����L���O /--------------------------------------------------------//
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING_POLYGON; nCntRanking++, pVtx += 4)
	{
		if (g_aRanking[nCntRanking].bUse == false)
		{
			g_aRanking[nCntRanking].pos = pos;
			g_aRanking[nCntRanking].col = col;
			g_aRanking[nCntRanking].nType = nType;
			g_aRanking[nCntRanking].fWidth = fWidth;
			g_aRanking[nCntRanking].fHeight = fHeight;
			g_aRanking[nCntRanking].bUse = true;
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-(g_aRanking[nCntRanking].fWidth / 2), -(g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aRanking[nCntRanking].fWidth / 2), -(g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aRanking[nCntRanking].fWidth / 2), (g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aRanking[nCntRanking].fWidth / 2), (g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			
			if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE1||g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE2 || g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE3 ||
				g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE4 || g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE5)
			{
				int nData = int(powf(10, float(g_nRankCnt)));

				pVtx[0].tex = D3DXVECTOR2((g_nRanking[g_nLenRank] % (10 * nData) / nData)* 0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_nRanking[g_nLenRank] % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_nRanking[g_nLenRank] % (10 * nData) / nData)* 0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_nRanking[g_nLenRank] % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
				g_nRankCnt++;
				if (g_nRankCnt >= 8)
				{
					g_nRankCnt = 0;
					g_nLenRank++;
				}
			}
			break;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();
}

//==============================================================================================================================
// �����L���O�̓���ւ�菈��
//==============================================================================================================================
void SortRanking(int nScore)
{
	int nRank;
	int aData[6];
	aData[5] = nScore;

	for (int nCntNum = 0; nCntNum < 5; nCntNum++)
	{
		aData[nCntNum] = g_nRanking[nCntNum];
	}
	for (int nCntNum = 1; nCntNum < 6; nCntNum++)
	{
		for (int nCntNum1 = nCntNum; nCntNum1 > 0; nCntNum1--)
		{
			if (aData[nCntNum1 - 1] <= aData[nCntNum1])
			{
				nRank = aData[nCntNum1];
				aData[nCntNum1] = aData[nCntNum1 - 1];
				aData[nCntNum1 - 1] = nRank;
			}
		}
	}
	for (int nCntNum = 0; nCntNum < 5; nCntNum++)
	{
		g_nRanking[nCntNum] = aData[nCntNum];
	}
	bHeightScore = true;
	g_nScoreNow = nScore;

}
