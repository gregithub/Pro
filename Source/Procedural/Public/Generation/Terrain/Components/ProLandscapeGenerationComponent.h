// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Generation/Noise/ProNoiseComponent.h"
#include "ProLandscapeGenerationComponent.generated.h"

class AProLandscapeChunk;


USTRUCT(BlueprintType)
struct FGeneratedWorldLandscapeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	int32 Global_MapSize = 40;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	float Global_ChunkSize = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	int32 ChunkVerticesPerAxis = 3;

public:
	bool IsValid() const { return (ChunkVerticesPerAxis >= 2); };
};

UCLASS(BlueprintType, ClassGroup = (Pro), meta = (BlueprintSpawnableComponent))
class PROCEDURAL_API UProLandscapeGenerationComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pro")
	FGeneratedWorldLandscapeSettings LandscapeSettings;

	UPROPERTY(EditDefaultsOnly, Category = "Pro")
	TSubclassOf<AProLandscapeChunk> ChunkClass;

public:
	UProLandscapeGenerationComponent(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TickUpdateRequestedChunks();

	const FGeneratedWorldLandscapeSettings& GetLandscapeSettings() const { return LandscapeSettings; };

	void ClearAllChunks();

protected:
	AProLandscapeChunk* RequestChunk(const FVector& InLocation);

protected:
	TMap<FIntVector2, AProLandscapeChunk*> CurrentChunks;

};
