//=============================================================================
//
// ���ˈʒu���� [shoot.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "character.h"
#include "game.h"
#include "score.h"
#include "sound.h"
#include "player.h"
#include "main.h"
#include "item.h"
#include "block.h"

#include <stdlib.h> 
#include <time.h>   

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_TYPE_SHOOT		(1)								// ���ˈʒu�̎��
#define BOSS_TEXTURENAME_0	"data/TEXTURE/human.png"		//���ˈʒu�̂̃e�N�X�`��1


#define CHARACTER_X (50)
#define CHARACTER_Y (80)



#define SHOOT_POWER (-20.0f)	//�W�����v��

#define C_SHOOT_POWER (-5.0f)	//�W�����v��
#define C_GRAVITY (1.2f)		//�d��

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureCharacter = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCharacter = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

CHARACTER g_aCharacter;



//int g_nNumBoss;	//�G�̑���
//=============================================================================
// ����������
//=============================================================================
void InitCharacter(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//int nCntBoss;	//�G�̃J�E���g

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �G�̏��̏�����

	g_aCharacter.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aCharacter.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_aCharacter.type = SHOOTTYPE_COIN;
	g_aCharacter.bUse = false;
	//g_aCharacter.direction = SHOOTDIRECTION_0;

	

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BOSS_TEXTURENAME_0, &g_apTextureCharacter);
	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCharacter,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCharacter->Lock(0, 0, (void**)&pVtx, 0);


	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(g_aCharacter.pos.x - CHARACTER_X, g_aCharacter.pos.y - CHARACTER_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aCharacter.pos.x + CHARACTER_X, g_aCharacter.pos.y - CHARACTER_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aCharacter.pos.x - CHARACTER_X, g_aCharacter.pos.y , 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aCharacter.pos.x + CHARACTER_X, g_aCharacter.pos.y , 0.0f);

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
	g_pVtxBuffCharacter->Unlock();

	//g_nNumBoss = 0; //�G�̑����̃N���A

}

//=============================================================================
// �I������
//=============================================================================
void UnInitCharacter(void)
{
	int nCntTex;

	// �e�N�X�`���̊J��
	for (nCntTex = 0; nCntTex < MAX_TYPE_SHOOT; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureCharacter != NULL)
		{
			g_apTextureCharacter->Release();
			g_apTextureCharacter = NULL;
		}
	}


	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffCharacter != NULL)
	{
		g_pVtxBuffCharacter->Release();
		g_pVtxBuffCharacter = NULL;
	}



	

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCharacter(void)
{

	// ���_���̍쐬
		VERTEX_2D *pVtx;

	D3DXVECTOR3 Shootpos;

	Shootpos.x = g_aCharacter.pos.x + CHARACTER_X + 10;
	Shootpos.y = g_aCharacter.pos.y - 5;


	D3DXVECTOR3 Shootmove;

	Shootmove.x = (rand() % 4) + 1.0f;
	Shootmove.y = SHOOT_POWER;




	g_aCharacter.move.x += -4.0f;

	if (g_aCharacter.bUse == true)
	{



		if (rand() % 80 == 0)
		{

			SetItem(Shootpos, ITEMTYPE_COIN, Shootmove);
		}

	}




	g_aCharacter.move.y += C_GRAVITY;
	

	g_aCharacter.move.x += (0.0f - g_aCharacter.move.x) * 0.35f;
	/*g_Player.move.y += (0.0f - g_Player.move.y) * 0.2f;*/



	g_aCharacter.pos.x += g_aCharacter.move.x;
	g_aCharacter.pos.y += g_aCharacter.move.y;




	/*if (g_aCharacter.pos.x >= 1310)
	{
		g_aCharacter.bUse = false;
	}

	if (g_aCharacter.pos.x <= -20)
	{
		g_aCharacter.bUse = false;
	}*/
	if (g_aCharacter.pos.x >= 1310)
	{
		g_aCharacter.pos = D3DXVECTOR3(10.0, g_aCharacter.pos.y, 0.0);
	}

	if (g_aCharacter.pos.x <= -10)
	{
		g_aCharacter.pos = D3DXVECTOR3(1280.0, g_aCharacter.pos.y, 0.0);
	}


	CollisionBlock(&g_aCharacter.pos, &g_aCharacter.posold, &g_aCharacter.move, g_aCharacter.fWidth, g_aCharacter.fHeight);
	


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCharacter->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(g_aCharacter.pos.x - CHARACTER_X, g_aCharacter.pos.y - CHARACTER_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aCharacter.pos.x + CHARACTER_X, g_aCharacter.pos.y - CHARACTER_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aCharacter.pos.x - CHARACTER_X, g_aCharacter.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aCharacter.pos.x + CHARACTER_X, g_aCharacter.pos.y, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCharacter->Unlock();
}
	//=============================================================================
	// �`�揈��
	//=============================================================================
	void DrawCharacter(void)
	{
		LPDIRECT3DDEVICE9 pDevice;
		// �|���S���̕`��


		if (g_aCharacter.bUse == true)
		{

			// �f�o�C�X�̎擾
			pDevice = GetDevice();

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffCharacter, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);



			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureCharacter);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}



//=============================================================================
// �G�̐ݒ�
//=============================================================================
void SetCharacter(D3DXVECTOR3 pos, int nType)
{
	//	int nCntBoss;
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffCharacter->Lock(0, 0, (void**)&pVtx, 0);


	if (g_aCharacter.bUse == false)	//�G���g�p����Ă��Ȃ�
	{
		g_aCharacter.pos = pos;

		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aCharacter.pos.x - CHARACTER_X, g_aCharacter.pos.y - CHARACTER_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aCharacter.pos.x + CHARACTER_X, g_aCharacter.pos.y - CHARACTER_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aCharacter.pos.x - CHARACTER_X, g_aCharacter.pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aCharacter.pos.x + CHARACTER_X, g_aCharacter.pos.y, 0.0f);


		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


		g_aCharacter.nType = nType;

		g_aCharacter.bUse = true;	//�g�p���Ă����Ԃɂ���


	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffCharacter->Unlock();
}

//=============================================================================
// �G�̎擾
//=============================================================================
CHARACTER *GetCharacter(void)
{
	return &g_aCharacter;
}
