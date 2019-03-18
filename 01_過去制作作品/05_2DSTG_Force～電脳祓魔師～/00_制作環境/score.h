//=============================================================================
//
// �X�R�A����[2D �|���S��] [score.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCORE_NUMBER (8)

class CScore : public CScene
{
public:
	typedef enum
	{
		SCORETYPE_NONE =0,	//��̏��
		SCORETYPE_GAME = 0,	//�Q�[����
		SCORETYPE_RESULT,	//���U���g��
	}SCORETYPE;
	CScore();	//�R���X�g���N�^
	~CScore();	//�f�X�g���N�^
	static CScore *Create(D3DXVECTOR3 pos, SCORETYPE type);	//�����̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetScore(int nScore);	//�X�R�A�̐ݒ�
	static int GetScore(void);	//�X�R�A�̎擾
	static void AddScore(int nValue);	//�X�R�A�̉��Z
	static void SetScoreNum(int nValue);	//�X�R�A�̉��Z
private:
	static CNumber *m_apNumber[SCORE_NUMBER];	//����
	static int m_nScore;	//�X�R�A
	SCORETYPE m_type;	//�^�C�v
};

#endif