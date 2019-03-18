//=============================================================================
//
// �A�C�e���̏��� [item.h]
// Author : ��������
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"
#include "model.h"
#include "meshField.h"
//*****************************************************************************
// �A�C�e���̍\����
//*****************************************************************************
typedef enum
{// �A�C�e���̎��
	ITEM_KOUTOKU = 0,			// �K��
	MAX_ITEM
}ItemType;

typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 posOld;		// �O��̈ʒu
	D3DXVECTOR3 InitPos;	// �����ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXMATRIX	mtxWorld;	// ���[���h�}�g���N�X
	float fJampPower;		// �W�����v��
	float fPosLenght;		// �v���C���[�Ƃ̋���
	__int8 nPattanAnime;	// �A�j���[�V�����̃p�^�[��
	float fMaxPattanAnime;	// �A�j���[�V�����̃p�^�[���̍ő吔
	int nCounterAnime;		// �A�j���[�V�����̃J�E���^�[
	int nIdyShadow;			// �g���Ă���ǂ̔ԍ�
	float fWidth;			// ��
	float fHeight;			// ����
	bool bUse;				// �g�p���Ă��邩�ǂ���
	ItemType ItemType;		// �A�C�e���̎��
	Model				*pModel;									// ���f���̏��
	MeshField			*pMeshField;								// ���b�V���t�B�[���h���

}Item;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ItemType type);	// �A�C�e���̐ݒu
void CollisionItem(D3DXVECTOR3 mtxWorld, float fRadius);		// �A�C�e���̓����蔻��
void DeleteItem(int nCntItem, ItemType type);					// �A�C�e���̌���
#endif
