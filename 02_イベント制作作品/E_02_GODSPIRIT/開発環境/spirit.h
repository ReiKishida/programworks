////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �X�s���b�g���� [spirit.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SPIRIT_H_
#define _SPIRIT_H_

#include "main.h"
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef enum
{
	SPIRITTEX_BG2 = 0,
	SPIRITTEX_GAGE,
	SPIRITTEX_BG,
	SPIRITTEX_MAX
}SPIRITTEX;

typedef struct
{
	LPDIRECT3DTEXTURE9	pTexture;	// �e�N�X�`���ւ̃|�C���^
	char	*pFileName;	// �e�N�X�`���ւ̃|�C���^
}SpiritData;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 rot;					// ����
	D3DXCOLOR col;						// �F
	D3DXMATRIX mtxWorld;				// ���[���h�}�g���b�N�X
	float fWidth;						// ��
	float fHeight;						// ����
	int nBlock_X;						// �u���b�NX
	int nBlock_Y;						// �u���b�NY
	int nType;							// ���
	int nNumVertex;						// ���_��
	int nNumIndex;						// �C���f�b�N�X��
	int nNumPolygon;					// �|���S����
	int nPatternAnim;					// �p�^�[��
	int nCounterAnim;					// �J�E���^�[
	bool bUse;							// �g�p����Ă��邩�ǂ���
	bool bUseDown;
	int nDrawDown;						// �����p
}Spirit;

//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitSpirit(void);
void UninitSpirit(void);
void UpdateSpirit(void);
void DrawSpirit(void);
void SetSpirit(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nType, int nBlock_X, int nBlock_Y, float fWidth, float fHeight, bool bUseDown);
Spirit *GetSpirit(void);
SpiritData *GetSpiritData(void);
void SetPlayerSpirit(int nSpirit);
void GetPlayerSpirit(int nSpirit);
#endif
