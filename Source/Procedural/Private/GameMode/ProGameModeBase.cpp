// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ProGameModeBase.h"
#include "ProceduralMeshComponent.h"
#include "GameMode/Components/ProWorldGenerationComponent.h"

AProGameModeBase::AProGameModeBase()
{
	WorldGenerationComponent = CreateDefaultSubobject<UProWorldGenerationComponent>(TEXT("ProWorldGenerationComponent"));
}

void AProGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) 
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AProGameModeBase::InitGameState()
{
	Super::InitGameState();
}

bool AProGameModeBase::RequestGenerateWorld()
{
	if (WorldGenerationComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("WorldGenerationComponent is invalid!"));
		return false;
	}

	WorldGenerationComponent->WorldGeneration_RequestTerrainGeneration();

	return true;
}