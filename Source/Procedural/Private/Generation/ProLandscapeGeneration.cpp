// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/ProLandscapeGeneration.h"
#include "ProceduralMeshComponent.h"
#include "GameInstance/ProGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/Components/Subcomponents/ProTerrainGenerationSubcomponent.h"
#include "GameMode/Components/ProWorldGenerationComponent.h"

AProLandscapeGeneration::AProLandscapeGeneration()
{
	PrimaryActorTick.bCanEverTick = true;

	WorldGenerationComponent = CreateDefaultSubobject<UProWorldGenerationComponent>(TEXT("ProWorldGenerationComponent"));
	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
}

void AProLandscapeGeneration::BeginPlay()
{
	Super::BeginPlay();

	RequestTerrainGeneration();
}

void AProLandscapeGeneration::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProLandscapeGeneration::RequestTerrainGeneration()
{
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

	ProceduralMeshComponent->CreateMeshSection(
		0,
		GeneratedWorldTerrainSettings.Vertices,
		GeneratedWorldTerrainSettings.Triangles,
		GeneratedWorldTerrainSettings.Normals,
		GeneratedWorldTerrainSettings.UVs,
		TArray<FColor>(),
		GeneratedWorldTerrainSettings.Tangents,
		false);
}

void AProLandscapeGeneration::CallInEditor_RegenerateTerrain()
{
	RequestTerrainGeneration();
}