////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���U���g�̏��� [result.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RESULT_H_
#define _RESILT_H_

//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 move;	// �ړ� 
	D3DXCOLOR col;		// �F
	int nCounterAnim;	// �J�E���^�[
	int nPatternAnim;	// �p�^�[��
	int nCounterFade;	// �t�F�[�h�J�E���^�[
	int nType;			// ���
	float fWidth;		// ��
	float fHeight;		// �c
	bool bUse;			// �g�p���Ă��邩�ǂ���
}Result;
typedef enum
{
	RESULTTEXTYPE_SCORE_MAX=0,
	RESULTTEXTYPE_SCORE,
	RESULTTEXTYPE_TIME,
	RESULTTEXTYPE_HIT,
	RESULTTEXTYPE_TEXT_SCORE_MAX,
	RESULTTEXTYPE_TEXT_SCORE,
	RESULTTEXTYPE_TEXT_TIME,
	RESULTTEXTYPE_TEXT_HIT,
	RESULTTEXTYPE_TEXT_DOUBLEKORON,
	RESULTTEXTYPE_TEXT_DOUBLEKORON1,
	RESULTTEXTYPE_TEXT_DOUBLEKORON2,
	RESULTTEXTYPE_TEXT_DOUBLEKORON3,
	RESULTTEXTYPE_TEXT_PT,
	RESULTTEXTYPE_TEXT_BYOU,
	RESULTTEXTYPE_TEXT_HITMOJI,
	RESULTTEXTYPE_TEXT_PT_MAX,
	RESULTTEXTYPE_GAMECLEAR_MAX,
}RESULTTEXTYPE_GAMECLEAR;

typedef enum
{
	RESULTTYPE_NONE = 0,
	RESULTTYPE_CLEAR,	// �N���A���
	RESULTTYPE_GAMEOVER,		// �Q�[���I�[�o�[���
	RESULTTYPE_MAX,
}RESULTTYPE;
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetResultType(RESULTTYPE type);
void SetResult(D3DXVECTOR3 pos, int nType,float fWidth,float fHeight);
RESULTTYPE GetResultType(void);
void GetScore(int nScore);
void GetTime(int nTime);
void GetHit(int nHit);
#endif // !_RESULT_H_

