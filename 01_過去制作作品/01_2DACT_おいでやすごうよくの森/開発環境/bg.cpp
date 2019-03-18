//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "bg.h"
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_TYPE_BG		(4)								// �w�i�̎��
#define BG_TEXTURENAME_0	"data/TEXTURE/bg0.jpg"		//�w�i�̃e�N�X�`��3
#define BG_TEXTURENAME_1	"data/TEXTURE/back2-1.png"		//�w�i�̃e�N�X�`��2
#define BG_TEXTURENAME_2	"data/TEXTURE/cloud2.png"		//�w�i�̃e�N�X�`��1
#define BG_TEXTURENAME_3	"data/TEXTURE/back9.png"		//�w�i�̃e�N�X�`��1


#define BG_POS_X		(0)							//�w�i����X���W
#define BG_POS_Y		(0)							//�w�i����Y���W
#define BG_WIDTH		(SCREEN_WIDTH)				//�w�i�̕�
#define BG_HEIGHT		(SCREEN_HEIGHT)				//�w�i�̍���
//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBG[MAX_TYPE_BG] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

D3DXVECTOR2 g_BG[2];

int nCntTime;
//=============================================================================
// ����������
//=============================================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;	//�w�i�̃J�E���g

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	nCntTime = 0;
	for (int nCount = 0; nCount < MAX_TYPE_BG; nCount++)
	{
		g_BG[0] = D3DXVECTOR2(0.0f, 0.0f);
		g_BG[1] = D3DXVECTOR2(0.0f, 0.0f);
		g_BG[2] = D3DXVECTOR2(0.0f, 0.0f);
	}

	// �e�N�X�`���̓ǂݍ���(3�̕�)
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME_0, &g_pTextureBG[0]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME_1, &g_pTextureBG[1]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME_2, &g_pTextureBG[2]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME_3, &g_pTextureBG[3]);

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TYPE_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < MAX_TYPE_BG; nCntBG++)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(0 ,-100 , 0.0f);
		pVtx[1].pos = D3DXVECTOR3( BG_WIDTH ,-100 , 0.0f);
		pVtx[2].pos = D3DXVECTOR3( 0 , BG_HEIGHT , 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BG_WIDTH , BG_HEIGHT, 0.0f);

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








		//���_�̍��W
		pVtx[4].pos = D3DXVECTOR3(0, 50, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(BG_WIDTH , 50, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(0, SCREEN_HEIGHT -90 , 0.0f);
		pVtx[7].pos = D3DXVECTOR3(BG_WIDTH , SCREEN_HEIGHT - 90, 0.0f);

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
		pVtx[4].tex = D3DXVECTOR2(0.0, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);







		pVtx[8].pos = D3DXVECTOR3(0, -50, 0.0f);
		pVtx[9].pos = D3DXVECTOR3(BG_WIDTH, -50, 0.0f);
		pVtx[10].pos = D3DXVECTOR3(0, BG_HEIGHT - 150, 0.0f);
		pVtx[11].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT -150, 0.0f);

		//1.0f�ŌŒ�
		pVtx[8].rhw = 1.0f;
		pVtx[9].rhw = 1.0f;
		pVtx[10].rhw = 1.0f;
		pVtx[11].rhw = 1.0f;

		//���_�J���[
		pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W
		pVtx[8].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[9].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[10].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[11].tex = D3DXVECTOR2(0.0f, 1.0f);




		pVtx[12].pos = D3DXVECTOR3(-20, -20, 0.0f);
		pVtx[13].pos = D3DXVECTOR3(BG_WIDTH + 20  , -20, 0.0f);
		pVtx[14].pos = D3DXVECTOR3(-20, BG_HEIGHT -50, 0.0f);
		pVtx[15].pos = D3DXVECTOR3(BG_WIDTH + 20, BG_HEIGHT -50, 0.0f);

		//1.0f�ŌŒ�
		pVtx[12].rhw = 1.0f;
		pVtx[13].rhw = 1.0f;
		pVtx[14].rhw = 1.0f;
		pVtx[15].rhw = 1.0f;

		//���_�J���[
		pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W
		pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();

	

}

//=============================================================================
// �I������
//=============================================================================
void UnInitBG(void)
{
	

	// �e�N�X�`���̊J��
	for (int nCount = 0; nCount < MAX_TYPE_BG; nCount++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureBG[nCount] != NULL)
		{
			g_pTextureBG[nCount]->Release();
			g_pTextureBG[nCount] = NULL;
		}
	}


	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBG(void)
{
	//int nCounter = 0;

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	
	g_BG[0].x -= 0.002f;
	//g_BG[1].x -= 0.002f;
	g_BG[2].x -= 0.0025f;

	
	nCntTime++;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);
	
			//�e�N�X�`�����W���X�V
			pVtx[0].tex = D3DXVECTOR2(g_BG[0].x, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_BG[0].x - 1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_BG[0].x, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_BG[0].x - 1.0f, 1.0f);

			//pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�

			////�e�N�X�`�����W���X�V
			//pVtx[4].tex = D3DXVECTOR2(g_BG[1].x, 0.0f);
			//pVtx[5].tex = D3DXVECTOR2(g_BG[1].x - 1.0f, 0.0f);
			//pVtx[6].tex = D3DXVECTOR2(g_BG[1].x, 1.0f);
			//pVtx[7].tex = D3DXVECTOR2(g_BG[1].x - 1.0f, 1.0f);
			////pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�

			//�e�N�X�`�����W���X�V
			pVtx[8].tex = D3DXVECTOR2(g_BG[2].x, 0.0f);
			pVtx[9].tex = D3DXVECTOR2(g_BG[2].x - 1.0f, 0.0f);
			pVtx[10].tex = D3DXVECTOR2(g_BG[2].x, 1.0f);
			pVtx[11].tex = D3DXVECTOR2(g_BG[2].x - 1.0f, 1.0f);


			//���_�J���[
			pVtx[0].col = D3DXCOLOR(0.7f - (0.00054f * nCntTime), 0.8f - (0.00085f * nCntTime), 1.0f - (0.0018f * nCntTime), 1.0f);
			pVtx[1].col = D3DXCOLOR(0.7f - (0.00054f * nCntTime), 0.8f - (0.00085f * nCntTime), 1.0f - (0.0018f * nCntTime), 1.0f);
			pVtx[2].col = D3DXCOLOR(0.8f - (0.00055f * nCntTime), 0.9f - (0.0055f * nCntTime), 1.0f - (0.0016f * nCntTime), 1.0f);
			pVtx[3].col = D3DXCOLOR(0.8f - (0.00055f * nCntTime), 0.9f - (0.0055f * nCntTime), 1.0f - (0.0016f * nCntTime), 1.0f);

			//���_�J���[
			pVtx[4].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);
			pVtx[5].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);
			pVtx[6].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);
			pVtx[7].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);


	
			//���_
			pVtx[8].col = D3DXCOLOR(1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f);
			pVtx[9].col = D3DXCOLOR(1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f);
			pVtx[10].col = D3DXCOLOR(1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f);
			pVtx[11].col = D3DXCOLOR(1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f);


			//���_�J���[
			pVtx[12].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);
			pVtx[13].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);
			pVtx[14].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);
			pVtx[15].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();

	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// �|���S���̕`��

		
			// �f�o�C�X�̎擾
			pDevice = GetDevice();

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntBG = 0; nCntBG < MAX_TYPE_BG; nCntBG++)
	{

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBG[nCntBG]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 2);
		
	}
}


