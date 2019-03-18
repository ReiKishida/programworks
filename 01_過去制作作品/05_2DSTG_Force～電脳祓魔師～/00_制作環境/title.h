//=============================================================================
//
// �^�C�g������[2D �|���S��] [title.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
class CTitleLogo;

class CTitle
{
public:
	CTitle();	//�R���X�g���N�^
	~CTitle();	//�f�X�g���N�^
	static HRESULT Init(void);	//����������
	static void Uninit(void);	//�I������
	static void Update(void);	//�X�V����
	static void Draw(void);	//�`�揈��
private:
	static CManager *m_pManager;//�}�l�[�W��
	static CScene *m_pScene;//�V�[��
	static CTitle *m_pTitle;//�^�C�g��
	static CBg *m_pBg;//�w�i
	static CTitleLogo *m_pTitleLogo;//�^�C�g�����S
	static int m_nCntFlame;

};

#endif