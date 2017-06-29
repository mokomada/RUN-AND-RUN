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
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"


/*******************************************************************************
* 関数名：CScene2D
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CScene2D::CScene2D(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* 関数名：~CScene2D
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CScene2D::~CScene2D( ) {

}


/*******************************************************************************
* 関数名：Init
* 引数：なし
* 戻り値：なし
* 説明：初期化処理
*******************************************************************************/

void CScene2D::Init(void) {
	// 変数の宣言
	int i; // for文ループ用
	VERTEX_2D *pVtx;

	CRenderer *renderer; // デバイス取得用変数
	renderer = CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得

	// メンバ変数初期化
	m_TexturePolygon = NULL;
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// テクスチャ設定
	D3DXCreateTextureFromFile(device, "data/TEXTURE/akira000.png", &m_TexturePolygon);

	// 頂点バッファの生成
	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_VertexBuffer, NULL);

	// 頂点情報へのポインタを取得
	m_VertexBuffer -> Lock(0, 0, (void**)&pVtx, 0);

	// 座標設定
	pVtx[0].pos = D3DXVECTOR3(POLYGON_X, POLYGON_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(POLYGON_X + POLYGON_WIDTH, POLYGON_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(POLYGON_X, POLYGON_Y + POLYGON_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(POLYGON_X + POLYGON_WIDTH, POLYGON_Y + POLYGON_HEIGHT, 0.0f);

	for(i = 0; i < VERTEX_NUM; i++) {
		pVtx[i].rhw = 1.0f; // 係数設定

		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 0); // 頂点カラー設定
	}

	// テクスチャUV座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_VertexBuffer -> Unlock( );
}


/*******************************************************************************
* 関数名：Uninit
* 引数：なし
* 戻り値：なし
* 説明：終了処理
*******************************************************************************/

void CScene2D::Uninit(void) {
	if(m_TexturePolygon != NULL) {
		m_TexturePolygon -> Release( ); // テクスチャの開放
		m_TexturePolygon = NULL;
	}
}


/*******************************************************************************
* 関数名：Update
* 引数：なし
* 戻り値：なし
* 説明：更新処理
*******************************************************************************/

void CScene2D::Update(void) {
	VERTEX_2D *pVtx;

	m_position.x += 0.1f;

	// 頂点情報へのポインタを取得
	m_VertexBuffer -> Lock(0, 0, (void**)&pVtx, 0);

	// 座標設定
	pVtx[0].pos += m_position;
	pVtx[1].pos += m_position;
	pVtx[2].pos += m_position;
	pVtx[3].pos += m_position;

	m_VertexBuffer -> Unlock( );



}


/*******************************************************************************
* 関数名：Draw
* 引数：なし
* 戻り値：なし
* 説明：描画処理
*******************************************************************************/

void CScene2D::Draw(void) {
	CRenderer *renderer; // デバイス取得用変数
	renderer= CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得

	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_VertexBuffer, 0, sizeof(VERTEX_2D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	device -> SetTexture(0, m_TexturePolygon);

	// ライト設定オフ
	device -> SetRenderState(D3DRS_LIGHTING, FALSE);

	// 2Dポリゴンの描画
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		TRIANGLE_NUM);

	// ライト設定オン
	device -> SetRenderState(D3DRS_LIGHTING, TRUE);

}


/*******************************************************************************
* 関数名：Create
* 引数：scene2D : アドレス
* 戻り値：なし
* 説明：オブジェクトの生成
*******************************************************************************/

CScene2D *CScene2D::Create(void) {
	CScene2D *scene2D;
	scene2D = new CScene2D;
	scene2D -> Init( );
	return scene2D;
}