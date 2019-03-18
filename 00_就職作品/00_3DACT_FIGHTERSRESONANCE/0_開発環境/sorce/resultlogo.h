//=============================================================================
//
// ���S�̏���[2D �|���S��] [logo.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _RESULTLOGO_H_
#define _RESULTLOGO_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_RESULT_NUM (8)
#define MAX_RESULT_TEXTURE (8)

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CResultLogo : public CScene
{
public:
	typedef enum
	{
		RESULTSTATE_NONE = 0,	//��̏��
		RESULTSTATE_NORMAL,		//�ʏ���
		RESULTSTATE_USE,			//�G���^�[�������ꂽ���
		RESULTSTATE_MAX			//��Ԃ̍ő吔
	}RESULTSTATE;
	typedef enum
	{
		RESULTTYPE_LOGO = 0,		//�^�C�g�����S
		RESULTTYPE_PRESS,		//�v���X�G���^�[
		RESULTTYPE_BONUS,		//�N���A�{�[�i�X
		RESULTTYPE_SCORE,		//�X�R�A�{�[�i�X
		RESULTTYPE_TIME,		//�^�C���{�[�i�X
		RESULTTYPE_COMBO,		//�R���{�{�[�i�X
		RESULTTYPE_DAMAGE,		//�_���[�W�{�[�i�X
		RESULTTYPE_TOTAL,		//TOTAL�{�[�i�X
		RESULTTYPE_MAX			//�^�C�g���̎�ނ̍ő吔
	}RESULTTYPE;
	CResultLogo();				//�R���X�g���N�^
	~CResultLogo();				//�f�X�g���N�^
	static HRESULT Load(void);	//�e�N�X�`���̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���̔j��
	static CResultLogo *Create(void);	//�G�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static void SwitchEnable(void);
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_RESULT_TEXTURE]; //���L�e�N�X�`���̃|�C���^
	static CScene2D *m_apScene2D[MAX_RESULT_NUM];//�V�[��2D
	static D3DXCOLOR m_col; //�F���
	int m_nCntFrame;//�t���[�����̃J�E���g
	float m_fWidth;//��
	float m_fHeight;//����
	static RESULTSTATE m_state[MAX_RESULT_NUM]; //���
	static RESULTTYPE m_type[MAX_RESULT_NUM];//�^�C�g���̎��
	int m_TexNum;	//�e�N�X�`���̔ԍ�
	static int m_nJumpRankingFrame;//�����L���O��ʂ֑J�ڂ���Ƃ��̃t���[���J�E���^
	static bool m_bEnable;
};

#endif