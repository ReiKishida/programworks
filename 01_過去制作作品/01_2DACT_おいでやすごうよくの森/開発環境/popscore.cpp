//=============================================================================
//
// �A�C�e������ [PopScore.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "popscore.h"
#include "player.h"
#include "main.h"
#include "input.h"
#include "game.h"
#include "block.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_TYPE_POPSCORE		(4)								// �A�C�e���̎��
#define POPSCORE_TEXTURENAME_0	"data/TEXTURE/coinscore.png"		//�A�C�e���̃e�N�X�`��1
#define POPSCORE_TEXTURENAME_1	"data/TEXTURE/vodkascore.png"		//�A�C�e���̃e�N�X�`��2
#define POPSCORE_TEXTURENAME_2	"data/TEXTURE/satsuscore.png"		//�A�C�e���̃e�N�X�`��4
#define POPSCORE_TEXTURENAME_3	"data/TEXTURE/areyscore.png"		//�A�C�e���̃e�N�X�`��3


#define POPSCORE_JUMP_POWER (-9.0f)	//�W�����v��
#define POPSCORE_GRAVITY (1.0f)		//�d��

#define POPSCORE_GRAVITY_MAX (25.0f)	//�d�͂̍ő��

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTexturePopScore[MAX_TYPE_POPSCORE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffIPopScore = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

POPSCORE g_aPopScore[MAX_POPSCORE];	//�u���b�N

						//bool bJump;


//=============================================================================
// ����������
//=============================================================================
void InitPopScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;


	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �A�C�e���̏��̏�����
	for (int nCntPopScore = 0; nCntPopScore < MAX_POPSCORE; nCntPopScore++)
	{
		g_aPopScore[nCntPopScore].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPopScore[nCntPopScore].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPopScore[nCntPopScore].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aPopScore[nCntPopScore].state = POPSCORESTATE_NOMAL;
		g_aPopScore[nCntPopScore].bUse = false;
		
		g_aPopScore[nCntPopScore].nCntGet = 0;

	}



	
	// �e�N�X�`���̓ǂݍ���(�S�̕�)
	D3DXCreateTextureFromFile(pDevice, POPSCORE_TEXTURENAME_0, &g_apTexturePopScore[0]);
	D3DXCreateTextureFromFile(pDevice, POPSCORE_TEXTURENAME_1, &g_apTexturePopScore[1]);
	D3DXCreateTextureFromFile(pDevice, POPSCORE_TEXTURENAME_2, &g_apTexturePopScore[2]);
	D3DXCreateTextureFromFile(pDevice, POPSCORE_TEXTURENAME_3, &g_apTexturePopScore[3]);


	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POPSCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffIPopScore,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffIPopScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPopScore = 0; nCntPopScore < MAX_POPSCORE; nCntPopScore++)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x - g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y - g_aPopScore[nCntPopScore].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x + g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y - g_aPopScore[nCntPopScore].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x - g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x + g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y, 0.0f);

		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffIPopScore->Unlock();


}

//=============================================================================
// �I������
//=============================================================================
void UnInitPopScore(void)
{

	int nCntTex;

	// �e�N�X�`���̊J��
	for (nCntTex = 0; nCntTex < MAX_TYPE_POPSCORE; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_apTexturePopScore[nCntTex] != NULL)
		{
			g_apTexturePopScore[nCntTex]->Release();
			g_apTexturePopScore[nCntTex] = NULL;
		}
	}


	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffIPopScore != NULL)
	{
		g_pVtxBuffIPopScore->Release();
		g_pVtxBuffIPopScore = NULL;
	}



}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePopScore(void)
{

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	for (int nCntPopScore = 0; nCntPopScore < MAX_POPSCORE; nCntPopScore++)
	{
		g_aPopScore[nCntPopScore].posold = g_aPopScore[nCntPopScore].pos;
	}


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffIPopScore->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntPopScore = 0; nCntPopScore < MAX_POPSCORE; nCntPopScore++)
	{
		if (g_aPopScore[nCntPopScore].bUse == true)	//�A�C�e�����g�p����Ă���
		{

			switch (g_aPopScore[nCntPopScore].PopScoreType)
			{
			case POPSCORETYPE_COIN_SCORE:
				g_aPopScore[nCntPopScore].nCntLost--;
				if (g_aPopScore[nCntPopScore].nCntLost <= 0)
				{
					g_aPopScore[nCntPopScore].bUse = false;
				}
				g_aPopScore[nCntPopScore].move.y += POPSCORE_GRAVITY;
				if (g_aPopScore[nCntPopScore].move.y >= POPSCORE_GRAVITY_MAX)
				{
					g_aPopScore[nCntPopScore].move.y = POPSCORE_GRAVITY_MAX;
				}
				//g_aPopScore[nCntPopScore].col.a -= 0.03f;


				g_aPopScore[nCntPopScore].pos.x += g_aPopScore[nCntPopScore].move.x;
				g_aPopScore[nCntPopScore].pos.y += g_aPopScore[nCntPopScore].move.y;
				break;

			case POPSCORETYPE_VODKA_SCORE:
				g_aPopScore[nCntPopScore].nCntLost--;
				if (g_aPopScore[nCntPopScore].nCntLost <= 0)
				{
					g_aPopScore[nCntPopScore].bUse = false;
				}
				g_aPopScore[nCntPopScore].move.y += POPSCORE_GRAVITY;
				if (g_aPopScore[nCntPopScore].move.y >= POPSCORE_GRAVITY_MAX)
				{
					g_aPopScore[nCntPopScore].move.y = POPSCORE_GRAVITY_MAX;
				}

				//g_aPopScore[nCntPopScore].col.a -= 0.03f;

				g_aPopScore[nCntPopScore].pos.x += g_aPopScore[nCntPopScore].move.x;
				g_aPopScore[nCntPopScore].pos.y += g_aPopScore[nCntPopScore].move.y;

				break;
			case POPSCORETYPE_SATSU_SCORE:
				g_aPopScore[nCntPopScore].nCntLost--;
				if (g_aPopScore[nCntPopScore].nCntLost <= 0)
				{
					g_aPopScore[nCntPopScore].bUse = false;
				}
				g_aPopScore[nCntPopScore].move.y += POPSCORE_GRAVITY;
				if (g_aPopScore[nCntPopScore].move.y >= POPSCORE_GRAVITY_MAX)
				{
					g_aPopScore[nCntPopScore].move.y = POPSCORE_GRAVITY_MAX;
				}

				//g_aPopScore[nCntPopScore].col.a -= 0.03f;

				g_aPopScore[nCntPopScore].pos.x += g_aPopScore[nCntPopScore].move.x;
				g_aPopScore[nCntPopScore].pos.y += g_aPopScore[nCntPopScore].move.y;
				break;

			case POPSCORETYPE_AREY_SCORE:
				g_aPopScore[nCntPopScore].nCntLost--;
				if (g_aPopScore[nCntPopScore].nCntLost <= 0)
				{
					g_aPopScore[nCntPopScore].bUse = false;
				}
			//	g_aPopScore[nCntPopScore].col.a -= 0.03f;
			g_aPopScore[nCntPopScore].move.y += POPSCORE_GRAVITY;
				if (g_aPopScore[nCntPopScore].move.y >= POPSCORE_GRAVITY_MAX)
				{
					g_aPopScore[nCntPopScore].move.y = POPSCORE_GRAVITY_MAX;
				}


				g_aPopScore[nCntPopScore].pos.x += g_aPopScore[nCntPopScore].move.x;
				g_aPopScore[nCntPopScore].pos.y += g_aPopScore[nCntPopScore].move.y;
				break;

			
			}

		}

		pVtx[0].col = g_aPopScore[nCntPopScore].col;
		pVtx[1].col = g_aPopScore[nCntPopScore].col;
		pVtx[2].col = g_aPopScore[nCntPopScore].col;
		pVtx[3].col = g_aPopScore[nCntPopScore].col;

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffIPopScore->Unlock();




	

	

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffIPopScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPopScore = 0; nCntPopScore < MAX_POPSCORE; nCntPopScore++)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x - g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y - g_aPopScore[nCntPopScore].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x + g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y - g_aPopScore[nCntPopScore].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x - g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x + g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y, 0.0f);

		


		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffIPopScore->Unlock();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPopScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// �|���S���̕`��

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffIPopScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntPopScore = 0; nCntPopScore < MAX_POPSCORE; nCntPopScore++)
	{
		if (g_aPopScore[nCntPopScore].bUse == true)
		{

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTexturePopScore[g_aPopScore[nCntPopScore].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPopScore * 4, 2);
		}
	}
}


//=============================================================================
// �A�C�e���̐ݒ�
//=============================================================================
void SetPopScore(D3DXVECTOR3 pos, int type, D3DXVECTOR3 move)
{
	int nCntPopScore;
	VERTEX_2D *pVtx;


	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffIPopScore->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntPopScore = 0; nCntPopScore < MAX_POPSCORE; nCntPopScore++)
	{
	
		if (g_aPopScore[nCntPopScore].bUse == false)	//�A�C�e�����g�p����Ă��Ȃ�
		{
			pVtx += (nCntPopScore * 4);

			g_aPopScore[nCntPopScore].pos = pos;
			g_aPopScore[nCntPopScore].move = move;
			
			g_aPopScore[nCntPopScore].nCntLost = 18;

			if (type == 0)
			{
				g_aPopScore[nCntPopScore].fHeight = 35;
				g_aPopScore[nCntPopScore].fWidth = 70;
			}

			if (type == 1)
			{
				g_aPopScore[nCntPopScore].fHeight = 40;
				g_aPopScore[nCntPopScore].fWidth = 75;
			}

			if (type == 2)
			{
				g_aPopScore[nCntPopScore].fHeight = 80;
				g_aPopScore[nCntPopScore].fWidth = 100;
			}

			if (type == 3)
			{
				g_aPopScore[nCntPopScore].fHeight = 40;
				g_aPopScore[nCntPopScore].fWidth = 80;
			}
			//���_�̍��W
			pVtx[0].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x - g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y - g_aPopScore[nCntPopScore].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x + g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y - g_aPopScore[nCntPopScore].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x - g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x + g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y, 0.0f);


			g_aPopScore[nCntPopScore].nType = type;

			g_aPopScore[nCntPopScore].bUse = true;	//�g�p���Ă����Ԃɂ���

			break;


			
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffIPopScore->Unlock();
}



//=============================================================================
// �A�C�e���̎擾
//=============================================================================
POPSCORE *GetPopScore(void)
{
		return &g_aPopScore[0];
}
