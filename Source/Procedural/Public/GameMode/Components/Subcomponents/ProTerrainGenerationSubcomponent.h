// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/Components/Subcomponents/ProWorldGenerationSubcomponentBase.h"
#include "ProceduralMeshComponent.h"
#include "ProTerrainGenerationSubcomponent.generated.h"

USTRUCT(BlueprintType)
struct FGridSize
{
	GENERATED_BODY()

	int32 X;
	int32 Y;

public:
	FGridSize()
	{
		X = 0;
		Y = 0;
	}
	FGridSize(int32 InX, int32 InY)
	{
		X = InX;
		Y = InY;
	}
};

USTRUCT()
struct FGeneratedWorldTerrainSettings
{
	GENERATED_BODY()

	FGridSize GridSize;
	float WorldScale;
	float CellsSize = 100.0f;

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;

	FGeneratedWorldTerrainSettings()
	{
		GridSize = FGridSize(100, 100);
		WorldScale = 10.0f;

		Vertices = TArray<FVector>();
		Triangles = TArray<int32>();
		Normals = TArray<FVector>();
		UVs = TArray<FVector2D>();
		Tangents = TArray<FProcMeshTangent>();
	}

	FGeneratedWorldTerrainSettings(
		FGridSize InGridSize,
		float InWorldScale,
		float InCellsSize,
		TArray<FVector> InVertices,
		TArray<int32> InTriangles,
		TArray<FVector> InNormals,
		TArray<FVector2D> InUVs,
		TArray<FProcMeshTangent> InTangents
	)
	{
		GridSize = InGridSize;
		WorldScale = InWorldScale;
		CellsSize = InCellsSize;
		Vertices = InVertices;
		Triangles = InTriangles;
		Normals = InNormals;
		UVs = InUVs;
		Tangents = InTangents;
	}

public:
	bool IsValid() const { return true; };
};


UCLASS()
class PROCEDURAL_API UProTerrainGenerationSubcomponent : public UProWorldGenerationSubcomponentBase
{
	GENERATED_BODY()
	
public:

	FGeneratedWorldTerrainSettings RequestTerrainGeneration();
};
