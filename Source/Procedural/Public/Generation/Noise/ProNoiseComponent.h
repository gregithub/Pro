// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ProNoiseComponent.generated.h"

UCLASS(BlueprintType)
class PROCEDURAL_API UNoiseCurveSettings : public UDataAsset
{
	GENERATED_BODY()

	// Higher values correspond to more inland biomes.
	UPROPERTY(EditDefaultsOnly, Category = "Pro")
	UCurveFloat* Curve_Continentalness = nullptr;

	// Higher values correspond to more flat terrain.
	UPROPERTY(EditDefaultsOnly, Category = "Pro")
	UCurveFloat* Curve_Erosion = nullptr;

	// Used for generating better peaks and valleys. Higher the value, higher the terrain.
	UPROPERTY(EditDefaultsOnly, Category = "Pro")
	UCurveFloat* Curve_PeaksAndValleys = nullptr;
public:
	const UCurveFloat* GetCurve_Continentalness() const { return Curve_Continentalness; };
	const UCurveFloat* GetCurve_Erosion() const { return Curve_Erosion; };
	const UCurveFloat* GetCurve_PeaksAndValleys() const { return Curve_PeaksAndValleys; };
};

// Offsets to randomize where Perlin noise is sampled for each dimension of a noise vector
struct FProNoiseOffsets
{
	FProNoiseOffsets(FRandomStream& RandomStream)
		: X(RandomStream.VRand()* RandomOffsetScale), Y(RandomStream.VRand()* RandomOffsetScale), Z(RandomStream.VRand()* RandomOffsetScale)
	{
		bInitialized = true;
	}

	FProNoiseOffsets() {}

	void SetOffsets(FRandomStream& RandomStream)
	{
		X = RandomStream.VRand() * RandomOffsetScale;
		Y = RandomStream.VRand() * RandomOffsetScale;
		Z = RandomStream.VRand() * RandomOffsetScale;
	}

	bool IsInitalized() const { return bInitialized; };

	FVector X, Y, Z;

private:
	// Offset scale chosen to keep reasonably small offsets while still de-correlating the noise in each dimension.
	static constexpr double RandomOffsetScale = 100.0;
	bool bInitialized = false;

};

USTRUCT(BlueprintType)
struct FProNoiseSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	bool bApplyNoise = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	float Amplitude = 2;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	float Frequency = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	int32 Octaves = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	float Persistence = 2.0f;

	FProNoiseOffsets NoiseOffsets;

public:
	void GenerateOffsets(const int32 InSeed)
	{
		FRandomStream RandomStream = FRandomStream(InSeed);

		NoiseOffsets = FProNoiseOffsets(RandomStream);
	}

	bool IsValid() const { return NoiseOffsets.IsInitalized(); };
	bool GetApplyNoise() const { return bApplyNoise; };
};


UCLASS(ClassGroup = Pro, meta = (BlueprintSpawnableComponent))
class PROCEDURAL_API UProNoiseComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNoiseCurveSettings* NoiseCurveSettings = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	FProNoiseSettings NoiseSettings_Continentalness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	FProNoiseSettings NoiseSettings_Erosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	FProNoiseSettings NoiseSettings_PeaksAndValleys;

public:
	static UProNoiseComponent* GetInstance(const UObject* const WorldContextObject);

public:
	UProNoiseComponent();
	void BeginPlay() override;

	float SinglePerling(const FVector& InLocation, const FProNoiseSettings& InNoiseSettings);

	const UNoiseCurveSettings* GetNoiseCurveSettings() const { return NoiseCurveSettings; };

	const FProNoiseSettings& GetNoiseSettings_Continentalness()  const { return NoiseSettings_Continentalness; };
	const FProNoiseSettings& GetNoiseSettings_Erosion()  const { return NoiseSettings_Erosion; };
	const FProNoiseSettings& GetNoiseSettings_PeaksAndValleys() const { return NoiseSettings_PeaksAndValleys; };

protected:
	float OctaveNoise(const FVector& Pos, const FProNoiseSettings& InNoiseSettings) const;

	FVector NoiseVector(const FVector& Pos, const FProNoiseSettings& InNoiseSettings) const;

};
