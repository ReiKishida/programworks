//=============================================================================
//
// サウンド処理 [sound.h]
// Author :Kishida Rei
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

class CSound
{
public:
	typedef enum
	{
		SOUND_LABEL_BGM_TITLE = 0,		// BGM(TITLE)
		SOUND_LABEL_BGM_MENU,			// BGM(MENU)
		SOUND_LABEL_BGM_TUTORIAL,		// BGM(TUTORIAL)
		SOUND_LABEL_BGM_GAME,			// BGM(GAME)
		SOUND_LABEL_BGM_RESULT,			// BGM(RESULT)
		SOUND_LABEL_SE_SHOT,			// ショット
		SOUND_LABEL_SE_ADDTIME,			// タイム加算
		SOUND_LABEL_FORCE,				// フォースセレクト
		SOUND_LABEL_SE_DECIDE,			// 決定
		SOUND_LABEL_SE_CURSOR,			// カーソル
		SOUND_LABEL_MAX
	} SOUND_LABEL;
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;


	CSound();	//コンストラクタ
	~CSound();	//デストラクタ
	HRESULT InitSound(HWND hWnd);	//初期化
	void UninitSound(void);			//終了処理
	HRESULT PlaySound(SOUND_LABEL label);//サウンドの再生
	void StopSound(SOUND_LABEL label);//サウンドの停止
	void StopSound(void);//サウンドの停止
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータへのポインタ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

	// 各音素材のパラメータ
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/title.wav", -1 },		// BGM0
		{ "data/BGM/menu.wav", -1 },		// BGM0
		{ "data/BGM/tutorial.wav", -1 },		// BGM0
		{ "data/BGM/game.wav", -1 },		// BGM1
		{ "data/BGM/result.wav",0 },		// BGM1
		{ "data/SE/shoot.wav", 0 },			// ショット
		{ "data/SE/addtime.wav", 0 },		// タイム加算
		{ "data/SE/forceselect.wav", 0 },	// タイム加算
		{ "data/SE/decide.wav", 0 },		// タイム加算
		{ "data/SE/cursor.wav", 0 }			// カーソル
	};


};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#endif
