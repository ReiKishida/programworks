//=============================================================================
//
// �|�[�Y���� [Pause.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "pause.h"
#include "main.h"
#include "input.h"
#include "pausemenu.h"
#include "sound.h"
#include "item.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_POLYGON (2)
#define BG_WIDTH		(SCREEN_WIDTH)				//�w�i�̕�
#define BG_HEIGHT		(SCREEN_HEIGHT)				//�w�i�̍���
#define BG_POS_X		(0)							//�w�i����X���W
#define BG_POS_Y		(0)							//�w�i����Y���W


#define BG2_WIDTH		(250)				//�w�i�̕�
#define BG2_HEIGHT		(250)				//�w�i�̍���
#define BG2_POS_X		(0)							//�w�i����X���W
#define BG2_POS_Y		(0)							//�w�i����Y���W


#define BG3_WIDTH		(300)				//�w�i�̕�
#define BG3_HEIGHT		(300)				//�w�i�̍���
#define BG3_POS_X		(0)							//�w�i����X���W
#define BG3_POS_Y		(0)							//�w�i����Y���W



#define SHOOT_X (100)
#define SHOOT_Y (100)



#define SHOOT_POWER (-19.2f)	//�W�����v��


#define BG2_TEXTURENAME_0	"data/TEXTURE/tate000.png"		//�w�i�̃e�N�X�`��1
#define BG3_TEXTURENAME_0	"data/TEXTURE/zoku.png"		//�w�i�̃e�N�X�`��1



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexPause(LPDIRECT3DDEVICE9 pDevice);
void MakeVertexPause2(LPDIRECT3DDEVICE9 pDevice);
void MakeVertexPause3(LPDIRECT3DDEVICE9 pDevice);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePause = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
PAUSE					g_Pause;					// �t�F�[�h���

LPDIRECT3DTEXTURE9		g_pTexturePause2 = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause2 = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
PAUSE					g_Pause2;					// �t�F�[�h���


LPDIRECT3DTEXTURE9		g_pTexturePause3 = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause3 = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
PAUSE					g_Pause3;					// �t�F�[�h���


int Direction;

//=============================================================================
// ����������
//=============================================================================
void InitPause(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_Pause.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);	//������ʂɂ��Ă���

	g_Pause2.pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 30, 0.0f);	//
	g_Pause2.col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);	//������ʂɂ��Ă���


	g_Pause3.pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT / 2 + 30, 0.0f);	//


// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BG2_TEXTURENAME_0, &g_pTexturePause2);
	D3DXCreateTextureFromFile(pDevice, BG3_TEXTURENAME_0, &g_pTexturePause3);

	Direction = 0;

	// ���_���̐ݒ�
	MakeVertexPause3(pDevice);


	// ���_���̐ݒ�
	MakeVertexPause2(pDevice);

	// ���_���̐ݒ�
	MakeVertexPause(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UnInitPause(void)
{

	//�e�N�X�`���̔j��
	if (g_pTexturePause != NULL)
	{
		g_pTexturePause->Release();
		g_pTexturePause = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}




	//�e�N�X�`���̔j��
	if (g_pTexturePause2 != NULL)
	{
		g_pTexturePause2->Release();
		g_pTexturePause2 = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause2 != NULL)
	{
		g_pVtxBuffPause2->Release();
		g_pVtxBuffPause2 = NULL;
	}



	//�e�N�X�`���̔j��
	if (g_pTexturePause3 != NULL)
	{
		g_pTexturePause3->Release();
		g_pTexturePause3 = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause3 != NULL)
	{
		g_pVtxBuffPause3->Release();
		g_pVtxBuffPause3 = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePause(void)
{

	// ���_���̍쐬
	VERTEX_2D *pVtx;


	D3DXVECTOR3 Shootpos;
	D3DXVECTOR3 Shootmove;




	if (rand() % 50 == 0)
	{
		Direction = 0;
	}

	if (rand() % 50 == 0)
	{
		Direction = 1;

	}

	switch (Direction)
	{
	case 0:
		Shootpos.x = g_Pause3.pos.x + SHOOT_X + 2;
		Shootpos.y = g_Pause3.pos.y - 5;
		Shootmove.x = (rand() % 12 + 2.0f);
		Shootmove.y = SHOOT_POWER;
		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffPause3->Lock(0, 0, (void**)&pVtx, 0);
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffPause3->Unlock();
		break;

	case 1:
		Shootpos.x = g_Pause3.pos.x - SHOOT_X + 9;
		Shootpos.y = g_Pause3.pos.y - 5;
		Shootmove.x = (rand() % 12 + 1.0f)  * -1;
		Shootmove.y = SHOOT_POWER;
		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffPause3->Lock(0, 0, (void**)&pVtx, 0);
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffPause3->Unlock();
		break;
	}








}

//=============================================================================
// �^�C�g�����
//=============================================================================
void DrawPause(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePause);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);



	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPause2, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePause2);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);




	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPause3, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePause3);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexPause(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

					// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	// ���_�o�b�t�@�̏���ݒ�
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);


	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(0 ,0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH , 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0 ,BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH , BG_HEIGHT, 0.0f);

	//1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);

	////�e�N�X�`�����W
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause->Unlock();
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexPause2(LPDIRECT3DDEVICE9 pDevice)
{

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

					// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause2,
		NULL);

	// ���_�o�b�t�@�̏���ݒ�
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffPause2->Lock(0, 0, (void**)&pVtx, 0);


	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(g_Pause2.pos.x - BG3_WIDTH, g_Pause2.pos.y - BG3_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Pause2.pos.x + BG3_WIDTH, g_Pause2.pos.y - BG3_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Pause2.pos.x - BG3_WIDTH, g_Pause2.pos.y + BG3_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Pause2.pos.x + BG3_WIDTH, g_Pause2.pos.y + BG3_HEIGHT, 0.0f);

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


	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause2->Unlock();

}




//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexPause3(LPDIRECT3DDEVICE9 pDevice)
{

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

					// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause3,
		NULL);

	// ���_�o�b�t�@�̏���ݒ�
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffPause3->Lock(0, 0, (void**)&pVtx, 0);


	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(g_Pause3.pos.x - BG2_WIDTH, g_Pause3.pos.y - BG2_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Pause3.pos.x + BG2_WIDTH, g_Pause3.pos.y - BG2_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Pause3.pos.x - BG2_WIDTH, g_Pause3.pos.y + BG2_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Pause3.pos.x + BG2_WIDTH, g_Pause3.pos.y + BG2_HEIGHT, 0.0f);

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


	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause3->Unlock();

}