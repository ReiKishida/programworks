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
#define	TEXTURE_HIGHSCORE		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexHighScore01(LPDIRECT3DDEVICE9 pDevice);

#define MAX_HIGHSCORE (8)	//����
#define HIGHSCORE_X (20.0f) //X�̑傫��
#define HIGHSCORE_Y (50.0f)//Y�̑傫��

//#define HIGH_X (150.0f) //X�̑傫��
//#define HIGH_Y (60.0f)//Y�̑傫��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureHighScore = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHighScore = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
int						g_nHighScore = 0;					// �X�R�A
int						g_aHighScoreNumber[MAX_HIGHSCORE];	//����
D3DXVECTOR3				g_HighScorepos;//�X�R�A���S���W

D3DXCOLOR				g_HighScoreColor0;
D3DXCOLOR				g_HighScoreColor1;
D3DXCOLOR				g_HighScoreColor2;
D3DXCOLOR				g_HighScoreColor3;


float fWidth;
float fHeight;

int nCntHigh;	//�n�C�X�R�A�_��

//=============================================================================
// ����������
//=============================================================================
void InitHighScore(void)
{

	 
	
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�X�R�A������
	g_nHighScore = 0;
	nCntHigh = 0;

	g_HighScoreColor0 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_HighScoreColor1 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_HighScoreColor2 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_HighScoreColor3 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	MODE *pMode;
	pMode = GetMode();
	
		g_HighScorepos = D3DXVECTOR3(820, SCREEN_HEIGHT - 150, 0.0f);
		fWidth = 20.0f;
		fHeight = 50.0f;
	

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_HIGHSCORE, &g_pTextureHighScore);



	// ���_���̍쐬
	MakeVertexHighScore01(pDevice);


}

//=============================================================================
// �I������
//=============================================================================
void UnInitHighScore(void)
{
	


	// �e�N�X�`���̊J��
	if (g_pTextureHighScore != NULL)
	{
		g_pTextureHighScore->Release();
		g_pTextureHighScore = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffHighScore != NULL)
	{
		g_pVtxBuffHighScore->Release();
		g_pVtxBuffHighScore = NULL;
	}




}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateHighScore(void)
{
	

	FADE pFade;
	pFade = *GetFade();

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	nCntHigh++;
	nCntHigh = nCntHigh % 30;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHighScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRanking = 0; nCntRanking < MAX_HIGHSCORE; nCntRanking++)
	{
		
			if (nCntHigh < 15)
			{
				g_HighScoreColor0 = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f);
				g_HighScoreColor1 = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f);
				g_HighScoreColor2 = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f);
				g_HighScoreColor3 = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f);
			}
			else if (nCntHigh >= 15)
			{
				g_HighScoreColor0 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				g_HighScoreColor1 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				g_HighScoreColor2 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				g_HighScoreColor3 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			}
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_HighScoreColor0;
			pVtx[1].col = g_HighScoreColor1;
			pVtx[2].col = g_HighScoreColor2;
			pVtx[3].col = g_HighScoreColor3;

			pVtx += 4;
	}
		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffHighScore->Unlock();

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffHighScore->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntScore = 0; nCntScore < MAX_HIGHSCORE; nCntScore++)
	{
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aHighScoreNumber[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aHighScoreNumber[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aHighScoreNumber[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aHighScoreNumber[nCntScore], 1.0f);
		pVtx += 4;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHighScore->Unlock();


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHighScore(void)
{
	
	int nCntHigh;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffHighScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureHighScore);

	for (nCntHigh = 0; nCntHigh < MAX_HIGHSCORE; nCntHigh++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntHigh * 4, 2);
	}





}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexHighScore01(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;


	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_HIGHSCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHighScore,
		NULL);



	// ���_����ݒ�
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHighScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHigh = 0; nCntHigh < MAX_HIGHSCORE; nCntHigh++)
	{


		pVtx[0].pos = D3DXVECTOR3(g_HighScorepos.x - fWidth, g_HighScorepos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_HighScorepos.x + fWidth, g_HighScorepos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_HighScorepos.x - fWidth, g_HighScorepos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_HighScorepos.x + fWidth, g_HighScorepos.y + fHeight, 0.0f);


		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aHighScoreNumber[nCntHigh], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aHighScoreNumber[nCntHigh], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aHighScoreNumber[nCntHigh], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aHighScoreNumber[nCntHigh], 1.0f);



		pVtx += 4;

		g_HighScorepos.x += 45;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHighScore->Unlock();
}




//=============================================================================
// �X�R�A�̃Z�b�g
//=============================================================================
void SetHighScore(int nScore)
{
	if (nScore > g_nHighScore)
	{
		// ���_���̍쐬
		//		VERTEX_2D *pVtx;


		g_nHighScore = nScore;

		//g_nHighScore += g_nHighScore;


		g_aHighScoreNumber[0] = g_nHighScore / 100000000;
		g_aHighScoreNumber[1] = g_nHighScore % 10000000 / 1000000;
		g_aHighScoreNumber[2] = g_nHighScore % 1000000 / 100000;
		g_aHighScoreNumber[3] = g_nHighScore % 100000 / 10000;
		g_aHighScoreNumber[4] = g_nHighScore % 10000 / 1000;
		g_aHighScoreNumber[5] = g_nHighScore % 1000 / 100;
		g_aHighScoreNumber[6] = g_nHighScore % 100 / 10;
		g_aHighScoreNumber[7] = g_nHighScore % 10;




	}
}