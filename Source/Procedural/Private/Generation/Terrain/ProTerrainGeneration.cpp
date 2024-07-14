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
}

void AProTerrainGeneration::CallInEditor_RegenerateTerrain()
{
	RequestTerrainGeneration();
}