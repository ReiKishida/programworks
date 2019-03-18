////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// サウンド処理 [sound.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"
//====================================
// サウンドクラス
//====================================
class CSound
{
public:
	typedef enum
	{// サウンドファイル
		LABEL_BGM_TITLE = 0,// タイトル
		LABEL_BGM_GAME,		// ゲーム
		LABEL_BGM_RESULT,		// ニュース

		LABEL_SE_POSE,		// ポーズをきめるとき
		LABEL_SE_SUCCES,	// 成功時
		LABEL_SE_FAILED,	// 失敗時
		LABEL_SE_ALERT,		// 警告音
		LABEL_SE_DECIDE,	// 決定音
		LABEL_MAX
	}LABEL;
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
		float fAddVolume;// ボリューム
	} SOUNDPARAM;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Update(void);
	void Uninit(void);
	HRESULT PlaySound(int nIdy, LABEL label);	// 再生
	void Stop(int nIdy, LABEL label);		// Uninitで同じ名前で止める
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

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX][MAX_PLAYER] = {};	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[LABEL_MAX] = {};					// オーディオデータへのポインタ
	DWORD m_aSizeAudio[LABEL_MAX] = {};						// オーディオデータサイズ
	bool m_abFadeOut[LABEL_MAX][MAX_PLAYER];
	// 各音素材のパラメータ
	SOUNDPARAM m_aSoundParam[LABEL_MAX] =
	{
		{ "data/BGM/title.wav", -1 ,1.0f },		// タイトル
		{ "data/BGM/game.wav", -1 ,1.0f },		// セレクト
		{ "data/BGM/result.wav", -1 ,1.0f },	// ゲーム

		{ "data/SE/pose.wav",0,1.0f },		// ポーズをとるとき
		{ "data/SE/succes.wav",0,1.5f },	// 成功したとき
		{ "data/SE/failed.wav",0,1.0f },	// 失敗したとき
		{ "data/SE/alert.wav",0,1.0f },		// 警告
		{ "data/SE/decide.wav",0,1.0f }		// 警告
	};

};

#endif
