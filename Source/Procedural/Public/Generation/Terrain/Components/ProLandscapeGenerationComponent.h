// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProLandscapeGenerationComponent.generated.h"

class AProLandscapeChunk;


USTRUCT(BlueprintType)
struct FGeneratedWorldLandscapeSettings
{
	GENERATED_BODY()

	//Todo: instead of 2d grid, use distance from player to make more oval terrain range
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	int32 Global_MapSize = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	float Global_WorldScale = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	float Global_ChunkSize = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	float Global_UVScale = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	int32 ChunkVerticesPerAxis = 2;

public:
	bool IsValid() const { return (ChunkVerticesPerAxis >= 2); };
};

UCLASS(BlueprintType, ClassGroup = (Pro), meta = (BlueprintSpawnableComponent))
class PROCEDURAL_API UProLandscapeGenerationComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGeneratedWorldLandscapeSettings LandscapeSettings;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProLandscapeChunk> ChunkClass;

public:
	UProLandscapeGenerationComponent(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TickUpdateRequestedChunks();

	AProLandscapeChunk* RequestChunk(const FVector& InLocation);

	bool TryGenerateLandscapeSettings();

	const FGeneratedWorldLandscapeSettings& GetLandscapeSettings() const { return LandscapeSettings; };

	TMap<FIntVector2, AProLandscapeChunk*> CurrentChunks;

	bool bTempInitalizedChunks = false;

};
