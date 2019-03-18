//=============================================================================
//
// ���U���g���� [Result.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "result.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "game.h"
#include "inputx.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULT		"data/TEXTURE/number002.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_HIGHSCORE	"data/TEXTURE/succed.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_HIGHSCORE2	"data/TEXTURE/failed.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexResult(LPDIRECT3DDEVICE9 pDevice);
void MakeVertexResult2(LPDIRECT3DDEVICE9 pDevice);

#define MAX_RESULT (8)	//����
#define MAX_TYPE_RESUT (2)
#define RESULT_X (30.0f) //X�̑傫��
#define RESULT_Y (60.0f)//Y�̑傫��

#define HIGH_X (SCREEN_WIDTH /2) //X�̑傫��
#define HIGH_Y (SCREEN_HEIGHT /2)//Y�̑傫��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureResult = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
int						g_nResult = 0;					// �X�R�A
int						g_aRResultNumber[MAX_RESULT];	//����
D3DXVECTOR3				g_Resultpos;//�X�R�A���S���W


LPDIRECT3DTEXTURE9		g_pTextureResult2[MAX_TYPE_RESUT] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult2 = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3				g_HighgResult;//�X�R�A���S���W
JoyState g_JoyStateResult;


//=============================================================================
// ����������
//=============================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�X�R�A������
	//g_nResult = 8000;

	g_Resultpos = D3DXVECTOR3(380, SCREEN_HEIGHT / 2 + 250.0f, 0.0f);

	g_HighgResult = D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT, &g_pTextureResult);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_HIGHSCORE, &g_pTextureResult2[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_HIGHSCORE2, &g_pTextureResult2[1]);

	// ���_���̍쐬
	MakeVertexResult2(pDevice);

	MakeVertexResult(pDevice);
	//�Q�[���p�b�h�̏��
	g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

}

//=============================================================================
// �I������
//=============================================================================
void UnInitResult(void)
{

	// �e�N�X�`���̊J��
	if (g_pTextureResult != NULL)
	{
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}


	for (int nCnt = 0; nCnt < MAX_TYPE_RESUT; nCnt++)
	{
		// �e�N�X�`���̊J��
		if (g_pTextureResult2[nCnt] != NULL)
		{
			g_pTextureResult2[nCnt]->Release();
			g_pTextureResult2[nCnt] = NULL;
		}
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffResult2 != NULL)
	{
		g_pVtxBuffResult2->Release();
		g_pVtxBuffResult2 = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	FADE pFade;
	pFade = *GetFade();


	//�Q�[���p�b�h��Ԏ擾
	JoyState pJoyState = GetJoystate();

	//���
	XINPUT_STATE state;
	XInputGetState(0, &state);


	// ���_���̍쐬
	VERTEX_2D *pVtx;

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
		{
			if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				PlaySound(SOUND_LABEL_SE_DECIDE);
				//���[�h�I��
				//SetMode(MODE_TITLE);
				SetFade(MODE_RANKING);
			}
			g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_PUSH;

		}
	}


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_RESULT; nCntScore++)
	{
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aRResultNumber[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aRResultNumber[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aRResultNumber[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aRResultNumber[nCntScore], 1.0f);
		pVtx += 4;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();

	if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}

}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	int nCntResult;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	GAMESTATE pGame;
	pGame = *GetGameState2();





	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult2, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	if (pGame == GAMESTATE_CLEAR)
	{
		//�e�N�X�`��
		pDevice->SetTexture(0, g_pTextureResult2[0]);
	}
	if (pGame == GAMESTATE_FAILED)
	{
		//�e�N�X�`��
		pDevice->SetTexture(0, g_pTextureResult2[1]);
	}
	//�v���C���[�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);


	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResult);

	for (nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResult * 4, 2);
	}

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexResult2(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;


	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RESULT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);



	// ���_����ݒ�
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{

		pVtx[0].pos = D3DXVECTOR3(g_Resultpos.x - RESULT_X, g_Resultpos.y - RESULT_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Resultpos.x + RESULT_X, g_Resultpos.y - RESULT_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Resultpos.x - RESULT_X, g_Resultpos.y + RESULT_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Resultpos.x + RESULT_X, g_Resultpos.y + RESULT_Y, 0.0f);


		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);


		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aRResultNumber[nCntResult], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aRResultNumber[nCntResult], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aRResultNumber[nCntResult], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aRResultNumber[nCntResult], 1.0f);



		pVtx += 4;

		g_Resultpos.x += 60;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();
}


//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexResult(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;


	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult2,
		NULL);


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult2->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(g_HighgResult.x - HIGH_X, g_HighgResult.y - HIGH_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_HighgResult.x + HIGH_X, g_HighgResult.y - HIGH_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_HighgResult.x - HIGH_X, g_HighgResult.y + HIGH_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_HighgResult.x + HIGH_X, g_HighgResult.y + HIGH_Y, 0.0f);


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



	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult2->Unlock();
}

//=============================================================================
// �X�R�A�̃Z�b�g
//=============================================================================
void SetResult(int nScore)
{
	if (nScore > g_nResult)
	{
		// ���_���̍쐬
		//		VERTEX_2D *pVtx;


		g_nResult = nScore;

		//g_nResult += g_nResult;


		g_aRResultNumber[0] = g_nResult / 100000000;
		g_aRResultNumber[1] = g_nResult % 10000000 / 1000000;
		g_aRResultNumber[2] = g_nResult % 1000000 / 100000;
		g_aRResultNumber[3] = g_nResult % 100000 / 10000;
		g_aRResultNumber[4] = g_nResult % 10000 / 1000;
		g_aRResultNumber[5] = g_nResult % 1000 / 100;
		g_aRResultNumber[6] = g_nResult % 100 / 10;
		g_aRResultNumber[7] = g_nResult % 10;




	}
}