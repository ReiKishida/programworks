//=============================================================================
//
// �w�i���� [bg.h]
// Author : �� �a��
//
//=============================================================================
#ifndef _BG_H_		// 2�d�C���N���[�h�h�~�̃}�N����`
#define _BG_H_

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	BG_TYPE_NORMAL = 0,	// �ʏ�w�i
	BG_TYPE_SCROLL,		// �X�N���[���w�i
	BG_TYPE_MAX			// ����
}BG_TYPE;

typedef enum
{
	BG_TEX_RESULT = 0,	// ��
	BG_TEX_RANKING,		// ��
	BG_TEX_MAX,			// ����
}BgTexName;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);
void SetBg(D3DXVECTOR3 pos, float fMove, D3DXCOLOR col, float fWidth, float fHeight, int nTexType, BG_TYPE type);

#endif
