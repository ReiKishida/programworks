//=============================================================================
//
// �G�t�F�N�g����[3D �|���S��] [effect.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "scene_billboard.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAXPARTICLE_TEXTURE (3)
//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CSceneBillBoard;
//*****************************************************************************
// �p�[�e�B�N���̃N���X�\����
//*****************************************************************************
class CParticle : public CSceneBillBoard
{

public:
	typedef enum
	{
		PARTICLEDRAWTYPE_ADD = 0,	//���Z����
		PARTICLEDRAWTYPE_DEST,		//���Z����
		PARTICLEDRAWTYPE_MAX		//�`��^�C�v�̍ő吔
	}PARTICLEDRAWTYPE;
	typedef enum
	{
		PARTICLETYPE_TEST = 0,			//�e�X�g�p�p�[�e�B�N��
		PARTICLETYPE_ATTACK_000,		//�U�����̃p�[�e�B�N��000
		PARTICLETYPE_ATTACK_SHORYUKEN,	//�������̃p�[�e�B�N��
		PARTICLETYPE_ATTACK_SKILL,		//���K�E�Z�̃p�[�e�B�N��
		PARTICLETYPE_ATTACK_SKILL_ENEMY,		//���K�E�Z�̃p�[�e�B�N��
		PARTICLETYPE_ATTACK_SPINKICK,	//��]�R��̃p�[�e�B�N��
		PARTICLETYPE_HIT_000,			//�q�b�g���̃p�[�e�B�N��000
		PARTICLETYPE_HIT_001,			//�q�b�g���̃p�[�e�B�N��001
		PARTICLETYPE_HIT_002,			//�q�b�g���̃p�[�e�B�N��002
		PARTICLETYPE_RAND,				//���n���̃p�[�e�B�N��
		PARTICLETYPE_SLIDING,			//�X���C�f�B���O���̃p�[�e�B�N��
		PARTICLETYPE_RUN,				//�����Ă��鎞�̃p�[�e�B�N��
		PARTICLETYPE_MAX				//�p�[�e�B�N���̎�ނ̍ő吔
	}PARTICLETYPE;
	CParticle(int nPriority = 7, OBJTYPE type = OBJTYPE_PARTICLE);	//�R���X�g���N�^
	~CParticle();	//�f�X�g���N�^
	static HRESULT Load(void);									//�e�N�X�`���̓ǂݍ���
	static void Unload(void);									//�e�N�X�`���̔j��
	static CParticle *Create(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLEDRAWTYPE drawtype, PARTICLETYPE type, int nLife);	//�p�[�e�B�N���̐���
	HRESULT Init(void);					//����������
	void Uninit(void);					//�I������
	void Update(void);					//�X�V����
	void Draw(void);					//�`�揈��

private:
	static LPDIRECT3DTEXTURE9		m_apTexture[MAXPARTICLE_TEXTURE];		// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3				m_pos;			// �|���S���̈ʒu
	D3DXVECTOR3				m_move;			// �|���S���̈ړ���
	D3DXVECTOR3				m_rot;			// �|���S���̉�]
	D3DXCOLOR				m_col;			//	�F���
	PARTICLEDRAWTYPE        m_Drawtype;		// �`��̎��
	PARTICLETYPE			m_Type;			// �p�[�e�B�N���̎��
	int						m_nLife;		// ����
	int						m_ParticleTextype;	//�p�[�e�B�N���̃e�N�X�`��
	float					m_fRadius;		// ���a

};
#endif