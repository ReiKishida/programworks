//=============================================================================
//
// �t�H�[�X������[2D �|���S��] [forcenumber.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _FORCENUMBER_H_
#define _FORCENUMBER_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FORCE_NUMBER (4)

class CForceNumber : public CScene
{
public:
	CForceNumber();	//�R���X�g���N�^
	~CForceNumber();	//�f�X�g���N�^
	static CForceNumber *Create(D3DXVECTOR3 pos);	//�����̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetForce(int nScore);	//�t�H�[�X���̐ݒ�
	int GetForce(void);	//�t�H�[�X���̎擾
	static void AddForce(int nValue);	//�t�H�[�X���̉��Z
private:
	static CNumber *m_apNumber[FORCE_NUMBER];	//����
	static int m_nForce;	//�t�H�[�X��
};

#endif