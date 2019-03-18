//=============================================================================
//
// �Q�[������[2D �|���S��] [game.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

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
class CLog;

class CGame
{
public:
	typedef enum
	{
		GAMESTATE_NONE = 0,	//�������Ă��Ȃ����
		GAMESTATE_NORMAL,	//�ʏ���
		GAMESTATE_CLEAR,	//�N���A���
		GAMESTATE_FAILED,	//���s���
		GAMESTATE_END,		//�I�����
		GAMESTATE_MAX		//�ő吔
	}GAMESTATE;
	CGame();	//�R���X�g���N�^
	~CGame();	//�f�X�g���N�^
	//static CGame *Create(void);	//�Q�[���̐���
	static HRESULT Init(void);	//����������
	static void Uninit(void);	//�I������
	static void Update(void);	//�X�V����
	static void Draw(void);	//�`�揈��
	static CPlayer *GetPlayer(void);
	static CBoss *GetBoss(void);
	static CEnemy *GetEnemy(int nCntEnemy);
	static void SetEnemy(void);
	static void SetGameState(GAMESTATE state);
	static GAMESTATE GetGameState(void);
	int GetFrame(void);

private:
	static CPlayer *m_pPlayer;//�v���C���[�̃|�C���^
	static CEnemy *m_apEnemy[MAX_ENEMY];	//�G�̃|�C���^
	static CUi *m_apUi[MAX_UI];	//UI�̃|�C���^
	static CBg *m_pBg;	//�w�i�̃|�C���^
	static CFollowForce *m_pFollowForce;	//�t�H�[�X�̃|�C���^
	static CForce *m_pForce;	//�t�H�[�X�̃|�C���^
	static CLog *m_pLog;	//�t�H�[�X�̃|�C���^
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

	static CGame *m_pGame;//�Q�[���̃|�C���^
	static GAMESTATE m_GameState;//�Q�[���̏��
	static int m_nCntGame;//�V�[���̈ڂ�
	static int m_nCntFrame;//�t���[�����̃J�E���g
};

#endif