/*******************************************************************************
* タイトル GM31 2Dポリゴン描画
* ファイル名 scene2D.cpp
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/04/12
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "airgauge.h"
#include "input.h"


bool CAirgauge::m_drawGaugeFlag = false;
LPDIRECT3DVERTEXBUFFER9 CAirgauge::m_pVtxBuffAirgauge = NULL;

/*******************************************************************************
* 関数名：CAirgauge
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CAirgauge::CAirgauge(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* 関数名：~CAirgauge
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CAirgauge::~CAirgauge( ) {

}


/*******************************************************************************
* 関数名：Init
* 引数：なし
* 戻り値：なし
* 説明：初期化処理
*******************************************************************************/

void CAirgauge::Init(void) {
	// 変数の宣言
	int i; // for文ループ用
	VERTEX_2D *pVtx;

	CRenderer *renderer; // デバイス取得用変数
	renderer = CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得

	// メンバ変数初期化
	m_TextureAirgauge = NULL;
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// テクスチャ設定
	D3DXCreateTextureFromFile(device, NULL, &m_TextureAirgauge);

	// 頂点バッファの生成
	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * AIRGAUGE_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffAirgauge, NULL);

	// 頂点情報へのポインタを取得
	m_pVtxBuffAirgauge -> Lock(0, 0, (void**)&pVtx, 0);

	// 座標設定
	pVtx[0].pos = D3DXVECTOR3(AIRGAUGE_X, AIRGAUGE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(AIRGAUGE_X + AIRGAUGE_WIDTH, AIRGAUGE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(AIRGAUGE_X, AIRGAUGE_Y + AIRGAUGE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(AIRGAUGE_X + AIRGAUGE_WIDTH, AIRGAUGE_Y + AIRGAUGE_HEIGHT, 0.0f);

	for(i = 0; i < AIRGAUGE_VERTEX_NUM; i++) {
		pVtx[i].rhw = 1.0f; // 係数設定

		pVtx[i].col = D3DCOLOR_RGBA(64, 64, 255, 255); // 頂点カラー設定
	}

	// テクスチャUV座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuffAirgauge -> Unlock( );

	m_drawGaugeFlag = false;
}


/*******************************************************************************
* 関数名：Uninit
* 引数：なし
* 戻り値：なし
* 説明：終了処理
*******************************************************************************/

void CAirgauge::Uninit(void) {
	if(m_TextureAirgauge != NULL) {
		m_TextureAirgauge -> Release( ); // テクスチャの開放
		m_TextureAirgauge = NULL;
	}
}


/*******************************************************************************
* 関数名：Update
* 引数：なし
* 戻り値：なし
* 説明：更新処理
*******************************************************************************/

void CAirgauge::Update(void) {

}


/*******************************************************************************
* 関数名：Draw
* 引数：なし
* 戻り値：なし
* 説明：描画処理
*******************************************************************************/

void CAirgauge::Draw(void) {
	if(m_drawGaugeFlag) {
		CRenderer *renderer; // デバイス取得用変数
		renderer= CManager::GetRenderer( ); // レンダラのアドレス取得
		LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得

		// 頂点バッファをデータストリームにバインド
		device -> SetStreamSource(0, m_pVtxBuffAirgauge, 0, sizeof(VERTEX_2D) );

		// 頂点フォーマットの設定
		device -> SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		device -> SetTexture(0, m_TextureAirgauge);

		// ライト設定オフ
//		device -> SetRenderState(D3DRS_LIGHTING, FALSE);

		// 2Dポリゴンの描画
		device -> DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			AIRGAUGE_TRIANGLE_NUM);

		// ライト設定オン
//		device -> SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}


/*******************************************************************************
* 関数名：Create
* 引数：scene2D : アドレス
* 戻り値：なし
* 説明：オブジェクトの生成
*******************************************************************************/

CAirgauge *CAirgauge::Create(void) {
	CAirgauge *scene2D;
	scene2D = new CAirgauge;
	scene2D -> Init( );
	return scene2D;
}


/*******************************************************************************
* 関数名：SetGauge
* 引数：height : ゲージの高さ
* 戻り値：なし
* 説明：ゲージの高さ設定
*******************************************************************************/

void CAirgauge::SetGauge(float height) {
	if(height >= 100) {
		m_drawGaugeFlag = false;
	}
	else {
		m_drawGaugeFlag = true;

		VERTEX_2D *pVtx;

		// 頂点情報へのポインタを取得
		m_pVtxBuffAirgauge -> Lock(0, 0, (void**)&pVtx, 0);

		// 座標設定
		pVtx[0].pos = D3DXVECTOR3(AIRGAUGE_X, AIRGAUGE_Y - height, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(AIRGAUGE_X + AIRGAUGE_WIDTH, AIRGAUGE_Y - height, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(AIRGAUGE_X, AIRGAUGE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(AIRGAUGE_X + AIRGAUGE_WIDTH, AIRGAUGE_Y, 0.0f);

		m_pVtxBuffAirgauge -> Unlock( );
	}
}