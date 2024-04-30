// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/Terrain/ProTerrainGeneration.h"
#include "ProceduralMeshComponent.h"
#include "GameInstance/ProGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Generation/Terrain/Components/ProLandscapeGenerationComponent.h"

AProTerrainGeneration::AProTerrainGeneration()
{
	PrimaryActorTick.bCanEverTick = true;

	ProLandscapeGenerationComponent = CreateDefaultSubobject<UProLandscapeGenerationComponent>(TEXT("ProWorldGenerationComponent"));
	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
}

void AProTerrainGeneration::BeginPlay()
{
	Super::BeginPlay();

	RequestTerrainGeneration();
}

void AProTerrainGeneration::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProTerrainGeneration::RequestTerrainGeneration()
{
	GeneratedWorldTerrainSettings = ProLandscapeGenerationComponent->GenerateLandscapeSettings();

	if (ProceduralMeshComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ProceduralMeshComponent is invalid!"));
		return;
	}

	if (GeneratedWorldTerrainSettings.IsValid() == false)
	{
		UE_LOG(LogTemp, Error, TEXT("GeneratedWorldTerrainSettings is invalid!"));
		return;
	}

	const int32 LocalSectionIndex = 0;

	ProceduralMeshComponent->ClearMeshSection(LocalSectionIndex);

	ProceduralMeshComponent->CreateMeshSection(
		LocalSectionIndex,
		GeneratedWorldTerrainSettings.Vertices,
		GeneratedWorldTerrainSettings.Triangles,
		GeneratedWorldTerrainSettings.Normals,
		GeneratedWorldTerrainSettings.UVs,
		TArray<FColor>(),
		GeneratedWorldTerrainSettings.Tangents,
		false);
}

void AProTerrainGeneration::CallInEditor_RegenerateTerrain()
{
	RequestTerrainGeneration();
}