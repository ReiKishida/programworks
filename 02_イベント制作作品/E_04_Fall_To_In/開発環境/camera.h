////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �J�������� [camera.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"
#define MAX_CAMERA		(3)

//******************************************************************************************************************************
// �J�����̃N���X
//******************************************************************************************************************************
class CCamera
{
public:
	typedef enum
	{
		CAMERASTATE_NONE = 0,
		CAMERASTATE_EVENT ,
		CAMERASTATE_START,
		CAMERASTATE_MAX,
	}CAMERASTATE;

	typedef struct
	{
		D3DXVECTOR3	m_posV;			// ���_
		D3DXVECTOR3	m_posVOld;		// �O��̎��_
		D3DXVECTOR3	m_posR;			// �����_
		D3DXVECTOR3	m_vecU;			// ������x�N�g��
		D3DXVECTOR3	m_rot;			// ��]��
		D3DXVECTOR3 m_ViewRot;		// ALT�L�[�ł̉�]
		D3DXVECTOR3 m_Diffrot;		// �p�x�̍���
		D3DXVECTOR3 m_Destrot;		// �ړI�̊p�x
		D3DXVECTOR3 m_posVDiff;		// �ꏊ�̍���
		D3DXVECTOR3 m_posVDest;		// �ړI�̏ꏊ
		D3DXVECTOR3 m_posRDiff;		// �ꏊ�̍���
		D3DXVECTOR3 m_posRDest;		// �ړI�̏ꏊ
		CAMERASTATE m_nState;			// ���
		int m_nStateCounter;			// ��ԃJ�E���^�[
		float m_fLength;				// V����
		float m_fHeight;				// V����
		float m_fLengthR;				// R����
		float m_fHeightR;				// R����
		D3DXMATRIX m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
		D3DXMATRIX m_mtxView;			// �r���[�}�g���b�N�X
		D3DVIEWPORT9 m_viewpoart;		// �r���[�|�[�g
	}Camera;

	CCamera();
	~CCamera();
	void SetPosV(const int nCamera, const D3DXVECTOR3 pos);
	void SetPosR(const int nCamera, const D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPosV(int nCamera) { return m_aCamera[nCamera].m_posV; }						// ���_�̏��m��
	D3DXVECTOR3 GetPosR(int nCamera) { return m_aCamera[nCamera].m_posR; }						// �����_�̏��m��
	D3DXVECTOR3 GetRot(int nCamera) { return m_aCamera[nCamera].m_rot; };

	D3DXMATRIX GetMtxView(int nCamera) { return m_aCamera[nCamera].m_mtxView; };
	D3DXMATRIX GetMtxProjection(int nCamera) { return m_aCamera[nCamera].m_mtxProjection; };
	CAMERASTATE GetState(int nCamera) { return m_aCamera[nCamera].m_nState; };

	bool GetSlow(void) { return m_bSlow; };
	bool GetGrid(void) { return m_bGrid; };
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Set(int nCamera);
	void SetState(int nCamera, CAMERASTATE state);
	void SetSlow(bool slow);
	void SetGrid(bool grid);
private:
	Camera m_aCamera[MAX_CAMERA];

	static bool m_bPause;			// �|�[�Y�����ǂ���
	bool m_bReverse;
	D3DXVECTOR3	m_PauseOldposV;		// ���_
	D3DXVECTOR3	m_PauseOldposR;		// �����_
	D3DXVECTOR3	m_PauseOldrot;		// ��]��
	float m_move;
	float m_Rotplus;
	int m_nCntPos;
	int m_nCntMove;					// �ړ��ʂ̃J�E���^�[
	int m_nCntRot;
	float m_TitleRot;
	bool m_bPos;
	bool m_bRot;					// ��]���邩�ǂ���
	bool m_bSlow;					// �X���[���[�h
	bool m_bGrid;					// �O���b�h���[�h
};
#endif
