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

	FGeneratedWorldTerrainSettings()
	{
		GridSize = FGridSize(10, 10);
		WorldScale = 1.0f;
	}

public:
	bool IsValid() const { return true; };
};

UCLASS()
class PROCEDURAL_API UProTerrainGenerationSubcomponent : public UProWorldGenerationSubcomponentBase
{
	GENERATED_BODY()
	
public:

	void RequestTerrainGeneration();
};
