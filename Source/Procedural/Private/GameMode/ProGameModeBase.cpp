// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ProGameModeBase.h"
#include "Generation/Noise/ProNoiseComponent.h"

AProGameModeBase::AProGameModeBase()
{
	ProNoiseComponent = CreateDefaultSubobject<UProNoiseComponent>(TEXT("ProNoiseComponent"));
}

void AProGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) 
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AProGameModeBase::InitGameState()
{
	Super::InitGameState();
}

AProGameModeBase* AProGameModeBase::GetInstance(const UObject* const WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		return Cast<AProGameModeBase>(World->GetAuthGameMode());
	}

	return nullptr;
}