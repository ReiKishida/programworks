//=============================================================================
//
// �X�R�A����[2D �|���S��] [score.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _DAMAGESCORE_H_
#define _DAMAGESCORE_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DAMAGE_SCORE_NUMBER (4)

class CNumber;
class CDamageScore : public CScene
{
public:

	CDamageScore(int nPriority = 7, OBJTYPE type = OBJTYPE_SCORE);	//�R���X�g���N�^
	~CDamageScore();	//�f�X�g���N�^
	static CDamageScore *Create(D3DXVECTOR3 pos);	//�����̐���
	HRESULT Init();	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetScore(int nScore);	//�X�R�A�̐ݒ�
	static int GetScore(void);	//�X�R�A�̎擾
	static void AddScoreFix();			//�ړI�̃X�R�A�։��Z
	static void SetScoreNum(int nValue);	//�X�R�A�̉��Z
private:
	static CNumber *m_apNumber[DAMAGE_SCORE_NUMBER];	//����
	static int m_nScore;		//�v�Z�p�X�R�A
	static int m_nDestScore;	//�ړI�̃X�R�A
	static int m_nNowScore;		//���݂̃X�R�A
	static D3DXVECTOR3 m_pos;	//�ʒu���
};

#endif