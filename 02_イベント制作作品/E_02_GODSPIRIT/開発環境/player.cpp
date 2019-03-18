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
#include "effect.h"
#include "model.h"
#include "fade.h"
#include "result.h"
#include "item.h"
#include "meshField.h"
#include "meshWall.h"
#include "line.h"
#include "orbit.h"
#include "gamepad.h"
#include "enemy.h"
#include "life.h"
#include "sound.h"
#include "effectUP.h"
#include "UI.h"
#include "game.h"
#include "tutorial.h"
#include "effect3D.h"
#include "pause.h"
#include "spirit.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define DOWN_SPEED						(0.4f)	// ������
#define PLAYER_ROT_SPEED_NORMAL			(0.15f)	// �ʏ펞�̊p�x��ς����
#define PLAYER_ROT_SPEED_ATTACK			(0.02f)	// �U�����̊p�x��ς����
#define GRAVITY_SPEED					(0.6f)	// �d�͉��Z�X�s�[�h
#define INIT_JUMP_SPEED					(7)		// �W�����v��
#define INIT_MOVE_SPEED					(0.3f)	// �ړ���
#define PLAYER_MOTION_STEP_MOVE_SPEED	(5.0f)	// �X�e�b�v�̃X�s�[�h
#define BLEND_FRAME						(3)		// �u�����h
#define ATTACKDAMAGE					(1)		// �A�^�b�N�̃_���[�W��
#define WARKZONE						(50)	// �����̃]�[��
#define PLAYER_MOTION_MOVE_ZONE			(1.0f)	// �����̃��[�V�����]�[��
#define AIR_COUNT						(2)		// �󒆂ōs���ł����
#define PLAYER_SPIRIT_TIME				(30)	// �X�s���b�g��1��������t���[����
#define PLAYER_SPIRIT_RECOVERY_TIME		(10)	// �X�s���b�g��1�񕜂���t���[����
#define PLAYER_SPIRIT_DAMAGE			(1)		// �X�s���b�g�������̃_���[�W

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
Player g_aPlayer[MAX_PLAYER];
int g_nKeyCounter;			// �L�[��
int g_nFrameCounter;		// �t���[����
int g_nFrameAllCounter;		// �ő�t���[����
int g_nFrame;				// �t���[
int g_nSpiritCounter;			// �X�s���b�g�����̃J�E���^�[
int g_nSpiritRecoveryCounter;	// �X�s���b�g�񕜂̃J�E���^�[

//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �ʒu�E�����̏����ݒ�
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aPlayer[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, -0.0f);
		g_aPlayer[nCntPlayer].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer[nCntPlayer].Diffpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer[nCntPlayer].Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		if (GetMode() == MODE_GAME)
		{
			g_aPlayer[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, -0.0f);
			g_aPlayer[nCntPlayer].posOld = D3DXVECTOR3(0.0f, 0.0f, -0.0f);
			g_aPlayer[nCntPlayer].Diffpos = D3DXVECTOR3(0.0f, 0.0f, -0.0f);
			g_aPlayer[nCntPlayer].Initpos = D3DXVECTOR3(0.0f, 0.0f, -0.0f);
		}
		g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_NEUTRAL;
		g_aPlayer[nCntPlayer].nMotionTypeOld = PLAYER_MOTION_TYPE_MAX;
		g_aPlayer[nCntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer[nCntPlayer].Diffrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer[nCntPlayer].Destrot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		g_aPlayer[nCntPlayer].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
		{
			g_aPlayer[nCntPlayer].bSound[nCntSound] = false;
		}
		g_aPlayer[nCntPlayer].nLife = g_aPlayer[nCntPlayer].nInitLife;
		g_aPlayer[nCntPlayer].nSpirit = g_aPlayer[nCntPlayer].nInitSpirit;
		g_aPlayer[nCntPlayer].nIdyLockEnemy = 0;
		g_aPlayer[nCntPlayer].state = PLAYERSTATE_NONE;
		g_aPlayer[nCntPlayer].nCntState = 0;
		g_aPlayer[nCntPlayer].pMeshFieldLand = NULL;
		for (int nbMotion = 0; nbMotion < PLAYER_BOOL_MAX; nbMotion++)
		{
			g_aPlayer[nCntPlayer].bMotionState[nbMotion] = false;
			g_aPlayer[nCntPlayer].bMotionStateOld[nbMotion] = false;
		}
		for (int nbMotion = 0; nbMotion < PLAYER_ATTACK_TYPE_MAX; nbMotion++)
		{
			g_aPlayer[nCntPlayer].bAttackType[nbMotion];
		}
		g_aPlayer[nCntPlayer].bBlend = false;
		g_aPlayer[nCntPlayer].nDamage = 0;
		g_aPlayer[nCntPlayer].bAttention = false;
		g_aPlayer[nCntPlayer].bJump = false;
		g_aPlayer[nCntPlayer].nFlyCounter = 0;						// ���ł��鎞��
		g_aPlayer[nCntPlayer].fRotSpeed = 0.0f;						// ��]�̌����X�s�[�h
		g_aPlayer[nCntPlayer].nAirCounter = 0;							// ���V���̍U������J�E���^�[
		g_aPlayer[nCntPlayer].nLoopCounter = 0;						// ���[�v�J�E���^�[
		g_aPlayer[nCntPlayer].nWeaponState = PLAYERWEAPON_HAND;		// �ŏ��͑f��
		for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
		{// ����̏��
			g_aPlayer[nCntPlayer].bWeaponNow[nCntWeapon] = false;
			for (int nCntParts = 0; nCntParts < g_aPlayer[nCntPlayer].nNumParts; nCntParts++)
			{// Key - ���� �Ԃ̍����v�Z
				g_aPlayer[nCntPlayer].aModel[nCntParts][nCntWeapon].rot = g_aPlayer[nCntPlayer].aMotion[PLAYER_MOTION_TYPE_JUMP][nCntWeapon].aKey[0].aPartsKey[nCntParts].rot;
			}
		}
		g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_HAND] = true;// �ŏ��͑f��
	
		 // �e�ݒ�
		g_aPlayer[nCntPlayer].nIdyShadow = SetShadow(g_aPlayer[nCntPlayer].pos, g_aPlayer[nCntPlayer].rot, g_aPlayer[nCntPlayer].fRadiusWidth, g_aPlayer[nCntPlayer].fRadiusWidth);
		// ���C������ݒ�
		g_aPlayer[nCntPlayer].nIdyLine = CollisionLine(g_aPlayer[nCntPlayer].pos, D3DXVECTOR3(-g_aPlayer[nCntPlayer].fRadiusWidth, 0.0f, -g_aPlayer[nCntPlayer].fRadiusWidth), D3DXVECTOR3(g_aPlayer[nCntPlayer].fRadiusWidth, g_aPlayer[nCntPlayer].fRadiusHeight, g_aPlayer[nCntPlayer].fRadiusWidth), LINETYPE_PLAYER);
		// ���C�t�̐ݒ�
		SetPlayerLife(g_aPlayer[nCntPlayer].nInitLife);
		// �������������̐ݒ�
		SetPlayerSpirit(g_aPlayer[nCntPlayer].nInitSpirit);


	}
	g_nKeyCounter = 0;
	g_nFrameCounter = 0;
	g_nFrameAllCounter = 0;
	g_nFrame = 0;
	g_nSpiritCounter = 0;
	g_nSpiritRecoveryCounter = 0;

}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
		{// ����̏��
			for (int nCntPlayerModel = 0; nCntPlayerModel < g_aPlayer[nCntPlayer].nNumParts; nCntPlayerModel++)
			{
				// ���b�V���̊J��
				if (g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pMesh != NULL)
				{
					g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pMesh->Release();
					g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pMesh = NULL;
				}
				// �}�e���A���̊J��
				if (g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pBuffMat != NULL)
				{
					g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pBuffMat->Release();
					g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pBuffMat = NULL;
				}
				// �e�N�X�`���̊J��
				for (int nCntMat = 0; nCntMat < int(g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].nNumMat); nCntMat++)
				{
					if (g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pTextuer[nCntMat] != NULL)
					{
						g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pTextuer[nCntMat]->Release();
						g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pTextuer[nCntMat] = NULL;
					}
				}
			}
		}
	}
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdatePlayer(int nCntPlayer)
{


	DIJOYSTATE2 *pGamePad = GetgamePadStick();
	MeshField *pMeshField = GetMeshField();
	Camera *pCamera = GetCamera();
	Model *pModel = GetModel();
	UI *pUI = GetUI();
	float nDirection = 0;
	float nDirectionY = 0;
	float nDirectionZ = 0;
	nDirection = SetRandomEffect();
	nDirectionY = SetRandomEffect();
	nDirectionZ = SetRandomEffect();

	// �O��̈ʒu �^�C�v
	g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;
	g_aPlayer[nCntPlayer].nMotionTypeOld = g_aPlayer[nCntPlayer].nMotionType;
	for (int nbMotion = 0; nbMotion < PLAYER_BOOL_MAX; nbMotion++)
	{
		g_aPlayer[nCntPlayer].bMotionStateOld[nbMotion] = g_aPlayer[nCntPlayer].bMotionState[nbMotion];
	}

	switch (g_aPlayer[nCntPlayer].state)
	{
	case PLAYERSTATE_NONE:
		break;
	case PLAYERSTATE_DAMAGESTART:
		g_aPlayer[nCntPlayer].state = PLAYERSTATE_DAMAGE;
		break;
	case PLAYERSTATE_DAMAGE:
		g_aPlayer[nCntPlayer].nCntState--;
		if (g_aPlayer[nCntPlayer].nCntState <= 0)
		{
			g_aPlayer[nCntPlayer].state = PLAYERSTATE_NONE;
		}
		break;
	case PLAYERSTATE_DEATH:
		g_aPlayer[nCntPlayer].nCntState--;
		GetCamera()->rot.y = -D3DX_PI / 4 + GetPlayer()->rot.y;
		GetCamera()->fLength = 150;
		GetCamera()->fHeight = 270;
		GetCamera()->fLengthR = 1;
		GetCamera()->fHeightR = 1;
		if (g_aPlayer[nCntPlayer].nCntState <= 0)
		{
			if (GetGameState() != GAMESTATE_END&&GetGameState() != GAMESTATE_END2)
			{
				SetGameState(GAMESTATE_END);
				SetResultType(RESULTTYPE_GAMEOVER);
		
			}
		}
		break;
	}
	// ����̏��
	switch (g_aPlayer[nCntPlayer].nWeaponState)
	{
	case PLAYERWEAPON_HAND:	// ��
		if (g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_HAND] == false)
		{// ����̏�Ԃ��������
			g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_HAND] = true;
			g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_CHANGE] = true;
		}
		pUI[UITEX_PUNCH].col.a = 1.0f;
		pUI[UITEX_LANCE].col.a = 0.5f;
		pUI[UITEX_SORD].col.a = 0.5f;
		pUI[UITEX_HAMMER].col.a = 0.5f;
		break;
	case PLAYERWEAPON_SORD:	// �ނ炭��
		if (g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_SORD] == false)
		{// ����̏�Ԃ��������
			PlaySound(SOUND_LABEL_SE_SPIN_KICK);
			g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_SORD] = true;
			g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_CHANGE] = true;
			for (int nCntEffect = 0; nCntEffect < 30; nCntEffect++)
			{
				nDirection = SetRandomEffect();
				nDirectionY = SetRandomEffect();
				nDirectionZ = SetRandomEffect();
				SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41 + sinf(nDirection) * 10, g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42 + cosf(nDirection) * 10, g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43 + cosf(nDirection) * 10),
					D3DXVECTOR3(sinf(nDirection), cosf(nDirectionY) * float((rand() % 40 / 10.0f) + 2) + float(rand() % 200 / 100.0f), cosf(nDirectionZ)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, float(rand() % 100 / 100.0f), 0.0f, float(rand() % 200 / 100.0f) + 0.5f), float(rand() % 100) + 30, 10, EFFECTTYPE_FIRE);
			}
		}
		SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41 + sinf(nDirection) * 10, g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42 + cosf(nDirection) * 10, g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43 + cosf(nDirection) * 10),
			D3DXVECTOR3(sinf(nDirection), cosf(nDirectionY) * float((rand() % 40 / 10.0f) + 2) + float(rand() % 200 / 100.0f), cosf(nDirectionZ)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, float(rand() % 100 / 100.0f), 0.0f, float(rand() % 100 / 100.0f) + 0.5f), float(rand() % 50) + 5, 10, EFFECTTYPE_FIRE);
		pUI[UITEX_PUNCH].col.a = 0.3f;
		pUI[UITEX_LANCE].col.a = 0.3f;
		pUI[UITEX_SORD].col.a = 1.0f;
		pUI[UITEX_HAMMER].col.a = 0.3f;
		break;
	case PLAYERWEAPON_LANCE:// �|�Z�C�h��
		if (g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_LANCE] == false)
		{// ����̏�Ԃ��������
			PlaySound(SOUND_LABEL_SE_SPIN_PUNCH);
			g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_LANCE] = true;
			g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_CHANGE] = true;
			for (int nCntEffect = 0; nCntEffect < 100; nCntEffect++)
			{
				nDirection = SetRandomEffect();
				nDirectionY = SetRandomEffect();
				nDirectionZ = SetRandomEffect();
				SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41 + sinf(nDirection)* (rand() % 50), g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42 + (rand() % 500) + 100, g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43 + cosf(nDirectionZ) * (rand() % 50)),
					D3DXVECTOR3(sinf(nDirection) * float((rand() % 11 / 10.0f)), cosf(nDirectionY) *  float((rand() % 11 / 10.0f)), cosf(nDirectionZ) *  float((rand() % 11 / 10.0f))), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), float(rand() % 30) + 1, 10, EFFECTTYPE_WATER_DOWN);
			}
		}
		SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43),
			D3DXVECTOR3(sinf(nDirection) * float(rand() % 5), cosf(nDirectionY) * float(rand() % 5), cosf(nDirectionZ)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 20), 30, EFFECTTYPE_WATER);
		pUI[UITEX_PUNCH].col.a = 0.3f;
		pUI[UITEX_LANCE].col.a = 1.0f;
		pUI[UITEX_SORD].col.a = 0.3f;
		pUI[UITEX_HAMMER].col.a = 0.3f;
		break;
	case PLAYERWEAPON_HAMMER:// �ɂ��ɂ�
		if (g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_HAMMER] == false)
		{// ����̏�Ԃ��������
			PlaySound(SOUND_LABEL_SE_JUMP_PUNCH);

			for (int nCntEffect = 0; nCntEffect < 100; nCntEffect++)
			{
				nDirection = SetRandomEffect();
				nDirectionY = SetRandomEffect();
				nDirectionZ = SetRandomEffect();
				SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41 + sinf(nDirection)* (rand() % 50), g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42 +  (rand() %500)+100, g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43 + cosf(nDirectionZ) * (rand() % 50)),
					D3DXVECTOR3(sinf(nDirection) * float((rand() % 11 / 10.0f)), cosf(nDirectionY) *  float((rand() % 11 / 10.0f)), cosf(nDirectionZ) *  float((rand() % 11 / 10.0f))), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), float(rand() % 50) + 30, 10, EFFECTTYPE_SUNDER_DOWN);
			}
			g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_HAMMER] = true;
			g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_CHANGE] = true;
		}
		SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41 + sinf(nDirection)* (rand() % 10), g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42 + cosf(nDirection) * (rand() % 10), g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43 + cosf(nDirectionZ) * (rand() % 10)),
			D3DXVECTOR3(sinf(nDirection) * float((rand() % 11 / 10.0f)), cosf(nDirectionY) *  float((rand() % 11 / 10.0f)), cosf(nDirectionZ) *  float((rand() % 11 / 10.0f))), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), float(rand() % 15) + 1, 10, EFFECTTYPE_SUNDER1);

		pUI[UITEX_PUNCH].col.a = 0.3f;
		pUI[UITEX_LANCE].col.a = 0.3f;
		pUI[UITEX_SORD].col.a = 0.3f;
		pUI[UITEX_HAMMER].col.a = 1.0f;
		break;
	}
	if (g_aPlayer[nCntPlayer].pos.y <= -500)
	{// ���ɗ������玀��
		g_aPlayer[nCntPlayer].nLife--;
		GetPlayerLife(g_aPlayer[nCntPlayer].nLife);
		if (g_aPlayer[nCntPlayer].nLife <= 0)
		{
			if (GetGameState() != GAMESTATE_END&&GetGameState() != GAMESTATE_END2)
			{
				SetGameState(GAMESTATE_END);
				SetResultType(RESULTTYPE_GAMEOVER);
				StopSound();				// �T�E���h�I��
				PlaySound(SOUND_LABEL_VOICE_DEATH);
				PlaySound(SOUND_LABEL_DEATH);
			}
		}
	}
	if (g_aPlayer[nCntPlayer].state != PLAYERSTATE_DEATH)
	{// �����Ă���Ȃ�
		if (pCamera->nState == CAMERASTATE_NONE)
		{

			if (GetMode() != MODE_RESULT)
			{
				PlayerController(nCntPlayer);
			}
			if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_DAMAGE] == false &&
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] == false && g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_GETUP] == false)
			{// ������т��Ă��Ȃ����
				if (GetKeyboardTrigger(DIK_K) == true || GetGamePadTrigger(BUTTON_B) == true)
				{// �U��
					if (g_aPlayer[nCntPlayer].bAttention == true)
					{// ���ڂ��Ă�����G�̕����֍U��
						g_aPlayer[nCntPlayer].Destrot.y = pCamera->rot.y - D3DX_PI;
						g_aPlayer[nCntPlayer].rot.y = pCamera->rot.y - D3DX_PI;
					}
					if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_2] == false&& g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_1] == true)
					{// �U��2
						g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_2] = true;
					}
					else if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_1] == false&&g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_0] == true)
					{// �U��1
						g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_1] = true;
					}
					else if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_0] == false)
					{// �U��0
						g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_0] = true;
					}
				}
				if (GetKeyboardTrigger(DIK_I) == true || GetGamePadTrigger(BUTTON_Y) == true)
				{// ���U��
					if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_BIG] == false)
					{// ���U��
						g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_BIG] = true;
					}
					if (g_aPlayer[nCntPlayer].bAttention == true)
					{// ���ڂ��Ă�����G�̕����֍U��
						g_aPlayer[nCntPlayer].Destrot.y = pCamera->rot.y - D3DX_PI;
						g_aPlayer[nCntPlayer].rot.y = pCamera->rot.y - D3DX_PI;
					}
				}
				if (GetKeyboardTrigger(DIK_J) == true || GetGamePadTrigger(BUTTON_R1) == true)
				{// �X�e�b�v
					if (g_aPlayer[nCntPlayer].nAirCounter < AIR_COUNT)
					{
						g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_STEP] = true;
					}
					PlaySound(SOUND_LABEL_SE_STEP);
				}
			}
			if (g_aPlayer[nCntPlayer].bJump == false && g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_JUMP] == false && g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_DAMAGE] == false &&
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] == false && g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_GETUP] == false)
			{// �W�����v&������т��Ă��Ȃ����
				if (GetKeyboardTrigger(DIK_SPACE) == true || GetGamePadTrigger(BUTTON_A) == true)
				{// �W�����v
					PlaySound(SOUND_LABEL_SE_JUMP);

					g_aPlayer[nCntPlayer].move.y = g_aPlayer[nCntPlayer].JumpSpeed;	// �ړ��ʉ��Z
					g_aPlayer[nCntPlayer].bJump = true;
					g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_JUMP] = true;
				}
			}
			if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] == true)
			{// ������ї͂��܂���������
				if (g_aPlayer[nCntPlayer].fFlyPower.x >= 0.1f ||
					g_aPlayer[nCntPlayer].fFlyPower.x <= -0.1f)
				{// ���������̐�����ї͂�����Ȃ�
					g_aPlayer[nCntPlayer].move.x += g_aPlayer[nCntPlayer].fFlyPower.x;	// ������ї͂����Z
				}
				if (g_aPlayer[nCntPlayer].fFlyPower.z >= 0.1f ||
					g_aPlayer[nCntPlayer].fFlyPower.z <= -0.1f)
				{// ���������̐�����ї͂�����Ȃ�
					g_aPlayer[nCntPlayer].move.z += g_aPlayer[nCntPlayer].fFlyPower.z;	// ������ї͂����Z
				}

				if (g_aPlayer[nCntPlayer].nFlyCounter > 0)
				{// ������ԃJ�E���^�[��0���傫��������
					g_aPlayer[nCntPlayer].fFlyPower.y -= 0.5f;
					if (g_aPlayer[nCntPlayer].fFlyPower.y > 0.0f)
					{// �W�����v�͂��܂���������
						g_aPlayer[nCntPlayer].move.y += g_aPlayer[nCntPlayer].fFlyPower.y;			// �㏸���Ă���
					}
				}
			}
			// �����̉�]���x�̌���
			if (g_aPlayer[nCntPlayer].nMotionType != PLAYER_MOTION_TYPE_GETUP &&
				g_aPlayer[nCntPlayer].nMotionType != PLAYER_MOTION_TYPE_STEP)
			{// �N���オ�莞����Ȃ����
				g_aPlayer[nCntPlayer].fRotSpeed = PLAYER_ROT_SPEED_NORMAL;			// �ʏ�̉�]���x������
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// �U���̎�ޕ����[�v
					if (g_aPlayer[nCntPlayer].bMotionState[nCntAttack] == true)
					{// �U������������
						g_aPlayer[nCntPlayer].fRotSpeed = PLAYER_ROT_SPEED_ATTACK;	// �����̉�]���x��x������
					}
				}
			}
		}
	}
	// �d�͉��Z
	g_aPlayer[nCntPlayer].move.y += -GRAVITY_SPEED;
	if ( g_aPlayer[nCntPlayer].nMotionType == PLAYER_MOTION_TYPE_STEP)
	{
		g_aPlayer[nCntPlayer].move.y = 0;
	}
	// �ړ��ʉ��Z
	g_aPlayer[nCntPlayer].pos += g_aPlayer[nCntPlayer].move;
	// �����X�V
	if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] == false)
	{
		g_aPlayer[nCntPlayer].move.x += (0 - g_aPlayer[nCntPlayer].move.x)*DOWN_SPEED;
		g_aPlayer[nCntPlayer].move.z += (0 - g_aPlayer[nCntPlayer].move.z)*DOWN_SPEED;
	}
	//--------------------------/ ���f���̓������Ȃ߂炩�ɂ��� /---------------------------------------------------------------------------------------//
	if (g_aPlayer[nCntPlayer].Destrot.y > D3DX_PI)
	{// ������3.14�𒴂���Ȃ�
		g_aPlayer[nCntPlayer].Destrot.y -= D3DX_PI * 2.0f;
	}
	if (g_aPlayer[nCntPlayer].Destrot.y < -D3DX_PI)
	{// ������-3.14�𒴂���Ȃ�
		g_aPlayer[nCntPlayer].Destrot.y += D3DX_PI * 2.0f;
	}
	g_aPlayer[nCntPlayer].Diffrot.y = g_aPlayer[nCntPlayer].Destrot.y - g_aPlayer[nCntPlayer].rot.y;// ���݂̊p�x�ƖړI�̊p�x�̍���
	if (g_aPlayer[nCntPlayer].Diffrot.y > D3DX_PI)
	{// ������3.14�𒴂���Ȃ�
		g_aPlayer[nCntPlayer].Diffrot.y -= D3DX_PI * 2.0f;
	}
	if (g_aPlayer[nCntPlayer].Diffrot.y < -D3DX_PI)
	{// ������-3.14�𒴂���Ȃ�
		g_aPlayer[nCntPlayer].Diffrot.y += D3DX_PI * 2.0f;
	}
	g_aPlayer[nCntPlayer].rot.y += g_aPlayer[nCntPlayer].Diffrot.y * g_aPlayer[nCntPlayer].fRotSpeed;	// �p�x�̉��Z
	if (g_aPlayer[nCntPlayer].rot.y > D3DX_PI)
	{// ���݂̊p�x��3.14�𒴂���Ȃ�
		g_aPlayer[nCntPlayer].rot.y -= D3DX_PI * 2.0f;
	}
	if (g_aPlayer[nCntPlayer].rot.y < -D3DX_PI)
	{// ���݂̊p�x��-3.14�𒴂���Ȃ�
		g_aPlayer[nCntPlayer].rot.y += D3DX_PI * 2.0f;
	}
	//----------------------------------/ �����蔻�� /---------------------------------------------------------------------------------------//
	// �Փ˔��菈��
	if (g_aPlayer[nCntPlayer].state != PLAYERSTATE_DEATH)
	{
		CollisionEnemy(&g_aPlayer[nCntPlayer].pos, g_aPlayer[nCntPlayer].fRadiusWidth);			// �L�����N�^�[���d�Ȃ�Ȃ����߂̓����蔻��
	}
	// ���b�V���t�B�[���h
	if (CollisionMeshField(&g_aPlayer[nCntPlayer].pos, &g_aPlayer[nCntPlayer].posOld, &g_aPlayer[nCntPlayer].move,
		g_aPlayer[nCntPlayer].fRadiusWidth, g_aPlayer[nCntPlayer].fRadiusHeight, &g_aPlayer[nCntPlayer].pMeshFieldLand) == true)
	{
		Shadow *pShadow = GetShadow();
		pShadow[g_aPlayer[nCntPlayer].nIdyShadow].pos.y = g_aPlayer[nCntPlayer].pos.y + 0.1f;
		pShadow[g_aPlayer[nCntPlayer].nIdyShadow].col.a = 1.0f;
		g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_JUMP] = false;
		g_aPlayer[nCntPlayer].bJump = false;
		g_aPlayer[nCntPlayer].nAirCounter = 0;// �󒆍U������J�E���^�[

		if (g_aPlayer[nCntPlayer].state != PLAYERSTATE_DEATH)
		{
			if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] == true)
			{
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] = false;
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_GETUP] = true;
				for (int nCntEffect = 0; nCntEffect < 2; nCntEffect++)
				{// ���n�G�t�F�N�g
					float nDirection = SetRandomEffect();
					float nDirection2 = SetRandomEffect();
					SetEffect(g_aPlayer[nCntPlayer].pos, D3DXVECTOR3(sinf(nDirection) * float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_WIND);
					SetEffect(g_aPlayer[nCntPlayer].pos, D3DXVECTOR3(sinf(nDirection2) *  float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection2) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_WIND);
				}
			}
		}
	
	}
	else
	{
		StopSound(SOUND_LABEL_SE_GRAUND);
		g_aPlayer[nCntPlayer].bSound[SOUND_LABEL_SE_GRAUND] = false;
	}
	if (g_aPlayer[nCntPlayer].pMeshFieldLand != NULL)
	{// ���ɏ���Ă���
		if ((g_aPlayer[nCntPlayer].move.x > 1.0f || g_aPlayer[nCntPlayer].move.z > 1.0f ||
			g_aPlayer[nCntPlayer].move.x < -1.0f || g_aPlayer[nCntPlayer].move.z < -1.0f))
		{// �����Ă���
			if (g_aPlayer[nCntPlayer].bSound[SOUND_LABEL_SE_GRAUND] == false)
			{
				PlaySound(SOUND_LABEL_SE_GRAUND);
				g_aPlayer[nCntPlayer].bSound[SOUND_LABEL_SE_GRAUND] = true;
			}
			for (int nCntEffect = 0; nCntEffect < 1; nCntEffect++)
			{// �n�ʂɂ��邭��
				float nDirection = SetRandomEffect();
				float nDirectio2 = SetRandomEffect();
				SetEffect(g_aPlayer[nCntPlayer].pos,
					D3DXVECTOR3(sinf(nDirection) * float(rand() % 10 / 10.0f), cosf(nDirectio2) * float(rand() % 5 / 10.0f), cosf(nDirection)*float(rand() % 10 / 10.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.5f, 0.4f, 0.4f, 0.8f), float(rand() % 10 + 20), 30, EFFECTTYPE_PLAYER);
			}
		}
		else
		{
			StopSound(SOUND_LABEL_SE_GRAUND);
			g_aPlayer[nCntPlayer].bSound[SOUND_LABEL_SE_GRAUND] = false;
		}
	}
	// �A�C�e���l��
	CollisionItem(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43), g_aPlayer[nCntPlayer].fRadiusWidth);
	// ���f��
	if (CollisionModelMinMax(&g_aPlayer[nCntPlayer].pos, &g_aPlayer[nCntPlayer].posOld, &g_aPlayer[nCntPlayer].move, g_aPlayer[nCntPlayer].fRadiusWidth, g_aPlayer[nCntPlayer].fRadiusHeight, &g_aPlayer[nCntPlayer].pModel) == 2)
	{
		Shadow *pShadow = GetShadow();
		pShadow[g_aPlayer[nCntPlayer].nIdyShadow].pos.y = g_aPlayer[nCntPlayer].pos.y + 0.1f;
		pShadow[g_aPlayer[nCntPlayer].nIdyShadow].col.a = 1.0f;
		g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_JUMP] = false;
		g_aPlayer[nCntPlayer].bJump = false;
		g_aPlayer[nCntPlayer].nAirCounter = 0;// �󒆍U������J�E���^�[

		if (g_aPlayer[nCntPlayer].state != PLAYERSTATE_DEATH)
		{
			if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] == true)
			{
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] = false;
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_GETUP] = true;
				for (int nCntEffect = 0; nCntEffect < 2; nCntEffect++)
				{// ���n�G�t�F�N�g
					float nDirection = SetRandomEffect();
					float nDirection2 = SetRandomEffect();
					SetEffect(g_aPlayer[nCntPlayer].pos, D3DXVECTOR3(sinf(nDirection) * float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_WIND);
					SetEffect(g_aPlayer[nCntPlayer].pos, D3DXVECTOR3(sinf(nDirection2) *  float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection2) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_WIND);
				}
			}
		}

	}
	// ���b�V���E�H�[��
	if (CollisionMeshWall(&g_aPlayer[nCntPlayer].pos, &g_aPlayer[nCntPlayer].posOld, &g_aPlayer[nCntPlayer].move, g_aPlayer[nCntPlayer].fRadiusWidth, g_aPlayer[nCntPlayer].fRadiusHeight) == true)
	{

	}
	// �U�����菈��
	for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
	{
		if (g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].bUse == true)
		{
			if (g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].nMotionType == g_aPlayer[nCntPlayer].nMotionType)
			{// ���߂��X�e�[�g�ƍ��̃X�e�[�g���ꏏ�������画��J�n
				if (g_nFrameAllCounter >= g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].nCollisionStart &&
					g_nFrameAllCounter <= g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].nCollisionEnd)
				{
					// �G�t�F�N�g
					PlayerMotionEffect(nCntPlayer, nCollision,g_aPlayer[nCntPlayer].nWeaponState);
					CollisionEnemyBody(g_aPlayer[nCntPlayer].pos,
						D3DXVECTOR3(g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43),
						g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].fRadius,
						g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].nAttackPower,
						g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].fDamageHorizontal,
						g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].fDamageVertical);					// �G�̑̂ƍU���̓����蔻��
					if (CollisionModelMinMax(&D3DXVECTOR3(g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43),
						&D3DXVECTOR3(g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorldOld._41, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorldOld._42, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorldOld._43), &g_aPlayer[nCntPlayer].move,
						g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].fRadius, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].fRadius, &g_aPlayer[nCntPlayer].pModel) != 0)
					{
						PlaySound(SOUND_LABEL_SE_ATTACKWOOD);
						for (int nCntEffect = 0; nCntEffect < 21; nCntEffect++)
						{// �Ή�
							float nDirection = SetRandomEffect();
							float nDirectio2 = SetRandomEffect();
							SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43),
								D3DXVECTOR3(sinf(nDirection) * 5, cosf(nDirection) * 5, cosf(nDirectio2)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								D3DXCOLOR(1.0f, 0.3f, 0.0f, 0.8f), 20, 30, EFFECTTYPE_EXPLOSION);
							SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43),
								D3DXVECTOR3(sinf(nDirectio2) * 5, cosf(nDirection), cosf(nDirectio2) * 5), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								D3DXCOLOR(0.8f, 0.8f, 0.0f, 0.8f), 20, 30, EFFECTTYPE_EXPLOSION);
						}
					}
				}
			}
		}
	}
	if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_JUMP] != g_aPlayer[nCntPlayer].bMotionStateOld[PLAYER_BOOL_JUMP])
	{// �O��ƍ���̃W�����v���[�V������������璅�n���[�V����
		g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_LANDING] = true;

		for (int nCntEffect = 0; nCntEffect < 10; nCntEffect++)
		{// ���n�G�t�F�N�g
			float nDirection = SetRandomEffect();
			float nDirection2 = SetRandomEffect();
			SetEffect(g_aPlayer[nCntPlayer].pos, D3DXVECTOR3(sinf(nDirection) * float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_WIND);
			SetEffect(g_aPlayer[nCntPlayer].pos, D3DXVECTOR3(sinf(nDirection2) *  float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection2) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_WIND);
		}
	}
	//------------------------------------/ ��{���[�V���� /-----------------------------------------------------------------/

	if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_0] == false&& g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_1] == false
		&&g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_2] == false&& g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_BIG] == false)
	{// �U����Ԃ���Ȃ����
		if (GetPovTrigger(POV_DOWN) == true || GetKeyboardTrigger(DIK_1) == true)
		{// �f��
			g_aPlayer[nCntPlayer].nWeaponState = PLAYERWEAPON_HAND;
			for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
			{// ����̏��
				g_aPlayer[nCntPlayer].bWeaponNow[nCntWeapon] = false;
			}
		}
		if (GetPovTrigger(POV_UP) == true || GetKeyboardTrigger(DIK_2) == true)
		{// ��
			g_aPlayer[nCntPlayer].nWeaponState = PLAYERWEAPON_SORD;
			for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
			{// ����̏��
				g_aPlayer[nCntPlayer].bWeaponNow[nCntWeapon] = false;
			}
		}
		if (GetPovTrigger(POV_RIGHT) == true || GetKeyboardTrigger(DIK_3) == true)
		{// ��
			g_aPlayer[nCntPlayer].nWeaponState = PLAYERWEAPON_LANCE;
			for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
			{// ����̏��
				g_aPlayer[nCntPlayer].bWeaponNow[nCntWeapon] = false;
			}
		}
		if (GetPovTrigger(POV_LEFT) == true || GetKeyboardTrigger(DIK_4) == true)
		{// �n���}�[
			g_aPlayer[nCntPlayer].nWeaponState = PLAYERWEAPON_HAMMER;
			for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
			{// ����̏��
				g_aPlayer[nCntPlayer].bWeaponNow[nCntWeapon] = false;
			}
		}
		if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_CHANGE] == true)
		{
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_CHANGE;

		}
		else if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_STEP] == true)
		{// �X�e�b�v��ԂȂ�
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_STEP;

			if (g_nFrameAllCounter >= 1 &&
				g_nFrameAllCounter <= 5)
			{// ���[�V�����̊Ԃ̎�
				g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 10);	// �v���C���[�̌����Ă�������ɋ}����
				g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 10);	// �v���C���[�̌����Ă�������ɋ}����
			}
		}
		else if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_GETUP] == true)
		{// �|��Ă��ԂȂ�
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_GETUP;
		}
		else if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] == true)
		{// �_���[�W(�������)���Ȃ�
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_FLY;
		}
		else if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_DAMAGE] == true)
		{// �_���[�W(������)���Ȃ�
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_DAMAGE;
		}
		else if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_JUMP] == true)
		{// �v���C���[���W�����v���Ȃ�
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_JUMP;
		}
		else if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_LANDING] == true)
		{// ���n���Ȃ�
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_LANDING;
		}
		else if (g_aPlayer[nCntPlayer].move.x > PLAYER_MOTION_MOVE_ZONE ||
			g_aPlayer[nCntPlayer].move.x < -PLAYER_MOTION_MOVE_ZONE ||
			g_aPlayer[nCntPlayer].move.z >  PLAYER_MOTION_MOVE_ZONE ||
			g_aPlayer[nCntPlayer].move.z < -PLAYER_MOTION_MOVE_ZONE)
		{// �v���C���[�������Ă���Ƃ�
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_MOVE;
		}
		else
		{// �v���C���[�������ĂȂ��Ƃ�
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_NEUTRAL;
		}
	}
	//--------------------------- �U�����[�V���� -----------------------------------------------------//
	if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_0] == true)
	{// �U���O���[�V�����ݒ�
		g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_ATTACK_0;
	}
	if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_1] == true)
	{// �U���P���[�V�����ݒ�
		g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_ATTACK_1;
	}
	if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_2] == true)
	{// �U���Q���[�V�����ݒ�
		g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_ATTACK_2;
	}
	if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_BIG] == true)
	{// ���U�����[�V�����ݒ�
		g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_ATTACK_BIG;
	}
	// ���[�V����
	MotionInfoPlayer(nCntPlayer);
	switch (g_aPlayer[nCntPlayer].nMotionType)
	{// �v���C���[�̃��[�V������
	case PLAYER_MOTION_TYPE_JUMP:						// �W�����v���[�V������������
		break;
	case PLAYER_MOTION_TYPE_LANDING:					// ���n���[�V������������
		if (g_nFrameAllCounter == 1)
		{
			PlaySound(SOUND_LABEL_SE_RANDING);
		}
		break;
	case PLAYER_MOTION_TYPE_DAMAGE:						// �_���[�W(������)���[�V������������
		break;
	case PLAYER_MOTION_TYPE_FLY:						// �_���[�W(�������)���[�V������������
		break;
	case PLAYER_MOTION_TYPE_GETUP:						// �N���オ�胂�[�V������������
		break;
	case PLAYER_MOTION_TYPE_STEP:						// �X�e�b�v���[�V������������
		break;
	case PLAYER_MOTION_TYPE_ATTACK_0:						// �U�����[�V������������
		if (g_nFrameAllCounter >= 0 && g_nFrameAllCounter <= 5)
		{// ���[�V�����̍ŏ��̃t���[���̎�
			g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * g_aPlayer[nCntPlayer].MoveSpeed * 2);
			g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * g_aPlayer[nCntPlayer].MoveSpeed * 2);
		}
		if (g_nFrameAllCounter == 18)
		{
			PlaySound(SOUND_LABEL_SE_PUNCH);
		}
		break;
	case PLAYER_MOTION_TYPE_ATTACK_1:				// �U���P
		if (g_nFrameAllCounter >= 0 && g_nFrameAllCounter <= 5)
		{// ���[�V�����̍ŏ��̃t���[���̎�
			g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * g_aPlayer[nCntPlayer].MoveSpeed * 2);
			g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * g_aPlayer[nCntPlayer].MoveSpeed * 2);
		}
		if (g_nFrameAllCounter == 13)
		{
			PlaySound(SOUND_LABEL_SE_PUNCH_PUNCH);
		}
		break;
	case PLAYER_MOTION_TYPE_ATTACK_2:			// �U���Q
		if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_HAND)
		{
			if (g_nFrameAllCounter >= 25 && g_nFrameAllCounter <= 33)
			{// ���[�V�����̍ŏ��̃t���[���̎�
				g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
				g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
			}
			if (g_nFrameAllCounter == 22)
			{//�t���[����0�̎�
				PlaySound(SOUND_LABEL_SE_PUNCH_PUNCH_PUNCH);
				PlaySound(SOUND_LABEL_VOICE_PUNCH_PUNCH_PUNCH);
			}
		}
		else if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_SORD)
		{
			if (g_nFrameAllCounter >= 0 && g_nFrameAllCounter <= 10)
			{// ���[�V�����̍ŏ��̃t���[���̎�
				g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
				g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
			}
			if (g_nFrameAllCounter == 1)
			{//�t���[����0�̎�
				PlaySound(SOUND_LABEL_SE_PUNCH_PUNCH_PUNCH);
				PlaySound(SOUND_LABEL_VOICE_PUNCH_PUNCH_PUNCH);
			}
		}
		else if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_LANCE)
		{
			if (g_nFrameAllCounter >= 0 && g_nFrameAllCounter <= 15)
			{// ���[�V�����̍ŏ��̃t���[���̎�
				g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
				g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
			}
			if (g_nFrameAllCounter == 1)
			{//�t���[����0�̎�
				PlaySound(SOUND_LABEL_SE_PUNCH_PUNCH_PUNCH);
				PlaySound(SOUND_LABEL_VOICE_PUNCH_PUNCH_PUNCH);
			}
		}
		else if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_HAMMER)
		{
			if (g_nFrameAllCounter >= 0 && g_nFrameAllCounter <= 5)
			{// ���[�V�����̍ŏ��̃t���[���̎�
				g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
				g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
			}
			if (g_nFrameAllCounter == 1)
			{//�t���[����0�̎�
				PlaySound(SOUND_LABEL_SE_PUNCH_PUNCH_PUNCH);
				PlaySound(SOUND_LABEL_VOICE_PUNCH_PUNCH_PUNCH);
			}
		}
		break;
	case PLAYER_MOTION_TYPE_ATTACK_BIG:			// ���U��
		if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_HAND)
		{
			if (g_nFrameAllCounter == 15)
			{
				SetEffect3D(g_aPlayer[nCntPlayer].pos + D3DXVECTOR3(sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 150, -200.0f, cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 150), D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECT3D_SORD);
				SetEffect3D(g_aPlayer[nCntPlayer].pos + D3DXVECTOR3(sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI / 4 * 3) * 150, -200.0f, cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI / 4 * 3) * 150), D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECT3D_SORD);
				SetEffect3D(g_aPlayer[nCntPlayer].pos + D3DXVECTOR3(sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI - D3DX_PI / 4) * 150, -200.0f, cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI - D3DX_PI / 4) * 150), D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECT3D_SORD);
			}
			if (g_nFrameAllCounter >= 15 && g_nFrameAllCounter <= 18)
			{
				g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * g_aPlayer[nCntPlayer].MoveSpeed * 3);
				g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * g_aPlayer[nCntPlayer].MoveSpeed * 3);
			}
			if (g_nFrameAllCounter == 20)
			{// ���[�V�����̍ŏ��̃t���[���̎�
				g_aPlayer[nCntPlayer].move.y = 15;
			}
			g_aPlayer[nCntPlayer].bJump = true;
			if (g_nFrameAllCounter == 22)
			{
				PlaySound(SOUND_LABEL_SE_PUNCH_SPIN_JUMP);
				PlaySound(SOUND_LABEL_VOICE_JUMP_PUNCH);
			}
		}
		else if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_SORD)
		{
			if (g_nFrameAllCounter >= 0 && g_nFrameAllCounter <= 10)
			{// ���[�V�����̍ŏ��̃t���[���̎�
				g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 6);
				g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 6);
			}
			if (g_nFrameAllCounter == 1)
			{//�t���[����0�̎�
				PlaySound(SOUND_LABEL_SE_PUNCH_SPIN_JUMP);
				PlaySound(SOUND_LABEL_VOICE_JUMP_PUNCH);
			}
		}
		else if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_LANCE)
		{
			if (g_nFrameAllCounter >= 10 && g_nFrameAllCounter <= 40)
			{// ���[�V�����̍ŏ��̃t���[���̎�
				g_aPlayer[nCntPlayer].move.y = 10;
			}
			if (g_nFrameAllCounter >= 35 && g_nFrameAllCounter <= 50)
			{
				g_aPlayer[nCntPlayer].move.y -= 5;
			}
			if (g_nFrameAllCounter == 35)
			{
				PlaySound(SOUND_LABEL_VOICE_JUMP_PUNCH);
				PlaySound(SOUND_LABEL_SE_JUMP_PUNCH);
			}
			if (g_nFrameAllCounter >= 44 )
			{
				SetEffectUP(g_aPlayer[nCntPlayer].pos + D3DXVECTOR3(0.0f, 0.1f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.1f, 1.0f, 0.1f, 1.0f), EFFECTUPTYPE_MAHOUJIN, 500);
			}
		}
		else if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_HAMMER)
		{
			if (g_nFrameAllCounter == 20)
			{
				SetEffect3D(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[17][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].aModel[17][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].aModel[17][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, g_aPlayer[nCntPlayer].rot.y, -D3DX_PI / 2), EFFECT3D_ARM);
			}
		}
		break;
	}
	//-------------------------------------/ �e �� �O�Ձ@�R���{�@���ړ����� /---------------------------------------------------------------------------------------//
	SetPositionShadow(g_aPlayer[nCntPlayer].nIdyShadow, D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43), g_aPlayer[nCntPlayer].Initpos);
	SetPositionLine(g_aPlayer[nCntPlayer].nIdyLine, g_aPlayer[nCntPlayer].pos, g_aPlayer[nCntPlayer].rot);
	SetPosOrbit(g_aPlayer[nCntPlayer].nMotionType,g_aPlayer[nCntPlayer].nWeaponState);
	PlayerSpirit(nCntPlayer);
}



//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawPlayer(int nCntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;				// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxParent;				// �e�̃}�g���b�N�X
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].mtxWorld);
	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nCntPlayer].rot.y, g_aPlayer[nCntPlayer].rot.x, g_aPlayer[nCntPlayer].rot.z);
	D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxRot);
	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y, g_aPlayer[nCntPlayer].pos.z);
	D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxTrans);
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].mtxWorld);
	for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
	{// ����̃}�g���b�N�X
		for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
		{// ����̏��
			if (g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].bUse == true)
			{// �U������
				// �O��̈ʒu��ɉh
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].mtxWorldOld = g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].mtxWorld;
				// �ʒu�𔽉f
				D3DXMatrixTranslation(&g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].mtxWorld, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].ofset.x, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].ofset.y, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].ofset.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].mtxWorld, &g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].mtxWorld, &g_aPlayer[nCntPlayer].aModel[g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].nIdxParent][nCntWeapon].mtxWorld);
			}
		}
		if (g_aPlayer[nCntPlayer].CollisionBody[nCollision].bUse == true)
		{// �_���[�W����
			// �O��̈ʒu��ɉh
			g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorldOld = g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorld;
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorld, g_aPlayer[nCntPlayer].CollisionBody[nCollision].ofset.x, g_aPlayer[nCntPlayer].CollisionBody[nCollision].ofset.y, g_aPlayer[nCntPlayer].CollisionBody[nCollision].ofset.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorld, &g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorld, &g_aPlayer[nCntPlayer].aModel[g_aPlayer[nCntPlayer].CollisionBody[nCollision].nIdxParent][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld);
		}
	}

		for (int nCntPlayerModel = 0; nCntPlayerModel < g_aPlayer[nCntPlayer].nNumParts; nCntPlayerModel++)
		{
			if (g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].nIdxModelPareant == -1)
			{// �v���C���[
				mtxParent = g_aPlayer[nCntPlayer].mtxWorld;
			}
			else
			{// �e�e
				mtxParent = g_aPlayer[nCntPlayer].aModel[g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].nIdxModelPareant][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld;
			}
			// ------------------------/ �e���f���̕`�� /-------------------------------------------//
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld);
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].rot.y, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].rot.x, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].rot.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &mtxRot);
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pos.x, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pos.y, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pos.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &mtxTrans);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &mtxParent);
			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);
			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pBuffMat->GetBufferPointer();

			for (int nCntPlayer2 = 0; nCntPlayer2 < (int)g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].nNumMat; nCntPlayer2++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntPlayer2].MatD3D);
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pTextuer[nCntPlayer2]);

				// ���f��(�p�[�c)�̕`��
				g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pMesh->DrawSubset(nCntPlayer2);
			}
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
			// ------------------------/ �q���f���̕`�� /-------------------------------------------//
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld);
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].rot.y, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].rot.x, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].rot.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &mtxRot);
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pos.x, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pos.y, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pos.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &mtxTrans);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &mtxParent);
			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);
			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pBuffMat->GetBufferPointer();
			for (int nCntPlayer2 = 0; nCntPlayer2 < (int)g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].nNumMat; nCntPlayer2++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntPlayer2].MatD3D);
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pTextuer[nCntPlayer2]);
				// ���f��(�p�[�c)�̕`��
				g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pMesh->DrawSubset(nCntPlayer2);
			}
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
}
//==============================================================================================================================
// ���f���̎擾
//==============================================================================================================================
Player *GetPlayer(void)
{
	return &g_aPlayer[0];
}
//==============================================================================================================================
// ���f���̃��[�V����
//==============================================================================================================================
void MotionInfoPlayer(int nCntPlayer)
{
	if (g_aPlayer[nCntPlayer].nMotionType != g_aPlayer[nCntPlayer].nMotionTypeOld)
	{// ���̃^�C�v�ƑO�̃^�C�v���������
		g_nKeyCounter = 0;						// �L�[�����Z�b�g
		g_nFrameCounter = 0;					// �t���[�������Z�b�g
		g_aPlayer[nCntPlayer].bBlend = true;	// �u�����h�ɂ���
		g_nFrame = BLEND_FRAME;					// �u�����h�̃t���[��
		g_nFrameAllCounter = 0;					// �S�Ẵt���[��
	}
	else if (g_aPlayer[nCntPlayer].bBlend == false )
	{// �ʏ펞�̃t���[��
		g_nFrame = g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].aKey[g_nKeyCounter].nFrame;
	}
	if (g_nFrameCounter == 0)
	{// ���̃L�[�t���[���̃��[�V������ǂݍ���
		for (int nCntParts = 0; nCntParts < g_aPlayer[nCntPlayer].nNumParts; nCntParts++)
		{// Key - ���� �Ԃ̍����v�Z
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].posAddMotion =
				(g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].aKey[g_nKeyCounter].aPartsKey[nCntParts].pos
					- g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].pos + g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].Initpos) / float(g_nFrame);

			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion = (g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].aKey[g_nKeyCounter].aPartsKey[nCntParts].rot - g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot);

			if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.x > D3DX_PI)
			{
				g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.x -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.x < -D3DX_PI)
			{
				g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.x += D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.y > D3DX_PI)
			{
				g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.y -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.y < -D3DX_PI)
			{
				g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.y += D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.z > D3DX_PI)
			{
				g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.z -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.z < -D3DX_PI)
			{
				g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.z += D3DX_PI * 2.0f;
			}
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion = g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion / float(g_nFrame);
		}
	}
	for (int nCntParts = 0; nCntParts < g_aPlayer[nCntPlayer].nNumParts; nCntParts++)
	{// ���[�V�����������Z	

		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.x > D3DX_PI)
		{// ������3.14�𒴂���Ȃ�
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.x -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.x < -D3DX_PI)
		{// ������-3.14�𒴂���Ȃ�
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.x += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.y > D3DX_PI)
		{// ������3.14�𒴂���Ȃ�
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.y -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.y < -D3DX_PI)
		{// ������-3.14�𒴂���Ȃ�
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.y += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.z > D3DX_PI)
		{// ������3.14�𒴂���Ȃ�
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.z -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.z < -D3DX_PI)
		{// ������-3.14�𒴂���Ȃ�
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.z += D3DX_PI * 2.0f;
		}
		g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].pos += g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].posAddMotion;
		g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot += g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion;

		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.x > D3DX_PI)
		{// ���݂̊p�x��3.14�𒴂���Ȃ�
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.x -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.x < -D3DX_PI)
		{// ���݂̊p�x��-3.14�𒴂���Ȃ�
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.x += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.y > D3DX_PI)
		{// ���݂̊p�x��3.14�𒴂���Ȃ�
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.y -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.y < -D3DX_PI)
		{// ���݂̊p�x��-3.14�𒴂���Ȃ�
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.y += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.z > D3DX_PI)
		{// ���݂̊p�x��3.14�𒴂���Ȃ�
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.z -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.z < -D3DX_PI)
		{// ���݂̊p�x��-3.14�𒴂���Ȃ�
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.z += D3DX_PI * 2.0f;
		}
	}
	g_nFrameCounter++;
	g_nFrameAllCounter++;
	if (g_aPlayer[nCntPlayer].bBlend == true && g_nFrameCounter == BLEND_FRAME)
	{// �u�����h�@�u�����h�̃t���[�����ɒB����
		g_aPlayer[nCntPlayer].bBlend = false;
		g_nFrameCounter = 0;
		g_nKeyCounter = 0;// �u�����h�Ŏ��̃��[�V������0�L�[�ڂ͍s���Ă�
		if (g_aPlayer[nCntPlayer].nMotionType == PLAYER_MOTION_TYPE_NEUTRAL)
		{
			g_nKeyCounter = 1;
		}
	}
	else if (g_aPlayer[nCntPlayer].bBlend == false&&g_nFrameCounter == g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].aKey[g_nKeyCounter].nFrame)
	{// �t���[���J�E���g������
		g_nFrameCounter = 0;
		if (g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].bLoop == false &&
			g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].nNumKey - 1 == g_nKeyCounter)
		{// ���[�v���Ȃ����L�[����������	
		//	g_nFrameAllCounter = 0;
			g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_LANDING] = false;	// ���n�L�����Z��
			
			switch (g_aPlayer[nCntPlayer].nMotionType)
			{// �v���C���[�̃��[�V������
	
			case PLAYER_MOTION_TYPE_JUMP:																// �W�����v���[�V������������
				break;
			case PLAYER_MOTION_TYPE_LANDING:															// ���n���[�V������������
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_LANDING] = false;										// ���n��Ԃ���Ȃ���Ԃɂ���
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// �U���̎�ޕ����[�v
					if (g_aPlayer[nCntPlayer].bAttackType[nCntAttack] == true)
					{// �U������������
						g_aPlayer[nCntPlayer].bAttackType[nCntAttack] = false;								// �U����Ԃ�����
					}
				}
				break;
			case PLAYER_MOTION_TYPE_DAMAGE:																// �_���[�W(������)���[�V������������
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_DAMAGE] = false;										// ���n��Ԃ���Ȃ���Ԃɂ���
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// �U���̎�ޕ����[�v
					if (g_aPlayer[nCntPlayer].bAttackType[nCntAttack] == true)
					{// �U������������
						g_aPlayer[nCntPlayer].bAttackType[nCntAttack] = false;								// �U����Ԃ�����
					}
				}
				break;
			case PLAYER_MOTION_TYPE_FLY:																// �_���[�W(�������)���[�V������������
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// �U���̎�ޕ����[�v
					if (g_aPlayer[nCntPlayer].bAttackType[nCntAttack] == true)
					{// �U������������
						g_aPlayer[nCntPlayer].bAttackType[nCntAttack] = false;								// �U����Ԃ�����
					}
				}
				break;
			case PLAYER_MOTION_TYPE_GETUP:																// �N���オ�胂�[�V������������
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_GETUP] = false;						// �N���オ��̏�Ԃ�����
				break;
			case PLAYER_MOTION_TYPE_STEP:																// �X�e�b�v���[�V������������
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_STEP] = false;						// �X�e�b�v�̏�Ԃ�����
				g_aPlayer[nCntPlayer].nAirCounter++;// �󒆍U������J�E���^�[
				break;
			case PLAYER_MOTION_TYPE_CHANGE:																// �X�e�b�v���[�V������������
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_CHANGE] = false;						// �X�e�b�v�̏�Ԃ�����
				break;

			case PLAYER_MOTION_TYPE_ATTACK_0:																// �U�����[�V������������
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// �U���̎�ޕ����[�v
					if (g_aPlayer[nCntPlayer].bAttackType[nCntAttack] == true)
					{// �U������������
						g_aPlayer[nCntPlayer].bAttackType[nCntAttack] = false;								// �U����Ԃ�����
					}
				}
				break;
			case PLAYER_MOTION_TYPE_ATTACK_1:																// �U�����[�V������������
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// �U���̎�ޕ����[�v
					if (g_aPlayer[nCntPlayer].bAttackType[nCntAttack] == true)
					{// �U������������
						g_aPlayer[nCntPlayer].bAttackType[nCntAttack] = false;								// �U����Ԃ�����
					}
				}
				break;
			case PLAYER_MOTION_TYPE_ATTACK_2:																// �U�����[�V������������
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// �U���̎�ޕ����[�v
					if (g_aPlayer[nCntPlayer].bAttackType[nCntAttack] == true)
					{// �U������������
						g_aPlayer[nCntPlayer].bAttackType[nCntAttack] = false;								// �U����Ԃ�����
					}
				}
				break;
			case PLAYER_MOTION_TYPE_ATTACK_BIG:																// �U�����[�V������������
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// �U���̎�ޕ����[�v
					if (g_aPlayer[nCntPlayer].bAttackType[nCntAttack] == true)
					{// �U������������
						g_aPlayer[nCntPlayer].bAttackType[nCntAttack] = false;								// �U����Ԃ�����
					}
				}
				break;
			}
		}
		else
		{// ���̃L�[�����l��
			g_nKeyCounter = (g_nKeyCounter + 1) % g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].nNumKey;
		}
	}

}
//==============================================================================================================================
// �v���C���[�̑̂̓����蔻��
//==============================================================================================================================
void CollisionPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 mtxWorld, float fRadius, int nPower, float fDamageSide, float fDamageVertical)
{
	if (g_aPlayer[0].state != PLAYERSTATE_DEATH)
	{
		if (g_aPlayer[0].bMotionState[PLAYER_BOOL_DAMAGE] == false &&
			g_aPlayer[0].bMotionState[PLAYER_BOOL_FLY] == false &&
			g_aPlayer[0].bMotionState[PLAYER_BOOL_GETUP] == false)
		{// �_���[�W������Ȃ����
			for (int nCntCollision = 0; nCntCollision < MAX_COLLISION; nCntCollision++)
			{// �̂̓����蔻�蕪���[�v
			 // ��̓����蔻��̍��W�̋������v�Z
				float PosX = (g_aPlayer[0].CollisionBody[nCntCollision].mtxWorld._41 - mtxWorld.x);		// (�v���C���[�̑̂̓����蔻��̍��W.x - �G�̍U���̓����蔻��̍��W.x)
				float PosY = (g_aPlayer[0].CollisionBody[nCntCollision].mtxWorld._42 - mtxWorld.y);		// (�v���C���[�̑̂̓����蔻��̍��W.y - �G�̍U���̓����蔻��̍��W.y)
				float PosZ = (g_aPlayer[0].CollisionBody[nCntCollision].mtxWorld._43 - mtxWorld.z);		// (�v���C���[�̑̂̓����蔻��̍��W.z - �G�̍U���̓����蔻��̍��W.z)

				float fPosLenght = (PosX * PosX) + (PosY * PosY) + (PosZ * PosZ);				// ���ꂼ���2�悵�Ă��ׂđ����ċ������o��

																								// ��̓����蔻��̔��a�̋������v�Z
				float fCollisionRadius = (g_aPlayer[0].CollisionBody[nCntCollision].fRadius + fRadius);		// �v���C���[�̑̂̓����蔻��̔��a+�G�̍U���̓����蔻��̔��a�𑫂�

				float fRadiusLenght = fCollisionRadius * fCollisionRadius;								// ��̔��a�𑫂����l��2�悵�āA���a�̋������v�Z���v�Z

				if (fRadiusLenght > fPosLenght &&
					-fRadiusLenght < fPosLenght)
				{// ���W�̋��������a�̋�������������������
					float fAttackAngle = atan2f(pos.x - g_aPlayer[0].CollisionBody[nCntCollision].mtxWorld._41,		// ���������p�x���v�Z
						pos.z - g_aPlayer[0].CollisionBody[nCntCollision].mtxWorld._43);
					if (GetMode() == MODE_GAME)
					{
						g_aPlayer[0].nLife -= nPower;															// ���C�t�����炷
					}
					GetPlayerLife(g_aPlayer[0].nLife);														// ���C�t�����炷
					PlaySound(SOUND_LABEL_VOICE_DAMAGE);

					if (g_aPlayer[0].nLife <= 0)
					{// ���C�t��0�ȉ��ɂȂ�����
						StopSound();				// �T�E���h�I��
						PlaySound(SOUND_LABEL_VOICE_DEATH);
						PlaySound(SOUND_LABEL_DEATH);

						g_aPlayer[0].state = PLAYERSTATE_DEATH;
						g_aPlayer[0].nCntState = 100;

						// ���������̐�����ї͂���
						g_aPlayer[0].fFlyPower.x = sinf(fAttackAngle - D3DX_PI) * fDamageSide;
						g_aPlayer[0].fFlyPower.z = cosf(fAttackAngle - D3DX_PI) * fDamageSide;

						// ���������̐�����ї͂���
						g_aPlayer[0].fFlyPower.y = fDamageVertical;
						g_aPlayer[0].nFlyCounter = 10;
						g_aPlayer[0].bMotionState[PLAYER_BOOL_FLY] = true;													// ������ђ��ɐݒ�
						g_aPlayer[0].Destrot.y = fAttackAngle - D3DX_PI;							// �L�����N�^�[�̌�����ύX
						g_aPlayer[0].rot.y = fAttackAngle - D3DX_PI;							// �L�����N�^�[�̌�����ύX

					}
					else
					{
						if (fDamageSide > 0.0f || fDamageVertical > 0.0f)
						{// ������ї͂�����Ƃ�

						 // ���������̐�����ї͂���
							g_aPlayer[0].fFlyPower.x = sinf(fAttackAngle - D3DX_PI) * fDamageSide;
							g_aPlayer[0].fFlyPower.z = cosf(fAttackAngle - D3DX_PI) * fDamageSide;

							// ���������̐�����ї͂���
							g_aPlayer[0].fFlyPower.y = fDamageVertical;
							g_aPlayer[0].nFlyCounter = 10;
							g_aPlayer[0].bMotionState[PLAYER_BOOL_FLY] = true;													// ������ђ��ɐݒ�
							g_aPlayer[0].Destrot.y = fAttackAngle - D3DX_PI;							// �L�����N�^�[�̌�����ύX
							g_aPlayer[0].rot.y = fAttackAngle - D3DX_PI;							// �L�����N�^�[�̌�����ύX
						}
						else
						{// ������ї͂��Ȃ��Ƃ�			
							g_aPlayer[0].bMotionState[PLAYER_BOOL_DAMAGE] = true;												// �_���[�W���ɐݒ�
							// ���ʉ��Đ�
							PlaySound(SOUND_LABEL_SE_HIT_1);		// �_���[�W���𗬂�
						}

					}
					for (int nCntEffect = 0; nCntEffect < 30; nCntEffect++)
					{// �Ή�
						float nDirection = SetRandomEffect();
						float nDirectio2 = SetRandomEffect();
						SetEffect(D3DXVECTOR3(g_aPlayer[0].aModel[9][g_aPlayer[0].nWeaponState].mtxWorld._41, g_aPlayer[0].aModel[9][g_aPlayer[0].nWeaponState].mtxWorld._42, g_aPlayer[0].aModel[9][g_aPlayer[0].nWeaponState].mtxWorld._43),
							D3DXVECTOR3(sinf(nDirection) * 5, cosf(nDirection) * 5, cosf(nDirectio2)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXCOLOR(1.0f, 0.3f, 0.0f, 0.8f), 20, 30, EFFECTTYPE_EXPLOSION);
						SetEffect(D3DXVECTOR3(g_aPlayer[0].aModel[9][g_aPlayer[0].nWeaponState].mtxWorld._41, g_aPlayer[0].aModel[9][g_aPlayer[0].nWeaponState].mtxWorld._42, g_aPlayer[0].aModel[9][g_aPlayer[0].nWeaponState].mtxWorld._43),
							D3DXVECTOR3(sinf(nDirectio2) * 5, cosf(nDirection), cosf(nDirectio2) * 5), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXCOLOR(0.8f, 0.8f, 0.0f, 0.8f), 20, 30, EFFECTTYPE_EXPLOSION);
					}
					break;
				}

			}
		}
	}
}
//==============================================================================================================================
// ����ݒ�(�U������j
//==============================================================================================================================
void SetAttackCollisionPlayer(int nIdxParent, D3DXVECTOR3 offset, float fRadius, int nCollisionStart, int nCollisionEnd, PlayerMotionType nMotionType,PLAYERWEAPON nWeaponState)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
		{
			if (g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].bUse == false)
			{
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].nIdxParent = nIdxParent;
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].ofset = offset;
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].fRadius = fRadius;
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].nCollisionStart = nCollisionStart;
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].nCollisionEnd = nCollisionEnd;
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].nMotionType = nMotionType;
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].bUse = true;
				// �ʒu�𔽉f
				D3DXMatrixTranslation(&g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].mtxWorld, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].ofset.x, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].ofset.y, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].ofset.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].aModel[g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].nIdxParent][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld);
				break;
			}
		}
	}
}
//==============================================================================================================================
// ����ݒ�i�_���[�W����j
//==============================================================================================================================
void SetBodyCollisionPlayer(int nIdxParent, D3DXVECTOR3 offset, float fRadius)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
		{
			if (g_aPlayer[nCntPlayer].CollisionBody[nCollision].bUse == false)
			{
				g_aPlayer[nCntPlayer].CollisionBody[nCollision].nIdxParent = nIdxParent;
				g_aPlayer[nCntPlayer].CollisionBody[nCollision].ofset = offset;
				g_aPlayer[nCntPlayer].CollisionBody[nCollision].fRadius = fRadius;
				g_aPlayer[nCntPlayer].CollisionBody[nCollision].bUse = true;
				// �ʒu�𔽉f
				D3DXMatrixTranslation(&g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorld, g_aPlayer[nCntPlayer].CollisionBody[nCollision].ofset.x, g_aPlayer[nCntPlayer].CollisionBody[nCollision].ofset.y, g_aPlayer[nCntPlayer].CollisionBody[nCollision].ofset.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorld, &g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorld, &g_aPlayer[nCntPlayer].aModel[g_aPlayer[nCntPlayer].CollisionBody[nCollision].nIdxParent][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld);
				break;
			}
		}
	}
}
//==============================================================================================================================
// �v���C���[�̑���
//==============================================================================================================================
void PlayerController(int nCntPlayer)
{
	DIJOYSTATE2 *pGamePad = GetgamePadStick();
	Camera *pCamera = GetCamera();
	g_aPlayer[nCntPlayer].CtrlMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (pGamePad->lX <= -1 || pGamePad->lX >= 1 || pGamePad->lY <= -1 || pGamePad->lY >= 1)
	{// �Q�[���p�b�h�̏���
		if (pGamePad->lX >= -WARKZONE && pGamePad->lX <= WARKZONE && pGamePad->lY >= -WARKZONE && pGamePad->lY <= WARKZONE)
		{
			float fGamePagStickAngle = atan2f(float(-pGamePad->lX), float(pGamePad->lY));
			g_aPlayer[nCntPlayer].CtrlMove.x = sinf(fGamePagStickAngle + D3DX_PI + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed / 2.0f;
			g_aPlayer[nCntPlayer].CtrlMove.z = cosf(fGamePagStickAngle + D3DX_PI + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed / 2.0f;
			g_aPlayer[nCntPlayer].Destrot.y = fGamePagStickAngle + pCamera->rot.y;	// �ړI�̊p�x
		}
	}
	if (pGamePad->lX < -WARKZONE || pGamePad->lX > WARKZONE || pGamePad->lY < -WARKZONE || pGamePad->lY> WARKZONE)
	{
		float fGamePagStickAngle = atan2f(float(-pGamePad->lX), float(pGamePad->lY));

		g_aPlayer[nCntPlayer].CtrlMove.x = sinf(fGamePagStickAngle + D3DX_PI + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
		g_aPlayer[nCntPlayer].CtrlMove.z = cosf(fGamePagStickAngle + D3DX_PI + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
		g_aPlayer[nCntPlayer].Destrot.y = fGamePagStickAngle + pCamera->rot.y;	// �ړI�̊p�x
	}
	if (GetKeyboardPress(DIK_W) == true)
	{// �O�ړ�
		if (GetKeyboardPress(DIK_D) == true)
		{// �E�΂ߑO�ړ�
			g_aPlayer[nCntPlayer].CtrlMove.x = sinf(D3DX_PI / 4 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].CtrlMove.z = cosf(D3DX_PI / 4 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].Destrot.y = -D3DX_PI / 4 * 3 + pCamera->rot.y;	// �ړI�̊p�x
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{// ���΂ߑO�ړ�
			g_aPlayer[nCntPlayer].CtrlMove.x = sinf(-D3DX_PI / 4 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].CtrlMove.z = cosf(-D3DX_PI / 4 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].Destrot.y = D3DX_PI / 4 * 3 + pCamera->rot.y;	// �ړI�̊p�x
		}
		else
		{// �O�ړ�
			g_aPlayer[nCntPlayer].CtrlMove.x = sinf(0.0f + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].CtrlMove.z = cosf(0.0f + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].Destrot.y = D3DX_PI + pCamera->rot.y;		// �ړI�̊p�x
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{// ���ړ�
		if (GetKeyboardPress(DIK_D) == true)
		{// �E�΂ߌ��ړ�
			g_aPlayer[nCntPlayer].CtrlMove.x = sinf(D3DX_PI / 4 * 3 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].CtrlMove.z = cosf(D3DX_PI / 4 * 3 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].Destrot.y = -D3DX_PI / 4 + pCamera->rot.y;	// �ړI�̊p�x
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{// ���΂ߌ��ړ�
			g_aPlayer[nCntPlayer].CtrlMove.x = sinf(-D3DX_PI / 4 * 3 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].CtrlMove.z = cosf(-D3DX_PI / 4 * 3 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].Destrot.y = D3DX_PI / 4 + pCamera->rot.y;	// �ړI�̊p�x
		}
		else
		{// ���ړ�
			g_aPlayer[nCntPlayer].CtrlMove.x = sinf(D3DX_PI + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].CtrlMove.z = cosf(D3DX_PI + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].Destrot.y = 0.0f + pCamera->rot.y;		// �ړI�̊p�x
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{// �E�ړ�
		g_aPlayer[nCntPlayer].CtrlMove.x = sinf(D3DX_PI / 2 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
		g_aPlayer[nCntPlayer].CtrlMove.z = cosf(D3DX_PI / 2 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
		g_aPlayer[nCntPlayer].Destrot.y = -D3DX_PI / 2 + pCamera->rot.y;	// �ړI�̊p�x
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{// ���ړ�
		g_aPlayer[nCntPlayer].CtrlMove.x = sinf(-D3DX_PI / 2 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
		g_aPlayer[nCntPlayer].CtrlMove.z = cosf(-D3DX_PI / 2 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
		g_aPlayer[nCntPlayer].Destrot.y = D3DX_PI / 2 + pCamera->rot.y;	// �ړI�̊p�x
	}
	if (g_aPlayer[nCntPlayer].nMotionType == PLAYER_MOTION_TYPE_NEUTRAL ||
		g_aPlayer[nCntPlayer].nMotionType == PLAYER_MOTION_TYPE_MOVE ||
		g_aPlayer[nCntPlayer].nMotionType == PLAYER_MOTION_TYPE_JUMP )
	{// ����̃��[�V���������ړ��ʉ��Z
		g_aPlayer[nCntPlayer].move += g_aPlayer[nCntPlayer].CtrlMove;
	}

}
//==============================================================================================================================
// �v���C���[�̃X�s���b�g�̏���
//==============================================================================================================================
void PlayerSpirit(int nCntPlayer)
{
	if (g_aPlayer[nCntPlayer].nWeaponState != PLAYERWEAPON_HAND)
	{
		if (g_aPlayer[nCntPlayer].state != PLAYERSTATE_DEATH)
		{
			// �X�s���b�g�񕜃J�E���^�[�����Z�b�g
			g_nSpiritRecoveryCounter = 0;
			// �X�s���b�g�����J�E���^�[��i�߂�
			g_nSpiritCounter++;
			if (g_nSpiritCounter % PLAYER_SPIRIT_TIME == 0)
			{
				if (g_aPlayer[nCntPlayer].nSpirit > 0)
				{ // �X�s���b�g���c���Ă�����X�s���b�g�����炷
					g_aPlayer[nCntPlayer].nSpirit--;
					GetPlayerSpirit(g_aPlayer[nCntPlayer].nSpirit);		// �X�s���b�g�𔽉f
				}
				else
				{// �c���Ă��Ȃ�������
					g_aPlayer[0].nLife -= PLAYER_SPIRIT_DAMAGE;		// ���C�t�����炷
					GetPlayerLife(g_aPlayer[0].nLife);				// ���C�t�����炷
					PlaySound(SOUND_LABEL_VOICE_DAMAGE);
					if (g_aPlayer[0].nLife <= 0)
					{// ���C�t��0�ȉ��ɂȂ�����
						StopSound();				// �T�E���h�I��
						PlaySound(SOUND_LABEL_VOICE_DEATH);
						PlaySound(SOUND_LABEL_DEATH);

						g_aPlayer[0].state = PLAYERSTATE_DEATH;
						g_aPlayer[0].nCntState = 100;

						g_aPlayer[0].bMotionState[PLAYER_BOOL_FLY] = true;													// ������ђ��ɐݒ�

					}
				}
			}
		}
	}
	else
	{
		// �X�s���b�g�����J�E���^�[�����Z�b�g
		g_nSpiritCounter = 0;
		// �X�s���b�g�񕜃J�E���^�[��i�߂�
		g_nSpiritRecoveryCounter++;
		if (g_nSpiritRecoveryCounter % PLAYER_SPIRIT_RECOVERY_TIME == 0)
		{
			if (g_aPlayer[nCntPlayer].nSpirit < g_aPlayer[nCntPlayer].nInitSpirit)
			{ // �X�s���b�g���c���Ă�����X�s���b�g�����炷
				g_aPlayer[nCntPlayer].nSpirit++;
				GetPlayerSpirit(g_aPlayer[nCntPlayer].nSpirit);		// �X�s���b�g�𔽉f
			}
		}
	}
}

//==============================================================================================================================
// �v���C���[�̃X�s���b�g�񕜏���
//==============================================================================================================================
void PlayerSpiritAdd(int nSpirit)
{
	g_aPlayer[0].nSpirit += nSpirit;
	if (g_aPlayer[0].nSpirit > g_aPlayer[0].nInitSpirit)
	{ // �����l�𒴂��Ă����ꍇ�����l�ɖ߂�
		g_aPlayer[0].nSpirit = g_aPlayer[0].nInitSpirit;
	}
	GetPlayerSpirit(g_aPlayer[0].nSpirit);		// �X�s���b�g�𔽉f
}
//==============================================================================================================================
// �v���C���[�̃G�t�F�N�g
//==============================================================================================================================
void PlayerMotionEffect(int nCntPlayer, int nCollision,PLAYERWEAPON nWeaponState)
{
	
	//SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].mtxWorld._43), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].fRadius, 10, EFFECTTYPE_EXPLOSION);
	
}
