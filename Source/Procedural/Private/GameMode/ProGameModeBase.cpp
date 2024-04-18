// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ProGameModeBase.h"
#include "ProceduralMeshComponent.h"
#include "GameMode/Components/ProWorldGenerationComponent.h"

AProGameModeBase::AProGameModeBase()
{
}

void AProGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) 
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AProGameModeBase::InitGameState()
{
	Super::InitGameState();
}