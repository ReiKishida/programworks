//=============================================================================
//
// ���U���g����[2D �|���S��] [result.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
class CResult
{
public:
	CResult();	//�R���X�g���N�^
	~CResult();	//�f�X�g���N�^
	//static CResult *Create();	//���U���g�̐���
	static HRESULT Init(void);	//����������
	static void Uninit(void);	//�I������
	static void Update(void);	//�X�V����
	static void Draw(void);	//�`�揈��
private:
	static CManager *m_pManager;//�}�l�[�W��

	static CResult *m_pResult;

};

#endif