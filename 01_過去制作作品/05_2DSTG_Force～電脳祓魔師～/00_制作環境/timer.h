//=============================================================================
//
// �^�C�}�[����[2D �|���S��] [timer.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TIMER_NUMBER (5)

class CTimer : public CScene
{
public:
	typedef enum
	{
		TIMERTYPE_MINUTE = 0,	//��
		TIMERTYPE_TENSECONDS,	//10�b��
		TIMERTYPE_ONESECONDS,	//1�b��
		TIMERTYPE_TENCOMMASECONDS,	//�R���}10�b��
		TIMERTYPE_ONECOMMASECONDS,	//�R���}1�b��

	}TIMETYPE;
	CTimer();	//�R���X�g���N�^
	~CTimer();	//�f�X�g���N�^
	static CTimer *Create(D3DXVECTOR3 pos);	//�����̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetTimer(int nMinutes, int nTenSeconds, int nOneSeconds, int nTenComma, int nOneComma);	//�X�R�A�̐ݒ�
	static int GetTimer(void);	//�X�R�A�̎擾
	static void AddSeconds(int nSeconds);	//�X�R�A�̉��Z
private:
	static CNumber *m_apNumber[TIMER_NUMBER];	//����
	static int m_nTimer;		//�^�C�}�[
	static int m_nMinutes;		//��
	static int m_nTenSeconds;	 //10�b��
	static int m_nOneSeconds;	 //10�b��

	static int m_nTenComma;	//�R���}�b
	static int m_nOneComma;	//�R���}�b

	int m_nCntFrame;	//�t���[�����J�E���g�p
	int m_nCntColor;	//�J���[�J�E���g�p
	bool bUse;//�J�E���g���邩
};

#endif