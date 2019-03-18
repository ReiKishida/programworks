//=============================================================================
//
// ����ʏ��� [world.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "world.h"
#include "fade.h"
#include "sound.h"
#include "topics.h"
#include "cursor.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_WORLD (2)

#define WORLD_TEXTURENAME	"DATA/TEXTURE/world2.png"	//�^�C�g���̃e�N�X�`����
#define WORLD_TEXTURENAME_2	"DATA/TEXTURE/press_enter.png"	//�^�C�g���̃e�N�X�`����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureWorld[MAX_WORLD] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWorld = NULL;	//���_�o�b�t�@�ւ̃|�C���^

WORLD g_aWorld[MAX_WORLD];
//=============================================================================
// �^�C�g������������
//=============================================================================
void InitWorld(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	
	InitTopics();

	InitCursor();


	SetTopics(D3DXVECTOR3(15.0f, 40.0f, 0.0f),TOPICSTYPE_MAIL_0, 0, 60.0f, 75.0f);

	SetTopics(D3DXVECTOR3(15.0f, 190.0f, 0.0f), TOPICSTYPE_MAIL_1, 0, 60.0f, 75.0f);

	SetTopics(D3DXVECTOR3(15.0f, 350.0f, 0.0f), TOPICSTYPE_NEWS_0, 1, 60.0f, 75.0f);
	SetTopics(D3DXVECTOR3(SCREEN_WIDTH /2 -45.0f, 280.0f, 0.0f), TOPICSTYPE_MISSION_0, 2, 70.0f, 50.0f);


	//�f�o�C�X�̎擾
	pDevice = GetDevice();



	g_aWorld[0].nType = 0;
	g_aWorld[1].nType = 1;

	g_aWorld[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aWorld[1].pos = D3DXVECTOR3(900, 660, 0.0f);

	g_aWorld[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aWorld[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, WORLD_TEXTURENAME, &g_pTextureWorld[0]);
	D3DXCreateTextureFromFile(pDevice, WORLD_TEXTURENAME_2, &g_pTextureWorld[1]);


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_WORLD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWorld,
		NULL);

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

					//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffWorld->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH , SCREEN_HEIGHT, 0.0f);


	//1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffWorld->Unlock();

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffWorld->Lock(0, 0, (void**)&pVtx, 0);


	//���_�̍��W
	pVtx[4].pos = D3DXVECTOR3(g_aWorld[1].pos.x - 150, g_aWorld[1].pos.y - 50, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aWorld[1].pos.x + 150, g_aWorld[1].pos.y - 50, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aWorld[1].pos.x - 150, g_aWorld[1].pos.y + 50, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aWorld[1].pos.x + 150, g_aWorld[1].pos.y + 50, 0.0f);

	//1.0f�ŌŒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//���_�J���[
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffWorld->Unlock();
}

//=============================================================================
// �^�C�g���I������
//=============================================================================
void UnInitWorld(void)
{
	
	UnInitTopics();
	UnInitCursor();


	for (int nCntWorld = 0; nCntWorld < MAX_WORLD; nCntWorld++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureWorld[nCntWorld] != NULL)
		{
			g_pTextureWorld[nCntWorld]->Release();
			g_pTextureWorld[nCntWorld] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWorld != NULL)
	{
		g_pVtxBuffWorld->Release();
		g_pVtxBuffWorld = NULL;
	}
}

//=============================================================================
// �^�C�g���X�V����
//=============================================================================
void UpdateWorld(void)
{
	
	UpdateTopics();
	UpdateCursor();

	FADE pFade;
	pFade = *GetFade();

	

	// ���_���̍쐬
	VERTEX_2D *pVtx;



	//	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffWorld->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[
	pVtx[4].col = g_aWorld[1].col;
	pVtx[5].col = g_aWorld[1].col;
	pVtx[6].col = g_aWorld[1].col;
	pVtx[7].col = g_aWorld[1].col;

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffWorld->Unlock();



	
}

//=============================================================================
// �^�C�g���`�揈��
//=============================================================================
void DrawWorld(void)
{
	

	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X���擾����
	pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffWorld, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntWorld = 0; nCntWorld < 1; nCntWorld++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureWorld[g_aWorld[nCntWorld].nType]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWorld * 4, 2);
	}

	DrawTopics();
	DrawCursor();

}
