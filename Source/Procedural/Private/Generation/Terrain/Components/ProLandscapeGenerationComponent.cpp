// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/Terrain/Components/ProLandscapeGenerationComponent.h"
#include "ProceduralMeshComponent.h"
#include "Generation/Terrain/ProLandscapeChunk.h"
#include "GameInstance/ProGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/ProGameModeBase.h"
#include "KismetProceduralMeshLibrary.h"
#include "LandscapeComponent.h"

UProLandscapeGenerationComponent::UProLandscapeGenerationComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UProLandscapeGenerationComponent::BeginPlay()
{
	Super::BeginPlay();

	TickUpdateRequestedChunks();
}

void UProLandscapeGenerationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UProLandscapeGenerationComponent::TickUpdateRequestedChunks()
{
	//Todo: this should be player coordinates
	const FIntVector2 TempCordinates = FIntVector2(0, 0);

	const FGeneratedWorldLandscapeSettings& CurrentLandscapeSettings = GetLandscapeSettings();

	const int32 FirstChunkLocationX = (TempCordinates.X - (CurrentLandscapeSettings.Global_MapSize / 2));
	const int32 FirstChunkLocationY = (TempCordinates.X - (CurrentLandscapeSettings.Global_MapSize / 2));

	for (int32 CurrentRow = FirstChunkLocationX; CurrentRow <= (FirstChunkLocationX + CurrentLandscapeSettings.Global_MapSize); CurrentRow++)
	{
		for (int32 CurrentColumn = FirstChunkLocationY; CurrentColumn <= (FirstChunkLocationY + CurrentLandscapeSettings.Global_MapSize); CurrentColumn++)
		{
			const FVector ChunkLocation = FVector(float(CurrentRow * CurrentLandscapeSettings.Global_ChunkSize), float(CurrentColumn * CurrentLandscapeSettings.Global_ChunkSize), 0.0f);

			if (AProLandscapeChunk* CreatedChunk = RequestChunk(ChunkLocation))
			{
				CurrentChunks.Add(FIntVector2(CurrentRow, CurrentColumn), CreatedChunk);
			}
		}
	}
}

bool UProLandscapeGenerationComponent::TryGenerateLandscapeSettings()
{
	const int RandomSeed = FMath::RandRange(10000,99999);

	return true;
}

AProLandscapeChunk* UProLandscapeGenerationComponent::RequestChunk(const FVector& InLocation)
{
	if (ChunkClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChunkClass is invalid!"));
		return nullptr;
	}

	const FTransform ChunkTransform = FTransform(FRotator::ZeroRotator, InLocation, FVector::OneVector);

	if (AProLandscapeChunk* CreatedChunk = GetWorld()->SpawnActor<AProLandscapeChunk>(ChunkClass, ChunkTransform))
	{
		CreatedChunk->RequestCreateMeshSection(InLocation, GetLandscapeSettings());

		return CreatedChunk;
	}

	return nullptr;
}
