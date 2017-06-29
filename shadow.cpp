/*******************************************************************************
* タイトル 影描画処理
* ファイル名 shadow.cpp
* 作成者 AT-12C-245 秋間雄太
* 作成日 2015/11/12
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "scene.h"
#include "shadow.h"
#include "renderer.h"


/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct {
	D3DXVECTOR3 pos; // 位置情報
	D3DXVECTOR3 *obj_pos; // 影が付く対象の座標情報へのポインタ
	D3DXVECTOR3 rot; // 回転情報
	D3DXVECTOR3 scl; // スケール値 基本的に全て1.0fで固定
	float size; // 影のサイズ
	bool use; // 使用中判定
}SHADOW;


/*******************************************************************************
* グローバル変数
*******************************************************************************/

LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL; //頂点バッファへのポインタ

SHADOW g_shadow[SHADOW_MAX]; // 影の情報

D3DXMATRIX g_mtxWorldShadow; // ワールドマトリックス


/*******************************************************************************
* 関数名：InitShadow
* 引数：なし
* 戻り値：なし
* 説明：影設定の初期化
*******************************************************************************/

void InitShadow(void) {
	// 変数宣言
	int i; // forループ用
	VERTEX_3D *pVtx;

	// グローバル変数の初期化

	// 変数の取得
	CRenderer *renderer; // デバイス取得用変数
	renderer= CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 pDevice = renderer -> GetDevice( );

	// テクスチャ設定
	D3DXCreateTextureFromFile(pDevice, SHADOW_TEXTURE_NAME, &g_pTextureShadow);

	// 頂点バッファの生成
	pDevice -> CreateVertexBuffer(sizeof(VERTEX_3D) * SHADOW_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShadow, NULL);
	

	// 頂点情報へのポインタを取得
	g_pVtxBuffShadow -> Lock(0, 0, (void**)&pVtx, 0);

	// 角度初期設定
	for(i = 0; i < SHADOW_MAX; i++) {
		g_shadow[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shadow[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shadow[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_shadow[i].obj_pos = NULL;
		g_shadow[i].use = false;
	}

	// 座標設定
	pVtx[0].pos = D3DXVECTOR3(-20, 0.0f, 20);
	pVtx[1].pos = D3DXVECTOR3(20, 0.0f, 20);
	pVtx[2].pos = D3DXVECTOR3(-20, 0.0f, -20);
	pVtx[3].pos = D3DXVECTOR3(20, 0.0f, -20);

	for(i = 0; i < SHADOW_VERTEX_NUM; i++) {
		pVtx[i].col = D3DCOLOR_RGBA(SHADOW_DEFAULT_COLOR, SHADOW_DEFAULT_COLOR, SHADOW_DEFAULT_COLOR, 255);
	}

	// 法線設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


	// テクスチャUV座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffShadow -> Unlock( );
}


/*******************************************************************************
* 関数名：UninitShadow
* 引数：なし
* 戻り値：なし
* 説明：影の終了処理
*******************************************************************************/

void UninitShadow(void) {
	if(g_pTextureShadow != NULL) {
		g_pTextureShadow -> Release( ); // テクスチャの開放
		g_pTextureShadow = NULL;
	}

	if(g_pVtxBuffShadow != NULL) {
		g_pVtxBuffShadow -> Release( );
		g_pVtxBuffShadow = NULL;
	}
}


/*******************************************************************************
* 関数名：UpdataShadow
* 引数：なし
* 戻り値：なし
* 説明：影の情報更新
*******************************************************************************/

void UpdateShadow(void) {
	D3DXVECTOR3 obj_pos; // 位置情報取得用
	int i; // for文ループ用

	for(i = 0; i < SHADOW_MAX; i++) {
		if(g_shadow[i].use) {
			obj_pos = *g_shadow[i].obj_pos;
			g_shadow[i].pos.x = obj_pos.x;
			g_shadow[i].pos.z = obj_pos.z;
		}
	}
}


/*******************************************************************************
* 関数名：DrawShadow
* 引数：なし
* 戻り値：なし
* 説明：影の描画処理
*******************************************************************************/

void DrawShadow(void) {
	int i, j; // for文ループ用

	CRenderer *renderer; // デバイス取得用変数
	renderer= CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 pDevice = renderer -> GetDevice( );
	VERTEX_3D *pVtx;
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // 計算用
	D3DXVECTOR3 obj_pos; // 位置情報取得用

	// アルファブレンドを減算合成に設定
	pDevice -> SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice -> SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// アルファテスト設定
	pDevice -> SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice -> SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice -> SetRenderState(D3DRS_ALPHAREF, 0);


	for(i = 0; i < SHADOW_MAX; i++) {
		if(g_shadow[i].use) {
			// 影のサイズ設定
			obj_pos = *g_shadow[i].obj_pos; // 位置情報取得

			g_pVtxBuffShadow -> Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].pos = D3DXVECTOR3(-g_shadow[i].size - g_shadow[i].size * (obj_pos.y / 100), 0.1f, g_shadow[i].size + g_shadow[i].size * (obj_pos.y / 100) );
			pVtx[1].pos = D3DXVECTOR3(g_shadow[i].size + g_shadow[i].size * (obj_pos.y / 100), 0.1f, g_shadow[i].size + g_shadow[i].size * (obj_pos.y / 100) );
			pVtx[2].pos = D3DXVECTOR3(-g_shadow[i].size - g_shadow[i].size * (obj_pos.y / 100), 0.1f, -g_shadow[i].size - g_shadow[i].size * (obj_pos.y / 100));
			pVtx[3].pos = D3DXVECTOR3(g_shadow[i].size + g_shadow[i].size * (obj_pos.y / 100), 0.1f, -g_shadow[i].size - g_shadow[i].size * (obj_pos.y / 100));

			for(j = 0; j < SHADOW_VERTEX_NUM; j++) {
				pVtx[j].col = D3DCOLOR_RGBA(SHADOW_DEFAULT_COLOR - int(obj_pos.y), SHADOW_DEFAULT_COLOR - int(obj_pos.y), SHADOW_DEFAULT_COLOR - int(obj_pos.y), 255);
				if(SHADOW_DEFAULT_COLOR - int(obj_pos.y) < 0) {
					pVtx[j].col = D3DCOLOR_RGBA(0, 0, 0, 255);
				}
			}

			g_pVtxBuffShadow -> Unlock( );

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldShadow);
			// ワールドマトリックスの作成
			// スケールを反映
			D3DXMatrixScaling(&mtxScl, g_shadow[i].scl.x, g_shadow[i].scl.y, g_shadow[i].scl.z);
			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_shadow[i].rot.y, g_shadow[i].rot.x, g_shadow[i].rot.z); // ※※※ y, x, z の順番に注意！！！ ※※※
			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTrans, g_shadow[i].pos.x, g_shadow[i].pos.y, g_shadow[i].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice -> SetTransform(D3DTS_WORLD, &g_mtxWorldShadow);

			// 頂点バッファをデータストリームにバインド
			pDevice -> SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D) );

			// 頂点フォーマットの設定
			pDevice -> SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice -> SetTexture(0, g_pTextureShadow);

			// ポリゴンの描画
			pDevice -> DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				SHADOW_TRIANGLE_NUM);
		}
	}
	// レンダーステート設定を元に戻す
	pDevice -> SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice -> SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice -> SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice -> SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice -> SetRenderState(D3DRS_ALPHAREF, 0);
}


/*******************************************************************************
* 関数名：SetShadow
* 引数：なし
* 戻り値：なし
* 説明：各物体に対し影を設定する
*******************************************************************************/

int SetShadow(D3DXVECTOR3 *new_pos, float width, float height) {
	int i; // for文ループ用

	for(i = 0; i < SHADOW_MAX; i++) {
		if(g_shadow[i].use == false) {
			g_shadow[i].pos = *new_pos;
			g_shadow[i].pos.y = 0.0f;
			g_shadow[i].obj_pos = new_pos;
			g_shadow[i].size = (width + height) * 0.25f;
			g_shadow[i].use = true;
			break;
		}
	}
	return i;
}



/*******************************************************************************
* 関数名：DeleteShadow
* 引数：なし
* 戻り値：なし
* 説明：影を消去する
*******************************************************************************/

void DeleteShadow(int num) {
	g_shadow[num].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_shadow[num].obj_pos = NULL;
	g_shadow[num].use = false;
}