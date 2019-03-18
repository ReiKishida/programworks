////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �{�^�����[�V�����\������ [buttonMotion.h]
// Author : �� �a��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _BUTTONMOTION_H_
#define _BUTTONMOTION_H_

#include "main.h"
#include "scene.h"
#include "scene3D.h"
//******************************************************************************************************************************
// 3D�|���S���̃N���X
//******************************************************************************************************************************
class CButtonMotion : public CScene3D
{
public:
	//+*++*++*++*++*++*++*++*++*++*++*
	// �\���̒�`
	//+*++*++*++*++*++*++*++*++*++*++*
	typedef enum
	{ // ���f�����
		TEX_TYPE_A = 0,	// A
		TEX_TYPE_B,		// B
		TEX_TYPE_X,		// X
		TEX_TYPE_Y,		// X
		TEX_TYPE_MAX	// �ő吔
	}TEX_TYPE;

	CButtonMotion(int nPriority = 2);
	~CButtonMotion();
	static HRESULT Load(void);	// �e�N�X�`������
	static void UnLoad(void);	// �e�N�X�`���j��

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CButtonMotion *Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, const TEX_TYPE texType);

private:
	TEX_TYPE	m_texType;
	int			m_nCounter;
	bool		m_bDisp;

	static LPDIRECT3DTEXTURE9	m_apTextureThis[TEX_TYPE_MAX];	// �e�N�X�`���ւ̃|�C���^
	static char					*m_aTexInfo[TEX_TYPE_MAX];	// �e�N�X�`�����O���
};
#endif
