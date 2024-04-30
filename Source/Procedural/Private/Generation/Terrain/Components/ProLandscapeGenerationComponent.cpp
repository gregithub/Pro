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

FGeneratedWorldLandscapeSettings UProLandscapeGenerationComponent::GenerateLandscapeSettings()
{
	FGeneratedWorldLandscapeSettings LandscapeSettings;

	const float LocalWorldScale = LandscapeSettings.WorldScale;

	for (int32 CurrentRow = 0; CurrentRow <= LandscapeSettings.GridSize.X; CurrentRow++)
	{
		for (int32 CurrentColumn = 0; CurrentColumn <= LandscapeSettings.GridSize.Y; CurrentColumn++)
		{
			const float NoiseValue = FMath::PerlinNoise2D(FVector2D(CurrentRow, CurrentColumn));
			const float Height = NoiseValue * 100.0f;

			LandscapeSettings.Vertices.Add(FVector(CurrentRow * LandscapeSettings.CellsSize, Height, CurrentColumn * LandscapeSettings.CellsSize));
			LandscapeSettings.UVs.Add(FVector2D(CurrentRow / LandscapeSettings.GridSize.X, CurrentColumn / LandscapeSettings.GridSize.Y));
		}
	}

	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(LandscapeSettings.GridSize.X + 1, LandscapeSettings.GridSize.Y + 1, true, LandscapeSettings.Triangles);
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(LandscapeSettings.Vertices, LandscapeSettings.Triangles, LandscapeSettings.UVs, LandscapeSettings.Normals, LandscapeSettings.Tangents);

	return LandscapeSettings;

	//
//{
// 
//  LandscapeSettings.Vertices.Add(FVector(CurrentRow * LandscapeSettings.CellsSize, CurrentColumn * LandscapeSettings.CellsSize, Height));
//  LandscapeSettings.Normals.Add(FVector(0, 0, 1));
//  LandscapeSettings.UVO.Add(FVector2D(CurrentRow, CurrentColumn)); 
// 
//	int32 CurrentIndex = CurrentRow + (CurrentColumn * LandscapeSettings.GridSize.X);
// 
// if ((CurrentRow < (LandscapeSettings.GridSize.X - 1)) && (CurrentColumn < (LandscapeSettings.GridSize.Y - 1)))
//{
//  LandscapeSettings.Triangles.Add(CurrentIndex);
//	LandscapeSettings.Triangles.Add(CurrentIndex + LandscapeSettings.GridSize.X);
//	LandscapeSettings.Triangles.Add(CurrentIndex + 1);
//
//	LandscapeSettings.Triangles.Add(CurrentIndex + LandscapeSettings.GridSize.X);
//	LandscapeSettings.Triangles.Add(CurrentIndex + LandscapeSettings.GridSize.X + 1);
//	LandscapeSettings.Triangles.Add(CurrentIndex + 1);
// }
//}
}