//=============================================================================
//
// �A�C�e������ [Reticle.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "reticle.h"
#include "player.h"
#include "main.h"
#include "input.h"
#include "game.h"
#include "camera.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RETICLE		"data/TEXTURE/a.png"	// �ǂݍ��ރe�N�X�`���t�@�C����


//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureReticle = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffReticle = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

RETICLE g_Reticle;	//�u���b�N



//=============================================================================
// ����������
//=============================================================================
void InitReticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();
	

	// �A�C�e���̏��̏�����
	
		g_Reticle.pos = D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 -55.0f, 0.0f);
		g_Reticle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
		g_Reticle.fWidth = 150.0f;
		g_Reticle.fHeight = 150.0f;

		g_Reticle.fLength = sqrtf((g_Reticle.fWidth / 2 + g_Reticle.fWidth / 2) * (g_Reticle.fHeight / 2 + g_Reticle.fHeight / 2));

		g_Reticle.fAngle = atan2f(g_Reticle.fWidth / 2, g_Reticle.fHeight / 2);


	


	
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_RETICLE, &g_pTextureReticle);




	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 ,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffReticle,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffReticle->Lock(0, 0, (void**)&pVtx, 0);

	
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(-D3DX_PI + g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(-D3DX_PI + g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);


		pVtx[1].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(D3DX_PI - g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(D3DX_PI - g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);


		pVtx[2].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(-g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(-g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);


		pVtx[3].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);

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
	g_pVtxBuffReticle->Unlock();


}

//=============================================================================
// �I������
//=============================================================================
void UnInitReticle(void)
{

	
	//�e�N�X�`���̔j��
	if (g_pTextureReticle != NULL)
	{
		g_pTextureReticle->Release();
		g_pTextureReticle = NULL;
	}
	


	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffReticle != NULL)
	{
		g_pVtxBuffReticle->Release();
		g_pVtxBuffReticle = NULL;
	}



}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateReticle(void)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;
	CAMERA *pCamera;
	pCamera = GetCamera();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffReticle->Lock(0, 0, (void**)&pVtx, 0);

	g_Reticle.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2 - 60.0f)+ pCamera->fPosR, 0.0f);

		g_Reticle.rot.x += 0.02f;
			g_Reticle.rot.y += 0.02f;



		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(-D3DX_PI + g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(-D3DX_PI + g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);


		pVtx[1].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(D3DX_PI - g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(D3DX_PI - g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);


		pVtx[2].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(-g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(-g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);


		pVtx[3].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);



	

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffReticle->Unlock();

}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawReticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// �|���S���̕`��

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffReticle, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureReticle);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	
	
}

RETICLE GetReticle(void)
{
	return g_Reticle;
}




