////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �n�ʃG�t�F�N�g���� [.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "effectUP.h"
#include "game.h"
#include "effect.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_EFFECTUP	(500)
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// ���S���W
	D3DXVECTOR3 rot;			// ����
	EFFECTUPTYPE nType;			// ���
	float fWidth;				// ��
	float fDepth;				// ���s��
} EffectUPInfo;
typedef struct
{
	char *pFileName;
}EffectUPTexture;
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexEffectUP(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffectUP = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_apTextureEffectUP[EFFECTUPTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
EffectUP g_aEffectUP[MAX_EFFECTUP];
EffectUPTexture g_aEffectUPTexInfo[EFFECTUPTYPE_MAX] =
{
	{ "data/TEXTURE/mohoujin.png" },
	{ "data/TEXTURE/LeafEffect.jpg" },
	{ "data/TEXTURE/LeafEffect.jpg" },
};
float fTexmove;
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitEffectUP(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	for (int nCntEffectUPTex = 0; nCntEffectUPTex < EFFECTUPTYPE_MAX; nCntEffectUPTex++)
	{// �e�N�X�`���̐ݒ�
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			g_aEffectUPTexInfo[nCntEffectUPTex].pFileName,	// �t�@�C���̖��O
			&g_apTextureEffectUP[nCntEffectUPTex]);			// �e�N�X�`���ւ̃|�C���^
	}

	for (int nCntEffectUP = 0; nCntEffectUP < MAX_EFFECTUP; nCntEffectUP++)
	{
		g_aEffectUP[nCntEffectUP].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aEffectUP[nCntEffectUP].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffectUP[nCntEffectUP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffectUP[nCntEffectUP].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aEffectUP[nCntEffectUP].fRadius = 0;								// ���a
		g_aEffectUP[nCntEffectUP].nType = EFFECTUPTYPE_MAHOUJIN;				// ���
		g_aEffectUP[nCntEffectUP].bUse = false;							// �g�p����Ă��邩�ǂ���
	}
	fTexmove = 0;
	MakeVertexEffectUP(pDevice);
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitEffectUP(void)
{
	// �e�N�X�`���̔j��
	for (int nCntEffectUP = 0; nCntEffectUP < EFFECTUPTYPE_MAX; nCntEffectUP++)
	{
		if (g_apTextureEffectUP[nCntEffectUP] != NULL)
		{
			g_apTextureEffectUP[nCntEffectUP]->Release();
			g_apTextureEffectUP[nCntEffectUP] = NULL;
		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffectUP != NULL)
	{
		g_pVtxBuffEffectUP->Release();
		g_pVtxBuffEffectUP = NULL;
	}
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateEffectUP(void)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

						// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEffectUP->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffectUP = 0; nCntEffectUP < MAX_EFFECTUP; nCntEffectUP++, pVtx += 4)
	{
		if (g_aEffectUP[nCntEffectUP].bUse == true)
		{
			switch (g_aEffectUP[nCntEffectUP].nType)
			{
			case EFFECTUPTYPE_MAHOUJIN:
				g_aEffectUP[nCntEffectUP].col.a += -0.01f;
				if (g_aEffectUP[nCntEffectUP].col.a <= 0.0f)
				{
					g_aEffectUP[nCntEffectUP].bUse = false;
				}
				if (rand() % 3 == 0)
				{// ���т�G�t�F�N�g
					float nDirection = SetRandomEffect();
					float nDirection2 = SetRandomEffect();
					SetEffect(g_aEffectUP[nCntEffectUP].pos + D3DXVECTOR3(sinf(nDirection) * (float(rand() % 400)), cosf(rand() % 314 / 100.0f) * (float(rand() % 400)), cosf(nDirection2) * (float(rand() % 400))),
						D3DXVECTOR3(sinf(nDirection) * float((rand() % 10 / 10.0f)), cosf(nDirection) *  float((rand() % 10 / 10.0f)), cosf(nDirection2) *  float((rand() % 10 / 10.0f))), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), float(rand() % 200) + 1, 10, EFFECTTYPE_SUNDER1);
					SetEffect(g_aEffectUP[nCntEffectUP].pos + D3DXVECTOR3(sinf(nDirection) * (float(rand() % 400)), cosf(rand() % 314 / 100.0f) * (float(rand() % 400)), cosf(nDirection2) * (float(rand() % 400))),
						D3DXVECTOR3(sinf(nDirection) *  float((rand() % 10 / 10.0f)), sinf(nDirection2) *  float((rand() % 10 / 10.0f)), cosf(nDirection) *  float((rand() % 10 / 10.0f))), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(0.1f, 0.1f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 200) + 1, 10, EFFECTTYPE_SUNDER1);
				}
				break;
			case EFFECTUPTYPE_LEAF:
				g_aEffectUP[nCntEffectUP].fRadius += 10;
				g_aEffectUP[nCntEffectUP].col.a += -0.02f;
				g_aEffectUP[nCntEffectUP].rot.y += 0.1f;
				if (g_aEffectUP[nCntEffectUP].col.a <= 0.0f)
				{
					g_aEffectUP[nCntEffectUP].bUse = false;
				}
				break;
			case EFFECTUPTYPE_HURRICANE:
				g_aEffectUP[nCntEffectUP].fRadius += 5;
				g_aEffectUP[nCntEffectUP].col.a += -0.02f;
				g_aEffectUP[nCntEffectUP].rot.y += 0.3f;
				if (g_aEffectUP[nCntEffectUP].col.a <= 0.0f)
				{
					g_aEffectUP[nCntEffectUP].bUse = false;
				}
				break;
			}
			g_aEffectUP[nCntEffectUP].pos += g_aEffectUP[nCntEffectUP].move;
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, (g_aEffectUP[nCntEffectUP].fRadius / 2));
			pVtx[1].pos = D3DXVECTOR3((g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, (g_aEffectUP[nCntEffectUP].fRadius / 2));
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, -(g_aEffectUP[nCntEffectUP].fRadius / 2));
			pVtx[3].pos = D3DXVECTOR3((g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, -(g_aEffectUP[nCntEffectUP].fRadius / 2));
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffectUP[nCntEffectUP].col;
			pVtx[1].col = g_aEffectUP[nCntEffectUP].col;
			pVtx[2].col = g_aEffectUP[nCntEffectUP].col;
			pVtx[3].col = g_aEffectUP[nCntEffectUP].col;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffectUP->Unlock();

}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawEffectUP(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

												// ���e�X�g��ݒ�
												//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g��L����
												//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���l��0
												//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���傫��
												// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// ���C�g�e���󂯂Ȃ�
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// Z�o�b�t�@�ւ̏�������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntEffectUP = 0; nCntEffectUP < MAX_EFFECTUP; nCntEffectUP++)
	{
		if (g_aEffectUP[nCntEffectUP].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffectUP[nCntEffectUP].mtxWorld);
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEffectUP[nCntEffectUP].rot.y, g_aEffectUP[nCntEffectUP].rot.x, g_aEffectUP[nCntEffectUP].rot.z);
			D3DXMatrixMultiply(&g_aEffectUP[nCntEffectUP].mtxWorld, &g_aEffectUP[nCntEffectUP].mtxWorld, &mtxRot);
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffectUP[nCntEffectUP].pos.x, g_aEffectUP[nCntEffectUP].pos.y, g_aEffectUP[nCntEffectUP].pos.z);
			D3DXMatrixMultiply(&g_aEffectUP[nCntEffectUP].mtxWorld, &g_aEffectUP[nCntEffectUP].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffectUP[nCntEffectUP].mtxWorld);
			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffectUP, 0, sizeof(VERTEX_3D));
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEffectUP[g_aEffectUP[nCntEffectUP].nType]);
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffectUP * 4, 2);

		}
	}

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// Z�\�[�X
	//	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// Z�o�b�t�@�ւ̏�������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���e�X�g�����ɖ߂�
	//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�𖳌���

}

//==============================================================================================================================
// ���_���̍쐬
//==============================================================================================================================
void MakeVertexEffectUP(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

						// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECTUP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffectUP,
		NULL);

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEffectUP->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntEffectUP = 0; nCntEffectUP < MAX_EFFECTUP; nCntEffectUP++, pVtx += 4)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, (g_aEffectUP[nCntEffectUP].fRadius / 2));
		pVtx[1].pos = D3DXVECTOR3((g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, (g_aEffectUP[nCntEffectUP].fRadius / 2));
		pVtx[2].pos = D3DXVECTOR3(-(g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, -(g_aEffectUP[nCntEffectUP].fRadius / 2));
		pVtx[3].pos = D3DXVECTOR3((g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, -(g_aEffectUP[nCntEffectUP].fRadius / 2));
		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffectUP->Unlock();

}
//==============================================================================================================================
// �|���S���̐ݒ�
//==============================================================================================================================
void SetEffectUP(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, EFFECTUPTYPE nType, float fRadius)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

						// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEffectUP->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffectUP = 0; nCntEffectUP < MAX_EFFECTUP; nCntEffectUP++, pVtx += 4)
	{
		if (g_aEffectUP[nCntEffectUP].bUse == false)
		{
			g_aEffectUP[nCntEffectUP].pos = pos;
			g_aEffectUP[nCntEffectUP].move = move;
			g_aEffectUP[nCntEffectUP].rot = rot;
			g_aEffectUP[nCntEffectUP].col = col;
			g_aEffectUP[nCntEffectUP].nType = nType;
			g_aEffectUP[nCntEffectUP].fRadius = fRadius;
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, (g_aEffectUP[nCntEffectUP].fRadius / 2));
			pVtx[1].pos = D3DXVECTOR3((g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, (g_aEffectUP[nCntEffectUP].fRadius / 2));
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, -(g_aEffectUP[nCntEffectUP].fRadius / 2));
			pVtx[3].pos = D3DXVECTOR3((g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, -(g_aEffectUP[nCntEffectUP].fRadius / 2));
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffectUP[nCntEffectUP].col;
			pVtx[1].col = g_aEffectUP[nCntEffectUP].col;
			pVtx[2].col = g_aEffectUP[nCntEffectUP].col;
			pVtx[3].col = g_aEffectUP[nCntEffectUP].col;
			g_aEffectUP[nCntEffectUP].bUse = true;
			break;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffectUP->Unlock();
}