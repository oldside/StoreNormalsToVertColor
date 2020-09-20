// StoreNormalsToVertColor.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <fbxsdk.h>

void PrintVector4(int index,FbxVector4 V4)
{
	std::cout << "Index:" << index << ",X:" << V4[0] << ",Y:" << V4[1] << ",Z:" << V4[2] << ",W:" << V4[3] << std::endl;
}
void PrintReferenceAndMappingMode(FbxLayerElement::EReferenceMode ReferenceMode, FbxLayerElement::EMappingMode MappingMode)
{
	switch (ReferenceMode)
	{
	case fbxsdk::FbxLayerElement::eDirect:printf("ReferenceMode:eDirect\n");
		break;
	case fbxsdk::FbxLayerElement::eIndex:printf("ReferenceMode:eIndex\n");
		break;
	case fbxsdk::FbxLayerElement::eIndexToDirect:printf("ReferenceMode:eIndexToDirect\n");
		break;
	default:printf("ReferenceMode:???ERROR???\n");
		break;
	}
	switch (MappingMode)
	{
	case fbxsdk::FbxLayerElement::eNone:printf("MappingMode:eNone\n");
		break;
	case fbxsdk::FbxLayerElement::eByControlPoint:printf("MappingMode:eByControlPoint\n");
		break;
	case fbxsdk::FbxLayerElement::eByPolygonVertex:printf("MappingMode:eByPolygonVertex\n");
		break;
	case fbxsdk::FbxLayerElement::eByPolygon:printf("MappingMode:eByPolygon\n");
		break;
	case fbxsdk::FbxLayerElement::eByEdge:printf("MappingMode:eByEdge\n");
		break;
	case fbxsdk::FbxLayerElement::eAllSame:printf("MappingMode:eAllSame\n");
		break;
	default:printf("MappingMode:???ERROR???\n");
		break;
	}
}

void StoreNormalsToVertColor(FbxNode* node) 
{
	if (node->GetChildCount())
	{
		for (int i = 0; i < node->GetChildCount(); i++)
		{
			if (node->GetChild(i)->GetMesh())
			{
				FbxMesh* mesh = node->GetChild(i)->GetMesh();
				FbxLayer* layer0 = mesh->GetLayer(0);
				FbxLayerElementVertexColor* VertColor = layer0->GetVertexColors();
				FbxLayerElementNormal* VertNormal = layer0->GetNormals();
				FbxLayerElementTangent* VertTangent = layer0->GetTangents();
				FbxLayerElementBinormal* VertBinomral = layer0->GetBinormals();
				
				//for (int j = 0; j < mesh->GetPolygonVertexCount(); j++)
				//{
				//	std::cout << mesh->GetPolygonVertices()[j] << std::endl;
				//	std::cout << mesh->GetPolygonVertices()[j] << std::endl;
				//
				//}
				//
				//FbxLayerElementArrayTemplate<int>& VertColorIndexArray = VertColor->GetIndexArray();
				//FbxLayerElementArrayTemplate<FbxColor>& VertColorValueArray = VertColor->GetDirectArray();
				//FbxLayerElementArrayTemplate<int>& VertNormalIndexArray = VertColor->GetIndexArray();
				
				
				
				for (int j = 0; j < mesh->GetPolygonVertexCount(); j++)
				{
					FbxArray<int> SameControlPointsIndex;
					for (int k = 0; k < mesh->GetPolygonVertexCount(); k++)
					{
						if (mesh->GetPolygonVertices()[k] == mesh->GetPolygonVertices()[j])
						{
							SameControlPointsIndex.Add(k);
						}

					}
					
					
					FbxArray<FbxVector4> Normals;
					for (int x = 0; x < SameControlPointsIndex.Size(); x++)
					{
						//std::cout << SameControlPointsIndex[x] << std::endl;
						//int VertNormalIndex = VertNormal->GetIndexArray()[SameControlPointsIndex[x]];
						FbxVector4 Normal = VertNormal->GetDirectArray()[SameControlPointsIndex[x]];
						Normals.AddUnique(Normal);
						
					}

					FbxVector4 SmoothNormal;
					for (int n = 0; n < Normals.Size(); n++)
					{
						SmoothNormal += Normals[n];
					}
					SmoothNormal.Normalize();
					//PrintVector4(j, SmoothNormal);

#if 1
					FbxVector4 Tangent = VertTangent->GetDirectArray()[j];
					FbxVector4 Normal = VertNormal->GetDirectArray()[j];
					//FbxVector4 Bitangent = Normal.CrossProduct(Tangent);
					FbxVector4 Bitangent = VertBinomral->GetDirectArray()[j];
					
					FbxVector4 tmpVector;
					
					tmpVector = SmoothNormal;
					tmpVector[0] = Tangent.DotProduct(SmoothNormal);
					tmpVector[1] = Bitangent.DotProduct(SmoothNormal);
					tmpVector[2] = Normal.DotProduct(SmoothNormal);
					tmpVector[3] = 0;
					SmoothNormal = tmpVector;
#endif
					//PrintReferenceAndMappingMode(VertNormal->GetReferenceMode(), VertNormal->GetMappingMode());
					//PrintReferenceAndMappingMode(VertTangent->GetReferenceMode(), VertTangent->GetMappingMode());
					//PrintReferenceAndMappingMode(VertBinomral->GetReferenceMode(), VertBinomral->GetMappingMode());
					//PrintReferenceAndMappingMode(VertColor->GetReferenceMode(), VertColor->GetMappingMode());

					int VertColorIndex = VertColor->GetIndexArray()[j];
					
					FbxColor Color;
					Color.mRed = SmoothNormal[0] * 0.5f + 0.5f;
					Color.mGreen = SmoothNormal[1] * 0.5f + 0.5f;
					Color.mBlue = SmoothNormal[2] * 0.5f + 0.5f;
					Color.mAlpha = VertColor->GetDirectArray()[VertColorIndex].mAlpha;

					VertColor->GetDirectArray().SetAt(VertColorIndex, Color);
					//std::cout << "R:" << VertColor->GetDirectArray()[j].mRed << ",G:" << VertColor->GetDirectArray()[j].mGreen << ",B:" << VertColor->GetDirectArray()[j].mBlue << ",A:" << VertColor->GetDirectArray()[j].mAlpha << std::endl;
					

				}






				



#if 0
				FbxLayerElement::EMappingMode MappingMode = VertColor->GetMappingMode();
				FbxLayerElement::EReferenceMode ReferenceMode = VertColor->GetReferenceMode();
				switch (MappingMode)
				{
				case fbxsdk::FbxLayerElement::eNone:printf("MappingMode:eNone\n");
					break;
				case fbxsdk::FbxLayerElement::eByControlPoint:printf("MappingMode:eByControlPoint\n");
					break;
				case fbxsdk::FbxLayerElement::eByPolygonVertex:printf("MappingMode:eByPolygonVertex\n");
					break;
				case fbxsdk::FbxLayerElement::eByPolygon:printf("MappingMode:eByPolygon\n");
					break;
				case fbxsdk::FbxLayerElement::eByEdge:printf("MappingMode:eByEdge\n");
					break;
				case fbxsdk::FbxLayerElement::eAllSame:printf("MappingMode:eAllSame\n");
					break;
				default:printf("MappingMode:???ERROR???\n");
					break;
				}
				switch (ReferenceMode)
				{
				case fbxsdk::FbxLayerElement::eDirect:printf("ReferenceMode:eDirect\n");
					break;
				case fbxsdk::FbxLayerElement::eIndex:printf("ReferenceMode:eIndex\n");
					break;
				case fbxsdk::FbxLayerElement::eIndexToDirect:printf("ReferenceMode:eIndexToDirect\n");
					break;
				default:printf("ReferenceMode:???ERROR???\n");
					break;
				}
#endif

				



				//for (int n = 0; n < IndexArray.GetCount(); n++)
				//{
				//	std::cout << IndexArray.GetAt(n) <<std::endl;
				//}
				//for (int m = 0; m < VertColorValueArray.GetCount(); m++)
				//{
				//	std::cout <<"R:"<< VertColorValueArray.GetAt(m).mRed <<",G:"<< VertColorValueArray.GetAt(m).mGreen<<",B:"<< VertColorValueArray.GetAt(m).mBlue<<std::endl;
				//}



			}
			StoreNormalsToVertColor(node->GetChild(i));
		}
	}
}



int main(int argc, char** argv) {

	// Change the following filename to a suitable filename value.
	const char* lFilename = "Weapon.fbx";
	const char* lFilename2 = "Export.fbx";

	// Initialize the SDK manager. This object handles all our memory management.
	FbxManager* lSdkManager = FbxManager::Create();

	// Create the IO settings object.
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	
	// Use the first argument as the filename for the importer.
	if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}

	// Create a new scene so that it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	// The file is imported; so get rid of the importer.
	lImporter->Destroy();
		
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode) {
		StoreNormalsToVertColor(lRootNode);
	}
		
	FbxExporter* lExporter = FbxExporter::Create(lSdkManager, "");
	bool lExportStatus = lExporter->Initialize(lFilename2, -1, lSdkManager->GetIOSettings());
	if (!lExportStatus) {
		printf("Call to FbxExporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
		return false;
	}
	lExporter->Export(lScene);
	

	lExporter->Destroy();




	
	// Destroy the SDK manager and all the other objects it was handling.
	lSdkManager->Destroy();
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
