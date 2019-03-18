////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �G�t�F�N�g�̏��� [effect.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "effect.h"
#include "player.h"
#include "enemy.h"
#include "effectUP.h"
#include "game.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_EFFECT	    (6244)
#define ANIM			(0.2f)
#define MAX_PATTERN		(5)
#define MAX_ANIMSPEED	(5)

//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	char *pFileName;
}EffectTexture;
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_apTextureEffect[EFFECTTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
Effect g_aEffect[MAX_EFFECT];

EffectTexture g_aEffectTexInfo[EFFECTTYPE_MAX] =
{
	{ "data/TEXTURE/effect000.jpg" },
	{ "data/TEXTURE/smoke.jpg" },
	{ "data/TEXTURE/Water.jpg" },
	{ "data/TEXTURE/snow.jpg" },
	{ "data/TEXTURE/rockon.jpg" },
	{ "data/TEXTURE/EffectExplosion001.png" },
	{ "data/TEXTURE/watereffect.png" },
	{ "data/TEXTURE/HitEffect000.jpg" },
	{ "data/TEXTURE/smoke.jpg" },
	{ "data/TEXTURE/smoke.jpg" },
	{ "data/TEXTURE/HitEffect001.png" },
	{ "data/TEXTURE/Sunder000.jpg" },
	{ "data/TEXTURE/Sunder001.jpg" },
	{ "data/TEXTURE/Pill.png" },
	{ "data/TEXTURE/smoke.jpg" },
	{ "data/TEXTURE/effect000.jpg" },
	{ "data/TEXTURE/smoke.jpg" },
	{ "data/TEXTURE/smoke.png" },
	{ "data/TEXTURE/FlozeEffect.png"},
	{ "data/TEXTURE/Sunder001.jpg" },
	{ "data/TEXTURE/Water.jpg" },

};
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	for (int nCntEffectTex = 0; nCntEffectTex < EFFECTTYPE_MAX; nCntEffectTex++)
	{// �e�N�X�`���̐ݒ�
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
			g_aEffectTexInfo[nCntEffectTex].pFileName,		// �t�@�C���̖��O
			&g_apTextureEffect[nCntEffectTex]);				// �e�N�X�`���ւ̃|�C���^
	}
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aEffect[nCntEffect].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��̈ʒu
		g_aEffect[nCntEffect].Destpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړI�̈ʒu
		g_aEffect[nCntEffect].Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����ʒu
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aEffect[nCntEffect].fAngle = 0;							// �p�x
		g_aEffect[nCntEffect].fLength = 0;							// ����
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
		g_aEffect[nCntEffect].fWidth = 0;							// ��
		g_aEffect[nCntEffect].fHeight = 0;							// ����
		g_aEffect[nCntEffect].fDepth = 0;							// ���s��
		g_aEffect[nCntEffect].fRadius = 0;							// ����
		g_aEffect[nCntEffect].fSinCosMove = 0;						// �h��
		g_aEffect[nCntEffect].fChangeRadius = 0;					// �g��k��
		g_aEffect[nCntEffect].nLife = 0;							// ����
		g_aEffect[nCntEffect].nType = EFFECTTYPE_EXPLOSION;			// ���
		g_aEffect[nCntEffect].nIdyShadow = 0;						// �e��ID
		g_aEffect[nCntEffect].nCounterAnim = 0;						// �A�j���[�V�����J�E���^�[
		g_aEffect[nCntEffect].nPatternAnim = 0;						// �A�j���[�V�����p�^�[��
		g_aEffect[nCntEffect].nSpeedCounter = 0;					// �J�E���^�[
		g_aEffect[nCntEffect].bUse = false;							// �g�p����Ă��邩�ǂ���
	}
	MakeVertexEffect(pDevice);
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitEffect(void)
{
	// �e�N�X�`���̔j��
	for (int nCntEffect = 0; nCntEffect < EFFECTTYPE_MAX; nCntEffect++)
	{
		if (g_apTextureEffect[nCntEffect] != NULL)
		{
			g_apTextureEffect[nCntEffect]->Release();
			g_apTextureEffect[nCntEffect] = NULL;
		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateEffect(void)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^
	Player *pPlayer = GetPlayer();
	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++,pVtx+=4)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			// �O��̈ʒu�̐ݒ�
			g_aEffect[nCntEffect].posOld = g_aEffect[nCntEffect].pos;
			// �ړ��ʉ��Z
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			switch (g_aEffect[nCntEffect].nType)
			{
			case EFFECTTYPE_EXPLOSION:	// �����G�t�F�N�g
				g_aEffect[nCntEffect].fRadius--;
				if (g_aEffect[nCntEffect].fRadius <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				g_aEffect[nCntEffect].col.a -= 0.01f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_PLAYER:	// �v���C���[
				g_aEffect[nCntEffect].pos.y -= 0.1f;
				g_aEffect[nCntEffect].fRadius -= 0.1f;
				if (g_aEffect[nCntEffect].fRadius <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				g_aEffect[nCntEffect].col.a -= 0.01f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}

				break;
			case EFFECTTYPE_WATER:	// �����Ԃ�
				g_aEffect[nCntEffect].move.y -= 0.49f;
				//g_aEffect[nCntEffect].col.a -= 0.01f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].pos.y <= -130)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_DRAIN:	// �z��
				if (g_aEffect[nCntEffect].nLife <= 0)
				{
					g_aEffect[nCntEffect].nSpeedCounter++;
					g_aEffect[nCntEffect].pos += (g_aEffect[nCntEffect].Destpos - g_aEffect[nCntEffect].pos)*0.01f*float(g_aEffect[nCntEffect].nSpeedCounter);
					g_aEffect[nCntEffect].fRadius -= 0.1f;
					g_aEffect[nCntEffect].col.a -= 0.02f;
				}
				g_aEffect[nCntEffect].nLife--;
				g_aEffect[nCntEffect].Destpos = D3DXVECTOR3(pPlayer->aModel[0][pPlayer->nWeaponState].mtxWorld._41, pPlayer->aModel[0][pPlayer->nWeaponState].mtxWorld._42, pPlayer->aModel[0][pPlayer->nWeaponState].mtxWorld._43);
				if (g_aEffect[nCntEffect].fRadius <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_AIM:
				g_aEffect[nCntEffect].rot.x += 0.1f;
				g_aEffect[nCntEffect].rot.y += 0.1f;
				g_aEffect[nCntEffect].fChangeRadius += 0.1f;
				g_aEffect[nCntEffect].pos = GetEnemy()[GetPlayer()->nIdyLockEnemy].pos + D3DXVECTOR3(0.0f, GetEnemy()[GetPlayer()->nIdyLockEnemy].fRadius, 0.0f);
				g_aEffect[nCntEffect].fRadius += sinf(g_aEffect[nCntEffect].fChangeRadius) * 5;
				if (GetPlayer()->bAttention == false)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				// ���_�̒�����]
				g_aEffect[nCntEffect].fAngle = atan2f((g_aEffect[nCntEffect].fRadius), (g_aEffect[nCntEffect].fRadius));
				g_aEffect[nCntEffect].fLength = sqrtf((g_aEffect[nCntEffect].fRadius)*(g_aEffect[nCntEffect].fRadius) + (g_aEffect[nCntEffect].fRadius)*(g_aEffect[nCntEffect].fRadius));
				break;
			case EFFECTTYPE_DEATH:	// ���S�G�t�F�N�g
				if (g_aEffect[nCntEffect].fRadius <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				//	g_aEffect[nCntEffect].col.a -= 0.01f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].move.x <= 1)
				{
					g_aEffect[nCntEffect].fRadius--;

				}
				g_aEffect[nCntEffect].move.x *= 0.97f;
				g_aEffect[nCntEffect].move.z *= 0.97f;
				g_aEffect[nCntEffect].move.y += 0.01f;

				break;
			case EFFECTTYPE_WATERFALL:
				g_aEffect[nCntEffect].move.y -= 0.49f;
				//g_aEffect[nCntEffect].col.a -= 0.01f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].pos.y <= -130)
				{
					g_aEffect[nCntEffect].bUse = false;
				}

				break;
			case EFFECTTYPE_IMPACT:
				g_aEffect[nCntEffect].fRadius += 20.0f;
				g_aEffect[nCntEffect].col.a -= 0.05f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].pos.y <= -200)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_SMOKE:	// ���ނ�
				g_aEffect[nCntEffect].fRadius++;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].move.x <= 2)
				{
					g_aEffect[nCntEffect].col.a -= 0.01f;
				}
				g_aEffect[nCntEffect].move.x *= 0.97f;
				g_aEffect[nCntEffect].move.z *= 0.97f;
				g_aEffect[nCntEffect].move.y += 0.003f;
				break;
			case EFFECTTYPE_SMOKEDOWN:	// ����
				g_aEffect[nCntEffect].fRadius++;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].move.x <= 2)
				{
					g_aEffect[nCntEffect].col.a -= 0.01f;
				}
				if (g_aEffect[nCntEffect].pos.y <= -100)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				g_aEffect[nCntEffect].move.x *= 0.97f;
				g_aEffect[nCntEffect].move.z *= 0.97f;
				g_aEffect[nCntEffect].move.y -= 0.98f;
				break;
			case EFFECTTYPE_HIT:
				g_aEffect[nCntEffect].fRadius += 20.0f;
				g_aEffect[nCntEffect].col.a -= 0.1f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_SUNDER:
				g_aEffect[nCntEffect].col.a -= 0.1f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_SUNDER1:
				g_aEffect[nCntEffect].col.a -= 0.1f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_POISON:
				g_aEffect[nCntEffect].col.a -= 0.03f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				g_aEffect[nCntEffect].fRadius -= 0.01f;
				if (g_aEffect[nCntEffect].fRadius <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_POISONSMOKE:
				g_aEffect[nCntEffect].fRadius += 0.3f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].move.x <= 1.5f&&g_aEffect[nCntEffect].move.x >= -1.5f ||
					g_aEffect[nCntEffect].move.z <= 1.5f&&g_aEffect[nCntEffect].move.z >= -1.5f)
				{
					g_aEffect[nCntEffect].col.a -= 0.015f;
				}

				g_aEffect[nCntEffect].move.x *= 0.97f;
				g_aEffect[nCntEffect].move.z *= 0.97f;
				g_aEffect[nCntEffect].move.y += 0.0001f;
				break;
			case EFFECTTYPE_HITEXPLOSION:
				g_aEffect[nCntEffect].fRadius--;
				if (g_aEffect[nCntEffect].fRadius <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].move.x <= 1 && g_aEffect[nCntEffect].move.x >= -1)
				{
					g_aEffect[nCntEffect].col.a -= 0.01f;
				}

				g_aEffect[nCntEffect].move.x *= 0.97f;
				g_aEffect[nCntEffect].move.z *= 0.97f;
				g_aEffect[nCntEffect].move.y *= 0.97f;
				break;
			case EFFECTTYPE_FIRE:
				g_aEffect[nCntEffect].fRadius += 0.3f;
				if (g_aEffect[nCntEffect].col.a <= 0.1f)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].move.x <= 1.5f&&g_aEffect[nCntEffect].move.x >= -1.5f ||
					g_aEffect[nCntEffect].move.z <= 1.5f&&g_aEffect[nCntEffect].move.z >= -1.5f)
				{
					g_aEffect[nCntEffect].col.a -= 0.1f;
				}

				g_aEffect[nCntEffect].move.x *= 0.97f;
				g_aEffect[nCntEffect].move.z *= 0.97f;
				g_aEffect[nCntEffect].move.y += 0.0001f;
				break;
			case EFFECTTYPE_WIND:
				g_aEffect[nCntEffect].fRadius += 0.3f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				g_aEffect[nCntEffect].col.a -= 0.03f;

				g_aEffect[nCntEffect].move.x *= 0.97f;
				g_aEffect[nCntEffect].move.z *= 0.97f;
				g_aEffect[nCntEffect].move.y += 0.003f;
				break;
			case EFFECTTYPE_FLOZE:
				g_aEffect[nCntEffect].fRadius += 0.1f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				g_aEffect[nCntEffect].col.a -= 0.03f;
				break;
			case EFFECTTYPE_SUNDER_DOWN:
				g_aEffect[nCntEffect].move.y -= 3.49f;
				if (pPlayer->pos.y >= g_aEffect[nCntEffect].pos.y)
				{
					g_aEffect[nCntEffect].move.y = 0;
					if (g_aEffect[nCntEffect].nSpeedCounter <= 0)
					{
						float nDirection2 = SetRandomEffect();
						g_aEffect[nCntEffect].move.x = sinf(nDirection2)*(rand() % 15);
						g_aEffect[nCntEffect].move.z = cosf(nDirection2)*(rand() % 15);
					}
					g_aEffect[nCntEffect].col.a -= 0.03f;
					g_aEffect[nCntEffect].nSpeedCounter++;

				}
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_WATER_DOWN:
				g_aEffect[nCntEffect].move.y -= 3.49f;
				if (pPlayer->pos.y >= g_aEffect[nCntEffect].pos.y)
				{
					g_aEffect[nCntEffect].move.y = 0;
					if (g_aEffect[nCntEffect].nSpeedCounter <= 0)
					{
						float nDirection2 = SetRandomEffect();
						g_aEffect[nCntEffect].move.x = sinf(nDirection2)*(rand() % 15);
						g_aEffect[nCntEffect].move.z = cosf(nDirection2)*(rand() % 15);
					}
					g_aEffect[nCntEffect].col.a -= 0.03f;
					g_aEffect[nCntEffect].nSpeedCounter++;

				}
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			}
			if (g_aEffect[nCntEffect].pos.y <= -300)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
			//// ���_���̐ݒ�
			//pVtx[0].pos = D3DXVECTOR3(sinf(-g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x)*g_aEffect[nCntEffect].fLength,
			//	cosf(-g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.y)*g_aEffect[nCntEffect].fLength, 0.0f) + g_aEffect[nCntEffect].pos;
			//pVtx[1].pos = D3DXVECTOR3(sinf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x)*g_aEffect[nCntEffect].fLength,
			//	cosf( g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.y)*g_aEffect[nCntEffect].fLength, 0.0f) + g_aEffect[nCntEffect].pos;
			//pVtx[2].pos = D3DXVECTOR3(D3DX_PI + sinf(-g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x)*g_aEffect[nCntEffect].fLength,
			//	cosf(D3DX_PI - g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.y)*g_aEffect[nCntEffect].fLength, 0.0f) + g_aEffect[nCntEffect].pos;
			//pVtx[3].pos = D3DXVECTOR3(D3DX_PI + sinf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x)*g_aEffect[nCntEffect].fLength,
			//	cosf(D3DX_PI - g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.y)*g_aEffect[nCntEffect].fLength, 0.0f) + g_aEffect[nCntEffect].pos;
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			g_aEffect[nCntEffect].nCounterAnim++;
			if (g_aEffect[nCntEffect].nType == EFFECTTYPE_WATERFALL)
			{
				if (g_aEffect[nCntEffect].nCounterAnim % 3 == 0)
				{
					g_aEffect[nCntEffect].nPatternAnim = (g_aEffect[nCntEffect].nPatternAnim + 1) % MAX_PATTERN;
					// �e�N�X�`���̐ݒ�
					pVtx[0].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 5) *ANIM, (g_aEffect[nCntEffect].nPatternAnim / 5)*0.33f);
					pVtx[1].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 5)*ANIM + ANIM, (g_aEffect[nCntEffect].nPatternAnim / 5)*0.33f);
					pVtx[2].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 5)*ANIM, (g_aEffect[nCntEffect].nPatternAnim / 5)*0.33f + 0.33f);
					pVtx[3].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 5)*ANIM + ANIM, (g_aEffect[nCntEffect].nPatternAnim / 5)*0.33f + 0.33f);
				}
			}
			else if (g_aEffect[nCntEffect].nType == EFFECTTYPE_DEATH)
			{
				if (g_aEffect[nCntEffect].nCounterAnim % 5 == 0)
				{
					g_aEffect[nCntEffect].nPatternAnim = (g_aEffect[nCntEffect].nPatternAnim + 1) % 7;
					// �e�N�X�`���̐ݒ�
					pVtx[0].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 7) *0.142f, (g_aEffect[nCntEffect].nPatternAnim / 7)*1.0f);
					pVtx[1].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 7)*0.142f + 0.142f, (g_aEffect[nCntEffect].nPatternAnim / 7)*1.0f);
					pVtx[2].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 7)*0.142f, (g_aEffect[nCntEffect].nPatternAnim / 7)*1.0f + 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 7)*0.142f +0.142f, (g_aEffect[nCntEffect].nPatternAnim / 7)*1.0f + 1.0f);
				}
			}
			else if (g_aEffect[nCntEffect].nType == EFFECTTYPE_SUNDER1|| g_aEffect[nCntEffect].nType == EFFECTTYPE_SUNDER_DOWN)
			{
				if (g_aEffect[nCntEffect].nCounterAnim % 5 == 0)
				{
					g_aEffect[nCntEffect].nPatternAnim = (g_aEffect[nCntEffect].nPatternAnim + 1) % 4;
					// �e�N�X�`���̐ݒ�
					pVtx[0].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4) *0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f);
					pVtx[1].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4)*0.25f + 0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f);
					pVtx[2].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4)*0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f + 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4)*0.25f + 0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f + 1.0f);
				}
			}
			else
			{
				// �e�N�X�`���̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();

}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							// �r���{�[�h�p�}�g���b�N�X


	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Z�o�b�t�@�ւ̏�������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�\�[�X
	//pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			if (g_aEffect[nCntEffect].nType == EFFECTTYPE_POISON||g_aEffect[nCntEffect].nType == EFFECTTYPE_WIND||g_aEffect[nCntEffect].nType == EFFECTTYPE_FLOZE)
			{
				// ���u�����f�B���O�����ɖ߂�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
			else
			{
				// ���u�����f�B���O�����Z�����ɐݒ�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			// �r���{�[�h�̐ݒ�
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			// �t�s��̐ݒ�
			g_aEffect[nCntEffect].mtxWorld._11 = mtxView._11;
			g_aEffect[nCntEffect].mtxWorld._12 = mtxView._21;
			g_aEffect[nCntEffect].mtxWorld._13 = mtxView._31;
			g_aEffect[nCntEffect].mtxWorld._21 = mtxView._12;
			g_aEffect[nCntEffect].mtxWorld._22 = mtxView._22;
			g_aEffect[nCntEffect].mtxWorld._23 = mtxView._32;
			g_aEffect[nCntEffect].mtxWorld._31 = mtxView._13;
			g_aEffect[nCntEffect].mtxWorld._32 = mtxView._23;
			g_aEffect[nCntEffect].mtxWorld._33 = mtxView._33;
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEffect[g_aEffect[nCntEffect].nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);

		}

	}

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// Z�o�b�t�@�ւ̏�������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


}

//==============================================================================================================================
// ���_���̍쐬
//==============================================================================================================================
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_aEffect[nCntEffect].col;
		pVtx[1].col = g_aEffect[nCntEffect].col;
		pVtx[2].col = g_aEffect[nCntEffect].col;
		pVtx[3].col = g_aEffect[nCntEffect].col;
		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();

}
//==============================================================================================================================
// �G�t�F�N�g�̐ݒ�
//==============================================================================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nLife, EFFECTTYPE nType)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].Initpos = pos;
			g_aEffect[nCntEffect].Destpos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].rot = rot;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nType = nType;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].fChangeRadius = 0;
			g_aEffect[nCntEffect].nPatternAnim = 0;
			g_aEffect[nCntEffect].nCounterAnim = 0;
			g_aEffect[nCntEffect].nSpeedCounter = 0;
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;
			if (g_aEffect[nCntEffect].nType == EFFECTTYPE_SUNDER1|| g_aEffect[nCntEffect].nType == EFFECTTYPE_SUNDER_DOWN)
			{
				g_aEffect[nCntEffect].nPatternAnim = rand() % 4;
				pVtx[0].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4) *0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f);
				pVtx[1].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4)*0.25f + 0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f);
				pVtx[2].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4)*0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f + 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4)*0.25f + 0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f + 1.0f);
			}
			g_aEffect[nCntEffect].bUse = true;
			break;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}
//==============================================================================================================================
// �����_�������̎擾
//==============================================================================================================================
float SetRandomEffect(void)
{
	float nDirection = 0;
	int nData;
	
	// �G�t�F�N�g�̕���
	nData = rand() % 2;
	if (nData == 0)
	{// �E��
		nDirection = float(rand() % 314);
	}
	else if (nData == 1)
	{// ����
		nDirection = float(rand() % 314) * -1;
	}
	nDirection /= 100;	// �����_��߂�

	return nDirection;
}
//==============================================================================================================================
// �G�t�F�N�g�̎擾
//==============================================================================================================================
Effect *GetEffect(void)
{
	return &g_aEffect[0];
}