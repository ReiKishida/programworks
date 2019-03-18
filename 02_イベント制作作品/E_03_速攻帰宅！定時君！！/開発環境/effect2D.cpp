////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �G�t�F�N�g�̏��� [effect2D.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "effect2D.h"
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	char *pFileName;
}Effect2DTexture;
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define ANIM				(0.25f)	// �A�j���[�V����
#define LENGTH				(0.5f)	// ����
#define PATTERN				(4)		// �p�^�[��
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexEffect2D(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// �O���[�o���ϐ��錾
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureEffect2D[EFFECTTYPE_2D_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect2D = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
EFFECT2D				g_aEffect2D[MAX_EFFECT_2D];		// �G�t�F�N�g�̏��
int						g_nRandType;				// �����_���G�t�F�N�g
Effect2DTexture g_aEffect2DTex[EFFECTTYPE_2D_MAX] =
{
	{ "data/TEXTURE/SpeedEffect.png" },	// ��


};
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitEffect2D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT_2D; nCntEffect2D++)
	{
		g_aEffect2D[nCntEffect2D].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �G�t�F�N�g�̒��S���W
		g_aEffect2D[nCntEffect2D].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �G�t�F�N�g�̈ړ���
		g_aEffect2D[nCntEffect2D].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �G�t�F�N�g�̐F
		g_aEffect2D[nCntEffect2D].nCounterAnim = 0;
		g_aEffect2D[nCntEffect2D].nPatternAnim = 0;
		g_aEffect2D[nCntEffect2D].fWidth = 0;	// ���a
		g_aEffect2D[nCntEffect2D].fHeight = 0;	// ���a
		g_aEffect2D[nCntEffect2D].nLife = 0;								// �G�t�F�N�g�̎���
		g_aEffect2D[nCntEffect2D].nType = EFFECTTYPE_2D_MAX;					// �G�t�F�N�g�̎��
		g_aEffect2D[nCntEffect2D].bUse = false;								// �G�t�F�N�g���g�p����Ă��邩�ǂ���
	}
	g_nRandType = 0;													// �����_���ȃG�t�F�N�g
	// �e�N�X�`���̓ǂݍ���
	for (int nTex = 0; nTex < EFFECTTYPE_2D_MAX; nTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aEffect2DTex[nTex].pFileName,
			&g_pTextureEffect2D[nTex]);
	}
	// ���_���̍쐬
	MakeVertexEffect2D(pDevice);
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitEffect2D(void)
{
	// �e�N�X�`���̊J��
	for(int nCnt = 0;nCnt < EFFECTTYPE_2D_MAX;nCnt++)
	if (g_pTextureEffect2D[nCnt] != NULL)
	{
		g_pTextureEffect2D[nCnt]->Release();
		g_pTextureEffect2D[nCnt] = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect2D != NULL)
	{
		g_pVtxBuffEffect2D->Release();
		g_pVtxBuffEffect2D = NULL;
	}
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateEffect2D(void)
{
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT_2D; nCntEffect2D++, pVtx += 4)
	{
		if (g_aEffect2D[nCntEffect2D].bUse == true)
		{// �G�t�F�N�g���g�p����Ă���
			g_aEffect2D[nCntEffect2D].pos += g_aEffect2D[nCntEffect2D].move;			// �ړ��ʉ��Z
			if (g_aEffect2D[nCntEffect2D].fWidth <= 0)
			{// ���]�h�~
				g_aEffect2D[nCntEffect2D].fWidth = 0;
				g_aEffect2D[nCntEffect2D].bUse = false;
			}
			if (g_aEffect2D[nCntEffect2D].nLife <= 0)
			{// �������Ȃ��Ȃ�����false�ɂ���
				g_aEffect2D[nCntEffect2D].bUse = false;
			}

			switch (g_aEffect2D[nCntEffect2D].nType)
			{
			case EFFECTTYPE_2D_SPEED:	// ���傤�ǂ̃G�t�F�N�g
				break;
			default:
				if (g_aEffect2D[nCntEffect2D].col.a <= 0.0f)
				{// �F���Ȃ��Ȃ�����false�ɂ���
					g_aEffect2D[nCntEffect2D].bUse = false;
				}
				break;
			}
			g_aEffect2D[nCntEffect2D].nCounterAnim++;
			if (g_aEffect2D[nCntEffect2D].nType == EFFECTTYPE_2D_SPEED)
			{
				if (g_aEffect2D[nCntEffect2D].nCounterAnim % 3 == 0)
				{
					int nPattern = rand() % PATTERN;
					if (nPattern == g_aEffect2D[nCntEffect2D].nPatternAnim)
					{
						g_aEffect2D[nCntEffect2D].nPatternAnim = (g_aEffect2D[nCntEffect2D].nPatternAnim + 1) % PATTERN;
					}
					else
					{
						g_aEffect2D[nCntEffect2D].nPatternAnim = nPattern;
					}
					// �e�N�X�`���̐ݒ�
					pVtx[0].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN) *ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f);
					pVtx[1].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN)*ANIM + ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f);
					pVtx[2].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN)*ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f + 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN)*ANIM + ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f + 1.0f);
				}
			}
			else
			{
				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect2D[nCntEffect2D].col;
			pVtx[1].col = g_aEffect2D[nCntEffect2D].col;
			pVtx[2].col = g_aEffect2D[nCntEffect2D].col;
			pVtx[3].col = g_aEffect2D[nCntEffect2D].col;
			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffect2D[nCntEffect2D].fWidth), -(g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aEffect2D[nCntEffect2D].fWidth), -(g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffect2D[nCntEffect2D].fWidth), (g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aEffect2D[nCntEffect2D].fWidth), (g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;

		}

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect2D->Unlock();
}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawEffect2D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
    pDevice->SetStreamSource(0, g_pVtxBuffEffect2D, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT_2D; nCntEffect2D++)
	{
		//if (g_aEffect2D[nCntEffect2D].nType != EFFECTTYPE_2D_ARROW)
		//{
		//	// ���u�����f�B���O�����Z�����ɐݒ�
		//	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		//}
		if (g_aEffect2D[nCntEffect2D].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect2D[g_aEffect2D[nCntEffect2D].nType]);
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nCntEffect2D,
				2);
		}
			// ���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//==============================================================================================================================
// ���_�̍쐬
//==============================================================================================================================
void MakeVertexEffect2D(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT_2D,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect2D,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT_2D; nCntEffect2D++, pVtx += 4)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(g_aEffect2D[nCntEffect2D].fWidth), -(g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
		pVtx[1].pos = D3DXVECTOR3((g_aEffect2D[nCntEffect2D].fWidth), -(g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
		pVtx[2].pos = D3DXVECTOR3(-(g_aEffect2D[nCntEffect2D].fWidth), (g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
		pVtx[3].pos = D3DXVECTOR3((g_aEffect2D[nCntEffect2D].fWidth), (g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
		// rhw�̐ݒ�
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}
		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_aEffect2D[nCntEffect2D].col;
		pVtx[1].col = g_aEffect2D[nCntEffect2D].col;
		pVtx[2].col = g_aEffect2D[nCntEffect2D].col;
		pVtx[3].col = g_aEffect2D[nCntEffect2D].col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect2D->Unlock();
}


//==============================================================================================================================
// �G�t�F�N�g�ݒ�
//==============================================================================================================================
void SetEffect2D(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int nLife,EFFECTTYPE_2D nType)
{
	VERTEX_2D*pVtx;

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEffect2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT_2D; nCntEffect2D++, pVtx += 4)
	{
		if (g_aEffect2D[nCntEffect2D].bUse == false)
		{// �e�����g�p����Ă��Ȃ�
			g_aEffect2D[nCntEffect2D].pos = pos;			// ���S���W
			g_aEffect2D[nCntEffect2D].move = move;			// �ړ���
			g_aEffect2D[nCntEffect2D].col = col;			// �F
			g_aEffect2D[nCntEffect2D].fWidth = fWidth;	// ���a
			g_aEffect2D[nCntEffect2D].fHeight = fHeight;	// ���a
			g_aEffect2D[nCntEffect2D].nLife = nLife;		// ����
			g_aEffect2D[nCntEffect2D].nType = nType;		// ����
			g_aEffect2D[nCntEffect2D].nCounterAnim = 0;
			g_aEffect2D[nCntEffect2D].nPatternAnim = 0;
			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffect2D[nCntEffect2D].fWidth), -(g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aEffect2D[nCntEffect2D].fWidth), -(g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffect2D[nCntEffect2D].fWidth), (g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aEffect2D[nCntEffect2D].fWidth), (g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect2D[nCntEffect2D].col;
			pVtx[1].col = g_aEffect2D[nCntEffect2D].col;
			pVtx[2].col = g_aEffect2D[nCntEffect2D].col;
			pVtx[3].col = g_aEffect2D[nCntEffect2D].col;
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			if (g_aEffect2D[nCntEffect2D].nType == EFFECTTYPE_2D_SPEED)
			{
					g_aEffect2D[nCntEffect2D].nPatternAnim = (g_aEffect2D[nCntEffect2D].nPatternAnim + 1) % PATTERN;
					// �e�N�X�`���̐ݒ�
					pVtx[0].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN) *ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f);
					pVtx[1].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN)*ANIM + ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f);
					pVtx[2].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN)*ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f + 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN)*ANIM + ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f + 1.0f);

			}
			g_aEffect2D[nCntEffect2D].bUse = true;	// �g�p���Ă����Ԃɂ���
			break;
		}
	}
	g_pVtxBuffEffect2D->Unlock();
}
//==============================================================================================================================
// �����_�������̎擾
//==============================================================================================================================
float SetRandomEffect2D(void)
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
EFFECT2D *GetEffect2D(void)
{
	return &g_aEffect2D[0];
}

void SetEffectSpeed(float fMove, float fMaxMove)
{
	for (int nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT_2D; nCntEffect2D++)
	{
		if (g_aEffect2D[nCntEffect2D].bUse == true)
		{
			switch (g_aEffect2D[nCntEffect2D].nType)
			{
			case EFFECTTYPE_2D_SPEED:	// ���傤�ǂ̃G�t�F�N�g
				g_aEffect2D[nCntEffect2D].col.a = fMove / fMaxMove;
				break;
			}
		}
	}
}