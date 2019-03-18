////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �X�R�A���� [score.cpp]
// Author : RyotaKinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "score.h"
#include "effect.h"
#include "ranking.h"
#include "result.h"
#include "input.h"
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// �u���b�N�̒��S���W
	D3DXCOLOR  col;			// �ړ���
	SCORETEX nType;			// �u���b�N�̎��
	float fWidth;				// �u���b�N�̕�
	float fHeight;				// �u���b�N�̍���
	int nNumCnt;				// ������
} ScoreInfo;
typedef struct
{
	char *pFileName;
}ScoreTexture;
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_SCORE	(30)
#define MAX_LEN_SCORE	(8)// �X�R�A�̌���

//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureScore[SCORETEX_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
Score					g_aScore[MAX_SCORE];
int						g_nScore;					// �X�R�A
int						g_nScoreDest;					// �X�R�A
ScoreInfo g_aScoreInfo[] =
{
	// �X�R�A�w�i
	{ D3DXVECTOR3(250,45.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,0.6f),SCORETEX_SCORE_BG,480,90 ,1 },
	// SCORE
	//{ D3DXVECTOR3(410.0f,40.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),SCORETEX_SCORE_NAME,70,70,1 },
	// �X�R�A����
	{ D3DXVECTOR3(380,40.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),SCORETEX_SCORE,40,60,MAX_LEN_SCORE },
};

// �e�N�X�`����
ScoreTexture  g_aScoreTex[SCORETEX_MAX] =
{
	{ "data/TEXTURE/scoreBG001.png" },	// �X�R�A�w�i
	{ "data/TEXTURE/score001.png" },	// �X�R�A���O
	{ "data/TEXTURE/number000.png" },	// �X�R�A����	
};
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nTex = 0; nTex < SCORETEX_MAX; nTex++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
			g_aScoreTex[nTex].pFileName,		// �t�@�C���̖��O
			&g_pTextureScore[nTex]);	// �e�N�X�`���ւ̃|�C���^
	}
	// ������
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		g_aScore[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���S
		g_aScore[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
		g_aScore[nCnt].nType = SCORETEX_MAX;						// ���
		g_aScore[nCnt].fWidth = 0;									// ��
		g_aScore[nCnt].fHeight = 0;									// ����
		g_aScore[nCnt].bUse = false;								// �g�p����Ă��邩�ǂ���
	}
	g_nScore = 0;	// �X�R�A
	g_nScoreDest = 0;
	// ���_���̍쐬
	MakeVertexScore(pDevice);
	for (int nCnt = 0; nCnt < sizeof g_aScoreInfo / sizeof(ScoreInfo); nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < g_aScoreInfo[nCnt].nNumCnt; nCnt2++)
		{
			SetScore(g_aScoreInfo[nCnt].pos - D3DXVECTOR3((g_aScoreInfo[nCnt].fWidth)*nCnt2, 0.0f, 0.0f), g_aScoreInfo[nCnt].col, g_aScoreInfo[nCnt].nType, g_aScoreInfo[nCnt].fWidth, g_aScoreInfo[nCnt].fHeight);
		}
	}
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitScore(void)
{
	// �e�N�X�`���̊J��
	for (int nTex = 0; nTex < SCORETEX_MAX; nTex++)
	{
		if (g_pTextureScore[nTex] != NULL)
		{
			g_pTextureScore[nTex]->Release();
			g_pTextureScore[nTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
	GetScore(g_nScore);
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateScore(void)
{
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
	if (g_nScoreDest <= g_nScore)
	{
		g_nScoreDest += rand() % 100;
	}
	if (g_nScoreDest >= g_nScore)
	{
		g_nScoreDest = g_nScore;
	}
	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	int nCntPow = 0;
	// �e�N�X�`�����W�̐ݒ�
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++, pVtx += 4)
	{
		if (g_aScore[nCnt].nType == SCORETEX_SCORE)
		{// �X�R�A�̂݉��Z�ł��ꂽ���ŗݏ�𐳂�������
			int nData = int(powf(10, float(nCntPow)));

			pVtx[0].tex = D3DXVECTOR2((g_nScoreDest % (10 * nData) / nData) * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_nScoreDest % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_nScoreDest % (10 * nData) / nData)* 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_nScoreDest % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
			nCntPow++;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nTex = 0; nTex < MAX_SCORE; nTex++)
	{
		if (g_aScore[nTex].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureScore[g_aScore[nTex].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nTex,
				2);
		}
	}
}

//==============================================================================================================================
// ���_���̍쐬
//==============================================================================================================================
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
						// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++, pVtx += 4)
	{
		// ���_����ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aScore[nCnt].fWidth / 2, -g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aScore[nCnt].fWidth / 2, -g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aScore[nCnt].fWidth / 2, g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aScore[nCnt].fWidth / 2, g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
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
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}
//==============================================================================================================================
// �X�R�A�̐ݒ�
//==============================================================================================================================
void SetScore(D3DXVECTOR3 pos, D3DXCOLOR col, SCORETEX nType, float fWidth, float fHeight)
{
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
						// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++, pVtx += 4)
	{
		if (g_aScore[nCnt].bUse == false)
		{
			g_aScore[nCnt].pos = pos;			// ���S
			g_aScore[nCnt].col = col;			// �F
			g_aScore[nCnt].nType = nType;		// ���
			g_aScore[nCnt].fWidth = fWidth;		// ��
			g_aScore[nCnt].fHeight = fHeight;	// ����
			g_aScore[nCnt].bUse = true;

			// ���_����ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aScore[nCnt].fWidth / 2, -g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aScore[nCnt].fWidth / 2, -g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aScore[nCnt].fWidth / 2, g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aScore[nCnt].fWidth / 2, g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
			if (g_aScore[nCnt].nType == SCORETEX_SCORE)
			{
				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
			}
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aScore[nCnt].col;
			pVtx[1].col = g_aScore[nCnt].col;
			pVtx[2].col = g_aScore[nCnt].col;
			pVtx[3].col = g_aScore[nCnt].col;
			break;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//==============================================================================================================================
// �X�R�A�̉��Z
//==============================================================================================================================
int AddScore(int nValue)
{
	g_nScore += nValue;
	return g_nScore;
}
