//=============================================================================
//
// �v���C���[���� [Shadow.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "Shadow.h"
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

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define HALF_X (30.0f)	//X������
#define HALF_Y (100.0f)	//Y������

#define LIMIT_MIN_X (30)	//��ʒ[-X
#define LIMIT_MAX_X (1250)	//��ʒ[X
#define LIMIT_MIN_Y (50)	//��ʒ[-Y
#define LIMIT_MAX_Y (595)	//��ʒ[Y

#define SJADOWANIMATION_SPEED (8)	//�A�j���[�V�����̃X�s�[�h

#define JUMP_COUNT (1)	//���i�W�����v��


#define SHADOW_MOVE (7.0f)	//�ړ���
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//���_�o�b�t�@�ւ̃|�C���^
SHADOW g_Shadow;	//�v���C���[

float fJShadowump;	//�W�����v��
float fShadowGravity;	//�d��


int nShadowAnimation_Y;	//�e�N�X�`���A�j���[�V����


//=============================================================================
// �v���C���[��������
//=============================================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

								//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_Shadow.pos = D3DXVECTOR3(350, 580, 0.0f);//�v���C���[���S���W

	g_Shadow.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���


	g_Shadow.state = SHADOWSTATE_NORMAL;
	g_Shadow.bDisp = true;
	g_Shadow.nCounterState = 0;
	g_Shadow.nCounterAnim = 0;
	g_Shadow.nPatternAnim = 0;
	g_Shadow.fWidth = HALF_X;
	g_Shadow.fHeight = HALF_Y;

	g_Shadow.bUseJump = true;


	nShadowAnimation_Y = 0;

	g_Shadow.bPress = false;

	fJShadowump = SHADOWJUMP_POWER;
	fShadowGravity = SHADOWGRAVITY;

	g_Shadow.nJumpCount = 0;
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Shadow.png", &g_pTextureShadow);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(g_Shadow.pos.x - g_Shadow.fWidth, g_Shadow.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Shadow.pos.x + g_Shadow.fWidth, g_Shadow.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Shadow.pos.x - g_Shadow.fWidth, g_Shadow.pos.y - g_Shadow.fHeight /2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Shadow.pos.x + g_Shadow.fWidth, g_Shadow.pos.y - g_Shadow.fHeight /2, 0.0f);
	////���_���W
	//pVtx[0].pos = D3DXVECTOR3(g_Shadow.pos.x - g_Shadow.fWidth, g_Shadow.pos.y - g_Shadow.fHeight, 0.0f);
	//pVtx[1].pos = D3DXVECTOR3(g_Shadow.pos.x + g_Shadow.fWidth, g_Shadow.pos.y - g_Shadow.fHeight, 0.0f);
	//pVtx[2].pos = D3DXVECTOR3(g_Shadow.pos.x - g_Shadow.fWidth, g_Shadow.pos.y, 0.0f);
	//pVtx[3].pos = D3DXVECTOR3(g_Shadow.pos.x + g_Shadow.fWidth, g_Shadow.pos.y, 0.0f);

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
	g_pVtxBuffShadow->Unlock();
}
//=============================================================================
// �v���C���[��������
//=============================================================================
void UnInitShadow(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}
//=============================================================================
// �v���C���[�X�V����
//=============================================================================
void UpdateShadow(void)
{
	g_Shadow.posold = g_Shadow.pos;

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
								//g_Shadow.move.x = 0;
								//float fShadowGravity;


	D3DXVECTOR3 EffectPos;

	EffectPos.x = g_Shadow.pos.x;
	EffectPos.y = g_Shadow.pos.y - 50;


	D3DXVECTOR3 ShadowPos;
	ShadowPos.x = g_Shadow.pos.x + 10;
	ShadowPos.y = g_Shadow.pos.y - 25;


	//SetShadow(ShadowPos, 35.0f, 1);

	switch (g_Shadow.state)
	{

	case SHADOWSTATE_NORMAL:
		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffShadow->Unlock();
		break;

	case SHADOWSTATE_DAMAGE:
		g_Shadow.nCounterState--;

		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffShadow->Unlock();

		if (g_Shadow.nCounterState <= 0)
		{
			g_Shadow.nCounterState = 0;
			g_Shadow.state = SHADOWSTATE_NORMAL;
		}

		break;
	}

	//SetEffect(EffectPos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 55.0f, 25);

	//=========================================
	//�C�ӂ̃L�[(A�L�[)�������ꂽ���ǂ���    //
	//=========================================
	if (g_Shadow.state == SHADOWSTATE_NORMAL)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{

			g_Shadow.move.x -= SHADOW_MOVE;
			g_Shadow.bPress = true;

			nShadowAnimation_Y = 1;
			if (GetKeyboardPress(DIK_RSHIFT) == true)
			{
				SetEffect(EffectPos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 50.0f, 25);
				g_Shadow.move.x -= 3;

			}

		}


		//=========================================
		//�C�ӂ̃L�[(D�L�[)�������ꂽ���ǂ���    //
		//=========================================
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_Shadow.move.x += SHADOW_MOVE;
			g_Shadow.bPress = true;

			nShadowAnimation_Y = 0;
			if (GetKeyboardPress(DIK_RSHIFT) == true)
			{
				SetEffect(EffectPos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 50.0f, 25);
				g_Shadow.move.x += 3;
			}
		}

		else if (g_Shadow.bPress == true)
		{
			g_Shadow.bPress = false;
			if (g_Shadow.nPatternAnim % 2 == 0)
			{
				g_Shadow.nPatternAnim++;
			}
		}



		//=========================================
		//�C�ӂ̃L�[(SPACE�L�[)�������ꂽ���ǂ��� //
		//=========================================

		//if (GetKeyboardPress(DIK_SPACE) == true && g_Shadow.bUseJump == false)
		if (GetKeyboardTrigger(DIK_SPACE) == true && g_Shadow.bUseJump == false)
		{
			//g_Shadow.nJumpCount++;
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_Shadow.bUseJump = true;

			if (g_Shadow.bUseJump == true)
			{

				g_Shadow.move.y += fJShadowump;
				if (g_Shadow.nPatternAnim == 0)
				{
					g_Shadow.nPatternAnim = 1;
				}
				if (g_Shadow.nPatternAnim == 2)
				{
					g_Shadow.nPatternAnim = 3;
				}
			}
			//SetEffect(EffectPos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 30.0f, 10);
		}
	}




	if (g_Shadow.bUseJump == false)//�n��ɂ���Ƃ�
	{
		if (g_Shadow.move.x >= 0.1f || g_Shadow.move.x <= -0.1f) //�ړ��ʂ�0�ł͂Ȃ�
		{
			g_Shadow.nCounterAnim++; //�J�E���^�[�̉��Z �����̑��x
			if (g_Shadow.nCounterAnim % SJADOWANIMATION_SPEED == 0)
			{
				g_Shadow.nPatternAnim = (g_Shadow.nPatternAnim + 1) % 4;//�p�^�[��No.�X�V
			}
		}
		//else if (g_Shadow.nPatternAnim % 2 != 0)
		else if (g_Shadow.nPatternAnim == 1 || g_Shadow.nPatternAnim == 3)

		{
			g_Shadow.nCounterAnim++; //�J�E���^�[�̉��Z �����̑��x
			if (g_Shadow.nCounterAnim % SJADOWANIMATION_SPEED == 0)
			{
				g_Shadow.nPatternAnim = (g_Shadow.nPatternAnim + 1) % 4;//�p�^�[��No.�X�V
			}
		}
	}


	g_Shadow.move.y += fShadowGravity;
	if (g_Shadow.move.y > SHADOWGRAVITY_MAX)
	{
		g_Shadow.move.y = SHADOWGRAVITY_MAX;
	}

	g_Shadow.move.x += (0.0f - g_Shadow.move.x) * 0.35f;
	/*g_Shadow.move.y += (0.0f - g_Shadow.move.y) * 0.2f;*/



	g_Shadow.pos.x += g_Shadow.move.x;
	g_Shadow.pos.y += g_Shadow.move.y;




	if (g_Shadow.pos.x >= SCREEN_WIDTH + 20)
	{
		g_Shadow.pos = D3DXVECTOR3(10.0, g_Shadow.pos.y, 0.0);
	}

	if (g_Shadow.pos.x <= 0)
	{
		g_Shadow.pos = D3DXVECTOR3(1280.0, g_Shadow.pos.y, 0.0);
	}



	if (CollisionBlock(&g_Shadow.pos, &g_Shadow.posold, &g_Shadow.move, g_Shadow.fWidth, g_Shadow.fHeight) == true)
	{//�ڐG����
		g_Shadow.bUseJump = false;

	}
	else
	{//�󒆂ɂ���Ƃ�
		if (g_Shadow.bUseJump == false)
		{
			g_Shadow.bUseJump = true;
		}
	}



	
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(g_Shadow.pos.x - g_Shadow.fWidth, g_Shadow.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Shadow.pos.x + g_Shadow.fWidth, g_Shadow.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Shadow.pos.x - g_Shadow.fWidth, g_Shadow.pos.y - g_Shadow.fHeight / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Shadow.pos.x + g_Shadow.fWidth, g_Shadow.pos.y - g_Shadow.fHeight / 2, 0.0f);


	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0 + (0.25f*g_Shadow.nPatternAnim), 0.5f * nShadowAnimation_Y);
	pVtx[1].tex = D3DXVECTOR2(0.25f + (0.25f*g_Shadow.nPatternAnim), 0.5f* nShadowAnimation_Y);
	pVtx[2].tex = D3DXVECTOR2(0 + (0.25f*g_Shadow.nPatternAnim), 0.5f* (nShadowAnimation_Y + 1));
	pVtx[3].tex = D3DXVECTOR2(0.25f + (0.25f*g_Shadow.nPatternAnim), 0.5f* (nShadowAnimation_Y + 1));
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();


}


//=============================================================================
// �v���C���[�`�揈��
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

							  //�f�o�C�X���擾����
	pDevice = GetDevice();

	if (g_Shadow.bDisp == true)
	{

		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`��
		pDevice->SetTexture(0, g_pTextureShadow);


		//�v���C���[�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	}
}


bool CollisionShadow(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{



	bool bLand = false; //���n������

	if (pPos->x >= g_Shadow.pos.x - fWidth && pPos->x <= g_Shadow.pos.x + g_Shadow.fWidth + fWidth
		&& pPos->y >= g_Shadow.pos.y  && pPos->y <= g_Shadow.pos.y + g_Shadow.fHeight + g_Shadow.fWidth)
	{//����͈̔͂̂Ƃ�	

		if (pPosold->y >= g_Shadow.posold.y&& pPos->y <= g_Shadow.pos.y)
		{//�����痈����	
			pPos->y = g_Shadow.pos.y;
			pMove->y = 0.0f;
		}

	}


	return bLand;
}




