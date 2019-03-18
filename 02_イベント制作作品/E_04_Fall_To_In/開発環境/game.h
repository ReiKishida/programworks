//=============================================================================
//
// �Q�[���̏��� [game.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
class CPlayer;
class CLight;
class CCamera;

//====================================
// �Q�[���N���X
//====================================
class CGame
{
public:
	typedef enum
	{// �X�e�[�^�X
		STATE_NONE=0,
		STATE_START,
		STATE_END,
		STATE_MAX,
	}STATE;
	typedef enum
	{
		STAGE_SPACE=0,
		STAGE_EARTH,
		STAGE_HELL,
		STAGE_MAX,
	}STAGE;
	CGame();
	~CGame();
	static CGame *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CLight *GetLight(void) { return m_pLight; }						// ���C�g
	static CCamera *GetCamera(void) { return m_pCamera; }					// �J����
	static CPlayer *GetPlayer(void) { return m_pPlayer; }	// �v���C���[
	static int m_WinId;

	static bool GetPause(void);					// �|�[�Y�擾
	static void SetPause(bool bPause);			// �|�[�Y�ݒ�
	static STATE GetState(void);				// �X�e�[�^�X
	static void SetState(const STATE state);	// �X�e�[�^�X
	static STAGE GetStage(void) {return m_Stage;};
private:
	static CLight *m_pLight;					// ���C�g
	static CCamera *m_pCamera;					// �J����
	static CPlayer *m_pPlayer;		// �v���C���[

	static bool m_bPause;						// �|�[�Y
	static STATE m_State;
	static int m_nStateCounter;					// �X�e�[�^�X�J�E���^�[
	static STAGE m_Stage;
	static bool m_bStage[STAGE_MAX];
	static int m_nPanelCounter;
};
#endif