////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���f������ [model.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_MODEL		(350)		// �ő吔
#define MAX_MODEL_TYPE	(40)		// ���f���̃^�C�v�̍ő吔
#define MAX_MODEL_COLLISION	(50)		// ���f���̔����

//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef enum
{
	MODELTYPE_BOOK = 0,
	MODELTYPE_BOOKSHELF,
	MODELTYPE_CANDLE,
	MODELTYPE_CHAIR,
	MODELTYPE_HANDRAIL,
	MODELTYPE_LARGEDOOR1,
	MODELTYPE_LARGEDOOR2,
	MODELTYPE_PLANT,
	MODELTYPE_SMALLDOOR,
	MODELTYPE_STONE,
	MODELTYPE_TABLE,
	MODELTYPE_WINDOW,
	MODELTYPE_CAP,
	MODELTYPE_GAISOU,
	MODELTYPE_GAITOU,
	MODELTYPE_HASHIRA1,
	MODELTYPE_HASHIRA2,
	MODELTYPE_HUNSUI,
	MODELTYPE_HURUDOKEI,
	MODELTYPE_KAIGA,
	MODELTYPE_KANSETUSYOUMEI,
	MODELTYPE_DISH,
	MODELTYPE_SHANDERIA,
	MODELTYPE_TUKUE,
	MODELTYPE_TUBO,
	MODELTYPE_TREE0,
	MODELTYPE_TREE1,
	MODELTYPE_SMALLDOOR_YOKO,
	MODELTYPE_YOKANDOOR1,
	MODELTYPE_YOKANDOOR2,
	MODELTYPE_ARMOR_OBJ,
	MODELTYPE_BOOK_V,
	MODELTYPE_HASIRA3,
	MODELTYPE_SHANDERIA_BIG,
	MODELTYPE_HANDRAIL_H,
	MODELTYPE_BOSSSAKU,
	MODELTYPE_MAX,
}MODELTYPE;

typedef struct
{
	LPD3DXMESH pMesh;							// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;						// �}�e���A�����ւ̃|�C���^
	DWORD nNumMat;								// �}�e���A�����̐�
	LPDIRECT3DTEXTURE9 *pTextuer;			// �e�N�X�`��
	D3DXVECTOR3 CollisionPos[MAX_MODEL_COLLISION];
	float fWidthCollision[MAX_MODEL_COLLISION];
	float fDepthCollision[MAX_MODEL_COLLISION];
	float fHeightCollision[MAX_MODEL_COLLISION];
	float fRadiusCollision[MAX_MODEL_COLLISION];
	bool bCollison[MAX_MODEL_COLLISION];
	char *pFileName;
	int nNumModel;
}ModelData;
typedef struct
{
	LPD3DXBUFFER pBuffMat;				// �}�e���A�����ւ̃|�C���^
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 posOld;					// �O��̈ʒu
	D3DXVECTOR3 Initpos;				// �����ʒu
	D3DXVECTOR3 move;					// �ړ���
	D3DXCOLOR col;						// �J���[
	MODELTYPE nType;					// ���
	D3DXVECTOR3 Diffrot;				// �p�x�̍���
	D3DXVECTOR3 Destrot;				// �ړI�̊p�x
	D3DXVECTOR3 rot;					// �p�x
	D3DXMATRIX mtxWorld;				// ���[���h�}�g���b�N�X
	int nIdyShadow;						// �e�̔ԍ�
	int nIdyLine[MAX_MODEL_COLLISION];							// ���̔ԍ�
	int nIdy;						// �ԍ�
	bool bCollision;					// �����蔻��
	bool bUse;
	bool bDraw;
}Model;

//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
int CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 rot, float fRadius);
int CollisionModelMinMax(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadiusWidth, float fRadiusHeight,Model **pModel);
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODELTYPE nType);	
void SetCollisionModel(D3DXVECTOR3 pos, float fRadiusCollision,float fWidthCollision, float fDepthCollision,float fHeightCollision, MODELTYPE nType);
Model *GetModel(void);
ModelData *GetModelData(void);
#endif
