//=============================================================================
//
// �w�i���� [fever.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "fever.h"
#include "main.h"
#include "timer.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	MAX_FEVER		(1)								// �w�i�̎��
#define FEVER_TEXTURENAME_0	"data/TEXTURE/fever.png"		//�w�i�̃e�N�X�`��3


#define BG_POS_X		(0)							//�w�i����X���W
#define BG_POS_Y		(0)							//�w�i����Y���W
#define BG_WIDTH		(SCREEN_WIDTH)				//�w�i�̕�
#define BG_HEIGHT		(SCREEN_HEIGHT)				//�w�i�̍���
//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureFever = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFever = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

FEVER g_Fever[MAX_FEVER];

//=============================================================================
// ����������
//=============================================================================
void InitFever(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntFever;	//�w�i�̃J�E���g

				// �f�o�C�X�̎擾
	pDevice = GetDevice();

	
	g_Fever[0].pos = D3DXVECTOR3(SCREEN_WIDTH * 2, SCREEN_HEIGHT /2, 0.0f);
	g_Fever[1].pos = D3DXVECTOR3(SCREEN_WIDTH * 3, SCREEN_HEIGHT / 2, 0.0f);
	g_Fever[2].pos = D3DXVECTOR3(SCREEN_WIDTH * 4, SCREEN_HEIGHT / 2, 0.0f);

	for (int nCntFever = 0; nCntFever < MAX_FEVER; nCntFever++)
	{
		g_Fever[nCntFever].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Fever[nCntFever].bUse = false;
	}
	

	// �e�N�X�`���̓ǂݍ���(3�̕�)
	D3DXCreateTextureFromFile(pDevice, FEVER_TEXTURENAME_0, &g_pTextureFever);

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_FEVER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFever,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFever->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntFever = 0; nCntFever < MAX_FEVER; nCntFever++)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x - SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y - SCREEN_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x + SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y - SCREEN_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x - SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y + SCREEN_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x + SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y + SCREEN_HEIGHT / 2, 0.0f);

		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);



		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFever->Unlock();



}

//=============================================================================
// �I������
//=============================================================================
void UnInitFever(void)
{


	
		//�e�N�X�`���̔j��
		if (g_pTextureFever != NULL)
		{
			g_pTextureFever->Release();
			g_pTextureFever = NULL;
		}
	


		// ���_�o�b�t�@�̊J��
		if (g_pVtxBuffFever != NULL)
		{
			g_pVtxBuffFever->Release();
			g_pVtxBuffFever = NULL;
		}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFever(void)
{
	TIMER pTimer;
	pTimer = GetTimer();

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	if (pTimer.nTime <= 13)
	{

		//PlaySound(SOUND_LABEL_SE_TRUMPET);

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffFever->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntFever = 0; nCntFever < 1; nCntFever++)
		{

			g_Fever[nCntFever].bUse = true;

			g_Fever[nCntFever].move.x -= 0.055f;

			g_Fever[nCntFever].pos.x += g_Fever[nCntFever].move.x;

			
		}
		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffFever->Unlock();
	}

	if (pTimer.nTime != 30 && pTimer.nTime != 20 && pTimer.nTime != 0 && pTimer.nTime  %  10 == 0)
	{

		PlaySound(SOUND_LABEL_SE_TRUMPET);
	}




	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFever->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFever = 0; nCntFever < MAX_FEVER; nCntFever++)
	{

		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x - SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y - SCREEN_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x + SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y - SCREEN_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x - SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y + SCREEN_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x + SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y + SCREEN_HEIGHT / 2, 0.0f);

		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFever->Unlock();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFever(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// �|���S���̕`��


	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffFever, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureFever);


	for (int nCntFever = 0; nCntFever < MAX_FEVER; nCntFever++)
	{

	

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFever * 4, 2);

	}
}


