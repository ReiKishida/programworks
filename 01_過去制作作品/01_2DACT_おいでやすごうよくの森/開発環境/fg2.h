//=============================================================================
//
// �O�i�Q���� [fg2.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _FG2_H_
#define _FG2_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_FG2	(1024)	// �G�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	//ITEMTYPE_NONE = 0,
	FG2TYPE_GLASS = 0,		//��
	FG2TYPE_MAX		//��ނ̑���
}FG2TYPE;


typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posold; //�ړ��O�ʒu
	D3DXVECTOR3 move;   //�ړ���
	D3DXCOLOR col;		//�J���[
	int nType;			//���
	FG2TYPE FgType;	//���
	float fWidth;		//��
	float fHeight;		//����
	bool bUse;			//�g�p���Ă��邩�ǂ���
}FG2;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitFg2(void);
void UnInitFg2(void);
void UpdateFg2(void);
void DrawFg2(void);


 //InitFg2();
 //UnInitFg2();
 //UpdateFg2();
 //DrawFg2();


void SetFg2(D3DXVECTOR3 pos, int type, float fHeight, float fWidth);

//void DeleteItem(int nCntItem);

//ITEM *GetItem(void);

//bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float *fWidth, float *fHeight);
#endif
