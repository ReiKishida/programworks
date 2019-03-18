//=============================================================================
//
// �u���b�N���� [item.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _FG_H_
#define _FG_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_FG	(1024)	// �G�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	//ITEMTYPE_NONE = 0,
	FGTYPE_GLASS = 0,		//��
	FGYPE_MAX		//��ނ̑���
}FGTYPE;


typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posold; //�ړ��O�ʒu
	D3DXVECTOR3 move;   //�ړ���
	D3DXCOLOR col;		//�J���[
	int nType;			//���
	FGTYPE FgType;	//���
	float fWidth;		//��
	float fHeight;		//����
	
	bool bUse;			//�g�p���Ă��邩�ǂ���
}FG;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitFg(void);
void UnInitFg(void);
void UpdateFg(void);
void DrawFg(void);



void SetFg(D3DXVECTOR3 pos, int type, float fHeight, float fWidth);

//void DeleteItem(int nCntItem);

//ITEM *GetItem(void);

//bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float *fWidth, float *fHeight);
#endif
