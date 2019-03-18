//=============================================================================
//
// �A�C�e������ [Fg.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "Fg.h"
#include "player.h"
#include "main.h"
#include "input.h"
#include "game.h"
#include "block.h"
#include "fg.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_TYPE_FG		(8)								// �A�C�e���̎��
#define Fg_TEXTURENAME_0	"data/TEXTURE/glass000-1.png"		//�A�C�e���̃e�N�X�`��1
#define Fg_TEXTURENAME_1	"data/TEXTURE/glass002-1.png"		//�A�C�e���̃e�N�X�`��2
#define Fg_TEXTURENAME_2	"data/TEXTURE/glass2525.png"		//�A�C�e���̃e�N�X�`��3
#define Fg_TEXTURENAME_3	"data/TEXTURE/watch.png"		//�A�C�e���̃e�N�X�`��4
#define Fg_TEXTURENAME_4	"data/TEXTURE/back5.png"		//�A�C�e���̃e�N�X�`��1
#define Fg_TEXTURENAME_5	"data/TEXTURE/vodka.png"		//�A�C�e���̃e�N�X�`��2
#define Fg_TEXTURENAME_6	"data/TEXTURE/arei2.png"		//�A�C�e���̃e�N�X�`��3
#define Fg_TEXTURENAME_7	"data/TEXTURE/watch.png"		//�A�C�e���̃e�N�X�`��4

#define Fg_X (50)
#define Fg_Y (50)

#define FgLOST (0)

#define Fg_JUMP_POWER (-9.0f)	//�W�����v��
#define Fg_GRAVITY (1.1f)		//�d��
#define PAPER_GRAVITY (0.98f)		//�d��

#define Fg_GRAVITY_MAX (25.5f)	//�d�͂̍ő��

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureFg[MAX_TYPE_FG] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFg = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

FG g_aFg[MAX_FG];	//�u���b�N

						//bool bJump;


//=============================================================================
// ����������
//=============================================================================
void InitFg(void)
{
	LPDIRECT3DDEVICE9 pDevice;


	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �A�C�e���̏��̏�����
	for (int nCntFg = 0; nCntFg < MAX_FG; nCntFg++)
	{
		g_aFg[nCntFg].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFg[nCntFg].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFg[nCntFg].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aFg[nCntFg].bUse = false;
		g_aFg[nCntFg].fHeight = 0;
		g_aFg[nCntFg].fWidth = 0;

	}




	// �e�N�X�`���̓ǂݍ���(�S�̕�)
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_0, &g_apTextureFg[0]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_1, &g_apTextureFg[1]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_2, &g_apTextureFg[2]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_3, &g_apTextureFg[3]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_4, &g_apTextureFg[4]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_5, &g_apTextureFg[5]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_6, &g_apTextureFg[6]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_7, &g_apTextureFg[7]);


	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_FG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFg,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFg = 0; nCntFg < MAX_FG; nCntFg++)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x - g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y - g_aFg[nCntFg].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x + g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y - g_aFg[nCntFg].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x - g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x + g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y, 0.0f);

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
	g_pVtxBuffFg->Unlock();


}

//=============================================================================
// �I������
//=============================================================================
void UnInitFg(void)
{

	int nCntTex;

	// �e�N�X�`���̊J��
	for (nCntTex = 0; nCntTex < MAX_TYPE_FG; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureFg[nCntTex] != NULL)
		{
			g_apTextureFg[nCntTex]->Release();
			g_apTextureFg[nCntTex] = NULL;
		}
	}


	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffFg != NULL)
	{
		g_pVtxBuffFg->Release();
		g_pVtxBuffFg = NULL;
	}



}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFg(void)
{

	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFg(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// �|���S���̕`��

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffFg, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntFg = 0; nCntFg < MAX_FG; nCntFg++)
	{
		if (g_aFg[nCntFg].bUse == true)
		{

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureFg[g_aFg[nCntFg].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFg * 4, 2);
		}
	}
}


//=============================================================================
// �A�C�e���̐ݒ�
//=============================================================================
void SetFg(D3DXVECTOR3 pos, int type, float fHeight, float fWidth)
{
	int nCntFg;
	VERTEX_2D *pVtx;



	for (nCntFg = 0; nCntFg < MAX_FG; nCntFg++)
	{
		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffFg->Lock(0, 0, (void**)&pVtx, 0);
		if (g_aFg[nCntFg].bUse == false)	//�A�C�e�����g�p����Ă��Ȃ�
		{
			pVtx += (nCntFg * 4);
			
				g_aFg[nCntFg].pos = pos;
				g_aFg[nCntFg].fHeight = fHeight;
				g_aFg[nCntFg].fWidth = fWidth;

				//���_�̍��W
				pVtx[0].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x - g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y - g_aFg[nCntFg].fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x + g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y - g_aFg[nCntFg].fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x - g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x + g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y, 0.0f);

				g_aFg[nCntFg].nType = type;

				g_aFg[nCntFg].bUse = true;	//�g�p���Ă����Ԃɂ���

				break;
			




			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffFg->Unlock();
		}
	}
}



//=============================================================================
// �A�C�e���̎擾
//=============================================================================
//Fg *GetFg(void)
//{
//	return &g_aFg[0];
//}
