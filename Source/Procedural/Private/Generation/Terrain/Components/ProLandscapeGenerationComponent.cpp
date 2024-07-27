// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/Terrain/Components/ProLandscapeGenerationComponent.h"
#include "Generation/Terrain/ProLandscapeChunk.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


UProLandscapeGenerationComponent::UProLandscapeGenerationComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.5f;
}

void UProLandscapeGenerationComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentChunks.Empty();
}

void UProLandscapeGenerationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickUpdateRequestedChunks();
}

void UProLandscapeGenerationComponent::TickUpdateRequestedChunks()
{
	const FGeneratedWorldLandscapeSettings& CurrentLandscapeSettings = GetLandscapeSettings();

	FIntVector2 CurrentCoordinates = FIntVector2(0, 0);

	if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		FVector2D PlayerLocation2D = FVector2D(PlayerCharacter->GetActorLocation());

		CurrentCoordinates = FIntVector2((PlayerLocation2D.X / CurrentLandscapeSettings.Global_ChunkSize), (PlayerLocation2D.Y / CurrentLandscapeSettings.Global_ChunkSize));
	}

	if ((LastCoordinates.IsSet()) && (LastCoordinates.GetValue() == CurrentCoordinates))
	{
		//Position didn't change, no need for update.
		return;
	}

	LastCoordinates = CurrentCoordinates;

	OnCoordinatesChanged.Broadcast(this);

	const int32 FirstChunkLocationX = (CurrentCoordinates.X - (CurrentLandscapeSettings.Global_MapSize / 2));
	const int32 FirstChunkLocationY = (CurrentCoordinates.Y - (CurrentLandscapeSettings.Global_MapSize / 2));

	TArray<FIntVector2> ChunksKeysToRemove;
	CurrentChunks.GetKeys(ChunksKeysToRemove);

	const float RadiusFromPlayer = (CurrentLandscapeSettings.Global_MapSize / 2);

	int32 NumOfCreatedChunksThisTick = 0;
	TOptional<float> MinNoiseValue;
	TOptional<float> MaxNoiseValue;

	for (int32 CurrentRow = FirstChunkLocationX; CurrentRow <= (FirstChunkLocationX + CurrentLandscapeSettings.Global_MapSize); CurrentRow++)
	{
		for (int32 CurrentColumn = FirstChunkLocationY; CurrentColumn <= (FirstChunkLocationY + CurrentLandscapeSettings.Global_MapSize); CurrentColumn++)
		{
			if (CurrentLandscapeSettings.bUseRadiusFromPlayer)
			{
				if (FVector2D::Distance(FVector2D(CurrentCoordinates.X, CurrentCoordinates.Y), FVector2D(CurrentRow, CurrentColumn)) > RadiusFromPlayer)
				{
					continue;
				}
			}
			
			const FIntVector2 CurrentChunkCoordinates = FIntVector2(CurrentRow, CurrentColumn);

			if (ChunksKeysToRemove.Contains(CurrentChunkCoordinates))
			{
				ChunksKeysToRemove.Remove(CurrentChunkCoordinates);
				continue;
			}

			const FVector ChunkLocation = FVector(float(CurrentRow * CurrentLandscapeSettings.Global_ChunkSize), float(CurrentColumn * CurrentLandscapeSettings.Global_ChunkSize), 0.0f);

			if (AProLandscapeChunk* CreatedChunk = RequestChunk(ChunkLocation))
			{
				if (bDebugNoiseValues)
				{
					NumOfCreatedChunksThisTick++;

					if (const float ChunkMinNoiseValue = CreatedChunk->GetMinNoiseTypeValue(ENoiseTerrainType::Continentalness))
					{
						if ((MinNoiseValue.IsSet() == false) || (ChunkMinNoiseValue < MinNoiseValue.GetValue()))
						{
							MinNoiseValue = ChunkMinNoiseValue;
						}
					}

					if (const float ChunkMaxNoiseValue = CreatedChunk->GetMaxNoiseTypeValue(ENoiseTerrainType::Continentalness))
					{
						if ((MaxNoiseValue.IsSet() == false) || (ChunkMaxNoiseValue > MaxNoiseValue.GetValue()))
						{
							MaxNoiseValue = ChunkMaxNoiseValue;
						}
					}
				}
				
				CurrentChunks.Add(CurrentChunkCoordinates, CreatedChunk);
			}
		}
	}

	if (bDebugNoiseValues)
	{
		if (NumOfCreatedChunksThisTick > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("NumOfCreatedChunksThisTick: %s"), *FString::FromInt(NumOfCreatedChunksThisTick));

			UE_LOG(LogTemp, Warning, TEXT("Continentalness - MinNoiseValue: %s; MaxNoiseValue: %s!"), 
				((MinNoiseValue.IsSet()) ? (*FString::SanitizeFloat(MinNoiseValue.GetValue())) :( *FString("Invalid value"))),
				((MaxNoiseValue.IsSet()) ? (*FString::SanitizeFloat(MaxNoiseValue.GetValue())) : (*FString("Invalid value"))));
		}
	}

	if (ChunksKeysToRemove.Num() > 0)
	{
		for (int32 ChunkToRemoveIterator = 0; ChunkToRemoveIterator < ChunksKeysToRemove.Num(); ChunkToRemoveIterator++)
		{
			AProLandscapeChunk* ChunkToBeRemoved;

			if (CurrentChunks.RemoveAndCopyValue(ChunksKeysToRemove[ChunkToRemoveIterator], ChunkToBeRemoved))
			{
				ChunkToBeRemoved->Destroy();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("RemoveChunk failed!"));
			}
		}
	}
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
		CreatedChunk->RequestCreateMeshSection(GetLandscapeSettings());

		return CreatedChunk;
	}

	return nullptr;
}

void UProLandscapeGenerationComponent::ClearAllChunks()
{
	if (CurrentChunks.Num() <= 0)
	{
		return;
	}

	for (TTuple<FIntVector2, AProLandscapeChunk*> MapTupple : CurrentChunks)
	{
		if (AProLandscapeChunk* ChunkToRemove = MapTupple.Value)
		{
			ChunkToRemove->Destroy();
		}
	}

	CurrentChunks.Empty();
	LastCoordinates.Reset();
}

FVector UProLandscapeGenerationComponent::GetCurrentCenterPosition() const
{
	if (LastCoordinates.IsSet())
	{
		return FVector(LastCoordinates.GetValue().X * GetLandscapeSettings().Global_ChunkSize, LastCoordinates.GetValue().Y * GetLandscapeSettings().Global_ChunkSize, 0.0f);
	}

	return FVector::ZeroVector;
}