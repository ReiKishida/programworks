
//=============================================================================
//
// �G�t�F�N�g�̏���[2D �|���S��] [effect.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_EFFECTTYPE (3)

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CEffect : public CScene2D
{
public:
	typedef enum
	{
		EFFECTTYPE_BULLET = 0,		//�e�̃G�t�F�N�g
		EFFECTTYPE_PLAYER,		//�v���C���[�̃G�t�F�N�g
		EFFECTTYPE_BOSS,		//�{�X�̃G�t�F�N�g
		EFFECTTYPE_MAX			//�G�t�F�N�g�̍ő吔
	}EFFECTTYPE;
	CEffect::CEffect();	//�R���X�g���N�^
	CEffect::~CEffect();	//�f�X�g���N�^
	static CEffect *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight , int nLife, EFFECTTYPE type);	//�e�̐���
	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nLife);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);	//�e�N�X�`���̃��[�h
	static void Unload(void);	//�e�N�X�`���̔j��
private:
	int m_nLife;
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_EFFECTTYPE]; //���L�e�N�X�`���̃|�C���^
	EFFECTTYPE m_type;	//�G�t�F�N�g�̎��
};

#endif