//***************************************************************************//
//
// サウンド処理 [sound.cpp]
// Author : TEAM_Fascinator
//
//***************************************************************************//
#include "sound.h"

//***************************************************************************//
// Structure definition(SoundParam)
//***************************************************************************//
typedef struct
{
	char *pFilename;	// ファイル名
	int nCntLoop;		// ループカウント(-1でループ再生)
} SOUNDPARAM;

//***************************************************************************//
// プロトタイプ宣言(Sound)
//***************************************************************************//
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//***************************************************************************//
// グローバル変数(Sound)
//***************************************************************************//
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2オブジェクトへのポインタ
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// マスターボイスへのポインタ
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイスへのポインタ
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータへのポインタ
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

// 各音素材のパラメータ
SOUNDPARAM g_aSoundParam[SOUND_LABEL_MAX] =
{
	{ "data/SOUND/BGM/Title.wav", -1 },				// BGM0(Title)	-1はループ,0は単発
	{ "data/SOUND/BGM/C-Select.wav", -1 },			// BGM1(C_Select)
	{ "data/SOUND/BGM/Gallery.wav", -1 },			// BGM2(Gallery)
	{ "data/SOUND/BGM/Prologue.wav", -1 },			// BGM3(Option)
	{ "data/SOUND/BGM/Dungeon.wav", -1 },			// BGM4(Game)
	{ "data/SOUND/BGM/BossBattle-Custom.wav", -1 },	// BGM5(Boss)
	{ "data/SOUND/BGM/Result4.wav", -1 },			// BGM6(Result75)
	{ "data/SOUND/BGM/Result3.wav", -1 },			// BGM7(Result50)
	{ "data/SOUND/BGM/Result2.wav", -1 },			// BGM8(Result25)
	{ "data/SOUND/BGM/TrueClear.wav", -1 },			// BGM9(TrueClear)
	{ "data/SOUND/SE/Title Decision.wav", 0 },		// タイトル決定
	{ "data/SOUND/SE/Tutorial Decision.wav", 0 },	// チュートリアル決定
	{ "data/SOUND/SE/cursor.wav", 0 },		// 選択
	{ "data/SOUND/SE/Decision Sound.wav", 0 },		// 決定
	{ "data/SOUND/SE/Cancel.wav", 0 },				// キャンセル
	{ "data/SOUND/SE/Footstep.wav", 0 },				// 足音
	{ "data/SOUND/SE/dead.wav", 0 },				// 死亡時
	{ "data/SOUND/SE/salsh.wav", 0 },				// 剣撃
	{ "data/SOUND/SE/fire.wav", 0 },				// 炎
	{ "data/SOUND/SE/slash2.wav", 0 },				// 剣撃2
	{ "data/SOUND/SE/hit.wav", 0 },					// 剣撃2
	{ "data/SOUND/SE/hit2.wav", 0 },					// 剣撃2
	{ "data/SOUND/SE/bomb2.wav", 0 },					// 剣撃2
	{ "data/SOUND/SE/hit3.wav", 0 },					// 剣撃2
	{ "data/SOUND/SE/mekuri.wav", 0 },					// 剣撃2
};	

//===========================================================================//
// 初期化処理(Sound)
//===========================================================================//
HRESULT InitSound(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリ初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクト作成
	hr = XAudio2Create(&g_pXAudio2, 0);

	// COMライブラリの終了処理
	if (FAILED(hr)) { MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING); CoUninitialize(); return E_FAIL; } 

    // マスターボイス生成
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		// XAudio2オブジェクト開放
		if (g_pXAudio2) { g_pXAudio2->Release(); g_pXAudio2 = NULL; } 

		// COMライブラリ終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータ初期化
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイル生成
		hFile = CreateFile(g_aSoundParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{	// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr)) { MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING); return S_FALSE; }

		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr)) { MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING); return S_FALSE; }

		if (dwFiletype != 'EVAW') { MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING); return S_FALSE; }

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr)) { MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING); return S_FALSE; }

		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr)) { MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING); return S_FALSE; }

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr)) { MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING); return S_FALSE; }

		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);

		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr)) { MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING); return S_FALSE; }

		// ソースボイス生成
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr)) { MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING); return S_FALSE; }

		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aSoundParam[nCntSound].nCntLoop;

		// オーディオバッファ登録
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
	}

	return S_OK;
}

//===========================================================================//
// 終了処理(Sound)
//===========================================================================//
void UninitSound(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);

			// ソースボイス破棄
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;

			// オーディオデータ開放
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}

	// マスターボイス破棄
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;

	// XAudio2オブジェクト開放
	if (g_pXAudio2) { g_pXAudio2->Release(); g_pXAudio2 = NULL; }

	// COMライブラリ終了処理
	CoUninitialize();
}

//===========================================================================//
// セグメント再生(停止)
//===========================================================================//
HRESULT PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aSoundParam[label].nCntLoop;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{	// 再生中
		// 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファ削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファ登録
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	g_apSourceVoice[label]->Start(0);

	return S_OK;
}

//===========================================================================//
// セグメント停止
//===========================================================================//
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{	// 再生中
		// 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファ削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//===========================================================================//
// セグメント停止
//===========================================================================//
void StopSound(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);

			// オーディオバッファ削除
			g_apSourceVoice[nCntSound]->FlushSourceBuffers();
		}
	}
}

//===========================================================================//
// チャンクのチェック
//===========================================================================//
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	// ファイルポインタを先頭に移動
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) { return HRESULT_FROM_WIN32(GetLastError()); } 

	while (hr == S_OK)
	{
		// チャンク読み込み
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0) { hr = HRESULT_FROM_WIN32(GetLastError()); } 

		// チャンクデータ読み込み
		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0) { hr = HRESULT_FROM_WIN32(GetLastError()); } 

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;

			// ファイルタイプ読み込み
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0) { hr = HRESULT_FROM_WIN32(GetLastError()); } 
			break;

		default:
			// ファイルポインタをチャンクデータ分移動
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER) { return HRESULT_FROM_WIN32(GetLastError()); } 
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format) { *pChunkSize = dwChunkDataSize; *pChunkDataPosition = dwOffset; return S_OK; }

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize) { return S_FALSE; }
	}

	return S_OK;
}

//===========================================================================//
// チャンクデータ読み込み
//===========================================================================//
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	// ファイルポインタを指定位置まで移動
	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) { return HRESULT_FROM_WIN32(GetLastError()); } 

	// データ読み込み
	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0) { return HRESULT_FROM_WIN32(GetLastError()); } 

	return S_OK;
}