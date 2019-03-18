//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "fade.h"
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_POLYGON (2)
#define MAX_PATTERN (16)
#define BG_WIDTH		(SCREEN_WIDTH)				//�w�i�̕�
#define BG_HEIGHT		(SCREEN_HEIGHT)				//�w�i�̍���
#define BG_POS_X		(0)							//�w�i����X���W
#define BG_POS_Y		(0)							//�w�i����Y���W
#define POS_X (0.125f * (g_nPatternAnim % 5))
#define POS_Y (0.5f * (g_nPatternAnim / 5))

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureFade = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

FADE					g_fade;					// �t�F�[�h���
MODE					g_modeNext;				// ���̃��[�h
D3DXCOLOR				g_colorFade;			// �t�F�[�h�F
D3DXCOLOR				g_colorFade2;			// �t�F�[�h�F
D3DXVECTOR3           g_posfade;

int g_nCounterAnim;		//�A�j���[�V�����J�E���^�[
int g_nPatternAnim;		//�A�j���[�V�����p�^�[��No.

//=============================================================================
// ����������
//=============================================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/load.png", &g_pTextureFade);


	// �l�̏�����
	g_fade = FADE_IN;
	
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//������ʂɂ��Ă���
	g_colorFade2 = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//������ʂɂ��Ă���
	g_posfade = D3DXVECTOR3(SCREEN_WIDTH / 2 + 350.0f, SCREEN_HEIGHT / 2 + 280.0f, 0.0f);
	// ���_���̐ݒ�
	MakeVertexFade(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitFade(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFade(void)
{
	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)
		{

			g_colorFade.a -= 0.02f;	//��ʂ𓧖��ɂ��Ă���
			g_colorFade2.a -= 0.02f;	//��ʂ𓧖��ɂ��Ă���
			if (g_colorFade.a <= 0.00f)
			{
				g_colorFade.a = 0.0f;
				g_colorFade2.a = 0.0f;

				g_fade = FADE_NONE;	//�������Ă��Ȃ����
			}
		}

		else if (g_fade == FADE_OUT)
		{
			g_colorFade.a += 0.02f;	//��ʂ�s������
			g_colorFade2.a += 0.02f;	//��ʂ�s������
			
			
			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_colorFade2.a = 1.0f;
				g_fade = FADE_IN;	//�������Ă��Ȃ����
				//���[�h�̐ݒ�
				SetMode(g_modeNext);
			}
		}

		

				
				
			
			
			
			

			if ((g_nCounterAnim % 40) == 0)
			{
				g_nPatternAnim = (g_nPatternAnim + 1) % MAX_PATTERN;
			}
			//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);
			//���_�J���[
			pVtx[0].col = g_colorFade;
			pVtx[1].col = g_colorFade;
			pVtx[2].col = g_colorFade;
			pVtx[3].col = g_colorFade;

			pVtx[4].col = g_colorFade2;
			pVtx[5].col = g_colorFade2;
			pVtx[6].col = g_colorFade2;
			pVtx[7].col = g_colorFade2;
			//�e�N�X�`�����W
			//�e�N�X�`�����W�̍X�V
			pVtx[0].tex = D3DXVECTOR2(POS_X, POS_Y);
			pVtx[1].tex = D3DXVECTOR2(POS_X + 0.125f, POS_Y);
			pVtx[2].tex = D3DXVECTOR2(POS_X, POS_Y + 0.5f);
			pVtx[3].tex = D3DXVECTOR2(POS_X + 0.125f, POS_Y + 0.5f);
			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffFade->Unlock();

		
	}

}

//=============================================================================
// �^�C�g�����
//=============================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, 0);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureFade);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	// ���_�o�b�t�@�̏���ݒ�
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);


	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(-250.0f, -70.0f, 0.0f) + g_posfade;
	pVtx[1].pos = D3DXVECTOR3(250.0f, -70.0f, 0.0f) + g_posfade;
	pVtx[2].pos = D3DXVECTOR3(-250.0f, 70.0f, 0.0f) + g_posfade;
	pVtx[3].pos = D3DXVECTOR3(250.0f, 70.0f, 0.0f) + g_posfade;

	//1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);


	//���_�̍��W
	pVtx[4].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(BG_WIDTH, BG_POS_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(BG_POS_X, BG_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);

	//1.0f�ŌŒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//���_�J���[
	pVtx[4].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//�e�N�X�`�����W
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffFade->Unlock();
}

//=============================================================================
// �t�F�[�h�̐ݒ�
//=============================================================================
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	//������ʂɂ��Ă���
	g_colorFade2 = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//������ʂɂ��Ă���


}

//=============================================================================
// �t�F�[�h�̎擾
//=============================================================================
FADE *GetFade(void)
{
	return &g_fade;
}

