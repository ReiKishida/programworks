//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
class CFade;
class CPause;
class CBoss;
class CMenuRanking;
class CTutorial;
class CInputX;


class CManager
{
public:
	typedef enum
	{
		MODE_TITLE = 0,		//�^�C�g�����
		MODE_TUTORIAL,		//�`���[�g���A�����
		MODE_MENU,			//�Q�[�����
		MODE_GAME,			//�Q�[�����
		MODE_RESULT,		//���U���g���
		MODE_MENURANKING,		//�����L���O���
		MODE_RANKING,		//�����L���O���
		MODE_MAX			//���[�h�̍ő吔
	}MODE;
	CManager();	//�R���X�g���N�^
	~CManager();//�f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static CInputKeyboard *GetInputKeyboard(void);	//�L�[�{�[�h�̎擾
	static CInputX *GetInputX(void);	//XInput�̎擾
	static CRenderer *GetRenderer(void);	//�����_���[�̎擾
	static CPlayer *GetPlayer(void);	//�v���C���[�̎擾
	static CEnemy *GetEnemy(void);	//�G�̎擾
	static CBoss *GetBoss(void);	//�{�X�̎擾
	static CSound *GetSound(void);	//�T�E���h�̎擾
	static CScore *GetScore(void);	//�G�̎擾
	static void SetMode(MODE mode);//���[�h�̐ݒ�
	static MODE GetMode(void);//���[�h�̎擾
	//�n�C�X�R�A
	static void SetHighScore(int nValue);
	static int GetHighScoreNum(void);
	//�X�R�A
	static void SetScore(int nValue);
	static int GetScoreNum(void);
private:
	static CRenderer *m_pRenderer;	//�����_���̃|�C���^
	static CInputKeyboard *m_pInputKeyboard;	//�L�[�{�[�h�̃|�C���^
	static CInputX *m_pInputX;	//�L�[�{�[�h�̃|�C���^
	static CPlayer *m_pPlayer;//�v���C���[�̃|�C���^
	static CEnemy *m_pEnemy;	//�G�̃|�C���^
	static CBoss *m_pBoss;//�{�X�̃|�C���^
	static CSound *m_pSound; //�T�E���h�̃|�C���^
	static CScore *m_pScore; //�X�R�A�̃|�C���^
	static CTitle *m_pTitle;//�^�C�g��
	static CGame *m_pGame;//�Q�[��
	static CTutorial *m_pTutorial;//���U���g
	static CMenu *m_pMenu;//���j���[
	static CResult *m_pResult;//���U���g
	static CRanking *m_pRanking;//�����L���O
	static CMenuRanking *m_pMenuRanking;//���j���[�����L���O
	static CFade *m_pFade;//�t�F�[�h
	static CPause *m_pPause;//�|�[�Y
	static MODE m_mode;	//���[�h
	//static bool m_bPause;//�|�[�Y
	static int m_nScore;	//�X�R�A
	static int m_nHighScore;	//�n�C�X�R�A

};


#endif