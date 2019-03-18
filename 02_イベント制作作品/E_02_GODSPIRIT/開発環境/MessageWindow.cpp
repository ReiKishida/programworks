////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Massage���� [Massage.cpp]
// Author : RyotaKinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MessageWindow.h"
#include "input.h"
#include "sound.h"
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// �u���b�N�̒��S���W
	D3DXCOLOR  col;				// �F
	MESSAGETEX nType;				// ���
	float fWidth;				// ��
	float fHeight;				// ����
} MessageWindowInfo;
typedef struct
{
	char *pFileName;
}MessageWindowTexture;
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_MESSAGEWNDOW		(10)
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexMessageWindow(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureMessageWindow[MESSAGETEX_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMessageWindow = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
MessageWindow						g_aMessageWindow[MAX_MESSAGEWNDOW];

// �e�N�X�`����
MessageWindowTexture  g_aMessageWindowTex[MESSAGETEX_MAX] =
{
	{ "data/TEXTURE/Massage000.png" },			// �ŏ�
	{ "data/TEXTURE/Massage001.png" },			// �ŏ�
	{ "data/TEXTURE/Massage002.png" },			// �ŏ�
	{ "data/TEXTURE/Massage003.png" },			// �򂪏オ����
	{ "data/TEXTURE/Massage004.png" },			// �K�����߂łƂ�
	{ "data/TEXTURE/Massage005.png" },			// �����Ȃ�
};

//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitMessageWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nTex = 0; nTex < MESSAGETEX_MAX; nTex++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
			g_aMessageWindowTex[nTex].pFileName,		// �t�@�C���̖��O
			&g_pTextureMessageWindow[nTex]);	// �e�N�X�`���ւ̃|�C���^
	}
	// ������
	for (int nCnt = 0; nCnt < MAX_MESSAGEWNDOW; nCnt++)
	{
		g_aMessageWindow[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���S
		g_aMessageWindow[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
		g_aMessageWindow[nCnt].nType = MESSAGETEX_TUTORIAL;						// ���
		g_aMessageWindow[nCnt].nState = MESSAGESTATE_NONE;
		g_aMessageWindow[nCnt].fWidth = 0;									// ��
		g_aMessageWindow[nCnt].fHeight = 0;									// ����
		g_aMessageWindow[nCnt].bUse = false;								// �g�p����Ă��邩�ǂ���
	}
	// ���_���̍쐬
	MakeVertexMessageWindow(pDevice);
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitMessageWindow(void)
{
	// �e�N�X�`���̊J��
	for (int nTex = 0; nTex < MESSAGETEX_MAX; nTex++)
	{
		if (g_pTextureMessageWindow[nTex] != NULL)
		{
			g_pTextureMessageWindow[nTex]->Release();
			g_pTextureMessageWindow[nTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffMessageWindow != NULL)
	{
		g_pVtxBuffMessageWindow->Release();
		g_pVtxBuffMessageWindow = NULL;
	}
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateMessageWindow(void)
{
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
						// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMessageWindow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_MESSAGEWNDOW; nCnt++, pVtx += 4)
	{
		if (g_aMessageWindow[nCnt].bUse == true)
		{
			
			g_aMessageWindow[nCnt].fWidth += (g_aMessageWindow[nCnt].fDiffWidth - g_aMessageWindow[nCnt].fWidth) * 0.5f;
			if (g_aMessageWindow[nCnt].nState == MESSAGESTATE_SET)
			{
				if (g_aMessageWindow[nCnt].fWidth >= g_aMessageWindow[nCnt].fDiffWidth - 10)
				{
					g_aMessageWindow[nCnt].fHeight += (g_aMessageWindow[nCnt].fDiffHeight - g_aMessageWindow[nCnt].fHeight) * 0.1f;
				}
			}
			if (g_aMessageWindow[nCnt].nState == MESSAGESTATE_DELETE)
			{
				if (g_aMessageWindow[nCnt].fWidth <= g_aMessageWindow[nCnt].fDiffWidth + 1)
				{
					g_aMessageWindow[nCnt].fHeight += (g_aMessageWindow[nCnt].fDiffHeight - g_aMessageWindow[nCnt].fHeight) * 0.1f;
					if (g_aMessageWindow[nCnt].fHeight <= g_aMessageWindow[nCnt].fHeight + 1)
					{
						g_aMessageWindow[nCnt].bUse = false;
					}
				}
			}
			// ���_����ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aMessageWindow[nCnt].fWidth / 2, -g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aMessageWindow[nCnt].fWidth / 2, -g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aMessageWindow[nCnt].fWidth / 2, g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aMessageWindow[nCnt].fWidth / 2, g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aMessageWindow[nCnt].col;
			pVtx[1].col = g_aMessageWindow[nCnt].col;
			pVtx[2].col = g_aMessageWindow[nCnt].col;
			pVtx[3].col = g_aMessageWindow[nCnt].col;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMessageWindow->Unlock();
}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawMessageWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMessageWindow, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nTex = 0; nTex < MAX_MESSAGEWNDOW; nTex++)
	{
		if (g_aMessageWindow[nTex].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMessageWindow[g_aMessageWindow[nTex].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nTex,
				2);
		}
	}
}

//==============================================================================================================================
// ���_���̍쐬
//==============================================================================================================================
void MakeVertexMessageWindow(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_MESSAGEWNDOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMessageWindow,
		NULL);

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMessageWindow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_MESSAGEWNDOW; nCnt++, pVtx += 4)
	{
		// ���_����ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aMessageWindow[nCnt].fWidth / 2, -g_aMessageWindow[nCnt].fHeight/2, 0.0f) + g_aMessageWindow[nCnt].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aMessageWindow[nCnt].fWidth / 2, -g_aMessageWindow[nCnt].fHeight/2, 0.0f) + g_aMessageWindow[nCnt].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aMessageWindow[nCnt].fWidth / 2, g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aMessageWindow[nCnt].fWidth / 2, g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;
		// rhw�̐ݒ�
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}
		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_aMessageWindow[nCnt].col;
		pVtx[1].col = g_aMessageWindow[nCnt].col;
		pVtx[2].col = g_aMessageWindow[nCnt].col;
		pVtx[3].col = g_aMessageWindow[nCnt].col;
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMessageWindow->Unlock();
}
//==============================================================================================================================
// �E�B���h�E�̐ݒ�
//==============================================================================================================================
void SetMessageWindow(D3DXVECTOR3 pos,float DiffWidth,float DiffHigeht,MESSAGETEX nType)
{
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMessageWindow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_MESSAGEWNDOW; nCnt++, pVtx += 4)
	{
		if (g_aMessageWindow[nCnt].bUse == false)
		{
			g_aMessageWindow[nCnt].pos = pos;				// ���S
			g_aMessageWindow[nCnt].fDiffWidth = DiffWidth;		// ��
			g_aMessageWindow[nCnt].fDiffHeight = DiffHigeht;		// ����

			g_aMessageWindow[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aMessageWindow[nCnt].nType = nType;		// ���
			g_aMessageWindow[nCnt].fWidth = 0;		// ��
			g_aMessageWindow[nCnt].fHeight = 10;	// ����
			g_aMessageWindow[nCnt].nState = MESSAGESTATE_SET;
			
				PlaySound(SOUND_LABEL_SE_TORI_MESSAGEWINDOW);
			
		
			g_aMessageWindow[nCnt].bUse = true;

			// ���_����ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aMessageWindow[nCnt].fWidth / 2, -g_aMessageWindow[nCnt].fHeight/2, 0.0f) + g_aMessageWindow[nCnt].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aMessageWindow[nCnt].fWidth / 2, -g_aMessageWindow[nCnt].fHeight/2, 0.0f) + g_aMessageWindow[nCnt].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aMessageWindow[nCnt].fWidth / 2, g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aMessageWindow[nCnt].fWidth / 2, g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aMessageWindow[nCnt].col;
			pVtx[1].col = g_aMessageWindow[nCnt].col;
			pVtx[2].col = g_aMessageWindow[nCnt].col;
			pVtx[3].col = g_aMessageWindow[nCnt].col;
			break;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMessageWindow->Unlock();
}
//==============================================================================================================================
// �E�B���h�E�̐ݒ�
//==============================================================================================================================
void DeleteMessageWndow(void)
{
	for (int nCnt = 0; nCnt < MAX_MESSAGEWNDOW; nCnt++)
	{
		if (g_aMessageWindow[nCnt].bUse == true)
		{
			g_aMessageWindow[nCnt].fDiffWidth = 0;		// ��
			g_aMessageWindow[nCnt].fDiffHeight = 0;	// ����
			g_aMessageWindow[nCnt].nState = MESSAGESTATE_DELETE;
		}
	}
}
//==============================================================================================================================
// ���b�Z�[�W�E�B���h�E�̊l��
//==============================================================================================================================
MessageWindow *GetMessageWindow(void)
{
	return &g_aMessageWindow[0];
}