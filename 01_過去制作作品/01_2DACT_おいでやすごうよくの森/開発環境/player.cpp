//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "main.h"
#include "effect.h"
#include "item.h"
#include "score.h"
#include "sound.h"
#include "timer.h"
#include "block.h"
#include "itemcounter.h"
#include "vodkacounter.h"
#include "satsucounter.h"
#include "areycounter.h"
#include "fade.h"
#include "shadow.h"
#include "popscore.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define HALF_X (30.0f)	//X������
#define HALF_Y (100.0f)	//Y������

#define LIMIT_MIN_X (30)	//��ʒ[-X
#define LIMIT_MAX_X (1250)	//��ʒ[X
#define LIMIT_MIN_Y (50)	//��ʒ[-Y
#define LIMIT_MAX_Y (595)	//��ʒ[Y

#define ANIMATION_SPEED (8)	//�A�j���[�V�����̃X�s�[�h

#define JUMP_COUNT (1)	//���i�W�����v��


#define PLAYER_MOVE (7.0f)	//�ړ���
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;	//���_�o�b�t�@�ւ̃|�C���^
PLAYER g_Player;	//�v���C���[
bool bJump;

float fJump;	//�W�����v��
float fGravity;	//�d��


int nAnimation_Y;	//�e�N�X�`���A�j���[�V����

bool bLand; //���n����

//=============================================================================
// �v���C���[��������
//=============================================================================
void InitPlayer(void)
{
	

	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_Player.pos = D3DXVECTOR3(400, 550, 0.0f);//�v���C���[���S���W

	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
	

	g_Player.state = PLAYERSTATE_NORMAL;
	g_Player.bDisp = true;
	g_Player.nCounterState = 0;
	g_Player.nCounterAnim = 0;
	g_Player.nPatternAnim = 0;
	g_Player.fWidth = HALF_X;
	g_Player.fHeight = HALF_Y;

	g_Player.bUseJump = true;

	bLand = false;

	nAnimation_Y = 0;

	g_Player.bPress = false;

	fJump = JUMP_POWER;
	fGravity = GRAVITY;

	g_Player.nJumpCount = 0;
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player000.png", &g_pTexturePolygon);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
	
	//���_���W
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y , 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y , 0.0f);


	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}
//=============================================================================
// �v���C���[��������
//=============================================================================
void UnInitPlayer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}
//=============================================================================
// �v���C���[�X�V����
//=============================================================================
void UpdatePlayer(void)
{
	g_Player.posold = g_Player.pos;

	/*BLOCK *pBlock;
	pBlock = GetBlock();*/

	ITEM *pItem;
	pItem = GetItem();

	TIMER pTimer;
	pTimer = GetTimer();

	int pCounter;
	pCounter = GetItemCounter();

	int pVodka;
	pVodka = GetVodkaCounter();

	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	//g_Player.move.x = 0;
	//float fGravity;


	D3DXVECTOR3 EffectPos;

	EffectPos.x = g_Player.pos.x;
	EffectPos.y = g_Player.pos.y - 50;


	D3DXVECTOR3 ShadowPos;
	ShadowPos.x = g_Player.pos.x + 10;
	ShadowPos.y = g_Player.pos.y - 25;


	//SetShadow(ShadowPos, 35.0f, 1);

	switch (g_Player.state)
	{

	case PLAYERSTATE_NORMAL:
		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPolygon->Unlock();
		break;

	case PLAYERSTATE_DAMAGE:
		g_Player.nCounterState--;

		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
	
		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPolygon->Unlock();

		if (g_Player.nCounterState <= 0)
		{
			g_Player.nCounterState = 0;
			g_Player.state = PLAYERSTATE_NORMAL;
		}

		break;
	}

	//SetEffect(EffectPos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 55.0f, 25);

	//=========================================
	//�C�ӂ̃L�[(A�L�[)�������ꂽ���ǂ���    //
	//=========================================
	if (g_Player.state == PLAYERSTATE_NORMAL)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{

			g_Player.move.x -= PLAYER_MOVE;
			g_Player.bPress = true;

			nAnimation_Y = 1;
			if (GetKeyboardPress(DIK_RSHIFT) == true || GetKeyboardPress(DIK_LSHIFT) == true)
			{
				SetEffect(EffectPos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 50.0f, 10);
				//SetEffect(EffectPos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 50.0f, 10);

				g_Player.move.x -= 3;

			}

		}


		//=========================================
		//�C�ӂ̃L�[(D�L�[)�������ꂽ���ǂ���    //
		//=========================================
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_Player.move.x += PLAYER_MOVE;
			g_Player.bPress = true;

			nAnimation_Y = 0;
			if (GetKeyboardPress(DIK_RSHIFT) == true  || GetKeyboardPress(DIK_LSHIFT) == true)
			{
				SetEffect(EffectPos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 50.0f, 10);
				//SetEffect(EffectPos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 50.0f, 10);

				g_Player.move.x += 3;
			}
		}

		else if (g_Player.bPress == true)
		{
			g_Player.bPress = false;
			if (g_Player.nPatternAnim % 2 == 0)
			{
				g_Player.nPatternAnim++;
			}
		}



		//=========================================
		//�C�ӂ̃L�[(SPACE�L�[)�������ꂽ���ǂ��� //
		//=========================================

		//if (GetKeyboardPress(DIK_SPACE) == true && g_Player.bUseJump == false)
		if (GetKeyboardTrigger(DIK_SPACE) == true && g_Player.bUseJump == false)
		{
			//g_Player.nJumpCount++;
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_Player.bUseJump = true;

			if (g_Player.bUseJump == true)
			{

				g_Player.move.y += fJump;
				if (g_Player.nPatternAnim == 0)
				{
					g_Player.nPatternAnim = 1;
				}
				if (g_Player.nPatternAnim == 2)
				{
					g_Player.nPatternAnim = 3;
				}
			}
			//SetEffect(EffectPos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 30.0f, 10);
		}
	}

	


	if (g_Player.bUseJump == false)//�n��ɂ���Ƃ�
	{
		if (g_Player.move.x >= 0.1f || g_Player.move.x <= -0.1f) //�ړ��ʂ�0�ł͂Ȃ�
		{
			g_Player.nCounterAnim++; //�J�E���^�[�̉��Z �����̑��x
			if (g_Player.nCounterAnim % ANIMATION_SPEED == 0)
			{
				g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % 4;//�p�^�[��No.�X�V
			}
		}
		//else if (g_Player.nPatternAnim % 2 != 0)
		else if (g_Player.nPatternAnim == 1 || g_Player.nPatternAnim == 3)

		{
			g_Player.nCounterAnim++; //�J�E���^�[�̉��Z �����̑��x
			if (g_Player.nCounterAnim % ANIMATION_SPEED == 0)
			{
				g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % 4;//�p�^�[��No.�X�V
			}
		}
	}


	g_Player.move.y += fGravity;
	if (g_Player.move.y > GRAVITY_MAX)
	{
		g_Player.move.y = GRAVITY_MAX;
	}

	g_Player.move.x += (0.0f - g_Player.move.x) * 0.35f;
	/*g_Player.move.y += (0.0f - g_Player.move.y) * 0.2f;*/



	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.y += g_Player.move.y;


	
	if (g_Player.pos.y <= 550)
	{
		if (g_Player.pos.x >= SCREEN_WIDTH)
		{
			g_Player.pos = D3DXVECTOR3(10.0, g_Player.pos.y, 0.0);
		}

		if (g_Player.pos.x <= 0)
		{
			g_Player.pos = D3DXVECTOR3(1200.0, g_Player.pos.y, 0.0);
		}
	}


	if (g_Player.pos.x >= 1215)
	{
		g_Player.pos = D3DXVECTOR3(58.0, g_Player.pos.y, 0.0);
	}

	if (g_Player.pos.x <= 55 )
	{
		g_Player.pos = D3DXVECTOR3(1200.0, g_Player.pos.y, 0.0);
	}



	if (CollisionBlock(&g_Player.pos, &g_Player.posold, &g_Player.move, g_Player.fWidth, g_Player.fHeight) == true)
	{//�ڐG����
		g_Player.bUseJump = false;
	
	}
	else
	{//�󒆂ɂ���Ƃ�
		if (g_Player.bUseJump == false)
		{
			g_Player.bUseJump = true;
		}
	}

	D3DXVECTOR3 Shootmove;
	Shootmove.x = 0.0f;
	Shootmove.y = -10.0f;

	D3DXVECTOR3 Shootpos;
	Shootpos.x = g_Player.pos.x;
	Shootpos.y = g_Player.pos.y - g_Player.fHeight - 35;

	if (g_Player.state == PLAYERSTATE_NORMAL)
	{
		if (pItem->state == ITEMSTATE_NOMAL || pItem->state == ITEMSTATE_LAND)
		{
			for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
			{
				if (pItem->bUse == true)
				{

					if (g_Player.pos.x + g_Player.fWidth >= pItem->pos.x - pItem->fWidth
						&& g_Player.pos.x - g_Player.fWidth <= pItem->pos.x + pItem->fWidth
						&& g_Player.pos.y - g_Player.fHeight / 2 >= pItem->pos.y - pItem->fHeight
						&& g_Player.pos.y - g_Player.fHeight / 2 <= pItem->pos.y + pItem->fHeight)
					{



						switch (pItem->ItemType)
						{
						case ITEMTYPE_COIN:
							pItem->bUse = false;
							if (g_Player.state == PLAYERSTATE_NORMAL)
							{
								PlaySound(SOUND_LABEL_SE_COIN);
								AddScore(2500);
								AddCounter(1);
								SetPopScore(Shootpos, POPSCORETYPE_COIN_SCORE, Shootmove);
								//SetItem(Shootpos, ITEMTYPE_COIN_SCORE, Shootmove);

							}
							break;

						case ITEMTYPE_VODKA:
							pItem->bUse = false;
							if (g_Player.state == PLAYERSTATE_NORMAL)
							{
								AddScore(5000);
								AddVodkaCounter(1);
								PlaySound(SOUND_LABEL_SE_DRINK);
								
								SetPopScore(Shootpos, POPSCORETYPE_VODKA_SCORE, Shootmove);
								//SetItem(Shootpos, ITEMTYPE_VODKA_SCORE, Shootmove);

							}
							break;

						case ITEMTYPE_AREY:
							pItem->bUse = false;
							if (g_Player.state == PLAYERSTATE_NORMAL)
							{
								AddScore(-1000);
								g_Player.state = PLAYERSTATE_DAMAGE;
								g_Player.nCounterState = 35;
								AddAreyCounter(1);
								SetPopScore(Shootpos, POPSCORETYPE_AREY_SCORE, Shootmove);
								//SetItem(Shootpos, ITEMTYPE_AREY_SCORE, Shootmove);

							}
							PlaySound(SOUND_LABEL_SE_HIT001);		// �q�b�g��1
							break;

						case ITEMTYPE_PAPER:
							pItem->bUse = false;
							if (g_Player.state == PLAYERSTATE_NORMAL)
							{
								AddScore(10000);
								AddSatsuCounter(1);
								PlaySound(SOUND_LABEL_SE_PEOPLE);		// �q�b�g��1

								SetPopScore(Shootpos, POPSCORETYPE_SATSU_SCORE, Shootmove);
								//SetItem(Shootpos, ITEMTYPE_SATSU_SCORE, Shootmove);

							}
						}
					}
				}
			}
		}
	}
	//if (g_Player.pos.y - g_Player.fHeight >= SCREEN_HEIGHT)
	//{
	//	FADE pFade;
	//	pFade = *GetFade();

	//	if (pFade == FADE_NONE)
	//	{


	//		//SetMode(MODE_GAME);
	//		SetFade(MODE_GAME);

	//	}
	//}

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
	//���_���W
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y - 0, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y + 0, 0.0f);


	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0 + (0.25f*g_Player.nPatternAnim), 0.5f * nAnimation_Y);
	pVtx[1].tex = D3DXVECTOR2(0.25f + (0.25f*g_Player.nPatternAnim), 0.5f* nAnimation_Y);
	pVtx[2].tex = D3DXVECTOR2(0 + (0.25f*g_Player.nPatternAnim), 0.5f* (nAnimation_Y + 1));
	pVtx[3].tex = D3DXVECTOR2(0.25f + (0.25f*g_Player.nPatternAnim), 0.5f* (nAnimation_Y + 1));
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();

	
}


//=============================================================================
// �v���C���[�`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X���擾����
	pDevice = GetDevice();

	if (g_Player.bDisp == true)
	{
	
	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`��
	pDevice->SetTexture(0, g_pTexturePolygon);

	
		//�v���C���[�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	}
}


bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	


	bool bLand = false; //���n������

			if (pPos->x >= g_Player.pos.x - fWidth && pPos->x <= g_Player.pos.x + g_Player.fWidth + fWidth
				&& pPos->y >= g_Player.pos.y  && pPos->y <= g_Player.pos.y + g_Player.fHeight + g_Player.fWidth)
			{//����͈̔͂̂Ƃ�	
				
				 if (pPosold->y >= g_Player.posold.y&& pPos->y  <= g_Player.pos.y)
				{//�����痈����	
					pPos->y = g_Player.pos.y;
					pMove->y = 0.0f;
				}
			
			}
		
	
	return bLand;
}




PLAYER GetPlayer()
{
	return g_Player;
}