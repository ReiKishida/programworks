//=============================================================================
//
// ���C������ [main.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _WINSOCK_DEPRECATED_NO_WARNINGS

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include <WinSock2.h>
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)	// �x���Ώ��p
#include "dinput.h"
#include "xaudio2.h"					// �T�E���h�����ŕK�v

//*****************************************************************************
// ���C�u�����̃����N
// [�\���v���p�e�B]->[�����J�[]->[����]->[�ǉ��̈ˑ��t�@�C��]�ɋL�q���Ă��\
//*****************************************************************************
#pragma comment(lib, "ws2_32.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")		// �V�X�e�������擾�p�ɕK�v
#pragma comment (lib, "dinput8.lib")

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH	(1280)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)				// �E�C���h�E�̍���
#define MAX_PLAYER		(4)					// �v���C���[�̍ő吔

// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���_�J���[ / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@���x�N�g�� / ���_�J���[  / �e�N�X�`�����W)
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE|D3DFVF_TEX1)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
int GetCntFPS(void);	// �J�E���^�[�擾

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float rhw;		// 1.0f�ŌŒ�
	D3DCOLOR col;	// ���_�J���[
	D3DXVECTOR2 tex;// �e�N�X�`�����W
}VERTEX_2D;
// �R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
} VERTEX_3D;

#endif