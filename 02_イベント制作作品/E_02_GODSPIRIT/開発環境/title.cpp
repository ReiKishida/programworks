////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �^�C�g���̏��� [title.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "effect.h"
#include "game.h"
#include "gamepad.h"
#include "camera.h"
#include "meshField.h"
#include "model.h"
#include "shadow.h"
#include "line.h"
#include "orbit.h"
#include "light.h"
#include "sky.h"
#include "meshWall.h"
#include "meshCylinder.h"
#include "effectUP.h"
#include "billboard.h"
#include "effect2D.h"
#include "meshDome.h"
#include "meshDome2.h"

//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// �u���b�N�̒��S���W
	TITLETYPE nType;			// �u���b�N�̎��
	float fWidth;				// �u���b�N�̕�
	float fHeight;				// �u���b�N�̍���
} TitleInfo;
typedef struct
{
	char *pFileName;
}TitleTexture;
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define ANIM				(0.33334f)								// �A�j���[�V�������W
#define ANIMPATTERN			(3)										// �A�j���[�V�����p�^�[��
#define ANIMSPEED			(5)									// �A�j���[�V�����X�s�[�h
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLETYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
Title g_aTitle[TITLETYPE_MAX];
int g_nPressEnterCounter;
int g_nTitleEndCounter;
int g_nTitleAutoCounter;
bool g_bDecide;
TITLESTATE g_nTitleState;
TitleInfo g_aTitleInfo[] =
{
	//Set    	 pos,								,						nType,	fWidth,	fHeight,	
	{D3DXVECTOR3(900, 250, 0.0f), TITLETYPE_LOGOBG, 600, 600},										// ���S�w�i
	{D3DXVECTOR3(SCREEN_WIDTH / 2 , 300, 0.0f), TITLETYPE_LOGO, 1150, 500},							// ���S
	{D3DXVECTOR3(SCREEN_WIDTH / 2 , 600, 0.0f), TITLETYPE_PRESSENTER, 700, 130 },					// PressEnter
};
TitleTexture g_aTitleTex[TITLETYPE_MAX] =
{
	{"data/TEXTURE/TitleLogoBG000.png"},	// �ǂݍ��ރe�N�X�`��
	{"data/TEXTURE/title001.png"},		// �ǂݍ��ރe�N�X�`��
	{"data/TEXTURE/pressElse000.png"}	,		// �ǂݍ��ރe�N�X�`��
};
//==============================================================================================================================
// �����������i�|���S���j
//==============================================================================================================================
void InitTitle(void)
{
	InitLight();
	InitCamera();
	InitMeshField();
	InitModel();
	InitShadow();
	InitLine();
	InitSky();
	InitMeshWall();

	InitEffect();
	InitEffect2D();

	InitEffectUP();
	InitMeshCylinder();
	InitBillboard();
	SetBillboardFileData("data/BillboardStage1.txt");
	SetStagePos("data/stageTitle.txt");
	// ���b�V���h�[���̏���������
	InitMeshDome();
	InitMeshDome2();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	for (int nTex = 0; nTex < TITLETYPE_MAX; nTex++)		
	{// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				g_aTitleTex[nTex].pFileName,
				&g_pTextureTitle[nTex]);
	}

	// �w�i�̏��̏�����
	for (int nCntTitle = 0; nCntTitle < TITLETYPE_MAX; nCntTitle++)
	{
		g_aTitle[nCntTitle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���W
		g_aTitle[nCntTitle].Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �������W
		g_aTitle[nCntTitle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);// �F
		g_aTitle[nCntTitle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aTitle[nCntTitle].nType = TITLETYPE_LOGOBG;					// �^�C�v
		g_aTitle[nCntTitle].nCounterAnim = 0;						// �A�j���[�V�����J�E���^�[
		g_aTitle[nCntTitle].nPatternAnim = 0;						// �A�j���[�V�����p�^�[��
		g_aTitle[nCntTitle].fWidth = 0;								// ��
		g_aTitle[nCntTitle].fHeight = 0;							// ����
		g_aTitle[nCntTitle].nCounterFade = 0;						// �t�F�[�h�J�E���^�[
		g_aTitle[nCntTitle].nFade = TITLEFADE_FADEOUT;				// ���S�̃t�F�[�h
		g_aTitle[nCntTitle].bUse = false;							// �g�p����Ă��邩�ǂ���
	}
	g_aTitle[TITLETYPE_LOGOBG].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, -0.5f);// �F
	g_aTitle[TITLETYPE_LOGO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);// �F
	g_aTitle[TITLETYPE_PRESSENTER].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);// �F
	g_nTitleState = TITLESTATE_NONE;								// �^�C�g���̏��
	g_nPressEnterCounter = 0;										// �G���^�[�J�E���^�[
	g_nTitleEndCounter = 0;											// �^�C�g���I��邽�߂̃J�E���^�[
	g_nTitleAutoCounter = 0;										// �����J�E���^�[
	g_bDecide = false;												// �n�߂Ă��邩�ǂ���
	// ���_���̍쐬
	MakeVertexTitle(pDevice);

	// �^�C�g���̐ݒ�
	for (int nCntTitle = 0; nCntTitle < sizeof g_aTitleInfo / sizeof(TitleInfo); nCntTitle++)
	{
		SetTitle(g_aTitleInfo[nCntTitle].pos, g_aTitleInfo[nCntTitle].nType, g_aTitleInfo[nCntTitle].fWidth, g_aTitleInfo[nCntTitle].fHeight);
	}

}
//==============================================================================================================================
// �I�������i�|���S���j
//==============================================================================================================================
void UninitTitle(void)
{
	UninitLight();
	UninitCamera();
	UninitShadow();
	UninitLine();
	UninitSky();
	UninitEffectUP();
	UninitMeshCylinder();
	UninitBillboard();
	UninitEffect();
	UninitEffect2D();
	// ���b�V���h�[���̏I������
	UninitMeshDome();
	UninitMeshDome2();

	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < TITLETYPE_MAX; nCntTex++)
	{
		if (g_pTextureTitle[nCntTex] != NULL)
		{
			g_pTextureTitle[nCntTex]->Release();
			g_pTextureTitle[nCntTex] = NULL;
		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}
//==============================================================================================================================
// �X�V�����i�|���S���j
//==============================================================================================================================
void UpdateTitle(void)
{
	UpdateLight();
	UpdateCamera();
	UpdateMeshField();
	UpdateModel();	
	UpdateShadow();
	UpdateLine();
	UpdateOrbit();
	UpdateSky();
	UpdateMeshWall();
	UpdateEffect();
	UpdateEffect2D();
	UpdateEffectUP();
	UpdateMeshCylinder();
	UpdateBillboard();
	// ���b�V���h�[���̍X�V����
	UpdateMeshDome();
	UpdateMeshDome2();

	// �t�F�[�h�̎擾
	FADE pFADE;
	pFADE = GetFade();

	if (g_bDecide == false)
	{
		if (g_aTitle[TITLETYPE_LOGO].col.a >= 1.0f)
		{// ���S����ʒu�ɂȂ�����
			// ����L�[���������ǂ��� Enter�Ŏn�߂�
			for (int nCntKey = 0; nCntKey < BUTTON_MAX; nCntKey++)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(nCntKey) == true)
				{
					g_bDecide = true;
				}

			}
		}
	}
	switch (g_nTitleState)
	{
	case TITLESTATE_NONE:
		g_nTitleAutoCounter++;
		if (pFADE == FADE_NONE)
		{
			if (g_nTitleAutoCounter >= 1500)
			{
				SetFade(MODE_RANKING);
			}
		}
		break;
	case TITLESTATE_END:
		g_nPressEnterCounter++;
		g_nTitleEndCounter++;
		if (g_nTitleEndCounter == 1)
		{
			PlaySound(SOUND_LABEL_SE_JUMP_SPIN);
		}
		for (int nCntEffect = 0; nCntEffect < 10; nCntEffect++)
		{// ���т�G�t�F�N�g
			float nDirection = SetRandomEffect();
			float nDirection2 = SetRandomEffect();
			SetEffect2D( D3DXVECTOR3(SCREEN_WIDTH/2+ sinf(nDirection)* float(rand() % 500), 550 + (float(rand() % 100)),0.0f),
				D3DXVECTOR3(sinf(nDirection) * float((rand() % 10 / 10.0f)), cosf(nDirection) *  float((rand() % 10 / 10.0f)), 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), float(rand() % 100) + 1, 10, EFFECTTYPE_2D_SUNDER);
			SetEffect2D(D3DXVECTOR3(SCREEN_WIDTH / 2 + sinf(nDirection)*float(rand() % 500), 550 + (float(rand() % 100)), 0.0f),
				D3DXVECTOR3(sinf(nDirection) *  float((rand() % 10 / 10.0f)), sinf(nDirection2) *  float((rand() % 10 / 10.0f)),0.0f), 
				D3DXCOLOR(1.0f, 1.0f, 0.0f, float(rand() % 100 / 100.0f)), float(rand() % 100) + 1, 10, EFFECTTYPE_2D_SUNDER);
		}
		break;
	}
	if (pFADE == FADE_NONE)
	{
		if (g_bDecide == true)
		{// ����L�[������
			g_nTitleState = TITLESTATE_END;
			if (g_nTitleEndCounter >= 30)
			{
				SetFade(MODE_TUTORIAL);
			}

		}
	}
//-------------------------/ �^�C�g���̍X�V��� /--------------------------------------------------------------------------//
	VERTEX_2D*pVtx;
	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntTitle = 0; nCntTitle < TITLETYPE_MAX; nCntTitle++, pVtx += 4)
	{
		switch (g_aTitle[nCntTitle].nType)
		{
		case TITLETYPE_LOGOBG:
			if (g_aTitle[nCntTitle].nFade != TITLEFADE_NONE)
			{// ���S�̌��̓_��
				if (g_aTitle[nCntTitle].nFade == TITLEFADE_FADEIN)
				{// ��\�����Ă���
					g_aTitle[nCntTitle].col.a -= 0.01f;
					if (g_aTitle[nCntTitle].col.a <= 0.0f)
					{
						g_aTitle[nCntTitle].nFade = TITLEFADE_FADEOUT;
					}
				}
				if (g_aTitle[nCntTitle].nFade == TITLEFADE_FADEOUT)
				{// �\�����Ă���
					g_aTitle[nCntTitle].col.a += 0.01f;
					if (g_aTitle[nCntTitle].col.a >= 1.5f)
					{
						g_aTitle[nCntTitle].nFade = TITLEFADE_FADEIN;
					}
				}
			}
			for (int nCntKey = 0; nCntKey < BUTTON_MAX; nCntKey++)
			{
				for (int nCntKeyBoard = 0; nCntKeyBoard < NUM_KEY_MAX; nCntKeyBoard++)
				{
					if (GetKeyboardTrigger(nCntKeyBoard) == true || GetGamePadTrigger(nCntKey) == true)
					{
						g_aTitle[nCntTitle].col.a = 1.0f;
					}
				}
			}
			break;
		case TITLETYPE_LOGO:
			g_aTitle[nCntTitle].col.a += 0.005f;
			for (int nCntKey = 0; nCntKey < BUTTON_MAX; nCntKey++)
			{
				for (int nCntKeyBoard = 0; nCntKeyBoard < NUM_KEY_MAX; nCntKeyBoard++)
				{
					if (GetKeyboardTrigger(nCntKeyBoard) == true || GetGamePadTrigger(nCntKey) == true)
					{
						g_aTitle[nCntTitle].col.a = 1.0f;
					}
				}
			}
			break;
		case TITLETYPE_PRESSENTER:
			if (g_aTitle[TITLETYPE_LOGO].col.a >= 1.0f)
			{
				g_nPressEnterCounter++;
				g_nPressEnterCounter = g_nPressEnterCounter % 120;
				if (g_nPressEnterCounter <= 60)
				{
					g_aTitle[nCntTitle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				}
				else
				{
					g_aTitle[nCntTitle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
			}
			break;
		}
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(g_aTitle[nCntTitle].fWidth / 2), -(g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
		pVtx[1].pos = D3DXVECTOR3((g_aTitle[nCntTitle].fWidth / 2), -(g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
		pVtx[2].pos = D3DXVECTOR3(-(g_aTitle[nCntTitle].fWidth / 2), (g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
		pVtx[3].pos = D3DXVECTOR3((g_aTitle[nCntTitle].fWidth / 2), (g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_aTitle[nCntTitle].col;
		pVtx[1].col = g_aTitle[nCntTitle].col;
		pVtx[2].col = g_aTitle[nCntTitle].col;
		pVtx[3].col = g_aTitle[nCntTitle].col;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();


}
//==============================================================================================================================
// �`�揈���i�|���S���j
//==============================================================================================================================
void DrawTitle(void)
{
	SetCamera(0);
	// ���b�V���h�[���̍X�V����
	DrawMeshDome();
	DrawMeshDome2();

	DrawMeshField();
	DrawModel();	
	DrawShadow();
	if (GetDrawDebug()[DRAW_DEBUG_LINE] == true)
	{
		DrawLine();
	}
	DrawOrbit();
//	DrawSky();
	DrawMeshWall();
	DrawEffect2D();
	DrawEffect();
	DrawEffectUP();
//	DrawMeshCylinder();
	DrawBillboard();
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCntTitle = 0; nCntTitle < TITLETYPE_MAX; nCntTitle++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[g_aTitle[nCntTitle].nType]);
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
								nCntTitle * 4,
								2);
	}

}
//==============================================================================================================================
// �^�C�g���̐ݒ�
//==============================================================================================================================
void SetTitle(D3DXVECTOR3 pos, TITLETYPE nType,float fWidth,float fHeight)
{
	VERTEX_2D*pVtx;

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntTitle = 0; nCntTitle < TITLETYPE_MAX; nCntTitle++, pVtx += 4)
	{
		if (g_aTitle[nCntTitle].bUse == false)
		{// �w�i���g�p����Ă��Ȃ�
			g_aTitle[nCntTitle].pos = pos;
			g_aTitle[nCntTitle].Initpos = pos;
			g_aTitle[nCntTitle].nType = nType;
			g_aTitle[nCntTitle].fWidth = fWidth;
			g_aTitle[nCntTitle].fHeight = fHeight;

			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-(g_aTitle[nCntTitle].fWidth / 2), -(g_aTitle[nCntTitle].fHeight/ 2), 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aTitle[nCntTitle].fWidth / 2), -(g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aTitle[nCntTitle].fWidth / 2), (g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aTitle[nCntTitle].fWidth / 2), (g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;

			g_aTitle[nCntTitle].bUse = true;	// �g�p���Ă����Ԃɂ���
			break;
		}
	}
	g_pVtxBuffTitle->Unlock();
}

//==============================================================================================================================
// ���_���̍쐬
//==============================================================================================================================
void MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLETYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < TITLETYPE_MAX; nCntTitle++, pVtx += 4)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(g_aTitle[nCntTitle].fWidth / 2), -(g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
		pVtx[1].pos = D3DXVECTOR3((g_aTitle[nCntTitle].fWidth / 2), -(g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
		pVtx[2].pos = D3DXVECTOR3(-(g_aTitle[nCntTitle].fWidth / 2), (g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
		pVtx[3].pos = D3DXVECTOR3((g_aTitle[nCntTitle].fWidth / 2), (g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// rhw�̐ݒ�
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}
		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_aTitle[nCntTitle].col;
		pVtx[1].col = g_aTitle[nCntTitle].col;
		pVtx[2].col = g_aTitle[nCntTitle].col;
		pVtx[3].col = g_aTitle[nCntTitle].col;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();

}