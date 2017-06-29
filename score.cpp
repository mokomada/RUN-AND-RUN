/*******************************************************************************
* タイトル スコア描画処理
* ファイル名 score.cpp
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/06/24
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "score.h"
#include "input.h"


/*******************************************************************************
* スタティック変数の生成
*******************************************************************************/

int CScore::m_nScore = 0;


/*******************************************************************************
* 関数名：CScore
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CScore::CScore(int priority, OBJTYPE objType){

}


/*******************************************************************************
* 関数名：~CScore
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CScore::~CScore( ) {

}


/*******************************************************************************
* 関数名：InitScore
* 引数：なし
* 戻り値：なし
* 説明：スコアの初期化
*******************************************************************************/

void CScore::Init(void) {
	/* 変数宣言 */
	int nCountVer, nCountBox, nCountDigit, nNumDigit; // for文ループ用
	VERTEX_2D *pVtx;

	/* メンバ変数の初期化 */
	m_pTextureScore = NULL; // スコア用テクスチャへのポインタ
	m_pVtxBuffScore = NULL; //頂点バッファへのポインタ

	/* 桁数設定 */
	nNumDigit = 1;
	for(nCountDigit = 0; nCountDigit < NUMBER_NUM; nCountDigit++) {
		nNumDigit *= 10;
	}

	/* 変数の取得 */
	CRenderer *renderer; // デバイス取得用変数
	renderer = CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得

	/* スコアテクスチャ設定 */
	D3DXCreateTextureFromFile(device, SCORE_TEXTURE_NAME, &m_pTextureScore);

	/* 頂点バッファの生成 */
	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * SCORE_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffScore, NULL);

	/* 頂点情報へのポインタを取得 */
	m_pVtxBuffScore -> Lock(0, 0, (void**)&pVtx, 0);

	/* 座標設定 */
	for(nCountBox = 0; nCountBox < NUMBER_NUM; nCountBox++) {
		pVtx[(nCountBox * 4) + 0].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) + nCountBox * BOX_WIDTH, 0.0f, 0.0f);
		pVtx[(nCountBox * 4) + 1].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) +  (nCountBox + 1) * BOX_WIDTH, 0.0f, 0.0f);
		pVtx[(nCountBox * 4) + 2].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) + nCountBox * BOX_WIDTH, BOX_HEIGHT, 0.0f);
		pVtx[(nCountBox * 4) + 3].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) +  (nCountBox + 1) * BOX_WIDTH, BOX_HEIGHT, 0.0f);
	}

	for(nCountVer = 0; nCountVer < SCORE_VERTEX_NUM; nCountVer++) {
		pVtx[nCountVer].rhw = 1.0f;

		pVtx[nCountVer].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	/* テクスチャUV座標設定 */
	for(nCountBox = 0; nCountBox < NUMBER_NUM; nCountBox++) {
		pVtx[(nCountBox * 4) + 0].tex = D3DXVECTOR2( (float) ( ( (m_nScore % nNumDigit) / (nNumDigit / 10) ) * TEX_LENGTH), 0.0f);
		pVtx[(nCountBox * 4) + 1].tex = D3DXVECTOR2( (float) ( ( (m_nScore % nNumDigit) / (nNumDigit / 10) + 1) * TEX_LENGTH), 0.0f);
		pVtx[(nCountBox * 4) + 2].tex = D3DXVECTOR2( (float) ( ( (m_nScore % nNumDigit) / (nNumDigit / 10) ) * TEX_LENGTH), 1.0f);
		pVtx[(nCountBox * 4) + 3].tex = D3DXVECTOR2( (float) ( ( (m_nScore % nNumDigit) / (nNumDigit / 10) + 1) * TEX_LENGTH), 1.0f);
		nNumDigit /= 10;
	}

	m_pVtxBuffScore -> Unlock( );

	m_nScore = 0;
}


/*******************************************************************************
* 関数名：UninitScore
* 引数：なし
* 戻り値：なし
* 説明：スコアの終了処理
*******************************************************************************/

void CScore::Uninit(void) {
	if(m_pTextureScore != NULL) {
		m_pTextureScore -> Release( ); // テクスチャの開放
		m_pTextureScore = NULL;
	}

	if(m_pVtxBuffScore != NULL) {
		m_pVtxBuffScore -> Release( );
		m_pVtxBuffScore = NULL;
	}
}


/*******************************************************************************
* 関数名：UpdateScore
* 引数：なし
* 戻り値：なし
* 説明：スコアの情報更新
*******************************************************************************/

void CScore::Update(void) {
	/* 変数宣言 */
	int score, nCountBox, nCountDigit, nNumDigit; // for文ループ用
	VERTEX_2D *pVtx;

	/* 移動時スコア処理 
	if(GetKeyboardPress(DIK_A) ) {
		if(GetKeyboardPress(DIK_W) ) { // 左上移動
			m_nScore += 100;
		}
		else if(GetKeyboardPress(DIK_S) ) { // 左下移動
			m_nScore += 100;
		}
		else { // 左移動
			m_nScore += 1;
		}
	}
	else if(GetKeyboardPress(DIK_D) ) {
		if(GetKeyboardPress(DIK_W) ) { // 右上移動
			m_nScore += 100;
		}
		else if(GetKeyboardPress(DIK_S) ) { // 右下移動
			m_nScore += 100;
		}
		else { // 右移動
			m_nScore += 1;
		}
	}
	else if(GetKeyboardPress(DIK_W) ) { // 上移動
			m_nScore += 1;
	}
	else if(GetKeyboardPress(DIK_S) ) { // 下移動
			m_nScore += 1;
	}*/

	/* 回転時スコア処理 
	if(GetKeyboardRepeat(DIK_LEFT) ) { // 左回転
		m_nScore += 10000;
	}

	if(GetKeyboardRepeat(DIK_RIGHT) ) { // 右回転
		m_nScore -= 10000;
	}*/

	/* 拡縮時のスコア処理 
	if(GetKeyboardTrigger(DIK_UP) ) { // 拡大
		m_nScore += 1000000;
	}
	
	if(GetKeyboardTrigger(DIK_DOWN) ) { // 縮小
		m_nScore -= 1000000;
	}*/

	m_nScore++;

	score = m_nScore / 60;

	/* 上限/下限処理 */
	if(m_nScore < SCORE_MIN) {
		m_nScore = SCORE_MIN;
	}
	if(m_nScore > SCORE_MAX) {
		m_nScore = SCORE_MAX;
	}

	/* 桁数設定 */
	nNumDigit = 1;
	for(nCountDigit = 0; nCountDigit < NUMBER_NUM; nCountDigit++) {
		nNumDigit *= 10;
	}

	m_pVtxBuffScore -> Lock(0, 0, (void**)&pVtx, 0);
	/* 数値の更新 */
	for(nCountBox = 0; nCountBox < NUMBER_NUM; nCountBox++) {
		pVtx[(nCountBox * 4) + 0].tex = D3DXVECTOR2( (float) ( ( (score % nNumDigit) / (nNumDigit / 10) ) * TEX_LENGTH), 0.0f);
		pVtx[(nCountBox * 4) + 1].tex = D3DXVECTOR2( (float) ( ( (score % nNumDigit) / (nNumDigit / 10) + 1) * TEX_LENGTH), 0.0f);
		pVtx[(nCountBox * 4) + 2].tex = D3DXVECTOR2( (float) ( ( (score % nNumDigit) / (nNumDigit / 10) ) * TEX_LENGTH), 1.0f);
		pVtx[(nCountBox * 4) + 3].tex = D3DXVECTOR2( (float) ( ( (score % nNumDigit) / (nNumDigit / 10) + 1) * TEX_LENGTH), 1.0f);
		nNumDigit /= 10;
	}

	m_pVtxBuffScore -> Unlock( );
}


/*******************************************************************************
* 関数名：DrawScore
* 引数：なし
* 戻り値：なし
* 説明：スコアの描画処理
*******************************************************************************/

void CScore::Draw(void) {
	// 変数の取得
	CRenderer *renderer; // デバイス取得用変数
	renderer = CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得

	/* 頂点バッファをデータストリームにバインド */
	device -> SetStreamSource(0, m_pVtxBuffScore, 0, sizeof(VERTEX_2D) );

	/* 頂点フォーマットの設定 */
	device -> SetFVF(FVF_VERTEX_2D);

	/* テクスチャの設定 */
	device -> SetTexture(0, m_pTextureScore);

	/* ポリゴンの描画 */
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		SCORE_TRIANGLE_NUM);
}


/*******************************************************************************
* 関数名：AddScore
* 引数：なし
* 戻り値：なし
* 説明：スコアの描画処理
*******************************************************************************/

void CScore::AddScore(int score) {
	m_nScore += score;
}


/*******************************************************************************
* 関数名：GetScore
* 引数：なし
* 戻り値：なし
* 説明：スコアの獲得
*******************************************************************************/

int CScore::GetScore(void) {
	return (m_nScore / 60);
}


/*******************************************************************************
* 関数名：Create
* 引数：CScore : アドレス
* 戻り値：なし
* 説明：オブジェクトの生成
*******************************************************************************/

CScore *CScore::Create(void) {
	CScore *score;
	score = new CScore;
	score -> Init( );
	return score;
}