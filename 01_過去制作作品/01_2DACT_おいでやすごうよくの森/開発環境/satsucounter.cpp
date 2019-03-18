//=============================================================================
//
// �D�E���^�[���� [satsucounter.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "satsucounter.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexSatsuCounter(LPDIRECT3DDEVICE9 pDevice);

#define MAX_SATSUCOUNTER (3)	//�A�C�e����
#define MAX_SASTU (3)	//����
#define SCORE_X (12.0f)
#define SCORE_Y (27.0f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureSatsuCounter = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSatsuCounter = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
int						g_nSatsuCounter;					// �X�R�A
int						g_aSatsuNumber[MAX_SASTU];
D3DXVECTOR3				g_aSatsuCounterpos;//�X�R�A���S���W

//=============================================================================
// ����������
//=============================================================================
void InitSatsuCounter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�X�R�A������
	
		g_nSatsuCounter = 0;
	



	g_aSatsuCounterpos = D3DXVECTOR3(520, 40, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_SCORE,		// �t�@�C���̖��O
		&g_pTextureSatsuCounter);	// �e�N�X�`���ւ̃|�C���^

									// ���_���̍쐬
	MakeVertexSatsuCounter(pDevice);


}
//=============================================================================
// �I������
//=============================================================================
void UninitSatsuCounter(void)
{

	// �e�N�X�`���̊J��
	if (g_pTextureSatsuCounter != NULL)
	{
		g_pTextureSatsuCounter->Release();
		g_pTextureSatsuCounter = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffSatsuCounter != NULL)
	{
		g_pVtxBuffSatsuCounter->Release();
		g_pVtxBuffSatsuCounter = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSatsuCounter(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSatsuCounter(void)
{
	int nCntSatsu;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffSatsuCounter, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureSatsuCounter);

	for (nCntSatsu = 0; nCntSatsu < (MAX_SASTU * MAX_SATSUCOUNTER); nCntSatsu++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSatsu * 4, 2);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexSatsuCounter(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;


	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_SASTU * MAX_SATSUCOUNTER),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSatsuCounter,
		NULL);



	// ���_����ݒ�
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSatsuCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSatsu = 0; nCntSatsu < MAX_SASTU; nCntSatsu++)
	{

		pVtx[0].pos = D3DXVECTOR3(g_aSatsuCounterpos.x - SCORE_X, g_aSatsuCounterpos.y - SCORE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aSatsuCounterpos.x + SCORE_X, g_aSatsuCounterpos.y - SCORE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aSatsuCounterpos.x - SCORE_X, g_aSatsuCounterpos.y + SCORE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aSatsuCounterpos.x + SCORE_X, g_aSatsuCounterpos.y + SCORE_Y, 0.0f);


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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);


		pVtx += 4;

		g_aSatsuCounterpos.x -= 30;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSatsuCounter->Unlock();
}


//=============================================================================
// �X�R�A�̉��Z
//=============================================================================
void AddSatsuCounter(int nValue)
{

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	
		g_nSatsuCounter += nValue;
	

	//g_nSatsuCounter += g_nSatsuCounter;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSatsuCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSatsu = 0; nCntSatsu < MAX_SASTU; nCntSatsu++)
	{
		g_aSatsuNumber[0] = g_nSatsuCounter % 10;
		g_aSatsuNumber[1] = g_nSatsuCounter % 100 / 10;
		g_aSatsuNumber[2] = g_nSatsuCounter % 1000 / 100;


		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aSatsuNumber[nCntSatsu], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aSatsuNumber[nCntSatsu], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aSatsuNumber[nCntSatsu], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aSatsuNumber[nCntSatsu], 1.0f);


		pVtx += 4;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSatsuCounter->Unlock();

}

//=============================================================================
// �X�R�A�̎擾
//=============================================================================
int GetSatsuCounter(void)
{
	return g_nSatsuCounter;
}