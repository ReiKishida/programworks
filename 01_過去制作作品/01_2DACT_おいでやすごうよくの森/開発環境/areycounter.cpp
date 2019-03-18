//=============================================================================
//
// �A���C�J�E���^�[���� [areycounter.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "areycounter.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexAreyCounter(LPDIRECT3DDEVICE9 pDevice);

#define MAX_AREYCOUNTER (3)	//�A�C�e����
#define MAX_AREY (3)	//����
#define SCORE_X (12.0f)
#define SCORE_Y (27.0f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureAreyCounter = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAreyCounter = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
int						g_nAreyCounter;					// �X�R�A
int						g_aAreyNumber[MAX_AREY];
D3DXVECTOR3				g_AreyCounterpos;//�X�R�A���S���W

//=============================================================================
// ����������
//=============================================================================
void InitAreyCounter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�X�R�A������
	
		g_nAreyCounter = 0;
	


	g_AreyCounterpos = D3DXVECTOR3(720, 40, 0.0f);


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_SCORE,		// �t�@�C���̖��O
		&g_pTextureAreyCounter);	// �e�N�X�`���ւ̃|�C���^

									// ���_���̍쐬
	MakeVertexAreyCounter(pDevice);


}
//=============================================================================
// �I������
//=============================================================================
void UninitAreyCounter(void)
{

	// �e�N�X�`���̊J��
	if (g_pTextureAreyCounter != NULL)
	{
		g_pTextureAreyCounter->Release();
		g_pTextureAreyCounter = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffAreyCounter != NULL)
	{
		g_pVtxBuffAreyCounter->Release();
		g_pVtxBuffAreyCounter = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateAreyCounter(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawAreyCounter(void)
{
	int nCntArey;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffAreyCounter, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureAreyCounter);

	for (nCntArey = 0; nCntArey < (MAX_AREY * MAX_AREYCOUNTER); nCntArey++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntArey * 4, 2);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexAreyCounter(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;


	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_AREY * MAX_AREYCOUNTER),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAreyCounter,
		NULL);



	// ���_����ݒ�
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAreyCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntArey = 0; nCntArey < MAX_AREY; nCntArey++)
	{

		pVtx[0].pos = D3DXVECTOR3(g_AreyCounterpos.x - SCORE_X, g_AreyCounterpos.y - SCORE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_AreyCounterpos.x + SCORE_X, g_AreyCounterpos.y - SCORE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_AreyCounterpos.x - SCORE_X, g_AreyCounterpos.y + SCORE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_AreyCounterpos.x + SCORE_X, g_AreyCounterpos.y + SCORE_Y, 0.0f);


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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);


		pVtx += 4;

		g_AreyCounterpos.x -= 30;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAreyCounter->Unlock();
}


//=============================================================================
// �X�R�A�̉��Z
//=============================================================================
void AddAreyCounter(int nValue)
{

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	
		g_nAreyCounter += nValue;
	

	//g_nAreyCounter += g_nAreyCounter;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAreyCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntArey = 0; nCntArey < MAX_AREY; nCntArey++)
	{
		g_aAreyNumber[0] = g_nAreyCounter% 10;
		g_aAreyNumber[1] = g_nAreyCounter % 100 / 10;
		g_aAreyNumber[2] = g_nAreyCounter% 1000 / 100;


		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aAreyNumber[nCntArey], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aAreyNumber[nCntArey], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aAreyNumber[nCntArey], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aAreyNumber[nCntArey], 1.0f);


		pVtx += 4;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAreyCounter->Unlock();

}

//=============================================================================
// �X�R�A�̎擾
//=============================================================================
int GetAreyCounter(void)
{
	return g_nAreyCounter;
}