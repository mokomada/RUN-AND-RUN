/*******************************************************************************
* ファイル名 shadow.h
* 作成者 AT-12C-245 秋間雄太
* 作成日 2015/11/12
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _SHADOW_H_
#define _SHADOW_H_

/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define SHADOW_VERTEX_NUM (4) // 描画する頂点の個数
#define SHADOW_TRIANGLE_NUM (2) // 描画する三角形の個数

#define SHADOW_TEXTURE_NAME "data/TEXTURE/shadow000.jpg" // テクスチャーに使う画像指定

#define SHADOW_DEFAULT_COLOR (128) // 影の濃さのデフォルト値

#define SHADOW_MAX (100) // 一度に表示できる影の最大数

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/

void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 *new_pos, float width, float height);
void DeleteShadow(int num);

#endif
/*** インクルードガード終了 ***/