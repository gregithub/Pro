// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ProGameModeBase.h"
#include "GameFramework/Character.h"
#include "Generation/Noise/ProNoiseComponent.h"
#include "Generation/Terrain/Components/ProLandscapeGenerationComponent.h"
#include "Kismet/GameplayStatics.h"

AProGameModeBase::AProGameModeBase()
{
	ProNoiseComponent = CreateDefaultSubobject<UProNoiseComponent>(TEXT("ProNoiseComponent"));

	ProLandscapeGenerationComponent = CreateDefaultSubobject<UProLandscapeGenerationComponent>(TEXT("ProWorldGenerationComponent"));
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


void AProGameModeBase::RequestRecalculateTerrain()
{
	if (ProLandscapeGenerationComponent != nullptr)
	{
		ProLandscapeGenerationComponent->ClearAllChunks();

		if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			PlayerCharacter->SetActorLocation(FVector(0.0f, 0.0f, 10000.0f));
		}
	}
}