//=============================================================================
//
// �����L���O���� [HighScore.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "highscore.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
//#include "bg.h"
//#include "block.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_GAME_HIGHSCORE		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_GAME_HIGHSCOREWAKU		"data/TEXTURE/highs.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexGameHighScore(LPDIRECT3DDEVICE9 pDevice);
void MakeVertexGameHighScoreWaku(LPDIRECT3DDEVICE9 pDevice); //�n�C�X�R�A�̘g

#define MAX_GAMEHIGHSCORE (8)	//����
#define GAMEHIGHSCORE_X (30.0f) //X�̑傫��
#define GAMEHIGHSCORE_Y (60.0f)//Y�̑傫��

#define GAMEHIGHSCOREWAKU_X (150.0f) //X�̑傫��
#define GAMEHIGHSCOREWAKU_Y (60.0f)//Y�̑傫��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureGameHighScore = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameHighScore = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
int						g_nGameHighScore;					// �X�R�A
int						g_aGameHighScoreNumber[MAX_GAMEHIGHSCORE];	//����
D3DXVECTOR3				g_GameHighScorepos;//�X�R�A���S���W


LPDIRECT3DTEXTURE9		g_pTextureGameHighScoreWaku = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameHighScoreWaku = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3				g_GameHighScoreWakupos;//�X�R�A���S���W


float fGameWidth;
float fGameHeight;

bool HighScore;

//=============================================================================
// ����������
//=============================================================================
void InitGameHighScore(void)
{



	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�X�R�A������
	if (HighScore == false)
	{
		g_nGameHighScore = 600000;
	}

	MODE *pMode;
	pMode = GetMode();

	g_GameHighScorepos = D3DXVECTOR3(980,82, 0.0f);
	g_GameHighScoreWakupos = D3DXVECTOR3(1080, 65, 0.0f);

	fGameWidth = 15.0f;
	fGameHeight = 30.0f;

	//HighScore = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME_HIGHSCORE, &g_pTextureGameHighScore);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME_HIGHSCOREWAKU, &g_pTextureGameHighScoreWaku);



	// ���_���̍쐬
	MakeVertexGameHighScore(pDevice);

	MakeVertexGameHighScoreWaku(pDevice);

	//SetGameHighScore(500000);
}

//=============================================================================
// �I������
//=============================================================================
void UnInitGameHighScore(void)
{



	// �e�N�X�`���̊J��
	if (g_pTextureGameHighScore != NULL)
	{
		g_pTextureGameHighScore->Release();
		g_pTextureGameHighScore = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffGameHighScore != NULL)
	{
		g_pVtxBuffGameHighScore->Release();
		g_pVtxBuffGameHighScore = NULL;
	}


	// �e�N�X�`���̊J��
	if (g_pTextureGameHighScoreWaku != NULL)
	{
		g_pTextureGameHighScoreWaku->Release();
		g_pTextureGameHighScoreWaku = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffGameHighScoreWaku != NULL)
	{
		g_pVtxBuffGameHighScoreWaku->Release();
		g_pVtxBuffGameHighScoreWaku = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGameHighScore(void)
{


	FADE pFade;
	pFade = *GetFade();

	// ���_���̍쐬
	VERTEX_2D *pVtx;


	g_aGameHighScoreNumber[0] = g_nGameHighScore / 100000000;
	g_aGameHighScoreNumber[1] = g_nGameHighScore % 10000000 / 1000000;
	g_aGameHighScoreNumber[2] = g_nGameHighScore % 1000000 / 100000;
	g_aGameHighScoreNumber[3] = g_nGameHighScore % 100000 / 10000;
	g_aGameHighScoreNumber[4] = g_nGameHighScore % 10000 / 1000;
	g_aGameHighScoreNumber[5] = g_nGameHighScore % 1000 / 100;
	g_aGameHighScoreNumber[6] = g_nGameHighScore % 100 / 10;
	g_aGameHighScoreNumber[7] = g_nGameHighScore % 10;


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameHighScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_GAMEHIGHSCORE; nCntScore++)
	{
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aGameHighScoreNumber[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aGameHighScoreNumber[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aGameHighScoreNumber[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aGameHighScoreNumber[nCntScore], 1.0f);
		pVtx += 4;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGameHighScore->Unlock();


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGameHighScore(void)
{

	int nCntGameHighScore;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGameHighScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureGameHighScore);

	for (nCntGameHighScore = 0; nCntGameHighScore < MAX_GAMEHIGHSCORE; nCntGameHighScore++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGameHighScore * 4, 2);
	}






	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGameHighScoreWaku, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureGameHighScoreWaku);

	
	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexGameHighScore(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;


	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GAMEHIGHSCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameHighScore,
		NULL);



	// ���_����ݒ�
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameHighScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGameHighScore = 0; nCntGameHighScore < MAX_GAMEHIGHSCORE; nCntGameHighScore++)
	{


		pVtx[0].pos = D3DXVECTOR3(g_GameHighScorepos.x - fGameWidth, g_GameHighScorepos.y - fGameHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_GameHighScorepos.x + fGameWidth, g_GameHighScorepos.y - fGameHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_GameHighScorepos.x - fGameWidth, g_GameHighScorepos.y + fGameHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_GameHighScorepos.x + fGameWidth, g_GameHighScorepos.y + fGameHeight, 0.0f);


		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);


		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aGameHighScoreNumber[nCntGameHighScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aGameHighScoreNumber[nCntGameHighScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aGameHighScoreNumber[nCntGameHighScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aGameHighScoreNumber[nCntGameHighScore], 1.0f);



		pVtx += 4;

		g_GameHighScorepos.x += 30;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGameHighScore->Unlock();
}


//=============================================================================
// ���_���̍쐬(�n�C�X�R�A�g)
//=============================================================================
void MakeVertexGameHighScoreWaku(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;


	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameHighScoreWaku,
		NULL);



	// ���_����ݒ�
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameHighScoreWaku->Lock(0, 0, (void**)&pVtx, 0);

	


		pVtx[0].pos = D3DXVECTOR3(g_GameHighScoreWakupos.x - GAMEHIGHSCOREWAKU_X, g_GameHighScoreWakupos.y - GAMEHIGHSCOREWAKU_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_GameHighScoreWakupos.x + GAMEHIGHSCOREWAKU_X, g_GameHighScoreWakupos.y - GAMEHIGHSCOREWAKU_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_GameHighScoreWakupos.x - GAMEHIGHSCOREWAKU_X, g_GameHighScoreWakupos.y + GAMEHIGHSCOREWAKU_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_GameHighScoreWakupos.x + GAMEHIGHSCOREWAKU_X, g_GameHighScoreWakupos.y + GAMEHIGHSCOREWAKU_Y, 0.0f);


		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);



	

	
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGameHighScoreWaku->Unlock();
}




//=============================================================================
// �X�R�A�̃Z�b�g
//=============================================================================
void SetGameHighScore(int nScore)
{
	if (nScore >= g_nGameHighScore)
	{
		// ���_���̍쐬
		//		VERTEX_2D *pVtx;


		g_nGameHighScore = nScore;

		//g_nGameHighScore += g_nGameHighScore;


		g_aGameHighScoreNumber[0] = g_nGameHighScore / 100000000;
		g_aGameHighScoreNumber[1] = g_nGameHighScore % 10000000 / 1000000;
		g_aGameHighScoreNumber[2] = g_nGameHighScore % 1000000 / 100000;
		g_aGameHighScoreNumber[3] = g_nGameHighScore % 100000 / 10000;
		g_aGameHighScoreNumber[4] = g_nGameHighScore % 10000 / 1000;
		g_aGameHighScoreNumber[5] = g_nGameHighScore % 1000 / 100;
		g_aGameHighScoreNumber[6] = g_nGameHighScore % 100 / 10;
		g_aGameHighScoreNumber[7] = g_nGameHighScore % 10;

		HighScore = true;


	}
}