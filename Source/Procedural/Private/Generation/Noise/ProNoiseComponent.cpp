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

	FRandomStream RandomStream_Continentalness = FRandomStream(2);
	FRandomStream RandomStream_Erosion = FRandomStream(3);
	FRandomStream RandomStream_PeaksAndValleys = FRandomStream(4);

	NoiseOffsets_Continentalness = FProNoiseOffsets(RandomStream_Continentalness);
	NoiseOffsets_Erosion = FProNoiseOffsets(RandomStream_Erosion);
	NoiseOffsets_PeaksAndValleys = FProNoiseOffsets(RandomStream_PeaksAndValleys);
}


float UProNoiseComponent::SinglePerling_Continentalness(const FVector& InLocation)
{
	const FVector OffsetedVector = NoiseVector(InLocation, NoiseOffsets_Continentalness);

	return OctaveNoise(OffsetedVector);
}

float UProNoiseComponent::SinglePerling_Erosion(const FVector& InLocation)
{
	const FVector OffsetedVector = NoiseVector(InLocation, NoiseOffsets_Erosion);

	return OctaveNoise(OffsetedVector);
}

float UProNoiseComponent::SinglePerling_PeaksANdValleys(const FVector& InLocation)
{
	const FVector OffsetedVector = NoiseVector(InLocation, NoiseOffsets_PeaksAndValleys);

	return OctaveNoise(OffsetedVector);
}

FVector UProNoiseComponent::NoiseVector(const FVector& Pos, const FProNoiseOffsets& Offsets) const
{
	if (NoiseSettings.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoiseSettings is invalid!"));
		return Pos;
	}

	FVector Base = Pos * NoiseSettings.Frequency;
	return FVector(
		OctaveNoise(Base + Offsets.X),
		OctaveNoise(Base + Offsets.Y),
		OctaveNoise(Base + Offsets.Z)
	) * NoiseSettings.Amplitude;
}

float UProNoiseComponent::OctaveNoise(const FVector& V) const
{
	float NoiseValue = 0;

	if (NoiseSettings.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoiseSettings is invalid!"));
		return NoiseValue;
	}

	float LocalFrequency = NoiseSettings.Frequency;
	float LocalAmplitude = NoiseSettings.Amplitude;
	float MaxValue = 0;

	for (int32 Octave = 0; Octave < NoiseSettings.Octaves; Octave++)
	{
		NoiseValue += FMath::PerlinNoise3D(V * LocalFrequency) * LocalAmplitude;

		MaxValue += LocalAmplitude;
		LocalAmplitude *= NoiseSettings.Persistence;
		LocalFrequency *= 2;
	}
	return NoiseValue / MaxValue;
}
