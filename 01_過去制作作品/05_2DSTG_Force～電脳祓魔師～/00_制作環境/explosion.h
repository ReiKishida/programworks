//=============================================================================
//
// ��������[2D �|���S��] [explosion.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

class CManager;

class CExplosion : public CScene2D
{
public:
	CExplosion::CExplosion();	//�R���X�g���N�^
	CExplosion::~CExplosion();	//�f�X�g���N�^
	static CExplosion *Create(D3DXVECTOR3 pos, float fRadius);	//�v���C���[�̐���
	HRESULT Init(D3DXVECTOR3 pos, float fRadius);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);//�e�N�X�`���̓ǂݍ���
	static void Unload(void);//�e�N�X�`���̔j��

private:
	static LPDIRECT3DTEXTURE9 m_pTexture; //���L�e�N�X�`���̃|�C���^
	int m_nCounterAnim;	//�A�j���[�V�����J�E���^�[
	int m_nPatternAnim;	//�A�j���[�V�����p�^�[��
};

#endif