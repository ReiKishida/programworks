//=============================================================================
//
// �e�L�X�g���� [text.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _ENEYTEXT_H_
#define _ENEYTEXT_H_
#define _CRT_SECURE_NO_WARNINGS

#include "main.h"
#include "stdio.h"
#include "conio.h"
#include "string.h"
//*****************************************************************************
// �\���̒�`
//*****************************************************************************



typedef struct
{
	char FileName[256];	//�t�@�C����
	float Index;
	float ModelPosX, ModelPosY, ModelPosZ;
	float ModelRotX, ModelRotY, ModelRotZ;
}EnemyTex;


typedef struct
{
	int MAX_FLAME;
	float MotionPosX, MotionPosY, MotionPosZ;
	float MotionRotX, MotionRotY, MotionRotZ;
}EnemyMotionTex;

typedef struct
{
	int MAX_TEXTPARTS;//�p�[�c�ő吔
	int MAX_MOTION;//���[�V�����̍ő吔
	int LOOP[25];	//���[�v���邩
	int KEYFLAME[25];//�L�[�t���[����
	int TOTALFRAME[25];//�t���[���̍��v��
	int MOVEPARTS[25];//���[�V�������ɓ����p�[�c��
	EnemyTex aModelTex[100];//�p�[�c
	EnemyMotionTex aMotionTex[25][50][50];//���[�V����
}EnemyText;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void LoadEnemyText(void);
EnemyText *GetEnemyText(void);

#endif

