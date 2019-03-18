//=============================================================================
//
// ���ˈʒu���� [shoot.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _SHOOT_H_
#define _SHOOT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_SHOOT	(128)	// ���ˈʒu�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	SHOOTTYPE_NORMAL = 0,	//�m�[�}��
	SHOOTTYPE_FEAVER,	//�t�B�[�o�[
	SHOOTTYPE_MAX
}SHOOTTYPPE;


typedef enum
{
	SHOOTDIRECTION_0 = 0,	//��
	SHOOTDIRECTION_1,	//�E
	SHOOTDIRECTION_MAX
}SHOOTDIRECTION;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posold; //�ړ��O�ʒu
	D3DXVECTOR3 rot;
	D3DXVECTOR3 FallenPoint;
	D3DXVECTOR3 move;   //�ړ���
	SHOOTTYPPE type;	//���
	SHOOTDIRECTION direction;
	float fWidth;		//��
	float fHeight;		//����
	float fLength;
	float fAngle;
	bool bUse;			//�g�p���Ă��邩�ǂ���
}SHOOT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitShoot(void);
void UnInitShoot(void);
void UpdateShoot(void);
void DrawShoot(void);


 

void SetShoot(D3DXVECTOR3 pos);
//void SetShoot(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType, ShootSTATE state);

SHOOT *GetShoot(void);

//bool CollisionShoot(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float *fWidth, float *fHeight);
#endif
