// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Components/Subcomponents/ProTerrainGenerationSubcomponent.h"
#include "LandscapeComponent.h"
#include "ProceduralMeshComponent.h"


void UProTerrainGenerationSubcomponent::RequestTerrainGeneration()
{
	FGeneratedWorldTerrainSettings WorldSettings;

	if (WorldSettings.IsValid() == false)
	{
		return;
	}

	TArray<FVector> Vertices;
	TArray<int32> Triangles;

	const float LocalWorldScale = WorldSettings.WorldScale;

	for (int32 CurrentRow = 0; CurrentRow <= WorldSettings.GridSize.X; CurrentRow++)
	{
		for (int32 CurrentColumn = 0; CurrentColumn <= WorldSettings.GridSize.Y; CurrentColumn++)
		{
			float X = CurrentRow * LocalWorldScale;
			float Y = CurrentColumn * LocalWorldScale;
			float Z = FMath::PerlinNoise2D(FVector2D(X, Y)) * 100.0f;
		}
	}

	return;
}