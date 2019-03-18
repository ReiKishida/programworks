//=============================================================================
//
// �`���[�g���A������[2D �|���S��] [tutorial.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY (50)
#define MAX_UI (12)

#include "main.h"
class CUi;
class CForce;
class CFollowForce;
class CCutin;
class CRemaining;
class CParticle;
class CBoss;
class CForceNumber;
class CHighScore;
class CPause;
class CTutorialLogo;
class CTutorial
{
public:
	typedef enum
	{
		TUTORIALSTATE_NONE = 0,	//�������Ă��Ȃ����
		TUTORIALSTATE_NORMAL,	//�ʏ���
		TUTORIALSTATE_CLEAR,	//�N���A���
		TUTORIALSTATE_FAILED,	//���s���
		TUTORIALSTATE_END,		//�I�����
		TUTORIALSTATE_MAX		//�ő吔
	}TUTORIALSTATE;
	CTutorial();	//�R���X�g���N�^
	~CTutorial();	//�f�X�g���N�^
				//static CTutorial *Create(void);	//�Q�[���̐���
	static HRESULT Init(void);	//����������
	static void Uninit(void);	//�I������
	static void Update(void);	//�X�V����
	static void Draw(void);	//�`�揈��
	static CPlayer *GetPlayer(void);
	static CBoss *GetBoss(void);
	static CEnemy *GetEnemy(int nCntEnemy);
	static void SetEnemy(void);
	static void SetTutorialState(TUTORIALSTATE state);
	static TUTORIALSTATE GetTutorialState(void);
	int GetFrame(void);

private:
	static CPlayer *m_pPlayer;//�v���C���[�̃|�C���^
	static CEnemy *m_apEnemy[MAX_ENEMY];	//�G�̃|�C���^
	static CUi *m_apUi[MAX_UI];	//UI�̃|�C���^
	static CBg *m_pBg;	//�w�i�̃|�C���^
	static CFollowForce *m_pFollowForce;	//�t�H�[�X�̃|�C���^
	static CForce *m_pForce;	//�t�H�[�X�̃|�C���^
	static CBullet *m_pBullet;	//�e�̃|�C���^
	static CEffect *m_pEffect;	//�G�t�F�N�g�̃|�C���^
	static CExplosion *m_pExplosion;	//�����̃|�C���^
	static CNumber *m_pNumber;	//�����̃|�C���^
	static CCutin *m_pCutin;	//�J�b�g�C���̃|�C���^
	static CRemaining *m_pRemaining;	//�c�@���̃|�C���^
	static CParticle *m_pParticle;	//�p�[�e�B�N���̃|�C���^
	static CBoss *m_pBoss;	//�{�X�̃|�C���^
	static CForceNumber *m_pForceNumber;	//�t�H�[�X���̃|�C���^
	static CHighScore *m_pHighScore;	//�n�C�X�R�A�̃|�C���^
	static CScore *m_pScore;	//�X�R�A�̃|�C���^

	static CTutorial *m_pTutorial;//�`���[�g���A���̃|�C���^
	static CTutorialLogo *m_pTutorialLogo;//�`���[�g���A�����S�̃|�C���^
	static TUTORIALSTATE m_TutorialState;//�Q�[���̏��
	static int m_nCntTutorial;//�V�[���̈ڂ�
	static int m_nCntFrame;//�t���[�����̃J�E���g
};

#endif