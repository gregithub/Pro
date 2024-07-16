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

	const FVector OffsetedVector = NoiseVector(InLocation, InNoiseSettings);

	return OctaveNoise(OffsetedVector, InNoiseSettings);
}

FVector UProNoiseComponent::NoiseVector(const FVector& Pos, const FProNoiseSettings& InNoiseSettings) const
{
	FVector Base = Pos * InNoiseSettings.Frequency;
	return FVector(
		OctaveNoise(Base + InNoiseSettings.NoiseOffsets.X, InNoiseSettings),
		OctaveNoise(Base + InNoiseSettings.NoiseOffsets.Y, InNoiseSettings),
		OctaveNoise(Base + InNoiseSettings.NoiseOffsets.Z, InNoiseSettings)
	) * InNoiseSettings.Amplitude;
}

float UProNoiseComponent::OctaveNoise(const FVector& Pos, const FProNoiseSettings& InNoiseSettings) const
{
	float NoiseValue = 0;

	if (InNoiseSettings.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoiseSettings is invalid!"));
		return NoiseValue;
	}

	float LocalFrequency = InNoiseSettings.Frequency;
	float LocalAmplitude = InNoiseSettings.Amplitude;
	float MaxValue = 0;

	for (int32 Octave = 0; Octave < InNoiseSettings.Octaves; Octave++)
	{
		NoiseValue += FMath::PerlinNoise3D(Pos * LocalFrequency) * LocalAmplitude;

		MaxValue += LocalAmplitude;
		LocalAmplitude *= InNoiseSettings.Persistence;
		LocalFrequency *= 2;
	}
	return NoiseValue / MaxValue;
}
