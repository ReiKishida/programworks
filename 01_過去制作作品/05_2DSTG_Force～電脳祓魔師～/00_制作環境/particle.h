
//=============================================================================
//
// �p�[�e�B�N���̏���[2D �|���S��] [particle.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************


//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CParticle : public CScene2D
{
public:
	typedef enum
	{
		PARTICLETYPE_NONE = 0,	//��̎��
		PARITCLETYPE_HIT,		//��e
		PARITCLETYPE_HIT_R,		//��e
		PARITCLETYPE_EXPLOSION,	//����
		PARITCLETYPE_EXPLOSION_R,	//����
		PARITCLETYPE_MAX,		//�p�[�e�B�N���̍ő吔
	}PARTICLETYPE;//�p�[�e�B�N���̎��
	CParticle::CParticle();	//�R���X�g���N�^
	CParticle::~CParticle();	//�f�X�g���N�^
	static CParticle *Create(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLETYPE type, int nLife);	//�e�̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);//�e�N�X�`���̓ǂݍ���
	static void Unload(void);//�e�N�X�`���̔j��
private:
	int m_nLife;//���C�t
	float m_fRadius;//���a
	static LPDIRECT3DTEXTURE9 m_pTexture; //���L�e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_move;	//�ړ���
	PARTICLETYPE m_type;//�p�[�e�B�N���̎��
	D3DXCOLOR m_col;//�F���
};


#endif