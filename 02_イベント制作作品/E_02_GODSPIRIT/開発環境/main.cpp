////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// デバッグ情報表示 [main.cpp]
// Author : RyotaKinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "sound.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "input.h"
#include "camera.h"
#include "fade.h"
#include "gamepad.h"
#include "player.h"
#include "model.h"
#include "meshField.h"
#include "meshWall.h"
#include "enemy.h"
#include "orbit.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define CLASS_NAME		"AppClass"				// ウインドウのクラス名
#define WINDOW_NAME		"GODSPIRIT"				// ウインドウのキャプション名
#define WINDOW_MODE		(TRUE)					// false最大化 trueウインドウ
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);
void DrawPOS(void);

//******************************************************************************************************************************
// グローバル変数:
//******************************************************************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Direct3Dデバイスへのポインタ
LPD3DXFONT			g_pFont = NULL;			// フォントへのポインタ
int					g_nCountFPS = 0;		// FPSカウンタ
MODE g_mode = MODE_TITLE;					// モード
D3DXMATRIX mView;
D3DXMATRIX mProj;
POINT g_Point;
bool g_bDebug[DRAW_DEBUG_MAX];
//==============================================================================================================================
// メイン関数
//==============================================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL, IDI_APPLICATION)
	};
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwExecLastTime;
	DWORD dwFrameCount;
	DWORD dwFPSLastTime;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindowEx(0,
							CLASS_NAME,
							WINDOW_NAME,
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							(rect.right - rect.left),
							(rect.bottom - rect.top),
							NULL,
							NULL,
							hInstance,
							NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// 各カウンターを初期化
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();		// システム時刻を取得(ミリ秒単位)
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();		// システム時刻を取得(ミリ秒単位)

	// ウインドウの表示(初期化処理の後に行う)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			// システム時刻を取得
			dwCurrentTime = timeGetTime();

			if((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
				// FPSを測定
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		// FPSを測定した時刻を保存
				dwFrameCount = 0;					// カウントをクリア
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 1/60秒経過
				dwExecLastTime = dwCurrentTime;		// 処理を実行した時刻を保存

				// 更新処理
				Update();

				// 描画処理
				Draw();

				// マウスカーソル
				GetPoint(hWnd);


				dwFrameCount++;		// カウントを加算
			}
		}
	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//==============================================================================================================================
// ウインドウプロシージャ
//==============================================================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_LBUTTONDOWN:


		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファフォーマットはディスプレイモードに合わせて使う
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = WINDOW_MODE;									// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF,
											hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}
	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	// デバッグ表示用フォントを設定
	// pDevice         : デバイスへのポインタ
	// Height          : 文字の高さ
	// Width           : 文字の幅
	// Weight          : フォントの太さ
	// MipLevels       : ミップマップレベル数
	// Italic          : イタリックフォント
	// CharSet         : フォントの文字セット
	// OutputPrecision : 実際のフォントと目的のフォントのサイズおよび特性の一致方法を指定
	// Quality         : 実際のフォントと目的のフォントとの一致方法を指定
	// PitchAndFamily  : ピッチとファミリインデックス
	// pFacename       : フォントの名前を保持する文字列
	// ppFont          : フォントへのポインタ
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);
	// サウンド初期化処理
	InitSound(hWnd);
	// キーボードの初期化処理
	InitKeyboard(hInstance, hWnd);
	// ゲームパッドの初期化設定
	InitGamePad(hInstance, hWnd);
	// フェード初期化処理
	InitFade(g_mode);
	// よみこみ
	InitOrbit();
	SetObjectFileData();
	EnemyLode();											// 敵のモデル&モーション情報をテキストファイルから取得
	SetPlayerFileData();
	// モードの設定
	SetMode(g_mode);

	for (int nDebug = 0; nDebug < DRAW_DEBUG_MAX; nDebug++)
	{
		g_bDebug[nDebug] = false;
	}
	return S_OK;
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void Uninit(void)
{
	if(g_pFont != NULL)
	{// デバッグ表示用フォントの開放
		g_pFont->Release();
		g_pFont = NULL;
	}
	// キーボードの終了処理
	UninitKeyboard();
	// ゲームパッドの終了処理
	UninitGamePad();
	// サウンドの終了処理
	UninitSound();
	// タイトルの終了処理
	UninitTitle();
	// チュートリアルの終了処理
	UninitTutorial();
	// ゲームの終了処理
	UninitGame();
	// リザルトの終了処理
	UninitResult();
	// ランキングの終了処理
	UninitRanking();
	// フェードの終了処理
	UninitFade();
	// 各種終了処理
	UninitMeshWall();
	UninitMeshField();
	UninitModel();
	UninitEnemy();
	UninitPlayer();
	UninitOrbit();
	if(g_pD3DDevice != NULL)
	{// デバイスの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void Update(void)
{

	// キーボードの更新処理
	UpdateKeyboard();
	// ゲームパッドの更新処理
	UpdateGamePad();
	if (GetKeyboardTrigger(DIK_F1) == true)
	{// デバッグ情報オンオフ
		g_bDebug[DRAW_DEBUG_POS] = g_bDebug[DRAW_DEBUG_POS] ? false : true;
	}
	if (GetKeyboardTrigger(DIK_F2) == true)
	{// ライン
		g_bDebug[DRAW_DEBUG_LINE]= g_bDebug[DRAW_DEBUG_LINE] ? false : true;
	}
	if (GetKeyboardTrigger(DIK_F3) == true)
	{// デバッグ情報オンオフ
		g_bDebug[DRAW_DEBUG_WIREFRAME] = g_bDebug[DRAW_DEBUG_WIREFRAME] ? false : true;
	}
	switch (g_mode)
	{
	case MODE_TITLE:	// タイトル
		UpdateTitle();
		break;
	case MODE_TUTORIAL:	// チュートリアル
		UpdateTutorial();
		break;
	case MODE_GAME:		// ゲーム
		UpdateGame();
		break;
	case MODE_RESULT:	// リザルト
		UpdateResult();
		break;
	case MODE_RANKING:	// ランキング
		UpdateRanking();
		break;
	}
	// フェードの更新処理
	UpdateFade();

}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);


	// Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		if (g_bDebug[DRAW_DEBUG_POS] == true)
		{
			// FPS表示
			DrawFPS();
			// 座標表示
			DrawPOS();
		}
		if (g_bDebug[DRAW_DEBUG_WIREFRAME] == true)
		{
			// ワイヤーフレーム
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else if (g_bDebug[DRAW_DEBUG_WIREFRAME] == false)
		{
			// ワイヤーフレーム
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}

		// 各種オブジェクトの描画処理
		switch (g_mode)
		{
		case MODE_TITLE:	// タイトル
			DrawTitle();
			break;
		case MODE_TUTORIAL:	// チュートリアル
			DrawTutorial();
			break;
		case MODE_GAME:		// ゲーム
			DrawGame();
			break;
		case MODE_RESULT:	// リザルト
			DrawResult();
			break;
		case MODE_RANKING:	// ランキング
			DrawRanking();
			break;
		}
		DrawFade();


		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==============================================================================================================================
// デバイスの取得
//==============================================================================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//=============================================================================
// モードの設定処理
//=============================================================================
void SetMode(MODE mode)
{
	// 各種終了処理
	switch (g_mode)
	{
	case MODE_TITLE:	// タイトル
		UninitTitle();
		StopSound();				// サウンド終了
		break;
	case MODE_TUTORIAL:	// チュートリアル
		UninitTutorial();
		StopSound();				// サウンド終了
		break;
	case MODE_GAME:		// ゲーム
		if (mode == MODE_RESULT)
		{

		}
		else
		{
			StopSound();				// サウンド終了
		}
		UninitGame();
		break;
	case MODE_RESULT:	// リザルト
		UninitResult();
		StopSound();				// サウンド終了
		break;
	case MODE_RANKING:	// ランキング
		UninitRanking();
		StopSound();				// サウンド終了
		break;
	}
	g_mode = mode;

	// 各種初期化処理
	switch (mode)
	{
	case MODE_TITLE:	// タイトル
		InitTitle();
		PlaySound(SOUND_LABEL_TITLEBGM);
		break;
	case MODE_TUTORIAL:	// チュートリアル
		PlaySound(SOUND_LABEL_TUTORIAL);
		InitTutorial();
		break;
	case MODE_GAME:		// ゲーム
		InitGame();
		PlaySound(SOUND_LABEL_GAMEBGM1);
		break;
	case MODE_RESULT:	// リザルト
		InitResult();
		break;
	case MODE_RANKING:	// ランキング
		InitRanking();
		PlaySound(SOUND_LABEL_RANKING);
		break;
	}
}
//=============================================================================
// デバイスの取得
//=============================================================================
MODE GetMode(void)
{
	return g_mode;
}
//=============================================================================
// マウスの獲得
//=============================================================================
void GetPoint(HWND hWnd)
{
	//マウスクリック座標算出処理
	if (GetKeyState(VK_LBUTTON) & 0x80)//マウス　左ボタン押下
	{
		GetCursorPos(&g_Point);//カーソル位置の絶対座標を取得
		ScreenToClient(hWnd, &g_Point);//カーソル絶対座標から、そのウィンドウ内の座標に補正
		g_Point.x = LONG((FLOAT)g_Point.x * ((FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_WIDTH));//さらにサイズ変更を考慮して、現在のサイズで補正（枠サイズ等あるので厳密ではない）
		g_Point.y = LONG((FLOAT)g_Point.y * ((FLOAT)SCREEN_HEIGHT / (FLOAT)SCREEN_HEIGHT));
	}

	//	D3DXMATRIX mWorldView;
	//	D3DXMATRIX m;
	//	D3DXVECTOR3 vRayDir;
	//	D3DXVECTOR3 vStartPoint;
	//	D3DXVECTOR3 v;

	//	//マウス座標をスクリーンの中心が原点になるように補正（正規化）
	//	v.x = (((2.0*g_Point.x) / (float)SCREEN_WIDTH) - 1) / mProj._11;
	//	v.y = -(((2.0*g_Point.y) / (float)SCREEN_HEIGHT) - 1) / mProj._22;
	//	v.z = 1.0;

	//	/*		mWorldView = XMesh[i].mWorld * mView;
	//	D3DXMatrixInverse(&m, NULL, &mWorldView);*/

	//	//ｖRayDirはレイを飛ばす方向ベクトル
	//	vRayDir.x = v.x*m._11 + v.y*m._21 + v.z*m._31;
	//	vRayDir.y = v.x*m._12 + v.y*m._22 + v.z*m._32;
	//	vRayDir.z = v.x*m._13 + v.y*m._23 + v.z*m._33;

	//	//vStartPointはスクリーン上の２D座標（クリック位置）を3D空間座標に変換したもの
	//	vStartPoint.x = m._41;
	//	vStartPoint.y = m._42;
	//	vStartPoint.z = m._43;

	//}
}
POINT *GetPoint(void)
{
	return &g_Point;
}
//==============================================================================================================================
// FPS表示処理
//==============================================================================================================================
void DrawFPS(void)
{
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	char aStr[256];

	// 文字列を代入
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	// テキスト描画
	// hDC      : デバイスコンテキストのハンドル
	// lpString : 描画する文字列へのポインタ
	// nCount   : 文字列の文字数
	// lpRect   : 長方形領域
	// uFormat  : テキストの整形方法を指定
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 0xff));
}
//==============================================================================================================================
// 頂点データ処理
//==============================================================================================================================
void DrawPOS(void)
{
	Camera *pCamera;
	pCamera = GetCamera();
	Player *pPlayer = GetPlayer();
	RECT rect = { 0, 40, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[512];

	// 文字列を代入
	wsprintf(&aStr[0], "プレイヤー位置\n%d %d %d  \n\nマウス座標\n%d %d\n\nカメラ座標\n%d %d %d\n\nカメラ回転\n%d %d %d\n\nF1:座標表示\n\nF2:ライン表示\n\nF3:ワイヤーフレーム",(int)pPlayer->pos.x, (int)pPlayer->pos.y, (int)pPlayer->pos.z,g_Point.x,g_Point.y,
		(int)pCamera->posV.x, (int)pCamera->posV.y, (int)pCamera->posV.z,(int)(pCamera->rot.x * 100.0f), (int)(pCamera->rot.y * 100.0f), (int)(pCamera->rot.z * 100.0f));

	// テキスト描画
	// hDC      : デバイスコンテキストのハンドル
	// lpString : 描画する文字列へのポインタ
	// nCount   : 文字列の文字数
	// lpRect   : 長方形領域
	// uFormat  : テキストの整形方法を指定
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(250, 250, 250, 0xff));
}
//==============================================================================================================================
// デバッグ情報表示オンオフ
//==============================================================================================================================
bool *GetDrawDebug(void)
{
	return &g_bDebug[0];
}
