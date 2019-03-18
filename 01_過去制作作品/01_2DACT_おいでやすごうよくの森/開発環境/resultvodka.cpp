//=============================================================================
//
// ���U���g�E�H�b�J���� [resultvodka.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "resultVodka.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULTIVODKA_NUMBER		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexResultVodka(LPDIRECT3DDEVICE9 pDevice);

#define MAX_RIGIT (3)	//����
#define NUMBER_X (30.0f) //X�̑傫��
#define NUMBER_Y (50.0f)//Y�̑傫��


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureResultVodka = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultVodka = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
int						g_nResultVodka = 0;					// �X�R�A
int						g_aResultVodkaNumber[MAX_RIGIT];	//����
D3DXVECTOR3				g_ResultVodkapos;//�X�R�A���S���W






float fVodkaWidth;
float fVodkaHeight;

//=============================================================================
// ����������
//=============================================================================
void InitResultVodka(void)
{



	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�X�R�A������
	g_nResultVodka = 0;

	MODE *pMode;
	pMode = GetMode();

	g_ResultVodkapos = D3DXVECTOR3(555, 200, 0.0f);
	fVodkaWidth = 20.0f;
	fVodkaHeight = 40.0f;


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULTIVODKA_NUMBER, &g_pTextureResultVodka);



	// ���_���̍쐬
	MakeVertexResultVodka(pDevice);


}

//=============================================================================
// �I������
//=============================================================================
void UnInitResultVodka(void)
{



	// �e�N�X�`���̊J��
	if (g_pTextureResultVodka != NULL)
	{
		g_pTextureResultVodka->Release();
		g_pTextureResultVodka = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffResultVodka != NULL)
	{
		g_pVtxBuffResultVodka->Release();
		g_pVtxBuffResultVodka = NULL;
	}




}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResultVodka(void)
{


	FADE pFade;
	pFade = *GetFade();

	// ���_���̍쐬
	VERTEX_2D *pVtx;



	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultVodka->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_RIGIT; nCntScore++)
	{
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultVodkaNumber[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultVodkaNumber[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultVodkaNumber[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultVodkaNumber[nCntScore], 1.0f);
		pVtx += 4;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultVodka->Unlock();


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResultVodka(void)
{

	int nCntResultVodka;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultVodka, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResultVodka);

	for (nCntResultVodka = 0; nCntResultVodka < MAX_RIGIT; nCntResultVodka++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultVodka * 4, 2);
	}





}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexResultVodka(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;


	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultVodka,
		NULL);



	// ���_����ݒ�
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultVodka->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultVodka = 0; nCntResultVodka < MAX_RIGIT; nCntResultVodka++)
	{


		pVtx[0].pos = D3DXVECTOR3(g_ResultVodkapos.x - fVodkaWidth, g_ResultVodkapos.y - fVodkaHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_ResultVodkapos.x + fVodkaWidth, g_ResultVodkapos.y - fVodkaHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ResultVodkapos.x - fVodkaWidth, g_ResultVodkapos.y + fVodkaHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ResultVodkapos.x + fVodkaWidth, g_ResultVodkapos.y + fVodkaHeight, 0.0f);


		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);


		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultVodkaNumber[nCntResultVodka], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultVodkaNumber[nCntResultVodka], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultVodkaNumber[nCntResultVodka], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultVodkaNumber[nCntResultVodka], 1.0f);



		pVtx += 4;

		g_ResultVodkapos.x += 40.0f;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultVodka->Unlock();
}




//=============================================================================
// �X�R�A�̃Z�b�g
//=============================================================================
void SetResultVodka(int nScore)
{

	// ���_���̍쐬
	//		VERTEX_2D *pVtx;


	g_nResultVodka += nScore;

	//g_nResultVodka += g_nResultVodka;



	g_aResultVodkaNumber[0] = g_nResultVodka % 1000 / 100;
	g_aResultVodkaNumber[1] = g_nResultVodka % 100 / 10;
	g_aResultVodkaNumber[2] = g_nResultVodka % 10;





}