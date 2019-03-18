//=============================================================================
//
// �u���b�N���� [block.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "block.h"
#include "player.h"
#include "main.h"
#include "input.h"
#include "item.h"
#include "game.h"
#include "sound.h"
#include "pause.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_TYPE_BLOCK		(5)								// �u���b�N�̎��
#define BLOCK_TEXTURENAME_0	"data/TEXTURE/block001-1.png"		//�u���b�N�̃e�N�X�`��1
#define BLOCK_TEXTURENAME_1	"data/TEXTURE/ground4-3.png"		//�u���b�N�̃e�N�X�`��2
#define BLOCK_TEXTURENAME_2	"data/TEXTURE/block002-1.png"		//�u���b�N�̃e�N�X�`��3
#define BLOCK_TEXTURENAME_3	"data/TEXTURE/block002-2.png"		//�u���b�N�̃e�N�X�`��4
#define BLOCK_TEXTURENAME_4	"data/TEXTURE/block004.png"		//�u���b�N�̃e�N�X�`��4


#define BLOCK_X (50)
#define BLOCK_Y (50)

#define BLOCK_SHOOT_POWER (-20.0f)	//�W�����v��


//#define JUMP_POWER (-20.0f)	//�W�����v��
#define BLOCK_GRAVITY (1.5f)		//�d��
#define BLOCK_GRAVITY_MAX (25.0f)	//�d�͂̍ő��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	float fWidth;		//��
	float fHeight;		//����
	int nType;			//�^�C�v
	BLOCKSTATE state;	//���
	float fMoveX;		//�ړ���X
	float fMoveY;		//�ړ���Y
}BLOCKINFO;

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureBlock[MAX_TYPE_BLOCK] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
BLOCK g_aBlock[MAX_BLOCK];	//�u���b�N


BLOCKINFO g_aBlockInfo[] = 
{
	{D3DXVECTOR3(-50.0f, 680, 0), 1500.0f, 100.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f},
	{D3DXVECTOR3(160, 500, 0), 100, 50, 0, BLOCKSTATE_NORMAL, 0.0f, 0.0f },
	{D3DXVECTOR3(0, 350, 0), 120.0f, 50.0f, 2, BLOCKSTATE_MOVE_Y, 0.0f, -2.0f},
	{D3DXVECTOR3(1020, 500, 0), 100, 50, 0, BLOCKSTATE_NORMAL, 0.0f, 0.0f},
	{D3DXVECTOR3(1180, 350, 0), 120.0f, 50.0f, 2, BLOCKSTATE_MOVE_Y, 0.0f, 2.0f}
};

//=============================================================================
// ����������
//=============================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;	//�u���b�N�̃J�E���g

	// �f�o�C�X�̎擾
	pDevice = GetDevice();


	

	
	// �u���b�N�̏��̏�����
	for(int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);	
		g_aBlock[nCntBlock].nType = 0;
		g_aBlock[nCntBlock].bUse = false;
		g_aBlock[nCntBlock].fHeight = 0;
		g_aBlock[nCntBlock].fWidth = 0;
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].state = BLOCKSTATE_NORMAL;
		g_aBlock[nCntBlock].FallenPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	}

	

	// �e�N�X�`���̓ǂݍ���(�S�̕�)
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME_0, &g_apTextureBlock[0]);
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME_1, &g_apTextureBlock[1]);
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME_2, &g_apTextureBlock[2]);
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME_3, &g_apTextureBlock[3]);
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME_4, &g_apTextureBlock[4]);

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
									D3DUSAGE_WRITEONLY,
									FVF_VERTEX_2D,
									D3DPOOL_MANAGED,
									&g_pVtxBuffBlock,
									NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x , g_aBlock[nCntBlock].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x , g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
		
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
	g_pVtxBuffBlock->Unlock();

	/*for (int nCntBlock = 0; nCntBlock < sizeof g_aBlockInfo / sizeof (BLOCKINFO) ; nCntBlock++)
	{
	SetBlock(g_aBlock[nCntBlock].pos,
	g_aBlock[nCntBlock].fWidth,
	g_aBlock[nCntBlock].fHeight,
	g_aBlock[nCntBlock].nType, g_aBlock[nCntBlock].state,
	g_aBlock[nCntBlock].Move_X,
	g_aBlock[nCntBlock].Move_Y);
	}*/
}

//=============================================================================
// �I������
//=============================================================================
void UnInitBlock(void)
{
	
	int nCntTex;

	// �e�N�X�`���̊J��
	for(nCntTex = 0; nCntTex < MAX_TYPE_BLOCK; nCntTex++)
	{
			//�e�N�X�`���̔j��
			if (g_apTextureBlock[nCntTex] != NULL)
			{
				g_apTextureBlock[nCntTex]->Release();
				g_apTextureBlock[nCntTex] = NULL;
			}
	}


	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}

	

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBlock(void)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;

	PAUSECHECK2 pScroll;
	pScroll = GetPauseCheck2();


	for (int nCntBlock = 0; nCntBlock <= MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].posold = g_aBlock[nCntBlock].pos;
		
	}

	for (int nCntBlock = 0; nCntBlock <= MAX_BLOCK; nCntBlock++)
	{

	PLAYER pPlayer;
	pPlayer = GetPlayer();

	float Scroll_0;	//���(-)
	float Scroll_1;	//����(+)

	Scroll_0 = 35.0f;
	Scroll_1 = 35.0f ;


	
		//if (pPlayer.pos.y <=  SCREEN_HEIGHT /2  -200 && pPlayer.move.y <= -0.1f)
		//{
		//	pScroll.bPause = true;
		//	if (pScroll.bPause == true)
		//	{
		//		//pPlayer.pos.y =
		//		g_aBlock[nCntBlock].pos.y += Scroll_0;
		//		
		//	}
		//}
		/*else if (pPlayer.pos.y  >= SCREEN_HEIGHT / 2 && pPlayer.move.y >= GRAVITY_MAX )
		{
			pScroll.bPause = true;
			if (pScroll.bPause == true)
			{
				g_aBlock[nCntBlock].pos.y -= Scroll_1;
			}
		}*/

		
	//g_aBlock[nCntBlock].pos.x -= pPlayer.move.x;


	/*	if (g_aBlock[nCntBlock].pos.y >= SCREEN_HEIGHT)
		{
			g_aBlock[nCntBlock].bUse = false;
		}*/
	}


	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		switch (g_aBlock[nCntBlock].state)
		{
		case BLOCKSTATE_NORMAL:

			break;
		case BLOCKSTATE_NORMAL2:

			break;
		case BLOCKSTATE_MOVE_X:
			g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;
			if (g_aBlock[nCntBlock].pos.x >= 1010)
			{
				g_aBlock[nCntBlock].move.x *= -1;
			}
			else if (g_aBlock[nCntBlock].pos.x <= 900)
			{
				g_aBlock[nCntBlock].move.x *= -1;

			}
			break;

		case BLOCKSTATE_MOVE_Y:
			g_aBlock[nCntBlock].pos.y += g_aBlock[nCntBlock].move.y;
			if (g_aBlock[nCntBlock].pos.y >= 430)
			{
				g_aBlock[nCntBlock].move.y *= -1;
			}
			else if (g_aBlock[nCntBlock].pos.y <= 200)
			{
				g_aBlock[nCntBlock].move.y *= -1;
			}
			break;

		case BLOCKSTATE_MOVE_XY:
			g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;
			if (g_aBlock[nCntBlock].pos.x >= 220)
			{
				g_aBlock[nCntBlock].move.x *= -1;
			}
			else if (g_aBlock[nCntBlock].pos.x <= 120)
			{
				g_aBlock[nCntBlock].move.x *= -1;

			}
			break;

		case BLOCKSTATE_AREY:
			g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;
			if (rand() % 100 == 0)
			{
				//SetItem(D3DXVECTOR3(g_aBlock[nCntBlock].FallenPoint.x, g_aBlock[nCntBlock].FallenPoint.y + 90, 0.0f), ITEMTYPE_AREY);
				//SetItem(g_aBlock[nCntBlock].FallenPoint, ITEMTYPE_AREY);


			}
			break;

		case BLOCKSTATE_CRASH:
			g_aBlock[nCntBlock].move.y += BLOCK_GRAVITY;
			if (g_aBlock[nCntBlock].move.y >= BLOCK_GRAVITY_MAX)
			{
				g_aBlock[nCntBlock].move.y = BLOCK_GRAVITY_MAX;
			}


			g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;
			g_aBlock[nCntBlock].pos.y += g_aBlock[nCntBlock].move.y;

			if(g_aBlock[nCntBlock].pos.y >= SCREEN_HEIGHT)
			{
				g_aBlock[nCntBlock].bUse = false;
			}

			break;

		case BLOCKSTATE_SPRING:
			break;
		}
	}

	


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// �|���S���̕`��



	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
	
			// �f�o�C�X�̎擾
			pDevice = GetDevice();

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

	
		
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureBlock[g_aBlock[nCntBlock].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);
		}
	}
}

//=============================================================================
// �ڐG����
//=============================================================================

bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	ITEM *pItem;
	pItem = GetItem();

	

	
	bool bLand = false; //���n������

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{

			if (pPos->x >= g_aBlock[nCntBlock].pos.x - fWidth / 2 && pPos->x <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fWidth / 2
				&& pPos->y >= g_aBlock[nCntBlock].pos.y  && pPos->y <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + g_aBlock[nCntBlock].fWidth)
			{//����͈̔͂̂Ƃ�	
				if (pPosold->y <= g_aBlock[nCntBlock].posold.y + 35 &&pPos->y >= g_aBlock[nCntBlock].pos.y + 35)
				{//�ォ�痈����
					bLand = true;
					//pItem->bItemLand = true;
					if (g_aBlock[nCntBlock].state == BLOCKSTATE_MOVE_X)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y;
						pPos->x += g_aBlock[nCntBlock].move.x;
						pMove->y = 0.0f;

					}
					if (g_aBlock[nCntBlock].state == BLOCKSTATE_MOVE_XY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y;
						pPos->x += g_aBlock[nCntBlock].move.x;
						pMove->y = 0.0f;

					}
					
					if (g_aBlock[nCntBlock].state == BLOCKSTATE_SPRING)
					{
						
						pMove->y -= 60.0f;


					}
					else
					{
						pPos->y = g_aBlock[nCntBlock].pos.y + 35;
						pMove->y = 0.0f;
					}
				}
				else if (pPosold->y <= g_aBlock[nCntBlock].posold.y && pPos->y >= g_aBlock[nCntBlock].pos.y )
				{//�ォ�痈����
					bLand = true;
					if (g_aBlock[nCntBlock].state == BLOCKSTATE_MOVE_Y)
					{

						pPos->y = g_aBlock[nCntBlock].pos.y;
						//pMove->y += g_aBlock[nCntBlock].move.y;

						pMove->y = 0.0f;

						if (g_aBlock[nCntBlock].Move_Y <= 0.1f)
						{
							pPos->y = g_aBlock[nCntBlock].pos.y;
							bLand = true;
						}

					}
				}
				else if (pPosold->y - fHeight >= g_aBlock[nCntBlock].posold.y + g_aBlock[nCntBlock].fHeight && pPos->y - fHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
				{//�����痈����	

				
					

						pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fHeight;
						pMove->y = 2.0f;
					
				}
				else  if (pPosold->x + fWidth / 2 <= g_aBlock[nCntBlock].posold.x  && pPos->x + fWidth / 2  >= g_aBlock[nCntBlock].pos.x )
				{//�������痈����
					//bLand = true;
					pPos->x = g_aBlock[nCntBlock].pos.x - fWidth / 2;
					pMove->x = 0.0f;
				}

				else  if (pPosold->x - fWidth / 2 >= g_aBlock[nCntBlock].posold.x + g_aBlock[nCntBlock].fWidth && pPos->x - fWidth / 2 <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth)
				{//�E�����痈����
					//bLand = true;
					pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fWidth / 2;
					pMove->x = 0.0f;
				}
			}
		}
	}
		return bLand;
}

//=============================================================================
// �ڐG����
//=============================================================================
bool CollisionBlockItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fLength)
{
#if 1
	ITEM *pItem;
	pItem = GetItem();




	bool bLand = false; //���n������

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{

			if (pPos->x >= g_aBlock[nCntBlock].pos.x - fLength / 2 && pPos->x <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fLength / 2
				&& pPos->y  >= g_aBlock[nCntBlock].pos.y + 35 && pPos->y <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + g_aBlock[nCntBlock].fWidth)
			{//����͈̔͂̂Ƃ�	
				if (pPosold->y  + fLength /2<= g_aBlock[nCntBlock].posold.y + 35 && pPos->y + fLength / 2 >= g_aBlock[nCntBlock].pos.y + 35)
				{//�ォ�痈����
					bLand = true;

					pPos->y = g_aBlock[nCntBlock].pos.y + 35;
					pMove->y = 0.0f;


					if (g_aBlock[nCntBlock].state == BLOCKSTATE_MOVE_Y)
					{

						pPos->y = g_aBlock[nCntBlock].pos.y;
						//pMove->y += g_aBlock[nCntBlock].move.y;

						pMove->y = 0.0f;

						if (g_aBlock[nCntBlock].Move_Y <= 0.1f)
						{
							pPos->y = g_aBlock[nCntBlock].pos.y;
							bLand = true;
						}

					}
				}
				else if (pPosold->y - fLength >= g_aBlock[nCntBlock].posold.y + g_aBlock[nCntBlock].fHeight && pPos->y - fLength <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
				{//�����痈����	

					pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fLength;
					pMove->y = 2.0f;

				}
				else  if (pPosold->x + fLength <= g_aBlock[nCntBlock].posold.x  && pPos->x + fLength >= g_aBlock[nCntBlock].pos.x)
				{//�������痈����
				 //bLand = true;
					pPos->x = g_aBlock[nCntBlock].pos.x - fLength;
					pMove->x *= -1;
				}

				else  if (pPosold->x - fLength >= g_aBlock[nCntBlock].posold.x + g_aBlock[nCntBlock].fWidth && pPos->x - fLength <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth)
				{//�E�����痈����
				 //bLand = true;
					pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fLength;
					pMove->x *= -1;
				}
			}
		}
	}
	return bLand;
#endif

}


//=============================================================================
// �ڐG����
//=============================================================================
bool CollisionBlockItem2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	ITEM *pItem;
	pItem = GetItem();




	bool bLand = false; //���n������

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{

			if (pPos->x >= g_aBlock[nCntBlock].pos.x - fWidth / 2 && pPos->x <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fWidth / 2
				&& pPos->y >= g_aBlock[nCntBlock].pos.y  && pPos->y <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + g_aBlock[nCntBlock].fWidth)
			{//����͈̔͂̂Ƃ�	
				if (pPosold->y <= g_aBlock[nCntBlock].posold.y + 35 && pPos->y >= g_aBlock[nCntBlock].pos.y + 35)
				{//�ォ�痈����
					bLand = true;
					//pItem->bItemLand = true;
					if (g_aBlock[nCntBlock].state == BLOCKSTATE_MOVE_X)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y;
						pPos->x += g_aBlock[nCntBlock].move.x;
						pMove->y = 0.0f;

					}
					if (g_aBlock[nCntBlock].state == BLOCKSTATE_MOVE_XY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y;
						pPos->x += g_aBlock[nCntBlock].move.x;
						pMove->y = 0.0f;

					}

					if (g_aBlock[nCntBlock].state == BLOCKSTATE_SPRING)
					{

						pMove->y -= 60.0f;


					}
					else
					{
						pPos->y = g_aBlock[nCntBlock].pos.y + 35;
						pMove->y = 0.0f;
					}
				}
				else if (pPosold->y <= g_aBlock[nCntBlock].posold.y && pPos->y >= g_aBlock[nCntBlock].pos.y)
				{//�ォ�痈����
					bLand = true;
					if (g_aBlock[nCntBlock].state == BLOCKSTATE_MOVE_Y)
					{

						pPos->y = g_aBlock[nCntBlock].pos.y;
						//pMove->y += g_aBlock[nCntBlock].move.y;

						pMove->y = 0.0f;

						if (g_aBlock[nCntBlock].Move_Y <= 0.1f)
						{
							pPos->y = g_aBlock[nCntBlock].pos.y;
							bLand = true;
						}

					}
				}
				else if (pPosold->y - fHeight >= g_aBlock[nCntBlock].posold.y + g_aBlock[nCntBlock].fHeight && pPos->y - fHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
				{//�����痈����	




					pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fHeight;
					pMove->y = 2.0f;

				}
				else  if (pPosold->x + fWidth / 2 <= g_aBlock[nCntBlock].posold.x  && pPos->x + fWidth / 2 >= g_aBlock[nCntBlock].pos.x)
				{//�������痈����
				 //bLand = true;
					pPos->x = g_aBlock[nCntBlock].pos.x - fWidth / 2;
					pMove->x *= -0.5;
				}

				else  if (pPosold->x - fWidth / 2 >= g_aBlock[nCntBlock].posold.x + g_aBlock[nCntBlock].fWidth && pPos->x - fWidth / 2 <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth)
				{//�E�����痈����
				 //bLand = true;
					pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fWidth / 2;
					pMove->x *= -0.5;
				}
			}
		}
	}
	return bLand;
}


//=============================================================================
// �u���b�N�̐ݒ�
//=============================================================================
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType, BLOCKSTATE state, float fMove_X, float fMove_Y)
{
	int nCntBlock;
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
			if (g_aBlock[nCntBlock].bUse == false)	//�u���b�N���g�p����Ă��Ȃ�
			{
				g_aBlock[nCntBlock].pos = pos;

				g_aBlock[nCntBlock].fHeight = fHeight;
				g_aBlock[nCntBlock].fWidth = fWidth;

				g_aBlock[nCntBlock].move.x = fMove_X;
				g_aBlock[nCntBlock].move.y = fMove_Y;

				g_aBlock[nCntBlock].nType = nType;


				g_aBlock[nCntBlock].state = state;
									
				//���_�̍��W
				pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x , g_aBlock[nCntBlock].pos.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y , 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x , g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);

				//if (g_aBlock[nCntBlock].nType == 1)
				//{


				//	//�e�N�X�`�����W
				//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				//	pVtx[1].tex = D3DXVECTOR2(g_aBlock[nCntBlock].fWidth / BLOCK_X, 0.0f);
				//	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
				//	pVtx[3].tex = D3DXVECTOR2(g_aBlock[nCntBlock].fWidth / BLOCK_X, 1.0f);
				//}
				//else
				//{
				//	//�e�N�X�`�����W
				//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				//}

				g_aBlock[nCntBlock].FallenPoint.x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2;
				//g_aBlock[nCntBlock].FallenPoint.y = g_aBlock[nCntBlock].pos.y + 120;
				g_aBlock[nCntBlock].FallenPoint.y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight * 2 ;


				g_aBlock[nCntBlock].bUse = true;	//�g�p���Ă����Ԃɂ���

				break;
			}
			pVtx +=  4;


		}
		//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBlock->Unlock();
}




//=============================================================================
// �u���b�N�̎擾
//=============================================================================
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}
