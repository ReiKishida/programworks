//=============================================================================
//
// ���U���g�������� [resultsatsu.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "resultsatsu.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULTISATSU_NUMBER		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexResultSatsu(LPDIRECT3DDEVICE9 pDevice);

#define MAX_RIGIT (3)	//����
#define NUMBER_X (30.0f) //X�̑傫��
#define NUMBER_Y (50.0f)//Y�̑傫��


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureResultSatsu = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultSatsu = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
int						g_nResultSatsu = 0;					// �X�R�A
int						g_aResultSatsuNumber[MAX_RIGIT];	//����
D3DXVECTOR3				g_ResultSatsupos;//�X�R�A���S���W






float fSatsuWidth;
float fSatsuHeight;

//=============================================================================
// ����������
//=============================================================================
void InitResultSatsu(void)
{



	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�X�R�A������
	g_nResultSatsu = 0;

	MODE *pMode;
	pMode = GetMode();

	g_ResultSatsupos = D3DXVECTOR3(570, 300, 0.0f);
	fSatsuWidth = 20.0f;
	fSatsuHeight = 40.0f;


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULTISATSU_NUMBER, &g_pTextureResultSatsu);



	// ���_���̍쐬
	MakeVertexResultSatsu(pDevice);


}

//=============================================================================
// �I������
//=============================================================================
void UnInitResultSatsu(void)
{



	// �e�N�X�`���̊J��
	if (g_pTextureResultSatsu != NULL)
	{
		g_pTextureResultSatsu->Release();
		g_pTextureResultSatsu = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffResultSatsu != NULL)
	{
		g_pVtxBuffResultSatsu->Release();
		g_pVtxBuffResultSatsu = NULL;
	}




}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResultSatsu(void)
{


	FADE pFade;
	pFade = *GetFade();

	// ���_���̍쐬
	VERTEX_2D *pVtx;



	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultSatsu->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_RIGIT; nCntScore++)
	{
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultSatsuNumber[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultSatsuNumber[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultSatsuNumber[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultSatsuNumber[nCntScore], 1.0f);
		pVtx += 4;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultSatsu->Unlock();


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResultSatsu(void)
{

	int nCntResultSatsu;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultSatsu, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResultSatsu);

	for (nCntResultSatsu = 0; nCntResultSatsu < MAX_RIGIT; nCntResultSatsu++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultSatsu * 4, 2);
	}





}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexResultSatsu(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;


	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultSatsu,
		NULL);



	// ���_����ݒ�
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultSatsu->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultSatsu = 0; nCntResultSatsu < MAX_RIGIT; nCntResultSatsu++)
	{


		pVtx[0].pos = D3DXVECTOR3(g_ResultSatsupos.x - fSatsuWidth, g_ResultSatsupos.y - fSatsuHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_ResultSatsupos.x + fSatsuWidth, g_ResultSatsupos.y - fSatsuHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ResultSatsupos.x - fSatsuWidth, g_ResultSatsupos.y + fSatsuHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ResultSatsupos.x + fSatsuWidth, g_ResultSatsupos.y + fSatsuHeight, 0.0f);


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
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultSatsuNumber[nCntResultSatsu], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultSatsuNumber[nCntResultSatsu], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultSatsuNumber[nCntResultSatsu], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultSatsuNumber[nCntResultSatsu], 1.0f);



		pVtx += 4;

		g_ResultSatsupos.x += 40.0f;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultSatsu->Unlock();
}




//=============================================================================
// �X�R�A�̃Z�b�g
//=============================================================================
void SetResultSatsu(int nScore)
{

	// ���_���̍쐬
	//		VERTEX_2D *pVtx;


	g_nResultSatsu += nScore;

	//g_nResultSatsu += g_nResultSatsu;



	g_aResultSatsuNumber[0] = g_nResultSatsu % 1000 / 100;
	g_aResultSatsuNumber[1] = g_nResultSatsu % 100 / 10;
	g_aResultSatsuNumber[2] = g_nResultSatsu % 10;





}