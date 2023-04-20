#pragma once

#include "fbxsdk.h"
#include "FbxModel.h"
#include "string"

#include <d3d12.h>
#include <d3dx12.h>

class FbxLoader
{
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

public:
	//初期化	
	void Initialize(ID3D12Device* device);
	//後始末
	void Finalize();

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

private:
	//デバイス
	ID3D12Device* device = nullptr;
	//FBXマネージャー
	FbxManager* fbxManager = nullptr;
	//FBXインポーター
	FbxImporter* fbxImporter = nullptr;

private:
	using string = std::string;	//stdを省略

	//定数
public:
	//モデル格納ルートパス
	static const string baseDirectory;
	//テクスチャない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;
	//FBXファイルの読み込み
	FbxModel* LoadModelFromFile(const string modelName, const string textureName);
	//ノード構成を解析
	void ParseNodeRecursive(FbxModel* model, FbxNode* fbxNode,Node* parent = nullptr);
	//メッシュ解析
	void ParseMesh(FbxModel* model, FbxNode* fbxNode);
	
	//メッシュ解析サブ関数
	
	//頂点座標読み取り
	void ParseMeshVertices(FbxModel* model, FbxMesh* fbxMesh);
	//面情報読み取り
	void ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh);
	//マテリアル読み取り
	void ParseMaterial(FbxModel* model, FbxNode* fbxNode);
	//テクスチャ読み取り
	void LoadTexture(FbxModel* model, const std::string& fullpath);
	//ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);

	//FBXの行列をXMMATRIXに変換
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst,const FbxAMatrix& src);
	//スキニング情報読み取り
	void ParseSkin(FbxModel* model, FbxMesh* fbxMesh);

private:
	string textureName;
};