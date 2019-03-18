//=============================================================================
//
// �����蔻�菈��[�T�O] [collision.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �V�[��3D�̃N���X�\����
//*****************************************************************************
class CCollision : public CScene
{
public:

	typedef enum
	{
		COLLISIONTYPE_PLAYER_LIGHTPUNCH = 0,	//�v���C���[�̍U��(��p���`)
		COLLISIONTYPE_PLAYER_MEDIUMPUNCH,		//�v���C���[�̍U��(���p���`)
		COLLISIONTYPE_PLAYER_SHORYUKEN,			//�v���C���[�̍U��(������)
		COLLISIONTYPE_PLAYER_HEAVYPUNCH,		//�v���C���[�̍U��(���p���`)
		COLLISIONTYPE_PLAYER_LIGHTKICK,			//�v���C���[�̍U��(��L�b�N)
		COLLISIONTYPE_PLAYER_MEDIUMKICK,		//�v���C���[�̍U��(���L�b�N)
		COLLISIONTYPE_PLAYER_SPINKICK,			//�v���C���[�̍U��(��]�R��)
		COLLISIONTYPE_PLAYER_HEAVYKICK,			//�v���C���[�̍U��(���L�b�N)
		COLLISIONTYPE_PLAYER_SKILL,				//�v���C���[�̍U��(���K�E�Z)
		COLLSIONTYPE_ENEMY_LIGHTATTACK,			//�G�̍U��(��U��)
		COLLSIONTYPE_ENEMY_HEAVYATTACK,			//�G�̍U��(���U��)
		COLLSIONTYPE_BOSS_LIGHTATTACK,			//�{�X�̍U��(��U��)
		COLLSIONTYPE_BOSS_HEAVYATTACK,			//�{�X�̍U��(���U��)
		COLLSIONTYPE_BOSS_SKILLATTACK_S,			//�{�X�̍U��(�K�E�Z)
		COLLSIONTYPE_BOSS_SKILLATTACK_L,			//�{�X�̍U��(�K�E�Z��)
		COLLSIONTYPE_MAX						//�U���ő吔
	}COLLSIONTYPE;

	CCollision(int nPriority = 6, OBJTYPE type = OBJTYPE_COLLISION);	//�R���X�g���N�^
	~CCollision();	//�f�X�g���N�^
	static CCollision *CreateAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nLife,  float fradius, COLLSIONTYPE type, D3DXVECTOR3 poscreater);		//�����蔻���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	D3DXVECTOR3 GetPos(void);		//�ʒu���̎擾
	void SetPos(D3DXVECTOR3 pos);	//�ʒu���̑��
	void SetRot(D3DXVECTOR3 rot);	//�ʒu���̑��
	void SetMtx(D3DXMATRIX mtx);	//�p�x���̑��
	void SetLife(int nLife);		//���C�t�̐ݒ�
	D3DXMATRIX GetMatrix(void);				//�}�g���b�N�X�̎擾
	void SetParent(CCollision *pModel);		//�e�q�֌W�̐ݒ�
	bool CollisionAttack(D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, float fHeight);	//�U���̓����蔻��
	int GetCollisionType(void);				//��ނ̎擾
	float GetAngle(void);					//�p�x�̎擾

private:
	CCollision *m_pParent;				//�e���f���̃|�C���^
	D3DXMATRIX m_mtxWorld;				//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_poscreater;			//�쐬�҂̈ʒu���
	D3DXVECTOR3 m_pos;					//�ʒu
	D3DXVECTOR3 m_rot;					//�p�x
	float m_fRadius;					//���a
	float m_fAngle;
	int m_nLife;						//���C�t
	int m_nTypeCollision;				//�����������̎��
	COLLSIONTYPE m_type;				//����̎��


};




#endif