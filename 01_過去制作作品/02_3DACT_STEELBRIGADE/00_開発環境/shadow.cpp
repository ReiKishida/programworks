//=============================================================================
//
// �e���� [shadow.cpp]
// Author : KishidaRei
//
//=============================================================================
#include "shadow.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);




//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//���_�o�b�t�@�ւ̃|�C���^



SHADOW g_Shadow[MAX_SHADOW];

//=============================================================================
// ����������
//=============================================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾


	MakeVertexShadow(pDevice);



	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�e�̈ʒu
		g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�e�̉�]
		float fWidth = 0.0f;			//�e�̕�
		float fDepth = 0.0f;			//�e�̉��s��
		g_Shadow[nCntShadow].bUse = false;
	}
	//g_Shadow[0].bUse = true;
}

//=============================================================================
// �I������
//=============================================================================
void UninitShadow(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateShadow(void)
{


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxrot, mtxtrans;

	
	
		// ���Z�����̐ݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


		for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
		{

			if (g_Shadow[nCntShadow].bUse == true)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_Shadow[nCntShadow].mtxWorld);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxrot,
					g_Shadow[nCntShadow].rot.y, g_Shadow[nCntShadow].rot.x, g_Shadow[nCntShadow].rot.z);

				D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorld, &g_Shadow[nCntShadow].mtxWorld, &mtxrot);


				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxtrans,
					g_Shadow[nCntShadow].pos.x, g_Shadow[nCntShadow].pos.y, g_Shadow[nCntShadow].pos.z);

				D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorld, &g_Shadow[nCntShadow].mtxWorld, &mtxtrans);


				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_Shadow[nCntShadow].mtxWorld);

				// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
				pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);


				// �e�N�X�`���̐ݒ�
				//pDevice->SetFVF(D3DXGetFVFVertexSize);
				pDevice->SetTexture(0, g_pTextureShadow);


				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntShadow, 2);

			}
		}
		// ���Z�����̐ݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow000.jpg", &g_pTextureShadow);


	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{

		//���_���W
		pVtx[0].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fWidth, 1.0f, g_Shadow[nCntShadow].fDepth);
		pVtx[1].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fWidth, 1.0f, g_Shadow[nCntShadow].fDepth);
		pVtx[2].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fWidth, 1.0f, -g_Shadow[nCntShadow].fDepth);
		pVtx[3].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fWidth, 1.0f, -g_Shadow[nCntShadow].fDepth);

		//�@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// �e�̐ݒ�
//=============================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float Depth)
{
	int nCntShadow;
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == false)
		{
			g_Shadow[nCntShadow].pos = pos;
			g_Shadow[nCntShadow].rot = rot;

			g_Shadow[nCntShadow].fWidth = fWidth;
			g_Shadow[nCntShadow].fDepth = Depth;

			//���_���W
			pVtx[0].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fWidth, 0.1f, g_Shadow[nCntShadow].fDepth);
			pVtx[1].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fWidth, 0.1f, g_Shadow[nCntShadow].fDepth);
			pVtx[2].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fWidth, 0.1f, -g_Shadow[nCntShadow].fDepth);
			pVtx[3].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fWidth, 0.1f, -g_Shadow[nCntShadow].fDepth);

			g_Shadow[nCntShadow].bUse = true;
			break;

			
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffShadow->Unlock();
	return nCntShadow;
}

//=============================================================================
// �e�̈ʒu�̐ݒ�
//=============================================================================
void SetPositionShadow(int IndexShadow, D3DXVECTOR3 pos)
{
	g_Shadow[IndexShadow].pos.x = pos.x;
	g_Shadow[IndexShadow].pos.z = pos.z;

}
void SetPositionBulletShadow(int IndexShadow, D3DXVECTOR3 pos)
{
	g_Shadow[IndexShadow].pos.x = pos.x;
	g_Shadow[IndexShadow].pos.x = pos.y;
	g_Shadow[IndexShadow].pos.z = pos.z;

}


//=============================================================================
// �e�̈ʒu�̐ݒ�
//=============================================================================
void DeleteShadow(int IndexShadow)
{
	g_Shadow[IndexShadow].bUse = false;
}

//=============================================================================
// �e�̑傫���̐ݒ�
//=============================================================================
void SetScaleShadow(int IndexShadow)
{
	/*g_Shadow[IndexShadow].fWidth ;
	g_Shadow[IndexShadow].fDepth;*/


}
