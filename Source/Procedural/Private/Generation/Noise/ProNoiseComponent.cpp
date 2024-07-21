// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/Noise/ProNoiseComponent.h"
#include "GameMode/ProGameModeBase.h"
#include "Math.h"

UProNoiseComponent::UProNoiseComponent()
{
}

UProNoiseComponent* UProNoiseComponent::GetInstance(const UObject* const WorldContextObject)
{
	if (AProGameModeBase* PlayGameModeBase = AProGameModeBase::GetInstance(WorldContextObject))
	{
		return &PlayGameModeBase->GetProNoiseComponent();
	}

	return nullptr;
}

void UProNoiseComponent::BeginPlay()
{
	Super::BeginPlay();

	NoiseSettings_Continentalness.GenerateOffsets(2);
	NoiseSettings_Erosion.GenerateOffsets(3);
	NoiseSettings_PeaksAndValleys.GenerateOffsets(4);
}

float UProNoiseComponent::SinglePerling(const FVector& InLocation, const FProNoiseSettings& InNoiseSettings)
{
	if (InNoiseSettings.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoiseSettings is invalid!"));
		return 0.0f;
	}

	return ProNoise2D(FVector2D(InLocation.X, InLocation.Y), InNoiseSettings);
}

float UProNoiseComponent::ProNoise2D(const FVector2D& InLocation, const FProNoiseSettings& InNoiseSettings) const
{
	float NoiseValue = 0.0f; 

	if (InNoiseSettings.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoiseSettings is invalid!"));
		return NoiseValue;
	}

	const FVector2D OffsettedVector = InLocation + InNoiseSettings.VectorOffset;

	return OctaveNoise2D(OffsettedVector, InNoiseSettings);
}

float UProNoiseComponent::OctaveNoise2D(const FVector2D& Pos, const FProNoiseSettings& InNoiseSettings) const
{
	float NoiseValue = 0.0f;

	if (InNoiseSettings.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoiseSettings is invalid!"));
		return NoiseValue;
	}

	float LocalFrequency = InNoiseSettings.Frequency;
	float LocalAmplitude = InNoiseSettings.Amplitude;
	float MaxValue = 0.0f;

	for (int32 Octave = 0; Octave < InNoiseSettings.Octaves; Octave++)
	{
		NoiseValue += ProPerlinNoise2D(Pos * LocalFrequency) * LocalAmplitude;

		MaxValue += LocalAmplitude;
		LocalAmplitude *= InNoiseSettings.Persistence;
		LocalFrequency *= 2;
	}

	return (NoiseValue / MaxValue);
}

float UProNoiseComponent::ProPerlinNoise2D(const FVector2D& Location) const
{
	float Xfl = FMath::FloorToFloat((float)Location.X);
	float Yfl = FMath::FloorToFloat((float)Location.Y);
	int32 Xi = (int32)(Xfl) & 255;
	int32 Yi = (int32)(Yfl) & 255;
	float X = (float)Location.X - Xfl;
	float Y = (float)Location.Y - Yfl;
	float Xm1 = X - 1.0f;
	float Ym1 = Y - 1.0f;

	int32 AA = Permutation[Xi] + Yi;
	int32 AB = AA + 1;
	int32 BA = Permutation[Xi + 1] + Yi;
	int32 BB = BA + 1;

	float U = Fade(X);
	float V = Fade(Y);

	// Note: Due to the choice of Grad2, this will be in the (-1,1) range with no additional scaling
	return FMath::Lerp(
		FMath::Lerp(Grad2(Permutation[AA], X, Y), Grad2(Permutation[BA], Xm1, Y), U),
		FMath::Lerp(Grad2(Permutation[AB], X, Ym1), Grad2(Permutation[BB], Xm1, Ym1), U),
		V);
}

float UProNoiseComponent::Grad2(int32 Hash, float X, float Y) const
{
	// corners and major axes (similar to the z=0 projection of the cube-edge-midpoint sampling from improved Perlin noise)
	switch (Hash & 7)
	{
	case 0: return X;
	case 1: return X + Y;
	case 2: return Y;
	case 3: return -X + Y;
	case 4: return -X;
	case 5: return -X - Y;
	case 6: return -Y;
	case 7: return X - Y;
		// can't happen
	default: return 0;
	}
}