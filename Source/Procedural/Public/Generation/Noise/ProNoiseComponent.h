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

struct FProNoiseSettings
{
	float Amplitude = 2;
	float Frequency = .1;
	int32 Octaves = 4;
	float PointSpacing = 1;
	float Lacunarity = 2;
	float Persistence = .5;

	FVector NoiseVector(const FVector& Pos, const FProNoiseOffsets& Offsets) const
	{
		FVector Base = Pos * Frequency;
		return FVector(
			OctaveNoise(Base + Offsets.X),
			OctaveNoise(Base + Offsets.Y),
			OctaveNoise(Base + Offsets.Z)
		) * Amplitude;
	}

	float OctaveNoise(const FVector& V) const
	{
		float NoiseValue = 0;
		float FreqScale = 1;
		float AmpScale = 1;
		for (int32 Octave = 0; Octave < Octaves; Octave++, FreqScale *= Lacunarity, AmpScale *= Persistence)
		{
			NoiseValue += FMath::PerlinNoise3D(V * FreqScale) * AmpScale;
		}
		return NoiseValue;
	}
};

UCLASS(ClassGroup = Pro, meta = (BlueprintSpawnableComponent))
class PROCEDURAL_API UProNoiseComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNoiseCurveSettings* NoiseCurveSettings = nullptr;

public:
	static UProNoiseComponent* GetInstance(const UObject* const WorldContextObject);

public:
	UProNoiseComponent();
	void BeginPlay() override;

	float SinglePerling(const FVector& InLocation);

	const FProNoiseOffsets& GetNoiseOffsets() const { return NoiseOffsets; };
	const UNoiseCurveSettings* GetNoiseCurveSettings() const { return NoiseCurveSettings; };
protected:

	// https://stackoverflow.com/questions/63211595/cubic-quintic-linear-interpolation
	static float InterpQuintic(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

protected:
	FProNoiseSettings NoiseSettings;
	FRandomStream RandomStream;
	FProNoiseOffsets NoiseOffsets;
};
