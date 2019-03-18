//=============================================================================
//
// ���U���g���� [Result.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "Result.h"
#include "fade.h"
#include "sound.h"
#include "game.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Result_TEXTURENAME0	"DATA/TEXTURE/clear.jpg"	//�^�C�g���̃e�N�X�`����
#define Result_TEXTURENAME1	"DATA/TEXTURE/gameover.jpg"	//�^�C�g���̃e�N�X�`����


#define RESULT_TYPE (2)


//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct
{
	D3DXCOLOR col;
}RESULT;


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult[RESULT_TYPE] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;	//���_�o�b�t�@�ւ̃|�C���^



//=============================================================================
// �^�C�g������������
//=============================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	GAMESTATE pGame;
	pGame = *GetGameState();

	//�f�o�C�X�̎擾
	pDevice = GetDevice();


	D3DXCreateTextureFromFile(pDevice, Result_TEXTURENAME0, &g_pTextureResult[0]);

	D3DXCreateTextureFromFile(pDevice, Result_TEXTURENAME1, &g_pTextureResult[1]);





	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

					//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffResult->Unlock();
}

//=============================================================================
// �^�C�g���I������
//=============================================================================
void UnInitResult(void)
{
	for (int nCntResult = 0; nCntResult < RESULT_TYPE; nCntResult++)
	{
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}

	}
	//�e�N�X�`���̔j��

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//=============================================================================
// �^�C�g���X�V����
//=============================================================================
void UpdateResult(void)
{
	FADE pFade;
	pFade = *GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);
			//���[�h�I��
			//SetMode(MODE_TITLE);
			SetFade(MODE_TITLE);
		}
	}
}

//=============================================================================
// �^�C�g���`�揈��
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	GAMESTATE pGame;
	pGame = *GetGameState2();

	//�f�o�C�X���擾����
	pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	if (pGame == GAMESTATE_CLEAR)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureResult[0]);
	}
	else if (pGame == GAMESTATE_FAILED)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureResult[1]);
	}
	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}