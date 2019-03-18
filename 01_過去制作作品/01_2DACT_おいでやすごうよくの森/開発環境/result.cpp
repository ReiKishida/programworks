//=============================================================================
//
// �^�C�g����ʏ��� [Result.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "Result.h"
#include "effect.h"
#include "fade.h"
#include "bg.h"
#include "block.h"
#include "item.h"
#include "highscore.h"
#include "sound.h"
#include "resultitem.h"
#include "resultarey.h"
#include "resultvodka.h"
#include "resultsatsu.h"
#include "resultitemall.h"
#include "fg.h"
#include "fg2.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Result_TEXTURENAME000	"DATA/TEXTURE/result1.png"	//�^�C�g���̃e�N�X�`����
#define Result_TEXTURENAME001	"DATA/TEXTURE/result01-1.png"	//�^�C�g���̃e�N�X�`����


#define MAX_RESULT_TYPE (2)
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT_TYPE] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;	//���_�o�b�t�@�ւ̃|�C���^

int g_nCntNextScene; //�����L���O�ւ̑J��

//=============================================================================
// ���U���g����������
//=============================================================================
void InitResult(void)
{
	//�w�i������
	InitBG();

	//�u���b�N�̏�����
	InitBlock();

	//�A�C�e���̏�����
	InitItem();

	//�n�C�X�R�A������
	InitHighScore();

	//�A�C�e���̐��Z������
	InitResultItem();

	//�A���C�̐��Z������
	InitResultArey();

	//�E�H�b�J�̐��Z������
	InitResultVodka();

	//�D�̐��Z������
	InitResultSatsu();

	//���U���g���̃A�C�e���X�R�A
	InitResultItemAll();


	//�O�i
	InitFg();

	//�O�i
	InitFg2();

	g_nCntNextScene = 0;


	SetBlock(D3DXVECTOR3(0.0f, 580, 0), 1280.0f, 150.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f);


	SetItem(D3DXVECTOR3(180, 100, 0.0f), ITEMTYPE_H_COIN, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetItem(D3DXVECTOR3(180, 200, 0.0f), ITEMTYPE_H_VODKA, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetItem(D3DXVECTOR3(180, 300, 0.0f), ITEMTYPE_H_PAPER, D3DXVECTOR3(0.0f, 0.0f, 0.0f));


	SetItem(D3DXVECTOR3(180, 400, 0.0f), ITEMTYPE_H_AREY, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetItem(D3DXVECTOR3(180, 400, 0.0f), ITEMTYPE_H_AREY, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetFg(D3DXVECTOR3(270, 660, 0.0f), 0, 85, 150);
	SetFg(D3DXVECTOR3(810, 660, 0.0f), 1, 75, 150);
	SetFg(D3DXVECTOR3(1100, 650, 0.0f), 0, 65, 150);

	//��
	SetFg(D3DXVECTOR3(100, 640, 0.0f), 2, 50, 25);
	SetFg(D3DXVECTOR3(640, 630, 0.0f), 2, 60, 30);
	SetFg(D3DXVECTOR3(450, 630, 0.0f), 2, 50, 30);
	SetFg(D3DXVECTOR3(600, 650, 0.0f), 2, 40, 20);


	SetFg2(D3DXVECTOR3(SCREEN_WIDTH / 2 + 10, 588, 0.0f), 0, 30, 470);

	SetFg2(D3DXVECTOR3(-10, 645, 0.0f), 4, 250, 120);
	SetFg2(D3DXVECTOR3(SCREEN_WIDTH + 10, 645, 0.0f), 5, 250, 120);

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, Result_TEXTURENAME000, &g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice, Result_TEXTURENAME001, &g_pTextureResult[1]);


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RESULT_TYPE,
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
	pVtx[1].pos = D3DXVECTOR3(400, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, 150, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(400, 150, 0.0f);

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
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�̍��W
	pVtx[4].pos = D3DXVECTOR3(50, 0, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(50, SCREEN_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//1.0f�ŌŒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//���_�J���[
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffResult->Unlock();
}

//=============================================================================
// �^�C�g���I������
//=============================================================================
void UnInitResult(void)
{
	//�w�i�I��
	UnInitBG();

	//�u���b�N�I��
	UnInitBlock();

	//�A�C�e���̏I��
	UnInitItem();

	//�n�C�X�R�A�I��
	UnInitHighScore();

	//�A�C�e���̐��Z�I��
	UnInitResultItem();

	//�A���C�̐��Z������
	UnInitResultArey();

	//�E�H�b�J�̐��Z�I��
	UnInitResultVodka();

	//�E�H�b�J�̐��Z�I��
	UnInitResultSatsu();

	//���U���g���̃A�C�e���X�R�A
	UnInitResultItemAll();

	//�O�i
	UnInitFg();

	//�O�i
	UnInitFg2();

	for (int nCntResult = 0; nCntResult <= MAX_RESULT_TYPE; nCntResult++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}

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
	//�w�i�X�V
	UpdateBG();

	//�u���b�N�̍X�V
	UpdateBlock();

	

	//�A�C�e���̍X�V
	UpdateItem();

	//�n�C�X�R�A�I��
	UpdateHighScore();

	//�A�C�e���̐��Z�X�V
	UpdateResultItem();

	//�A���C�̐��Z������
	UpdateResultArey();

	//�E�H�b�J�̐��Z�X�V
	UpdateResultVodka();

	//�E�H�b�J�̐��Z�X�V
	UpdateResultSatsu();



	//���U���g���̃A�C�e���X�R�A
	UpdateResultItemAll();

	//�O�i
	UpdateFg();

	//�O�i
	UpdateFg2();

	FADE pFade;
	pFade = *GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			//���[�h�I��
			//SetMode(MODE_TITLE);
			SetFade(MODE_RANKING);
			PlaySound(SOUND_LABEL_SE_DECIDE);
		}
	}

	g_nCntNextScene++;

	if (g_nCntNextScene % 680 == 0)
	{
		if (pFade == FADE_NONE)
		{

			//���[�h�I��
			SetFade(MODE_RANKING);

		}
	}
}

//=============================================================================
// �^�C�g���`�揈��
//=============================================================================
void DrawResult(void)
{
	//�w�i�`��
	DrawBG();



	//�u���b�N�`��
	DrawBlock();

	//�O�i
	DrawFg();

	//�O�i
	DrawFg2();

	//�A�C�e���`��
	DrawItem();

	//�n�C�X�R�A�`��
	DrawHighScore();

	//�A�C�e���̐��Z�`��
	DrawResultItem();

	//�A���C�̐��Z������
	DrawResultArey();

	//�E�H�b�J�̐��Z�`��
	DrawResultVodka();

	//�E�H�b�J�̐��Z�`��
	DrawResultSatsu();

	//���U���g���̃A�C�e���X�R�A
	DrawResultItemAll();



	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

								//�f�o�C�X���擾����
	pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntResult = 0; nCntResult < MAX_RESULT_TYPE; nCntResult++)
	{

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureResult[nCntResult]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResult * 4, 2);
	}
}