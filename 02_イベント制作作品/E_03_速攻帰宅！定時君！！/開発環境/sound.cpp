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
	{ "data/BGM/Title000.wav", -1 },		// タイトル
	{ "data/BGM/M_Select000.wav", -1 },		// ゲーム
	{ "data/BGM/Select000.wav", -1 },		// ゲーム
	{ "data/BGM/Buttle000.wav", -1 },		// ゲーム
	{ "data/BGM/Buttle001.wav", -1 },		// ゲーム
	{ "data/BGM/Result000.wav", -1 },		// ゲーム
	{ "data/SE/共通SE/hit000.wav", 0 },		// ヒット000
	{ "data/SE/共通SE/hit001.wav", 0 },		// ヒット001
	{ "data/SE/共通SE/jump000.wav", 0 },	// ジャンプ
	{ "data/SE/共通SE/Land000.wav", 0 },	// 着地
	{ "data/SE/共通SE/Character_Move000.wav", 0 },	// 移動
	{ "data/SE/システム/GameEnd000.wav", 0 },		// ゲームエンド000
	{ "data/SE/システム/GaneSetting000.wav", 0 },	// ゲームセット000
	{ "data/SE/システム/GameStart000.wav", 0 },		//ゲームスタート000
	{ "data/SE/システム/GameStart001.wav", 0 },		// ゲームスタート001
	{ "data/SE/システム/TimeUp000.wav", 0 },		// タイムアップ000
	{ "data/SE/システム/Pause000.wav", 0 },		// ポーズ000
	{ "data/SE/システム/SystemSelect000.wav", 0 },		// ポーズ000
	//ロボット
	{ "data/SE/ロボット/ガード/Robot_Guard000.wav", 0 },	//ロボット　ガード
	{ "data/SE/ロボット/ぶっ飛び/Robot_Blow000.wav", 0 },	//ロボット　ぶっ飛び
	{ "data/SE/ロボット/回避/Robot_Avoid000.wav", 0 },		//ロボット　回避
	{ "data/SE/ロボット/攻撃/Robot_Attack000.wav", 0 },		//ロボット　攻撃 000
	{ "data/SE/ロボット/攻撃/Robot_Attack001.wav", 0 },		//ロボット　攻撃 001
	{ "data/SE/ロボット/攻撃/Robot_Attack002.wav", 0 },		//ロボット　攻撃002
	{ "data/SE/ロボット/攻撃/Robot_Attack003.wav", 0 },		//ロボット　攻撃003
	{ "data/SE/ロボット/攻撃/Robot_Attack004.wav", 0 },		//ロボット　攻撃004
	{ "data/SE/ロボット/攻撃/Robot_Attack005.wav", 0 },		//ロボット　攻撃005
	{ "data/SE/ロボット/戦闘不能/Robot_Down000.wav", 0 },  //ロボット　戦闘不能
	{ "data/SE/ロボット/投げる/Robot_Throw000.wav", 0 },	//ロボット　投げる
	{ "data/SE/ロボット/被ダメージ/Robot_Damage000.wav", 0 },// ロボット　被ダメージ
	{ "data/SE/ロボット/移動/Robot_Move000.wav", 0 },// ロボット　移動
	{ "data/SE/ロボット/ジャンプ/Robot_Jump000.wav", 0 },// ロボット　ジャンプ

	 //河童
	{ "data/SE/河童/ガード/Kappa_Guard000.wav", 0 }, // 河童　ガード
	{ "data/SE/河童/ぶっ飛び/Kappa_Blow000.wav", 0 },//河童　ぶっ飛び
	{ "data/SE/河童/回避/Kappa_Avoid000.wav", 0 },   // 河童　回避
	{ "data/SE/河童/攻撃/Kappa_Attack000.wav", 0 },  // 河童　攻撃000
	{ "data/SE/河童/攻撃/Kappa_Attack001.wav", 0 },  // 河童　攻撃001
	{ "data/SE/河童/攻撃/Kappa_Attack002.wav", 0 },  // 河童　攻撃002
	{ "data/SE/河童/攻撃/Kappa_Attack003.wav", 0 },  // 河童　攻撃003
	{ "data/SE/河童/攻撃/Kappa_Attack004.wav", 0 },  // 河童　攻撃004
	{ "data/SE/河童/攻撃/Kappa_Attack005.wav", 0 },  // 河童　攻撃005
	{ "data/SE/河童/戦闘不能/Kappa_Down000.wav", 0 },//河童　戦闘不能
	{ "data/SE/河童/戦闘不能/Kappa_Down001.wav", 0 },//河童　戦闘不能
	{ "data/SE/河童/投げる/Kappa_Throw000.wav", 0 }, // 河童　投げる
	{ "data/SE/河童/被ダメージ/Kappa_Damage000.wav", 0 },//河童　被ダメージ 000
	{ "data/SE/河童/被ダメージ/Kappa_Damage001.wav", 0 },//河童　被ダメージ 001
	{ "data/SE/河童/被ダメージ/Kappa_Damage002.wav", 0 },//河童　被ダメージ 002
	{ "data/SE/河童/勝利時/Kappa_Win000.wav", 0 },      //河童　勝利時
	{ "data/SE/河童/移動/Kappa_Move000.wav", 0 },      //河童　移動
	//人間
	{ "data/SE/人間/ガード/Girl_Guard000.wav", 0 },  // 人間　ガード
	{ "data/SE/人間/ガード/Girl_Guard001.wav", 0 },  // 人間　ガード
	{ "data/SE/人間/ぶっ飛び/Girl_Blow000.wav", 0 }, //人間　ぶっ飛び
	{ "data/SE/人間/回避/Girl_Avoid000.wav", 0 },    //人間　回避
	{ "data/SE/人間/攻撃/Girl_Attack000.wav", 0 },   //人間　攻撃  000
	{ "data/SE/人間/攻撃/Girl_Attack001.wav", 0 },   //人間　攻撃001
	{ "data/SE/人間/攻撃/Girl_Attack002.wav", 0 },   //人間　攻撃002
	{ "data/SE/人間/攻撃/Girl_Attack003.wav", 0 },   //人間　攻撃003
	{ "data/SE/人間/攻撃/Girl_Attack004.wav", 0 },   //人間　攻撃004
	{ "data/SE/人間/攻撃/Girl_Attack005.wav", 0 },   //人間　攻撃005
	{ "data/SE/人間/攻撃/Girl_Attack006.wav", 0 },   //人間　攻撃006
	{ "data/SE/人間/攻撃/Girl_Attack007.wav", 0 },   //人間　攻撃007
	{ "data/SE/人間/攻撃/Girl_Attack008.wav", 0 },   //人間　攻撃008
	{ "data/SE/人間/攻撃/Girl_Attack009.wav", 0 },   //人間　攻撃009
	{ "data/SE/人間/戦闘不能/Girl_Down000.wav", 0 },  //人間　戦闘不能
	{ "data/SE/人間/投げる/Girl_Throw000.wav", 0 },   //人間　投げる
	{ "data/SE/人間/被ダメージ/Girl_Damage000.wav", 0 },//人間 被ダメージ
	{ "data/SE/人間/勝利時/Girl_win.wav", 0 },      //人間 勝利時　

	//天狗
	{ "data/SE/天狗/ガード/Tengu_Guard000.wav", 0 },// 　天狗　ガード
	{ "data/SE/天狗/ぶっ飛び/Tengu_Blow000.wav", 0 },//　天狗　ぶっ飛び
	{ "data/SE/天狗/回避/Tengu_Avoid000.wav", 0 },// 　　天狗　回避
	{ "data/SE/天狗/攻撃/Tengu_Attack000.wav", 0 },// 　　天狗　攻撃000
	{ "data/SE/天狗/攻撃/Tengu_Attack001.wav", 0 },// 　　天狗　攻撃001
	{ "data/SE/天狗/攻撃/Tengu_Attack002.wav", 0 },// 　　天狗　攻撃002
	{ "data/SE/天狗/攻撃/Tengu_Attack003.wav", 0 },// 　　天狗　攻撃003
	{ "data/SE/天狗/攻撃/Tengu_Attack004.wav", 0 },// 　　天狗　攻撃004
	{ "data/SE/天狗/攻撃/Tengu_Attack005.wav", 0 },// 　　天狗　攻撃005
	{ "data/SE/天狗/戦闘不能/Tengu_Down000.wav", 0 },// 　天狗　戦闘不能 000
	{ "data/SE/天狗/戦闘不能/Tengu_Down001.wav", 0 },// 　天狗　戦闘不能 001
	{ "data/SE/天狗/投げる/Tengu_Throw000.wav", 0 },// 　　天狗　投げる
	{ "data/SE/天狗/被ダメージ/Tengu_Damage000.wav", 0 },//天狗　被ダメージ
	{ "data/SE/天狗/勝利時/Tengu_Win000.wav", 0 },//天狗　勝利時
	//狐
	{ "data/SE/狐/ガード/Fox_Guard000.wav", 0 },// 　狐　ガード
	{ "data/SE/狐/ぶっ飛び/Fox_Blow000.wav", 0 },//　狐　ぶっ飛び
	{ "data/SE/狐/回避/Fox_Avoid000.wav", 0 },// 　　狐　回避
	{ "data/SE/狐/攻撃/Fox_Attack000.wav", 0 },// 　　狐　攻撃000
	{ "data/SE/狐/攻撃/Fox_Attack001.wav", 0 },// 　　狐　攻撃001
	{ "data/SE/狐/攻撃/Fox_Attack002.wav", 0 },// 　　狐　攻撃002
	{ "data/SE/狐/攻撃/Fox_Attack003.wav", 0 },// 　　狐　攻撃003
	{ "data/SE/狐/攻撃/Fox_Attack004.wav", 0 },// 　　狐　攻撃004
	{ "data/SE/狐/攻撃/Fox_Attack005.wav", 0 },// 　　狐　攻撃005
	{ "data/SE/狐/戦闘不能/Fox_Down000.wav", 0 },// 　狐　戦闘不能
	{ "data/SE/狐/投げる/Fox_Throw000.wav", 0 },// 　　狐　投げる
	{ "data/SE/狐/被ダメージ/Fox_Damage000.wav", 0 },//狐　被ダメージ
	{ "data/SE/狐/勝利時/Fox_Win000.wav", 0 },     //狐　勝利時　
	// 戦闘系
	{ "data/SE/戦闘周辺SE/Tengu_Wind000.wav", 0 },// 風
	{ "data/SE/戦闘周辺SE/Tengu_Wing001.wav", 0 },// 風
	{ "data/SE/戦闘周辺SE/Slash000.wav", 0 },// ひっかく000
	{ "data/SE/戦闘周辺SE/Slash001.wav", 0 },// ひっかく001
	{ "data/SE/戦闘周辺SE/Slash002.wav", 0 },// ひっかく002
	{ "data/SE/戦闘周辺SE/Fox_Flame000.wav", 0 },// 火炎放射 000
	{ "data/SE/戦闘周辺SE/Fox_Flame001.wav", 0 },// 火炎放射　001
	{ "data/SE/戦闘周辺SE/bomb1.wav", 0 },// 爆発　000
	{ "data/SE/戦闘周辺SE/bomb2.wav", 0 },// 爆発　001
	{ "data/SE/戦闘周辺SE/Guard.wav", 0 },// ガード
	{ "data/SE/戦闘周辺SE/GuardBreak.wav", 0 },// ガードクラッシュ
	// 太鼓　
	{ "data/SE/taiko01.wav", 0 },// 太鼓音
	{ "data/SE/taiko02.wav", 0 },// 太鼓音
	{ "data/SE/taiko03.wav", 0 },// 太鼓音
	{ "data/SE/taiko04.wav", 0 },// 太鼓音
	// 歓声
	{ "data/SE/歓声/Cheer000.wav", 0 },// 歓声
	{ "data/SE/歓声/Cheer001.wav", 0 },// 歓声
	// システム
	{ "data/SE/Select.wav", 0 },// 移動音
	{ "data/SE/Decision001.wav", 0 },// 決定音
	{ "data/SE/Decision000.wav", 0 },// 決定音
	{ "data/SE/必殺技用SE/CutIn.wav", 0 },// カットイン
	{ "data/SE/必殺技用SE/SpecialLogo.wav", 0 },// SPECIAL
	{ "data/SE/CharacterSelect.wav", 0 },// キャラクター選択
	{ "data/SE/SelectInfo.wav", 0 },// 準備完了
	{ "data/SE/Alert000.wav", 0 },// 警告音
	{ "data/SE/Event_Thunder.wav", 0 },// 雷
	{ "data/SE/Event_Rain.wav", 1 },// 雨
	{ "data/SE/Event_Flood.wav", 1 },// 氾濫
	{ "data/SE/LifeUp000.wav", 0 },// UP
	{ "data/SE/AttackUp000.wav", 0 },// UP
	{ "data/SE/SpeedUp000.wav", 0 },// UP
	{ "data/SE/Event_Meteo.wav", -1 },// メテオ
	{ "data/SE/Event_Explosion.wav", 0 },// 爆発
	{ "data/SE/Event_Gust.wav", 0 },// 突風
	{ "data/SE/TimeLimit.wav", 0 },// 突風

	{ "data/SE/result2.wav", 0 },		// 突風
	{ "data/SE/houseHappy_se.wav", 0 },// 突風
	{ "data/SE/teizida_se.wav", 0 },		// 突風
	{ "data/SE/sokkoukitaku_se.wav", 0 },// 突風glass_break2
	{ "data/SE/glass_break2.wav", 0 },// 突風
	{ "data/SE/hit_1.wav", 0 },// 突風
	{ "data/SE/chair.wav", -1 },// 突風

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