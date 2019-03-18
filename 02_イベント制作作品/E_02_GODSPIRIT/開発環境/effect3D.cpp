////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �G�t�F�N�g3D���� [effct3D.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "effect3D.h"
#include "player.h"
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	char *pFileName;
}Effect3DInfo;
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_EFFECT3D (100)
#define MAX_SPEED	(1.0f)	// �ړ���
#define DOWN_SPEED	(0.15f)	// ������
#define MAX_HEIGHT	(100)	// ��̏��
#define MIN_HEIGHT	(-50)	// ���̏��

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
Effect3D g_aEffect3D[MAX_EFFECT3D];
Effect3DData g_aEffect3DData[EFFECT3D_MAX];
Effect3DInfo g_aEffect3DFilename[EFFECT3D_MAX]=
{
	{ "data/MODEL/hanma�Qefect.x" },
	{ "data/MODEL/handEffectLong.x" },
	{"data/MODEL/sordEffect.x"},
};
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitEffect3D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �ʒu�E�����̏����ݒ�
	for (int nCntMat = 0; nCntMat < MAX_EFFECT3D; nCntMat++)
	{
		g_aEffect3D[nCntMat].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect3D[nCntMat].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect3D[nCntMat].Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect3D[nCntMat].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect3D[nCntMat].nType = EFFECT3D_ARM;
		g_aEffect3D[nCntMat].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect3D[nCntMat].Diffrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect3D[nCntMat].Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect3D[nCntMat].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect3D[nCntMat].bCollision = true;
		g_aEffect3D[nCntMat].nIdy = 0;
		g_aEffect3D[nCntMat].bUse = false;
	}	
	for (int nCntMat = 0; nCntMat < EFFECT3D_MAX; nCntMat++)
	{
		// ���f���f�[�^�̓ǂݍ���
		g_aEffect3DData[nCntMat].pMesh = NULL;
		g_aEffect3DData[nCntMat].pBuffMat = NULL;
		g_aEffect3DData[nCntMat].nNumMat = 0;

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(g_aEffect3DFilename[nCntMat].pFileName,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aEffect3DData[nCntMat].pBuffMat,
			NULL,
			&g_aEffect3DData[nCntMat].nNumMat,
			&g_aEffect3DData[nCntMat].pMesh);
		D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aEffect3DData[nCntMat].pBuffMat->GetBufferPointer();
		// �e�N�X�`���̔z��ݒ�
		g_aEffect3DData[nCntMat].pTextuer = new LPDIRECT3DTEXTURE9[(int)g_aEffect3DData[nCntMat].nNumMat];

		// �e�N�X�`���̓ǂݍ���
		for (int nCntMatTex = 0; nCntMatTex < (int)g_aEffect3DData[nCntMat].nNumMat; nCntMatTex++)
		{
			g_aEffect3DData[nCntMat].pTextuer[nCntMatTex] = NULL;

			if (pMat[nCntMatTex].pTextureFilename != NULL)
			{
				// �e�N�X�`���̐ݒ�
				D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
					pMat[nCntMatTex].pTextureFilename,	// �t�@�C���̖��O
					&g_aEffect3DData[nCntMat].pTextuer[nCntMatTex]);		// �e�N�X�`���ւ̃|�C���^
			}
		}
	}
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitEffect3D(void)
{
	for (int nCntMat = 0; nCntMat < EFFECT3D_MAX; nCntMat++)
	{
		// ���b�V���̊J��
		if (g_aEffect3DData[nCntMat].pMesh != NULL)
		{
			g_aEffect3DData[nCntMat].pMesh->Release();
			g_aEffect3DData[nCntMat].pMesh = NULL;
		}
		// �}�e���A���̊J��
		if (g_aEffect3DData[nCntMat].pBuffMat != NULL)
		{
			g_aEffect3DData[nCntMat].pBuffMat->Release();
			g_aEffect3DData[nCntMat].pBuffMat = NULL;
		}
		// �e�N�X�`���̊J��
		for (int nCntMatTex = 0; nCntMatTex < int(g_aEffect3DData[nCntMat].nNumMat); nCntMatTex++)
		{
			if (g_aEffect3DData[nCntMat].pTextuer[nCntMatTex] != NULL)
			{
				g_aEffect3DData[nCntMat].pTextuer[nCntMatTex]->Release();
				g_aEffect3DData[nCntMat].pTextuer[nCntMatTex] = NULL;
			}
		}
	}

}
//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateEffect3D(void)
{
	Player *pPlayer = GetPlayer();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;				// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntMat = 0; nCntMat < MAX_EFFECT3D; nCntMat++)
	{
		if (g_aEffect3D[nCntMat].bUse == true)
		{
			switch (g_aEffect3D[nCntMat].nType)
			{
			case EFFECT3D_ARM:	// �p���`�G�t�F�N�g
				if (pPlayer->nMotionType == PLAYER_MOTION_TYPE_ATTACK_BIG&&pPlayer->nWeaponState == PLAYERWEAPON_HAMMER)
				{
					g_aEffect3D[nCntMat].pos = D3DXVECTOR3(pPlayer->aModel[17][pPlayer->nWeaponState].mtxWorld._41, pPlayer->aModel[17][pPlayer->nWeaponState].mtxWorld._42, pPlayer->aModel[17][pPlayer->nWeaponState].mtxWorld._43);
					g_aEffect3D[nCntMat].Destrot.x = pPlayer->aModel[6][pPlayer->nWeaponState].rot.x;
				}
				else
				{
					g_aEffect3D[nCntMat].Destrot.x = 0.0f;
					// ���݂̃}�e���A�����擾
					pDevice->GetMaterial(&matDef);
					// �}�e���A���f�[�^�ւ̃|�C���^���擾
					pMat = (D3DXMATERIAL*)g_aEffect3DData[g_aEffect3D[nCntMat].nType].pBuffMat->GetBufferPointer();
					for (int nCntMat2 = 0; nCntMat2 < (int)g_aEffect3DData[g_aEffect3D[nCntMat].nType].nNumMat; nCntMat2++)
					{
						pMat[nCntMat2].MatD3D.Diffuse.a -= 0.03f;
						if (pMat[nCntMat2].MatD3D.Diffuse.a <= 0.0f)
						{
							g_aEffect3D[nCntMat].bUse = false;
						}
						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat2].MatD3D);
					}
					// �}�e���A�����f�t�H���g�ɖ߂�
					pDevice->SetMaterial(&matDef);
				}
				break;
			case EFFECT3D_ARM2:	// �����O�p���`�G�t�F�N�g
					g_aEffect3D[nCntMat].pos += g_aEffect3D[nCntMat].move;
					g_aEffect3D[nCntMat].move.y *= 0.9f;
					// ���݂̃}�e���A�����擾
					pDevice->GetMaterial(&matDef);
					// �}�e���A���f�[�^�ւ̃|�C���^���擾
					pMat = (D3DXMATERIAL*)g_aEffect3DData[g_aEffect3D[nCntMat].nType].pBuffMat->GetBufferPointer();
					for (int nCntMat2 = 0; nCntMat2 < (int)g_aEffect3DData[g_aEffect3D[nCntMat].nType].nNumMat; nCntMat2++)
					{
						pMat[nCntMat2].MatD3D.Diffuse.a -= 0.001f;

						if (pMat[nCntMat2].MatD3D.Diffuse.a <= 0.0f)
						{
							g_aEffect3D[nCntMat].bUse = false;
						}
						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat2].MatD3D);
					}
					// �}�e���A�����f�t�H���g�ɖ߂�
					pDevice->SetMaterial(&matDef);
				
				break;
			case EFFECT3D_SORD:	// ���G�t�F�N�g
				if (pPlayer->nMotionType == PLAYER_MOTION_TYPE_ATTACK_BIG&&pPlayer->nWeaponState == PLAYERWEAPON_HAND) 
				{
					g_aEffect3D[nCntMat].pos += g_aEffect3D[nCntMat].move;
					g_aEffect3D[nCntMat].Destrot.y += 0.1f;
					g_aEffect3D[nCntMat].move.y *= 0.9f;

				}
				else
				{
					// ���݂̃}�e���A�����擾
					pDevice->GetMaterial(&matDef);
					// �}�e���A���f�[�^�ւ̃|�C���^���擾
					pMat = (D3DXMATERIAL*)g_aEffect3DData[g_aEffect3D[nCntMat].nType].pBuffMat->GetBufferPointer();
					for (int nCntMat2 = 0; nCntMat2 < (int)g_aEffect3DData[g_aEffect3D[nCntMat].nType].nNumMat; nCntMat2++)
					{
						pMat[nCntMat2].MatD3D.Diffuse.a -= 0.01f;
						if (pMat[nCntMat2].MatD3D.Diffuse.a <= 0.0f)
						{
							g_aEffect3D[nCntMat].bUse = false;
						}
						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat2].MatD3D);
					}
					// �}�e���A�����f�t�H���g�ɖ߂�
					pDevice->SetMaterial(&matDef);
				}
				break;
			}
			// X�̍���
			if (g_aEffect3D[nCntMat].Destrot.x > D3DX_PI)
			{// ������3.14�𒴂���Ȃ�
				g_aEffect3D[nCntMat].Destrot.x -= D3DX_PI * 2.0f;
			}
			if (g_aEffect3D[nCntMat].Destrot.x < -D3DX_PI)
			{// ������-3.14�𒴂���Ȃ�
				g_aEffect3D[nCntMat].Destrot.x += D3DX_PI * 2.0f;
			}
			g_aEffect3D[nCntMat].Diffrot.x = g_aEffect3D[nCntMat].Destrot.x - g_aEffect3D[nCntMat].rot.x;
			if (g_aEffect3D[nCntMat].Diffrot.x > D3DX_PI)
			{// ������3.14�𒴂���Ȃ�
				g_aEffect3D[nCntMat].Diffrot.x -= D3DX_PI * 2.0f;
			}
			if (g_aEffect3D[nCntMat].Diffrot.x < -D3DX_PI)
			{// ������-3.14�𒴂���Ȃ�
				g_aEffect3D[nCntMat].Diffrot.x += D3DX_PI * 2.0f;
			}
			g_aEffect3D[nCntMat].rot.x += g_aEffect3D[nCntMat].Diffrot.x * 0.3f;
			if (g_aEffect3D[nCntMat].rot.x > D3DX_PI)
			{// ������3.14�𒴂���Ȃ�
				g_aEffect3D[nCntMat].rot.x -= D3DX_PI * 2.0f;
			}
			if (g_aEffect3D[nCntMat].rot.x < -D3DX_PI)
			{// ������-3.14�𒴂���Ȃ�
				g_aEffect3D[nCntMat].rot.x += D3DX_PI * 2.0f;
			}
			// �x�̍���
			if (g_aEffect3D[nCntMat].Destrot.y > D3DX_PI)
			{// ������3.14�𒴂���Ȃ�
				g_aEffect3D[nCntMat].Destrot.y -= D3DX_PI * 2.0f;
			}
			if (g_aEffect3D[nCntMat].Destrot.y < -D3DX_PI)
			{// ������-3.14�𒴂���Ȃ�
				g_aEffect3D[nCntMat].Destrot.y += D3DX_PI * 2.0f;
			}
			g_aEffect3D[nCntMat].Diffrot.y = g_aEffect3D[nCntMat].Destrot.y - g_aEffect3D[nCntMat].rot.y;
			if (g_aEffect3D[nCntMat].Diffrot.y > D3DX_PI)
			{// ������3.14�𒴂���Ȃ�
				g_aEffect3D[nCntMat].Diffrot.y -= D3DX_PI * 2.0f;
			}
			if (g_aEffect3D[nCntMat].Diffrot.y < -D3DX_PI)
			{// ������-3.14�𒴂���Ȃ�
				g_aEffect3D[nCntMat].Diffrot.y += D3DX_PI * 2.0f;
			}
			g_aEffect3D[nCntMat].rot.y += g_aEffect3D[nCntMat].Diffrot.y * 0.3f;
			if (g_aEffect3D[nCntMat].rot.y > D3DX_PI)
			{// ������3.14�𒴂���Ȃ�
				g_aEffect3D[nCntMat].rot.y -= D3DX_PI * 2.0f;
			}
			if (g_aEffect3D[nCntMat].rot.y < -D3DX_PI)
			{// ������-3.14�𒴂���Ȃ�
				g_aEffect3D[nCntMat].rot.y += D3DX_PI * 2.0f;
			}
		}
	}

}
//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawEffect3D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;				// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^

	// ���e�X�g��ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���l��0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���傫��
																// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Z�o�b�t�@�ւ̏�������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntMat = 0; nCntMat < MAX_EFFECT3D; nCntMat++)
	{
		if (g_aEffect3D[nCntMat].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect3D[nCntMat].mtxWorld);
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEffect3D[nCntMat].rot.y, g_aEffect3D[nCntMat].rot.x, g_aEffect3D[nCntMat].rot.z);
			D3DXMatrixMultiply(&g_aEffect3D[nCntMat].mtxWorld, &g_aEffect3D[nCntMat].mtxWorld, &mtxRot);
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect3D[nCntMat].pos.x, g_aEffect3D[nCntMat].pos.y, g_aEffect3D[nCntMat].pos.z);
			D3DXMatrixMultiply(&g_aEffect3D[nCntMat].mtxWorld, &g_aEffect3D[nCntMat].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect3D[nCntMat].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEffect3DData[g_aEffect3D[nCntMat].nType].pBuffMat->GetBufferPointer();

			for (int nCntMat2 = 0; nCntMat2 < (int)g_aEffect3DData[g_aEffect3D[nCntMat].nType].nNumMat; nCntMat2++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat2].MatD3D);
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aEffect3DData[g_aEffect3D[nCntMat].nType].pTextuer[nCntMat2]);			
				// ���f��(�p�[�c)�̕`��
				g_aEffect3DData[g_aEffect3D[nCntMat].nType].pMesh->DrawSubset(nCntMat2);
				// �}�e���A�����f�t�H���g�ɖ߂�
				pDevice->SetMaterial(&matDef);
			}		
		}	
	}

	// ���e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�𖳌���
																// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// Z�o�b�t�@�ւ̏�������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//==============================================================================================================================
// ���f���̐ݒ�
//==============================================================================================================================
void SetEffect3D(D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXVECTOR3 rot, EFFECT3DTYPE nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;				// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^
	Player *pPlayer = GetPlayer();

	for (int nCntMat = 0; nCntMat < MAX_EFFECT3D; nCntMat++)
	{
		if (g_aEffect3D[nCntMat].bUse == false)
		{
			g_aEffect3D[nCntMat].pos = pos;
			g_aEffect3D[nCntMat].rot = rot;
			g_aEffect3D[nCntMat].Destrot = rot;
			g_aEffect3D[nCntMat].move = move;
			g_aEffect3D[nCntMat].nType = nType;
			g_aEffect3D[nCntMat].nIdy = nCntMat;

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);
			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEffect3DData[g_aEffect3D[nCntMat].nType].pBuffMat->GetBufferPointer();
			for (int nCntMat2 = 0; nCntMat2 < (int)g_aEffect3DData[g_aEffect3D[nCntMat].nType].nNumMat; nCntMat2++)
			{
				pMat[nCntMat2].MatD3D.Diffuse.a = 1.0f;
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat2].MatD3D);
			}
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
			if (g_aEffect3D[nCntMat].nType == EFFECT3D_ARM2)
			{
				g_aEffect3D[nCntMat].Destrot.y = float(atan2(g_aEffect3D[nCntMat].pos.x - pPlayer->pos.x, g_aEffect3D[nCntMat].pos.z - pPlayer->pos.z));
			}
			g_aEffect3D[nCntMat].bUse = true;
			break;
		}
	}
}
//==============================================================================================================================
// ���f���̎擾
//==============================================================================================================================
Effect3D *GetEffect3D(void)
{
	return &g_aEffect3D[0];
}
Effect3DData *GetEffect3DData(void)
{
	return &g_aEffect3DData[0];
}
