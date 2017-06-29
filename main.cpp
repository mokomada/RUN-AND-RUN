/*******************************************************************************
* タイトル GM31 2Dポリゴン描画
* ファイル名 main.cpp
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/04/13
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "debugProc.h"


/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define ID_TIMER (121) // タイマーのID


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);


/*******************************************************************************
* グローバル変数
*******************************************************************************/

int FPS = 0;


/*******************************************************************************
* 関数名：main
* 引数：なし
* 戻り値：(int)msg.wParam
* 説明：メッセージボックス表示
*******************************************************************************/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH) (COLOR_WINDOW + 1),
		NULL,
		"2D",
		NULL
	};

	HWND hWnd;
	MSG msg;

	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	RegisterClassEx(&wcex);

	hWnd = CreateWindowEx(
		0,
		"2D",
		"　",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		hInstance,
		NULL);
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	CManager::Init(hInstance, hWnd, TRUE);

	while(1) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
			if(msg.message == WM_QUIT) {
				break;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
#ifdef _DEBUG
			// ＦＰＳ表示用変数
			static int nFPSCount = 0; // フレーム数のカウント
			static int nFPSDrawWait = 0; // 次回のＦＰＳ表示更新までの時間
			char szFPS[256];

			// ＦＰＳ制御用
			static int nLastTime = 0; // 次回の更新時間
			int nNewTime = timeGetTime(); // 現在の時間

			// 更新
			if(nLastTime <= nNewTime)
			{
				nLastTime = nNewTime + 16; // １６ｍｓに一度更新する（約６０ＦＰＳ）
				nFPSCount++; // フレーム数カウント追加
				// ＦＰＳ表示更新
				if(nFPSDrawWait <= nNewTime)
				{
					FPS = (int)(nFPSCount * 1000) / (nNewTime - nFPSDrawWait + 1000);
					nFPSDrawWait = nNewTime + 1000;	//次回ＦＰＳ表示更新は１０００ｍｓ（１秒）後
					nFPSCount = 0;	//フレーム数カウントリセット
				}
			}
#endif

			CManager::Update( );
			CManager::Draw( );
		}
	}

	CManager::Uninit( );

	UnregisterClass("2D", wcex.hInstance);
	return (int)msg.wParam;
}


/*******************************************************************************
* 関数名：WndProc
* 引数：hWnd, uMsg, wParam, lParam
* 戻り値：DefWindowProc(hWnd, uMsg, wParam, lParam)
* 説明：ウインドウの設定
*******************************************************************************/

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	int nID; // メッセージボックスからの戻り値保存用

	switch(uMsg) {
	case WM_DESTROY: // ウインドウが破棄された
		PostQuitMessage(0);
		break;

	case WM_CLOSE: // バツボタンが押された時の処理
			nID = MessageBox(hWnd, "終了してよろしいですか？", "プログラムの終了", MB_YESNO);
			if(nID == IDNO) {
				return 0;
			}
		break;

	case WM_CREATE: // ウインドウが生成された

		SetTimer(hWnd, ID_TIMER, 1000/60, NULL);

		break;

	case WM_KEYDOWN: // キーが押された
		switch(wParam){
		case VK_ESCAPE: // Escキー
			nID = MessageBox(hWnd, "終了してよろしいですか？", "プログラムの終了", MB_YESNO);
			if(nID == IDYES) {
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_LBUTTONDOWN: // マウス左クリック
		SetFocus(hWnd);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


/*******************************************************************************
* 関数名：Init
* 引数：hInstance, hWnd, bWindow
* 戻り値：E_FAIL(処理失敗時) or S_OK(処理成功時)
* 説明：初期化処理
*******************************************************************************/

void Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow) {

}

/*******************************************************************************
* 関数名：Uninit
* 引数：なし
* 戻り値：なし
* 説明：終了処理
*******************************************************************************/

void Uninit(void) {

}

/*******************************************************************************
* 関数名：Update
* 引数：なし
* 戻り値：なし
* 説明：更新処理
*******************************************************************************/

void Update(void) {

}


/*******************************************************************************
* 関数名：Draw
* 引数：なし
* 戻り値：なし
* 説明：描画処理
*******************************************************************************/

void Draw(void) {

}


/*******************************************************************************
* 関数名：DrawFPS
* 引数：なし
* 戻り値：なし
* 説明：FPSを表示
*******************************************************************************/

void DrawFPS(void) {
//	char aStr[1024];
	// 文字設定
//	sprintf(&aStr[0], "[カメラの視点       : (%3.2f : %3.2f : %3.2f)]\n[カメラの注視点     : (%3.2f : %3.2f : %3.2f)]\n[カメラの向き       : (%3.2f)]\n\n*** カメラ操作 ***\n視点上昇　　 ： [ Ｙ ]\n視点下降　　 ： [ Ｎ ]\n注視点上昇　 ： [ Ｔ ]\n注視点下降　 ： [ Ｂ ]\nズームイン　 ： [ Ｕ ]\nズームアウト ： [ Ｍ ]\n\n*** 飛行機操作 ***\n[飛行機の位置 : (%3.2f : %3.2f : %3.2f)]\n前移動 ： [ Ｗ ]\n後移動 ： [ Ｓ ]\n左移動 ： [ Ａ ]\n右移動 ： [ Ｄ ]\n上昇　 ： [ Ｉ ]\n下降　 ： [ Ｋ ]\n\n[飛行機の向き : (%3.2f)]\n左旋回 : [ LSHIFT ]\n右旋回 : [ RSHIFT ]\n\n弾発射 : [ SPACE ]\n", camera.posV.x, camera.posV.y, camera.posV.z, camera.posR.x, camera.posR.y, camera.posR.z, camera.rot.y * D3DX_PI, model.pos.x, model.pos.y, model.pos.z, model.rot.y);
	// 表示
	PrintDebugProc("FPS:%d\n", FPS);
}