//=============================================================================
//
// �J�������� [camera.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"
#include "inputx.h"
#include "reticle.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ROT_SPEED (0.25f)//�J�����̐��񑬓x(�㉺)
#define ROT_SPEED_Y (0.3f)	//�J�����̐��񑬓x(���E)



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAMERA g_Camera;	//�J�����̏��
JoyState g_JoyStateCamera;//�Q�[���p�b�h�̏�(�J����)
float fTrueZoom;//�ڕW�̃Y�[���l
float fZoom;//���̃Y�[���l
float fDestZoom;//�Y�[���l�̍���

//=============================================================================
// �J�����̏���������
//=============================================================================
void InitCamera(void)
{
	g_JoyStateCamera.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	//�v���C���[�̎擾
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//���[�h�̎擾
	MODE *pMode;
	pMode = GetMode();

	//�v���C���[�Ǝ��_�̋���
	g_Camera.fLength = pPlayer->pos.z - g_Camera.posV.z;
	g_Camera.fRLength = 10.0f;
	g_Camera.fAngle = 0.0f;

	//�p�x
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//�p�x�̍���(���_)
	g_Camera.posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0);

	//�p�x�̍���(�����_)
	g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0);

	g_Camera.posV = D3DXVECTOR3(pPlayer->pos.x,
		pPlayer->pos.y/* + 50.0f*/,
		pPlayer->pos.z/* - 100.0f*/);

	g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0);

	g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0);

	g_Camera.fPosR = -5.0f;

	fZoom = 0.0f;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	JoyState pJoyState = GetJoystate();

	XINPUT_STATE state;
	XInputGetState(0, &state);

	//�v���C���[�̏��擾
	PLAYER *pPlayer;
	pPlayer = GetPlayer();


	//===========================��荞��===================================//
	g_Camera.fDestAngle.y = D3DX_PI + pPlayer->rot.y - g_Camera.rot.y;
	if (g_Camera.fDestAngle.y > D3DX_PI)
	{
		g_Camera.fDestAngle.y -= D3DX_PI * 2;
	}
	if (g_Camera.fDestAngle.y < -D3DX_PI)
	{
		g_Camera.fDestAngle.y += D3DX_PI * 2;
	}
	g_Camera.rot.y += g_Camera.fDestAngle.y * ROT_SPEED;

	RETICLE pReticle;
	pReticle = GetReticle();
	if (GetKeyboardTrigger(DIK_UP) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP && pJoyState.bConnectionJoypad == true))
	{//�\���L�[�̏オ�����ꂽ�Ƃ�
		if (g_JoyStateCamera.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			fDestZoom = -30.0f;
			pReticle.fHeight = 600.0f;
			pReticle.fWidth = 600.0f;


		}
		g_JoyStateCamera.nJoypadState = INPUT_JOYSTATE_PUSH;

	}

	if (GetKeyboardTrigger(DIK_DOWN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN && pJoyState.bConnectionJoypad == true))
	{//�\���L�[�̉��������ꂽ�Ƃ�
		if (g_JoyStateCamera.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			fDestZoom = 0;
			pReticle.fHeight = 150.0f;
			pReticle.fWidth = 150.0f;

		}
		g_JoyStateCamera.nJoypadState = INPUT_JOYSTATE_PUSH;

	}

	fTrueZoom = fDestZoom - fZoom;
	fZoom += fTrueZoom *0.05f;
 	if (GetKeyboardPress(DIK_T) == true || (state.Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
	{//�s�L�[�������ꂽ�Ƃ��A�E�X�e�B�b�N����ɓ|������
		g_Camera.fPosR -= 2.0f;
	}

	else if (GetKeyboardPress(DIK_G) == true || (state.Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
	{//G�L�[�������ꂽ�Ƃ��A�E�X�e�B�b�N�����ɓ|������
		g_Camera.fPosR += 2.0f;
	}

	//============================================================================//


	//===========================������Ɛ�Ǐ]�i�O�j===================================//
	g_Camera.posRDest.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * -2.0f;
	g_Camera.posRDest.y = pPlayer->pos.y + 50.0f;
	g_Camera.posRDest.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * -2.0f;


	g_Camera.posVDest.x = pPlayer->pos.x - sinf(g_Camera.rot.y) * -170.0f;
	g_Camera.posVDest.y = pPlayer->pos.y + 70.0f + g_Camera.fPosR;
	g_Camera.posVDest.z = pPlayer->pos.z - cosf(g_Camera.rot.y) * -170.0f;

	g_Camera.posR.x += (g_Camera.posRDest.x - g_Camera.posR.x) * ROT_SPEED;
	g_Camera.posR.y += (g_Camera.posRDest.y - g_Camera.posR.y) * ROT_SPEED_Y;
	g_Camera.posR.z += (g_Camera.posRDest.z - g_Camera.posR.z) * ROT_SPEED;

	g_Camera.posV.x += (g_Camera.posVDest.x - g_Camera.posV.x) * ROT_SPEED;
	g_Camera.posV.y += (g_Camera.posVDest.y - g_Camera.posV.y) * ROT_SPEED_Y;
	g_Camera.posV.z += (g_Camera.posVDest.z - g_Camera.posV.z) * ROT_SPEED;
	//===========================================================================//



	OverView();//������������

	if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB && pJoyState.bConnectionJoypad == true))
	{
		if (g_JoyStateCamera.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			g_Camera.fPosR = -6.0f;
		}
		g_JoyStateCamera.nJoypadState = INPUT_JOYSTATE_PUSH;

	}

	if (GetKeyboardPress(DIK_H) == true)
	{//H�L�[�������ꂽ�Ƃ�
		g_Camera.fPosR = 0;

	}

	if (g_JoyStateCamera.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateCamera.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateCamera.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateCamera.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}

}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

												// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	/*D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection,
	D3DXToRadian(45.0f),
	(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
	10.0f,
	1000.0f);*/
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection,
		D3DXToRadian(45.0f + fZoom),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		2000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera.mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&g_Camera.mtxView,//�r���[�}�g���b�N�X
		&g_Camera.posV,//���_
		&g_Camera.posR,//�����_
		&g_Camera.vecU);//������x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);
}

//=============================================================================
// �J�����̒���
//=============================================================================
void OverView()
{

	if (g_Camera.fPosR > 70)
	{//��Ɍ����������Ƃ�

		g_Camera.fPosR = 70;
	}

	if (g_Camera.fPosR < -70)
	{//���Ɍ����������Ƃ�

		g_Camera.fPosR = -70;
	}


	//�����������Ƃ�//
	if (g_Camera.fAngle > D3DX_PI)
	{
		g_Camera.fAngle -= D3DX_PI * 2;
	}

	if (g_Camera.fAngle < -D3DX_PI)
	{
		g_Camera.fAngle += D3DX_PI * 2;
	}
}



//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA *GetCamera(void)
{
	return &g_Camera;
}



