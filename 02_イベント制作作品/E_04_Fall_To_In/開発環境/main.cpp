//=============================================================================
//
// メイン処理 [main.cpp]
// Author : Ryota Kinoshita
//
//=============================================================================
//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "main.h"
#include "manager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"		// ウインドウのクラス名
#define WINDOW_NAME		"Fall to In"	// ウインドウのキャプション名
#define WINDOW_MODE		(true)
//#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define malloc(X) _malloc_dbg(X,_NORMAL_BLOCK,__FILE__,__LINE__)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
int	g_nCountFPS;	// FPSカウンタ

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF  | _CRTDBG_LEAK_CHECK_DF);
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
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
	// マネージャの動的確保
	CManager *pManager = NULL;
	if (pManager == NULL)
	{
		pManager = new CManager;
	}
	// 初期化処理
	if (pManager != NULL)
	{
		if (FAILED(pManager->Init(hInstance, hWnd, FALSE)))
		{
			return-1;
		}
	}
	// 分解能を設定
	timeBeginPeriod(1);

	// フレームカウント初期化
	dwCurrentTime =
	dwFrameCount = 0;
	dwExecLastTime =
	dwFPSLastTime = timeGetTime();

	// ウインドウの表示
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
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();	// 現在の時間を取得
			if((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒ごとに実行
				// FPSを算出
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// 現在の時間を保存
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 1/60秒経過
				dwExecLastTime = dwCurrentTime;	// 現在の時間を保存

				if (pManager != NULL)
				{
					// 更新処理
					pManager->Update();
					// 描画処理
					pManager->Draw();
				}
				dwFrameCount++;
			}
		}
	}
	if (pManager != NULL)
	{// マネージャクラスの破棄
		// 終了処理
		pManager->Uninit();
		delete pManager;
		pManager = NULL;

	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	return (int)msg.wParam;
}

//=============================================================================
// ウインドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:				// [ESC]キーが押された
			DestroyWindow(hWnd);	// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// FPSカウンター取得
//=============================================================================
int GetCntFPS(void)
{
	return g_nCountFPS;
}
