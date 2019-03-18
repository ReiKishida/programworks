//=============================================================================
//
// �J�b�g�C������[2D �|���S��] [cutin.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef CUTIN_H_
#define _CUTIN_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TEXTURE_CUTIN (6)	//�e�N�X�`����
#define MAX_LIFE_RYOU (75)
#define MAX_LIFE_IKI (75)
#define MAX_LIFE_TEN (55)
#define MAX_LIFE_KAI (45)
#define MAX_LIFE_CUTIN (90)

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CCutin : public CScene2D
{
public:
	typedef enum
	{
		CUTIN_TYPE_NONE = 0,
		CUTIN_TYPE_RYOU,
		CUTIN_TYPE_IKI,
		CUTIN_TYPE_TEN,
		CUTIN_TYPE_KAI,
		CUTIN_TYPE_CUT_PLAYER,
		CUTIN_TYPE_CUT_ENEMY,
		CUTIN_TYP_MAX
	}CUTINTYPE;
	CCutin();	//�R���X�g���N�^
	~CCutin();	//�f�X�g���N�^
	static CCutin *Create(D3DXVECTOR3 pos, CUTINTYPE type);	//�w�i�̐���
	HRESULT Init(D3DXVECTOR3 pos, CUTINTYPE type);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);//�e�N�X�`���̓ǂݍ���
	static void Unload(void);//�e�N�X�`���̔j��
	static void NextAnim(void);	//�e�N�X�`���A�j���[�V�����A�j���[�V����
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE_CUTIN]; //���L�e�N�X�`���̃|�C���^
	 CUTINTYPE m_Type;	//�w�i�̃^�C�v
	D3DXCOLOR m_Color;	//�F�ݒ�
	int m_nCntColor;	//�J���[�J�E���g�p
	int m_nLife;	//���C�t
	float m_fWidth;	//��
	float m_fHeight;	//����
	static int m_nPatternAnim;//�p�^�[���A�j���[�V����
	static int m_nCounterAnim;//�p�^�[���A�j���[�V����
};

#endif