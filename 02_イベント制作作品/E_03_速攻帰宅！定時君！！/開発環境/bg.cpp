//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : �� �a��
//
//=============================================================================
#include "main.h"
#include "bg.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_WIDTH			(SCREEN_WIDTH)				// �w�i�̕�
#define BG_HEIGHT			(SCREEN_HEIGHT)				// �w�i�̍���
#define MAX_BG				(10)						// �w�i�̍ő吔
#define MAX_TEX_TYPE_BG		(2)							// �w�i�̃e�N�X�`���̎��

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	float fMove;				// �X�N���[����
	float fScrollCut;			// �w�i�X�N���[���̕�����
	float fScrollCutPattern;	// �X�N���[�������̈�R�}��
	float fScrollCounter;		// �X�N���[���̃J�E���^�[
	D3DXCOLOR col;				// �F
	float fWidth;				// �w�i�̕�
	float fHeight;				// �w�i�̍���
	int nTexType;				// �e�N�X�`���̎��
	BG_TYPE type;				// ���
	bool bUse;					// �g�p���Ă��邩�ǂ���
}Bg;


typedef struct
{
	char *pFilename;	// �t�@�C����
} BgTexInfo;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureBg[BG_TEX_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
Bg g_aBg[MAX_BG];										// �G�̏��
D3DXVECTOR3 g_moveBg;									// �w�i�̈ړ���
														// �ǂݍ��ރe�N�X�`�����̐ݒ�
BgTexInfo g_apTextureBgInfo[BG_TEX_MAX] =
{
	{ "data//TEXTURE//rizaruto.jpg" },			// ��
	{ "data//TEXTURE//result001.jpg" },			// ��

};
//=============================================================================
// �w�i����������
//=============================================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	// �w�i�̏��̏�����
	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		g_aBg[nCntBg].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[nCntBg].fMove = 0.0f;
		g_aBg[nCntBg].fScrollCut = 4.0f;
		g_aBg[nCntBg].fScrollCutPattern = float(1 / g_aBg[nCntBg].fScrollCut);
		g_aBg[nCntBg].fScrollCounter = 0.0f;
		g_aBg[nCntBg].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aBg[nCntBg].fWidth = 0.0f;
		g_aBg[nCntBg].fHeight = 0.0f;
		g_aBg[nCntBg].nTexType = 0;
		g_aBg[nCntBg].type = BG_TYPE_NORMAL;
		g_aBg[nCntBg].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	for (int nCntBgTex = 0; nCntBgTex < BG_TEX_MAX; nCntBgTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_apTextureBgInfo[nCntBgTex].pFilename,
			&g_apTextureBg[nCntBgTex]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BG,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffBg,
								NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++, pVtx += 4)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		pVtx[0].col = g_aBg[nCntBg].col;
		pVtx[1].col = g_aBg[nCntBg].col;
		pVtx[2].col = g_aBg[nCntBg].col;
		pVtx[3].col = g_aBg[nCntBg].col;
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg->Unlock();
}

//=============================================================================
// �w�i�I������
//=============================================================================
void UninitBG(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < BG_TEX_MAX; nCntTex++)
	{
		if (g_apTextureBg[nCntTex] != NULL)
		{
			g_apTextureBg[nCntTex]->Release();
			g_apTextureBg[nCntTex] = NULL;
		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//=============================================================================
// �w�i�X�V����
//=============================================================================
void UpdateBG(void)
{
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++, pVtx += 4)
	{
		if (g_aBg[nCntBg].bUse == true)
		{ // �w�i���g�p����Ă���
			// �ʒu�X�V
			if (g_aBg[nCntBg].type == BG_TYPE_SCROLL)
			{ // �X�N���[������w�i�̏ꍇ
				g_aBg[nCntBg].fScrollCounter += g_aBg[nCntBg].fMove;
				if (g_aBg[nCntBg].fScrollCounter > 1.0f - g_aBg[nCntBg].fScrollCutPattern)
				{
					g_aBg[nCntBg].fScrollCounter = 0.0f;
				}
			}
			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x - g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y - g_aBg[nCntBg].fHeight / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x + g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y - g_aBg[nCntBg].fHeight / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x - g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y + g_aBg[nCntBg].fHeight / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x + g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y + g_aBg[nCntBg].fHeight / 2, 0.0f);

			if (g_aBg[nCntBg].type == BG_TYPE_SCROLL)
			{
				pVtx[0].tex = D3DXVECTOR2(g_aBg[nCntBg].fScrollCounter, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aBg[nCntBg].fScrollCounter + g_aBg[nCntBg].fScrollCutPattern, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aBg[nCntBg].fScrollCounter, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aBg[nCntBg].fScrollCounter + g_aBg[nCntBg].fScrollCutPattern, 1.0f);
			}
			else
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg->Unlock();
}

//=============================================================================
// �w�i�`�揈��
//=============================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		if (g_aBg[nCntBg].bUse == true)
		{ // �G���g�p����Ă���
		  // �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureBg[g_aBg[nCntBg].nTexType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg * 4, 2);
		}
	}
}

//=============================================================================
// �w�i�̐ݒ�
//=============================================================================
void SetBg(D3DXVECTOR3 pos, float fMove, D3DXCOLOR col, float fWidth, float fHeight ,int nTexType, BG_TYPE type)
{
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++, pVtx += 4)
	{
		if (g_aBg[nCntBg].bUse == false)
		{ // �������g�p����Ă��Ȃ��ꍇ
		  // ���_�ʒu�̐ݒ�
			g_aBg[nCntBg].pos = pos;
			g_aBg[nCntBg].fMove = fMove;
			g_aBg[nCntBg].col = col;
			g_aBg[nCntBg].fWidth = fWidth;
			g_aBg[nCntBg].fHeight = fHeight;
			g_aBg[nCntBg].nTexType = nTexType;
			g_aBg[nCntBg].type = type;

			pVtx[0].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x - g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y - g_aBg[nCntBg].fHeight / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x + g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y - g_aBg[nCntBg].fHeight / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x - g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y + g_aBg[nCntBg].fHeight / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x + g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y + g_aBg[nCntBg].fHeight / 2, 0.0f);
			pVtx[0].col = g_aBg[nCntBg].col;
			pVtx[1].col = g_aBg[nCntBg].col;
			pVtx[2].col = g_aBg[nCntBg].col;
			pVtx[3].col = g_aBg[nCntBg].col;

			g_aBg[nCntBg].bUse = true;	// �g�p�����Ԃɂ���
			break;	// ����break�͂��Ȃ���
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg->Unlock();
}

