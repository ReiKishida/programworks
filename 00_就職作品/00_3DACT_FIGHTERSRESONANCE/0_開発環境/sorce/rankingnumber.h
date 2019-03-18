//=============================================================================
//
// �����L���O��������[2D �|���S��] [rankingnumber.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _RANKINGNUMBER_H_
#define _RANKINGNUMBER_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCORE_NUMBER (8)
#define MAX_RANKING (5)

class CRankingScore : public CScene
{
public:

	CRankingScore();	//�R���X�g���N�^
	~CRankingScore();	//�f�X�g���N�^
	static CRankingScore *Create(D3DXVECTOR3 pos);	//�����̐���
	HRESULT Init();	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static void SetRankingScore(void);	//�X�R�A�̉��Z
	static void SetRankingNum(int nValue);	//�X�R�A�̉��Z
private:
	static CNumber *m_apNumber[MAX_RANKING][SCORE_NUMBER];	//����
	static int m_nRanking[MAX_RANKING];	//�����L���O
	D3DXVECTOR3 m_pos;		//�ʒu���
	static int m_nRankingPlayer;//�v���C���[�̃X�R�A
	static int m_nCntRanking;
	static bool m_bPlayer;//�_�ŗp
};

#endif