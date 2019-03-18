////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �v���C���[���� [player.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "model.h"
#include "fade.h"
#include "meshField.h"
#include "meshWall.h"
#include "line.h"
#include "orbit.h"
#include "sound.h"
#include "game.h"
#include "orbit.h"
#include "gamepad.h"
#include "effect.h"
#include "effect2D.h"
#include "spGage.h"
#include "MessageWindow.h"
#include "time.h"
#include "effectUP.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define DOWN_SPEED						(0.01f)	// ������
#define PLAYER_ROT_SPEED_NORMAL			(0.3f)	// �ʏ펞�̊p�x��ς����
#define PLAYER_ROT_SPEED_ATTACK			(0.03f)	// �U�����̊p�x��ς����
#define GRAVITY_SPEED					(0.6f)	// �d�͉��Z�X�s�[�h
#define INIT_JUMP_SPEED					(7)		// �W�����v��
#define INIT_MOVE_SPEED					(0.3f)	// �ړ���
#define BLEND_FRAME						(3)		// �u�����h
#define WARKZONE						(50)	// �����̃]�[��
#define PLAYER_MOTION_MOVE_ZONE			(1.0f)	// �����̃��[�V�����]�[��
#define MAX_SKY							(1.0f)	// �K�[�h�̍ő僉�C�t
#define MAX_CAMERASTATE					(30)
#define DOWN_JUMP_SPEED					(1.5f)	// �W�����v
#define MAX_SPEED						(30.0f)	//
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void SetPlayerVoice( PlayerMotionType motion);
void PlayerLifeZero(int nIdy,float fAngle, float fDamageH, float fDamageV);
//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
Player g_aPlayer[MAX_PLAYER];
PlayerData g_aPlayerData[MAX_PLAYER];
int g_nNumAll = 0;
int g_nCutInTime;
int g_nSpecialPlayer;
bool g_bCutIn;
bool g_bStartMove;
bool g_bStopStart;
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_bStopStart = true;
	// �ʒu�E�����̏����ݒ�
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		g_aPlayer[nCnt].pos = D3DXVECTOR3(0.0f + nCnt * 100, 0.0f, 0.0f);		// ���W
		g_aPlayer[nCnt].posOld = D3DXVECTOR3(0.0f + nCnt * 100, 0.0f, 0.0f);		// �O��̍��W
		g_aPlayer[nCnt].Diffpos = D3DXVECTOR3(0.0f + nCnt * 100, 0.0f, 0.0f);	// ���W�̍���
		g_aPlayer[nCnt].Initpos = D3DXVECTOR3(0.0f + nCnt * 100, 0.0f, 0.0f);	// �������W
		g_aPlayer[nCnt].nMotionType = PLAYER_MOTION_TYPE_NEUTRAL;	// ���[�V�����^�C�v
		g_aPlayer[nCnt].nMotionTypeOld = PLAYER_MOTION_TYPE_MAX;	// �O��̃��[�V�����^�C�v
		g_aPlayer[nCnt].nMotionGetCol = PLAYER_MOTION_TYPE_NEUTRAL;	// ���[�V�����^�C�v
		g_aPlayer[nCnt].nMotionGetColOld = PLAYER_MOTION_TYPE_MAX;	// �O��̃��[�V�����^�C�v
		g_aPlayer[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_aPlayer[nCnt].moveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_aPlayer[nCnt].moveL = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_aPlayer[nCnt].Diffrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]�̍���
		g_aPlayer[nCnt].Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړI�̉�]
		g_aPlayer[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]
		for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
		{// ��
			g_aPlayer[nCnt].bSound[nCntSound] = false;
		}
		;		g_aPlayer[nCnt].state = PLAYERSTATE_NONE;
		g_aPlayer[nCnt].nCntState = 0;
		g_aPlayer[nCnt].pMeshFieldLand = NULL;
		g_aPlayer[nCnt].pModel = NULL;
		g_aPlayer[nCnt].pModelAtkData = NULL;
		g_aPlayer[nCnt].pModelAtk = NULL;
		for (int nbMotion = 0; nbMotion < PLAYER_BOOL_MAX; nbMotion++)
		{
			g_aPlayer[nCnt].bMotionState[nbMotion] = false;
			g_aPlayer[nCnt].bMotionStateOld[nbMotion] = false;
		}
		g_aPlayer[nCnt].bBlend = false;
		g_aPlayer[nCnt].nDamage = 0;
		g_aPlayer[nCnt].bJump = false;							// �W�����v
		g_aPlayer[nCnt].nFlyCounter = 0;						// ���ł��鎞��
		g_aPlayer[nCnt].fRotSpeed = 0.0f;						// ��]�̌����X�s�[�h
		g_aPlayer[nCnt].nAirCounter = 0;						// ���V���̍U������J�E���^�[
		g_aPlayer[nCnt].nLoopCounter = 0;						// ���[�v�J�E���^�[
		g_aPlayer[nCnt].nIdy = nCnt;							// �ԍ�
		g_aPlayer[nCnt].nKeyCounter = 0;		// �L�[��
		g_aPlayer[nCnt].nFrameCounter = 0;		// �t���[����
		g_aPlayer[nCnt].nFrameAllCounter = 0;	// �ő�t���[��
		g_aPlayer[nCnt].nFrame = 0;				// �t���[��
		g_aPlayer[nCnt].bUse = false;
		g_aPlayer[nCnt].bDraw = true;			// �f���Ă邩�ǂ���
	}
	g_aPlayer[0].col = D3DXCOLOR(0.92f, 0.43f, 0.62f, 1.0f);	// �s���N
	g_nCutInTime = 0;
	g_nSpecialPlayer = 0;
	g_bCutIn = false;
	g_bStartMove = false;


	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		for (int nCntParts = 0; nCntParts < g_aPlayerData[nCnt].nNumParts; nCntParts++)
		{// Key - ���� �Ԃ̍����v�Z
			g_aPlayerData[nCnt].aModel[nCntParts].rot = g_aPlayerData[nCnt].aMotion[PLAYER_MOTION_TYPE_NEUTRAL].aKey[0].aPartsKey[nCntParts].rot;
		}
	}
	g_nNumAll = 0;
	SetEffect2D(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_HEIGHT, SCREEN_HEIGHT, 1, EFFECTTYPE_2D_SPEED);
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		for (int nCntModel = 0; nCntModel < g_aPlayerData[nCnt].nNumParts; nCntModel++)
		{
			// ���b�V���̊J��
			if (g_aPlayerData[nCnt].aModel[nCntModel].pMesh != NULL)
			{
				g_aPlayerData[nCnt].aModel[nCntModel].pMesh->Release();
				g_aPlayerData[nCnt].aModel[nCntModel].pMesh = NULL;
			}
			// �}�e���A���̊J��
			if (g_aPlayerData[nCnt].aModel[nCntModel].pBuffMat != NULL)
			{
				g_aPlayerData[nCnt].aModel[nCntModel].pBuffMat->Release();
				g_aPlayerData[nCnt].aModel[nCntModel].pBuffMat = NULL;
			}
			// �e�N�X�`���̊J��
			for (int nCntMat = 0; nCntMat < int(g_aPlayerData[nCnt].aModel[nCntModel].nNumMat); nCntMat++)
			{
				for (int nCntCol = 0; nCntCol < MAX_PLAYER; nCntCol++)
				{
					if (g_aPlayerData[nCnt].aModel[nCntModel].pTextuer[nCntMat] != NULL)
					{
						g_aPlayerData[nCnt].aModel[nCntModel].pTextuer[nCntMat]->Release();
						g_aPlayerData[nCnt].aModel[nCntModel].pTextuer[nCntMat] = NULL;
					}
				}
			}
		}
	}

}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdatePlayer(int nIdy)
{
	MeshField *pMeshField = GetMeshField();
	Camera *pCamera = GetCamera();
	Model *pModel = GetModel();


	if (g_aPlayer[nIdy].bUse)
	{
		// �O��̈ʒu �^�C�v
		g_aPlayer[nIdy].posOld = g_aPlayer[nIdy].pos;
		g_aPlayer[nIdy].nMotionTypeOld = g_aPlayer[nIdy].nMotionType;

		for (int nbMotion = 0; nbMotion < PLAYER_BOOL_MAX; nbMotion++)
		{// �O��̃��[�V����
			g_aPlayer[nIdy].bMotionStateOld[nbMotion] = g_aPlayer[nIdy].bMotionState[nbMotion];
		}
	}
	switch (g_aPlayer[nIdy].state)
	{
	case PLAYERSTATE_NONE:
		break;
	case PLAYERSTATE_DAMAGESTART:
		g_aPlayer[nIdy].state = PLAYERSTATE_DAMAGE;
		break;
	case PLAYERSTATE_DAMAGE:
		g_aPlayer[nIdy].nCntState--;
		if (g_aPlayer[nIdy].nCntState <= 0)
		{
			g_aPlayer[nIdy].state = PLAYERSTATE_NONE;
		}
		break;
	case PLAYERSTATE_FALL:	// ����
		g_aPlayer[nIdy].move.y = 0;
		for (int nbMotion = 0; nbMotion < PLAYER_BOOL_MAX; nbMotion++)
		{
			g_aPlayer[nIdy].bMotionState[nbMotion] = false;
			g_aPlayer[nIdy].bMotionStateOld[nbMotion] = false;
		}
		g_aPlayer[nIdy].nCntState--;
		if (g_aPlayer[nIdy].nCntState <= 0)
		{
			g_aPlayer[nIdy].state = PLAYERSTATE_NONE;
		}
		break;
	case PLAYERSTATE_INVINCIBLE:
		g_aPlayer[nIdy].nCntState--;
		if (g_aPlayer[nIdy].nCntState % 2 == 0)
		{// �_��
			g_aPlayer[nIdy].bDraw = g_aPlayer[nIdy].bDraw ? false : true;
		}
		if (g_aPlayer[nIdy].nCntState <= 0)
		{
			g_aPlayer[nIdy].bDraw = true;
			g_aPlayer[nIdy].state = PLAYERSTATE_NONE;
		}
		break;
	case PLAYERSTATE_DEATH:
		DeleteShadow(g_aPlayer[nIdy].nIdyShadow);
		g_aPlayer[nIdy].move = D3DXVECTOR3(0, 0, 0);
		g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] = true;
		if (g_nNumAll <= 1)
		{
			if (GetGameState() != GAMESTATE_END)
			{
				SetGameState(GAMESTATE_END);
			}
		}
		break;
	}
	switch (pCamera[0].nState)
	{
	case CAMERASTATE_NONE:
		break;
	}
	if (g_aPlayer[nIdy].state != PLAYERSTATE_DEATH)
	{// �����Ă���Ȃ�
		if (GetMode() != MODE_RESULT&&GetGameState() != GAMESTATE_END)
		{
			if (GetGameState() != GAMESTATE_START)
			{
				PlayerController(nIdy);
			}
			// �_���[�W�̎��̌v�Z
			if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] || g_aPlayer[nIdy].state == PLAYERSTATE_DAMAGE)
			{// ������ї͂��܂���������
				if (g_aPlayer[nIdy].fFlyPower.x >= 0.1f ||
					g_aPlayer[nIdy].fFlyPower.x <= -0.1f)
				{// ���������̐�����ї͂�����Ȃ�
					g_aPlayer[nIdy].move.x = g_aPlayer[nIdy].fFlyPower.x;	// ������ї͂����Z
				}
				if (g_aPlayer[nIdy].fFlyPower.z >= 0.1f ||
					g_aPlayer[nIdy].fFlyPower.z <= -0.1f)
				{// ���������̐�����ї͂�����Ȃ�
					g_aPlayer[nIdy].move.z = g_aPlayer[nIdy].fFlyPower.z;	// ������ї͂����Z
				}

				if (g_aPlayer[nIdy].nFlyCounter > 0)
				{// ������ԃJ�E���^�[��0���傫��������
					g_aPlayer[nIdy].fFlyPower.y -= 0.5f;
					if (g_aPlayer[nIdy].fFlyPower.y > 0.0f)
					{// �W�����v�͂��܂���������
						g_aPlayer[nIdy].move.y = g_aPlayer[nIdy].fFlyPower.y;			// �㏸���Ă���
					}
					if (rand() % 2 == 0)
					{// ���G�t�F�N�g
						float fDirection[3] = { SetRandomEffect(),SetRandomEffect(),SetRandomEffect() };
						SetEffect(g_aPlayer[nIdy].pos + D3DXVECTOR3(sinf(fDirection[0]) * 10, cosf(fDirection[1]) * 10, cosf(fDirection[2]) * 10),
							D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), g_aPlayer[nIdy].col, float(rand() % 80 + 10), 10, EFFECTTYPE_SMOKE);
					}
				}
			}

		}
	}
	if (g_aPlayer[nIdy].nMotionType != g_aPlayer[nIdy].nMotionTypeOld)
	{
		g_aPlayer[nIdy].fRotSpeed = PLAYER_ROT_SPEED_NORMAL;			// �ʏ�̉�]���x������

	}

	// �d��
	g_aPlayer[nIdy].move.y -= GRAVITY_SPEED;
	// �ړ��ʉ��Z
	g_aPlayer[nIdy].pos += g_aPlayer[nIdy].move;
	if (g_bStopStart)
	{
		g_aPlayer[nIdy].pos = g_aPlayer[nIdy].Initpos;
		if (GetGameStartTime())
		{
			DeleteMessageWndow();

			g_bStopStart = false;
			g_aPlayer[nIdy].move.x = 0.0f;
		}
	}

	// �����X�V
	if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] == false && g_aPlayer[nIdy].state != PLAYERSTATE_DEATH)
	{
		g_aPlayer[nIdy].moveR.x += (0 - g_aPlayer[nIdy].move.x)*DOWN_SPEED;
		g_aPlayer[nIdy].moveR.z += (0 - g_aPlayer[nIdy].move.z)*DOWN_SPEED;
		g_aPlayer[nIdy].moveL.x += (0 - g_aPlayer[nIdy].move.x)*DOWN_SPEED;
		g_aPlayer[nIdy].moveL.z += (0 - g_aPlayer[nIdy].move.z)*DOWN_SPEED;
		if (g_aPlayer[nIdy].moveL.z >= MAX_SPEED)
		{
			g_aPlayer[nIdy].moveL.z = MAX_SPEED;
		}
		if (g_aPlayer[nIdy].moveR.z >= MAX_SPEED)
		{
			g_aPlayer[nIdy].moveR.z = MAX_SPEED;
		}
		g_aPlayer[nIdy].move = D3DXVECTOR3((g_aPlayer[nIdy].moveR.x + g_aPlayer[nIdy].moveL.x) / 2.0f,
			(g_aPlayer[nIdy].move.y + g_aPlayer[nIdy].move.y) / 2.0f,
			(g_aPlayer[nIdy].moveR.z + g_aPlayer[nIdy].moveL.z) / 2.0f);

	}

	//--------------------------/ ���f���̓������Ȃ߂炩�ɂ��� /---------------------------------------------------------------------------------------//
	if (g_aPlayer[nIdy].Destrot.y > D3DX_PI)
	{// ������3.14�𒴂���Ȃ�
		g_aPlayer[nIdy].Destrot.y -= D3DX_PI * 2.0f;
	}
	if (g_aPlayer[nIdy].Destrot.y < -D3DX_PI)
	{// ������-3.14�𒴂���Ȃ�
		g_aPlayer[nIdy].Destrot.y += D3DX_PI * 2.0f;
	}
	g_aPlayer[nIdy].Diffrot.y = g_aPlayer[nIdy].Destrot.y - g_aPlayer[nIdy].rot.y;// ���݂̊p�x�ƖړI�̊p�x�̍���
	if (g_aPlayer[nIdy].Diffrot.y > D3DX_PI)
	{// ������3.14�𒴂���Ȃ�
		g_aPlayer[nIdy].Diffrot.y -= D3DX_PI * 2.0f;
	}
	if (g_aPlayer[nIdy].Diffrot.y < -D3DX_PI)
	{// ������-3.14�𒴂���Ȃ�
		g_aPlayer[nIdy].Diffrot.y += D3DX_PI * 2.0f;
	}
	g_aPlayer[nIdy].rot.y += g_aPlayer[nIdy].Diffrot.y * g_aPlayer[nIdy].fRotSpeed;	// �p�x�̉��Z
	if (g_aPlayer[nIdy].rot.y > D3DX_PI)
	{// ���݂̊p�x��3.14�𒴂���Ȃ�
		g_aPlayer[nIdy].rot.y -= D3DX_PI * 2.0f;
	}
	if (g_aPlayer[nIdy].rot.y < -D3DX_PI)
	{// ���݂̊p�x��-3.14�𒴂���Ȃ�
		g_aPlayer[nIdy].rot.y += D3DX_PI * 2.0f;
	}
	//----------------------------------/ �����蔻�� /---------------------------------------------------------------------------------------//
		// ���b�V���t�B�[���h
	if (CollisionMeshField(&g_aPlayer[nIdy].pos, &g_aPlayer[nIdy].posOld, &g_aPlayer[nIdy].move,
		g_aPlayer[nIdy].fRadiusWidth, g_aPlayer[nIdy].fRadiusHeight, &g_aPlayer[nIdy].pMeshFieldLand) == true)
	{
		Shadow *pShadow = GetShadow();
		pShadow[g_aPlayer[nIdy].nIdyShadow].pos.y = g_aPlayer[nIdy].pos.y + 0.1f;
		pShadow[g_aPlayer[nIdy].nIdyShadow].col.a = 1.0f;
		g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP] = false;
		g_aPlayer[nIdy].bJump = false;
		g_aPlayer[nIdy].nAirCounter = 0;// �󒆍U������J�E���^�[

		if (g_aPlayer[nIdy].state != PLAYERSTATE_DEATH)
		{
			if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] == true)
			{
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] = false;
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_LANDING] = true;
			}
		}
		if (!g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP] && g_aPlayer[nIdy].bMotionStateOld[PLAYER_BOOL_JUMP])
		{// ���n����
			for (int nCntEffect = 0; nCntEffect < 2; nCntEffect++)
			{// ���n�G�t�F�N�g
				float nDirection = SetRandomEffect();
				float nDirection2 = SetRandomEffect();
				SetEffect(g_aPlayer[nIdy].pos, D3DXVECTOR3(sinf(nDirection) * float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_SMOKE);
				SetEffect(g_aPlayer[nIdy].pos, D3DXVECTOR3(sinf(nDirection2) *  float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection2) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_SMOKE);
			}
			PlaySound(SOUND_LABEL_LAND);
			g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_LANDING] = true;
		}
	}
	else
	{
	}
	if (g_aPlayer[nIdy].pMeshFieldLand != NULL)
	{// ���ɏ���Ă���
		if ((g_aPlayer[nIdy].move.x > 1.0f || g_aPlayer[nIdy].move.z > 1.0f ||
			g_aPlayer[nIdy].move.x < -1.0f || g_aPlayer[nIdy].move.z < -1.0f))
		{// �����Ă���
			if (!g_aPlayer[nIdy].bSound[SOUND_CHAR])
			{
				if (GetGameState() != GAMESTATE_START)
				{
					PlaySound(SOUND_CHAR);
					g_aPlayer[nIdy].bSound[SOUND_CHAR] = true;
				}
			}
		}
		else
		{
		}
	}
	else
	{
		g_aPlayer[nIdy].bSound[SOUND_CHAR] = false;

	}
	// �v���C���[���m
	PlayerOverLap(g_aPlayer[nIdy].nIdy);
	if (CollisionModelMinMax(&g_aPlayer[nIdy].pos, &g_aPlayer[nIdy].posOld, &g_aPlayer[nIdy].move, g_aPlayer[nIdy].fRadiusWidth, g_aPlayer[nIdy].fRadiusHeight, &g_aPlayer[nIdy].pModel) == 1)
	{// �Ԃ���
	}
	if (g_aPlayer[nIdy].pModel != NULL)
	{
		if (g_aPlayer[nIdy].pModel->nType == MODELTYPE_HAUSE)
		{
			if (GetFade() == FADE_NONE)
			{
				SetFade(MODE_RESULT);
			}
		}
		else
		{
			HitEffect(D3DXVECTOR3(g_aPlayer[nIdy].aModel[2].mtxWorld._41, g_aPlayer[nIdy].aModel[2].mtxWorld._42, g_aPlayer[nIdy].aModel[2].mtxWorld._43));
			PlaySound(SOUND_HIT_HAC);
			g_aPlayer[nIdy].state = PLAYERSTATE_DAMAGE;
			g_aPlayer[nIdy].nCntState = 10;

			// ���������̐�����ї͂���
			g_aPlayer[nIdy].fFlyPower.z = -20;
			// ���������̐�����ї͂���
			g_aPlayer[nIdy].fFlyPower.y = 10;
			g_aPlayer[nIdy].nFlyCounter = 10;
			g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] = true;	// ������ђ��ɐݒ�

			if (g_aPlayer[nIdy].pModel->nType == MODELTYPE_GUARD)
			{
				g_aPlayer[nIdy].state = PLAYERSTATE_DAMAGE;
				g_aPlayer[nIdy].nCntState = 10;

				// ���������̐�����ї͂���
				g_aPlayer[nIdy].fFlyPower.x = -g_aPlayer[nIdy].move.x;
				g_aPlayer[nIdy].fFlyPower.z = -10;
				// ���������̐�����ї͂���
				g_aPlayer[nIdy].fFlyPower.y = 10;
				g_aPlayer[nIdy].nFlyCounter = 10;
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] = true;	// ������ђ��ɐݒ�
			}
		}
		g_aPlayer[nIdy].moveR = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_aPlayer[nIdy].moveL = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	// ���f��
	if (CollisionModelMinMax(&g_aPlayer[nIdy].pos, &g_aPlayer[nIdy].posOld, &g_aPlayer[nIdy].move, g_aPlayer[nIdy].fRadiusWidth, g_aPlayer[nIdy].fRadiusHeight, &g_aPlayer[nIdy].pModel) == 2)
	{// ��ɂ̂�
		Shadow *pShadow = GetShadow();
		pShadow[g_aPlayer[nIdy].nIdyShadow].pos.y = g_aPlayer[nIdy].pos.y + 0.1f;
		pShadow[g_aPlayer[nIdy].nIdyShadow].col.a = 1.0f;
		g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP] = false;
		g_aPlayer[nIdy].bJump = false;
		g_aPlayer[nIdy].nAirCounter = 0;// �󒆍U������J�E���^�[
		if (g_aPlayer[nIdy].state != PLAYERSTATE_DEATH)
		{
			if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] == true)
			{
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] = false;
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_LANDING] = true;
			}
		}
		if (!g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP] && g_aPlayer[nIdy].bMotionStateOld[PLAYER_BOOL_JUMP])
		{// ���n����
			for (int nCntEffect = 0; nCntEffect < 20; nCntEffect++)
			{// ���n�G�t�F�N�g
				SetEffect(g_aPlayer[nIdy].pos + D3DXVECTOR3(sinf(SetRandomEffect()) * 10, 0.0f, cosf(SetRandomEffect()) * 10),
					D3DXVECTOR3(sinf(SetRandomEffect()) * float(rand() % 5), cosf(SetRandomEffect())*0.5f, cosf(SetRandomEffect()) * float(rand() % 5)),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.9f, 0.9f, 1.0f), float(rand() % 30), 0, EFFECTTYPE_WALK);
			}
			PlaySound(SOUND_LABEL_LAND);
		}
		if ((g_aPlayer[nIdy].move.x > g_aPlayer[nIdy].MoveSpeed || g_aPlayer[nIdy].move.z > g_aPlayer[nIdy].MoveSpeed ||
			g_aPlayer[nIdy].move.x < -g_aPlayer[nIdy].MoveSpeed || g_aPlayer[nIdy].move.z < -g_aPlayer[nIdy].MoveSpeed))
		{// �����Ă���@�����@����
		 // �G�Ɠ�������̂̋���
			float fPosX = (g_aPlayer[nIdy].pos.x - 200);
			float fPosZ = (g_aPlayer[nIdy].pos.z - 200);
			float fRadiusA = fPosX*fPosX + fPosZ*fPosZ;
			float fRadiusB = (150 + 150)* (150 + 150);

			if (fRadiusB > fRadiusA&&
				-fRadiusB < fRadiusA)
			{// �΂̏�
				SetEffect(g_aPlayer[nIdy].pos + D3DXVECTOR3(sinf(SetRandomEffect()) * 10, cosf(SetRandomEffect()), cosf(SetRandomEffect()) * 10),
					D3DXVECTOR3(sinf(SetRandomEffect()) * 2 + sinf(g_aPlayer[nIdy].rot.y), cosf(SetRandomEffect()), cosf(SetRandomEffect()) * 2 + cosf(g_aPlayer[nIdy].rot.y)),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.9f, 0.9f, 1.0f), float(rand() % 30), 0, EFFECTTYPE_WALK);
				if (rand() % 5 == 0) {
					SetEffect(g_aPlayer[nIdy].pos + D3DXVECTOR3(sinf(SetRandomEffect()) * 10, cosf(SetRandomEffect()), cosf(SetRandomEffect()) * 10) + D3DXVECTOR3(sinf(g_aPlayer[nIdy].rot.y) * 30, cosf(SetRandomEffect()), cosf(g_aPlayer[nIdy].rot.y) * 30),
						D3DXVECTOR3(sinf(SetRandomEffect()) * 2 + sinf(g_aPlayer[nIdy].rot.y), 0.5f, cosf(SetRandomEffect()) * 2 + cosf(g_aPlayer[nIdy].rot.y)),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.9f, 0.9f, 2.0f), float(rand() % 4) * 10, 0, EFFECTTYPE_CARTOON_TA);
				}
			}
			else
			{// �y
				SetEffect(g_aPlayer[nIdy].pos + D3DXVECTOR3(sinf(SetRandomEffect()) * 10, cosf(SetRandomEffect()), cosf(SetRandomEffect()) * 10),
					D3DXVECTOR3(sinf(SetRandomEffect()) * 2 + sinf(g_aPlayer[nIdy].rot.y), cosf(SetRandomEffect()), cosf(SetRandomEffect()) * 2 + cosf(g_aPlayer[nIdy].rot.y)),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.9f, 0.9f, 1.0f), float(rand() % 30), 0, EFFECTTYPE_DIRTY);
			}

		}
		else
		{
		}
	}

	if (g_aPlayer[nIdy].pos.z >= 800&&g_aPlayer[nIdy].pos.z <= 850)
	{
		PlaySound(SOUND_RESULT_GARASU);
		for (int nCntE = 0; nCntE < 400; nCntE++)
		{// �N���b�V��
			D3DXVECTOR3 rot = D3DXVECTOR3(SetRandomEffect(), SetRandomEffect(), SetRandomEffect());
			D3DXVECTOR3 move = D3DXVECTOR3(sinf(rot.x) * 40, cosf(rot.y) * 40, cosf(rot.z) * 40 + 30);
			SetEffectUP(4, D3DXVECTOR3(0, 550, 800+float(rand()%100)),
				move, rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EFFECTUPTYPE_GUARD, float(rand() % 200 + 50));
		}
	}
	// ���b�V���E�H�[��
	if (CollisionMeshWall(&g_aPlayer[nIdy].pos, &g_aPlayer[nIdy].posOld, &g_aPlayer[nIdy].move, g_aPlayer[nIdy].fRadiusWidth, g_aPlayer[nIdy].fRadiusHeight))
	{

	}


	if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP] != g_aPlayer[nIdy].bMotionStateOld[PLAYER_BOOL_JUMP])
	{// �O��ƍ���̃W�����v���[�V������������璅�n���[�V����
		g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_LANDING] = true;
	}
	//------------------------------------/ ��{���[�V���� /-----------------------------------------------------------------/


	if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY])
	{// �_���[�W(�������)���Ȃ�
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_FLY;
	}
	else if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP])
	{// �v���C���[���W�����v���Ȃ�
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_JUMP;
	}
	else if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_LANDING])
	{// ���n���Ȃ�
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_LANDING;
	}
	else if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_KICK_L])
	{// �L�b�NL
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_KICK_L;
	}
	else if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_KICK_R])
	{// �L�b�NR
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_KICK_R;
	}
	else
	{// �v���C���[�������ĂȂ��Ƃ�
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_NEUTRAL;
	}
	// ���[�V����
	MotionInfoPlayer(nIdy);

	//********************************************************************************************************************************
	// �L�����N�^�[����
	//*************************************************************************************************************************
	switch (g_aPlayer[nIdy].nMotionType)
	{// �v���C���[�̃��[�V������
	case PLAYER_MOTION_TYPE_JUMP:						// �W�����v���[�V������������
		break;
	case PLAYER_MOTION_TYPE_LANDING:					// ���n���[�V������������
		break;
	case PLAYER_MOTION_TYPE_FLY:						// �_���[�W(�������)���[�V������������
		break;
	}
	if (g_aPlayer[nIdy].bMotionStateOld[PLAYER_BOOL_FLY])
	{// �N���オ��I�������
		g_aPlayer[nIdy].nCntState = 60;
		g_aPlayer[nIdy].state = PLAYERSTATE_INVINCIBLE;	// ���G
	}

	//-------------------------------------/ �e �� ���ړ����� /---------------------------------------------------------------------------------------//
	SetPositionShadow(g_aPlayer[nIdy].nIdyShadow, D3DXVECTOR3(g_aPlayer[nIdy].aModel[0].mtxWorld._41, g_aPlayer[nIdy].aModel[0].mtxWorld._42, g_aPlayer[nIdy].aModel[0].mtxWorld._43), g_aPlayer[nIdy].Initpos);
	SetPositionLine(g_aPlayer[nIdy].nIdyLine, g_aPlayer[nIdy].pos, g_aPlayer[nIdy].rot);

	SetEffectSpeed(g_aPlayer[nIdy].move.z, MAX_SPEED);

	for (int nCnt = 0; nCnt < int(g_aPlayer[nIdy].move.z); nCnt++)
	{
		SetEffect(g_aPlayer[nIdy].pos + D3DXVECTOR3(sinf(SetRandomEffect())*(rand() % 10), cosf(SetRandomEffect())*(rand() % 3), cosf(SetRandomEffect())*(rand() % 10)),
			D3DXVECTOR3(sinf(SetRandomEffect())*(rand() % 7), cosf(SetRandomEffect())*(rand() % 1), cosf(SetRandomEffect())*(rand() % 7)),
			D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1.0f, (rand() % 50 / 100.0f), 0.0f, 1.0f), 3, 60, EFFECTTYPE_HIBANA);
	}

}



//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawPlayer(int nIdy, int nCamera)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;				// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxParent;				// �e�̃}�g���b�N�X


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aPlayer[nIdy].mtxWorld);
	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nIdy].rot.y, g_aPlayer[nIdy].rot.x, g_aPlayer[nIdy].rot.z);
	D3DXMatrixMultiply(&g_aPlayer[nIdy].mtxWorld, &g_aPlayer[nIdy].mtxWorld, &mtxRot);
	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nIdy].pos.x, g_aPlayer[nIdy].pos.y, g_aPlayer[nIdy].pos.z);
	D3DXMatrixMultiply(&g_aPlayer[nIdy].mtxWorld, &g_aPlayer[nIdy].mtxWorld, &mtxTrans);
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nIdy].mtxWorld);

	if (g_aPlayer[nIdy].bUse)
	{
		for (int nCntPlayerModel = 0; nCntPlayerModel < g_aPlayerData[nIdy].nNumParts; nCntPlayerModel++)
		{

			// �ߋ�
			g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorldOld = g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld;

			if (g_aPlayerData[nIdy].aModel[nCntPlayerModel].nIdxModelPareant == -1)
			{// �v���C���[
				mtxParent = g_aPlayer[nIdy].mtxWorld;
			}
			else
			{// �e�e
				mtxParent = g_aPlayer[nIdy].aModel[g_aPlayerData[nIdy].aModel[nCntPlayerModel].nIdxModelPareant].mtxWorld;
			}
			// ------------------------/ �e���f���̕`�� /-------------------------------------------//
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld);
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nIdy].aModel[nCntPlayerModel].rot.y, g_aPlayer[nIdy].aModel[nCntPlayerModel].rot.x, g_aPlayer[nIdy].aModel[nCntPlayerModel].rot.z);
			D3DXMatrixMultiply(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &mtxRot);
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nIdy].aModel[nCntPlayerModel].pos.x,
				g_aPlayer[nIdy].aModel[nCntPlayerModel].pos.y,
				g_aPlayer[nIdy].aModel[nCntPlayerModel].pos.z);
			D3DXMatrixMultiply(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &mtxTrans);
			D3DXMatrixMultiply(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &mtxParent);
			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);
			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aPlayerData[nIdy].aModel[nCntPlayerModel].pBuffMat->GetBufferPointer();
			if (g_aPlayer[nIdy].bDraw)
			{
				for (int nCntPlayer2 = 0; nCntPlayer2 < (int)g_aPlayerData[nIdy].aModel[nCntPlayerModel].nNumMat; nCntPlayer2++)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntPlayer2].MatD3D);
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_aPlayerData[nIdy].aModel[nCntPlayerModel].pTextuer[nCntPlayer2]);

					// ���f��(�p�[�c)�̕`��
					g_aPlayerData[nIdy].aModel[nCntPlayerModel].pMesh->DrawSubset(nCntPlayer2);
				}
			}
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);

			// ------------------------/ �q���f���̕`�� /-------------------------------------------//
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld);
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nIdy].aModel[nCntPlayerModel].rot.y, g_aPlayer[nIdy].aModel[nCntPlayerModel].rot.x, g_aPlayer[nIdy].aModel[nCntPlayerModel].rot.z);
			D3DXMatrixMultiply(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &mtxRot);
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nIdy].aModel[nCntPlayerModel].pos.x,
				g_aPlayer[nIdy].aModel[nCntPlayerModel].pos.y,
				g_aPlayer[nIdy].aModel[nCntPlayerModel].pos.z);

			D3DXMatrixMultiply(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &mtxTrans);
			D3DXMatrixMultiply(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &mtxParent);
			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);
			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aPlayerData[nIdy].aModel[nCntPlayerModel].pBuffMat->GetBufferPointer();
			if (g_aPlayer[nIdy].bDraw)
			{
				for (int nCntPlayer2 = 0; nCntPlayer2 < (int)g_aPlayerData[nIdy].aModel[nCntPlayerModel].nNumMat; nCntPlayer2++)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntPlayer2].MatD3D);
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_aPlayerData[nIdy].aModel[nCntPlayerModel].pTextuer[nCntPlayer2]);
					// ���f��(�p�[�c)�̕`��
					g_aPlayerData[nIdy].aModel[nCntPlayerModel].pMesh->DrawSubset(nCntPlayer2);
				}
			}
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
			for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
			{// ����̃}�g���b�N�X
				if (g_aPlayer[nIdy].ColAttack[nCollision].bUse)
				{// �U������
				 // �O��̈ʒu��ɉh
					g_aPlayer[nIdy].ColAttack[nCollision].mtxWorldOld = g_aPlayer[nIdy].ColAttack[nCollision].mtxWorld;
					// �ʒu�𔽉f
					D3DXMatrixTranslation(&g_aPlayer[nIdy].ColAttack[nCollision].mtxWorld, g_aPlayerData[nIdy].ColAttack[nCollision].ofset.x, g_aPlayerData[nIdy].ColAttack[nCollision].ofset.y, g_aPlayerData[nIdy].ColAttack[nCollision].ofset.z);
					D3DXMatrixMultiply(&g_aPlayer[nIdy].ColAttack[nCollision].mtxWorld, &g_aPlayer[nIdy].ColAttack[nCollision].mtxWorld, &g_aPlayer[nIdy].aModel[g_aPlayer[nIdy].ColAttack[nCollision].nIdxParent].mtxWorld);
				}
				if (g_aPlayer[nIdy].ColBody[nCollision].bUse)
				{// �_���[�W����
				 // �O��̈ʒu��ɉh
					g_aPlayer[nIdy].ColBody[nCollision].mtxWorldOld = g_aPlayer[nIdy].ColBody[nCollision].mtxWorld;
					// �ʒu�𔽉f
					D3DXMatrixTranslation(&g_aPlayer[nIdy].ColBody[nCollision].mtxWorld, g_aPlayer[nIdy].ColBody[nCollision].ofset.x, g_aPlayer[nIdy].ColBody[nCollision].ofset.y, g_aPlayer[nIdy].ColBody[nCollision].ofset.z);
					D3DXMatrixMultiply(&g_aPlayer[nIdy].ColBody[nCollision].mtxWorld, &g_aPlayer[nIdy].ColBody[nCollision].mtxWorld, &g_aPlayer[nIdy].aModel[g_aPlayer[nIdy].ColBody[nCollision].nIdxParent].mtxWorld);
				}
			}

		}
		PlayerMotionType type = g_aPlayer[nIdy].nMotionType;
		for (int nCntOrbit = 0; nCntOrbit < MAX_PLAYER_ORBIT; nCntOrbit++)
		{
			if (g_aPlayer[nIdy].orbit[type][nCntOrbit].nOrbitLength <= 0)
			{
				break;
			}
			// �O�Րݒ�
			SetOrbit(g_aPlayer[nIdy].aModel[g_aPlayer[nIdy].orbit[type][nCntOrbit].nIdyOrbit].mtxWorld,
				g_aPlayer[nIdy].aModel[g_aPlayer[nIdy].orbit[type][nCntOrbit].nIdyOrbit].mtxWorldOld,
				g_aPlayer[nIdy].orbit[type][nCntOrbit].nOrbitLength,
				g_aPlayer[nIdy].orbit[type][nCntOrbit].OrbitStart, g_aPlayer[nIdy].orbit[type][nCntOrbit].OrbitEnd,
				ORBITTYPE(g_aPlayer[nIdy].orbit[type][nCntOrbit].nType), g_aPlayer[nIdy].orbit[type][nCntOrbit].motion);
		}
		PlayerColInfo(nIdy);
	}
}
//==============================================================================================================================
// ���f���̎擾
//==============================================================================================================================
Player *GetPlayer(void)
{
	return &g_aPlayer[0];
}
PlayerData *GetPlayerData(void)
{
	return &g_aPlayerData[0];
}

//==============================================================================================================================
// ���f���̃��[�V����
//==============================================================================================================================
void MotionInfoPlayer(int nIdy)
{
	for (int nCnt = 0 ; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_aPlayer[nIdy].nFrameAllCounter == 1)
		{// ���̃^�C�v�ƑO�̃^�C�v���������
			g_aPlayer[nCnt].nMotionGetCol = PLAYER_MOTION_TYPE_MAX;
		}
	}
	if (g_aPlayer[nIdy].nMotionType != g_aPlayer[nIdy].nMotionTypeOld)
	{// ���̃^�C�v�ƑO�̃^�C�v���������
		g_aPlayer[nIdy].nKeyCounter = 0;						// �L�[�����Z�b�g
		g_aPlayer[nIdy].nFrameCounter = 0;					// �t���[�������Z�b�g
		g_aPlayer[nIdy].bBlend = true;	// �u�����h�ɂ���
		g_aPlayer[nIdy].nFrame = BLEND_FRAME;					// �u�����h�̃t���[��
		g_aPlayer[nIdy].nFrameAllCounter = 0;					// �S�Ẵt���[��
	}
	else if (g_aPlayer[nIdy].bBlend == false )
	{// �ʏ펞�̃t���[��
		g_aPlayer[nIdy].nFrame = g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].aKey[g_aPlayer[nIdy].nKeyCounter].nFrame;
	}
	if (g_aPlayer[nIdy].nFrameCounter == 0)
	{// ���̃L�[�t���[���̃��[�V������ǂݍ���
		for (int nCntParts = 0; nCntParts < g_aPlayerData[nIdy].nNumParts; nCntParts++)
		{// Key - ���� �Ԃ̍����v�Z
			g_aPlayer[nIdy].aModel[nCntParts].posAddMotion =
				(g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].aKey[g_aPlayer[nIdy].nKeyCounter].aPartsKey[nCntParts].pos
					- g_aPlayer[nIdy].aModel[nCntParts].pos + g_aPlayer[nIdy].aModel[nCntParts].Initpos) / float(g_aPlayer[nIdy].nFrame);

			g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion = (g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].aKey[g_aPlayer[nIdy].nKeyCounter].aPartsKey[nCntParts].rot - g_aPlayer[nIdy].aModel[nCntParts].rot);

			if (g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.x > D3DX_PI)
			{
				g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.x -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.x < -D3DX_PI)
			{
				g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.x += D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.y > D3DX_PI)
			{
				g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.y -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.y < -D3DX_PI)
			{
				g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.y += D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.z > D3DX_PI)
			{
				g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.z -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.z < -D3DX_PI)
			{
				g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.z += D3DX_PI * 2.0f;
			}
			g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion = g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion / float(g_aPlayer[nIdy].nFrame);
		}
	}
	for (int nCntParts = 0; nCntParts < g_aPlayerData[nIdy].nNumParts; nCntParts++)
	{// ���[�V�����������Z

		if (g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.x > D3DX_PI)
		{// ������3.14�𒴂���Ȃ�
			g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.x -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.x < -D3DX_PI)
		{// ������-3.14�𒴂���Ȃ�
			g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.x += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.y > D3DX_PI)
		{// ������3.14�𒴂���Ȃ�
			g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.y -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.y < -D3DX_PI)
		{// ������-3.14�𒴂���Ȃ�
			g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.y += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.z > D3DX_PI)
		{// ������3.14�𒴂���Ȃ�
			g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.z -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.z < -D3DX_PI)
		{// ������-3.14�𒴂���Ȃ�
			g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.z += D3DX_PI * 2.0f;
		}
		g_aPlayer[nIdy].aModel[nCntParts].pos += g_aPlayer[nIdy].aModel[nCntParts].posAddMotion;
		g_aPlayer[nIdy].aModel[nCntParts].rot += g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion;

		if (g_aPlayer[nIdy].aModel[nCntParts].rot.x > D3DX_PI)
		{// ���݂̊p�x��3.14�𒴂���Ȃ�
			g_aPlayer[nIdy].aModel[nCntParts].rot.x -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rot.x < -D3DX_PI)
		{// ���݂̊p�x��-3.14�𒴂���Ȃ�
			g_aPlayer[nIdy].aModel[nCntParts].rot.x += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rot.y > D3DX_PI)
		{// ���݂̊p�x��3.14�𒴂���Ȃ�
			g_aPlayer[nIdy].aModel[nCntParts].rot.y -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rot.y < -D3DX_PI)
		{// ���݂̊p�x��-3.14�𒴂���Ȃ�
			g_aPlayer[nIdy].aModel[nCntParts].rot.y += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rot.z > D3DX_PI)
		{// ���݂̊p�x��3.14�𒴂���Ȃ�
			g_aPlayer[nIdy].aModel[nCntParts].rot.z -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rot.z < -D3DX_PI)
		{// ���݂̊p�x��-3.14�𒴂���Ȃ�
			g_aPlayer[nIdy].aModel[nCntParts].rot.z += D3DX_PI * 2.0f;
		}
	}
	g_aPlayer[nIdy].nFrameCounter++;
	g_aPlayer[nIdy].nFrameAllCounter++;
	if (g_aPlayer[nIdy].bBlend  && g_aPlayer[nIdy].nFrameCounter == BLEND_FRAME)
	{// �u�����h�@�u�����h�̃t���[�����ɒB����
		g_aPlayer[nIdy].bBlend = false;
		g_aPlayer[nIdy].nFrameCounter = 0;
		g_aPlayer[nIdy].nKeyCounter = 0;// �u�����h�Ŏ��̃��[�V������0�L�[�ڂ͍s���Ă�
		if (g_aPlayer[nIdy].nMotionType == PLAYER_MOTION_TYPE_NEUTRAL)
		{
			g_aPlayer[nIdy].nKeyCounter = 1;
		}
	}
	else if (g_aPlayer[nIdy].bBlend == false&& g_aPlayer[nIdy].nFrameCounter == g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].aKey[g_aPlayer[nIdy].nKeyCounter].nFrame)
	{// �t���[���J�E���g������
		g_aPlayer[nIdy].nFrameCounter = 0;
		if (g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].bLoop == false &&
			g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].nNumKey - 1 == g_aPlayer[nIdy].nKeyCounter)
		{// ���[�v���Ȃ����L�[����������
			g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_LANDING] = false;	// ���n�L�����Z��

			switch (g_aPlayer[nIdy].nMotionType)
			{// �v���C���[�̃��[�V������
			case PLAYER_MOTION_TYPE_JUMP:																// �W�����v���[�V������������
				break;
			case PLAYER_MOTION_TYPE_LANDING:															// ���n���[�V������������
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_LANDING] = false;										// ���n��Ԃ���Ȃ���Ԃɂ���
				break;
			case PLAYER_MOTION_TYPE_FLY:																// �_���[�W(�������)���[�V������������
				break;
			default:
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_KICK_R] = false;										// ���n��Ԃ���Ȃ���Ԃɂ���
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_KICK_L] = false;										// ���n��Ԃ���Ȃ���Ԃɂ���

				break;
			}
			g_aPlayer[nIdy].nMotionGetCol = PLAYER_MOTION_TYPE_MAX;
		}
		else
		{// ���̃L�[�����l��
			g_aPlayer[nIdy].nKeyCounter = (g_aPlayer[nIdy].nKeyCounter + 1) % g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].nNumKey;
		}
	}

}
//==============================================================================================================================
// �v���C���[�̑̂̓����蔻��
//==============================================================================================================================
bool CollisionPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 mtxWorld, float fRadius, float nPower, float fDamageSide, float fDamageV,int nIdy)
{
	bool bHit = false;
	if (GetCamera()->nState == CAMERASTATE_NONE )
	{
		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			if (g_aPlayer[nCnt].bUse)
			{
				if (nCnt != nIdy)
				{
					if (g_aPlayer[nCnt].state != PLAYERSTATE_DEATH&&
						g_aPlayer[nCnt].state != PLAYERSTATE_INVINCIBLE&&
						g_aPlayer[nCnt].state != PLAYERSTATE_DAMAGE &&
						!g_aPlayer[nCnt].bMotionState[PLAYER_BOOL_FLY])
					{// ����łȂ�
						{// �_���[�W������Ȃ����
							for (int nCntCollision = 0; nCntCollision < MAX_COLLISION; nCntCollision++)
							{// �̂̓����蔻�蕪���[�v
								if (g_aPlayer[nCnt].ColBody[nCntCollision].bUse)
								{
									// ��̓����蔻��̍��W�̋������v�Z
									float PosX = (g_aPlayer[nCnt].ColBody[nCntCollision].mtxWorld._41 - mtxWorld.x);		// (�v���C���[�̑̂̓����蔻��̍��W.x - �G�̍U���̓����蔻��̍��W.x)
									float PosY = (g_aPlayer[nCnt].ColBody[nCntCollision].mtxWorld._42 - mtxWorld.y);		// (�v���C���[�̑̂̓����蔻��̍��W.y - �G�̍U���̓����蔻��̍��W.y)
									float PosZ = (g_aPlayer[nCnt].ColBody[nCntCollision].mtxWorld._43 - mtxWorld.z);		// (�v���C���[�̑̂̓����蔻��̍��W.z - �G�̍U���̓����蔻��̍��W.z)

									float fPosLenght = (PosX * PosX) + (PosY * PosY) + (PosZ * PosZ);				// ���ꂼ���2�悵�Ă��ׂđ����ċ������o��
									// ��̓����蔻��̔��a�̋������v�Z
									float fCollisionRadius = (g_aPlayer[nCnt].ColBody[nCntCollision].fRadius + fRadius);	// �v���C���[�̑̂̓����蔻��̔��a+�G�̍U���̓����蔻��̔��a�𑫂�
									float fRadiusLenght = fCollisionRadius * fCollisionRadius;	// ��̔��a�𑫂����l��2�悵�āA���a�̋������v�Z���v�Z

									if (fRadiusLenght > fPosLenght &&
										-fRadiusLenght < fPosLenght)
									{// ���W�̋��������a�̋�������������������
										float fAttackAngle = atan2f(pos.x - g_aPlayer[nCnt].pos.x, pos.z - g_aPlayer[nCnt].pos.z);	// ���������p�x

										if (g_aPlayer[nCnt].nMotionGetCol != g_aPlayer[nIdy].nMotionType)
										{
											g_aPlayer[nCnt].fLife -= nPower;

											if (g_aPlayer[nCnt].fLife <= 0)
											{// ���C�t��0�ȉ��ɂȂ�����
												PlayerLifeZero(nCnt, fAttackAngle, fDamageSide, fDamageV);
											}
											else
											{// �q�b�g
												if (fDamageSide > 1.0f && fDamageV > 5.0f)
												{// ������ї͂�����Ƃ�
													PlaySound(SOUND_LABEL_HIT001);
													SetPlayerVoice(PLAYER_MOTION_TYPE_FLY);
													g_aPlayer[nCnt].state = PLAYERSTATE_DAMAGE;
													g_aPlayer[nCnt].nCntState = 1;
													// ���������̐�����ї͂���
													g_aPlayer[nCnt].fFlyPower.x = sinf(fAttackAngle - D3DX_PI) * fDamageSide;
													g_aPlayer[nCnt].fFlyPower.z = cosf(fAttackAngle - D3DX_PI) * fDamageSide;
													// ���������̐�����ї͂���
													g_aPlayer[nCnt].fFlyPower.y = fDamageV;
													g_aPlayer[nCnt].nFlyCounter = 10;
													g_aPlayer[nCnt].bMotionState[PLAYER_BOOL_FLY] = true;	// ������ђ��ɐݒ�

													g_aPlayer[nCnt].Destrot.y = fAttackAngle - D3DX_PI;		// �L�����N�^�[�̌�����ύX
													g_aPlayer[nCnt].rot.y = fAttackAngle - D3DX_PI;			// �L�����N�^�[�̌�����ύX
												}
												else
												{// �����̂�


													PlaySound(SOUND_LABEL_HIT000);

													// ������ї͂���
													g_aPlayer[nCnt].state = PLAYERSTATE_DAMAGE;
													g_aPlayer[nCnt].nCntState = 3;
													g_aPlayer[nCnt].fFlyPower.x = sinf(fAttackAngle - D3DX_PI) * fDamageSide;
													g_aPlayer[nCnt].fFlyPower.z = cosf(fAttackAngle - D3DX_PI) * fDamageSide;
													g_aPlayer[nCnt].fFlyPower.y = fDamageV;
													g_aPlayer[nCnt].nFlyCounter = 10;
													g_aPlayer[nCnt].Destrot.y = fAttackAngle - D3DX_PI;		// �L�����N�^�[�̌�����ύX
													g_aPlayer[nCnt].rot.y = fAttackAngle - D3DX_PI;			// �L�����N�^�[�̌�����ύX

												}

											}
											// �q�b�g�G�t�F�N�g
											HitEffect(D3DXVECTOR3(g_aPlayer[nCnt].ColBody[nCntCollision].mtxWorld._41, g_aPlayer[nCnt].ColBody[nCntCollision].mtxWorld._42, g_aPlayer[nCnt].ColBody[nCntCollision].mtxWorld._43));

											bHit = true;	// ��������									// ���[�V�������擾
										}
										g_aPlayer[nCnt].nMotionGetCol = g_aPlayer[nIdy].nMotionType;
										break;
									}
								}
							}
						}
					}
				}

			}
		}
	}
	return bHit;
}
//==============================================================================================================================
// ����ݒ�(�U������j
//==============================================================================================================================
void SetAttackCollisionPlayer(int nIdy,int nIdxParent, D3DXVECTOR3 offset, float fRadius, int nCollisionStart, int nCollisionEnd,int nStopTime, PlayerMotionType nMotionType)
{
	for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
	{
		if (g_aPlayerData[nIdy].ColAttack[nCollision].bUse == false)
		{
			g_aPlayerData[nIdy].ColAttack[nCollision].nIdxParent = nIdxParent;
			g_aPlayerData[nIdy].ColAttack[nCollision].ofset = offset;
			g_aPlayerData[nIdy].ColAttack[nCollision].fRadius = fRadius;
			g_aPlayerData[nIdy].ColAttack[nCollision].nCollisionStart = nCollisionStart;
			g_aPlayerData[nIdy].ColAttack[nCollision].nCollisionEnd = nCollisionEnd;
			g_aPlayerData[nIdy].ColAttack[nCollision].nStopTime = nStopTime;
			g_aPlayerData[nIdy].ColAttack[nCollision].nMotionType = nMotionType;
			g_aPlayerData[nIdy].ColAttack[nCollision].bUse = true;
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&g_aPlayerData[nIdy].ColAttack[nCollision].mtxWorld, g_aPlayerData[nIdy].ColAttack[nCollision].ofset.x, g_aPlayerData[nIdy].ColAttack[nCollision].ofset.y, g_aPlayerData[nIdy].ColAttack[nCollision].ofset.z);
			D3DXMatrixMultiply(&g_aPlayerData[nIdy].ColAttack[nCollision].mtxWorld, &g_aPlayerData[nIdy].ColAttack[nCollision].mtxWorld, &g_aPlayerData[nIdy].aModel[g_aPlayerData[nIdy].ColAttack[nCollision].nIdxParent].mtxWorld);
			break;
		}
	}

}
//==============================================================================================================================
// ����ݒ�i�_���[�W����j
//==============================================================================================================================
void SetBodyCollisionPlayer(int nIdy,int nIdxParent, D3DXVECTOR3 offset, float fRadius)
{
	for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
	{
		if (g_aPlayerData[nIdy].ColBody[nCollision].bUse == false)
		{
			g_aPlayerData[nIdy].ColBody[nCollision].nIdxParent = nIdxParent;
			g_aPlayerData[nIdy].ColBody[nCollision].ofset = offset;
			g_aPlayerData[nIdy].ColBody[nCollision].fRadius = fRadius;
			g_aPlayerData[nIdy].ColBody[nCollision].bUse = true;
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&g_aPlayerData[nIdy].ColBody[nCollision].mtxWorld, g_aPlayerData[nIdy].ColBody[nCollision].ofset.x, g_aPlayerData[nIdy].ColBody[nCollision].ofset.y, g_aPlayerData[nIdy].ColAttack[nCollision].ofset.z);
			D3DXMatrixMultiply(&g_aPlayerData[nIdy].ColBody[nCollision].mtxWorld, &g_aPlayerData[nIdy].ColBody[nCollision].mtxWorld, &g_aPlayerData[nIdy].aModel[g_aPlayerData[nIdy].ColBody[nCollision].nIdxParent].mtxWorld);
			break;
		}
	}

}
//==============================================================================================================================
// �v���C���[�̑���
//==============================================================================================================================
void PlayerController(int nIdy)
{
	GamePad *pGamePad = GetgamePadStick();
	Camera *pCamera = GetCamera();
#if 1
	static int nCon = 0;

	if (nCon == nIdy)
	{
		if (GetKeyboardTrigger(DIK_J)||GetGamePadTrigger(0,BUTTON_R1))
		{
			PlaySound(SOUND_SELECTMOVE);
			g_aPlayer[nIdy].moveL.x += sinf(-D3DX_PI / 4 + pCamera->rot.y)*g_aPlayer[nIdy].MoveSpeed;
			g_aPlayer[nIdy].moveL.z += cosf(-D3DX_PI / 4 + pCamera->rot.y)*g_aPlayer[nIdy].MoveSpeed;
			g_aPlayer[nIdy].Destrot.y = D3DX_PI / 4 * 3 + pCamera->rot.y;	// �ړI�̊p�x
			g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_KICK_L] = true;
		}
		if (GetKeyboardTrigger(DIK_F)|| GetGamePadTrigger(0, BUTTON_L1))
		{
			PlaySound(SOUND_SELECTMOVE);
			g_aPlayer[nIdy].moveR.x += sinf(D3DX_PI / 4 + pCamera->rot.y)*g_aPlayer[nIdy].MoveSpeed;
			g_aPlayer[nIdy].moveR.z += cosf(D3DX_PI / 4  + pCamera->rot.y)*g_aPlayer[nIdy].MoveSpeed;
			g_aPlayer[nIdy].Destrot.y = -D3DX_PI / 4 * 3 + pCamera->rot.y;	// �ړI�̊p�x
			g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_KICK_R] = true;
		}
		SetSpGageDestSkillPoint(int(g_aPlayer[nIdy].moveL.z / MAX_SPEED * 100), LEG_TYPE_RIGHT );
		SetSpGageDestSkillPoint(int(g_aPlayer[nIdy].moveR.z / MAX_SPEED * 100), LEG_TYPE_LEFT);

		if (!g_aPlayer[nIdy].bJump &&
			!g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP] &&
			!g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] )
		{// �W�����v&������т��Ă��Ȃ����
			if (GetKeyboardTrigger(DIK_SPACE)||GetGamePadTrigger(0,BUTTON_A))
			{// �W�����v
				PlaySound(SOUND_LABEL_JUMP);
				g_aPlayer[nIdy].move.y = g_aPlayer[nIdy].JumpSpeed;	// �ړ��ʉ��Z
				g_aPlayer[nIdy].bJump = true;
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP] = true;
			}

		}
	}
#endif
}
//==============================================================================================================================
// �v���C���[���m�̏Փ˂̔���
//==============================================================================================================================
void PlayerOverLap(int nIdy)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_aPlayer[nCnt].bUse)
		{
			if (g_aPlayer[nIdy].state != PLAYERSTATE_DEATH || g_aPlayer[nCnt].state != PLAYERSTATE_DAMAGE)
			{
				if (nIdy != g_aPlayer[nCnt].nIdy)
				{
					// �G�Ɠ�������̂̋���
					float fPosX = (g_aPlayer[nIdy].pos.x - g_aPlayer[nCnt].pos.x);
					float fPosY = (g_aPlayer[nIdy].pos.y - g_aPlayer[nCnt].pos.y);
					float fPosZ = (g_aPlayer[nIdy].pos.z - g_aPlayer[nCnt].pos.z);
					float fRadiusA = fPosX*fPosX + fPosY*fPosY + fPosZ*fPosZ;
					float fRadiusB = 0;

					fRadiusB = (g_aPlayer[nIdy].fRadiusWidth + g_aPlayer[nIdy].fRadiusWidth)* (g_aPlayer[nCnt].fRadiusWidth + g_aPlayer[nCnt].fRadiusWidth);

					if (fRadiusB > fRadiusA&&
						-fRadiusB < fRadiusA)
					{// �l�Ɛl�̂����蔻��
						float fAngle2 = atan2f(g_aPlayer[nIdy].mtxWorld._41 - g_aPlayer[nCnt].pos.x, g_aPlayer[nIdy].mtxWorld._43 - g_aPlayer[nCnt].pos.z);
						g_aPlayer[nIdy].pos.x = g_aPlayer[nCnt].pos.x + sinf(fAngle2) * (g_aPlayer[nIdy].fRadiusWidth + g_aPlayer[nCnt].fRadiusWidth);
						g_aPlayer[nIdy].pos.z = g_aPlayer[nCnt].pos.z + cosf(fAngle2) * (g_aPlayer[nIdy].fRadiusWidth + g_aPlayer[nCnt].fRadiusWidth);
					}
				}

			}
		}
	}

}
//==============================================================================================================================
// �v���C���[�̃^�C�v�ݒ�
//==============================================================================================================================
void SetPlayer(int nIdy, D3DXVECTOR3 pos)
{
	if (!g_aPlayer[nIdy].bUse)
	{
		g_nNumAll++;
		g_aPlayer[nIdy].pos = pos;
		g_aPlayer[nIdy].MoveSpeed = g_aPlayerData[nIdy].MoveSpeed;
		g_aPlayer[nIdy].JumpSpeed = g_aPlayerData[nIdy].JumpSpeed;
		g_aPlayer[nIdy].fRadiusWidth = g_aPlayerData[nIdy].fRadiusWidth;
		g_aPlayer[nIdy].fRadiusHeight = g_aPlayerData[nIdy].fRadiusHeight;
		g_aPlayer[nIdy].fLife = float(g_aPlayerData[nIdy].nInitLife);
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_NEUTRAL;	// ���[�V�����^�C�v
		g_aPlayer[nIdy].nMotionTypeOld = PLAYER_MOTION_TYPE_MAX;	// �O��̃��[�V�����^�C�v
		g_aPlayer[nIdy].nMotionGetCol = PLAYER_MOTION_TYPE_NEUTRAL;	// ���[�V�����^�C�v
		g_aPlayer[nIdy].nMotionGetColOld = PLAYER_MOTION_TYPE_MAX;	// �O��̃��[�V�����^�C�v
		g_aPlayer[nIdy].nIdy = nIdy;
		for (int nbMotion = 0; nbMotion < PLAYER_BOOL_MAX; nbMotion++)
		{
			g_aPlayer[nIdy].bMotionState[nbMotion] = false;
			g_aPlayer[nIdy].bMotionStateOld[nbMotion] = false;
		}
		for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
		{// �_���[�W�ݒ�
			g_aPlayer[nIdy].ColBody[nCollision].nIdxParent = g_aPlayerData[nIdy].ColBody[nCollision].nIdxParent;
			g_aPlayer[nIdy].ColBody[nCollision].ofset = g_aPlayerData[nIdy].ColBody[nCollision].ofset;
			g_aPlayer[nIdy].ColBody[nCollision].fRadius = g_aPlayerData[nIdy].ColBody[nCollision].fRadius;
			g_aPlayer[nIdy].ColBody[nCollision].bUse = g_aPlayerData[nIdy].ColBody[nCollision].bUse;
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&g_aPlayer[nIdy].ColBody[nCollision].mtxWorld, g_aPlayer[nIdy].ColBody[nCollision].ofset.x, g_aPlayer[nIdy].ColBody[nCollision].ofset.y, g_aPlayer[nIdy].ColAttack[nCollision].ofset.z);
			D3DXMatrixMultiply(&g_aPlayer[nIdy].ColBody[nCollision].mtxWorld, &g_aPlayer[nIdy].ColBody[nCollision].mtxWorld, &g_aPlayer[nIdy].aModel[g_aPlayer[nIdy].ColBody[nCollision].nIdxParent].mtxWorld);
		}
		for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
		{// �U���ݒ�
			g_aPlayer[nIdy].ColAttack[nCollision].nIdxParent = g_aPlayerData[nIdy].ColAttack[nCollision].nIdxParent;
			g_aPlayer[nIdy].ColAttack[nCollision].ofset = g_aPlayerData[nIdy].ColAttack[nCollision].ofset;
			g_aPlayer[nIdy].ColAttack[nCollision].fRadius = g_aPlayerData[nIdy].ColAttack[nCollision].fRadius;
			g_aPlayer[nIdy].ColAttack[nCollision].nCollisionStart = g_aPlayerData[nIdy].ColAttack[nCollision].nCollisionStart;
			g_aPlayer[nIdy].ColAttack[nCollision].nCollisionEnd = g_aPlayerData[nIdy].ColAttack[nCollision].nCollisionEnd;
			g_aPlayer[nIdy].ColAttack[nCollision].nStopTime = g_aPlayerData[nIdy].ColAttack[nCollision].nStopTime;
			g_aPlayer[nIdy].ColAttack[nCollision].nMotionType = g_aPlayerData[nIdy].ColAttack[nCollision].nMotionType;
			g_aPlayer[nIdy].ColAttack[nCollision].bUse = g_aPlayerData[nIdy].ColAttack[nCollision].bUse;
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&g_aPlayer[nIdy].ColAttack[nCollision].mtxWorld, g_aPlayer[nIdy].ColAttack[nCollision].ofset.x, g_aPlayer[nIdy].ColAttack[nCollision].ofset.y, g_aPlayer[nIdy].ColAttack[nCollision].ofset.z);
			D3DXMatrixMultiply(&g_aPlayer[nIdy].ColAttack[nCollision].mtxWorld, &g_aPlayer[nIdy].ColAttack[nCollision].mtxWorld, &g_aPlayer[nIdy].aModel[g_aPlayer[nIdy].ColAttack[nCollision].nIdxParent].mtxWorld);

		}
		for (int nIdyModel = 0; nIdyModel < g_aPlayerData[nIdy].nNumParts; nIdyModel++)
		{// ���f�������ʒu�ݒ�
			g_aPlayer[nIdy].aModel[nIdyModel].pos = g_aPlayerData[nIdy].aModel[nIdyModel].pos;
			g_aPlayer[nIdy].aModel[nIdyModel].Initpos = g_aPlayerData[nIdy].aModel[nIdyModel].pos;
			g_aPlayer[nIdy].aModel[nIdyModel].rot = g_aPlayerData[nIdy].aModel[nIdyModel].rot;
			g_aPlayer[nIdy].aModel[nIdyModel].rot = g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].aKey[0].aPartsKey[nIdyModel].rot;
		}
		for (int nIdyMotion = 0; nIdyMotion < PLAYER_MOTION_TYPE_MAX; nIdyMotion++)
		{
			for (int nIdyOrbit = 0; nIdyOrbit < MAX_PLAYER_ORBIT; nIdyOrbit++)
			{
				g_aPlayer[nIdy].orbit[nIdyMotion][nIdyOrbit].nIdyOrbit = g_aPlayerData[nIdy].orbit[nIdyMotion][nIdyOrbit].nIdyOrbit;				// �O�Ղ̐�
				g_aPlayer[nIdy].orbit[nIdyMotion][nIdyOrbit].nOrbitLength = g_aPlayerData[nIdy].orbit[nIdyMotion][nIdyOrbit].nOrbitLength;		// �O�Ղ̒���
				g_aPlayer[nIdy].orbit[nIdyMotion][nIdyOrbit].OrbitStart = g_aPlayerData[nIdy].orbit[nIdyMotion][nIdyOrbit].OrbitStart;			// ���_
				g_aPlayer[nIdy].orbit[nIdyMotion][nIdyOrbit].OrbitEnd = g_aPlayerData[nIdy].orbit[nIdyMotion][nIdyOrbit].OrbitEnd;				// �I�_
				g_aPlayer[nIdy].orbit[nIdyMotion][nIdyOrbit].nType = g_aPlayerData[nIdy].orbit[nIdyMotion][nIdyOrbit].nType;					// �O�Ղ̃^�C�v
				g_aPlayer[nIdy].orbit[nIdyMotion][nIdyOrbit].motion = g_aPlayerData[nIdy].orbit[nIdyMotion][nIdyOrbit].motion;	// ���[�V����
			}
		}
		if (GetMode() == MODE_GAME)
		{
			// �e�ݒ�
			g_aPlayer[nIdy].nIdyShadow = SetShadow(g_aPlayer[nIdy].pos, g_aPlayer[nIdy].rot, g_aPlayer[nIdy].fRadiusWidth * 2.5f, g_aPlayer[nIdy].fRadiusWidth);
			// ���C������ݒ�
			g_aPlayer[nIdy].nIdyLine = CollisionLine(g_aPlayer[nIdy].pos, D3DXVECTOR3(-g_aPlayer[nIdy].fRadiusWidth, 0.0f, -g_aPlayer[nIdy].fRadiusWidth), D3DXVECTOR3(g_aPlayer[nIdy].fRadiusWidth, g_aPlayer[nIdy].fRadiusHeight, g_aPlayer[nIdy].fRadiusWidth), LINETYPE_PLAYER);
		}
		g_aPlayer[nIdy].bUse = true;
	}
}
void ReleasePlayer(int nIdy)
{
	if (g_aPlayer[nIdy].bUse)
	{
		g_nNumAll--;
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_NEUTRAL;	// ���[�V�����^�C�v
		g_aPlayer[nIdy].nMotionTypeOld = PLAYER_MOTION_TYPE_MAX;	// �O��̃��[�V�����^�C�v
		g_aPlayer[nIdy].nMotionGetCol = PLAYER_MOTION_TYPE_NEUTRAL;	// ���[�V�����^�C�v
		g_aPlayer[nIdy].nMotionGetColOld = PLAYER_MOTION_TYPE_MAX;	// �O��̃��[�V�����^�C�v
		g_aPlayer[nIdy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_aPlayer[nIdy].Diffrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]�̍���
		g_aPlayer[nIdy].Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړI�̉�]
		g_aPlayer[nIdy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]

		for (int nIdySound = 0; nIdySound < SOUND_LABEL_MAX; nIdySound++)
		{// ��
			g_aPlayer[nIdy].bSound[nIdySound] = false;
		}
		g_aPlayer[nIdy].state = PLAYERSTATE_NONE;
		g_aPlayer[nIdy].nCntState = 0;
		g_aPlayer[nIdy].pMeshFieldLand = NULL;
		g_aPlayer[nIdy].pModel = NULL;
		g_aPlayer[nIdy].pModelAtk = NULL;
		for (int nbMotion = 0; nbMotion < PLAYER_BOOL_MAX; nbMotion++)
		{
			g_aPlayer[nIdy].bMotionState[nbMotion] = false;
			g_aPlayer[nIdy].bMotionStateOld[nbMotion] = false;
		}
		g_aPlayer[nIdy].bBlend = false;
		g_aPlayer[nIdy].nDamage = 0;
		g_aPlayer[nIdy].bJump = false;							// �W�����v
		g_aPlayer[nIdy].nFlyCounter = 0;						// ���ł��鎞��
		g_aPlayer[nIdy].fRotSpeed = 0.0f;						// ��]�̌����X�s�[�h
		g_aPlayer[nIdy].nAirCounter = 0;						// ���V���̍U������J�E���^�[
		g_aPlayer[nIdy].nLoopCounter = 0;						// ���[�v�J�E���^�[
		g_aPlayer[nIdy].nIdy = nIdy;							// �ԍ�
		g_aPlayer[nIdy].nKeyCounter = 0;		// �L�[��
		g_aPlayer[nIdy].nFrameCounter = 0;		// �t���[����
		g_aPlayer[nIdy].nFrameAllCounter = 0;	// �ő�t���[��
		g_aPlayer[nIdy].nFrame = 0;				// �t���[��
		g_aPlayer[nIdy].bUse = false;
		g_aPlayer[nIdy].bDraw = true;			// �f���Ă邩�ǂ���
	}
}
//==============================================================================================================================
// �v���C���[�̃G�t�F�N�g
//==============================================================================================================================
void PlayerMotionEffect(int nCntPlayer, int nCollision)
{
//	SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].ColAttack[nCollision].mtxWorld._41, g_aPlayer[nCntPlayer].ColAttack[nCollision].mtxWorld._42, g_aPlayer[nCntPlayer].ColAttack[nCollision].mtxWorld._43), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aPlayer[nCntPlayer].ColAttack[nCollision].fRadius, 10,EFFECTTYPE_SMOKE);
}
//==============================================================================================================================
// �v���C���[�̔���
//==============================================================================================================================
void PlayerColInfo(int nIdy)
{
	// �U�����菈��
	for (int nCnt = 0; nCnt < MAX_COLLISION; nCnt++)
	{
		if (g_aPlayer[nIdy].ColAttack[nCnt].bUse == true)
		{
			if (g_aPlayer[nIdy].ColAttack[nCnt].nMotionType == g_aPlayer[nIdy].nMotionType)
			{// ���߂��X�e�[�g�ƍ��̃X�e�[�g���ꏏ�������画��J�n
				if (g_aPlayer[nIdy].nFrameAllCounter >= g_aPlayer[nIdy].ColAttack[nCnt].nCollisionStart &&
					g_aPlayer[nIdy].nFrameAllCounter <= g_aPlayer[nIdy].ColAttack[nCnt].nCollisionEnd)
				{
					if (CollisionModelMinMax(&D3DXVECTOR3(g_aPlayer[nIdy].ColAttack[nCnt].mtxWorld._41, g_aPlayer[nIdy].ColAttack[nCnt].mtxWorld._42, g_aPlayer[nIdy].ColAttack[nCnt].mtxWorld._43),
						&D3DXVECTOR3(g_aPlayer[nIdy].aModel[0].mtxWorld._41, g_aPlayer[nIdy].aModel[0].mtxWorld._42, g_aPlayer[nIdy].aModel[0].mtxWorld._43), &D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						g_aPlayer[nIdy].ColAttack[nCnt].fRadius, g_aPlayer[nIdy].ColAttack[nCnt].fRadius, &g_aPlayer[nIdy].pModel) != 0)
					{
					}
					PlayerMotionEffect(nIdy, nCnt);
					// �U�������蔻��

					if (CollisionModelGetInfo(&D3DXVECTOR3(g_aPlayer[nIdy].ColAttack[nCnt].mtxWorld._41, g_aPlayer[nIdy].ColAttack[nCnt].mtxWorld._42, g_aPlayer[nIdy].ColAttack[nCnt].mtxWorld._43),
						g_aPlayer[nIdy].ColAttack[nCnt].fRadius, g_aPlayer[nIdy].ColAttack[nCnt].fRadius, &g_aPlayer[nIdy].pModelAtkData) != 0)
					{
					}
					if (g_aPlayer[nIdy].pModelAtkData != NULL)
					{
					}
				}
			}
		}
	}

}

//=====================================================================================================================
// ���C�t�[��
//=====================================================================================================================
void PlayerLifeZero(int nCnt, float fAngle, float fDamageH, float fDamageV)
{
	g_nNumAll--;
	g_aPlayer[nCnt].state = PLAYERSTATE_DEATH;
	g_aPlayer[nCnt].nCntState = 100;

	// ���������̐�����ї͂���
	g_aPlayer[nCnt].fFlyPower.x = sinf(fAngle - D3DX_PI) * fDamageH;
	g_aPlayer[nCnt].fFlyPower.z = cosf(fAngle - D3DX_PI) * fDamageH;

	// ���������̐�����ї͂���
	g_aPlayer[nCnt].fFlyPower.y = fDamageV;
	g_aPlayer[nCnt].nFlyCounter = 20;
	g_aPlayer[nCnt].bMotionState[PLAYER_BOOL_FLY] = true;													// ������ђ��ɐݒ�
	g_aPlayer[nCnt].Destrot.y = fAngle - D3DX_PI;							// �L�����N�^�[�̌�����ύX
	g_aPlayer[nCnt].rot.y = fAngle - D3DX_PI;							// �L�����N�^�[�̌�����ύX

}
//=========================================================================================================================
// �{�C�X
//===========================================================================================================================
void SetPlayerVoice(PlayerMotionType motion)
{
	if (motion == PLAYER_MOTION_TYPE_FLY)
	{
		PlaySound(SOUND_HUMAN_BUTOBI);
	}
	if (motion == PLAYER_MOTION_TYPE_MAX)
	{
		PlaySound(SOUND_HUMAN_GARD000);
	}
}
//==============================================================================================================================
//�@���C�t�̃`�F�b�N
//==============================================================================================================================
int CheckLife(void)
{
	//�ϐ��錾
	int BigLife = 0;

	for (int nCntId = 0; nCntId < MAX_PLAYER - 1; nCntId++)
	{
		if (g_aPlayer[BigLife].fLife > g_aPlayer[nCntId + 1].fLife)
		{
			BigLife = BigLife;		//Id�̕ۑ�
		}
		else
		{
			BigLife = nCntId + 1;	//Id�̕ۑ�
		}
	}

	return BigLife;
}