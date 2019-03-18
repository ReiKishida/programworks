//=============================================================================
//
// ����ʏ��� [briefing.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "briefing.h"
#include "fade.h"
#include "sound.h"
#include "inputx.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BRIEFING (2)

#define BRIEFING_TEXTURENAME	"DATA/TEXTURE/btiefing.png"	//�u���[�t�B���O�̃e�N�X�`����
#define BRIEFING_TEXTURENAME_2	"DATA/TEXTURE/press_enter2.png"	//�u���[�t�B���O�̃e�N�X�`����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBriefing[MAX_BRIEFING] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBriefing = NULL;	//���_�o�b�t�@�ւ̃|�C���^

BRIEFING g_aBriefing[MAX_BRIEFING];

JoyState g_JoyStateBriefing;

//=============================================================================
// �u���[�t�B���O����������
//=============================================================================
void InitBriefing(void)
{
	LPDIRECT3DDEVICE9 pDevice;



	//�f�o�C�X�̎擾
	pDevice = GetDevice();


	//��ސݒ�
	g_aBriefing[0].nType = 0;
	g_aBriefing[1].nType = 1;

	//�ʒu������
	g_aBriefing[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aBriefing[1].pos = D3DXVECTOR3(1050, 600, 0.0f);

	//�F��񏉊���
	g_aBriefing[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aBriefing[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BRIEFING_TEXTURENAME, &g_pTextureBriefing[0]);
	D3DXCreateTextureFromFile(pDevice, BRIEFING_TEXTURENAME_2, &g_pTextureBriefing[1]);


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BRIEFING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBriefing,
		NULL);

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

					//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBriefing->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffBriefing->Unlock();

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBriefing->Lock(0, 0, (void**)&pVtx, 0);


	//���_�̍��W
	pVtx[4].pos = D3DXVECTOR3(g_aBriefing[1].pos.x - 150, g_aBriefing[1].pos.y - 45, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aBriefing[1].pos.x + 150, g_aBriefing[1].pos.y - 45, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aBriefing[1].pos.x - 150, g_aBriefing[1].pos.y + 45, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aBriefing[1].pos.x + 150, g_aBriefing[1].pos.y + 45, 0.0f);

	//1.0f�ŌŒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//���_�J���[
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBriefing->Unlock();

	//�Q�[���p�b�h�̏��
	g_JoyStateBriefing.nJoypadState = INPUT_JOYSTATE_NOTPUSH;


}

//=============================================================================
// �u���[�t�B���O�I������
//=============================================================================
void UnInitBriefing(void)
{


	for (int nCntBriefing = 0; nCntBriefing < MAX_BRIEFING; nCntBriefing++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureBriefing[nCntBriefing] != NULL)
		{
			g_pTextureBriefing[nCntBriefing]->Release();
			g_pTextureBriefing[nCntBriefing] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBriefing != NULL)
	{
		g_pVtxBuffBriefing->Release();
		g_pVtxBuffBriefing = NULL;
	}
}

//=============================================================================
// �u���[�t�B���O�X�V����
//=============================================================================
void UpdateBriefing(void)
{

	//�Q�[���p�b�h��Ԏ擾
	JoyState pJoyState = GetJoystate();


	//���
	XINPUT_STATE state;
	XInputGetState(0, &state);

	FADE pFade;
	pFade = *GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
		{//Enter�L�[�������ꂽ�Ƃ��A�X�^�[�g�{�^�������ꂽ�Ƃ�
			if (g_JoyStateBriefing.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				//���艹
				PlaySound(SOUND_LABEL_SE_DECIDE);

				//�Q�[���ֈړ�
				SetFade(MODE_GAME);
			}
			g_JoyStateBriefing.nJoypadState = INPUT_JOYSTATE_PUSH;
		}
	}

	// ���_���̍쐬
	VERTEX_2D *pVtx;



	//	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBriefing->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);


	//���_�J���[
	pVtx[4].col = g_aBriefing[1].col;
	pVtx[5].col = g_aBriefing[1].col;
	pVtx[6].col = g_aBriefing[1].col;
	pVtx[7].col = g_aBriefing[1].col;

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBriefing->Unlock();



	switch (g_aBriefing[1].state)
	{
	case BRIEFINGSTATE_NONE://��̏��
		g_aBriefing[1].col.a -= 0.02f;
		if (g_aBriefing[1].col.a <= 0.0f)
		{
			g_aBriefing[1].col.a = 0.0f;
			g_aBriefing[1].state = BRIEFINGSTATE_USE;
		}
		break;

	case BRIEFINGSTATE_USE://�g�p���Ă���Ƃ�
		g_aBriefing[1].col.a += 0.02f;
		if (g_aBriefing[1].col.a >= 1.0f)
		{
			g_aBriefing[1].col.a = 1.0f;
			g_aBriefing[1].state = BRIEFINGSTATE_NONE;
		}
		break;
	}

}

//=============================================================================
// �u���[�t�B���O�`�揈��
//=============================================================================
void DrawBriefing(void)
{


	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

								//�f�o�C�X���擾����
	pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBriefing, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBriefing = 0; nCntBriefing < MAX_BRIEFING; nCntBriefing++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBriefing[g_aBriefing[nCntBriefing].nType]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBriefing * 4, 2);
	}

}
