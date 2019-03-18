//=============================================================================
//
// �c�@����[2D �|���S��] [remaining.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _REMAINING_H_
#define _REMAINING_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define REMAINING_NUMBER (2)

class CRemaining : public CScene
{
public:
	CRemaining();	//�R���X�g���N�^
	~CRemaining();	//�f�X�g���N�^
	static CRemaining *Create(D3DXVECTOR3 pos);	//�����̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetScore(int nScore);	//�X�R�A�̐ݒ�
	static int GetRemaining(void);	//�X�R�A�̎擾
	static void DecreseRemaining(int nValue);	//�X�R�A�̉��Z
	static HRESULT Load(void);//�e�N�X�`���̓ǂݍ���
	static void Unload(void);//�e�N�X�`���̔j��

private:
	static LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	static CScene2D *m_apRemaining[REMAINING_NUMBER];	//����
	static int m_nRemaining;	//�c�@��
};

#endif