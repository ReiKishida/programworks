////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �J�������� [camera.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "camera.h"
#include "input.h"
#include "main.h"
#include "player.h"
#include "enemy.h"
#include "gamepad.h"
#include "game.h"
#include "effect.h"
#include "sound.h"
#include "tutorial.h"
#include "meshWall.h"
#include "model.h"
#include "billboard.h"
#include "light.h"
#include "MessageWindow.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_DISTANCE	(300.0f)		// ����
#define MAX_HEIGHT		(230)			// �����̋���
#define MAX_SPEED		(10.0f)			// �J�����̃X�s�[�h
#define MAX_ROLL		(0.03f)			// ��]��
#define MAX_DISTANCE_R	(10)			// �v���C���[�ƒ����_�̋���
#define MAX_HEIGHT_R	(70)			// �����_�̍���
#define MAX_CAMERA		(3)
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3	posV;			// ���_
	D3DXVECTOR3	posR;			// �����_
	D3DXVECTOR3	rot;			// ��]��
	float fLength;				// V����
	float fHeight;				// V����
	float fLengthR;				// R����
	float fHeightR;				// R����
	int posX;					// �r���[�|�[�g
	int posY;					// �r���[�|�[�g
	int Width;					// �r���[�|�[�g
	int Hight;					// �r���[�|�[�g
	float MinZ;					// �r���[�|�[�g
	float MaxZ;					// �r���[�|�[�g
	float Viewrot;				// ��p
}CameraInfo;

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
Camera g_camera[MAX_CAMERA];// �J�����̏��
CameraInfo g_cameraInfo[] =
{
	{ D3DXVECTOR3(0.0f, MAX_HEIGHT, -MAX_DISTANCE) ,D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.8f, 0.0f, 0.0f),MAX_DISTANCE,MAX_HEIGHT,MAX_DISTANCE_R,MAX_HEIGHT_R,
	0,0,SCREEN_WIDTH ,SCREEN_HEIGHT,0.0f,1.0f,45.0f },
	{ D3DXVECTOR3(0.0f, MAX_HEIGHT, -MAX_DISTANCE) ,D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.9f, 0.0f, 0.0f),80,0,0,5,
	875,350,600,300,0.0f,1.0f,45.0f },
	{ D3DXVECTOR3(0.0f, MAX_HEIGHT, -MAX_DISTANCE) ,D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.9f, 0.0f, 0.0f),150,0,0,-50,
	int(SCREEN_WIDTH / 2.5f),0,SCREEN_WIDTH / 2,SCREEN_HEIGHT,0.0f,1.0f,45.0f },
};
bool g_bAutoCamera;
int nDamageCameraCounter;
bool bDamageCamera;
float g_fAngle;
bool bPauseCamera;
int g_nTitleCameraCnt;
int g_nRankingEnemyCnt;
//==============================================================================================================================
// �J�����̏���������
//==============================================================================================================================
void InitCamera(void)
{
	for (int nCamera = 0; nCamera < MAX_CAMERA; nCamera++)
	{
		g_camera[nCamera].posV = g_cameraInfo[nCamera].posV;
		g_camera[nCamera].posVOld = g_cameraInfo[nCamera].posV;
		g_camera[nCamera].posR = g_cameraInfo[nCamera].posR;
		g_camera[nCamera].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_camera[nCamera].rot = g_cameraInfo[nCamera].rot;
		g_camera[nCamera].Diffrot = g_cameraInfo[nCamera].rot;
		g_camera[nCamera].Destrot = g_cameraInfo[nCamera].rot;
		g_camera[nCamera].posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCamera].posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCamera].posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCamera].posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCamera].nState = CAMERASTATE_NONE;
		g_camera[nCamera].nStateCounter = 0;
		g_camera[nCamera].fLength = g_cameraInfo[nCamera].fLength;
		g_camera[nCamera].fHeight = g_cameraInfo[nCamera].fHeight;
		g_camera[nCamera].fLengthR = g_cameraInfo[nCamera].fLengthR;
		g_camera[nCamera].fHeightR = g_cameraInfo[nCamera].fHeightR;
		g_camera[nCamera].viewpoart.X = g_cameraInfo[nCamera].posX;
		g_camera[nCamera].viewpoart.Y = g_cameraInfo[nCamera].posY;
		g_camera[nCamera].viewpoart.Width = g_cameraInfo[nCamera].Width;
		g_camera[nCamera].viewpoart.Height = g_cameraInfo[nCamera].Hight;
		g_camera[nCamera].viewpoart.MinZ = g_cameraInfo[nCamera].MinZ;
		g_camera[nCamera].viewpoart.MaxZ = g_cameraInfo[nCamera].MaxZ;
		g_camera[nCamera].AttentionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCamera].fLengthAdd = 0;
		g_camera[nCamera].fHeightWall = 0;
		g_camera[nCamera].PauseOldposV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCamera].PauseOldposR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	bPauseCamera = false;
	g_bAutoCamera = false;
	g_fAngle = 0;
	g_nTitleCameraCnt = 0;
	g_nRankingEnemyCnt = 0;
}

//==============================================================================================================================
// �J�����̏I������
//==============================================================================================================================
void UninitCamera(void)
{
}

//==============================================================================================================================
// �J�����̍X�V����
//==============================================================================================================================
void UpdateCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	MODE Mode;
	Mode = GetMode();
	Player *pPlayer;
	pPlayer = GetPlayer();
	DIJOYSTATE2 *pGamePad = GetgamePadStick();
	for (int nCamera = 0; nCamera < 3; nCamera++)
	{
		if (nCamera == 0)
		{
			g_camera[nCamera].posVOld = g_camera[nCamera].posV;
			if (Mode == MODE_GAME || Mode == MODE_TUTORIAL || Mode == MODE_RESULT)
			{
				if (g_bAutoCamera == false)
				{
					// �J�����̈ړ�
					if (GetPauseView() == true )
					{
						//	if (GetPause() == true || GetPauseTutorial() == true)
						{
							if (pGamePad->lX < -GAMEPAD_DEADZONE || pGamePad->lX > GAMEPAD_DEADZONE || pGamePad->lY < -GAMEPAD_DEADZONE || pGamePad->lY> GAMEPAD_DEADZONE)
							{
								float fGamePagStickAngle = atan2f(float(-pGamePad->lX), float(pGamePad->lY));
								g_camera[nCamera].posV.x += sinf(fGamePagStickAngle - D3DX_PI + g_camera[nCamera].rot.y)*MAX_SPEED;
								g_camera[nCamera].posV.z += cosf(fGamePagStickAngle - D3DX_PI + g_camera[nCamera].rot.y)*MAX_SPEED;
								g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
								g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
							}
							if (pGamePad->lZ < -GAMEPAD_DEADZONE || pGamePad->lZ > GAMEPAD_DEADZONE)
							{// �A�i���O�X�e�B�b�N�̕���[�p�x]�Ɉړ�
							 // �p�x���v�Z����
								g_camera[nCamera].rot.y += float(pGamePad->lZ / (MAX_ZONE*20.0f));
								g_camera[nCamera].posV.x = g_camera[nCamera].posR.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
								g_camera[nCamera].posV.z = g_camera[nCamera].posR.z + cosf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
							}
							if (GetKeyboardPress(DIK_Z) == true)
							{// ���_������
								g_camera[nCamera].rot.y += MAX_ROLL;
								g_camera[nCamera].Destrot.y += MAX_ROLL;
								g_camera[nCamera].posV.x = g_camera[nCamera].posR.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
								g_camera[nCamera].posV.z = g_camera[nCamera].posR.z + cosf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
							}
							if (GetKeyboardPress(DIK_C) == true)
							{// ���_�E����
								g_camera[nCamera].rot.y += -MAX_ROLL;
								g_camera[nCamera].Destrot.y += -MAX_ROLL;
								g_camera[nCamera].posV.x = g_camera[nCamera].posR.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
								g_camera[nCamera].posV.z = g_camera[nCamera].posR.z + cosf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
							}
							if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
							{// ���ړ�
								g_camera[nCamera].posV.x += sinf(-D3DX_PI / 2 + g_camera[nCamera].rot.y)*MAX_SPEED;
								g_camera[nCamera].posV.z += cosf(-D3DX_PI / 2 + g_camera[nCamera].rot.y)*MAX_SPEED;
								g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
								g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
							}
							if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
							{// �E�ړ�
								g_camera[nCamera].posV.x += sinf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*MAX_SPEED;
								g_camera[nCamera].posV.z += cosf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*MAX_SPEED;
								g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
								g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
							}
							if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
							{// �O�ړ�
								g_camera[nCamera].posV.x += sinf(0 + g_camera[nCamera].rot.y)*MAX_SPEED;
								g_camera[nCamera].posV.z += cosf(0 + g_camera[nCamera].rot.y)*MAX_SPEED;
								g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
								g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
							}
							if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
							{// ���ړ�
								g_camera[nCamera].posV.x += sinf(D3DX_PI + g_camera[nCamera].rot.y)*MAX_SPEED;
								g_camera[nCamera].posV.z += cosf(D3DX_PI + g_camera[nCamera].rot.y)*MAX_SPEED;
								g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
								g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
							}
							if (GetGamePadPress(ELECOM_Y) == true || GetKeyboardPress(DIK_R) == true)
							{
								g_camera[nCamera].posV.y += MAX_SPEED;
								g_camera[nCamera].posR.y += MAX_SPEED;
							}
							if (GetGamePadPress(ELECOM_X) == true || GetKeyboardPress(DIK_F) == true)
							{
								g_camera[nCamera].posV.y -= MAX_SPEED;
								g_camera[nCamera].posR.y -= MAX_SPEED;
							}
							if (GetKeyboardPress(DIK_T) == true)
							{// I��������
							 // �p�x���v�Z����
								g_camera[nCamera].rot.z += -0.02f;
								g_camera[nCamera].posV.y = g_camera[nCamera].posR.y + cosf(g_camera[nCamera].rot.z + g_camera[nCamera].rot.x) * g_camera[nCamera].fHeight;
							}
							if (GetKeyboardPress(DIK_G) == true)
							{// K��������
							 // �p�x���v�Z����
								g_camera[nCamera].rot.z += 0.02f;
								g_camera[nCamera].posV.y = g_camera[nCamera].posR.y + cosf(g_camera[nCamera].rot.z + g_camera[nCamera].rot.x) * g_camera[nCamera].fHeight;
							}
							if (pGamePad->lRz > GAMEPAD_DEADZONE || pGamePad->lRz < -GAMEPAD_DEADZONE)
							{// �A�i���O�X�e�B�b�N�̕���[�p�x]�Ɉړ�
							 // �p�x���v�Z����
								g_camera[nCamera].rot.x += float(pGamePad->lRz / (MAX_ZONE*50.0f));
								g_camera[nCamera].posV.y = g_camera[nCamera].posR.y + cosf(g_camera[nCamera].rot.x) * g_camera[nCamera].fHeight;
							}
							bPauseCamera = true;
						}
					}
					else
					{
						if (bPauseCamera == false)
						{
							g_camera[nCamera].PauseOldposV = g_camera[nCamera].posV;
							g_camera[nCamera].PauseOldposR = g_camera[nCamera].posR;
							g_camera[nCamera].PauseOldrot = g_camera[nCamera].rot;
						}
						if (bPauseCamera == true)
						{
							g_camera[nCamera].posVDest = g_camera[nCamera].PauseOldposV;
							g_camera[nCamera].posRDest = g_camera[nCamera].PauseOldposR;
							g_camera[nCamera].rot = g_camera[nCamera].PauseOldrot;
							bPauseCamera = false;
						}
#if 0
						if (GetKeyboardPress(DIK_Q) == true)
						{// �����_������
						 //	g_camera[nCamera].rot.y += -MAX_ROLL;
							g_camera[nCamera].Destrot.y += -MAX_ROLL;
							g_camera[nCamera].posV.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
							g_camera[nCamera].posV.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
						}
						if (GetKeyboardPress(DIK_E) == true)
						{// �����_�E����
						 //	g_camera[nCamera].rot.y += MAX_ROLL;
							g_camera[nCamera].Destrot.y += MAX_ROLL;
							g_camera[nCamera].posV.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
							g_camera[nCamera].posV.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
						}
#endif
						if (GetKeyboardPress(DIK_Z) == true)
						{// ���_������
						 //g_camera[nCamera].rot.y += MAX_ROLL;
							g_camera[nCamera].Destrot.y += MAX_ROLL;
							g_camera[nCamera].posV.x = g_camera[nCamera].posR.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
							g_camera[nCamera].posV.z = g_camera[nCamera].posR.z + cosf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
						}
						if (GetKeyboardPress(DIK_C) == true)
						{// ���_�E����
						 //	g_camera[nCamera].rot.y += -MAX_ROLL;
							g_camera[nCamera].Destrot.y += -MAX_ROLL;
							g_camera[nCamera].posV.x = g_camera[nCamera].posR.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
							g_camera[nCamera].posV.z = g_camera[nCamera].posR.z + cosf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
						}

						if (pGamePad->lZ < -GAMEPAD_DEADZONE || pGamePad->lZ > GAMEPAD_DEADZONE)
						{// �A�i���O�X�e�B�b�N�̕���[�p�x]�Ɉړ�
						 // �p�x���v�Z����
						 //	g_camera[nCamera].rot.y += float(pGamePad->lZ / (MAX_ZONE*20.0f));
							g_camera[nCamera].Destrot.y += float(pGamePad->lZ / (MAX_ZONE*20.0f));
							g_camera[nCamera].posV.x = g_camera[nCamera].posR.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
							g_camera[nCamera].posV.z = g_camera[nCamera].posR.z + cosf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
						}

						//---------------- //�㉺���_// ----------------------------------------------//
						if (pGamePad->lRz > GAMEPAD_DEADZONE || pGamePad->lRz < -GAMEPAD_DEADZONE)
						{// �A�i���O�X�e�B�b�N�̕���[�p�x]�Ɉړ�
						 // �p�x���v�Z����
							g_camera[nCamera].Destrot.x += float(pGamePad->lRz / (MAX_ZONE*50.0f));
							g_camera[nCamera].posV.y = g_camera[nCamera].posR.y + cosf(g_camera[nCamera].rot.x) * g_camera[nCamera].fHeight;
							g_camera[nCamera].posV.z = g_camera[nCamera].posR.z + cosf(g_camera[nCamera].rot.x) * g_camera[nCamera].fLength;
						}
						if (GetKeyboardPress(DIK_R) == true)
						{// I��������
						 // �p�x���v�Z����
							g_camera[nCamera].Destrot.x += -0.02f;
							g_camera[nCamera].posV.y = g_camera[nCamera].posR.y + cosf(g_camera[nCamera].rot.x) * g_camera[nCamera].fHeight;
							g_camera[nCamera].posV.z = g_camera[nCamera].posR.z + cosf(g_camera[nCamera].rot.x) * g_camera[nCamera].fLength;
						}
						if (GetKeyboardPress(DIK_F) == true)
						{// K��������
						 // �p�x���v�Z����
							g_camera[nCamera].Destrot.x += 0.02f;
							g_camera[nCamera].posV.y = g_camera[nCamera].posR.y + cosf(g_camera[nCamera].rot.x) * g_camera[nCamera].fHeight;
							g_camera[nCamera].posV.z = g_camera[nCamera].posR.z + cosf(g_camera[nCamera].rot.x) * g_camera[nCamera].fLength;
						}
						Enemy *pEnemy = GetEnemy();
						float fEnemyPlayerLength = 0;
						float fEnemyPlayerLengthMin = 10000000;
						float fRockLength = 0;
						bool bLock = false;
						for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
						{

							if (pEnemy[nCntEnemy].bUse == true && pEnemy[nCntEnemy].State != ENEMY_STATE_DEATH)
							{
								float fPosX = ((sinf(g_camera[nCamera].rot.y) * 300 + pPlayer->pos.x) - pEnemy[nCntEnemy].pos.x);
								float fPosZ = ((cosf(g_camera[nCamera].rot.y) * 300 + pPlayer->pos.z) - pEnemy[nCntEnemy].pos.z);
								fEnemyPlayerLength = fPosX*fPosX + fPosZ*fPosZ;	// �v���C���[�ƓG�̋���
								fRockLength = 500 * 500;		// Active�͈�
								if (fRockLength > fEnemyPlayerLength &&
									-fRockLength < fEnemyPlayerLength)
								{
									bLock = true;
									if (fEnemyPlayerLengthMin >= fEnemyPlayerLength)
									{// ���̍ŏ��l
										fEnemyPlayerLengthMin = fEnemyPlayerLength;
										if (pPlayer->bAttention == false)
										{
											pPlayer->nIdyLockEnemy = nCntEnemy;
										}
									}
								}
							}
						}
						if (GetGamePadTrigger(BUTTON_L1) == true || GetKeyboardTrigger(DIK_L) == true)
						{
							if (pPlayer->bAttention == false)
							{
								if (bLock == true && pEnemy[pPlayer->nIdyLockEnemy].State != ENEMY_STATE_DEATH&&pPlayer->state != PLAYERSTATE_DEATH)
								{
									if (fRockLength > fEnemyPlayerLengthMin &&
										-fRockLength < fEnemyPlayerLengthMin)
									{// �ǔ�
										PlaySound(SOUND_LABEL_SE_LOCKON);
										pPlayer->bAttention = true;
										SetEffect(pEnemy[pPlayer->nIdyLockEnemy].pos + D3DXVECTOR3(0.0f, pEnemy[pPlayer->nIdyLockEnemy].fRadius, 0.0f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), pEnemy[pPlayer->nIdyLockEnemy].fRadius, 50, EFFECTTYPE_AIM);

									}
								}
							}
							else
							{
								pPlayer->bAttention = false;
							}
						}
						if (pPlayer->bAttention == true)
						{// ���ڂ��Ă���
							if (pEnemy[pPlayer->nIdyLockEnemy].bUse == true)
							{// �G�ƃv���C���[�̊p�x���߂�
								float fAngle = atan2f(pEnemy[pPlayer->nIdyLockEnemy].pos.x - pPlayer->pos.x, pEnemy[pPlayer->nIdyLockEnemy].pos.z - pPlayer->pos.z);
								g_camera[nCamera].Destrot.y = fAngle;
							}
							if (pEnemy[pPlayer->nIdyLockEnemy].State == ENEMY_STATE_DEATH || pEnemy[pPlayer->nIdyLockEnemy].bUse == false)
							{
								pPlayer->bAttention = false;
							}

						}
						else
						{
							if (g_camera[nCamera].nState != CAMERASTATE_BOSS_MOVIE)
							{
								g_camera[nCamera].fLength += (MAX_DISTANCE - g_camera[nCamera].fLength)*0.1f;
								if (g_camera[nCamera].fLength >= MAX_DISTANCE)
								{
									g_camera[nCamera].fLength = MAX_DISTANCE;
								}
								g_camera[nCamera].AttentionPos.x -= 2;
								if (g_camera[nCamera].AttentionPos.x <= 0)
								{
									g_camera[nCamera].AttentionPos.x = 0;
								}
								g_camera[nCamera].fHeight += (MAX_HEIGHT - g_camera[nCamera].fHeight)*0.1f;
								if (g_camera[nCamera].fHeight >= MAX_HEIGHT)
								{
									g_camera[nCamera].fHeight = MAX_HEIGHT;
								}
								g_camera[nCamera].fLengthR += (MAX_DISTANCE_R - g_camera[nCamera].fLengthR)*0.1f;
								if (g_camera[nCamera].fLengthR >= MAX_DISTANCE_R)
								{
									g_camera[nCamera].fLengthR = MAX_DISTANCE_R;
								}
								g_camera[nCamera].fHeightR += (MAX_HEIGHT_R - g_camera[nCamera].fHeightR)*0.1f;
								if (g_camera[nCamera].fHeightR >= MAX_HEIGHT_R)
								{
									g_camera[nCamera].fHeightR = MAX_HEIGHT_R;
								}
							}
						}

						// -----------------------/ x �̍��� /-----------------------------------------//
						if (g_camera[nCamera].Destrot.x >= D3DX_PI / 2.5f)
						{// ���E�n
							g_camera[nCamera].Destrot.x = D3DX_PI / 2.5f;
						}
						if (g_camera[nCamera].Destrot.x <= 0)
						{// ���E�n
							g_camera[nCamera].Destrot.x = 0;
						}
						if (g_camera[nCamera].Destrot.x > D3DX_PI)
						{// ������3.14�𒴂���Ȃ�
							g_camera[nCamera].Destrot.x -= D3DX_PI * 2.0f;
						}
						if (g_camera[nCamera].Destrot.x < -D3DX_PI)
						{// ������-3.14�𒴂���Ȃ�
							g_camera[nCamera].Destrot.x += D3DX_PI * 2.0f;
						}
						g_camera[nCamera].Diffrot.x = g_camera[nCamera].Destrot.x - g_camera[nCamera].rot.x;
						if (g_camera[nCamera].Diffrot.x > D3DX_PI)
						{// ������3.14�𒴂���Ȃ�
							g_camera[nCamera].Diffrot.x -= D3DX_PI * 2.0f;
						}
						if (g_camera[nCamera].Diffrot.x < -D3DX_PI)
						{// ������-3.14�𒴂���Ȃ�
							g_camera[nCamera].Diffrot.x += D3DX_PI * 2.0f;
						}
						g_camera[nCamera].rot.x += g_camera[nCamera].Diffrot.x*0.15f;
						if (g_camera[nCamera].rot.x > D3DX_PI)
						{// ���݂̊p�x��3.14�𒴂���Ȃ�
							g_camera[nCamera].rot.x -= D3DX_PI * 2.0f;
						}
						if (g_camera[nCamera].rot.x < -D3DX_PI)
						{// ���݂̊p�x��-3.14�𒴂���Ȃ�
							g_camera[nCamera].rot.x += D3DX_PI * 2.0f;
						}
						// -----------------------/ y �̍��� /-----------------------------------------//
						if (g_camera[nCamera].Destrot.y > D3DX_PI)
						{// ������3.14�𒴂���Ȃ�
							g_camera[nCamera].Destrot.y -= D3DX_PI * 2.0f;
						}
						if (g_camera[nCamera].Destrot.y < -D3DX_PI)
						{// ������-3.14�𒴂���Ȃ�
							g_camera[nCamera].Destrot.y += D3DX_PI * 2.0f;
						}
						g_camera[nCamera].Diffrot.y = g_camera[nCamera].Destrot.y - g_camera[nCamera].rot.y;
						if (g_camera[nCamera].Diffrot.y > D3DX_PI)
						{// ������3.14�𒴂���Ȃ�
							g_camera[nCamera].Diffrot.y -= D3DX_PI * 2.0f;
						}
						if (g_camera[nCamera].Diffrot.y < -D3DX_PI)
						{// ������-3.14�𒴂���Ȃ�
							g_camera[nCamera].Diffrot.y += D3DX_PI * 2.0f;
						}
						g_camera[nCamera].rot.y += g_camera[nCamera].Diffrot.y*0.15f;
						if (g_camera[nCamera].rot.y > D3DX_PI)
						{// ���݂̊p�x��3.14�𒴂���Ȃ�
							g_camera[nCamera].rot.y -= D3DX_PI * 2.0f;
						}
						if (g_camera[nCamera].rot.y < -D3DX_PI)
						{// ���݂̊p�x��-3.14�𒴂���Ȃ�
							g_camera[nCamera].rot.y += D3DX_PI * 2.0f;
						}
						if (g_camera[nCamera].nState == CAMERASTATE_NONE)
						{// �ʏ펞
							if (GetGameState() == GAMESTATE_BOSS&&pPlayer->state != PLAYERSTATE_DEATH)
							{
								g_camera[nCamera].fLengthAdd = 100;
							}
							else
							{
								g_camera[nCamera].fLengthAdd = 0;
							}
							// ���_�ړ�
							g_camera[nCamera].posV.x = pPlayer->pos.x + sinf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*g_camera[nCamera].AttentionPos.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * (g_camera[nCamera].fLength + g_camera[nCamera].fLengthAdd);
							g_camera[nCamera].posV.y = pPlayer->pos.y + cosf(g_camera[nCamera].rot.x) * (g_camera[nCamera].fHeight + g_camera[nCamera].fHeightWall);			// �p�x�̉��Z
							g_camera[nCamera].posV.z = pPlayer->pos.z + cosf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*g_camera[nCamera].AttentionPos.x + cosf(g_camera[nCamera].rot.y - D3DX_PI) * (g_camera[nCamera].fLength + g_camera[nCamera].fLengthAdd);		// �p�x�̉��Z																																										// �v���C���[�̏����O�̒����_�ǔ�
							g_camera[nCamera].posRDest.x = pPlayer->pos.x + sinf(D3DX_PI / 2 + g_camera[nCamera].rot.y) * g_camera[nCamera].AttentionPos.x + sinf(pPlayer->rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
							g_camera[nCamera].posRDest.y = pPlayer->pos.y + cosf(pPlayer->rot.x) * g_camera[nCamera].fHeightR;
							g_camera[nCamera].posRDest.z = pPlayer->pos.z + cosf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*g_camera[nCamera].AttentionPos.x + cosf(pPlayer->rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
							g_camera[nCamera].posRDiff = g_camera[nCamera].posRDest - g_camera[nCamera].posR;// ���݂̊p�x�ƖړI�̊p�x�̍���
							g_camera[nCamera].posR.x += g_camera[nCamera].posRDiff.x * 0.3f;				// �p�x�̉��Z
							g_camera[nCamera].posR.y += g_camera[nCamera].posRDiff.y * 0.1f;				// �p�x�̉��Z
							g_camera[nCamera].posR.z += g_camera[nCamera].posRDiff.z * 0.3f;				// �p�x�̉��Z
						}
						if (g_camera[nCamera].nState == CAMERASTATE_CHANGESTAGE)
						{// �X�e�[�W�ړ���
							g_camera[nCamera].posVDest.x = pPlayer->pos.x + sinf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*g_camera[nCamera].AttentionPos.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
							g_camera[nCamera].posVDest.y = pPlayer->pos.y + cosf(g_camera[nCamera].rot.x) * (g_camera[nCamera].fHeight);			// �p�x�̉��Z
							g_camera[nCamera].posVDest.z = pPlayer->pos.z + cosf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*g_camera[nCamera].AttentionPos.x + cosf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;		// �p�x�̉��Z
							g_camera[nCamera].posVDiff = g_camera[nCamera].posVDest - g_camera[nCamera].posV;// ���݂̊p�x�ƖړI�̊p�x�̍���			
							g_camera[nCamera].posV.x += g_camera[nCamera].posVDiff.x* 0.3f;// �p�x�̉��Z
							g_camera[nCamera].posV.y += g_camera[nCamera].posVDiff.y* 0.3f;
							g_camera[nCamera].posV.z += g_camera[nCamera].posVDiff.z* 0.3f;
							g_camera[nCamera].nStateCounter--;
							if (g_camera[nCamera].nStateCounter <= 0)
							{
								g_camera[nCamera].nState = CAMERASTATE_NONE;
							}
							// �v���C���[�̏����O�̒����_�ǔ�
							g_camera[nCamera].posRDest.x = pPlayer->pos.x + sinf(D3DX_PI / 2 + g_camera[nCamera].rot.y) * g_camera[nCamera].AttentionPos.x + sinf(pPlayer->rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
							g_camera[nCamera].posRDest.y = pPlayer->pos.y + cosf(pPlayer->rot.x) * g_camera[nCamera].fHeightR;
							g_camera[nCamera].posRDest.z = pPlayer->pos.z + cosf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*g_camera[nCamera].AttentionPos.x + cosf(pPlayer->rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
							g_camera[nCamera].posRDiff = g_camera[nCamera].posRDest - g_camera[nCamera].posR;// ���݂̊p�x�ƖړI�̊p�x�̍���
							g_camera[nCamera].posR.x += g_camera[nCamera].posRDiff.x * 0.3f;				// �p�x�̉��Z
							g_camera[nCamera].posR.y += g_camera[nCamera].posRDiff.y * 0.3f;				// �p�x�̉��Z
							g_camera[nCamera].posR.z += g_camera[nCamera].posRDiff.z * 0.3f;				// �p�x�̉��Z
						}
						// �v���C���[�̏����O�̒����_�ǔ�
						if (g_camera[nCamera].nState == CAMERASTATE_BOSS_MOVIE)
						{// �{�X�̃J����
							g_camera[nCamera].posV.x = g_camera[nCamera].posR.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
							g_camera[nCamera].posV.y = g_camera[nCamera].posR.y + cosf(g_camera[nCamera].rot.x) * g_camera[nCamera].fHeight;
							g_camera[nCamera].posV.z = g_camera[nCamera].posR.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
							g_camera[nCamera].nStateCounter--;
							if (g_camera[nCamera].nStateCounter <= 0)
							{
								g_camera[nCamera].nState = CAMERASTATE_NONE;
							}
						}
						if (g_camera[nCamera].nState == CAMERASTATE_BOSS_MOVIE_START)
						{
							g_camera[nCamera].posV.x = g_camera[nCamera].posR.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
							g_camera[nCamera].posV.y = g_camera[nCamera].posR.y + cosf(g_camera[nCamera].rot.x) * g_camera[nCamera].fHeight;
							g_camera[nCamera].posV.z = g_camera[nCamera].posR.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;

						}
					}
				}
			}
			else if (Mode == MODE_TITLE)
			{
				if (g_nTitleCameraCnt == 0)
				{// �n�ߏ����ʒu�ݒ�

					g_camera[nCamera].posV = D3DXVECTOR3(0, 400.0f, -2770.0f);
					g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
					g_camera[nCamera].posR.y = g_camera[nCamera].posV.y + cosf(g_camera[nCamera].rot.x - D3DX_PI) * g_camera[nCamera].fHeightR;
					g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
					g_nTitleCameraCnt++;
				}
				if (g_nTitleCameraCnt == 1)
				{// �����ړ�
					Model *pModel = GetModel();
					g_camera[nCamera].rot.x = D3DX_PI / 2;
					g_camera[nCamera].rot.y = 0;
					g_camera[nCamera].posV.x += sinf(0 + g_camera[nCamera].rot.y) * 5;
					g_camera[nCamera].posV.z += cosf(0 + g_camera[nCamera].rot.y) * 5;
					g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLengthR;
					g_camera[nCamera].posR.y = g_camera[nCamera].posV.y + cosf(g_camera[nCamera].rot.x - D3DX_PI) *g_camera[nCamera].fHeightR;
					g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLengthR;
					if (g_camera[nCamera].posV.z >= 7000)
					{// ���ʒu�ɂ���
						g_nTitleCameraCnt++;
					}
				}
				if (g_nTitleCameraCnt == 2)
				{// ��
					g_camera[nCamera].rot.x = D3DX_PI / 2.0f;
					g_camera[nCamera].rot.y = -D3DX_PI / 2;
					g_camera[nCamera].posV = D3DXVECTOR3(2000.0f, 20.0f, 1300.0f);
					g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
					g_camera[nCamera].posR.y = g_camera[nCamera].posV.y + cosf(g_camera[nCamera].rot.x - D3DX_PI) * g_camera[nCamera].fHeightR;
					g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
					g_nTitleCameraCnt++;
				}
				if (g_nTitleCameraCnt == 3)
				{// ��̗���
					g_camera[nCamera].posV.x += sinf(g_camera[nCamera].rot.y) * 5;
					g_camera[nCamera].posV.y += 0.2f;
					g_camera[nCamera].rot.x += 0.0001f;
					g_camera[nCamera].posV.z += cosf(g_camera[nCamera].rot.y) * 5;
					g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLengthR;
					g_camera[nCamera].posR.y = g_camera[nCamera].posV.y + cosf(g_camera[nCamera].rot.x - D3DX_PI) *g_camera[nCamera].fHeightR;
					g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLengthR;
					if (g_camera[nCamera].posV.x <= -200)
					{
						g_nTitleCameraCnt = 4;
					}
				}
				if (g_nTitleCameraCnt == 4)
				{// 
					g_camera[nCamera].rot.x = D3DX_PI / 2.0f;
					g_camera[nCamera].rot.y = D3DX_PI / 2;
					g_camera[nCamera].posV = D3DXVECTOR3(-3650.0f, 600.0f, -3350.0f);
					g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLengthR;
					g_camera[nCamera].posR.y = g_camera[nCamera].posV.y + cosf(g_camera[nCamera].rot.x - D3DX_PI) * g_camera[nCamera].fHeightR;
					g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLengthR;
					g_nTitleCameraCnt++;

				}
				if (g_nTitleCameraCnt == 5)
				{// �O�i
					g_camera[nCamera].rot.y += 0.01f;
					g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLengthR;
					g_camera[nCamera].posR.y = g_camera[nCamera].posV.y + cosf(g_camera[nCamera].rot.x - D3DX_PI) * g_camera[nCamera].fHeightR;
					g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLengthR;
				}
			}
			else if (Mode == MODE_RANKING)
			{
				if (g_nRankingEnemyCnt == 0)
				{
					pPlayer->pos = D3DXVECTOR3(0, 460, 0);
				}
				g_camera[nCamera].rot.x = D3DX_PI;
				g_camera[nCamera].rot.y = 0;
				g_camera[nCamera].fHeightR = 220;
				g_camera[nCamera].posV = D3DXVECTOR3(0, 830, -700);
				g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) *  g_camera[nCamera].fLength;
				g_camera[nCamera].posR.y = g_camera[nCamera].posV.y + cosf(g_camera[nCamera].rot.x) * g_camera[nCamera].fHeightR;
				g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) *  g_camera[nCamera].fLength;

				g_nRankingEnemyCnt++;
				if (g_nRankingEnemyCnt % 60 == 0)
				{
					pPlayer->Destrot.y = (rand() % 628) / 100.0f;
				}
				pPlayer->move.x += sinf(pPlayer->rot.y - D3DX_PI)*pPlayer->MoveSpeed;
				pPlayer->move.z += cosf(pPlayer->rot.y - D3DX_PI)*pPlayer->MoveSpeed;

			}
		}
		if (nCamera == 1)
		{
			g_camera[nCamera].Destrot.y = pPlayer->rot.y - D3DX_PI / 4;
			if (g_camera[nCamera].Destrot.y > D3DX_PI)
			{// ������3.14�𒴂���Ȃ�
				g_camera[nCamera].Destrot.y -= D3DX_PI * 2.0f;
			}
			if (g_camera[nCamera].Destrot.y < -D3DX_PI)
			{// ������-3.14�𒴂���Ȃ�
				g_camera[nCamera].Destrot.y += D3DX_PI * 2.0f;
			}
			g_camera[nCamera].Diffrot.y = g_camera[nCamera].Destrot.y - g_camera[nCamera].rot.y;
			if (g_camera[nCamera].Diffrot.y > D3DX_PI)
			{// ������3.14�𒴂���Ȃ�
				g_camera[nCamera].Diffrot.y -= D3DX_PI * 2.0f;
			}
			if (g_camera[nCamera].Diffrot.y < -D3DX_PI)
			{// ������-3.14�𒴂���Ȃ�
				g_camera[nCamera].Diffrot.y += D3DX_PI * 2.0f;
			}
			g_camera[nCamera].rot.y += g_camera[nCamera].Diffrot.y;
			if (g_camera[nCamera].rot.y > D3DX_PI)
			{// ���݂̊p�x��3.14�𒴂���Ȃ�
				g_camera[nCamera].rot.y -= D3DX_PI * 2.0f;
			}
			if (g_camera[nCamera].rot.y < -D3DX_PI)
			{// ���݂̊p�x��-3.14�𒴂���Ȃ�
				g_camera[nCamera].rot.y += D3DX_PI * 2.0f;
			}

			g_camera[nCamera].posV.x = pPlayer->aModel[3][pPlayer->nWeaponState].mtxWorld._41 + sinf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*g_camera[nCamera].AttentionPos.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * (g_camera[nCamera].fLength + g_camera[nCamera].fLengthAdd);
			g_camera[nCamera].posV.y = pPlayer->aModel[3][pPlayer->nWeaponState].mtxWorld._42 + cosf(g_camera[nCamera].rot.x) * (g_camera[nCamera].fHeight + g_camera[nCamera].fHeightWall);			// �p�x�̉��Z
			g_camera[nCamera].posV.z = pPlayer->aModel[3][pPlayer->nWeaponState].mtxWorld._43 + cosf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*g_camera[nCamera].AttentionPos.x + cosf(g_camera[nCamera].rot.y - D3DX_PI) * (g_camera[nCamera].fLength + g_camera[nCamera].fLengthAdd);		// �p�x�̉��Z																																										// �v���C���[�̏����O�̒����_�ǔ�
			g_camera[nCamera].posRDest.x = pPlayer->aModel[3][pPlayer->nWeaponState].mtxWorld._41 + sinf(D3DX_PI / 2 + g_camera[nCamera].rot.y) * g_camera[nCamera].AttentionPos.x + sinf(pPlayer->rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
			g_camera[nCamera].posRDest.y = pPlayer->aModel[3][pPlayer->nWeaponState].mtxWorld._42 + g_camera[nCamera].fHeightR;
			g_camera[nCamera].posRDest.z = pPlayer->aModel[3][pPlayer->nWeaponState].mtxWorld._43 + cosf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*g_camera[nCamera].AttentionPos.x + cosf(pPlayer->rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
			g_camera[nCamera].posRDiff = g_camera[nCamera].posRDest - g_camera[nCamera].posR;// ���݂̊p�x�ƖړI�̊p�x�̍���
			g_camera[nCamera].posR.x += g_camera[nCamera].posRDiff.x;				// �p�x�̉��Z
			g_camera[nCamera].posR.y += g_camera[nCamera].posRDiff.y;				// �p�x�̉��Z
			g_camera[nCamera].posR.z += g_camera[nCamera].posRDiff.z;				// �p�x�̉��Z
		}
		if (nCamera == 2)
		{
			g_camera[nCamera].Destrot.y = pPlayer->rot.y;
			if (g_camera[nCamera].Destrot.y > D3DX_PI)
			{// ������3.14�𒴂���Ȃ�
				g_camera[nCamera].Destrot.y -= D3DX_PI * 2.0f;
			}
			if (g_camera[nCamera].Destrot.y < -D3DX_PI)
			{// ������-3.14�𒴂���Ȃ�
				g_camera[nCamera].Destrot.y += D3DX_PI * 2.0f;
			}
			g_camera[nCamera].Diffrot.y = g_camera[nCamera].Destrot.y - g_camera[nCamera].rot.y;
			if (g_camera[nCamera].Diffrot.y > D3DX_PI)
			{// ������3.14�𒴂���Ȃ�
				g_camera[nCamera].Diffrot.y -= D3DX_PI * 2.0f;
			}
			if (g_camera[nCamera].Diffrot.y < -D3DX_PI)
			{// ������-3.14�𒴂���Ȃ�
				g_camera[nCamera].Diffrot.y += D3DX_PI * 2.0f;
			}
			g_camera[nCamera].rot.y += g_camera[nCamera].Diffrot.y;
			if (g_camera[nCamera].rot.y > D3DX_PI)
			{// ���݂̊p�x��3.14�𒴂���Ȃ�
				g_camera[nCamera].rot.y -= D3DX_PI * 2.0f;
			}
			if (g_camera[nCamera].rot.y < -D3DX_PI)
			{// ���݂̊p�x��-3.14�𒴂���Ȃ�
				g_camera[nCamera].rot.y += D3DX_PI * 2.0f;
			}

			g_camera[nCamera].posV.x = pPlayer->aModel[0][pPlayer->nWeaponState].mtxWorld._41 + sinf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*g_camera[nCamera].AttentionPos.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * (g_camera[nCamera].fLength + g_camera[nCamera].fLengthAdd);
			g_camera[nCamera].posV.y = pPlayer->aModel[0][pPlayer->nWeaponState].mtxWorld._42 + cosf(g_camera[nCamera].rot.x) * (g_camera[nCamera].fHeight + g_camera[nCamera].fHeightWall);			// �p�x�̉��Z
			g_camera[nCamera].posV.z = pPlayer->aModel[0][pPlayer->nWeaponState].mtxWorld._43 + cosf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*g_camera[nCamera].AttentionPos.x + cosf(g_camera[nCamera].rot.y - D3DX_PI) * (g_camera[nCamera].fLength + g_camera[nCamera].fLengthAdd);		// �p�x�̉��Z																																										// �v���C���[�̏����O�̒����_�ǔ�
			g_camera[nCamera].posRDest.x = pPlayer->aModel[0][pPlayer->nWeaponState].mtxWorld._41 + sinf(D3DX_PI / 2 + g_camera[nCamera].rot.y) * g_camera[nCamera].AttentionPos.x + sinf(pPlayer->rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
			g_camera[nCamera].posRDest.y = pPlayer->aModel[0][pPlayer->nWeaponState].mtxWorld._42 + cosf(pPlayer->rot.x) * g_camera[nCamera].fHeightR;
			g_camera[nCamera].posRDest.z = pPlayer->aModel[0][pPlayer->nWeaponState].mtxWorld._43 + cosf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*g_camera[nCamera].AttentionPos.x + cosf(pPlayer->rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
			g_camera[nCamera].posRDiff = g_camera[nCamera].posRDest - g_camera[nCamera].posR;// ���݂̊p�x�ƖړI�̊p�x�̍���
			g_camera[nCamera].posR.x += g_camera[nCamera].posRDiff.x;				// �p�x�̉��Z
			g_camera[nCamera].posR.y += g_camera[nCamera].posRDiff.y;				// �p�x�̉��Z
			g_camera[nCamera].posR.z += g_camera[nCamera].posRDiff.z;				// �p�x�̉��Z
		}
	}
}

//==============================================================================================================================
// �J�����̐ݒ菈��
//==============================================================================================================================
void SetCamera(int nIdxCamera)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
												// �r���[�|�[�g
	pDevice->SetViewport(&g_camera[nIdxCamera].viewpoart);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera[nIdxCamera].mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_camera[nIdxCamera].mtxProjection, D3DXToRadian(45.0f),
		(float)g_camera[nIdxCamera].viewpoart.Width / (float)g_camera[nIdxCamera].viewpoart.Height,
		10.0f,			// �ŏ�
		30000.0f);		// �ő�

						// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera[nIdxCamera].mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera[nIdxCamera].mtxView);
	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&g_camera[nIdxCamera].mtxView,
		&g_camera[nIdxCamera].posV, &g_camera[nIdxCamera].posR, &g_camera[nIdxCamera].vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera[nIdxCamera].mtxView);

}

//==============================================================================================================================
// �J�����̎擾
//==============================================================================================================================
Camera *GetCamera(void)
{
	return &g_camera[0];
}