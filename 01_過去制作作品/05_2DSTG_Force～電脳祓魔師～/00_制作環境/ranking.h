//=============================================================================
//
// �����L���O����[2D �|���S��] [ranking.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

class CRanking
{
public:
	CRanking();	//�R���X�g���N�^
	~CRanking();	//�f�X�g���N�^
				//static CRanking *Create();	//���U���g�̐���
	static HRESULT Init(void);	//����������
	static void Uninit(void);	//�I������
	static void Update(void);	//�X�V����
	static void Draw(void);	//�`�揈��
private:
	static int m_nCntFlame;

	//static CManager *m_pManager;//�}�l�[�W��

	//static CRanking *m_pResult;

};

#endif