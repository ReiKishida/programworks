//=============================================================================
//
// �n�C�X�R�A����[2D �|���S��] [highscore.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _HIGHSCORE_H_
#define _HIGHSCORE_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define HIGHSCORE_NUMBER (8)

class CHighScore : public CScene
{
public:
	CHighScore();	//�R���X�g���N�^
	~CHighScore();	//�f�X�g���N�^
	static CHighScore *Create(D3DXVECTOR3 pos);	//�����̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetScore(int nScore);	//�X�R�A�̐ݒ�
	int GetScore(void);	//�X�R�A�̎擾
	static void AddScore(int nValue);	//�X�R�A�̉��Z
	static void SetHighScore(int nValue);//�X�R�A�̐ݒ�
private:
	static CNumber *m_apNumber[HIGHSCORE_NUMBER];	//����
	static int m_nHighScore;	//�X�R�A
};

#endif