// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/Components/Subcomponents/ProWorldGenerationSubcomponentBase.h"
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

USTRUCT(BlueprintType)
struct FGeneratedWorldTerrainSettings
{
	GENERATED_BODY()

	UPROPERTY()
	FGridSize GridSize;

	UPROPERTY()
	float WorldScale;

	UPROPERTY()
	float CellsSize = 100.0f;

	UPROPERTY()
	TArray<FVector> Vertices;
	UPROPERTY()
	TArray<int32> Triangles;
	UPROPERTY()
	TArray<FVector> Normals;
	UPROPERTY()
	TArray<FVector2D> UVO;

	FGeneratedWorldTerrainSettings()
	{
		GridSize = FGridSize(10, 10);
		WorldScale = 1.0f;

		Vertices = TArray<FVector>();
		Triangles = TArray<int32>();
		Normals = TArray<FVector>();
		UVO = TArray<FVector2D>();
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
