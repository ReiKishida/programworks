//=============================================================================
//
// �A�C�e���̏��� [item.cpp]
// Author : ��������
//
//=============================================================================
#include "item.h"
#include "fade.h"
#include "player.h"
#include "shadow.h"
#include "game.h"
#include "meshField.h"
#include "model.h"
#include "score.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ITEM_KOUTOKU_TEXTURE	("data/TEXTURE/koutoku_Item.png")	// �K���̃e�N�X�`���̃A�h���X
#define MAX_ITEM				(128)								// �A�C�e���̍ő吔
#define ITEM_KOUTOKU_WIDTH		(25.0f / 2.0f)						// �K���̕�
#define ITEM_KOUTOKU_HEIGHT		(25.0f)								// �K���̍���
#define ITEM_KOUTOKU_SCORE		(100)								// �K���̓��_
#define KOUTOKU_PATTEN			(1.0f)								// �K���̃e�N�X�`���A�j���[�V�����̃p�^�[����
#define ITEM_GRAVITY			(-1.5f)								// �d��
#define ITEM_INERTIA			(0.10f)								// ����
#define ITEM_JUMP				(3.0f)								// �W�����v��
#define ITEM_JUMP_DOWN			(0.3f)								// �A�C�e���̃W�����v�͂̌�����
#define ITEM_GET_RADIUS			(400.0f)							// �v���C���[���߂Â�����z�����Ă���͈�
#define ITEM_GET_PLAYER_RADIUS	(50.0f)								// �v���C���[�̓����蔻��̔��a
#define ITEM_DROP_MOVE			(1.0f)								// �z���t���Ă��鑬�x
#define ITEM_ADD_SPIRIT			(3)									// �A�C�e���擾���̃X�s���b�g������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexItem(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureItem = NULL;		// �e�N�X�`���ւ̃|�C���^
Item					g_Item[MAX_ITEM];			// �A�C�e���̏��
float					g_fItemGetRadius;			// �߂Â�����z���t���Ă���͈�

//=============================================================================
// ����������
//=============================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();						// �f�o�C�X�̎擾

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,								// 3D�f�o�C�X�̃A�h���X
							  ITEM_KOUTOKU_TEXTURE,					// �ǂݍ��ރe�N�X�`���̃t�@�C���̃A�h���X
							  &g_pTextureItem);						// �e�N�X�`���̃|�C���^

	// �A�C�e�����̏�����
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// �A�C�e���̑��������[�v
		g_Item[nCntItem].pos			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu��������
		g_Item[nCntItem].posOld			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��̈ʒu��������
		g_Item[nCntItem].InitPos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �����ʒu
		g_Item[nCntItem].move			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ��ʂ�������
		g_Item[nCntItem].fJampPower		 = 0.0f;							// �W�����v�͂�������

		g_Item[nCntItem].nCounterAnime	 = 0;								// �J�E���^�[��������
		g_Item[nCntItem].nPattanAnime	 = 0;								// �p�^�[����������
		g_Item[nCntItem].fMaxPattanAnime = 0;								// �p�^�[���̍ő吔��������
		g_Item[nCntItem].nIdyShadow		 = 0;								// �e�̔ԍ���������
		g_Item[nCntItem].ItemType		 = ITEM_KOUTOKU;					// �A�C�e���̎�ނ�������
		g_Item[nCntItem].fWidth			 = 0.0f;							// �A�C�e���̕���������
		g_Item[nCntItem].fHeight		 = 0.0f;							// �A�C�e���̍�����������
										 
		g_Item[nCntItem].bUse			 = false;							// �g�p���Ă��Ȃ���Ԃɂ���
	}

	g_fItemGetRadius = ITEM_GET_RADIUS * ITEM_GET_RADIUS;					// �z���t���Ă���͈͂��v�Z

	// ���_���̍쐬
	MakeVertexItem(pDevice);

}

//=============================================================================
// �I������
//=============================================================================
void UninitItem(void)
{
	// ���_�o�b�t�@�̊J��(�j��)
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}

	// �e�N�X�`���̔j��
	if (g_pTextureItem != NULL)
	{
		g_pTextureItem->Release();
		g_pTextureItem = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateItem(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;															// ���_���̃|�C���^

	Player *pPlayer = GetPlayer();												// �v���C���[���̎擾

	// ���_�o�b�t�@�����b�N���A���_�Ńf�[�^�ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pVtx += 4)
	{// �A�C�e���̐������[�v

		if (g_Item[nCntItem].bUse == true)
		{// �g���Ă���Ƃ�

			// �L�����N�^�[�ƃA�C�e���̋����̌v�Z
			float PosX = (g_Item[nCntItem].pos.x - pPlayer->pos.x) * (g_Item[nCntItem].pos.x - pPlayer->pos.x);		// (�A�C�e���̈ʒu�|�L�����̈ʒu)���悷��						
			float PosY = (g_Item[nCntItem].pos.y - pPlayer->pos.y) * (g_Item[nCntItem].pos.y - pPlayer->pos.y);		// (�A�C�e���̈ʒu�|�L�����̈ʒu)���悷��
			float PosZ = (g_Item[nCntItem].pos.z - pPlayer->pos.z) * (g_Item[nCntItem].pos.z - pPlayer->pos.z);		// (�A�C�e���̈ʒu�|�L�����̈ʒu)���悷��
			g_Item[nCntItem].fPosLenght = PosX + PosY + PosZ;														// �v�Z�����l�𑫂��āA�������v�Z����

			//-----------------------------------------------
			// �ʒu�̍X�V
			//-----------------------------------------------
			g_Item[nCntItem].posOld = g_Item[nCntItem].pos;									// �O��̈ʒu��ۑ�

			g_Item[nCntItem].move.y += ITEM_GRAVITY;										// �d�͂̉��Z

			if (g_Item[nCntItem].fJampPower > 0.0f)
			{// �W�����v�͂��܂���������
				g_Item[nCntItem].move.y += g_Item[nCntItem].fJampPower;						// �W�����v�ʂ��ړ��ʂɉ��Z����(�㏸���Ă���)
				g_Item[nCntItem].fJampPower -= ITEM_JUMP_DOWN;								// �W�����v�͂������Ă���
			}

			if (g_Item[nCntItem].fPosLenght < g_fItemGetRadius)
			{// �z���t���Ă���͈͂̒��ɂ�����
				float fAngle;																// �v���C���[�ƃA�C�e���̊p�x������ϐ�
				float fAngleY;																// �v���C���[�ƃA�C�e���̊p�x������ϐ�

				// �A�C�e������v���C���[�ւ̊p�x���v�Z
				fAngle = atan2f(pPlayer->pos.x - g_Item[nCntItem].pos.x, pPlayer->pos.z - g_Item[nCntItem].pos.z);
				fAngleY = atanf((pPlayer->aModel[0][pPlayer->nWeaponState].mtxWorld._42) - g_Item[nCntItem].pos.y);											// y���̊p�x���v�Z

				// �v���C���[�̂ق��Ɉړ�
				g_Item[nCntItem].move.x += sinf(fAngle) * ITEM_DROP_MOVE;
				g_Item[nCntItem].move.y += sinf(fAngleY) * 3.0f;
				g_Item[nCntItem].move.z += cosf(fAngle) * ITEM_DROP_MOVE;
																																														// 
			}

			// ����
			//g_Item[nCntItem].move.x += (0.0f - g_Item[nCntItem].move.x) * ITEM_INERTIA;
			//g_Item[nCntItem].move.z += (0.0f - g_Item[nCntItem].move.z) * ITEM_INERTIA;


			g_Item[nCntItem].pos += g_Item[nCntItem].move;									// �ʒu�̍X�V

			//-----------------------------------------------
			// �����蔻��
			//-----------------------------------------------
			if(CollisionMeshField(&g_Item[nCntItem].pos, &g_Item[nCntItem].posOld, &g_Item[nCntItem].move, g_Item[nCntItem].fWidth, g_Item[nCntItem].fHeight, &g_Item[nCntItem].pMeshField) == true)
			{// �n�ʂ̏�ɂ�����
				g_Item[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ������Ȃ��悤�ɂ���
				Shadow *pShadow = GetShadow();
				pShadow[g_Item[nCntItem].nIdyShadow].pos.y = g_Item[nCntItem].pos.y + 0.1f;
				pShadow[g_Item[nCntItem].nIdyShadow].col.a = 1.0f;
			}		
			
			if (CollisionModelMinMax(&g_Item[nCntItem].pos, &g_Item[nCntItem].posOld, &g_Item[nCntItem].move, g_Item[nCntItem].fWidth, g_Item[nCntItem].fHeight, &g_Item[nCntItem].pModel) == 1)
			{
				g_Item[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ������Ȃ��悤�ɂ���
				Shadow *pShadow = GetShadow();
				pShadow[g_Item[nCntItem].nIdyShadow].pos.y = g_Item[nCntItem].pos.y + 0.1f;
				pShadow[g_Item[nCntItem].nIdyShadow].col.a = 1.0f;
			}

			//-----------------------------------------------
			// �e�N�X�`���A�j���[�V�����̍X�V
			//-----------------------------------------------
			if (g_Item[nCntItem].nCounterAnime % 10 == 0)
			{// 5�t���[�����Ƃ�
				// �e�N�X�`���̃R�}����i�߂�
				g_Item[nCntItem].nPattanAnime = (g_Item[nCntItem].nPattanAnime + 1) % int(g_Item[nCntItem].fMaxPattanAnime);

				//SetParticle(g_Item[nCntItem].pos, D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f), 20, 0);
				//SetParticle(g_Item[nCntItem].pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 20, 0);
				//SetParticle(g_Item[nCntItem].pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 20, 0);
				//SetParticle(g_Item[nCntItem].pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 20, 0);

				// �e�N�X�`�����W�̍X�V
				pVtx[0].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime)											 + 0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + (1.0f / g_Item[nCntItem].fMaxPattanAnime) + 0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime)											 + 0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + (1.0f / g_Item[nCntItem].fMaxPattanAnime) + 0.0f, 1.0f);
			}

			g_Item[nCntItem].nCounterAnime++;											// �J�E���^�[�𖈃t���[�����Z

			SetPositionShadow(g_Item[nCntItem].nIdyShadow, g_Item[nCntItem].pos, g_Item[nCntItem].InitPos);		// �e�̈ʒu�̍X�V
		}

	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffItem->Unlock();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxView, mtxTrans;				// �v�Z�p�̃}�g���N�X

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);				// "REF"���t�@�����X,��l
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// "FUNC"�t�@���N�V����,"GREATER"���傫��,��r���@������

	// Z�o�b�t�@�̐ݒ�
	//pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			// "FALSE"�ŕ`��̏��Ԃ𖳎����āA��Ɏ�O�ɕ`�悷��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// "FALSE"�ŕ`�悷�鏇�Ԃ���������`�悷��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// �`�悷�鏇�Ԃ����߂�

	// ���C�e�B���O���[�h�̉e�����󂯂Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);						// ���C�e�B���O���[�h�L��

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == true)
		{// �g���Ă���Ƃ�

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Item[nCntItem].mtxWorld);

			// �r���[�}�g���N�X�̎擾
			pDevice->GetTransform(D3DTS_VIEW,			// �r���[�}�g���N�X���擾����
								  &mtxView);			// �擾�����l��������

			// �t�s��̐ݒ�
			g_Item[nCntItem].mtxWorld._11 = mtxView._11;
			g_Item[nCntItem].mtxWorld._12 = mtxView._21;
			g_Item[nCntItem].mtxWorld._13 = mtxView._31;
			g_Item[nCntItem].mtxWorld._21 = mtxView._12;
			g_Item[nCntItem].mtxWorld._22 = mtxView._22;
			g_Item[nCntItem].mtxWorld._23 = mtxView._32;
			g_Item[nCntItem].mtxWorld._31 = mtxView._13;
			g_Item[nCntItem].mtxWorld._32 = mtxView._23;
			g_Item[nCntItem].mtxWorld._33 = mtxView._33;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
								  g_Item[nCntItem].pos.x,					// �ʒu��X���W�������Ă���
								  g_Item[nCntItem].pos.y,
								  g_Item[nCntItem].pos.z);

			D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorld,					// �Q�ԖڂƂR�Ԗڂ̈������|���Z���ꂽ�l����Ԗڂ̈����ɑ�����Ă���
							   &g_Item[nCntItem].mtxWorld,
							   &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Item[nCntItem].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureItem);

			// �A�C�e���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntItem * 4,						// �`��ŏ��̒��_								
				2);
		}
	}

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z�o�b�t�@�̐ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���C�e�B���O���[�h�̐ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ���C�e�B���O���[�h�L��
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexItem(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ITEM,			// �m�ۂ���o�b�t�@�̃T�C�Y�isizeof(VERTEX_3D)*�K�v�Ȑ��j
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,															// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;															// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�Ńf�[�^�ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pVtx += 4)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_Item[nCntItem].fWidth, g_Item[nCntItem].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( g_Item[nCntItem].fWidth, g_Item[nCntItem].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_Item[nCntItem].fWidth, 0.0f,					  0.0f);
		pVtx[3].pos = D3DXVECTOR3( g_Item[nCntItem].fWidth, 0.0f,					  0.0f);

		// �@���̐ݒ�			�����l��1�𒴂��Ă͂����Ȃ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.5f, 0.8f, 0.8f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.8f, 0.8f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.8f, 0.8f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.8f, 0.8f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime)																	  + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + (g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + 0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime)																      + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + (g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + 0.0f, 1.0f);

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// �A�C�e���̐ݒ�
//=============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ItemType type)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;															// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�Ńf�[�^�ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pVtx += 4)
	{// �ő吔�����[�v
		if (g_Item[nCntItem].bUse == false)
		{// �g���Ă��Ȃ�������
			g_Item[nCntItem].pos = pos;						// �ʒu��ݒ�
			g_Item[nCntItem].posOld = pos;					// �O��̈ʒu��ݒ�
			g_Item[nCntItem].InitPos = pos;					// �����ʒu
			g_Item[nCntItem].move = move;					// �ړ��ʂ�ݒ�
			g_Item[nCntItem].ItemType = type;				// �A�C�e���̎�ނ�ݒ�
			g_Item[nCntItem].fJampPower = ITEM_JUMP;		// �W�����v�͂�ݒ�
			g_Item[nCntItem].nCounterAnime = 0;				// �J�E���^�[��ݒ�
			g_Item[nCntItem].nPattanAnime = 0;				// �p�^�[����ݒ�
			g_Item[nCntItem].bUse = true;					// �g�p���Ă����Ԃɐݒ�

			switch (g_Item[nCntItem].ItemType)
			{
			case ITEM_KOUTOKU:			// �K��
				g_Item[nCntItem].fWidth = ITEM_KOUTOKU_WIDTH;		// ����ݒ�
				g_Item[nCntItem].fHeight = ITEM_KOUTOKU_HEIGHT;		// ������ݒ�
				g_Item[nCntItem].fMaxPattanAnime = KOUTOKU_PATTEN;		// �p�^�[���̍ő吔��������

				break;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_Item[nCntItem].fWidth, g_Item[nCntItem].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3( g_Item[nCntItem].fWidth, g_Item[nCntItem].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_Item[nCntItem].fWidth, 0.0f,					  0.0f);
			pVtx[3].pos = D3DXVECTOR3( g_Item[nCntItem].fWidth, 0.0f,					  0.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime)																	  + 0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + (g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + 0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime)																	  + 0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + (g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + 0.0f, 1.0f);


			// �e��ݒ�
			g_Item[nCntItem].nIdyShadow = SetShadow(g_Item[nCntItem].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),g_Item[nCntItem].fWidth,g_Item[nCntItem].fHeight);

			break;										// ���[�v�𔲂���
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// �A�C�e���̓����蔻��
//=============================================================================
void CollisionItem(D3DXVECTOR3 mtxWorld,float fRadius)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// �ő吔�����[�v
		if (g_Item[nCntItem].bUse == true)
		{// �g���Ă��Ȃ�������

		 	// �L�����N�^�[�ƃA�C�e���̋����̌v�Z
			float PosX = (g_Item[nCntItem].pos.x - mtxWorld.x);									// (�A�C�e���̈ʒu�|�L�����̈ʒu)���悷��						
			float PosY = ((g_Item[nCntItem].pos.y + ITEM_KOUTOKU_HEIGHT / 2) - mtxWorld.y);		// (�A�C�e���̈ʒu�|�L�����̈ʒu)���悷��
			float PosZ = (g_Item[nCntItem].pos.z - mtxWorld.z);									// (�A�C�e���̈ʒu�|�L�����̈ʒu)���悷��
			float fItemLenght = (PosX * PosX) + (PosY * PosY) + (PosZ * PosZ);															// �v�Z�����l�𑫂��āA�������v�Z����

			// �L�����ƃA�C�e���̔��a���v�Z
			float fRadiusLenght;																			// (�A�C�e���̔��a�|�L�����̔��a)���悵�ċ������v�Z����

			fRadiusLenght = (g_Item[nCntItem].fWidth + ITEM_GET_PLAYER_RADIUS) * 
							(g_Item[nCntItem].fWidth + ITEM_GET_PLAYER_RADIUS);					// �A�C�e���ƃL�����̔��a���v�Z

			//--------------------------------------
			// �����Ɣ��a�̔�r
			//--------------------------------------
			if (fItemLenght <  fRadiusLenght)
			{// �A�C�e���ƃL���������a�����߂�������

				DeleteItem(nCntItem, g_Item[nCntItem].ItemType);			// �A�C�e���擾

			}
		}
	}
}

//=============================================================================
// �A�C�e���̎擾����
//=============================================================================
void DeleteItem(int nCntItem, ItemType type)
{
	switch (type)
	{
	case ITEM_KOUTOKU:								// �K��
		AddScore(1000);
		PlayerSpiritAdd(ITEM_ADD_SPIRIT);
		break;
	}
	PlaySound(SOUND_LABEL_SE_ITEMGET);
	g_Item[nCntItem].bUse = false;				// �A�C�e���̍폜
	DeleteShadow(g_Item[nCntItem].nIdyShadow);	// �e�̍폜
}