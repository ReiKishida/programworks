////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �|�[�Y�̏��� [pause.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _PAUSE_H_
#define _PAUSE_H_
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef enum
{
	PAUSETYPE_BG = 0,
	PAUSETYPE_PAUSE,
	PAUSETYPE_CONTINUE,
	PAUSETYPE_RETRY,
	PAUSETYPE_QUIT,
	PAUSE_LOCK_PUNCH_PUNCH,				// �p���`->�p���`
	PAUSE_LOCK_PUNCH_PUNCH_PUNCH,		// �p���`->�p���`->�p���`
	PAUSE_LOCK_PUNCH_SPIN,				// �p���`->�X�s��
	PAUSE_LOCK_PUNCH_SPIN_JUMP,			// �p���`->�X�s��->�W�����v
	PAUSE_LOCK_KICK_KICK,				// �L�b�N->�L�b�N
	PAUSE_LOCK_KICK_KICK_KICK,			// �L�b�N->�L�b�N->�L�b�N
	PAUSE_LOCK_KICK_SPIN,				// �L�b�N->�X�s��
	PAUSE_LOCK_KICK_JUMP,				// �L�b�N->�W�����v
	PAUSE_LOCK_KICK_JUMP_PUNCH,			// �L�b�N->�W�����v->�p���`	
	PAUSE_LOCK_SPIN_PUNCH,				// �X�s��->�p���`
	PAUSE_LOCK_SPIN_KICK,				// �X�s��->�L�b�N
	PAUSE_LOCK_SPIN_PUNCH_KICK,			// �X�s��->�p���`->�L�b�N	
	PAUSE_LOCK_JUMP_KICK,				// �W�����v->�L�b�N
	PAUSE_LOCK_JUMP_SPIN,				// �W�����v->�X�s��
	PAUSE_LOCK_JUMP_PUNCH,				// �W�����v->�p���`
	PAUSETYPE_MAX
}PAUSETYPE;
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 Initpos;// �����ʒu
	D3DXCOLOR col;		// �F
	D3DXVECTOR3 move;	// �ړ� 
	int nType;			// ���
	int nCounterAnim;	// �J�E���^�[
	int nPatternAnim;	// �p�^�[��
	float fWidth;		// ��
	float fHeight;		// ����
	bool bUse;			// �g�p���Ă��邩�ǂ���
} PAUSE;
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SetPause(D3DXVECTOR3 pos, D3DXCOLOR col, int nType, float fWidth, float fHeight);
PAUSE *GetPauseData(void);
#endif // !_PAUSE_H_

