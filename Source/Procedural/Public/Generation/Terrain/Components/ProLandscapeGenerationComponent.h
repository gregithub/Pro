// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Generation/Noise/ProNoise.h"
#include "ProLandscapeGenerationComponent.generated.h"

class AProLandscapeChunk;

UCLASS(BlueprintType)
class PROCEDURAL_API UGenerationTerrainCurveSettings : public UDataAsset
{
	GENERATED_BODY()

	// Higher values correspond to more inland biomes.
	UPROPERTY(EditDefaultsOnly, Category = "Pro")
	UCurveFloat* Curve_Continentalness = nullptr;

	// Higher values correspond to more flat terrain.
	UPROPERTY(EditDefaultsOnly, Category = "Pro")
	UCurveFloat* Curve_Erosion = nullptr;

	// Used for generatting better peaks and valleys. Higher the value, higher the terrain.
	UPROPERTY(EditDefaultsOnly, Category = "Pro")
	UCurveFloat* Curve_PeaksAndValleys = nullptr;
};


USTRUCT(BlueprintType)
struct FGeneratedWorldLandscapeSettings
{
	GENERATED_BODY()

	FGeneratedWorldLandscapeSettings ()
	{
		RandomStream = FRandomStream(1);

		NoiseOffsets = FProNoiseOffsets(RandomStream);
	}

	//Todo: instead of 2d grid, use distance from player to make more oval terrain range
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	int32 Global_MapSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	float Global_WorldScale = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	float Global_ChunkSize = 100.0f;

	FProNoiseOffsets NoiseOffsets = FProNoiseOffsets();
	FRandomStream RandomStream;
public:
	bool IsValid() const { return true; };

	const FProNoiseOffsets& GetNoiseOffsets() const { return NoiseOffsets; };
};

class AProGameModeBase;

UCLASS(BlueprintType, ClassGroup = (Pro), meta = (BlueprintSpawnableComponent))
class PROCEDURAL_API UProLandscapeGenerationComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UGenerationTerrainCurveSettings* GenerationTerrainCurveSettings = nullptr;

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

};
