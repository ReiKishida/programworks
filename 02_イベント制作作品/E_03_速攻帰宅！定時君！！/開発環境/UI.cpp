////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UI���� [UI.cpp]
// Author : RyotaKinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "UI.h"
#include "player.h"
#include "effect2D.h"
#include "input.h"
#include "game.h"
#include "result.h"
#include "gamepad.h"
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// �u���b�N�̒��S���W
	D3DXCOLOR  col;				// �F
	UITEX nType;				// ���
	float fWidth;				// ��
	float fHeight;				// ����
	D3DXVECTOR3 Stoppos;
} UIInfo;
typedef struct
{
	char *pFileName;
}UITexture;
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************

#define MAX_UI	(17)
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexUI(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureUI[UITEX_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
UI						g_aUI[MAX_UI];
// �e�N�X�`����
UITexture  g_aUITex[UITEX_MAX] =
{
	{ "data/TEXTURE/UI_R.png" },			// ������
	{ "data/TEXTURE/UI_L.png" },			// ������
	{ "data/TEXTURE/konma.png" },			// ������

};
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nTex = 0; nTex < UITEX_MAX; nTex++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
			g_aUITex[nTex].pFileName,		// �t�@�C���̖��O
			&g_pTextureUI[nTex]);	// �e�N�X�`���ւ̃|�C���^
	}
	// ������
	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		g_aUI[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���S
		g_aUI[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
		g_aUI[nCnt].nType = UITEX_MAX;						// ���
		g_aUI[nCnt].fWidth = 0;									// ��
		g_aUI[nCnt].fHeight = 0;									// ����
		g_aUI[nCnt].bUse = false;								// �g�p����Ă��邩�ǂ���
	}
	// ���_���̍쐬
	MakeVertexUI(pDevice);
	SetUI(D3DXVECTOR3(560, 80.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), UI_KONMA, 60, 70);
	SetUI(D3DXVECTOR3(720, 80.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), UI_KONMA, 60, 70);
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitUI(void)
{
	// �e�N�X�`���̊J��
	for (int nTex = 0; nTex < UITEX_MAX; nTex++)
	{
		if (g_pTextureUI[nTex] != NULL)
		{
			g_pTextureUI[nTex]->Release();
			g_pTextureUI[nTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffUI != NULL)
	{
		g_pVtxBuffUI->Release();
		g_pVtxBuffUI = NULL;
	}
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateUI(void)
{
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
	Player *pPlayer = GetPlayer();
	int nCntPow = 0;
	int nCntPow2 = 0;
//-------------------------------------/ �q�b�g���ɉ��������� /----------------------------------------------------------------//
	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++, pVtx += 4)
	{
		if (g_aUI[nCntUI].bUse == true)
		{

			if (g_aUI[nCntUI].nType == UITEX_R)
			{
				if (GetKeyboardTrigger(DIK_J) || GetGamePadTrigger(0, BUTTON_R1))
				{
					g_aUI[nCntUI].fWidth = g_aUI[nCntUI].fWidthInit*1.2f;
					g_aUI[nCntUI].fHeight = g_aUI[nCntUI].fHeightInit*1.2f;

				}
				g_aUI[nCntUI].fWidth += (g_aUI[nCntUI].fWidthInit - g_aUI[nCntUI].fWidth)*0.1f;
				g_aUI[nCntUI].fHeight += (g_aUI[nCntUI].fHeightInit - g_aUI[nCntUI].fHeight)*0.1f;
			}
			if (g_aUI[nCntUI].nType == UITEX_L)
			{

				if (GetKeyboardTrigger(DIK_F) || GetGamePadTrigger(0, BUTTON_L1))
				{
					g_aUI[nCntUI].fWidth = g_aUI[nCntUI].fWidthInit*1.2f;
					g_aUI[nCntUI].fHeight = g_aUI[nCntUI].fHeightInit*1.2f;
				}
				g_aUI[nCntUI].fWidth += (g_aUI[nCntUI].fWidthInit - g_aUI[nCntUI].fWidth)*0.1f;
				g_aUI[nCntUI].fHeight += (g_aUI[nCntUI].fHeightInit - g_aUI[nCntUI].fHeight)*0.1f;

			}
			// ���_����ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aUI[nCntUI].fWidth / 2, -g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].fWidth / 2, -g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aUI[nCntUI].fWidth / 2, g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].fWidth / 2, g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aUI[nCntUI].col;
			pVtx[1].col = g_aUI[nCntUI].col;
			pVtx[2].col = g_aUI[nCntUI].col;
			pVtx[3].col = g_aUI[nCntUI].col;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUI->Unlock();
}


//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nTex = 0; nTex < MAX_UI; nTex++)
	{
		if (g_aUI[nTex].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureUI[g_aUI[nTex].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nTex,
				2);
		}
	}
}

//==============================================================================================================================
// ���_���̍쐬
//==============================================================================================================================
void MakeVertexUI(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_UI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUI,
		NULL);

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_UI; nCnt++, pVtx += 4)
	{
		// ���_����ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aUI[nCnt].fWidth / 2, -g_aUI[nCnt].fHeight/2, 0.0f) + g_aUI[nCnt].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aUI[nCnt].fWidth / 2, -g_aUI[nCnt].fHeight/2, 0.0f) + g_aUI[nCnt].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aUI[nCnt].fWidth / 2, g_aUI[nCnt].fHeight / 2, 0.0f) + g_aUI[nCnt].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aUI[nCnt].fWidth / 2, g_aUI[nCnt].fHeight / 2, 0.0f) + g_aUI[nCnt].pos;
		// rhw�̐ݒ�
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}
		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_aUI[nCnt].col;
		pVtx[1].col = g_aUI[nCnt].col;
		pVtx[2].col = g_aUI[nCnt].col;
		pVtx[3].col = g_aUI[nCnt].col;
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUI->Unlock();
}
//==============================================================================================================================
// �X�R�A�̐ݒ�
//==============================================================================================================================
void SetUI(D3DXVECTOR3 pos, D3DXCOLOR col, UITEX nType, float fWidth, float fHeight)
{
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++, pVtx += 4)
	{
		if (g_aUI[nCntUI].bUse == false)
		{
			g_aUI[nCntUI].pos = pos;			// ���S
			g_aUI[nCntUI].col = col;			// �F
			g_aUI[nCntUI].nType = nType;		// ���
			g_aUI[nCntUI].fWidth = fWidth;		// ��
			g_aUI[nCntUI].fHeight = fHeight;	// ����
			g_aUI[nCntUI].fWidthInit = fWidth;		// ��
			g_aUI[nCntUI].fHeightInit = fHeight;	// ����

			g_aUI[nCntUI].bUse = true;
			// ���_����ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aUI[nCntUI].fWidth / 2, -g_aUI[nCntUI].fHeight/2, 0.0f) + g_aUI[nCntUI].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].fWidth / 2, -g_aUI[nCntUI].fHeight/2, 0.0f) + g_aUI[nCntUI].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aUI[nCntUI].fWidth / 2, g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].fWidth / 2, g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aUI[nCntUI].col;
			pVtx[1].col = g_aUI[nCntUI].col;
			pVtx[2].col = g_aUI[nCntUI].col;
			pVtx[3].col = g_aUI[nCntUI].col;

			break;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUI->Unlock();
}
void DeleteUI(UITEX nType)
{
	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		if (g_aUI[nCntUI].nType == nType)
		{
			g_aUI[nCntUI].bUse = false;
		}

	}
}