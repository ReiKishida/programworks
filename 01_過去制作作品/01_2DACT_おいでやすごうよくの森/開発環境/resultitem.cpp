//=============================================================================
//
// ���U���g�A�C�e������ [resultitem.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "resultitem.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULTITEM_NUMBER		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexResultCoin(LPDIRECT3DDEVICE9 pDevice);

#define MAX_RIGIT (3)	//����
#define NUMBER_X (25.0f) //X�̑傫��
#define NUMBER_Y (45.0f)//Y�̑傫��


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureResultCoin = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultCoin = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
int						g_nResultCoin = 0;					// �X�R�A
int						g_aResultCoinNumber[MAX_RIGIT];	//����
D3DXVECTOR3				g_ResultCoinpos;//�X�R�A���S���W






float fNumberWidth;
float fNumberHeight;

//=============================================================================
// ����������
//=============================================================================
void InitResultItem(void)
{



	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�X�R�A������
	g_nResultCoin = 0;

	MODE *pMode;
	pMode = GetMode();

	g_ResultCoinpos = D3DXVECTOR3(555, 100, 0.0f);
	fNumberWidth = 20.0f;
	fNumberHeight = 40.0f;


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULTITEM_NUMBER, &g_pTextureResultCoin);



	// ���_���̍쐬
	MakeVertexResultCoin(pDevice);


}

//=============================================================================
// �I������
//=============================================================================
void UnInitResultItem(void)
{



	// �e�N�X�`���̊J��
	if (g_pTextureResultCoin != NULL)
	{
		g_pTextureResultCoin->Release();
		g_pTextureResultCoin = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffResultCoin != NULL)
	{
		g_pVtxBuffResultCoin->Release();
		g_pVtxBuffResultCoin = NULL;
	}




}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResultItem(void)
{


	FADE pFade;
	pFade = *GetFade();

	// ���_���̍쐬
	VERTEX_2D *pVtx;



	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultCoin->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_RIGIT; nCntScore++)
	{
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultCoinNumber[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultCoinNumber[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultCoinNumber[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultCoinNumber[nCntScore], 1.0f);
		pVtx += 4;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultCoin->Unlock();


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResultItem(void)
{

	int nCntResultItem;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultCoin, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResultCoin);

	for (nCntResultItem = 0; nCntResultItem < MAX_RIGIT; nCntResultItem++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultItem * 4, 2);
	}





}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexResultCoin(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;


	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultCoin,
		NULL);



	// ���_����ݒ�
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultCoin->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultItem = 0; nCntResultItem < MAX_RIGIT; nCntResultItem++)
	{


		pVtx[0].pos = D3DXVECTOR3(g_ResultCoinpos.x - fNumberWidth, g_ResultCoinpos.y - fNumberHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_ResultCoinpos.x + fNumberWidth, g_ResultCoinpos.y - fNumberHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ResultCoinpos.x - fNumberWidth, g_ResultCoinpos.y + fNumberHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ResultCoinpos.x + fNumberWidth, g_ResultCoinpos.y + fNumberHeight, 0.0f);


		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);


		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultCoinNumber[nCntResultItem], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultCoinNumber[nCntResultItem], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultCoinNumber[nCntResultItem], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultCoinNumber[nCntResultItem], 1.0f);



		pVtx += 4;

		g_ResultCoinpos.x += 40.0f;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultCoin->Unlock();
}




//=============================================================================
// �X�R�A�̃Z�b�g
//=============================================================================
void SetResultCoin(int nScore)
{
	
		// ���_���̍쐬
		//		VERTEX_2D *pVtx;


		g_nResultCoin += nScore;

		//g_nResultCoin += g_nResultCoin;


		
		g_aResultCoinNumber[0] = g_nResultCoin % 1000 / 100;
		g_aResultCoinNumber[1] = g_nResultCoin % 100 / 10;
		g_aResultCoinNumber[2] = g_nResultCoin % 10;




	
}