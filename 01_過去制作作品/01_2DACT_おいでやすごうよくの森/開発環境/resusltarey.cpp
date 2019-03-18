//=============================================================================
//
// ���U���g�A���C���� [resultarey.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "resultarey.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULTIAREY_NUMBER		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexResultArey(LPDIRECT3DDEVICE9 pDevice);

#define MAX_RIGIT (3)	//����
#define NUMBER_X (30.0f) //X�̑傫��
#define NUMBER_Y (50.0f)//Y�̑傫��


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureResultArey = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultArey = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
int						g_nResultArey = 0;					// �X�R�A
int						g_aResultAreyNumber[MAX_RIGIT];	//����
D3DXVECTOR3				g_ResultAreypos;//�X�R�A���S���W






float fAreyWidth;
float fAreyHeight;

//=============================================================================
// ����������
//=============================================================================
void InitResultArey(void)
{



	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�X�R�A������
	g_nResultArey = 0;

	MODE *pMode;
	pMode = GetMode();

	g_ResultAreypos = D3DXVECTOR3(580, 400, 0.0f);
	fAreyWidth = 20.0f;
	fAreyHeight = 40.0f;


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULTIAREY_NUMBER, &g_pTextureResultArey);



	// ���_���̍쐬
	MakeVertexResultArey(pDevice);


}

//=============================================================================
// �I������
//=============================================================================
void UnInitResultArey(void)
{



	// �e�N�X�`���̊J��
	if (g_pTextureResultArey != NULL)
	{
		g_pTextureResultArey->Release();
		g_pTextureResultArey = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffResultArey != NULL)
	{
		g_pVtxBuffResultArey->Release();
		g_pVtxBuffResultArey = NULL;
	}




}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResultArey(void)
{


	FADE pFade;
	pFade = *GetFade();

	// ���_���̍쐬
	VERTEX_2D *pVtx;



	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultArey->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_RIGIT; nCntScore++)
	{
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultAreyNumber[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultAreyNumber[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultAreyNumber[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultAreyNumber[nCntScore], 1.0f);
		pVtx += 4;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultArey->Unlock();


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResultArey(void)
{

	int nCntResultArey;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultArey, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResultArey);

	for (nCntResultArey = 0; nCntResultArey < 4; nCntResultArey++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultArey * 4, 2);
	}





}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexResultArey(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;


	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultArey,
		NULL);



	// ���_����ݒ�
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultArey->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultArey = 0; nCntResultArey < MAX_RIGIT; nCntResultArey++)
	{


		pVtx[0].pos = D3DXVECTOR3(g_ResultAreypos.x - fAreyWidth, g_ResultAreypos.y - fAreyHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_ResultAreypos.x + fAreyWidth, g_ResultAreypos.y - fAreyHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ResultAreypos.x - fAreyWidth, g_ResultAreypos.y + fAreyHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ResultAreypos.x + fAreyWidth, g_ResultAreypos.y + fAreyHeight, 0.0f);


		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);


		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultAreyNumber[nCntResultArey], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultAreyNumber[nCntResultArey], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultAreyNumber[nCntResultArey], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultAreyNumber[nCntResultArey], 1.0f);



		pVtx += 4;

		g_ResultAreypos.x += 40.0f;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultArey->Unlock();
}




//=============================================================================
// �X�R�A�̃Z�b�g
//=============================================================================
void SetResultArey(int nScore)
{
	
		// ���_���̍쐬
		//		VERTEX_2D *pVtx;


		g_nResultArey += nScore;

		//g_nResultArey += g_nResultArey;



		g_aResultAreyNumber[0] = g_nResultArey % 1000 / 100;
		g_aResultAreyNumber[1] = g_nResultArey % 100 / 10;
		g_aResultAreyNumber[2] = g_nResultArey % 10;




	
}