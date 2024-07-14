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
	}

	FProNoiseOffsets() {}

	void SetOffsets(FRandomStream& RandomStream)
	{
		X = RandomStream.VRand() * RandomOffsetScale;
		Y = RandomStream.VRand() * RandomOffsetScale;
		Z = RandomStream.VRand() * RandomOffsetScale;
	}

	FVector X, Y, Z;
private:
	// Offset scale chosen to keep reasonably small offsets while still de-correlating the noise in each dimension.
	static constexpr double RandomOffsetScale = 100.0;
};

USTRUCT(BlueprintType)
struct FProNoiseSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	float Amplitude = 2;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	float Frequency = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	int32 Octaves = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	float Persistence = 2.0f;

public:
	bool IsValid() const { return true; };
};

UCLASS(ClassGroup = Pro, meta = (BlueprintSpawnableComponent))
class PROCEDURAL_API UProNoiseComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FProNoiseSettings NoiseSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNoiseCurveSettings* NoiseCurveSettings = nullptr;

public:
	static UProNoiseComponent* GetInstance(const UObject* const WorldContextObject);

public:
	UProNoiseComponent();
	void BeginPlay() override;

	float SinglePerling_Continentalness(const FVector& InLocation);
	float SinglePerling_Erosion(const FVector& InLocation);
	float SinglePerling_PeaksANdValleys(const FVector& InLocation);

	const UNoiseCurveSettings* GetNoiseCurveSettings() const { return NoiseCurveSettings; };
protected:

	float OctaveNoise(const FVector& V) const;

	FVector NoiseVector(const FVector& Pos, const FProNoiseOffsets& Offsets) const;

protected:
	FProNoiseOffsets NoiseOffsets_Continentalness;
	FProNoiseOffsets NoiseOffsets_Erosion;
	FProNoiseOffsets NoiseOffsets_PeaksAndValleys;

};
