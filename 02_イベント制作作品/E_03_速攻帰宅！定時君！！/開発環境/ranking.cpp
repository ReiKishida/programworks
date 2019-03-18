//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// ���U���g���� [ranking.cpp]
// Author : �� �a��
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "main.h"
#include "ranking.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "fade.h"
#include "icon.h"
#include "rankTime.h"
#include "bg.h"
#include "gamepad.h"
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �}�N����`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define TIME_RANK1				(17*60*60 + 31*60 + 00)			// �����N1�̋L�^�i��*60*60 + ��*60 + �b
#define TIME_RANK2				(17*60*60 + 31*60 + 20)			// �����N2�̋L�^�i��*60*60 + ��*60 + �b
#define TIME_RANK3				(17*60*60 + 31*60 + 40)			// �����N3�̋L�^�i��*60*60 + ��*60 + �b
#define TIME_RANK4				(17*60*60 + 32*60 + 10)			// �����N4�̋L�^�i��*60*60 + ��*60 + �b
#define TIME_RANK5				(17*60*60 + 32*60 + 50)			// �����N5�̋L�^�i��*60*60 + ��*60 + �b

//#define TIME_RANK1				(01*60*60 + 23*60 + 45)			// �����N1�̋L�^�i��*60*60 + ��*60 + �b
//#define TIME_RANK2				(23*60*60 + 45*60 + 67)			// �����N2�̋L�^�i��*60*60 + ��*60 + �b
//#define TIME_RANK3				(45*60*60 + 67*60 + 89)			// �����N3�̋L�^�i��*60*60 + ��*60 + �b
//#define TIME_RANK4				(67*60*60 + 89*60 + 01)			// �����N4�̋L�^�i��*60*60 + ��*60 + �b
//#define TIME_RANK5				(89*60*60 + 01*60 + 23)			// �����N5�̋L�^�i��*60*60 + ��*60 + �b

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �\���̒�`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �O���[�o���ϐ�
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
int			g_nRankingCounter;
static int	g_nRanking[MAX_RANKING] = { TIME_RANK1,TIME_RANK2,TIME_RANK3,TIME_RANK4,TIME_RANK5 };	// �X�R�A
int			g_nTimeNow;
bool		g_bHeightTime;
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���U���g����������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void InitRanking(void)
{
	g_nRankingCounter = 0;
	// �e�평��������
	// �J�����̏���������
	InitCamera();
	// ���C�g�̏���������
	InitLight();

	// �����L���O�̎��ԏ�����
	InitRankTime();
	// �A�C�R���̏�����
	InitIcon();
	// �w�i�̏�����
	InitBG();

	SetIcon(D3DXVECTOR3(SCREEN_WIDTH/2.0f, 100.0f, 0.0f), ICON_TEX_RANKINGLOGO, 93.0f * 4.0f, 22.0f * 4.0f);


	SetIcon(D3DXVECTOR3(250.0f, 200.0f, 0.0f), ICON_TEX_RANK1, 150.0f, 150.0f);
	SetIcon(D3DXVECTOR3(250.0f, 300.0f, 0.0f), ICON_TEX_RANK2, 150.0f, 150.0f);
	SetIcon(D3DXVECTOR3(250.0f, 400.0f, 0.0f), ICON_TEX_RANK3, 150.0f, 150.0f);
	SetIcon(D3DXVECTOR3(250.0f, 500.0f, 0.0f), ICON_TEX_RANK4, 150.0f, 150.0f);
	SetIcon(D3DXVECTOR3(250.0f, 600.0f, 0.0f), ICON_TEX_RANK5, 150.0f, 150.0f);

	for (int nCntRank = 0;nCntRank < MAX_RANKING;nCntRank++)
	{
		SetRankTimeResult(nCntRank, g_nRanking[nCntRank]);
	}

	SetBg(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH, SCREEN_HEIGHT, BG_TEX_RESULT, BG_TYPE_NORMAL);

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���U���g�I������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UninitRanking(void)
{
	// �e��I������
	// �J�����̏I������
	UninitCamera();
	// ���C�g�̏I������
	UninitLight();

	// �����L���O�̏I������
	UninitRankTime();
	// �A�C�R���̏I������
	UninitIcon();
	// �w�i�̏I������
	UninitBG();

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���U���g�X�V����
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateRanking(void)
{
	g_nRankingCounter++;
	FADE pFade;
	// �t�F�[�h���[�h�̎擾
	pFade = GetFade();
	if (pFade == FADE_NONE)
	{
		// ����L�[�������ꂽ���ǂ���
		if (GetKeyboardTrigger(DIK_RETURN) == true||GetGamePadTrigger(0,BUTTON_B))
		{
			// �t�F�[�h�ݒ�
			SetFade(MODE_GAME);
		}
	}

	// �e��X�V����
	// �J�����̍X�V����
	UpdateCamera();
	// ���C�g�̍X�V����
	UpdateLight();

	// �����L���O�̍X�V����
	UpdateRankTime();
	// �A�C�R���̍X�V����
	UpdateIcon();
	// �w�i�̍X�V����
	UpdateBG();

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���U���g�`�揈��
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void DrawRanking(void)
{
	// �e��`�揈��
	// �J�����̐ݒ�
	SetCamera(0);

	// �w�i�̕`��
	DrawBG();

	// �A�C�R���̕`�揈��
	DrawIcon();
	// �����L���O�̎��ԏ�����
	DrawRankTime();

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �����L���O�̓���ւ�菈��
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SortRanking(int nTime)
{
	g_bHeightTime = false;

	int nRank;
	int aData[6];
	aData[5] = nTime;

	for (int nCntNum = 0; nCntNum < 5; nCntNum++)
	{
		aData[nCntNum] = g_nRanking[nCntNum];
	}
	for (int nCntNum = 1; nCntNum < 6; nCntNum++)
	{
		for (int nCntNum1 = nCntNum; nCntNum1 > 0; nCntNum1--)
		{
			if (aData[nCntNum1 - 1] >= aData[nCntNum1])
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
	g_bHeightTime = true;
	g_nTimeNow = nTime;

}
