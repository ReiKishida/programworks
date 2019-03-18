//=============================================================================
//
// ���ˈʒu���� [shoot.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "shoot.h"
#include "game.h"
#include "score.h"
#include "sound.h"
#include "player.h"
#include "main.h"
#include "item.h"
#include "timer.h"

#include <stdlib.h> 
#include <time.h>   

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_TYPE_SHOOT		(1)								// ���ˈʒu�̎��
#define BOSS_TEXTURENAME_0	"data/TEXTURE/zoku.png"		//���ˈʒu�̂̃e�N�X�`��1


#define SHOOT_X (100)
#define SHOOT_Y (100)



#define SHOOT_POWER (-20.5f)	//�W�����v��


//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureShoot = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShoot = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

SHOOT g_aShoot;

int nDirection;


//int g_nNumBoss;	//�V���[�^�[�̑���
//=============================================================================
// ����������
//=============================================================================
void InitShoot(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//int nCntBoss;	//�V���[�^�[�̃J�E���g

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �V���[�^�[�̏��̏�����

	g_aShoot.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aShoot.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aShoot.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_aShoot.type = SHOOTTYPE_COIN;
	g_aShoot.bUse = false;
	g_aShoot.direction = SHOOTDIRECTION_0;
	g_aShoot.fHeight = 110.0f;
	g_aShoot.fWidth = 110.0f;
	//g_aShoot.fLength = 0.0f;

	nDirection = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BOSS_TEXTURENAME_0, &g_apTextureShoot);
	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShoot,
		NULL);


	g_aShoot.fLength = sqrtf((g_aShoot.fWidth / 2+ g_aShoot.fWidth / 2) * (g_aShoot.fHeight / 2 + g_aShoot.fHeight / 2));
	g_aShoot.fAngle =atan2f(g_aShoot.fWidth /2, g_aShoot.fHeight / 2);
	//g_aShoot.fAngle = D3DX_PI / 4;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShoot->Lock(0, 0, (void**)&pVtx, 0);


	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(-D3DX_PI + g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
							  g_aShoot.pos.y + cosf(-D3DX_PI + g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


	pVtx[1].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(D3DX_PI - g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
							  g_aShoot.pos.y + cosf(D3DX_PI - g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


	pVtx[2].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(-g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
							  g_aShoot.pos.y + cosf(-g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


	pVtx[3].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
							  g_aShoot.pos.y + cosf(g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);



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



	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShoot->Unlock();

	//g_nNumBoss = 0; //�V���[�^�[�̑����̃N���A

}

//=============================================================================
// �I������
//=============================================================================
void UnInitShoot(void)
{
	int nCntTex;

	// �e�N�X�`���̊J��
	for (nCntTex = 0; nCntTex < MAX_TYPE_SHOOT; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureShoot != NULL)
		{
			g_apTextureShoot->Release();
			g_apTextureShoot = NULL;
		}
	}


	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffShoot != NULL)
	{
		g_pVtxBuffShoot->Release();
		g_pVtxBuffShoot = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateShoot(void)
{

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	D3DXVECTOR3 Shootpos;
	D3DXVECTOR3 Shootmove;

	PLAYER pPlayer;
	pPlayer = GetPlayer();

	TIMER pTimer;
	pTimer = GetTimer();

	float Scroll_0;	//���(-)
	float Scroll_1;	//����(+)

	Scroll_0 = 35.0f;
	Scroll_1 = 35.0f;


	PAUSECHECK2 pScroll;
	pScroll = GetPauseCheck2();

	//g_aShoot.rot.x += 0.01f;
	//g_aShoot.rot.y += 0.01f;

	if (rand() % 50 == 0)
	{
		g_aShoot.direction = SHOOTDIRECTION_0;
	}

	if (rand() % 50 == 0)
	{
		g_aShoot.direction = SHOOTDIRECTION_1;

	}

	switch (g_aShoot.direction)
	{
	case SHOOTDIRECTION_0:
		Shootpos.x = g_aShoot.pos.x + SHOOT_X ;
		Shootpos.y = g_aShoot.pos.y - 10;
		Shootmove.x = (rand() % 11 - + 0.01f) ;
		Shootmove.y = SHOOT_POWER;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffShoot->Lock(0, 0, (void**)&pVtx, 0);
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffShoot->Unlock();


		if (pTimer.nTime <= 10)
		{
			if (g_aShoot.bUse == true)
			{
				if (rand() % 10000 == 0)
				{

					//SetItem(Shootpos, ITEMTYPE_AREY, D3DXVECTOR3(Shootmove.x + 5.0f, Shootmove.y, 0.0f));


				}

				if (rand() % 2 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_COIN, D3DXVECTOR3(Shootmove.x + 3.0f, Shootmove.y, 0.0f));

				}

				if (rand() % 15 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_VODKA, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
				if (rand() % 20 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_PAPER, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
			}
		}
		else
		{
			if (g_aShoot.bUse == true)
			{
				if (rand() % 60 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_AREY, D3DXVECTOR3(Shootmove.x + 5.0f, Shootmove.y, 0.0f));


				}

				if (rand() % 3 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_COIN, D3DXVECTOR3(Shootmove.x + 3.0f, Shootmove.y, 0.0f));

				}

				if (rand() % 25 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_VODKA, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
				if (rand() % 150 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_PAPER, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
			}
		}
		break;

	case SHOOTDIRECTION_1:
		Shootpos.x = g_aShoot.pos.x - SHOOT_X + 5;
		Shootpos.y = g_aShoot.pos.y - 10;
		Shootmove.x = (rand() % 11 + 0.01f)  * -1;
		Shootmove.y = SHOOT_POWER;
		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffShoot->Lock(0, 0, (void**)&pVtx, 0);
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffShoot->Unlock();

		

		if (pTimer.nTime <= 10)
		{
			if (g_aShoot.bUse == true)
			{
				if (rand() % 10000 == 0)
				{

					//SetItem(Shootpos, ITEMTYPE_AREY, D3DXVECTOR3(Shootmove.x + 5.0f, Shootmove.y, 0.0f));


				}

				if (rand() % 2 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_COIN, D3DXVECTOR3(Shootmove.x + 3.0f, Shootmove.y, 0.0f));

				}

				if (rand() % 15 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_VODKA, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
				if (rand() % 20 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_PAPER, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
			}
		}
		else
		{
			if (g_aShoot.bUse == true)
			{
				if (rand() % 60 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_AREY, D3DXVECTOR3(Shootmove.x + 5.0f, Shootmove.y, 0.0f));


				}

				if (rand() % 3 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_COIN, D3DXVECTOR3(Shootmove.x + 3.0f, Shootmove.y, 0.0f));

				}

				if (rand() % 25 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_VODKA, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
				if (rand() % 150 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_PAPER, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
			}
		}
		break;
	}



	

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShoot->Lock(0, 0, (void**)&pVtx, 0);


	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(-D3DX_PI + g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
		g_aShoot.pos.y + cosf(-D3DX_PI + g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


	pVtx[1].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(D3DX_PI - g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
		g_aShoot.pos.y + cosf(D3DX_PI - g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


	pVtx[2].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(-g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
		g_aShoot.pos.y + cosf(-g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


	pVtx[3].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
		g_aShoot.pos.y + cosf(g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShoot->Unlock();



}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawShoot(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// �|���S���̕`��


	if (g_aShoot.bUse == true)
	{

		// �f�o�C�X�̎擾
		pDevice = GetDevice();

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pVtxBuffShoot, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);



		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureShoot);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}


//=============================================================================
// �V���[�^�[�̐ݒ�
//=============================================================================
void SetShoot(D3DXVECTOR3 pos)
{
	//	int nCntBoss;
	VERTEX_2D *pVtx;

	g_aShoot.fLength = sqrtf((g_aShoot.fWidth / 2 + g_aShoot.fWidth / 2) * (g_aShoot.fHeight / 2 + g_aShoot.fHeight / 2));
	g_aShoot.fAngle = atan2f(g_aShoot.fWidth / 2, g_aShoot.fHeight / 2);

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffShoot->Lock(0, 0, (void**)&pVtx, 0);


	if (g_aShoot.bUse == false)	//�V���[�^�[���g�p����Ă��Ȃ�
	{
		g_aShoot.pos = pos;

		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(-D3DX_PI + g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
								  g_aShoot.pos.y + cosf(-D3DX_PI + g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


		pVtx[1].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(D3DX_PI - g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
								  g_aShoot.pos.y + cosf(D3DX_PI - g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


		pVtx[2].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(-g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
								  g_aShoot.pos.y + cosf(-g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


		pVtx[3].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
							      g_aShoot.pos.y + cosf(g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);




		g_aShoot.bUse = true;	//�g�p���Ă����Ԃɂ���


	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffShoot->Unlock();
}

//=============================================================================
// �V���[�^�[�̎擾
//=============================================================================
SHOOT *GetShoot(void)
{
	return &g_aShoot;
}
