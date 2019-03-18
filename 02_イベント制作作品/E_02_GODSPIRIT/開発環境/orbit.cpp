////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �O�Ղ̏��� [orbit.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "orbit.h"
#include "player.h"
#include "gamepad.h"
#include "effect.h"
#include "input.h"
#include "game.h"
#include "tutorial.h"
#include "player.h"

//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	char *pFileName;
}OrbitTexture;
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexOrbit(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureOrbit[ORBITTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
Orbit g_aOrbit[MAX_ORBIT];
OrbitTexture g_aOrbitTexInfo[ORBITTYPE_MAX] =
{
	{ "data/TEXTURE/orbit000.jpg" },
	{ "data/TEXTURE/orbit001.jpg" },
	{ "data/TEXTURE/orbit002.jpg" },
	{ "data/TEXTURE/orbit003.jpg" },
};
int g_nCntOrbit;
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitOrbit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	
	for (int nCntOrbitTex = 0; nCntOrbitTex < ORBITTYPE_MAX; nCntOrbitTex++)
	{// �e�N�X�`���̐ݒ�
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			g_aOrbitTexInfo[nCntOrbitTex].pFileName,	// �t�@�C���̖��O
			&g_apTextureOrbit[nCntOrbitTex]);			// �e�N�X�`���ւ̃|�C���^
	}

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		g_aOrbit[nCntOrbit].nType = ORBITTYPE_0;					// ���
		g_aOrbit[nCntOrbit].nMotionType = PLAYER_MOTION_TYPE_MAX;
		g_aOrbit[nCntOrbit].Start = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �n�_
		g_aOrbit[nCntOrbit].End = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aOrbit[nCntOrbit].InitStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �n�_
		g_aOrbit[nCntOrbit].InitEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aOrbit[nCntOrbit].nIdxParent = 0;							// �e���f��
		g_aOrbit[nCntOrbit].nLength = MAX_ORBIT_LENGTH;				// ����
		g_aOrbit[nCntOrbit].nPlus = 0;								// ���炷��
		g_aOrbit[nCntOrbit].nCharacterType = ORBIT_PLAYER;				// �ǂ����Ŏg����
		g_aOrbit[nCntOrbit].nWeaponState = PLAYERWEAPON_HAND;			// ����^�C�v
		g_aOrbit[nCntOrbit].nCntTrueTime = 0;						// ������܂ł̎���
		g_aOrbit[nCntOrbit].bUse = false;							// �g�p����Ă��邩�ǂ���
	}

	MakeVertexOrbit(pDevice);
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitOrbit(void)
{
	// �e�N�X�`���̔j��
	for (int nCntOrbit = 0; nCntOrbit < ORBITTYPE_MAX; nCntOrbit++)
	{
		if (g_apTextureOrbit[nCntOrbit] != NULL)
		{
			g_apTextureOrbit[nCntOrbit]->Release();
			g_apTextureOrbit[nCntOrbit] = NULL;
		}
	}
	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		// ���_�o�b�t�@�̔j��
		if (g_aOrbit[nCntOrbit].pVtxBuff != NULL)
		{
			g_aOrbit[nCntOrbit].pVtxBuff->Release();
			g_aOrbit[nCntOrbit].pVtxBuff = NULL;
		}
	}
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateOrbit(void)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
		g_aOrbit[nCntOrbit].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		if (g_aOrbit[nCntOrbit].bUse == true)
		{
			for (int nCntLength = 0; nCntLength < g_aOrbit[nCntOrbit].nLength; nCntLength++)
			{
				pVtx[0 + nCntLength * 2].pos = pVtx[2 + nCntLength * 2].pos;
				pVtx[1 + nCntLength * 2].pos = pVtx[3 + nCntLength * 2].pos;
				pVtx[0 + nCntLength * 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.01f*nCntLength);
				pVtx[1 + nCntLength * 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.01f*nCntLength);
			}
			pVtx[g_aOrbit[nCntOrbit].nLength * 2 - 1].pos = D3DXVECTOR3(g_aOrbit[nCntOrbit].mtxStart._41, g_aOrbit[nCntOrbit].mtxStart._42, g_aOrbit[nCntOrbit].mtxStart._43);
			pVtx[g_aOrbit[nCntOrbit].nLength * 2 - 2].pos = D3DXVECTOR3(g_aOrbit[nCntOrbit].mtxEnd._41, g_aOrbit[nCntOrbit].mtxEnd._42, g_aOrbit[nCntOrbit].mtxEnd._43);
			pVtx[g_aOrbit[nCntOrbit].nLength * 2 - 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[g_aOrbit[nCntOrbit].nLength * 2 - 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			g_aOrbit[nCntOrbit].Start = g_aOrbit[nCntOrbit].InitStart;
			g_aOrbit[nCntOrbit].End = g_aOrbit[nCntOrbit].InitEnd;

			for (int nCntLength = 0; nCntLength < g_aOrbit[nCntOrbit].nLength; nCntLength++)
			{
				pVtx[0 + nCntLength * 2].pos = D3DXVECTOR3(g_aOrbit[nCntOrbit].mtxStart._41, g_aOrbit[nCntOrbit].mtxStart._42, g_aOrbit[nCntOrbit].mtxStart._43);
				pVtx[1 + nCntLength * 2].pos = D3DXVECTOR3(g_aOrbit[nCntOrbit].mtxEnd._41, g_aOrbit[nCntOrbit].mtxEnd._42, g_aOrbit[nCntOrbit].mtxEnd._43);
			}

		}
		// ���_�o�b�t�@���A�����b�N����
		g_aOrbit[nCntOrbit].pVtxBuff->Unlock();
	}
}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawOrbit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	Player *pPlayer = GetPlayer();
	D3DXMATRIX mtxPlayerWorld;
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// ���ʂ��J�����O																		
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);					// Z�o�b�t�@�ւ̏�������
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);						// ���C�g�e���󂯂Ȃ�
	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		if (g_aOrbit[nCntOrbit].nCharacterType == ORBIT_PLAYER)
		{
			 mtxPlayerWorld = pPlayer->aModel[g_aOrbit[nCntOrbit].nIdxParent][pPlayer->nWeaponState].mtxWorld;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aOrbit[nCntOrbit].mtxStart);
		D3DXMatrixTranslation(&g_aOrbit[nCntOrbit].mtxStart, g_aOrbit[nCntOrbit].Start.x, g_aOrbit[nCntOrbit].Start.y, g_aOrbit[nCntOrbit].Start.z);	// �X�^�[�g
		D3DXMatrixMultiply(&g_aOrbit[nCntOrbit].mtxStart, &g_aOrbit[nCntOrbit].mtxStart, &mtxPlayerWorld);	// [�n�_*�v���C���[]�̃}�g���b�N�X�v�Z
		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aOrbit[nCntOrbit].mtxStart);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aOrbit[nCntOrbit].mtxEnd);
		D3DXMatrixTranslation(&g_aOrbit[nCntOrbit].mtxEnd, g_aOrbit[nCntOrbit].End.x, g_aOrbit[nCntOrbit].End.y, g_aOrbit[nCntOrbit].End.z);	// �G���h
		D3DXMatrixMultiply(&g_aOrbit[nCntOrbit].mtxEnd, &g_aOrbit[nCntOrbit].mtxEnd, &mtxPlayerWorld);		// [�I�_*�v���C���[]�̃}�g���b�N�X�v�Z
		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aOrbit[nCntOrbit].mtxEnd);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aOrbit[nCntOrbit].mtxWorld);
		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aOrbit[nCntOrbit].mtxWorld);

		if (g_aOrbit[nCntOrbit].bUse == true)
		{	
			for (int nCntLength = 0; nCntLength < g_aOrbit[nCntOrbit].nLength + g_aOrbit[nCntOrbit].nPlus ; nCntLength++)
			{
				// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
				pDevice->SetStreamSource(0, g_aOrbit[nCntOrbit].pVtxBuff, 0, sizeof(VERTEX_3D));
				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureOrbit[g_aOrbit[nCntOrbit].nType]);
				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLength, 2);				
			}
		}
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);					// Z�o�b�t�@�ւ̏�������
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ���C�g�e���󂯂Ȃ�
	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


}

//==============================================================================================================================
// ���_���̍쐬
//==============================================================================================================================
void MakeVertexOrbit(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		// ���_���̍쐬
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2 * MAX_ORBIT_LENGTH,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_aOrbit[nCntOrbit].pVtxBuff,
			NULL);
		// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
		g_aOrbit[nCntOrbit].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT_LENGTH; nCntOrbit++, pVtx += 2)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �e�N�X�`���̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		// ���_�o�b�t�@���A�����b�N����
		g_aOrbit[nCntOrbit].pVtxBuff->Unlock();

	}
}
//==============================================================================================================================
// �O�Ղ̐ݒ�
//==============================================================================================================================
void SetOrbit(int nIdxParent,int nLength, D3DXVECTOR3 Start, D3DXVECTOR3 End, ORBITTYPE nType, PlayerMotionType nMotionType, PLAYERWEAPON nWeaponState)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
		g_aOrbit[nCntOrbit].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		if (g_aOrbit[nCntOrbit].bUse == false)
		{
			g_aOrbit[nCntOrbit].nIdxParent = nIdxParent;
			g_aOrbit[nCntOrbit].Start = Start;
			g_aOrbit[nCntOrbit].End = End;
			g_aOrbit[nCntOrbit].InitStart = Start;
			g_aOrbit[nCntOrbit].InitEnd = End;
			g_aOrbit[nCntOrbit].nCharacterType = ORBIT_PLAYER;
			g_aOrbit[nCntOrbit].nType = nType;
			g_aOrbit[nCntOrbit].nMotionType = nMotionType;
			g_aOrbit[nCntOrbit].nWeaponState = nWeaponState;
			g_aOrbit[nCntOrbit].nLength = nLength;
			if (g_aOrbit[nCntOrbit].nLength >= 5)
			{	
				g_aOrbit[nCntOrbit].nPlus = g_aOrbit[nCntOrbit].nLength - 3;
			}
			for (int nCntLength = 0; nCntLength < g_aOrbit[nCntOrbit].nLength; nCntLength++, pVtx += 2)
			{
				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aOrbit[nCntOrbit].mtxStart._41, g_aOrbit[nCntOrbit].mtxStart._42, g_aOrbit[nCntOrbit].mtxStart._43);
				pVtx[1].pos = D3DXVECTOR3(g_aOrbit[nCntOrbit].mtxEnd._41, g_aOrbit[nCntOrbit].mtxEnd._42, g_aOrbit[nCntOrbit].mtxEnd._43);
			}
			g_aOrbit[nCntOrbit].bUse = true;
			// ���_�o�b�t�@���A�����b�N����
			g_aOrbit[nCntOrbit].pVtxBuff->Unlock();

			break;

		}
	}
}
void SetPosOrbit(PlayerMotionType nMotionType, PLAYERWEAPON nWeaponState)
{
	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		if (g_aOrbit[nCntOrbit].nCharacterType == ORBIT_PLAYER)
		{
			if (g_aOrbit[nCntOrbit].nWeaponState == nWeaponState)
			{
				if (g_aOrbit[nCntOrbit].nMotionType == nMotionType)
				{
					g_aOrbit[nCntOrbit].bUse = true;

					g_aOrbit[nCntOrbit].nCntTrueTime = g_aOrbit[nCntOrbit].nLength;
					if (g_aOrbit[nCntOrbit].nMotionType == PLAYER_MOTION_TYPE_MOVE)
					{// ���������͑��߂ɐ؂�
						g_aOrbit[nCntOrbit].nCntTrueTime = 10;
					}
					// �I�[�r�b�g���L�т�

					if (g_aOrbit[nCntOrbit].InitStart.x > 0 || g_aOrbit[nCntOrbit].InitStart.x < 0)
					{
						g_aOrbit[nCntOrbit].Start.x *= 1.01f;
					}
					if (g_aOrbit[nCntOrbit].InitEnd.x > 0 || g_aOrbit[nCntOrbit].InitEnd.x < 0)
					{
						g_aOrbit[nCntOrbit].End.x *= 1.02f;
					}
					if (g_aOrbit[nCntOrbit].InitStart.y > 0 || g_aOrbit[nCntOrbit].InitStart.y < 0)
					{
						g_aOrbit[nCntOrbit].Start.y *= 1.01f;
					}
					if (g_aOrbit[nCntOrbit].InitEnd.y > 0 || g_aOrbit[nCntOrbit].InitEnd.y < 0)
					{
						g_aOrbit[nCntOrbit].End.y *= 1.02f;
					}
					if (g_aOrbit[nCntOrbit].InitStart.z > 0 || g_aOrbit[nCntOrbit].InitStart.z < 0)
					{
						g_aOrbit[nCntOrbit].Start.z *= 1.01f;
					}
					if (g_aOrbit[nCntOrbit].InitEnd.z > 0 || g_aOrbit[nCntOrbit].InitEnd.z < 0)
					{
						g_aOrbit[nCntOrbit].End.z *= 1.02f;
					}

				}
				else
				{
					g_aOrbit[nCntOrbit].Start = g_aOrbit[nCntOrbit].InitStart;
					g_aOrbit[nCntOrbit].End = g_aOrbit[nCntOrbit].InitEnd;

					g_aOrbit[nCntOrbit].nCntTrueTime--;
					if (g_aOrbit[nCntOrbit].nCntTrueTime <= 0)
					{
						g_aOrbit[nCntOrbit].nCntTrueTime = 0;
						g_aOrbit[nCntOrbit].bUse = false;
					}
				}
			}
			else
			{
				g_aOrbit[nCntOrbit].Start = g_aOrbit[nCntOrbit].InitStart;
				g_aOrbit[nCntOrbit].End = g_aOrbit[nCntOrbit].InitEnd;

				g_aOrbit[nCntOrbit].nCntTrueTime--;
				if (g_aOrbit[nCntOrbit].nCntTrueTime <= 0)
				{
					g_aOrbit[nCntOrbit].nCntTrueTime = 0;
					g_aOrbit[nCntOrbit].bUse = false;
				}
			}
		}
	}
}
