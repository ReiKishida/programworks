//=============================================================================
//
// �����蔻�菈��[�T�O] [Collision.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "collision.h"
#include "scene2D.h"
#include "debugproc.h"
#include "scene_meshorbit.h"
#include "particle.h"
#include "enemy.h"
#include "enemy_pow.h"
#include "enemy_trc.h"
#include "player.h"
#include "boss.h"
#include "game.h"
#include "tutorial.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CHAR (256)
#define GRAVITY_NUM (0.7f)

//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CCollision::CCollision(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{

}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CCollision::~CCollision()
{
}



//=============================================================================
// �֐����F�����蔻��̐���
// �֐��̊T�v�F�����蔻��𐶐�����
//=============================================================================
CCollision * CCollision::CreateAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, float fradius, COLLSIONTYPE type, D3DXVECTOR3 poscreater)
{
	CCollision  *pCollision;
	pCollision = new CCollision;//�R���W�����N���X�̐���

	pCollision->m_pos = pos;
	pCollision->m_rot = rot;
	pCollision->m_fRadius = fradius;
	pCollision->m_nLife = nLife;
	pCollision->m_type = type;
	pCollision->m_poscreater = poscreater;
	pCollision->Init();
	return pCollision;
}

//=============================================================================
// �֐����F�����蔻��̏���������
// �֐��̊T�v�FX�t�@�C�����̐ݒ�
//=============================================================================
HRESULT CCollision::Init(void)
{
	if (m_type == COLLISIONTYPE_PLAYER_LIGHTPUNCH || m_type == COLLISIONTYPE_PLAYER_MEDIUMPUNCH
		|| m_type == COLLISIONTYPE_PLAYER_HEAVYPUNCH || m_type == COLLISIONTYPE_PLAYER_SHORYUKEN
		|| m_type == COLLISIONTYPE_PLAYER_LIGHTKICK || m_type == COLLISIONTYPE_PLAYER_MEDIUMKICK
		|| m_type == COLLISIONTYPE_PLAYER_HEAVYKICK || m_type == COLLISIONTYPE_PLAYER_SPINKICK
		|| m_type == COLLISIONTYPE_PLAYER_SKILL)
	{

		SetObjType(OBJTYPE_COLLISION);
	}
	else
	{
		SetObjType(OBJTYPE_COLLISION_ENEMY);
	}


	return S_OK;
}

//=============================================================================
// �֐����F�����蔻��̏I������
// �֐��̊T�v�F���b�V���A�}�e���A���A���g�̔j��
//=============================================================================
void CCollision::Uninit(void)
{
	//���g�̔j��
	Release();
}

//=============================================================================
// �֐����F�����蔻��̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CCollision::Update(void)
{



		m_nLife--;

		if (m_nLife <= 0)
		{
			m_nLife = 0;
			//�I������
			Uninit();
		}

		//CDebugProc::Print("�����蔻�蔭��\n");

}


//=============================================================================
// �֐����F�����蔻��̕`�揈��
// �֐��̊T�v�F--
//=============================================================================
void CCollision::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	//�e�̃}�g���b�N�X
	D3DXMATRIX mtxParent;


	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxrot, mtxtrans;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxrot);


	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxtrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxtrans);


	//�e�̃}�g���b�N�X�Ɗ|�����킹��
	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetMatrix();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

}
//=============================================================================
// �֐����F�ʒu���̎擾
// �֐��̊T�v�F�ʒu�����擾����
//=============================================================================
D3DXVECTOR3 CCollision::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// �֐����F�ʒu���̑��
// �֐��̊T�v�F�ʒu����������
//=============================================================================
void CCollision::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}


//=============================================================================
// �֐����F�p�x���̑��
// �֐��̊T�v�F�p�x����������
//=============================================================================
void CCollision::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// �֐����F���[���h�}�g���b�N�X�̑��
// �֐��̊T�v�F���[���h�}�g���b�N�X����������
//=============================================================================
void CCollision::SetMtx(D3DXMATRIX mtx)
{
	m_mtxWorld = m_mtxWorld;
}

//=============================================================================
// �֐����F���C�t�̑��
// �֐��̊T�v�F���C�t��������
//=============================================================================
void CCollision::SetLife(int nLife)
{
	m_nLife = nLife;
}


//=============================================================================
// �֐����F���[���h�}�g���b�N�X�̎擾
// �֐��̊T�v�F���[���h�}�g���b�N�X�����擾����
//=============================================================================
D3DXMATRIX CCollision::GetMatrix(void)
{
	return m_mtxWorld;
}

//=============================================================================
// �֐����F�e�̃��f��
// �֐��̊T�v�F�e�̃��f���̐ݒ�
//=============================================================================
void CCollision::SetParent(CCollision *pCollision)
{
	m_pParent = pCollision;
}



//=============================================================================
// �֐����F�����蔻��i�U���j
// �֐��̊T�v�F�U��->�̂̓����蔻��
//=============================================================================
bool CCollision::CollisionAttack(D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, float fHeight)
{
	bool bCollision = false;

	D3DXVECTOR3 ColPos;
	float fLength = 0.0f;		//����
	float fRadius = 0.0f;		//���a


	//m_fAngle = atan2f(pos->x - m_poscreater.x, pos->z - m_poscreater.z);// ���������p�x���v�Z

	ColPos.x = (pos->x - m_pos.x) * (pos->x - m_pos.x);
	ColPos.y = (pos->y - m_pos.y) * (pos->y - m_pos.y);
	ColPos.z = (pos->z - m_pos.z) * (pos->z - m_pos.z);
	fLength = ColPos.x + ColPos.y + ColPos.z;
	fRadius = (m_fRadius + fRadius) * (m_fRadius + fRadius);

	if (m_pos.y <= pos->y + fHeight  && m_pos.y >= pos->y)
	{//���a���ɓ�������

		if (ColPos.x + ColPos.z <= fRadius)
		{

			bCollision = true;

			switch (m_type)
			{
			case COLLISIONTYPE_PLAYER_LIGHTPUNCH:
				m_nTypeCollision = COLLISIONTYPE_PLAYER_LIGHTPUNCH;
				break;
			case COLLISIONTYPE_PLAYER_MEDIUMPUNCH:
				m_nTypeCollision = 1;
				break;
			case COLLISIONTYPE_PLAYER_SHORYUKEN:
				m_nTypeCollision = 2;
				break;
			case COLLISIONTYPE_PLAYER_HEAVYPUNCH:
				m_nTypeCollision = 3;
				break;
			case COLLISIONTYPE_PLAYER_LIGHTKICK:
				m_nTypeCollision = 4;
				break;
			case COLLISIONTYPE_PLAYER_MEDIUMKICK:
				m_nTypeCollision = 5;
				break;
			case COLLISIONTYPE_PLAYER_SPINKICK:
				m_nTypeCollision = 6;
				break;
			case COLLISIONTYPE_PLAYER_HEAVYKICK:
				m_nTypeCollision = 7;
				break;
			case COLLISIONTYPE_PLAYER_SKILL:
				m_nTypeCollision = 8;
				break;
			case COLLSIONTYPE_ENEMY_LIGHTATTACK:
				m_nTypeCollision = 9;
				break;
			case COLLSIONTYPE_ENEMY_HEAVYATTACK:
				m_nTypeCollision = 10;
				break;
			case COLLSIONTYPE_BOSS_LIGHTATTACK:
				m_nTypeCollision = 11;
				break;
			case COLLSIONTYPE_BOSS_HEAVYATTACK:
				m_nTypeCollision = 12;
				break;
			case COLLSIONTYPE_BOSS_SKILLATTACK_S:
				m_nTypeCollision = 13;
				break;
			case COLLSIONTYPE_BOSS_SKILLATTACK_L:
				m_nTypeCollision = 14;
				break;
			}
		}
	}
	return bCollision;
}

//=============================================================================
// �֐����F�����蔻��̎�ނ̎擾
// �֐��̊T�v�F����̎�ނ�Ԃ�
//=============================================================================
int CCollision::GetCollisionType(void)
{
	return m_nTypeCollision;
}

float CCollision::GetAngle(void)
{
	return m_fAngle;
}









