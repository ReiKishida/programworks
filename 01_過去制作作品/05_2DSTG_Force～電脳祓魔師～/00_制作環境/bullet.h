//=============================================================================
//
// �e�̏���[2D �|���S��] [bullet.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************


//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CBullet : public CScene2D
{
public:
	typedef enum
	{
		BULLETTYPE_PLAYER = 0,		//�v���C���[�̒e
		BULLETTYPE_ENEMY_NORMAL,	//�G�̒e(�ʏ�)
		BULLETTYPE_ENEMY_HORMING,	//�G�̒e(�v���C���[�ǔ�)
		BULLETTYPE_ENEMY_BOMB,		//�G�̒e(�{��)
		BULLETTYPE_ENEMY_LASER,		//���[�U�[(R)
		BULLETTYPE_ENEMY_LASER_L,		//���[�U�[(R)
		BULLETTYPE_ENEMY_LASER_R,		//���[�U�[(L)
		BULLETTYPE_MAX				//�e�̎�ނ̍ő吔
	}BULLETTYPE;
	CBullet::CBullet();	//�R���X�g���N�^
	CBullet::~CBullet();	//�f�X�g���N�^
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type, int nLife ,int nAttack);	//�e�̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);
	static void Unload(void);
private:
	int m_nLife;	//���C�t
	int m_nAttack;	//�U����
	static LPDIRECT3DTEXTURE9 m_pTexture; //���L�e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_BulletMove;//�e�̈ړ���
	D3DXVECTOR3 m_Move;//�ړ���
	BULLETTYPE m_type;	//�e�̎��
	int m_nCntFrame;
};

#endif