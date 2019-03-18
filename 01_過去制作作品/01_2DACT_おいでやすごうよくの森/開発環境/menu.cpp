//************************************************************************************************************************
//
// �|�[�Y���j���[���� [PauseMenu.cpp]
// Author : Kishida Rei
//
//************************************************************************************************************************
#include "menu.h"
#include "input.h"
#include "game.h"
#include "fade.h"
#include "sound.h"
#include "bg.h"
#include "block.h"
#include "fg.h"
#include "fg2.h"

//*****************************************************************************************************************************************************************************
// �}�N����`
//*****************************************************************************************************************************************************************************
#define	MAX_TYPE_MENU		(3)								// �L�[�̎��
#define MENU_TEXTURENAME_0	"data/TEXTURE/gamemode.png"			//�L�[�̃e�N�X�`��1
#define MENU_TEXTURENAME_1	"data/TEXTURE/menuranking.png"		//�L�[�̃e�N�X�`��2
#define MENU_TEXTURENAME_2	"data/TEXTURE/quit.png"				//�L�[�̃e�N�X�`��3




#define MENU_POS_X		(300.0f)							//�w�i����X���W
#define MENU_POS_Y		(100.0f)							//�w�i����Y���W


#define MENU_GAMEMODE (0)		//�R���e�B�j���[
#define MENU_MENU_RANKING (1)		//���g���C
#define MENU_QUIT (2)		//modori



//*****************************************************************************************************************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureMenu[MAX_TYPE_MENU] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMenu = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

PAUSEMENU g_aMenu[MAX_TYPE_MENU];
int g_nNowMenuSelect;    //���݂̑I�����

int nCnt;

//************************************************************************************************************************
// ����������
//************************************************************************************************************************
void InitMenu(void)
{

	//�w�i������
	InitBG();

	//�u���b�N������
	InitBlock();

	nCnt = 0;

	SetBlock(D3DXVECTOR3(0.0f, 580, 0), 1280.0f, 150.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f);

	//�O�i
	InitFg();
	InitFg2();
	


	SetFg(D3DXVECTOR3(270, 660, 0.0f), 0, 85, 150);
	SetFg(D3DXVECTOR3(810, 660, 0.0f), 1, 75, 150);
	SetFg(D3DXVECTOR3(1100, 650, 0.0f), 0, 65, 150);
	SetFg2(D3DXVECTOR3(SCREEN_WIDTH / 2, 600, 0.0f), 0, 50, 500);

	SetFg2(D3DXVECTOR3(-10, 645, 0.0f), 4, 250, 120);
	SetFg2(D3DXVECTOR3(SCREEN_WIDTH + 10, 645, 0.0f), 5, 250, 120);


	LPDIRECT3DDEVICE9 pDevice;
	int nCntMenu;	//�L�[�̃J�E���g

	// �f�o�C�X�̎擾
	pDevice = GetDevice();


	g_nNowMenuSelect = GAMEMODE;
	
		g_aMenu[GAMEMODE].pos = D3DXVECTOR3(640, 170, 0);
		g_aMenu[MENU_RANKING].pos = D3DXVECTOR3(640, 400, 0);
		g_aMenu[QUIT].pos = D3DXVECTOR3(640, 610, 0);

		g_aMenu[GAMEMODE].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		g_aMenu[MENU_RANKING].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		g_aMenu[QUIT].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		g_aMenu[GAMEMODE].nType = 0;
		g_aMenu[MENU_RANKING].nType = 1;
		g_aMenu[QUIT].nType = 2;


		g_aMenu[GAMEMODE].state = MENUSTATE_NORMAL;
		g_aMenu[MENU_RANKING].state = MENUSTATE_NORMAL;
		g_aMenu[QUIT].state = MENUSTATE_NORMAL;

		g_aMenu[GAMEMODE].uState = MENUSTATE_NONE;
		g_aMenu[MENU_RANKING].uState = MENUSTATE_NONE;
		g_aMenu[QUIT].uState = MENUSTATE_NONE;

	// �e�N�X�`���̓ǂݍ���(�S�̕�)
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME_0, &g_apTextureMenu[MENU_GAMEMODE]);
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME_1, &g_apTextureMenu[MENU_MENU_RANKING]);
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME_2, &g_apTextureMenu[MENU_QUIT]);




	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TYPE_MENU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMenu,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMenu = 0; nCntMenu < MAX_TYPE_MENU; nCntMenu++)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aMenu[nCntMenu].pos.x - MENU_POS_X, g_aMenu[nCntMenu].pos.y - MENU_POS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aMenu[nCntMenu].pos.x + MENU_POS_X, g_aMenu[nCntMenu].pos.y - MENU_POS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aMenu[nCntMenu].pos.x - MENU_POS_X, g_aMenu[nCntMenu].pos.y + MENU_POS_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aMenu[nCntMenu].pos.x + MENU_POS_X, g_aMenu[nCntMenu].pos.y + MENU_POS_Y, 0.0f);

		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
		
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMenu->Unlock();
}

//************************************************************************************************************************
// �I������
//************************************************************************************************************************
void UnInitMenu(void)
{
	//�w�i�I��
	UnInitBG();

	//�u���b�N�I��
	UnInitBlock();

	UnInitFg();
	UnInitFg2();
	
	int nCntTex;

	// �e�N�X�`���̊J��
	for (nCntTex = 0; nCntTex < MAX_TYPE_MENU; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureMenu[nCntTex] != NULL)
		{
			g_apTextureMenu[nCntTex]->Release();
			g_apTextureMenu[nCntTex] = NULL;
		}
	}


	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffMenu != NULL)
	{
		g_pVtxBuffMenu->Release();
		g_pVtxBuffMenu = NULL;
	}
}

//************************************************************************************************************************
// �X�V����
//************************************************************************************************************************
void UpdateMenu(void)
{
	//�w�i�X�V
	UpdateBG();

	//�u���b�N�X�V
	UpdateBlock();

	UpdateFg();
	UpdateFg2();
	


	nCnt++;


	if (nCnt % 1800 == 0)
	{
		InitBG();
	}

	//g_nNowMenuSelect = GAMEMODE;
	PAUSECHECK *pPause;
	pPause = GetPauseCheck();

	FADE pFade;
	pFade = *GetFade();



	PushMenu();

	VERTEX_2D *pVtx;
			


	if (g_aMenu[g_nNowMenuSelect].state == MENUSTATE_SELECT)
	{
		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
		switch (g_aMenu[g_nNowMenuSelect].uState)
		{
		case MENUSTATE_NONE:
			g_aMenu[g_nNowMenuSelect].col.a -= 0.02f;
			if (g_aMenu[g_nNowMenuSelect].col.a <= 0.0f)
			{
				g_aMenu[g_nNowMenuSelect].col.a = 0.0f;
				g_aMenu[g_nNowMenuSelect].uState = MENUSTATE_USE;
			}
			break;

		case MENUSTATE_USE:
			g_aMenu[g_nNowMenuSelect].col.a += 0.02f;
			if (g_aMenu[g_nNowMenuSelect].col.a >= 1.0f)
			{
				g_aMenu[g_nNowMenuSelect].col.a = 1.0f;
				g_aMenu[g_nNowMenuSelect].uState = MENUSTATE_NONE;
			}
			break;
		}
		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffMenu->Unlock();
	}







	
			if (GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_UP) == true)
			{//W�L�[
				PlaySound(SOUND_LABEL_SE_SELECT);
				g_nNowMenuSelect = (g_nNowMenuSelect + (MENU_MAX - 1)) % MENU_MAX;//�I����Ԃ���グ��
				//PushPauseMenu();
				
			}


			else if (GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_DOWN) == true) 
			{//S�L�[
				PlaySound(SOUND_LABEL_SE_SELECT);
				g_nNowMenuSelect = (g_nNowMenuSelect + 1 ) % MENU_MAX;//�I����Ԃ��������
				//PushPauseMenu();
				
			}

			else if (GetKeyboardTrigger(DIK_RETURN) == true)
			{//RETURN�L�[
				//PushPauseMenu();
				switch (g_nNowMenuSelect)
				{//���ݑI�𒆂̏�Ԃɂ���ď����𕪊�
				case GAMEMODE://�Q�[���I�𒆂Ȃ�
					if (pFade == FADE_NONE)
					{
						if (GetKeyboardTrigger(DIK_RETURN) == true)
						{
							PlaySound(SOUND_LABEL_SE_DECIDE);
							SetFade(MODE_TUTO);
						}
					}
					break;


				case MENU_RANKING://�ݒ�I�𒆂Ȃ�
					if (pFade == FADE_NONE)
					{
						if (GetKeyboardTrigger(DIK_RETURN) == true)
						{
							PlaySound(SOUND_LABEL_SE_DECIDE);
							SetFade(MODE_MENURANKING);
						}
					}
					break;

				case QUIT://�ݒ�I�𒆂Ȃ�
					if (pFade == FADE_NONE)
					{
						if (GetKeyboardTrigger(DIK_RETURN) == true)
						{
							PlaySound(SOUND_LABEL_SE_DECIDE);
							SetFade(MODE_TITLE);
						}
					}
					break;
				}
			}







			if (g_nNowMenuSelect == GAMEMODE)
			{//�I����
				g_aMenu[GAMEMODE].state = MENUSTATE_SELECT;
				// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
				g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (GAMEMODE * 4);

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);

				//���_�o�b�t�@���A�����b�N����
				g_pVtxBuffMenu->Unlock();
			}
			if (g_nNowMenuSelect != GAMEMODE)
			{//�ʏ펞
				g_aMenu[GAMEMODE].state = MENUSTATE_NORMAL;

				// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
				g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (GAMEMODE * 4);

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

				//���_�o�b�t�@���A�����b�N����
				g_pVtxBuffMenu->Unlock();
			}


			if (g_nNowMenuSelect == MENU_RANKING)
			{//�I����
				g_aMenu[MENU_RANKING].state = MENUSTATE_SELECT;

				// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
				g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (MENU_RANKING * 4);

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[4].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);

				//���_�o�b�t�@���A�����b�N����
				g_pVtxBuffMenu->Unlock();
			}
			if (g_nNowMenuSelect != MENU_RANKING)
			{//�ʏ펞
				g_aMenu[MENU_RANKING].state = MENUSTATE_NORMAL;

				// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
				g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (MENU_RANKING * 4);

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

				//���_�o�b�t�@���A�����b�N����
				g_pVtxBuffMenu->Unlock();
			}

			if (g_nNowMenuSelect == QUIT)
			{//�I����
			 // ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
				g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (QUIT * 4);

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);

				//���_�o�b�t�@���A�����b�N����
				g_pVtxBuffMenu->Unlock();
			}
			if (g_nNowMenuSelect != QUIT)
			{//�ʏ펞
			 // ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
				g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (QUIT * 4);

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

				//���_�o�b�t�@���A�����b�N����
				g_pVtxBuffMenu->Unlock();
			}


			if (g_aMenu[g_nNowMenuSelect].state == MENUSTATE_SELECT)
			{
				// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
				g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
				switch (g_aMenu[g_nNowMenuSelect].uState)
				{
				case MENUSTATE_NONE:
					g_aMenu[g_nNowMenuSelect].col.a -= 0.02f;
					if (g_aMenu[g_nNowMenuSelect].col.a <= 0.0f)
					{
						g_aMenu[g_nNowMenuSelect].col.a = 0.0f;
						g_aMenu[g_nNowMenuSelect].uState = MENUSTATE_USE;
					}
					break;

				case MENUSTATE_USE:
					g_aMenu[g_nNowMenuSelect].col.a += 0.02f;
					if (g_aMenu[g_nNowMenuSelect].col.a >= 1.0f)
					{
						g_aMenu[g_nNowMenuSelect].col.a = 1.0f;
						g_aMenu[g_nNowMenuSelect].uState = MENUSTATE_NONE;
					}
					break;
				}
				// ���_�o�b�t�@���A�����b�N����
				g_pVtxBuffMenu->Unlock();
			}



}
	

//************************************************************************************************************************
// �`�揈��
//************************************************************************************************************************
void DrawMenu(void)
{
	//�w�i�`��
	DrawBG();

	//�u���b�N�`��
	DrawBlock();

	DrawFg2();
	DrawFg();


	LPDIRECT3DDEVICE9 pDevice;
	// �|���S���̕`��

			// �f�o�C�X�̎擾
			pDevice = GetDevice();

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffMenu, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntMenu = 0; nCntMenu < MAX_TYPE_MENU; nCntMenu++)
	{
		

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureMenu[g_aMenu[nCntMenu].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMenu * 4, 2);
		}
	

}


//************************************************************************************************************************
// �L�[����
//************************************************************************************************************************
//void PushPauseMenu(int nCntMenu)
void PushMenu(void)
{
	PAUSECHECK *pPause;
	pPause = GetPauseCheck();

	FADE pFade;
	pFade = *GetFade();

	//VERTEX_2D *pVtx;




	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//�G���^�[�L�[�������ꂽ��

		switch (g_nNowMenuSelect)
		{//���ݑI�𒆂̏�Ԃɂ���ď����𕪊�
		case GAMEMODE://�Q�[���I�𒆂Ȃ�
			pPause->bPause = false;
			break;


		case MENU_RANKING://�ݒ�I�𒆂Ȃ�
			if (pFade == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);
					SetFade(MODE_GAME);
				}
			}
			break;

		case QUIT://�ݒ�I�𒆂Ȃ�
			if (pFade == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);
					SetFade(MODE_TITLE);
				}
			}
			break;
		}
	}
}
	
	
		
	
	
	
//************************************************************************************************************************
// �L�[�摜�̐ݒ�
//************************************************************************************************************************
void SetMenu(D3DXVECTOR3 pos, int nType)
{
	
}


//************************************************************************************************************************
// �L�[�̎擾
//************************************************************************************************************************
PAUSEMENU *GetMenu(void)
{
	return &g_aMenu[0];
}





