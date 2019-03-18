//=============================================================================
//
// �X�R�A����[2D �|���S��] [score.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _FINALSCORE_H_
#define _FINALSCORE_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCORE_NUMBER (8)

class CNumber;
class CFinalScore : public CScene
{
public:
	CFinalScore(int nPriority = 7, OBJTYPE type = OBJTYPE_SCORE);	//�R���X�g���N�^
	~CFinalScore();	//�f�X�g���N�^
	static CFinalScore *Create(D3DXVECTOR3 pos);	//�����̐���
	HRESULT Init();	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetScore(int nScore);	//�X�R�A�̐ݒ�
	static int GetScore(void);	//�X�R�A�̎擾
	static void AddScore(int nValue);	//�X�R�A�̉��Z
	static void AddScoreFix();			//�ړI�̃X�R�A�։��Z
	static void SetScoreNum(int nValue);	//�X�R�A�̉��Z
private:
	static CNumber *m_apNumber[SCORE_NUMBER];	//����
	static int m_nScore;		//�v�Z�p�X�R�A
	static int m_nDestScore;	//�ړI�̃X�R�A
	static int m_nNowScore;		//���݂̃X�R�A
	static D3DXVECTOR3 m_pos;	//�ʒu���
};

#endif