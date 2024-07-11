// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/Terrain/ProTerrainGeneration.h"
#include "ProceduralMeshComponent.h"
#include "GameInstance/ProGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Generation/Terrain/ProLandscapeChunk.h"
#include "Generation/Terrain/Components/ProLandscapeGenerationComponent.h"

AProTerrainGeneration::AProTerrainGeneration()
{
	PrimaryActorTick.bCanEverTick = false;

	ProLandscapeGenerationComponent = CreateDefaultSubobject<UProLandscapeGenerationComponent>(TEXT("ProWorldGenerationComponent"));
}

void AProTerrainGeneration::BeginPlay()
{
	Super::BeginPlay();
}

void AProTerrainGeneration::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProTerrainGeneration::RequestTerrainGeneration()
{
	if (ProLandscapeGenerationComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ProLandscapeGenerationComponent is invalid!"));
		return;
	}
	
	if (ProLandscapeGenerationComponent->TryGenerateLandscapeSettings() == false)
	{
		UE_LOG(LogTemp, Error, TEXT("TryGenerateLandscapeSettings failed!"));
		return;
	}

	const FGeneratedWorldLandscapeSettings& TerrainSettings = ProLandscapeGenerationComponent->GetLandscapeSettings();

	if (TerrainSettings.IsValid() == false)
	{
		UE_LOG(LogTemp, Error, TEXT("TerrainSettings is invalid!"));
		return;
	}
}

void AProTerrainGeneration::CallInEditor_RegenerateTerrain()
{
	RequestTerrainGeneration();
}