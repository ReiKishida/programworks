//=============================================================================
//
// �����L���O���S�̏���[2D �|���S��] [rankinglogo.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _RANKINGLOGO_H_
#define _RANKINGLOGO_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_RANKING_NUM (3)
#define MAX_RANKING_TEXTURE (3)

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CRankingLogo : public CScene
{
public:
	typedef enum
	{
		RANKINGSTATE_NONE = 0,	//��̏��
		RANKINGSTATE_NORMAL,		//�ʏ���
		RANKINGSTATE_USE,			//�G���^�[�������ꂽ���
		RANKINGSTATE_MAX			//��Ԃ̍ő吔
	}RANKINGSTATE;
	typedef enum
	{
		RANKINGTYPE_FLAME = 0,		//�v���X�G���^�[
		RANKINGTYPE_LOGO,		//�^�C�g�����S
		RANKINGTYPE_PRESS,		//�v���X�G���^�[
		RANKINGTYPE_MAX			//�^�C�g���̎�ނ̍ő吔
	}RANKINGTYPE;
	CRankingLogo(int nPriority = 6, OBJTYPE type = OBJTYPE_RANKINGLOGO);				//�R���X�g���N�^
	~CRankingLogo();				//�f�X�g���N�^
	static HRESULT Load(void);	//�e�N�X�`���̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���̔j��
	static CRankingLogo *Create(void);	//�G�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_RANKING_TEXTURE]; //���L�e�N�X�`���̃|�C���^
	static CScene2D *m_apScene2D[MAX_RANKING_NUM];//�V�[��2D
	static D3DXCOLOR m_col; //�F���
	int m_nCntFrame;//�t���[�����̃J�E���g
	float m_fWidth;//��
	float m_fHeight;//����
	RANKINGSTATE m_state;//���
	RANKINGTYPE m_type;//�^�C�g���̎��
	int m_TexNum;	//�e�N�X�`���̔ԍ�
	static int m_nJumpTitleFrame;//�^�C�g����ʂ֑J�ڂ���Ƃ��̃t���[���J�E���^
};

#endif