// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/Terrain/Components/ProLandscapeGenerationComponent.h"
#include "ProceduralMeshComponent.h"
#include "GameInstance/ProGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/ProGameModeBase.h"
#include "KismetProceduralMeshLibrary.h"
#include "LandscapeComponent.h"
#include "Generation/Noise/ProNoise.h"

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

bool UProLandscapeGenerationComponent::TryGenerateLandscapeSettings()
{
	LandscapeSettings = FGeneratedWorldLandscapeSettings();

	const int temp_seed = 10;

	for (int32 CurrentRow = 0; CurrentRow <= GridSize.X; CurrentRow++)
	{
		for (int32 CurrentColumn = 0; CurrentColumn <= GridSize.Y; CurrentColumn++)
		{
			const float NoiseValue = ProNoise::SinglePerling(temp_seed, CurrentRow, CurrentColumn);
			const float Height = NoiseValue * 100.0f;

			LandscapeSettings.Vertices.Add(FVector(CurrentRow * CellsSize, Height, CurrentColumn * CellsSize));
			LandscapeSettings.UVs.Add(FVector2D(CurrentRow / GridSize.X, CurrentColumn / GridSize.Y));
		}
	}

	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(GridSize.X + 1, GridSize.Y + 1, true, LandscapeSettings.Triangles);
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(LandscapeSettings.Vertices, LandscapeSettings.Triangles, LandscapeSettings.UVs, LandscapeSettings.Normals, LandscapeSettings.Tangents);

	return true;
}