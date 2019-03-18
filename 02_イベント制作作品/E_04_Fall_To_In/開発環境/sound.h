////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �T�E���h���� [sound.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"
//====================================
// �T�E���h�N���X
//====================================
class CSound
{
public:
	typedef enum
	{// �T�E���h�t�@�C��
		LABEL_BGM_TITLE = 0,// �^�C�g��
		LABEL_BGM_GAME,		// �Q�[��
		LABEL_BGM_RESULT,		// �j���[�X

		LABEL_SE_POSE,		// �|�[�Y�����߂�Ƃ�
		LABEL_SE_SUCCES,	// ������
		LABEL_SE_FAILED,	// ���s��
		LABEL_SE_ALERT,		// �x����
		LABEL_SE_DECIDE,	// ���艹
		LABEL_MAX
	}LABEL;
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g(-1�Ń��[�v�Đ�)
		float fAddVolume;// �{�����[��
	} SOUNDPARAM;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Update(void);
	void Uninit(void);
	HRESULT PlaySound(int nIdy, LABEL label);	// �Đ�
	void Stop(int nIdy, LABEL label);		// Uninit�œ������O�Ŏ~�߂�
	void Stop(void);
	void OnceStop(int nIdy, LABEL label, bool bStop);
	void ChangeVolume(int nIdy, LABEL label, float fVolume);
	void ChangePitch(int nIdy, LABEL label, float fPitch);
	float ChangeVolumeRange(int nIdy, LABEL label, D3DXVECTOR3 sound, D3DXVECTOR3 pos, float MaxRange, float MinRange);
	bool GetPlaySound(int nIdy, LABEL label);
	void SetFadeOut(int nIdy, LABEL label, bool bFadeOut);
private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X�ւ̃|�C���^
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX][MAX_PLAYER] = {};	// �\�[�X�{�C�X�ւ̃|�C���^
	BYTE *m_apDataAudio[LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�ւ̃|�C���^
	DWORD m_aSizeAudio[LABEL_MAX] = {};						// �I�[�f�B�I�f�[�^�T�C�Y
	bool m_abFadeOut[LABEL_MAX][MAX_PLAYER];
	// �e���f�ނ̃p�����[�^
	SOUNDPARAM m_aSoundParam[LABEL_MAX] =
	{
		{ "data/BGM/title.wav", -1 ,1.0f },		// �^�C�g��
		{ "data/BGM/game.wav", -1 ,1.0f },		// �Z���N�g
		{ "data/BGM/result.wav", -1 ,1.0f },	// �Q�[��

		{ "data/SE/pose.wav",0,1.0f },		// �|�[�Y���Ƃ�Ƃ�
		{ "data/SE/succes.wav",0,1.5f },	// ���������Ƃ�
		{ "data/SE/failed.wav",0,1.0f },	// ���s�����Ƃ�
		{ "data/SE/alert.wav",0,1.0f },		// �x��
		{ "data/SE/decide.wav",0,1.0f }		// �x��
	};

};

#endif
