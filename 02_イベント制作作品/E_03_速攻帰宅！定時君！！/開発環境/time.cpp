//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// �^�C������ [time.cpp]
// Author : �� �a��
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "time.h"
#include "main.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �}�N����`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define	MAX_TIME				(6)							// �^�C���̍ő包��
#define TIME_WIDTH				(60)						// �ꕶ���̕�
#define TIME_HEIGHT				(80)						// �ꕶ���̍���
#define TIME_TEXTURENAME	"data//TEXTURE//time000.png"		// �ǂݍ��ރe�N�X�`���̃t�@�C����
#define ANIM_CUT				(10)							// �A�j���[�V�����̕�����
#define ANIM_CUT_WIDTH			(10.0f)							// �A�j���[�V�����̉��̕�����
#define ANIM_CUT_HEIGHT			(1.0f)							// �A�j���[�V�����̏c�̕�����
#define ANIM_PATTERN_WIDTH		float(1 / ANIM_CUT_WIDTH)		// �A�j���[�V�����̉��p�^�[���̉��Z��
#define ANIM_PATTERN_HEIGHT		float(1 / ANIM_CUT_HEIGHT)		// �A�j���[�V�����̏c�p�^�[���̉��Z��
#define GAME_TIME				(17*60*60 + 29*60 + 50)			// �Q�[���̏����^�C��(��+����+�b��
#define GAME_START_TIME			(17*60*60 + 30*60 + 00)			// �Q�[���̏����^�C��(��+����+�b��
#define TIME_WIDTH_INTERVAL		(40.0f)							// �����b�̊Ԃ̕�

#define TIME_POS_X				(SCREEN_WIDTH / 2.0f + TIME_WIDTH*(MAX_TIME/2) +TIME_WIDTH_INTERVAL + TIME_WIDTH/2.0f)			// �ʒuX
#define TIME_POS_Y				(80.0f)							// �ʒuY

#define TIME_RESULT_POS_X		(1130.0f)			// �ʒuX
#define TIME_RESULT_POS_Y		(500.0f)							// �ʒuY


//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �\���̒�`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	int nTime;			// �^�C��
	bool bUse;			// �g�p���Ă��邩�ǂ���
}Time;

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �v���g�^�C�v�錾
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
//*****************************************************************************
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice);

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �O���[�o���ϐ��錾
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
LPDIRECT3DTEXTURE9		g_pTextureTime = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Time					g_aTime[MAX_TIME];		// �^�C���̏��
int						g_nTime;				// �G��|�����|�C���g���L�^
bool					g_bStop;				// �^�C������~���Ă��邩�ǂ���
bool					g_bGameStart;			// �Q�[���X�^�[�g���邩�ǂ���

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ����������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	g_bGameStart = false;
	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �^�C���̏��̏�����
	g_bStop = false;
	for(int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		g_aTime[nCntTime].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTime[nCntTime].nTime = 0;
		g_aTime[nCntTime].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		TIME_TEXTURENAME,
		&g_pTextureTime);

	// ���_���̍쐬
	MakeVertexTime(pDevice);

	MODE mode = GetMode();

	switch (mode)
	{
	case MODE_GAME:
		g_nTime = GAME_TIME;
		// �^�C����\��
		for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
		{
			SetTime(D3DXVECTOR3(TIME_POS_X - float(((TIME_WIDTH) * (1 + nCntTime))) - TIME_WIDTH_INTERVAL * (nCntTime / 2), TIME_POS_Y, 0.0f));
		}
		break;
	case MODE_RESULT:
		// �^�C����\��
		for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
		{
			SetTime(D3DXVECTOR3(TIME_RESULT_POS_X - float(((TIME_WIDTH) * (1 + nCntTime))) - TIME_WIDTH_INTERVAL * (nCntTime / 2), TIME_RESULT_POS_Y, 0.0f));
		}
		break;
	}

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �I������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UninitTime(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �X�V����
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateTime()
{
	static int nFrameCnt = 0;
	nFrameCnt++;

	MODE mode = GetMode();

	switch (mode)
	{
	case MODE_GAME:
		if (g_bStop == false)
		{
			if (nFrameCnt % 20 == 0)
			{
				g_nTime++;
			}
		}
		break;
	}

	// �Q�[���X�^�[�g����
	if (GAME_START_TIME <= g_nTime)
	{
		g_bGameStart = true;
	}

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++, pVtx += 4)
	{
		if (g_aTime[nCntTime].bUse == true)	// �^�C�����g�p����Ă���
		{
			if (nCntTime >= 4)
			{ // ���Ԃ̏���
			  // �b����60�Ŋ����ĕ������Z�o���Ă����60�Ŋ����Ď��Ԃ��Z�o
				int nTimeHour = (g_nTime / 60) / 60;

				if (nCntTime % 2 == 0)
				{
					g_aTime[nCntTime].nTime = nTimeHour % (int(powf(10.0f, float(nCntTime - 4))) * 10) / int(powf(10.0f, float(nCntTime - 4)));
				}
				else
				{
					int nDigit2 = nTimeHour % (int(powf(10.0f, float(nCntTime - 4))) * 10) / int(powf(10.0f, float(nCntTime - 4)));

					g_aTime[nCntTime].nTime = nDigit2 % 6;
				}
			}
			else if (nCntTime >= 2)
			{ // �����̏���
				// �b����60�Ŋ����ĕ������Z�o
				int nTimeMinutes = g_nTime / 60;
				nTimeMinutes = nTimeMinutes % 60;

				if (nCntTime % 2 == 0)
				{
					g_aTime[nCntTime].nTime = nTimeMinutes % (int(powf(10.0f, float(nCntTime-2))) * 10) / int(powf(10.0f, float(nCntTime-2)));
				}
				else
				{
					int nDigit2 = nTimeMinutes % (int(powf(10.0f, float(nCntTime-2))) * 10) / int(powf(10.0f, float(nCntTime-2)));

					g_aTime[nCntTime].nTime = nDigit2 % 6;
				}
			}
			else
			{// �b���̏���
				/*int nTimeSeconds = g_nTime / 60 / 60;
				nTimeSeconds = nTimeSeconds % 60;*/

				int nTimeSeconds = g_nTime % 60;

				if (nCntTime % 2 == 0)
				{
					g_aTime[nCntTime].nTime = nTimeSeconds % (int(powf(10.0f, float(nCntTime))) * 10) / int(powf(10.0f, float(nCntTime)));
				}
				else
				{
					int nDigit2 = nTimeSeconds % (int(powf(10.0f, float(nCntTime))) * 10) / int(powf(10.0f, float(nCntTime)));

					g_aTime[nCntTime].nTime = nDigit2 % 6;
				}
			}

			pVtx[0].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH + ANIM_PATTERN_WIDTH, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH + ANIM_PATTERN_WIDTH, 1.0f);
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �`�揈��
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void DrawTime(void)
{
	int nCntTime;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTime);

	// �|���S���̕`��
	for(nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		if (g_aTime[nCntTime].bUse == true)	// �^�C�����g�p����Ă���
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �^�C���ݒ�
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetTime(D3DXVECTOR3 pos)
{
	int nCntTime;
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++, pVtx += 4)
	{
		if (g_aTime[nCntTime].bUse == false)
		{ // �^�C�����g�p����Ă��Ȃ��ꍇ
		  // ���_���̐ݒ�
			g_aTime[nCntTime].pos = pos;
			pVtx[0].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - TIME_WIDTH / 2, g_aTime[nCntTime].pos.y - TIME_HEIGHT / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + TIME_WIDTH / 2, g_aTime[nCntTime].pos.y - TIME_HEIGHT / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - TIME_WIDTH / 2, g_aTime[nCntTime].pos.y + TIME_HEIGHT / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + TIME_WIDTH / 2, g_aTime[nCntTime].pos.y + TIME_HEIGHT / 2, 0.0f);

			if (nCntTime >= 4)
			{ // ���Ԃ̏���
			  // �b����60�Ŋ����ĕ������Z�o���Ă����60�Ŋ����Ď��Ԃ��Z�o
				int nTimeHour = (g_nTime / 60) / 60;

				if (nCntTime % 2 == 0)
				{
					g_aTime[nCntTime].nTime = nTimeHour % (int(powf(10.0f, float(nCntTime - 4))) * 10) / int(powf(10.0f, float(nCntTime - 4)));
				}
				else
				{
					int nDigit2 = nTimeHour % (int(powf(10.0f, float(nCntTime - 4))) * 10) / int(powf(10.0f, float(nCntTime - 4)));

					g_aTime[nCntTime].nTime = nDigit2 % 6;
				}
			}
			else if (nCntTime >= 2)
			{ // �����̏���
			  // �b����60�Ŋ����ĕ������Z�o
				int nTimeMinutes = g_nTime / 60;
				nTimeMinutes = nTimeMinutes % 60;

				if (nCntTime % 2 == 0)
				{
					g_aTime[nCntTime].nTime = nTimeMinutes % (int(powf(10.0f, float(nCntTime - 2))) * 10) / int(powf(10.0f, float(nCntTime - 2)));
				}
				else
				{
					int nDigit2 = nTimeMinutes % (int(powf(10.0f, float(nCntTime - 2))) * 10) / int(powf(10.0f, float(nCntTime - 2)));

					g_aTime[nCntTime].nTime = nDigit2 % 6;
				}
			}
			else
			{// �b���̏���
			 /*int nTimeSeconds = g_nTime / 60 / 60;
			 nTimeSeconds = nTimeSeconds % 60;*/

				int nTimeSeconds = g_nTime % 60;

				if (nCntTime % 2 == 0)
				{
					g_aTime[nCntTime].nTime = nTimeSeconds % (int(powf(10.0f, float(nCntTime))) * 10) / int(powf(10.0f, float(nCntTime)));
				}
				else
				{
					int nDigit2 = nTimeSeconds % (int(powf(10.0f, float(nCntTime))) * 10) / int(powf(10.0f, float(nCntTime)));

					g_aTime[nCntTime].nTime = nDigit2 % 6;
				}
			}

			pVtx[0].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH + ANIM_PATTERN_WIDTH, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH + ANIM_PATTERN_WIDTH, 1.0f);

			g_aTime[nCntTime].bUse = true;	// �g�p�����Ԃɂ���
			break;	// ����break�͂��Ȃ���
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���_���̍쐬
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	VERTEX_2D *pVtx;

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++, pVtx += 4)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - TIME_WIDTH / 2, g_aTime[nCntTime].pos.y - TIME_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + TIME_WIDTH / 2, g_aTime[nCntTime].pos.y - TIME_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - TIME_WIDTH / 2, g_aTime[nCntTime].pos.y + TIME_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + TIME_WIDTH / 2, g_aTime[nCntTime].pos.y + TIME_HEIGHT / 2, 0.0f);
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
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �^�C���̎擾
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
int GetTime(void)
{
	return g_nTime;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �^�C���̒�~
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void StopTime(void)
{
	g_bStop = true;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���U���g�̎��Ԑݒ�
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetTimeResult(int nTime)
{
	g_nTime = nTime;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �Q�[���J�n�\���ǂ����̏���
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
bool GetGameStartTime(void)
{
	return g_bGameStart;
}