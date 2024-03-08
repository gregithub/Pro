// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ProGameModeBase.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "GameMode/Components/ProWorldGenerationComponent.h"

AProGameModeBase::AProGameModeBase()
{
	WorldGenerationComponent = CreateDefaultSubobject<UProWorldGenerationComponent>(TEXT("ProWorldGenerationComponent"));
	
	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
}

void AProGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) 
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AProGameModeBase::InitGameState()
{
	Super::InitGameState();

	WorldGenerationComponent->Initialize(this);
}

bool AProGameModeBase::RequestGenerateWorld()
{
	if (WorldGenerationComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("WorldGenerationComponent is invalid!"));
		return false;
	}

	WorldGenerationComponent->RequestTerrainGeneration();

	return true;
}