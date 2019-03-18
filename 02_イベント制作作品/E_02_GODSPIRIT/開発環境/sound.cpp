//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "sound.h"
//*****************************************************************************
// パラメータ構造体定義
//*****************************************************************************
typedef struct
{
	char *pFilename;	// ファイル名
	int nCntLoop;		// ループカウント(-1でループ再生)
	D3DXVECTOR3 pos;	// サウンドの位置
} SOUNDPARAM;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2オブジェクトへのポインタ
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// マスターボイスへのポインタ
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイスへのポインタ
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータへのポインタ
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

															// 各音素材のパラメータ
SOUNDPARAM g_aSoundParam[SOUND_LABEL_MAX] =
{
	{ "data/BGM/TitleBGM000.wav", -1 },		// タイトル
	{ "data/BGM/TutorialBGM000.wav",-1 },	// チュートリアル
	{ "data/BGM/GameBGM000.wav", -1 },		// ゲーム
	{ "data/BGM/GameBossBGM.wav", -1 },		// ゲーム
	{ "data/BGM/GameClearBGM.wav", -1 },			// クリア
	{ "data/BGM/GameOverBGM.wav", -1 },			// ゲームオーバー
	{ "data/BGM/RankingBGM.wav", -1 },			// ゲームオーバー
	{ "data/BGM/DeathBGM.wav", -1 },			// ゲームオーバー
	{ "data/SE/Decide.wav", 0 },				// 決定
	{ "data/SE/Select.wav", 0 },				// 選ぶ
	{ "data/SE/Pause001.wav", 0 },				// ポーズ
	{ "data/SE/ItemGet001.wav", 0 },			// アイテムゲット
	{ "data/SE/Hit001.wav", 0 },			// ヒット
	{ "data/SE/Hit002.wav", 0 },			// ヒット
	{ "data/SE/Lockon.wav", 0 },			// ロックオン
	{ "data/SE/WalkGrand.wav", -1 },				// 地面
	{ "data/SE/Explosion.wav", 0 },				// 爆発
	{ "data/SE/AtackWood.wav", 0 },			// 木を攻撃
	{ "data/SE/PlayerAttack.wav", 0 },			// 攻撃
	{ "data/SE/VOICE/V_DAMAGE.wav", 0 },			// ダメージ
	{ "data/SE/VOICE/V_DEATH.wav", 0 },			// 死ぬ
	{ "data/SE/VOICE/V_OK.wav", 0 },			// チュートリアル終わり
	{ "data/SE/GameClearUI.wav", 0 },				// クリア
	{ "data/SE/GameOverUI.wav", 0 },				// ゲームオーバ
	{ "data/SE/MessageWindow.wav", 0 },				// メッセージウィンドウ
//------------------------------------キック系----------------------------------------------//
	{ "data/SE/KickBasic.wav", 0 },													// キック基本
	{ "data/SE/KickKick.wav", 0 },													// キック→キック
	{ "data/SE/Samaso.wav", 0 },												// キック→キック→キック(サマーソルト)
	{ "data/SE/Volcano_cut.wav", 0 },												// キック→ジャンプ(ボルケーノカット)
	{ "data/SE/Slams.wav", 0 },														// キック→ジャンプ→パンチ(叩きつけ)
	{ "data/SE/Ultimate.wav", 0 },														// キック→ジャンプ→パンチ(叩きつけ2)
	{ "data/SE/SpinKick.wav", 0 },													// キック→ジャンプ→パンチ(回し蹴り)
//------------------------------------パンチ系----------------------------------------------//
	{ "data/SE/PunchBasic.wav", 0 },												// パンチ基本
	{ "data/SE/PunchPunch.wav", 0 },												// パンチ→パンチ
	{ "data/SE/RightStraight2.wav", 0 },												// パンチ→パンチ→パンチ(右ストレート)
	{ "data/SE/Syouryuuken.wav", 0 }	,											// パンチ→回転→ジャンプ(昇竜拳)
	{ "data/SE/Back_Fist.wav",  0 },													// パンチ→回転(裏拳)
//------------------------プレイヤー攻撃ヒット音汎用---------------------------------//
	{ "data/SE/PowerIncrease.wav", 0 },										//コンボ攻撃汎用(ヒット音)
//--------------------------------------------回転系-----------------------------------------//
	{ "data/SE/SpinBasic.wav", 0 },												// 回転基本
	{ "data/SE/FlameDance.wav", 0 }	,											// フレイムダンス
	{ "data/SE/Cartwheels.wav", 0 }	,											// 側転
	{ "data/SE/Break_Dance.wav", 0 }	,										// ブレイクダンス
//-------------------------------------着地、ステップ-------------------------------------//
	{ "data/SE/Randing.wav", 0 }	,												// 着地音
	{ "data/SE/Step3.wav", 0 }	,													// ステップ
//--------------------------------ジャンプ-------------------------------------------------------------//
	{ "data/SE/Step3.wav", 0 },														// ジャンプ基本
	{ "data/SE/Riderkick.wav", 0 }	,												// 飛び蹴り
	{ "data/SE/HammerPanch.wav", 0 }	,										// ハンマーパンチ
	{ "data/SE/KickWave.wav", 0 }	,												// 蹴撃波
//--------------------------------奪踊の舞--------------------------------------------------------------//
	{ "data/SE/PowerIncrease.wav", 0 }	,												// 奪踊の舞
//--------------------------------鎧の敵--------------------------------------------------------------//
	{ "data/SE/Armor_Action.wav", 0 }	,										// 鎧の敵_攻撃
	{ "data/SE/Armor_Walk.wav", 0 }	,										// 鎧の敵_歩行
//--------------------------------本ちゃん、ひげちゃんなど----------------------------------------//
	{ "data/SE/Enemy_Attack_Generic.wav", 0 }	,						// 敵攻撃汎用
	{ "data/SE/VOICE/V_DATUYOU.wav", 0 },			// 死ぬ
	{ "data/SE/VOICE/V_JUMP_PUNCH.wav", 0 },			// 死ぬ
	{ "data/SE/VOICE/V_KICK_JUMP_PUNCH.wav", 0 },			// 死ぬ
	{ "data/SE/VOICE/V_PUNCH.wav", 0 },			// 死ぬ
	{ "data/SE/VOICE/V_SPIN.wav", 0 },			// 死ぬ

};

//=============================================================================
// 初期化
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}

	// マスターボイスの生成
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (g_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(g_aSoundParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// ソースボイスの生成
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aSoundParam[nCntSound].nCntLoop;

		// オーディオバッファの登録
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSound(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);

			// ソースボイスの破棄
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;

			// オーディオデータの開放
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}

	// マスターボイスの破棄
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;

	if (g_pXAudio2)
	{
		// XAudio2オブジェクトの開放
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// セグメント再生(停止)
//=============================================================================
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
	{// 再生中
	 // 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	g_apSourceVoice[label]->Start(0);


	return S_OK;

}

//=============================================================================
// セグメント停止
//=============================================================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// セグメント停止
//=============================================================================
void StopSound(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);

			// オーディオバッファの削除
			g_apSourceVoice[nCntSound]->FlushSourceBuffers();
		}
	}
}

//=============================================================================
// チャンクのチェック
//=============================================================================
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

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}
//=============================================================================
// 音量変化
//=============================================================================
void ChangeVolume(SOUND_LABEL label, float fVolume)
{
	g_apSourceVoice[label]->SetVolume(fVolume);
}
//=============================================================================
// 半径チェック
//=============================================================================
void CheckPos(D3DXVECTOR3 pos)
{
	//bool bGetRadius = false;
	//for (int nCntPos = 0; nCntPos < 3; nCntPos++)
	//{
	//	float fPosX = (g_aSoundParam[SOUND_LABEL_SE_WATERFALL].pos.x - pos.x);
	//	float fPosZ = (g_aSoundParam[SOUND_LABEL_SE_WATERFALL].pos.z + nCntPos*500 - pos.z);
	//	float fLength = fPosX*fPosX + fPosZ*fPosZ;	// プレイヤーと敵の距離
	//	for (int nCntSound = 0; nCntSound < 100; nCntSound++)
	//	{
	//		float fRadius = float((3500 * 3500) / (nCntSound + 1));		// Active範囲
	//		if (fRadius > fLength &&
	//			-fRadius < fLength)
	//		{// 音量変化
	//			g_apSourceVoice[SOUND_LABEL_SE_WATERFALL]->SetVolume(nCntSound / 10.0f);
	//			bGetRadius = true;
	//		}
	//	}
	//	if (bGetRadius == false)
	//	{
	//		g_apSourceVoice[SOUND_LABEL_SE_WATERFALL]->SetVolume(0.0f);

	//	}
	//}
}