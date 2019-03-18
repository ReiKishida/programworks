//=============================================================================
//
// �g�s�b�N���� [Topics.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "topics.h"
#include "player.h"
#include "main.h"
#include "input.h"
#include "game.h"
#include "sound.h"
#include "cursor.h"
#include "inputx.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_TYPE_TOPICS		(6)								// �g�s�b�N�̎��
#define TOPICS_TEXTURENAME_0	"data/TEXTURE/mail.png"		//�g�s�b�N�̃e�N�X�`��1
#define TOPICS_TEXTURENAME_1	"data/TEXTURE/news.png"		//�g�s�b�N�̃e�N�X�`��2
#define TOPICS_TEXTURENAME_2	"data/TEXTURE/circle.png"		//�g�s�b�N�̃e�N�X�`��2
#define TOPICS_TEXTURENAME_3	"data/TEXTURE/mail_01.png"		//�g�s�b�N�̃e�N�X�`��2
#define TOPICS_TEXTURENAME_4	"data/TEXTURE/mail_flame2.png"		//�g�s�b�N�̃e�N�X�`��2
#define TOPICS_TEXTURENAME_5	"data/TEXTURE/mail_flame3.png"		//�g�s�b�N�̃e�N�X�`��2




//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureTopics[MAX_TYPE_TOPICS] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTopics = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
TOPICS g_aTopics[MAX_TOPICS];	//�g�s�b�N
JoyState g_JoyStateTopics;



//=============================================================================
// ����������
//=============================================================================
void InitTopics(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTopics;	//�g�s�b�N�̃J�E���g

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//�Q�[���p�b�h�̏��
	g_JoyStateTopics.nJoypadState = INPUT_JOYSTATE_NOTPUSH;


	
	// �g�s�b�N�̏��̏�����
	for(int nCntTopics = 0; nCntTopics < MAX_TOPICS; nCntTopics++)
	{
		g_aTopics[nCntTopics].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);	
		g_aTopics[nCntTopics].nType = 0;
		g_aTopics[nCntTopics].bUse = false;
		g_aTopics[nCntTopics].fHeight = 0;
		g_aTopics[nCntTopics].fWidth = 0;
		g_aTopics[nCntTopics].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTopics[nCntTopics].state = MAILSTATE_UNREAD;
		g_aTopics[nCntTopics].FallenPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	}

	

	// �e�N�X�`���̓ǂݍ���(�S�̕�)
	D3DXCreateTextureFromFile(pDevice, TOPICS_TEXTURENAME_0, &g_apTextureTopics[0]); //��
	D3DXCreateTextureFromFile(pDevice, TOPICS_TEXTURENAME_1, &g_apTextureTopics[1]); //���[���P
	D3DXCreateTextureFromFile(pDevice, TOPICS_TEXTURENAME_2, &g_apTextureTopics[2]); //���[��2
	D3DXCreateTextureFromFile(pDevice, TOPICS_TEXTURENAME_3, &g_apTextureTopics[3]); //���[��2
	D3DXCreateTextureFromFile(pDevice, TOPICS_TEXTURENAME_4, &g_apTextureTopics[4]); //���[��2
	D3DXCreateTextureFromFile(pDevice, TOPICS_TEXTURENAME_5, &g_apTextureTopics[5]); //���[��2


	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TOPICS,
									D3DUSAGE_WRITEONLY,
									FVF_VERTEX_2D,
									D3DPOOL_MANAGED,
									&g_pVtxBuffTopics,
									NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTopics->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntTopics = 0; nCntTopics < MAX_TOPICS; nCntTopics++)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x , g_aTopics[nCntTopics].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x + g_aTopics[nCntTopics].fWidth, g_aTopics[nCntTopics].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x , g_aTopics[nCntTopics].pos.y + g_aTopics[nCntTopics].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x + g_aTopics[nCntTopics].fWidth, g_aTopics[nCntTopics].pos.y + g_aTopics[nCntTopics].fHeight, 0.0f);
		
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
				pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
		
			pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTopics->Unlock();


}

//=============================================================================
// �I������
//=============================================================================
void UnInitTopics(void)
{
	
	int nCntTex;

	// �e�N�X�`���̊J��
	for(nCntTex = 0; nCntTex < MAX_TYPE_TOPICS; nCntTex++)
	{
			//�e�N�X�`���̔j��
			if (g_apTextureTopics[nCntTex] != NULL)
			{
				g_apTextureTopics[nCntTex]->Release();
				g_apTextureTopics[nCntTex] = NULL;
			}
	}


	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTopics != NULL)
	{
		g_pVtxBuffTopics->Release();
		g_pVtxBuffTopics = NULL;
	}

	

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTopics(void)
{
	//�Q�[���p�b�h��Ԏ擾
	JoyState pJoyState = GetJoystate();

	//���
	XINPUT_STATE state;
	XInputGetState(0, &state);

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	int nCntTex = 0;

	CURSOR pCursor;
	pCursor = GetCursor000();

	if (pCursor.bUpdate == false && GetKeyboardTrigger(DIK_RETURN) == TRUE && g_aTopics[4].bUse == true
		||(state.Gamepad.wButtons &XINPUT_GAMEPAD_B && pJoyState.bConnectionJoypad == true && g_aTopics[4].bUse == true && pCursor.bUpdate == true ))
	{
		if (g_JoyStateTopics.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pCursor.bUpdate = true;
			g_aTopics[4].bUse = false;
			//g_aTopics[4].state = MAILSTATE_ALREADY;
		}
		g_JoyStateTopics.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
	if (pCursor.bUpdate == false && GetKeyboardTrigger(DIK_RETURN) == TRUE && g_aTopics[5].bUse == true
		|| (state.Gamepad.wButtons &XINPUT_GAMEPAD_B && pJoyState.bConnectionJoypad == true && g_aTopics[5].bUse == true && pCursor.bUpdate == true))
	{
		if (g_JoyStateTopics.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pCursor.bUpdate = true;
			g_aTopics[5].bUse = false;
			//g_aTopics[5].state = MAILSTATE_ALREADY;
		}
		g_JoyStateTopics.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
	if (pCursor.bUpdate == false && GetKeyboardTrigger(DIK_RETURN) == TRUE && g_aTopics[6].bUse == true
		|| (state.Gamepad.wButtons &XINPUT_GAMEPAD_B && pJoyState.bConnectionJoypad == true && g_aTopics[6].bUse == true && pCursor.bUpdate == true))
	{
		if (g_JoyStateTopics.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pCursor.bUpdate = true;
			g_aTopics[6].bUse = false;
			//g_aTopics[6].state = MAILSTATE_ALREADY;
		}
		g_JoyStateTopics.nJoypadState = INPUT_JOYSTATE_PUSH;

	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTopics->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTopics = 0; nCntTopics < MAX_TOPICS; nCntTopics++)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x, g_aTopics[nCntTopics].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x + g_aTopics[nCntTopics].fWidth, g_aTopics[nCntTopics].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x, g_aTopics[nCntTopics].pos.y + g_aTopics[nCntTopics].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x + g_aTopics[nCntTopics].fWidth, g_aTopics[nCntTopics].pos.y + g_aTopics[nCntTopics].fHeight, 0.0f);

		if (g_aTopics[nCntTopics].type == TOPICSTYPE_MAIL_0_1 || g_aTopics[nCntTopics].type == TOPICSTYPE_MAIL_1_1 || g_aTopics[nCntTopics].type == TOPICSTYPE_NEWS_0_1)
		{
			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		else
		{
			if (g_aTopics[nCntTopics].state == MAILSTATE_UNREAD)
			{
				//�e�N�X�`�����W
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
			}	

		}

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTopics->Unlock();

	if (g_JoyStateTopics.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateTopics.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateTopics.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateTopics.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTopics(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// �|���S���̕`��



	for (int nCntTopics = 0; nCntTopics < MAX_TOPICS; nCntTopics++)
	{
		if (g_aTopics[nCntTopics].bUse == true)
		{
	
			// �f�o�C�X�̎擾
			pDevice = GetDevice();

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffTopics, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

	
		
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureTopics[g_aTopics[nCntTopics].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTopics * 4, 2);
		}
	}
}

void DeleteTopics(int nIndexPolygon)
{
	g_aTopics[nIndexPolygon].bUse = false;
}





//=============================================================================
// �g�s�b�N�̐ݒ�
//=============================================================================
void SetTopics(D3DXVECTOR3 pos, TOPICSTYPE Type ,int nType, float fHeight, float fWidth)
{
	int nCntTopics;
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTopics->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntTopics = 0; nCntTopics < MAX_TOPICS; nCntTopics++)
	{
			if (g_aTopics[nCntTopics].bUse == false)	//�g�s�b�N���g�p����Ă��Ȃ�
			{
				g_aTopics[nCntTopics].pos = pos;
			
				g_aTopics[nCntTopics].state = MAILSTATE_UNREAD;

					g_aTopics[nCntTopics].fHeight = fHeight;
					g_aTopics[nCntTopics].fWidth = fHeight;
			

					if (Type == TOPICSTYPE_MAIL_0_1 || Type == TOPICSTYPE_MAIL_1_1 || Type == TOPICSTYPE_NEWS_0_1)
					{
						//�e�N�X�`�����W
						pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
					}

					else
					{
						//�e�N�X�`�����W
						pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
					}


					g_aTopics[nCntTopics].nType = nType;
					g_aTopics[nCntTopics].type = Type;

				//���_�̍��W
				pVtx[0].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x , g_aTopics[nCntTopics].pos.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x + g_aTopics[nCntTopics].fWidth, g_aTopics[nCntTopics].pos.y , 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x , g_aTopics[nCntTopics].pos.y + g_aTopics[nCntTopics].fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x + g_aTopics[nCntTopics].fWidth, g_aTopics[nCntTopics].pos.y + g_aTopics[nCntTopics].fHeight, 0.0f);

	
				g_aTopics[nCntTopics].bUse = true;	//�g�p���Ă����Ԃɂ���

				break;
			}
			pVtx +=  4;


		}
		//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTopics->Unlock();
}

//=============================================================================
// �g�s�b�N�̓����蔻��
//=============================================================================
TOPICSTYPE CollisionTopics(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosold)
{
	TOPICSTYPE Type = TOPICSTYPE_NONE;


	for (int nCntTopics = 0; nCntTopics < MAX_TOPICS; nCntTopics++)
	{


		if (g_aTopics[nCntTopics].bUse == true)
		{

			if (pPos->x >= g_aTopics[nCntTopics].pos.x - g_aTopics[nCntTopics].fWidth
				&& pPos->x <= g_aTopics[nCntTopics].pos.x + g_aTopics[nCntTopics].fWidth
				&&pPos->y >= g_aTopics[nCntTopics].pos.y - g_aTopics[nCntTopics].fHeight
				&& pPos->y <= g_aTopics[nCntTopics].pos.y + g_aTopics[nCntTopics].fHeight
				)
			{

				if (g_aTopics[nCntTopics].type == TOPICSTYPE_MAIL_0)
				{
					Type = TOPICSTYPE_MAIL_0;
				}
				if (g_aTopics[nCntTopics].type == TOPICSTYPE_MAIL_1)
				{
					Type = TOPICSTYPE_MAIL_1;
				}
				if (g_aTopics[nCntTopics].type == TOPICSTYPE_NEWS_0)
				{
					Type = TOPICSTYPE_NEWS_0;
				}
				if (g_aTopics[nCntTopics].type == TOPICSTYPE_MISSION_0)
				{
					Type = TOPICSTYPE_MISSION_0;
				}
			}

		}
	}
	return Type;
}


//=============================================================================
// �g�s�b�N�̎擾
//=============================================================================
TOPICS *GetTopics(void)
{
	return &g_aTopics[0];
}
