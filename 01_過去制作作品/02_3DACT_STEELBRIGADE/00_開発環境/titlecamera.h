//=============================================================================
//
// �J�������� [titlecamera.h]
// Author : 
//
//=============================================================================
#ifndef _TITLECAMERA_H_
#define _TITLECAMERA_H_

#include "main.h"

//*****************************************************************************
// �J�����̍\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 posV;		//���_
	D3DXVECTOR3 posVDest;		//�ړI�̎��_
	D3DXVECTOR3 posR;		//�����_  
	D3DXVECTOR3 posRDest;		//�ړI�̒����_  
	D3DXVECTOR3 vecU;		//������x�N�g��
	D3DXVECTOR3 rot;		//��]
	D3DXVECTOR3 rotDest;		//�ړI�̉�]
	D3DXMATRIX mtxProjection;//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X
	float fAngle;
	float fLength;
	float fRLength;
	D3DXVECTOR3 fDestAngle;
}TITLECAMERA;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitTitleCamera(void);
void UninitTitleCamera(void);
void UpdateTitleCamera(void);
void SetTitleCamera(void);

void OverTitleView();

TITLECAMERA *GetTitleCamera(void);
#endif
