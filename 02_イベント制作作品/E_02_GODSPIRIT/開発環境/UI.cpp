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
#include "tutorial.h"
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
UIInfo g_aUIInfo[] =
{
	// ����
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,-25,0.0f),D3DXCOLOR(0.0f,0.0f,0.0f,1.0f),UITEX_BG,SCREEN_WIDTH,50 ,  D3DXVECTOR3(SCREEN_WIDTH / 2,25,0.0f) },
	{ D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT+25,0.0f),D3DXCOLOR(0.0f,0.0f,0.0f,1.0f),UITEX_BG1,SCREEN_WIDTH,50 ,  D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT-25,0.0f) },

	// �Q�[��CLEAROVER
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,0.0f),UITEX_RESULT,SCREEN_WIDTH,SCREEN_HEIGHT,D3DXVECTOR3(0.0f,0.0f,0.0f) },// �p�[�Z���g
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,0.0f),UITEX_GAMECLEAR,600,600,D3DXVECTOR3(0.0f,0.0f,0.0f) },// �p�[�Z���g
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,0.0f),UITEX_GAMEOVER,500,500,D3DXVECTOR3(0.0f,0.0f,0.0f) },// �p�[�Z���g

	// ���UI
	{ D3DXVECTOR3(180,550,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),UITEX_BOTTON,225,225,D3DXVECTOR3(0.0f,0.0f,0.0f) },// �p���`
	
	{ D3DXVECTOR3(280,550,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),UITEX_LANCE,100,100,D3DXVECTOR3(0.0f,0.0f,0.0f) },// �p���`
	{ D3DXVECTOR3(180,450,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),UITEX_SORD,100,100,D3DXVECTOR3(0.0f,0.0f,0.0f) },// �L�b�N
	{ D3DXVECTOR3(80,550,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),UITEX_HAMMER,100,100,D3DXVECTOR3(0.0f,0.0f,0.0f) },// �X�s��
	{ D3DXVECTOR3( 180,650,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),UITEX_PUNCH,100,100,D3DXVECTOR3(0.0f,0.0f,0.0f) },// �W�����v

};
// �e�N�X�`����
UITexture  g_aUITex[UITEX_MAX] =
{
	{ "data/TEXTURE/ui.png" },			// ������
	{ "data/TEXTURE/ui2.png" },			// ������
	{ "data/TEXTURE/UI.jpg" },			// ��
	{ "data/TEXTURE/GameClearUI.png" },		// �Q�[���N���A
	{ "data/TEXTURE/GameOverUI.png" },		// �Q�[���I�[�o�[
	{ "data/TEXTURE/botann.png" },	// �{�^��
	{ "data/TEXTURE/yari.png" },	// �p���`
	{ "data/TEXTURE/ken.png" },	// �L�b�N
	{ "data/TEXTURE/hanma.png" },	// �X�s��
	{ "data/TEXTURE/hand.png" },	// �W�����v

};
bool g_bResultUI[2];				// �I��������ǂ���
int g_bSoundUICnt;					// �T�E���h�p�J�E���g
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
	g_bResultUI[0] = false;	// ���U���g�Ȃ������ǂ���
	g_bResultUI[1] = false;	// ���U���g�Ȃ������ǂ���
	g_bSoundUICnt = 0;
	for (int nCnt = 0; nCnt < sizeof g_aUIInfo / sizeof(UIInfo); nCnt++)
	{
		SetUI(g_aUIInfo[nCnt].pos , g_aUIInfo[nCnt].col, g_aUIInfo[nCnt].nType, g_aUIInfo[nCnt].fWidth, g_aUIInfo[nCnt].fHeight );
	}
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
			if (g_aUI[nCntUI].nType == UITEX_BG|| g_aUI[nCntUI].nType == UITEX_BG1)
			{// ����
				if (pPlayer->bAttention == true)
				{
					g_aUI[nCntUI].pos.y += (g_aUIInfo[nCntUI].Stoppos.y - g_aUI[nCntUI].pos.y) / 5;
				}
				else
				{
					g_aUI[nCntUI].pos.y += (g_aUIInfo[nCntUI].pos.y - g_aUI[nCntUI].pos.y) / 10;

				}
			}
	
			if (g_aUI[nCntUI].nType == UITEX_RESULT)
			{
				if (GetGameState() == GAMESTATE_END||GetGameState() == GAMESTATE_END2)
				{
					g_aUI[nCntUI].col.a += 0.005f;
					if (g_aUI[nCntUI].col.a >= 0.5f)
					{
						g_aUI[nCntUI].col.a = 0.5f;
						if (GetResultType() == RESULTTYPE_CLEAR)
						{
							g_bResultUI[0] = true;
						}
						else if (GetResultType() == RESULTTYPE_GAMEOVER)
						{
							g_bResultUI[1] = true;
						}
					}
				}
			
			}	
			if (g_aUI[nCntUI].nType == UITEX_GAMECLEAR)
			{// �Q�[���N���A
				if (g_bResultUI[0] == true)
				{
					g_bSoundUICnt++;
					g_aUI[nCntUI].col.a = 1.0f;
					g_aUI[nCntUI].fWidth += (g_aUIInfo[nCntUI].fWidth - g_aUI[nCntUI].fWidth)*0.3f;
					g_aUI[nCntUI].fHeight += (g_aUIInfo[nCntUI].fHeight - g_aUI[nCntUI].fHeight)*0.3f;
					if (g_bSoundUICnt == 1)
					{
						PlaySound(SOUND_LABEL_SE_GAMECLEARUI);
					}
				}
			}
			if (g_aUI[nCntUI].nType == UITEX_GAMEOVER)
			{// �Q�[���I�[�o�[
				if (g_bResultUI[1] == true)
				{
					g_bSoundUICnt++;
					g_aUI[nCntUI].col.a = 1.0f;
					g_aUI[nCntUI].fWidth += (g_aUIInfo[nCntUI].fWidth - g_aUI[nCntUI].fWidth)*0.3f;
					g_aUI[nCntUI].fHeight += (g_aUIInfo[nCntUI].fHeight - g_aUI[nCntUI].fHeight)*0.3f;
					if (g_bSoundUICnt == 1)
					{
						PlaySound(SOUND_LABEL_SE_GAMEOVERUI);
					}
				}
			}
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
			if (GetPlayer()->state != PLAYERSTATE_DEATH||g_aUI[nTex].nType == UITEX_RESULT || g_aUI[nTex].nType == UITEX_GAMEOVER || g_aUI[nTex].nType == UITEX_GAMECLEAR)
			{		
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_pTextureUI[g_aUI[nTex].nType]);

					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
						4 * nTex,
						2);
				
			}
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
// UI�̐ݒ�
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
			
			if (g_aUI[nCntUI].nType == UITEX_GAMECLEAR || g_aUI[nCntUI].nType == UITEX_GAMEOVER)
			{
				g_aUI[nCntUI].fWidth = 3000;		// ��
				g_aUI[nCntUI].fHeight = 3000;	// ����
			}
			break;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUI->Unlock();
}
//==============================================================================================================================
// UI�̎擾
//==============================================================================================================================
UI *GetUI(void)
{
	return &g_aUI[0];
}
