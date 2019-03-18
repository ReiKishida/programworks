//=============================================================================
//
// �G�t�F�N�g����[3D �|���S��] [effect.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

#include "main.h"
#include "scene_billboard_anim.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CSceneBillBoardAnim;
//*****************************************************************************
// �G�t�F�N�g�̃N���X�\����
//*****************************************************************************
class CEffect : public CSceneBillBoardAnim
{
public:
	typedef enum
	{
		EFFECTMODE_SPREAD = 0,
		EFFECTMODE_SHRINKING,
		EFFECTMODE_MAX
	}EFFECTMODE;

	typedef enum
	{
		EFFECTTYPE_SMALL = 0,
		EFFECTTYPE_MEDIUM,
		EFFECTTYPE_LARGE,
		EFFECTTYPE_MAX
	}EFFECTTYPE;

	CEffect(int nPriority = 3, OBJTYPE type = OBJTYPE_EFFECT);	//�R���X�g���N�^
	~CEffect();	//�f�X�g���N�^
	static HRESULT Load(void);									//�e�N�X�`���̓ǂݍ���
	static void Unload(void);									//�e�N�X�`���̔j��
	static CEffect *Create(D3DXVECTOR3 pos, float fRadius, int nLife, EFFECTTYPE type, EFFECTMODE mode);//�G�t�F�N�g�̐���
	HRESULT Init(void);						//����������
	void Uninit(void);						//�I������
	void Update(void);						//�X�V����
	void Draw(void);						//�`�揈��

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[3]; //���L�e�N�X�`���̃|�C���^
	D3DXVECTOR3				m_pos;			// �|���S���̈ʒu
	D3DXVECTOR3				m_rot;			// �|���S���̉�]
	float m_fRadius;	//��
	int m_nLife;		//�A�j���[�V�����J�E���^�[
	EFFECTTYPE m_type;
	EFFECTMODE m_mode;
};

#endif