// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/Terrain/Components/ProLandscapeGenerationComponent.h"
#include "ProceduralMeshComponent.h"
#include "GameInstance/ProGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/ProGameModeBase.h"
#include "KismetProceduralMeshLibrary.h"
#include "LandscapeComponent.h"
#include "Math.h"

UProLandscapeGenerationComponent::UProLandscapeGenerationComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UProLandscapeGenerationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UProLandscapeGenerationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FGeneratedWorldTerrainSettings UProLandscapeGenerationComponent::RequestTerrainGeneration()
{
	FGeneratedWorldTerrainSettings WorldSettings;

	const float LocalWorldScale = WorldSettings.WorldScale;

	for (int32 CurrentRow = 0; CurrentRow <= WorldSettings.GridSize.X; CurrentRow++)
	{
		for (int32 CurrentColumn = 0; CurrentColumn <= WorldSettings.GridSize.Y; CurrentColumn++)
		{
			const float NoiseValue = FMath::PerlinNoise2D(FVector2D(CurrentRow, CurrentColumn));
			const float Height = NoiseValue * 100.0f;

			WorldSettings.Vertices.Add(FVector(CurrentRow * WorldSettings.CellsSize, Height, CurrentColumn * WorldSettings.CellsSize));
			WorldSettings.UVs.Add(FVector2D(CurrentRow / WorldSettings.GridSize.X, CurrentColumn / WorldSettings.GridSize.Y));
		}
	}

	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(WorldSettings.GridSize.X + 1, WorldSettings.GridSize.Y + 1, true, WorldSettings.Triangles);
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(WorldSettings.Vertices, WorldSettings.Triangles, WorldSettings.UVs, WorldSettings.Normals, WorldSettings.Tangents);

	return WorldSettings;

	//
//{
// 
//  WorldSettings.Vertices.Add(FVector(CurrentRow * WorldSettings.CellsSize, CurrentColumn * WorldSettings.CellsSize, Height));
//  WorldSettings.Normals.Add(FVector(0, 0, 1));
//  WorldSettings.UVO.Add(FVector2D(CurrentRow, CurrentColumn)); 
// 
//	int32 CurrentIndex = CurrentRow + (CurrentColumn * WorldSettings.GridSize.X);
// 
// if ((CurrentRow < (WorldSettings.GridSize.X - 1)) && (CurrentColumn < (WorldSettings.GridSize.Y - 1)))
//{
//  WorldSettings.Triangles.Add(CurrentIndex);
//	WorldSettings.Triangles.Add(CurrentIndex + WorldSettings.GridSize.X);
//	WorldSettings.Triangles.Add(CurrentIndex + 1);
//
//	WorldSettings.Triangles.Add(CurrentIndex + WorldSettings.GridSize.X);
//	WorldSettings.Triangles.Add(CurrentIndex + WorldSettings.GridSize.X + 1);
//	WorldSettings.Triangles.Add(CurrentIndex + 1);
// }
//}
}