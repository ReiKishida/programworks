//=============================================================================
//
// �Q�[������[2D �|���S��] [game.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
class CModel;
class CMeshCylinder;
class CSceneMeshWall;
class CComboGauge;
class CNumber;
class CTimer;
class CBoss;
//*****************************************************************************
// �Q�[���̃N���X�\����
//*****************************************************************************
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
	CGame();										//�R���X�g���N�^
	~CGame();										//�f�X�g���N�^
	 HRESULT Init(void);							//����������
	 void Uninit(void);								//�I������
	 void Update(void);								//�X�V����
	 void Draw(void);								//�`�揈��
	 void CreateEnemy(void);						//�G�̐���
	 static CSceneMeshField *GetSceneMeshField(void);//���b�V���t�B�[���h�V�[���̎擾
	 static CPlayer *GetPlayer(void);				//�v���C���[�̎擾
	 static CBoss *GetBoss(void);				//�v���C���[�̎擾
	 static CEnemy *GetEnemy(int nIndex);			//�G�̎擾
	 static CSceneMeshWall *GetWall(int nCnt);		//�ǂ̎擾
	 static CComboGauge *GetComboGauge(int nCnt);	//�R���{�Q�[�W�̎擾
	 static void SetGameState(GAMESTATE state);		//�Q�[���̏�Ԃ̐ݒ�
	 static GAMESTATE GetGameState(void);			//�Q�[���̏�Ԃ̎擾
	 static void ChangeBoolCreateBoss(void);		//�u�[���̐؂�ւ�
	 static bool GetBool(void);						//�X�C�b�`�̎擾

private:
	static CGame *m_pGame;					//�Q�[��
	static CSceneMeshField *m_pMeshField[4];//���b�V���t�B�[���h
	static CSceneMeshWall *m_apMeshWall[15];//���b�V���E�H�[��
	static CComboGauge *m_apComboGauge[2];  //�R���{�Q�[�W
	static CMeshCylinder *m_pMeshCylinder;	//�V�����_�[
	static CBoss *m_pBoss;					//�{�X
	static CPlayer *m_pPlayer;				//�v���C���[
	static CEnemy *m_pEnemy[30];			//�G
	static CModel *m_pModel;				//���f��
	static CObject *m_pObject;				//�I�u�W�F�N�g
	static CNumber *m_pNumber;				//����
	static CTimer *m_pTimer;				//����
	static CLight *m_pLight;				//���C�g

	static GAMESTATE m_GameState;//�Q�[���̏��
	static int m_nCntGame;//�V�[���̈ڂ�
	int m_nID;
	static bool m_bCreateBoss;
};

#endif