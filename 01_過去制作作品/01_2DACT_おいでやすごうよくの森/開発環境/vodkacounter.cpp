//=============================================================================
//
// �A�C�e���J�E���^�[���� [itemcounter.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "itemcounter.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexVodkaCounter(LPDIRECT3DDEVICE9 pDevice);

#define MAX_ITEMCOUNTER (3)	//�A�C�e����
#define MAX_SCORE (3)	//����
#define SCORE_X (12.0f)
#define SCORE_Y (27.0f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureVodkaCounter = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffVodkaCounter = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
int						g_nVodkaCounter;					// �X�R�A
int						g_aVodkaNumber[MAX_SCORE];
D3DXVECTOR3				g_aVodkaCounterpos;//�X�R�A���S���W

//=============================================================================
// ����������
//=============================================================================
void InitVodkaCounter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�X�R�A������
	
		g_nVodkaCounter= 0;
	


	g_aVodkaCounterpos = D3DXVECTOR3(320, 40, 0.0f);
	

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_SCORE,		// �t�@�C���̖��O
		&g_pTextureVodkaCounter);	// �e�N�X�`���ւ̃|�C���^

								// ���_���̍쐬
	MakeVertexVodkaCounter(pDevice);


}
//=============================================================================
// �I������
//=============================================================================
void UninitVodkaCounter(void)
{

	// �e�N�X�`���̊J��
	if (g_pTextureVodkaCounter != NULL)
	{
		g_pTextureVodkaCounter->Release();
		g_pTextureVodkaCounter = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffVodkaCounter != NULL)
	{
		g_pVtxBuffVodkaCounter->Release();
		g_pVtxBuffVodkaCounter = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateVodkaCounter(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawVodkaCounter(void)
{
	int nCntCounter;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffVodkaCounter, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureVodkaCounter);

	for (nCntCounter = 0; nCntCounter < (MAX_SCORE * MAX_ITEMCOUNTER); nCntCounter++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCounter * 4, 2);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexVodkaCounter(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;


	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_SCORE * MAX_ITEMCOUNTER),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffVodkaCounter,
		NULL);



	// ���_����ݒ�
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffVodkaCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCounter = 0; nCntCounter < MAX_SCORE; nCntCounter++)
	{

		pVtx[0].pos = D3DXVECTOR3(g_aVodkaCounterpos.x - SCORE_X, g_aVodkaCounterpos.y - SCORE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aVodkaCounterpos.x + SCORE_X, g_aVodkaCounterpos.y - SCORE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aVodkaCounterpos.x - SCORE_X, g_aVodkaCounterpos.y + SCORE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aVodkaCounterpos.x + SCORE_X, g_aVodkaCounterpos.y + SCORE_Y, 0.0f);


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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);


		pVtx += 4;

		g_aVodkaCounterpos.x -= 30;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffVodkaCounter->Unlock();
}


//=============================================================================
// �X�R�A�̉��Z
//=============================================================================
void AddVodkaCounter(int nValue)
{

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	
		g_nVodkaCounter += nValue;
	

	//g_nVodkaCounter += g_nVodkaCounter;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffVodkaCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCounter = 0; nCntCounter < MAX_SCORE; nCntCounter++)
	{
		g_aVodkaNumber[0] = g_nVodkaCounter % 10;
		g_aVodkaNumber[1] = g_nVodkaCounter % 100 / 10;
		g_aVodkaNumber[2] = g_nVodkaCounter % 1000 / 100;


		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aVodkaNumber[nCntCounter], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aVodkaNumber[nCntCounter], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aVodkaNumber[nCntCounter], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aVodkaNumber[nCntCounter], 1.0f);


		pVtx += 4;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffVodkaCounter->Unlock();

}

//=============================================================================
// �X�R�A�̎擾
//=============================================================================
int GetVodkaCounter(void)
{
	return g_nVodkaCounter;
}