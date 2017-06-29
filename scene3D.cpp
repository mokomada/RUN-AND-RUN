/*******************************************************************************
* タイトル GM31 3Dポリゴン描画
* ファイル名 scene3D.cpp
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/04/12
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene3D.h"
#include "manager.h"
#include "input.h"


/*******************************************************************************
* 関数名：CScene3D
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CScene3D::CScene3D(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* 関数名：~CScene3D
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CScene3D::~CScene3D( ) {

}


/*******************************************************************************
* 関数名：Init
* 引数：なし
* 戻り値：なし
* 説明：初期化処理
*******************************************************************************/

void CScene3D::Init(void) {
	// 変数の宣言
	int i; // for文ループ用
	VERTEX_3D *pVtx;
	D3DXVECTOR3 n0, v1, v2;

	CRenderer *renderer; // デバイス取得用変数
	renderer = CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得

	// メンバ変数初期化
	m_TexturePolygon = NULL;
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// テクスチャ設定
	D3DXCreateTextureFromFile(device, FIELD_TEXTURE_NAME, &m_TexturePolygon);

	// 頂点バッファの生成
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_VertexBuffer, NULL);

	// 頂点情報へのポインタを取得
	m_VertexBuffer -> Lock(0, 0, (void**)&pVtx, 0);

	// 座標設定
	pVtx[0].pos = D3DXVECTOR3(FIELD_X, 0.0f, FIELD_Z + FIELD_HEIGHT);
	pVtx[1].pos = D3DXVECTOR3(FIELD_X + FIELD_WIDTH, 100.0f, FIELD_Z + FIELD_HEIGHT);
	pVtx[2].pos = D3DXVECTOR3(FIELD_X, 100.0f, FIELD_Z);
	pVtx[3].pos = D3DXVECTOR3(FIELD_X + FIELD_WIDTH, 0.0f, FIELD_Z);

	// 法線計算
	v1 = pVtx[1].pos - pVtx[0].pos;
	v2 = pVtx[2].pos - pVtx[0].pos;

	D3DXVec3Cross(&n0, &v1, &v2);
	D3DXVec3Normalize(&n0, &n0);

	pVtx[0].nor = n0;

	v1 = pVtx[2].pos - pVtx[3].pos;
	v2 = pVtx[1].pos - pVtx[3].pos;

	D3DXVec3Cross(&n0, &v1, &v2);
	D3DXVec3Normalize(&n0, &n0);

	pVtx[3].nor = n0;

	n0 = (pVtx[0].nor + pVtx[3].nor) * 0.5f;
	D3DXVec3Normalize(&n0, &n0);
	pVtx[1].nor = n0;
	pVtx[2].nor = n0;


	for(i = 0; i < VERTEX_NUM; i++) {
		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255); // 頂点カラー設定
	}


	// テクスチャUV座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(6.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 6.0f);
	pVtx[3].tex = D3DXVECTOR2(6.0f, 6.0f);

	m_VertexBuffer -> Unlock( );

}


/*******************************************************************************
* 関数名：Uninit
* 引数：なし
* 戻り値：なし
* 説明：終了処理
*******************************************************************************/

void CScene3D::Uninit(void) {
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

void CScene3D::Update(void) {
//	VERTEX_3D *pVtx;
	CInput *input;
	input = CManager::GetInput( ); // 入力情報取得

//	m_position.x += 0.1f;

	// 頂点情報へのポインタを取得
/*	m_VertexBuffer -> Lock(0, 0, (void**)&pVtx, 0);

	// 座標設定
	pVtx[0].pos += m_position;
	pVtx[1].pos += m_position;
	pVtx[2].pos += m_position;
	pVtx[3].pos += m_position;

	m_VertexBuffer -> Unlock( );*/

	if(input -> GetKeyboardPress(DIK_SPACE) ) {
		Uninit( );
		Release(SCENE3D_PRIORITY_NUM);
	}
}


/*******************************************************************************
* 関数名：Draw
* 引数：なし
* 戻り値：なし
* 説明：描画処理
*******************************************************************************/

void CScene3D::Draw(void) {
/*	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // 計算用
	D3DXMATRIX mtxView;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	// ワールドマトリックスの作成
	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rotation.y, m_rotation.x, m_rotation.z); // ※※※ y, x, z の順番に注意！！！ ※※※
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_position.x, m_position.y, m_position.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_VertexBuffer, 0, sizeof(VERTEX_3D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	device -> SetTexture(0, m_TexturePolygon);

	// 3Dポリゴンの描画
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		TRIANGLE_NUM);*/
}


/*******************************************************************************
* 関数名：Create
* 引数：scene3D : アドレス
* 戻り値：なし
* 説明：オブジェクトの生成
*******************************************************************************/

CScene3D *CScene3D::Create(void) {
	CScene3D *scene3D;
	scene3D = new CScene3D;
	scene3D -> Init( );
	return scene3D;
}