// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/Terrain/ProTerrainGeneration.h"
#include "ProceduralMeshComponent.h"
#include "GameInstance/ProGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Generation/Terrain/Components/ProLandscapeGenerationComponent.h"

AProTerrainGeneration::AProTerrainGeneration()
{
	//NOTE: Is tick needed?
	PrimaryActorTick.bCanEverTick = true;

	ProLandscapeGenerationComponent = CreateDefaultSubobject<UProLandscapeGenerationComponent>(TEXT("ProWorldGenerationComponent"));

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	ProceduralMeshComponent->SetupAttachment(GetRootComponent());
	ProceduralMeshComponent->SetCastShadow(false);
	ProceduralMeshComponent->bUseAsyncCooking = true; //Async collision geometry

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
	if (ProceduralMeshComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ProceduralMeshComponent is invalid!"));
		return;
	}

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

	FGeneratedWorldLandscapeSettings TerrainSettings = ProLandscapeGenerationComponent->GetLandscapeSettings();

	if (TerrainSettings.IsValid() == false)
	{
		UE_LOG(LogTemp, Error, TEXT("TerrainSettings is invalid!"));
		return;
	}

	const int32 LocalSectionIndex = 0;

	ProceduralMeshComponent->ClearMeshSection(LocalSectionIndex);

	ProceduralMeshComponent->CreateMeshSection(
		LocalSectionIndex,
		TerrainSettings.Vertices,
		TerrainSettings.Triangles,
		TerrainSettings.Normals,
		TerrainSettings.UVs,
		TArray<FColor>(),
		TerrainSettings.Tangents,
		false);

	if (MaterialInterface != nullptr)
	{
		ProceduralMeshComponent->SetMaterial(0, MaterialInterface);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MaterialInterface is invalid!"));
		return;
	}
}

void AProTerrainGeneration::CallInEditor_RegenerateTerrain()
{
	RequestTerrainGeneration();
}