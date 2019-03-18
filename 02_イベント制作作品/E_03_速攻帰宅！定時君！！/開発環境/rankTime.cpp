//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// �^�C������ [rankRankTime.cpp]
// Author : �� �a��
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "rankTime.h"
#include "main.h"
#include "ranking.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �}�N����`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define	MAX_RANKTIME				(6)							// �^�C���̍ő包��
#define RANKTIME_WIDTH				(60)						// �ꕶ���̕�
#define RANKTIME_HEIGHT				(80)						// �ꕶ���̍���
#define RANKTIME_TEXTURENAME		"data//TEXTURE//time000.png"		// �ǂݍ��ރe�N�X�`���̃t�@�C����
#define ANIM_CUT					(10)							// �A�j���[�V�����̕�����
#define ANIM_CUT_WIDTH				(10.0f)							// �A�j���[�V�����̉��̕�����
#define ANIM_CUT_HEIGHT				(1.0f)							// �A�j���[�V�����̏c�̕�����
#define ANIM_PATTERN_WIDTH			float(1 / ANIM_CUT_WIDTH)		// �A�j���[�V�����̉��p�^�[���̉��Z��
#define ANIM_PATTERN_HEIGHT			float(1 / ANIM_CUT_HEIGHT)		// �A�j���[�V�����̏c�p�^�[���̉��Z��
#define GAME_RANKTIME				(17*60*60 + 29*60 + 50)			// �Q�[���̏����^�C��(��+����+�b��

#define RANKTIME_WIDTH_INTERVAL		(40.0f)							// �����b�̊Ԃ̕�

#define RANKTIME_POS_X				(SCREEN_WIDTH / 2.0f + RANKTIME_WIDTH*(MAX_RANKTIME/2) +RANKTIME_WIDTH_INTERVAL + RANKTIME_WIDTH/2.0f)			// �ʒuX
#define RANKTIME_POS_Y				(200.0f)				// �ʒuY

#define RANKTIME_RESULT_POS_X		(1200.0f)			// �ʒuX
#define RANKTIME_RESULT_POS_Y		(600.0f)			// �ʒuY

#define RANK_HEIGHT_INTERVAL		(100.0f)			// Y�̊Ԋu


//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �\���̒�`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	int nRankTime;			// �^�C��
	bool bUse;			// �g�p���Ă��邩�ǂ���
}RankTime;

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �v���g�^�C�v�錾
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
void MakeVertexRankTime(LPDIRECT3DDEVICE9 pDevice);

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �O���[�o���ϐ��錾
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
LPDIRECT3DTEXTURE9		g_pTextureRankTime = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankTime = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
RankTime				g_aRankTime[MAX_RANKING][MAX_RANKTIME];	// �^�C���̏��
int						g_nRankTime[MAX_RANKING];				// �����L���O�̎��Ԃ�

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ����������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void InitRankTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntTime = 0; nCntTime < MAX_RANKTIME; nCntTime++)
		{
			g_aRankTime[nCntRank][nCntTime].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aRankTime[nCntRank][nCntTime].nRankTime = 0;
			g_aRankTime[nCntRank][nCntTime].bUse = false;
		}
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		RANKTIME_TEXTURENAME,
		&g_pTextureRankTime);

	// ���_���̍쐬
	MakeVertexRankTime(pDevice);

	MODE mode = GetMode();


	// �^�C����\��
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntTime = 0; nCntTime < MAX_RANKTIME; nCntTime++)
		{
			SetRankTime(D3DXVECTOR3(RANKTIME_POS_X - float(((RANKTIME_WIDTH) * (1 + nCntTime))) - RANKTIME_WIDTH_INTERVAL * (nCntTime / 2), RANKTIME_POS_Y + RANK_HEIGHT_INTERVAL * nCntRank, 0.0f));
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �I������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UninitRankTime(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureRankTime != NULL)
	{
		g_pTextureRankTime->Release();
		g_pTextureRankTime = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffRankTime != NULL)
	{
		g_pVtxBuffRankTime->Release();
		g_pVtxBuffRankTime = NULL;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �X�V����
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateRankTime()
{
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRankTime->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntTime = 0; nCntTime < MAX_RANKTIME; nCntTime++, pVtx += 4)
		{
			if (g_aRankTime[nCntRank][nCntTime].bUse == true)	// �^�C�����g�p����Ă���
			{
				if (nCntTime >= 4)
				{ // ���Ԃ̏���
				  // �b����60�Ŋ����ĕ������Z�o���Ă����60�Ŋ����Ď��Ԃ��Z�o
					int nRankTimeHour = (g_nRankTime[nCntRank] / 60) / 60;

					if (nCntTime % 2 == 0)
					{
						g_aRankTime[nCntRank][nCntTime].nRankTime = nRankTimeHour % (int(powf(10.0f, float(nCntTime - 4))) * 10) / int(powf(10.0f, float(nCntTime - 4)));
					}
					else
					{
						int nDigit2 = nRankTimeHour % (int(powf(10.0f, float(nCntTime - 4))) * 10) / int(powf(10.0f, float(nCntTime - 4)));

						g_aRankTime[nCntRank][nCntTime].nRankTime = nDigit2 % 6;
					}
				}
				else if (nCntTime >= 2)
				{ // �����̏���
					// �b����60�Ŋ����ĕ������Z�o
					int nRankTimeMinutes = g_nRankTime[nCntRank] / 60;
					nRankTimeMinutes = nRankTimeMinutes % 60;

					if (nCntTime % 2 == 0)
					{
						g_aRankTime[nCntRank][nCntTime].nRankTime = nRankTimeMinutes % (int(powf(10.0f, float(nCntTime - 2))) * 10) / int(powf(10.0f, float(nCntTime - 2)));
					}
					else
					{
						int nDigit2 = nRankTimeMinutes % (int(powf(10.0f, float(nCntTime - 2))) * 10) / int(powf(10.0f, float(nCntTime - 2)));

						g_aRankTime[nCntRank][nCntTime].nRankTime = nDigit2 % 6;
					}
				}
				else
				{// �b���̏���
					/*int nRankTimeSeconds = g_nRankTime / 60 / 60;
					nRankTimeSeconds = nRankTimeSeconds % 60;*/

					int nRankTimeSeconds = g_nRankTime[nCntRank] % 60;

					if (nCntTime % 2 == 0)
					{
						g_aRankTime[nCntRank][nCntTime].nRankTime = nRankTimeSeconds % (int(powf(10.0f, float(nCntTime))) * 10) / int(powf(10.0f, float(nCntTime)));
					}
					else
					{
						int nDigit2 = nRankTimeSeconds % (int(powf(10.0f, float(nCntTime))) * 10) / int(powf(10.0f, float(nCntTime)));

						g_aRankTime[nCntRank][nCntTime].nRankTime = nDigit2 % 6;
					}
				}

				pVtx[0].tex = D3DXVECTOR2(g_aRankTime[nCntRank][nCntTime].nRankTime * ANIM_PATTERN_WIDTH, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aRankTime[nCntRank][nCntTime].nRankTime * ANIM_PATTERN_WIDTH + ANIM_PATTERN_WIDTH, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aRankTime[nCntRank][nCntTime].nRankTime * ANIM_PATTERN_WIDTH, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aRankTime[nCntRank][nCntTime].nRankTime * ANIM_PATTERN_WIDTH + ANIM_PATTERN_WIDTH, 1.0f);
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRankTime->Unlock();
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �`�揈��
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void DrawRankTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffRankTime, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRankTime);

	// �|���S���̕`��
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntTime = 0; nCntTime < MAX_RANKTIME; nCntTime++)
		{
			if (g_aRankTime[nCntRank][nCntTime].bUse == true)	// �^�C�����g�p����Ă���
			{
				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntRank * MAX_RANKTIME)*4 + nCntTime * 4, 2);
			}
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �^�C���ݒ�
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetRankTime(D3DXVECTOR3 pos)
{
	int nCntTime;
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	bool bBreak = false;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRankTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (nCntTime = 0; nCntTime < MAX_RANKTIME; nCntTime++, pVtx += 4)
		{
			if (g_aRankTime[nCntRank][nCntTime].bUse == false)
			{ // �^�C�����g�p����Ă��Ȃ��ꍇ
			  // ���_���̐ݒ�
				g_aRankTime[nCntRank][nCntTime].pos = pos;
				pVtx[0].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x - RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y - RANKTIME_HEIGHT / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x + RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y - RANKTIME_HEIGHT / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x - RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y + RANKTIME_HEIGHT / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x + RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y + RANKTIME_HEIGHT / 2, 0.0f);

				g_aRankTime[nCntRank][nCntTime].bUse = true;	// �g�p�����Ԃɂ���
				bBreak = true;
				break;	// ����break�͂��Ȃ���
			}
		}
		if (bBreak == true)
		{
			break;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRankTime->Unlock();
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���_���̍쐬
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void MakeVertexRankTime(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	VERTEX_2D *pVtx;

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKTIME * MAX_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankTime,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRankTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntTime = 0; nCntTime < MAX_RANKTIME; nCntTime++, pVtx += 4)
		{
			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x - RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y - RANKTIME_HEIGHT / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x + RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y - RANKTIME_HEIGHT / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x - RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y + RANKTIME_HEIGHT / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x + RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y + RANKTIME_HEIGHT / 2, 0.0f);
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(ANIM_PATTERN_WIDTH, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, ANIM_PATTERN_HEIGHT);
			pVtx[3].tex = D3DXVECTOR2(ANIM_PATTERN_WIDTH, ANIM_PATTERN_HEIGHT);
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRankTime->Unlock();

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �^�C���̎擾
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
int GetRankTime(void)
{
	return g_nRankTime[0];
}



//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���U���g�̎��Ԑݒ�
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetRankTimeResult(int nRank,int nRankTime)
{
	g_nRankTime[nRank] = nRankTime;
}