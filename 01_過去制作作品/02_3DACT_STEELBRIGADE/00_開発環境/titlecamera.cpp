//=============================================================================
//
// �J�������� [titlecamera.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "titlecamera.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MDD2 (1.0f)
#define ROT_CAMERASPEED (0.025f)
#define ROT_SPEED (0.2f)

#define CAMERA_ANGLE (12.0f)


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
TITLECAMERA g_TitleCamera;


//=============================================================================
// �J�����̏���������
//=============================================================================
void InitTitleCamera(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	MODE *pMode;
	pMode = GetMode();


		g_TitleCamera.posV = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
		g_TitleCamera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_TitleCamera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0);

		g_TitleCamera.fLength = 0.0f;
		g_TitleCamera.fAngle = 0.0f;

		g_TitleCamera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0);


}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitTitleCamera(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateTitleCamera(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();




	MODE *pMode;
	pMode = GetMode();



	OverTitleView();


}
//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetTitleCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_TitleCamera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	/*D3DXMatrixPerspectiveFovLH(&g_TitleCamera.mtxProjection,
	D3DXToRadian(45.0f),
	(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
	10.0f,
	1000.0f);*/
	D3DXMatrixPerspectiveFovLH(&g_TitleCamera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		1200.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_TitleCamera.mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_TitleCamera.mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&g_TitleCamera.mtxView,//�r���[�}�g���b�N�X
		&g_TitleCamera.posV,//���_
		&g_TitleCamera.posR,//�����_
		&g_TitleCamera.vecU);//������x�N�g��

						// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_TitleCamera.mtxView);
}

//=============================================================================
// �J�����̒���
//=============================================================================
void OverTitleView()
{
	if (g_TitleCamera.rot.y > D3DX_PI)
	{

		g_TitleCamera.rot.y -= D3DX_PI * 2;
	}

	if (g_TitleCamera.rot.y < -D3DX_PI)
	{

		g_TitleCamera.rot.y += D3DX_PI * 2;
	}

	if (g_TitleCamera.rot.x > D3DX_PI / 4)
	{

		g_TitleCamera.rot.x = D3DX_PI / 4;
	}

	if (g_TitleCamera.rot.x < -D3DX_PI / 4)
	{

		g_TitleCamera.rot.x = -D3DX_PI / 4;
	}




	if (g_TitleCamera.fAngle > D3DX_PI)
	{
		g_TitleCamera.fAngle -= D3DX_PI * 2;
	}

	if (g_TitleCamera.fAngle < -D3DX_PI)
	{
		g_TitleCamera.fAngle += D3DX_PI * 2;
	}
}



//=============================================================================
// �J�����̎擾
//=============================================================================
TITLECAMERA *GetCamera(void)
{
	return &g_TitleCamera;
}



