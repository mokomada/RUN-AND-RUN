/*******************************************************************************
* タイトル GM31 オブジェクト管理
* ファイル名 object.cpp
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/09/27
*******************************************************************************/


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "sceneX.h"
#include "camera.h"
#include "stencilshadow.h"
#include "meshfield.h"
#include "player.h"
#include "game.h"


/*******************************************************************************
* 関数名：CStencilShadow
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CStencilShadow::CStencilShadow(int priority, OBJTYPE objType) {

}


/*******************************************************************************
* 関数名：~CStencilShadow
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CStencilShadow::~CStencilShadow( ) {

}


/*******************************************************************************
* 関数名：Init
* 引数：なし
* 戻り値：なし
* 説明：初期化処理
*******************************************************************************/

void CStencilShadow::Init(void) {

}

void CStencilShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth) {
	// 変数宣言
	int i, j; // forループ用
	float angle_xz;
	VERTEX_2D *pVtx2D;
	VERTEX_3D *pVtx;
	WORD *pIdx;

	// 変数の取得
	CRenderer *renderer; // デバイス取得用変数
	renderer = CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得
	CPlayer *get_player = CGame::GetPlayer( );
	MODEL player = get_player -> GetModel( );

	// 引数格納
	m_height = height;
	m_width = width;

	// メンバ変数の初期化
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pVtxBuffLid = NULL;
	m_pIdxBuff = NULL;

	// 円柱生成
	D3DXCreateCylinder(device, m_width, m_width, m_height, 12, 1, &m_pMesh, NULL);

	// 角度計算
	angle_xz = 2 * D3DX_PI / STENCILSHADOW_X_BLOCK;

	// テクスチャ設定
	D3DXCreateTextureFromFile(device, STENCILSHADOW_TEXTURE_NAME, &m_pTexture);

	// 頂点バッファの生成
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * STENCILSHADOW_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);


	// 頂点情報へのポインタを取得
	m_pVtxBuff -> Lock(0, 0, (void**)&pVtx, 0);

	// 外郭の初期設定
	m_stencilshadow.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_stencilshadow.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_stencilshadow.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	for(i = 0; i < STENCILSHADOW_VERTEX_NUM; i++) {
		// 座標設定
		pVtx[i].pos = D3DXVECTOR3(
			cosf(angle_xz * (i % (STENCILSHADOW_X_BLOCK + 1) ) ) * m_width,	// X座標
			(STENCILSHADOW_Y_BLOCK - (i / (STENCILSHADOW_X_BLOCK + 1) ) ) * m_height / STENCILSHADOW_Y_BLOCK - m_height * 0.5f, // Y座標
			sinf(angle_xz * (i % (STENCILSHADOW_X_BLOCK + 1) ) ) * m_width	// Z座標
		);

		// 頂点色設定
		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// 法線設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		// UV座標設定
		pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	m_pVtxBuff -> Unlock( );

	// インデックスバッファ確保
	device -> CreateIndexBuffer(sizeof(WORD) * STENCILSHADOW_INDEX_NUM,
								 D3DUSAGE_WRITEONLY,
								 D3DFMT_INDEX16,
								 D3DPOOL_MANAGED,
								 &m_pIdxBuff,
								 NULL);

	m_pIdxBuff -> Lock(0, 0, (void**) &pIdx, 0);

	for(i = 0; i < STENCILSHADOW_Y_BLOCK; i++) {
		if(i != 0) {
			pIdx[i * (STENCILSHADOW_X_BLOCK + 2) * 2 - 1] = (i + 1) * (STENCILSHADOW_X_BLOCK + 1);
		}

		for(j = 0; j < (STENCILSHADOW_X_BLOCK + 1) * 2; j++) {
			pIdx[i * (STENCILSHADOW_X_BLOCK + 2) * 2 + j] = (i + 1 - j % 2) * (STENCILSHADOW_X_BLOCK + 1) + (j / 2);
		}

		if(i < STENCILSHADOW_Y_BLOCK - 1) {
			pIdx[i * (STENCILSHADOW_X_BLOCK + 2) * 2 + j] = (i + 1 - (j - 1) % 2) * (STENCILSHADOW_X_BLOCK + 1) + ( (j - 1) / 2);
		}
	}

	m_pIdxBuff -> Unlock( );

	// 頂点バッファの生成(蓋)
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * STENCILSHADOW_VERTEX_FAN_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffLid, NULL);


	// 頂点情報へのポインタを取得
	m_pVtxBuffLid -> Lock(0, 0, (void**)&pVtx, 0);

		// 原点設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

		for(i = 0; i < STENCILSHADOW_VERTEX_FAN_NUM - 1; i++) {
			// 位置設定
			pVtx[i + 1].pos = D3DXVECTOR3(
				cosf(-angle_xz * i) * m_width, // X座標
				0.0f,						 // Y座標
				sinf(-angle_xz * i) * m_width  // Z座標
			);

			// 頂点色設定
			pVtx[i + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			// 法線設定
			pVtx[i + 1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			// テクスチャ設定
			pVtx[i + 1].tex = D3DXVECTOR2(0.0f, 0.0f);
		}

	m_pVtxBuff -> Unlock( );


	/* 頂点バッファの生成 */
	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * STENCILSHADOW_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffScreen, NULL);

	/* 頂点情報へのポインタを取得 */
	m_pVtxBuffScreen -> Lock(0, 0, (void**)&pVtx2D, 0);

	/* 座標設定 */
	pVtx2D[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx2D[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx2D[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx2D[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);


	for(i = 0; i < STENCILSHADOW_VERTEX_NUM; i++) {
		pVtx2D[i].rhw = 1.0f;

		pVtx2D[i].col = D3DCOLOR_RGBA(0, 0, 0, 128);
	}

	/* テクスチャUV座標設定 */
	pVtx2D[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2D[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2D[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx2D[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuffScreen -> Unlock( );
}


/*******************************************************************************
* 関数名：Uninit
* 引数：なし
* 戻り値：なし
* 説明：終了処理
*******************************************************************************/

void CStencilShadow::Uninit(void) {
	if(m_pTexture != NULL) {
		m_pTexture -> Release( ); // テクスチャの開放
		m_pTexture = NULL;
	}

	if(m_pVtxBuff != NULL) {
		m_pVtxBuff -> Release( );
		m_pVtxBuff = NULL;
	}

	if(m_pVtxBuffLid != NULL) {
		m_pVtxBuffLid -> Release( );
		m_pVtxBuffLid = NULL;
	}

	if(m_pIdxBuff != NULL) {
		m_pIdxBuff -> Release( );
		m_pIdxBuff = NULL;
	}
}


/*******************************************************************************
* 関数名：Update
* 引数：なし
* 戻り値：なし
* 説明：更新処理
*******************************************************************************/

void CStencilShadow::Update(void) {
	CPlayer *get_player = CGame::GetPlayer( );
	MODEL player = get_player -> GetModel( );

	m_stencilshadow.pos = player.pos;
	m_stencilshadow.pos.y += (1.0f - m_height * 0.5f);

	m_oldPlayerPos = player.pos;
}


/*******************************************************************************
* 関数名：Draw
* 引数：なし
* 戻り値：なし
* 説明：描画処理
*******************************************************************************/

void CStencilShadow::Draw(void) {
	// 変数の取得
	CRenderer *renderer; // デバイス取得用変数
	renderer = CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // 計算用

	// ライト設定オフ
	device -> SetRenderState(D3DRS_LIGHTING, FALSE);

	// 円柱（表）描画
	device -> SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	device -> SetRenderState(D3DRS_STENCILENABLE, TRUE);
	device -> SetRenderState(D3DRS_COLORWRITEENABLE, 0);
	device -> SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	device -> SetRenderState(D3DRS_STENCILREF, 1);
	device -> SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
	device -> SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	// ワールドマトリックスの作成
	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_stencilshadow.scl.x, m_stencilshadow.scl.y, m_stencilshadow.scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_stencilshadow.rot.y, m_stencilshadow.rot.x + D3DX_PI * 0.5f, m_stencilshadow.rot.z); // ※※※ y, x, z の順番に注意！！！ ※※※
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_stencilshadow.pos.x, m_stencilshadow.pos.y, m_stencilshadow.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	m_pMesh -> DrawSubset(0);

	// インデックスバッファをバインド
/*	device -> SetIndices(m_pIdxBuff);

	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	device -> SetTexture(0, NULL);

	device -> DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		STENCILSHADOW_VERTEX_NUM,
		0,
		STENCILSHADOW_TRIANGLE_NUM);

	// 蓋の描画
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	// ワールドマトリックスの作成
	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_stencilshadow.scl.x, m_stencilshadow.scl.y, m_stencilshadow.scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_stencilshadow.rot.y, m_stencilshadow.rot.x, m_stencilshadow.rot.z); // ※※※ y, x, z の順番に注意！！！ ※※※
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_stencilshadow.pos.x, m_stencilshadow.pos.y + m_height * 0.5f, m_stencilshadow.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_pVtxBuffLid, 0, sizeof(VERTEX_3D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_3D);

	device->DrawPrimitive(
		D3DPT_TRIANGLEFAN,
		0,
		STENCILSHADOW_TRIANGLE_FAN_NUM);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	// ワールドマトリックスの作成
	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_stencilshadow.scl.x, m_stencilshadow.scl.y, m_stencilshadow.scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_stencilshadow.rot.y, m_stencilshadow.rot.x + D3DX_PI, m_stencilshadow.rot.z); // ※※※ y, x, z の順番に注意！！！ ※※※
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_stencilshadow.pos.x, m_stencilshadow.pos.y - m_height * 0.5f, m_stencilshadow.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_pVtxBuffLid, 0, sizeof(VERTEX_3D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_3D);

	device->DrawPrimitive(
		D3DPT_TRIANGLEFAN,
		0,
		STENCILSHADOW_TRIANGLE_FAN_NUM);*/

	// 円柱（裏）描画
	device -> SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);
	device -> SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pMesh -> DrawSubset(0);

	// ワールドマトリックスの初期化
/*	D3DXMatrixIdentity(&m_mtxWorld);
	// ワールドマトリックスの作成
	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_stencilshadow.scl.x, m_stencilshadow.scl.y, m_stencilshadow.scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_stencilshadow.rot.y, m_stencilshadow.rot.x + D3DX_PI * 0.5f, m_stencilshadow.rot.z); // ※※※ y, x, z の順番に注意！！！ ※※※
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_stencilshadow.pos.x, m_stencilshadow.pos.y, m_stencilshadow.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);*/

/*	// インデックスバッファをバインド
	device -> SetIndices(m_pIdxBuff);

	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	device -> SetTexture(0, NULL);

	device -> DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		STENCILSHADOW_VERTEX_NUM,
		0,
		STENCILSHADOW_TRIANGLE_NUM);

	// 蓋の描画
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	// ワールドマトリックスの作成
	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_stencilshadow.scl.x, m_stencilshadow.scl.y, m_stencilshadow.scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_stencilshadow.rot.y, m_stencilshadow.rot.x, m_stencilshadow.rot.z); // ※※※ y, x, z の順番に注意！！！ ※※※
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_stencilshadow.pos.x, m_stencilshadow.pos.y + m_height * 0.5f, m_stencilshadow.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_pVtxBuffLid, 0, sizeof(VERTEX_3D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_3D);

	device->DrawPrimitive(
		D3DPT_TRIANGLEFAN,
		0,
		STENCILSHADOW_TRIANGLE_FAN_NUM);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	// ワールドマトリックスの作成
	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_stencilshadow.scl.x, m_stencilshadow.scl.y, m_stencilshadow.scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_stencilshadow.rot.y, m_stencilshadow.rot.x + D3DX_PI, m_stencilshadow.rot.z); // ※※※ y, x, z の順番に注意！！！ ※※※
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_stencilshadow.pos.x, m_stencilshadow.pos.y - m_height * 0.5f, m_stencilshadow.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_pVtxBuffLid, 0, sizeof(VERTEX_3D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_3D);

	device->DrawPrimitive(
		D3DPT_TRIANGLEFAN,
		0,
		STENCILSHADOW_TRIANGLE_FAN_NUM);*/

	// 全画面2Dポリゴン描画
	device -> SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	device -> SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
	device -> SetRenderState(D3DRS_ZENABLE, FALSE);
	device -> SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_pVtxBuffScreen, 0, sizeof(VERTEX_2D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	device -> SetTexture(0, NULL);

	// 2Dポリゴンの描画
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

	device -> SetRenderState(D3DRS_ZENABLE, TRUE);
	device -> SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	device -> SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// ライト設定オン
	device -> SetRenderState(D3DRS_LIGHTING, TRUE);
}


/*******************************************************************************
* 関数名：Create
* 引数：pos : 座標
*		rot : 回転量
*		height : 高さ
*		width : 幅
*		depth : 奥行き
* 戻り値：object : 新しく生成したクラスのアドレス
* 説明：オブジェクトの生成
*******************************************************************************/

CStencilShadow *CStencilShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth) {
	CStencilShadow *object;
	object = new CStencilShadow;
	object -> Init(pos, rot, height, width, depth);
	return object;
}