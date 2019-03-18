//=============================================================================
//
// �A�C�e������ [Fg.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "Fg2.h"
#include "player.h"
#include "main.h"
#include "input.h"
#include "game.h"
#include "block.h"
#include "fg.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_TYPE_FG2		(8)								// �A�C�e���̎��
#define Fg_TEXTURENAME_0	"data/TEXTURE/fg003-1.png"		//�A�C�e���̃e�N�X�`��1
#define Fg_TEXTURENAME_1	"data/TEXTURE/glass002.png"		//�A�C�e���̃e�N�X�`��2
#define Fg_TEXTURENAME_2	"data/TEXTURE/back4.png"		//�A�C�e���̃e�N�X�`��3
#define Fg_TEXTURENAME_3	"data/TEXTURE/glass2525.png"		//�A�C�e���̃e�N�X�`��4
#define Fg_TEXTURENAME_4	"data/TEXTURE/woodleft.png"		//�A�C�e���̃e�N�X�`��1
#define Fg_TEXTURENAME_5	"data/TEXTURE/woodright.png"		//�A�C�e���̃e�N�X�`��2
#define Fg_TEXTURENAME_6	"data/TEXTURE/fever.png"		//�A�C�e���̃e�N�X�`��3
#define Fg_TEXTURENAME_7	"data/TEXTURE/watch.png"		//�A�C�e���̃e�N�X�`��4




//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureFg2[MAX_TYPE_FG2] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFg2 = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

FG g_aFg2[MAX_FG];	//�u���b�N

					//bool bJump;


//=============================================================================
// ����������
//=============================================================================
void InitFg2(void)
{
	LPDIRECT3DDEVICE9 pDevice;


	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �A�C�e���̏��̏�����
	for (int nCntFg2 = 0; nCntFg2 < MAX_FG; nCntFg2++)
	{
		g_aFg2[nCntFg2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFg2[nCntFg2].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFg2[nCntFg2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aFg2[nCntFg2].bUse = false;
		g_aFg2[nCntFg2].fHeight = 0;
		g_aFg2[nCntFg2].fWidth = 0;

	}




	// �e�N�X�`���̓ǂݍ���(�S�̕�)
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_0, &g_apTextureFg2[0]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_1, &g_apTextureFg2[1]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_2, &g_apTextureFg2[2]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_3, &g_apTextureFg2[3]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_4, &g_apTextureFg2[4]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_5, &g_apTextureFg2[5]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_6, &g_apTextureFg2[6]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_7, &g_apTextureFg2[7]);


	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_FG2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFg2,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFg2->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFg2 = 0; nCntFg2 < MAX_FG2; nCntFg2++)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x - g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y - g_aFg2[nCntFg2].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x + g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y - g_aFg2[nCntFg2].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x - g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x + g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y, 0.0f);

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

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFg2->Unlock();


}

//=============================================================================
// �I������
//=============================================================================
void UnInitFg2(void)
{

	int nCntTex;

	// �e�N�X�`���̊J��
	for (nCntTex = 0; nCntTex < MAX_TYPE_FG2; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureFg2[nCntTex] != NULL)
		{
			g_apTextureFg2[nCntTex]->Release();
			g_apTextureFg2[nCntTex] = NULL;
		}
	}


	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffFg2 != NULL)
	{
		g_pVtxBuffFg2->Release();
		g_pVtxBuffFg2 = NULL;
	}



}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFg2(void)
{
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFg2(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// �|���S���̕`��

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffFg2, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntFg2 = 0; nCntFg2 < MAX_FG; nCntFg2++)
	{
		if (g_aFg2[nCntFg2].bUse == true)
		{

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureFg2[g_aFg2[nCntFg2].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFg2 * 4, 2);
		}
	}
}


//=============================================================================
// �A�C�e���̐ݒ�
//=============================================================================
void SetFg2(D3DXVECTOR3 pos, int type, float fHeight, float fWidth)
{
	int nCntFg2;
	VERTEX_2D *pVtx;



	for (nCntFg2 = 0; nCntFg2 < MAX_FG2; nCntFg2++)
	{
		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffFg2->Lock(0, 0, (void**)&pVtx, 0);
		if (g_aFg2[nCntFg2].bUse == false)	//�A�C�e�����g�p����Ă��Ȃ�
		{
			pVtx += (nCntFg2 * 4);

			g_aFg2[nCntFg2].pos = pos;
			g_aFg2[nCntFg2].fHeight = fHeight;
			g_aFg2[nCntFg2].fWidth = fWidth;

			//���_�̍��W
			pVtx[0].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x - g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y - g_aFg2[nCntFg2].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x + g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y - g_aFg2[nCntFg2].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x - g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x + g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y, 0.0f);

			g_aFg2[nCntFg2].nType = type;

			g_aFg2[nCntFg2].bUse = true;	//�g�p���Ă����Ԃɂ���

			break;





			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffFg2->Unlock();
		}
	}
}



//=============================================================================
// �A�C�e���̎擾
//=============================================================================
//Fg *GetFg(void)
//{
//	return &g_aFg2[0];
//}
