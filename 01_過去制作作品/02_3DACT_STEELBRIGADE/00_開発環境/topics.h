//=============================================================================
//
// ��񏈗� [topics.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _TOPICS_H_
#define _TOPICS_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_TOPICS	(10)	// �G�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	TOPICSTYPE_NONE = 0,		//���[��(���r�B�f�B�b�N�̊Ǌ����A�]�X)
	TOPICSTYPE_MAIL_0 ,		//���[��(���r�B�f�B�b�N�̊Ǌ����A�]�X)
	TOPICSTYPE_MAIL_0_1,		//���[��(�˗����Ă܂��A�]�X)
	TOPICSTYPE_MAIL_1 ,		//���[��
	TOPICSTYPE_MAIL_1_1,		//�j���[�X
	TOPICSTYPE_NEWS_0,		//�j���[�X
	TOPICSTYPE_NEWS_0_1,		//�j���[�X
	TOPICSTYPE_MISSION_0,	//�~�b�V����01
	TOPICSTYPE_MISSION_1,//�~�b�V����02
	TOPICSTYPE_MAX		//��ނ̑���
}TOPICSTYPE;

typedef enum
{
	MAILSTATE_UNREAD = 0,	//���Ǐ��
	MAILSTATE_ALREADY,		//���Ǐ��
	MAILSTATE_MAX			//�ő吔
}TOPICSSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posold; //�ړ��O�ʒu
	D3DXVECTOR3 FallenPoint;
	D3DXVECTOR3 move;   //�ړ���
	TOPICSSTATE state;	//���
	TOPICSTYPE type;
	int nType;			//���
	float fWidth;		//��
	float fHeight;		//����
	float Move_X;	//�ړ��ʂ̃J�E���g(X)
	float Move_Y;	//�ړ��ʂ̃J�E���g(Y)
	
	bool bUse;			//�g�p���Ă��邩�ǂ���
	int nIndexPolygon;
}TOPICS;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitTopics(void);
void UnInitTopics(void);
void UpdateTopics(void);
void DrawTopics(void);

void DeleteTopics(int nIndexPolygon);

void SetTopics(D3DXVECTOR3 pos, TOPICSTYPE Type, int nType, float fHeight, float fWidth);

TOPICSTYPE CollisionTopics(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold);
TOPICS *GetTopics(void);


#endif
