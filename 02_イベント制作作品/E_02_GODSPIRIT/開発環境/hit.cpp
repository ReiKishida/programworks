////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �q�b�g���� [hit.cpp]
// Author : RyotaKinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "hit.h"
#include "effect.h"
#include "input.h"
#include "result.h"
#include "score.h"
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// �u���b�N�̒��S���W
	D3DXCOLOR  col;				// �F
	HITTEX nType;				// ���
	float fWidth;				// ��
	float fHeight;				// ����
	int nNumCnt;				// ������
} HitInfo;
typedef struct
{
	char *pFileName;
}HitTexture;
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_HIT	(5)
#define MAX_LEN_HIT	(3)// �X�R�A�̌���
#define HIT_TIME	(300) // �q�b�g�̌p������
#define GET_HITMOVE (30.0f)	// �q�b�g�}�����x
#define HIT_STOPPOS	(500.0f) // �q�b�g��~�ʒu
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexHit(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureHit[HITTEX_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHit = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
Hit						g_aHit[MAX_HIT];
HitInfo g_aHitInfo[] =
{
	// �q�b�g�w�i
	{ D3DXVECTOR3(150.0f,410.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),HITTEX_BG,510,100 ,1 },
	// �q�b�g��
	{ D3DXVECTOR3(320.0f,360.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),HITTEX_HITTEXT,140,70 ,1 },
	// �q�b�g��
	{ D3DXVECTOR3(200.0f,360.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),HITTEX_HIT,50,70,1 },
	{ D3DXVECTOR3(150.0f,363.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),HITTEX_HIT,70,90,1 },
	{ D3DXVECTOR3(90.0f,366.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),HITTEX_HIT,90,110,1 },
};
// �e�N�X�`����
HitTexture  g_aHitTex[HITTEX_MAX] =
{
	{ "data/TEXTURE/hitBG000.png" },			// �q�b�g�w�i
	{ "data/TEXTURE/hit.png" },				// �q�b�g
	{ "data/TEXTURE/HitNum001.png" },		// ����
};
int						g_nHit;					// �X�R�A
int						g_nHitMax;				// �ő�q�b�g��
int						g_nHitLength;			// �q�b�g���̕�����
int						g_nHitTime;				// �q�b�g�̎���
D3DXCOLOR				g_HitColStart;			// �F��
D3DXCOLOR				g_HitColEnd;			// �F��
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitHit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nTex = 0; nTex < HITTEX_MAX; nTex++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
			g_aHitTex[nTex].pFileName,		// �t�@�C���̖��O
			&g_pTextureHit[nTex]);	// �e�N�X�`���ւ̃|�C���^
	}
	// ������
	for (int nCnt = 0; nCnt < MAX_HIT; nCnt++)
	{
		g_aHit[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���S
		g_aHit[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
		g_aHit[nCnt].nType = HITTEX_MAX;						// ���
		g_aHit[nCnt].fWidth = 0;									// ��
		g_aHit[nCnt].fHeight = 0;									// ����
		g_aHit[nCnt].bUse = false;								// �g�p����Ă��邩�ǂ���
	}
	g_nHit = 0;	// �q�b�g
	g_nHitMax = 0; // �q�b�g��
	g_nHitLength = 1;
	g_nHitTime = 0;
	g_HitColStart = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_HitColEnd = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// ���_���̍쐬
	MakeVertexHit(pDevice);
	for (int nCnt = 0; nCnt < sizeof g_aHitInfo / sizeof(HitInfo); nCnt++)
	{
		SetHit(g_aHitInfo[nCnt].pos , g_aHitInfo[nCnt].col, g_aHitInfo[nCnt].nType, g_aHitInfo[nCnt].fWidth, g_aHitInfo[nCnt].fHeight );
	}
	for (int nCnt = 0; nCnt < MAX_HIT; nCnt++)
	{
		g_aHit[nCnt].bUse = false;
	}
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitHit(void)
{
	// �e�N�X�`���̊J��
	for (int nTex = 0; nTex < HITTEX_MAX; nTex++)
	{
		if (g_pTextureHit[nTex] != NULL)
		{
			g_pTextureHit[nTex]->Release();
			g_pTextureHit[nTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffHit != NULL)
	{
		g_pVtxBuffHit->Release();
		g_pVtxBuffHit = NULL;
	}
	GetHit(g_nHitMax);

}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateHit(void)
{
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
	int nCntPow = 0;

//-------------------------------------/ �q�b�g���ɉ��������� /----------------------------------------------------------------//
	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffHit->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	for (int nCnt = 0; nCnt < MAX_HIT; nCnt++, pVtx += 4)
	{
		if (g_nHitTime >= HIT_TIME)
		{
			g_nHit = 0;
			g_nHitTime = 0;
		}
		if (g_nHit < 1000)
		{// 3��
			g_HitColStart = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			g_HitColEnd = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			g_nHitLength = 3;
			if (g_nHit < 100)
			{// 2��
				g_HitColStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				g_HitColEnd = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				g_nHitLength = 2;
				if (g_nHit < 10)
				{// 1��
					g_HitColStart = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
					g_HitColEnd = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					g_nHitLength = 1;
					if (g_nHit <= 0)
					{
						g_nHitLength = 0;
					}
				}
			}
		}
		if (g_nHitTime >= HIT_TIME - 50)
		{// �q�b�g�����Z�b�g���邽�߉�ʊO
			g_aHit[nCnt].pos.x -= GET_HITMOVE;
			if (g_aHit[nCnt].pos.x <= g_aHitInfo[nCnt].pos.x - HIT_STOPPOS)
			{
				g_aHit[nCnt].pos.x = g_aHitInfo[nCnt].pos.x - HIT_STOPPOS;
			}
		}
		else if (g_nHit > 0&&g_nHitTime <= 50)
		{// �q�b�g����
			g_aHit[nCnt].pos.x += GET_HITMOVE;
			if (g_aHit[nCnt].pos.x >= g_aHitInfo[nCnt].pos.x)
			{
				g_aHit[nCnt].pos.x = g_aHitInfo[nCnt].pos.x;
			}
		}
		// ���_����ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aHit[nCnt].fWidth / 2, -g_aHit[nCnt].fHeight, 0.0f) + g_aHit[nCnt].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aHit[nCnt].fWidth / 2, -g_aHit[nCnt].fHeight, 0.0f) + g_aHit[nCnt].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aHit[nCnt].fWidth / 2, 0.0f, 0.0f) + g_aHit[nCnt].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aHit[nCnt].fWidth / 2, 0.0f, 0.0f) + g_aHit[nCnt].pos;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHit->Unlock();
//-------------------------------------/ �q�b�g������ /----------------------------------------------------------------//
	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffHit->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	for (int nCnt = 0; nCnt < MAX_HIT; nCnt++, pVtx += 4)
	{
		if (g_aHit[nCnt].nType == HITTEX_HIT)
		{// ���Z�ł��ꂽ���ŗݏ�𐳂�������
			g_aHit[nCnt].bUse = true;
			int nData = int(powf(10, float(nCntPow)));
			pVtx[0].tex = D3DXVECTOR2((g_nHit % (10 * nData) / nData) * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_nHit % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_nHit % (10 * nData) / nData)* 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_nHit % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_HitColStart;
			pVtx[1].col = g_HitColStart;
			pVtx[2].col = g_HitColEnd;
			pVtx[3].col = g_HitColEnd;
			nCntPow++;
			if (nCntPow == g_nHitLength)
			{// ���������ʂɂ���ꍇ��菜������
				for (int nFalse = nCntPow; nFalse < MAX_LEN_HIT; nFalse++)
				{// �ő包�����@- ���݂̌�������
					g_aHit[nCnt + nFalse].bUse = false;
				}
				break;
			}
			else if (g_nHitLength == 0)
			{
				g_aHit[nCnt].bUse = false;
			}
		}
	}
	g_nHitTime++;

}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawHit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffHit, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nTex = 0; nTex < MAX_HIT; nTex++)
	{
		if (g_aHit[nTex].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureHit[g_aHit[nTex].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nTex,
				2);
		}
	}

}

//==============================================================================================================================
// ���_���̍쐬
//==============================================================================================================================
void MakeVertexHit(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_HIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHit,
		NULL);

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffHit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_HIT; nCnt++, pVtx += 4)
	{
		// ���_����ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aHit[nCnt].fWidth / 2, -g_aHit[nCnt].fHeight, 0.0f) + g_aHit[nCnt].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aHit[nCnt].fWidth / 2, -g_aHit[nCnt].fHeight, 0.0f) + g_aHit[nCnt].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aHit[nCnt].fWidth / 2, 0.0f, 0.0f) + g_aHit[nCnt].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aHit[nCnt].fWidth / 2, 0.0f, 0.0f) + g_aHit[nCnt].pos;		
		// rhw�̐ݒ�
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}
		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_HitColStart;
		pVtx[1].col = g_HitColStart;
		pVtx[2].col = g_HitColEnd;
		pVtx[3].col = g_HitColEnd;
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHit->Unlock();
}
//==============================================================================================================================
// �X�R�A�̐ݒ�
//==============================================================================================================================
void SetHit(D3DXVECTOR3 pos, D3DXCOLOR col, HITTEX nType, float fWidth, float fHeight)
{
	VERTEX_2D*pVtx;		// ���_���ւ̃|�C���^
	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffHit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_HIT; nCnt++, pVtx += 4)
	{
		if (g_aHit[nCnt].bUse == false)
		{
			g_aHit[nCnt].pos = pos;			// ���S
			g_aHit[nCnt].pos.x -= HIT_STOPPOS;	// �����ʒu����ʊO
			g_aHit[nCnt].col = col;			// �F
			g_aHit[nCnt].nType = nType;		// ���
			g_aHit[nCnt].fWidth = fWidth;		// ��
			g_aHit[nCnt].fHeight = fHeight;	// ����
			g_aHit[nCnt].bUse = true;

			// ���_����ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aHit[nCnt].fWidth / 2, -g_aHit[nCnt].fHeight, 0.0f) + g_aHit[nCnt].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aHit[nCnt].fWidth / 2, -g_aHit[nCnt].fHeight, 0.0f) + g_aHit[nCnt].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aHit[nCnt].fWidth / 2,0.0f, 0.0f) + g_aHit[nCnt].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aHit[nCnt].fWidth / 2, 0.0f, 0.0f) + g_aHit[nCnt].pos;
			if (g_aHit[nCnt].nType == HITTEX_HIT)
			{
				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
			}
			break;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHit->Unlock();
}

//==============================================================================================================================
// �X�R�A�̉��Z
//==============================================================================================================================
void AddHit(int nHit)
{
	g_nHit += nHit;
	AddScore(g_nHit);
	g_nHitTime = 0;
	for (int nCnt = 0; nCnt < MAX_HIT; nCnt++)
	{
		if (g_aHit[nCnt].nType != HITTEX_HIT)
		{
			g_aHit[nCnt].bUse = true;
		}
	}
	if (g_nHitMax <= g_nHit)
	{// �ő�q�b�g��
		g_nHitMax = g_nHit;
	}
}