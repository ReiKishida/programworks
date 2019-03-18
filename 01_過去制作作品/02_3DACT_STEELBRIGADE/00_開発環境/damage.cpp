//=============================================================================
//
// ��e���� [Damage.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "damage.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "particle.h"
#include "player.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_DAMAGETYPE (4)

#define POLYGON_WIDTH (10.0f)
#define POLYGON_HEIGHT (20.0f)

#define ROT_SPEED (0.05f)

#define RADIUS (45.0f)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexDamage(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

DAMAGE g_Damage;


//int nCnt = 0;

//int g_nNowSelect;

//int g_nCntCoolDown;


//=============================================================================
// ����������
//=============================================================================
void InitDamage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/damage.png", &g_Damage.aPolygon[nCntDamage].pTexture);
	}

	MakeVertexDamage(pDevice);

	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	CAMERA *pCamera;
	pCamera = GetCamera();


	g_Damage.bUse = true;
	g_Damage.pos.y = 10.0f + pPlayer.pos.y;

	g_Damage.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		g_Damage.aPolygon[nCntDamage].nType = nCntDamage;

		g_Damage.aPolygon[nCntDamage].nIdxModelParent = -1;

	}

	g_Damage.aPolygon[0].fpos = 1.0f;
	g_Damage.aPolygon[1].fpos = -0.5f;
	g_Damage.aPolygon[2].fpos = 0.0f;
	g_Damage.aPolygon[3].fpos = 0.5f;

	//g_nCntCoolDown = 0;
	g_Damage.status = DAMAGESTATUS_NONE;
	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		g_Damage.aPolygon[nCntDamage].pos.x = sinf(g_Damage.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_DAMAGE) * nCntDamage) * RADIUS + g_Damage.pos.x;
		g_Damage.aPolygon[nCntDamage].pos.y = 0.0f + g_Damage.pos.y;
		g_Damage.aPolygon[nCntDamage].pos.z = cosf(g_Damage.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_DAMAGE) * nCntDamage) * RADIUS + g_Damage.pos.z;

		g_Damage.aPolygon[nCntDamage].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}


	g_Damage.nIdxModelParent = -1;
}

//=============================================================================
// �I������
//=============================================================================
void UninitDamage(void)
{
	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{

			//�e�N�X�`���̔j��
			if (g_Damage.aPolygon[nCntDamage].pTexture != NULL)
			{
				g_Damage.aPolygon[nCntDamage].pTexture->Release();
				g_Damage.aPolygon[nCntDamage].pTexture = NULL;
			}


		// ���_�o�b�t�@�̊J��
		if (g_Damage.aPolygon[nCntDamage].pVtxBuff != NULL)
		{
			g_Damage.aPolygon[nCntDamage].pVtxBuff->Release();
			g_Damage.aPolygon[nCntDamage].pVtxBuff = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateDamage(void)
{
	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	CAMERA *pCamera;
	pCamera = GetCamera();

	g_Damage.pos.x = pPlayer.pos.x;
	g_Damage.pos.y = 10.0f + pPlayer.pos.y;
	g_Damage.pos.z = pPlayer.pos.z;


	/*if (GetKeyboardTrigger(DIK_0) == true)
	{

	g_Damage.bUse = false;

	}
	if (GetKeyboardTrigger(DIK_O) == true)
	{

	g_Damage.bUse = true;

	}*/

	//g_Damage.fAngle = g_Damage.fDestAngle - g_Damage.rot.y;

	/*if (g_Damage.fAngle > D3DX_PI)
	{
	g_Damage.fAngle -= D3DX_PI * 2 ;
	}

	if (g_Damage.fAngle < -D3DX_PI)
	{
	g_Damage.fAngle += D3DX_PI * 2;
	}*/
	//g_Damage.rot.y += g_Damage.fAngle * ROT_SPEED;
	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{

		if (g_Damage.bUse == true && g_Damage.bUse != false)
		{
			if (g_Damage.status == DAMAGESTATUS_NONE)
			{
				switch (g_Damage.state)
				{
				case DAMAGESTATE_NONE:
					g_Damage.aPolygon[nCntDamage].col.a -= 0.03f;
					if (g_Damage.aPolygon[nCntDamage].col.a <= 0.1f)
					{
						g_Damage.aPolygon[nCntDamage].col.a = 0.1f;
						g_Damage.state = DAMAGESTATE_USE;
					}
					break;

				case DAMAGESTATE_USE:
					g_Damage.aPolygon[nCntDamage].col.a += 0.03f;
					if (g_Damage.aPolygon[nCntDamage].col.a >= 1.0f)
					{
						g_Damage.aPolygon[nCntDamage].col.a = 1.0f;
						g_Damage.state = DAMAGESTATE_NONE;
					}
					break;
				}
			}

			g_Damage.rot.y -= 0.01f;
			g_Damage.nLife--;
			if (g_Damage.nLife <= 0)
			{
				g_Damage.bUse = false;
			}
		}
	}
	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		g_Damage.aPolygon[nCntDamage].pos.x = sinf(g_Damage.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_DAMAGE) * nCntDamage) * RADIUS + g_Damage.pos.x;
		g_Damage.aPolygon[nCntDamage].pos.y = 0.0f + g_Damage.pos.y;
		g_Damage.aPolygon[nCntDamage].pos.z = cosf(g_Damage.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_DAMAGE) * nCntDamage) * RADIUS + g_Damage.pos.z;
	}


	if (g_Damage.rot.y > D3DX_PI)
	{
		g_Damage.rot.y -= D3DX_PI * 2;
	}

	if (g_Damage.rot.y < -D3DX_PI)
	{
		g_Damage.rot.y += D3DX_PI * 2;
	}

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{

		//���_�o�b�t�@�����b�N
		g_Damage.aPolygon[nCntDamage].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);




			//���_�J���[
			pVtx[0].col = g_Damage.aPolygon[nCntDamage].col;
			pVtx[1].col = g_Damage.aPolygon[nCntDamage].col;
			pVtx[2].col = g_Damage.aPolygon[nCntDamage].col;
			pVtx[3].col = g_Damage.aPolygon[nCntDamage].col;


			pVtx += 4;


		//���_�o�b�t�@���A�����b�N
		g_Damage.aPolygon[nCntDamage].pVtxBuff->Unlock();
	}
}





//=============================================================================
// �`�揈��
//=============================================================================
void DrawDamage(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxrot, mtxtrans;


	D3DXMATRIX mtxParent;

	PLAYER pPlayer;
	pPlayer = *GetPlayer();


	CAMERA *pCamera;
	pCamera = GetCamera();


	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		if (g_Damage.nIdxModelParent == -1)
		{//��
			mtxParent = pPlayer.mtxWorld;
		}

		//========================//
		//�v���C���[�̃C���f�b�N�X//
		//========================//
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Damage.mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxrot,
			g_Damage.rot.y, g_Damage.rot.x, g_Damage.rot.z);
		D3DXMatrixMultiply(&g_Damage.mtxWorld, &g_Damage.mtxWorld, &mtxrot);


		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxtrans,
			g_Damage.pos.x, g_Damage.pos.y, g_Damage.pos.z);
		D3DXMatrixMultiply(&g_Damage.mtxWorld, &g_Damage.mtxWorld, &mtxtrans);


		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Damage.mtxWorld);



		for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
		{
			if (g_Damage.bUse == true && g_Damage.bUse != false)
			{

				if (g_Damage.aPolygon[nCntDamage].nIdxModelParent == -1)
				{//��
					mtxParent = g_Damage.mtxWorld;
				}

				//=======================================================================//
				//						    �e���f���̃C���f�b�N�X						//
				//=====================================================================//
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_Damage.aPolygon[nCntDamage].mtxWorld);

				// ��]�𔽉f
				/*D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_Damage.aPolygon[nCntDamage].rot.y, g_Damage.aPolygon[nCntDamage].rot.x, g_Damage.aPolygon[nCntDamage].rot.z);
				D3DXMatrixMultiply(&g_Damage.aPolygon[nCntDamage].mtxWorld, &g_Damage.aPolygon[nCntDamage].mtxWorld, &mtxrot);*/


				pDevice->GetTransform(D3DTS_VIEW, &pCamera->mtxView);

				g_Damage.aPolygon[nCntDamage].mtxWorld._11 = pCamera->mtxView._11;
				g_Damage.aPolygon[nCntDamage].mtxWorld._12 = pCamera->mtxView._21;
				g_Damage.aPolygon[nCntDamage].mtxWorld._13 = pCamera->mtxView._31;

				g_Damage.aPolygon[nCntDamage].mtxWorld._21 = pCamera->mtxView._12;
				g_Damage.aPolygon[nCntDamage].mtxWorld._22 = pCamera->mtxView._22;
				g_Damage.aPolygon[nCntDamage].mtxWorld._23 = pCamera->mtxView._32;

				g_Damage.aPolygon[nCntDamage].mtxWorld._31 = pCamera->mtxView._13;
				g_Damage.aPolygon[nCntDamage].mtxWorld._32 = pCamera->mtxView._23;
				g_Damage.aPolygon[nCntDamage].mtxWorld._33 = pCamera->mtxView._33;

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxtrans,
					g_Damage.aPolygon[nCntDamage].pos.x, g_Damage.aPolygon[nCntDamage].pos.y, g_Damage.aPolygon[nCntDamage].pos.z);
				D3DXMatrixMultiply(&g_Damage.aPolygon[nCntDamage].mtxWorld, &g_Damage.aPolygon[nCntDamage].mtxWorld, &mtxtrans);

				//�e�̃}�g���b�N�X���f
				//D3DXMatrixMultiply(&g_Damage.aPolygon[nCntDamage].mtxWorld, &g_Damage.aPolygon[nCntDamage].mtxWorld, &mtxParent);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_Damage.aPolygon[nCntDamage].mtxWorld);

				// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
				pDevice->SetStreamSource(0, g_Damage.aPolygon[nCntDamage].pVtxBuff, 0, sizeof(VERTEX_3D));

				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_Damage.aPolygon[nCntDamage].pTexture);

				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDamage * 4, 2);
			}
		}
	}


}
//=============================================================================
// �R�[���I��
//=============================================================================
void HitDamage(void)
{
	g_Damage.bUse = true;
	g_Damage.nLife = 480;
}
//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexDamage(LPDIRECT3DDEVICE9 pDevice)
{

	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{

		// ���_���̍쐬
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_DAMAGE,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_Damage.aPolygon[nCntDamage].pVtxBuff,
			NULL);

		// ���_���̐ݒ�
		VERTEX_3D *pVtx;


		//���_�o�b�t�@�����b�N
		g_Damage.aPolygon[nCntDamage].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCntDamage2 = 0; nCntDamage2 < MAX_DAMAGE; nCntDamage2++)
		{

			//���_���W
			pVtx[0].pos = D3DXVECTOR3(-POLYGON_WIDTH, POLYGON_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(POLYGON_WIDTH, POLYGON_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-POLYGON_WIDTH, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(POLYGON_WIDTH, 0.0f, 0.0f);

			//�@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += 4;
		}

		//���_�o�b�t�@���A�����b�N
		g_Damage.aPolygon[nCntDamage].pVtxBuff->Unlock();
	}

}


