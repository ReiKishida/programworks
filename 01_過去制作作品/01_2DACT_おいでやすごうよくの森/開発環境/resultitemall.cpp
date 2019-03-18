//=============================================================================
//
// ���U���g���̃A�C�e���X�R�A���� [ResultItemAll.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "resultitemall.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "bg.h"
#include "block.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULTITEMALL		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
//#define	TEXTURE_RESULTITEMALL_CHARACTER	"data/TEXTURE/ResultItemAll.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexResultItemAll(LPDIRECT3DDEVICE9 pDevice);

#define MAX_TEXTURE_TYPE (1) //�e�N�X�`����
#define ResultItemAll_DIGIT (8)	//����
#define MAX_RESULTITEMALL (4)	//���U���g���̃A�C�e���X�R�A��
#define CHANGE_ResultItemAll (6)//����ւ��p
#define MAX_NUMER (40) // �����̑���


#define RESULTITEMALL_X (45.0f) //�����̑傫��_X
#define RESULTITEMALL_Y (65.0f)//�����̑傫��_Y
//#define RANK_CHARACTER_X	(280)
//#define RANK_CHARACTER_Y	(250)


#define RESULTITEMALL_POS0 ((D3DXVECTOR3(SCREEN_WIDTH/2 + RESULTITEMALL_X * 4 + 280.0f, 100, 0.0f))) //���U���g���̃R�C���X�R�A�̈ʒu
#define RESULTITEMALL_POS1 ((D3DXVECTOR3(SCREEN_WIDTH/2 + RESULTITEMALL_X * 4 + 280.0f, 100, 0.0f))) //���U���g���̃E�H�b�J�X�R�A�̈ʒu
#define RESULTITEMALL_POS2 ((D3DXVECTOR3(SCREEN_WIDTH/2 + RESULTITEMALL_X * 4 + 280.0f, 100, 0.0f))) //���U���g���̂����X�R�A�̈ʒu

#define RESULTITEMALL_POS3 ((D3DXVECTOR3(SCREEN_WIDTH/2 + RESULTITEMALL_X * 4 + 335.0f, 120, 0.0f))) //���U���g���̃A���C�X�R�A�̈ʒu



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureResultItemAll = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultItemAll = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
static int				g_nResultItemAll[MAX_RESULTITEMALL] = { 0, 0, 0, 0 };					// �X�R�A
//int						g_aResultItemAllNumber[ResultItemAll_DIGIT];



//=============================================================================
// ����������
//=============================================================================
void InitResultItemAll(void)
{
	

	for (int nCntBlock = 0; nCntBlock < 15; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(nCntBlock * 100.0f, 690, 0), 100.0f, 50.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f);
	}



	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// �e�N�X�`���̓ǂݍ���(����)
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULTITEMALL, &g_pTextureResultItemAll);
	// �e�N�X�`���̓ǂݍ���(���U���g���̃A�C�e���X�R�A���W)

	// ���_���̍쐬
	MakeVertexResultItemAll(pDevice);




}

//=============================================================================
// �I������
//=============================================================================
void UnInitResultItemAll(void)
{
	

	// �e�N�X�`���̊J��
	if (g_pTextureResultItemAll != NULL)
	{
		g_pTextureResultItemAll->Release();
		g_pTextureResultItemAll = NULL;
	}
	


	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffResultItemAll != NULL)
	{
		g_pVtxBuffResultItemAll->Release();
		g_pVtxBuffResultItemAll = NULL;
	}





}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResultItemAll(void)
{
	

	// ���_���̍쐬
	VERTEX_2D *pVtx;
	FADE pFade;
	pFade = *GetFade();

	// ���_���̍쐬
	//VERTEX_2D *pVtx;

	

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultItemAll->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntResultItemAll = 0; nCntResultItemAll < MAX_RESULTITEMALL; nCntResultItemAll++)
	{
		for (int nCntDigit = 0; nCntDigit < 8; nCntDigit++, pVtx += 4)
		{

			int nData = 1;

			for (int nCntData = nCntDigit; nCntData != 0; nCntData--)
			{
				nData *= 10;
			}

			pVtx[0].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
		}
	}





	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultItemAll->Lock(0, 0, (void**)&pVtx, 0);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResultItemAll(void)
{
	


	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultItemAll, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);





	//=============================================================//
	//					���U���g���̃A�C�e���X�R�A����			   //
	//=============================================================//
	// �e�N�X�`���̐ݒ�

	for (int nCntResultItemAll = 0; nCntResultItemAll < (8 * MAX_RESULTITEMALL); nCntResultItemAll++)
	{
		pDevice->SetTexture(0, g_pTextureResultItemAll);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntResultItemAll, 2);

	}




}



//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexResultItemAll(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 8 * MAX_RESULTITEMALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultItemAll,
		NULL);



	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultItemAll->Lock(0, 0, (void**)&pVtx, 0);

	//=============================================================================//
	// ���U���g���̃A�C�e���X�R�A�̃X�R�A(�S�A���C�܂�)						       //
	//=============================================================================//
	for (int nCntResultItemAll = 0; nCntResultItemAll < 4; nCntResultItemAll++)
	{

		for (int nCntDigit = 0; nCntDigit < 8; nCntDigit++, pVtx += 4)
		{
			if (nCntResultItemAll == 0)
			{
				pVtx[0].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS0;
				pVtx[1].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS0;
				pVtx[2].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS0;
				pVtx[3].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS0;
			}
			if (nCntResultItemAll == 1)
			{
				pVtx[0].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS1;
				pVtx[1].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS1;
				pVtx[2].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS1;
				pVtx[3].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS1;
			}

			if (nCntResultItemAll == 2)
			{
				pVtx[0].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS2;
				pVtx[1].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS2;
				pVtx[2].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS2;
				pVtx[3].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS2;
			}if (nCntResultItemAll == 3)
			{
				pVtx[0].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS3;
				pVtx[1].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS3;
				pVtx[2].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS3;
				pVtx[3].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS3;
			}

			//1.0f�ŌŒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

		
			if (nCntResultItemAll == 0)
			{//�R�C���̃X�R�A�J���[
				//���_�J���[
				pVtx[0].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);
			}
			else if (nCntResultItemAll == 1)
			{//�E�H�b�J�̃X�R�A�J���[
				//���_�J���[
				pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			}
			else if (nCntResultItemAll == 2)
			{//�D�̃X�R�A�J���[
			 //���_�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			}
			else
			{//�A���C�̃X�R�A�J���[
				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
			

			int nData = 1;

			for (int nCntData = nCntDigit; nCntData != 0; nCntData--)
			{
				nData *= 10;
			}

			pVtx[0].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);


		}

	}

	
}


//=============================================================================
// �X�R�A�̃Z�b�g
//=============================================================================
void SetResultItemAll(int nNumCoin, int nNumVodka , int nNumSatsu, int nNumArey)
{
	g_nResultItemAll[0] = 2500 * nNumCoin;
	g_nResultItemAll[1] = 5000 * nNumVodka;
	g_nResultItemAll[2] = 10000 * nNumSatsu;
	g_nResultItemAll[3] = 1000 * nNumArey;

}
