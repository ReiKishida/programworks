//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// �A�C�R������ [icon.cpp]
// Author : �� �a��
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "icon.h"
#include "main.h"
#include "player.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"
#include "game.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �}�N����`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MAX_ICON			(64)							// �A�C�R���̍ő吔

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �\���̒�`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
typedef struct
{
	char *pFilename;	// �t�@�C����
} IconTexInfo;

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �v���g�^�C�v�錾
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
void MakeVertexIcon(LPDIRECT3DDEVICE9 pDevice);

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �O���[�o���ϐ��錾
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
LPDIRECT3DTEXTURE9		g_pTextureIcon[ICON_TEX_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffIcon = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
Icon					g_aIcon[MAX_ICON];					// �A�C�R���̏��

// �ǂݍ��ރe�N�X�`�����̐ݒ�
IconTexInfo g_apTextureIconInfo[ICON_TEX_MAX] =
{
	{ "data//TEXTURE//rizaruto_001.png" },			// ����̋A���
	{ "data//TEXTURE//rank1.png" },			// rank1
	{ "data//TEXTURE//rank2.png" },			// rank2
	{ "data//TEXTURE//rank3.png" },			// rank3
	{ "data//TEXTURE//rank4.png" },			// rank4
	{ "data//TEXTURE//rank5.png" },			// rank5sokkoukitakuou
	{ "data//TEXTURE//sokkoukitakuou.png" },// rank5
};

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ����������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void InitIcon(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �A�C�R���̏��̏�����
	for(int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
	{
		g_aIcon[nCntIcon].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aIcon[nCntIcon].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aIcon[nCntIcon].nTexType = ICON_TEX_GOGOME_TIME;
		g_aIcon[nCntIcon].fWidth = 0.0f;
		g_aIcon[nCntIcon].fHeight = 0.0f;
		g_aIcon[nCntIcon].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	for (int nCntIconTex = 0; nCntIconTex < ICON_TEX_MAX; nCntIconTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_apTextureIconInfo[nCntIconTex].pFilename,
			&g_pTextureIcon[nCntIconTex]);
	}

	// ���_���̍쐬
	MakeVertexIcon(pDevice);

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �I������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UninitIcon(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < ICON_TEX_MAX; nCntTex++)
	{
		// �e�N�X�`���̊J��
		if (g_pTextureIcon[nCntTex] != NULL)
		{
			g_pTextureIcon[nCntTex]->Release();
			g_pTextureIcon[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffIcon != NULL)
	{
		g_pVtxBuffIcon->Release();
		g_pVtxBuffIcon = NULL;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �X�V����
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateIcon()
{

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �`�揈��
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void DrawIcon(void)
{
	int nCntIcon;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffIcon, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �A�C�R���̕`��
	for(nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureIcon[g_aIcon[nCntIcon].nTexType]);
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntIcon * 4, 2);
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���_���̍쐬
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void MakeVertexIcon(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	VERTEX_2D *pVtx;

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ICON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffIcon,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++, pVtx += 4)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffIcon->Unlock();
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �A�C�R���̐ݒ�
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetIcon(D3DXVECTOR3 pos, int Textype, float fWidth, float fHeight)
{
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^
							//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++, pVtx += 4)
	{
		if (g_aIcon[nCntIcon].bUse == false)
		{ // �A�C�R�����g�p����Ă��Ȃ��ꍇ
		  // ���_�ʒu�̐ݒ�
			g_aIcon[nCntIcon].pos = pos;
			g_aIcon[nCntIcon].fWidth = fWidth;
			g_aIcon[nCntIcon].fHeight = fHeight;

			// �A�C�R���̎�ނ̐ݒ�
			g_aIcon[nCntIcon].nTexType = Textype;

			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aIcon[nCntIcon].pos.x - g_aIcon[nCntIcon].fWidth / 2, g_aIcon[nCntIcon].pos.y - g_aIcon[nCntIcon].fHeight / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aIcon[nCntIcon].pos.x + g_aIcon[nCntIcon].fWidth / 2, g_aIcon[nCntIcon].pos.y - g_aIcon[nCntIcon].fHeight / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aIcon[nCntIcon].pos.x - g_aIcon[nCntIcon].fWidth / 2, g_aIcon[nCntIcon].pos.y + g_aIcon[nCntIcon].fHeight / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aIcon[nCntIcon].pos.x + g_aIcon[nCntIcon].fWidth / 2, g_aIcon[nCntIcon].pos.y + g_aIcon[nCntIcon].fHeight / 2, 0.0f);

			g_aIcon[nCntIcon].bUse = true;	// �g�p�����Ԃɂ���
			break;	// ����break�͂��Ȃ���
		}
		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffIcon->Unlock();
	}
}

