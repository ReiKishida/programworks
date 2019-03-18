//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// �`���[�g���A������ [tutorial.cpp]
// Author : �� �a��
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "main.h"
#include "tutorial.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �}�N����`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MAX_TUTORIAL				(4)							// �`���[�g���A���̍ő吔
#define MAX_TUTORIALPAGE			(3)							// �`���[�g���A���̃X���C�h�̖���
#define TUTORIALPAGE_PATTERN_ANIM	float(1.0f / float(MAX_TUTORIALPAGE))	// �y�[�W�̉��Z��
#define TUTORIAL_DISP_NORMAL		(30)						// �ʏ펞�̓_�ł̊Ԋu
#define TUTORIAL_DISP_FAST			(2)							// ���莞�̓_�ł̊Ԋu
#define SLIDE_TIME					(20)						// �X�N���[���X�s�[�h

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �\���̒�`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
typedef enum
{
	TUTORIAL_TEX_MAIN = 0,		// ���C���摜
	TUTORIAL_TEX_PRESS_START,	// �����Ă��������n
	TUTORIAL_TEX_ARROWW_RIGHT,	// ���E
	TUTORIAL_TEX_ARROWW_LEFT,	// ���
	TUTORIAL_TEX_MAX,			// ����
}TutorialTexName;

typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	TUTORIALTYPE type;	// ���
	D3DXCOLOR	col;	// �F
	int nTexType;		// �e�N�X�`���̎��
	float fWidth;			// ����
	float fHeight;		// �c��
	int nMoveCounter;	// �l�X�ȓ����̃J�E���^�[ 
	int nDispSpeed;		// �_�ł̃X�s�[�h
	bool bDisp;			// �\��
	bool bUse;			// �g�p���Ă��邩�ǂ���
}Tutorial;

typedef struct
{
	char *pFilename;	// �t�@�C����
} TutorialTexInfo;

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �v���g�^�C�v�錾
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
void MakeVertexTutorial(LPDIRECT3DDEVICE9 pDevice);

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �O���[�o���ϐ�
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
LPDIRECT3DTEXTURE9 g_pTextureTutorial[TUTORIAL_TEX_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
Tutorial g_aTutorial[MAX_TUTORIAL];								// �`���[�g���A���̏��

// �ǂݍ��ރe�N�X�`�����̐ݒ�
TutorialTexInfo g_apTextureTutorialInfo[TUTORIAL_TEX_MAX] =
{
	{ "data//TEXTURE//tutorial004.png" },		// ���C���摜
	{ "data//TEXTURE//press_enter.png" },		// �����Ă��������n
	{ "data//TEXTURE//arrowRight.png" },		// ���E
	{ "data//TEXTURE//arrowLeft.png" },			// ���
};

int g_nCntPage;				// ���݂̃y�[�W�����J�E���g
int g_nSlideFrameCnt;		// �X���C�h���Ԃ̃J�E���^�[
bool g_bSlideUse;			// �X���C�h�����ǂ���
float g_fSlideTexAnim;		// �X���C�h�̃e�N�X�`���̌��݂̒l
float g_fSlideTexAnimAdd;	// �X���C�h�̃e�N�X�`���̑����l
int g_nDispCounter;			// �_�ł̃J�E���^�[

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �`���[�g���A������������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	// �`���[�g���A���̏��̏�����
	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		g_aTutorial[nCntTutorial].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTutorial[nCntTutorial].type = TUTORIALTYPE_MAIN;
		g_aTutorial[nCntTutorial].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aTutorial[nCntTutorial].nTexType = TUTORIAL_TEX_MAIN;
		g_aTutorial[nCntTutorial].nDispSpeed = TUTORIAL_DISP_NORMAL;
		g_aTutorial[nCntTutorial].fWidth = 0;
		g_aTutorial[nCntTutorial].fHeight = 0;
		g_aTutorial[nCntTutorial].bDisp = false;
		g_aTutorial[nCntTutorial].bUse = false;
	}

	g_nCntPage = 0;		
	g_nSlideFrameCnt = 0;
	g_bSlideUse = false;	
	g_fSlideTexAnim = 0.0f;
	g_fSlideTexAnimAdd = 0.0f;
	g_nDispCounter = 0;

	// �e�N�X�`���̓ǂݍ���
	for (int nCntTutorialTex = 0; nCntTutorialTex < TUTORIAL_TEX_MAX; nCntTutorialTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_apTextureTutorialInfo[nCntTutorialTex].pFilename,
			&g_pTextureTutorial[nCntTutorialTex]);
	}

	// ���_���
	MakeVertexTutorial(pDevice);	

	SetTutorial(D3DXVECTOR3(float(SCREEN_WIDTH / 2), float(SCREEN_HEIGHT / 2), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TUTORIALTYPE_MAIN,(float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	// �v���[�Y�G���^�[
	SetTutorial(D3DXVECTOR3(float(SCREEN_WIDTH -260), SCREEN_HEIGHT -50, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), TUTORIALTYPE_PRESS_START, 480.0f, 80.0f);
	SetTutorial(D3DXVECTOR3(float(SCREEN_WIDTH - 40.0f), float(SCREEN_HEIGHT / 2), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TUTORIALTYPE_ARROW_RIGHT, 70.0f, 200.0f);
	SetTutorial(D3DXVECTOR3(40.0f, float(SCREEN_HEIGHT / 2), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TUTORIALTYPE_ARROW_LEFT, 70.0f, 200.0f);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �`���[�g���A���I������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UninitTutorial(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < TUTORIAL_TEX_MAX; nCntTex++)
	{
		if (g_pTextureTutorial[nCntTex] != NULL)
		{
			g_pTextureTutorial[nCntTex]->Release();
			g_pTextureTutorial[nCntTex] = NULL;
		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �`���[�g���A���X�V����
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateTutorial(void)
{
	FADE pFade;
	// �t�F�[�h���[�h�̎擾
	pFade = GetFade();
	g_nSlideFrameCnt++;
	g_nDispCounter++;

	if (g_bSlideUse == false)
	{ // �X���C�h���Ŗ����ꍇ

		if ((GetKeyboardTrigger(DIK_RIGHT) == true || GetKeyboardTrigger(DIK_D) == true || GetGamePadTrigger(BUTTON_R1) == true
			|| GetPovTrigger(POV_RIGHT) == true || GetGamePadStickTrigger(ANALOG_STICK_RIGHT) == true) && g_nCntPage != MAX_TUTORIALPAGE-1)
		{ // �y�[�W���ő�l�łȂ��ꍇ
			// �y�[�W��i�߂�
			g_nCntPage++;
			g_bSlideUse = true;
			g_nSlideFrameCnt = 0;
			g_fSlideTexAnimAdd = float(TUTORIALPAGE_PATTERN_ANIM / SLIDE_TIME);
			// SE�̍Đ�
			PlaySound(SOUND_LABEL_SE_SELECT);
		}
		else if ((GetKeyboardTrigger(DIK_LEFT) == true || GetKeyboardTrigger(DIK_A) == true || GetGamePadTrigger(BUTTON_L1) == true
			|| GetPovTrigger(POV_LEFT) == true || GetGamePadStickTrigger(ANALOG_STICK_LEFT) == true) && g_nCntPage != 0)
		{ // �y�[�W���ŏ��l�łȂ��ꍇ
			// �y�[�W�߂�
			g_nCntPage--;
			g_bSlideUse = true;
			g_nSlideFrameCnt = 0;
			g_fSlideTexAnimAdd = float(-TUTORIALPAGE_PATTERN_ANIM / SLIDE_TIME);
			// SE�̍Đ�
			PlaySound(SOUND_LABEL_SE_SELECT);
		}

	}

	// �X���C�h�̃e�N�X�`���̒l�ɑ����l�����Z
	g_fSlideTexAnim += g_fSlideTexAnimAdd;

	if (g_nSlideFrameCnt == SLIDE_TIME && g_bSlideUse == true)
	{ // �X���C�h���Ńt���[���J�E���^�[���K��l�ɒB�����ꍇ
		g_nSlideFrameCnt = 0;
		g_bSlideUse = false;
		g_fSlideTexAnimAdd = 0.0f;
		g_fSlideTexAnim = g_nCntPage * TUTORIALPAGE_PATTERN_ANIM;
	}
	
	if (pFade == FADE_NONE)
	{ // �t�F�[�h�����ǂ���
		if (g_nCntPage == MAX_TUTORIALPAGE-1 && (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(ELECOM_A) == true || GetGamePadTrigger(ELECOM_START) == true))
		{ // ����L�[�������ꂽ���ǂ���
			// SE�̍Đ�
			PlaySound(SOUND_LABEL_SE_DECIDE);

			// ���[�h�ݒ�
			SetFade(MODE_GAME);

			for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
			{
				if (g_aTutorial[nCntTutorial].bUse == true)
				{
					if (g_aTutorial[nCntTutorial].type == TUTORIALTYPE_PRESS_START)
					{
						g_aTutorial[nCntTutorial].nDispSpeed = TUTORIAL_DISP_FAST;
					}
				}
			}
		}
	}

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		if (g_aTutorial[nCntTutorial].bUse == true)
		{
			if (g_aTutorial[nCntTutorial].type == TUTORIALTYPE_MAIN)
			{ // �X���C�h
				pVtx[0].tex = D3DXVECTOR2(g_fSlideTexAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_fSlideTexAnim + TUTORIALPAGE_PATTERN_ANIM, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_fSlideTexAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_fSlideTexAnim + TUTORIALPAGE_PATTERN_ANIM, 1.0f);
			}
			else if (g_aTutorial[nCntTutorial].type == TUTORIALTYPE_PRESS_START)
			{ // �v�b�V���{�^��
				if (g_nCntPage == MAX_TUTORIALPAGE - 1 && g_bSlideUse == false)
				{
					if (g_nDispCounter % g_aTutorial[nCntTutorial].nDispSpeed == 0)
					{
						g_aTutorial[nCntTutorial].bDisp = g_aTutorial[nCntTutorial].bDisp ? false : true;
					}
				}
				else
				{
					g_aTutorial[nCntTutorial].bDisp = false;
				}
			}
			else if (g_aTutorial[nCntTutorial].type == TUTORIALTYPE_ARROW_RIGHT)
			{ // �v�b�V���{�^��
				if (g_nCntPage != MAX_TUTORIALPAGE - 1 && g_bSlideUse == false)
				{
					if (g_nDispCounter % g_aTutorial[nCntTutorial].nDispSpeed == 0)
					{
						g_aTutorial[nCntTutorial].bDisp = g_aTutorial[nCntTutorial].bDisp ? false : true;
					}
				}
				else
				{
					g_aTutorial[nCntTutorial].bDisp = false;
				}
			}
			else if (g_aTutorial[nCntTutorial].type == TUTORIALTYPE_ARROW_LEFT)
			{ // �v�b�V���{�^��
				if (g_nCntPage != 0 && g_bSlideUse == false)
				{
					if (g_nDispCounter % g_aTutorial[nCntTutorial].nDispSpeed == 0)
					{
						g_aTutorial[nCntTutorial].bDisp = g_aTutorial[nCntTutorial].bDisp ? false : true;
					}
				}
				else
				{
					g_aTutorial[nCntTutorial].bDisp = false;
				}
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorial->Unlock();
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �`���[�g���A���`�揈��
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		if (g_aTutorial[nCntTutorial].bUse == true)
		{ // �G���g�p����Ă���
			if (g_aTutorial[nCntTutorial].bDisp == true)
			{
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureTutorial[g_aTutorial[nCntTutorial].nTexType]);

				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTutorial * 4, 2);
			}
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// Tutorial�̐ݒ�
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetTutorial(D3DXVECTOR3 pos, D3DXCOLOR col, TUTORIALTYPE type, float fWidth, float fHeight)
{
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++,pVtx+=4)
	{
		if (g_aTutorial[nCntTutorial].bUse == false)
		{ // �^�C�g�����g�p����Ă��Ȃ��ꍇ
		  // ���_�ʒu�̐ݒ�
			g_aTutorial[nCntTutorial].pos = pos;
			g_aTutorial[nCntTutorial].fWidth = fWidth;
			g_aTutorial[nCntTutorial].fHeight = fHeight;
			// �F�̐ݒ�
			g_aTutorial[nCntTutorial].col = col;
			// �`���[�g���A���̎�ނ̐ݒ�
			g_aTutorial[nCntTutorial].type = type;
			g_aTutorial[nCntTutorial].nTexType = type;
			
			 // ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x - g_aTutorial[nCntTutorial].fWidth / 2, g_aTutorial[nCntTutorial].pos.y - g_aTutorial[nCntTutorial].fHeight / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x + g_aTutorial[nCntTutorial].fWidth / 2, g_aTutorial[nCntTutorial].pos.y - g_aTutorial[nCntTutorial].fHeight / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x - g_aTutorial[nCntTutorial].fWidth / 2, g_aTutorial[nCntTutorial].pos.y + g_aTutorial[nCntTutorial].fHeight / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x + g_aTutorial[nCntTutorial].fWidth / 2, g_aTutorial[nCntTutorial].pos.y + g_aTutorial[nCntTutorial].fHeight / 2, 0.0f);
	
			pVtx[0].col = g_aTutorial[nCntTutorial].col;
			pVtx[1].col = g_aTutorial[nCntTutorial].col;
			pVtx[2].col = g_aTutorial[nCntTutorial].col;
			pVtx[3].col = g_aTutorial[nCntTutorial].col;

			if (g_aTutorial[nCntTutorial].type == TUTORIALTYPE_MAIN)
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(TUTORIALPAGE_PATTERN_ANIM, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(TUTORIALPAGE_PATTERN_ANIM, 1.0f);
			}

			g_aTutorial[nCntTutorial].bDisp = true;	// �\����Ԃɂ���
			g_aTutorial[nCntTutorial].bUse = true;	// �g�p�����Ԃɂ���			
			break;	// ����break�͂��Ȃ���
		}
		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffTutorial->Unlock();
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���_���̍쐬
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void MakeVertexTutorial(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TUTORIAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++, pVtx += 4)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorial->Unlock();

}