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

	RandomStream = FRandomStream(2);

	NoiseOffsets = FProNoiseOffsets(RandomStream);
}


float UProNoiseComponent::SinglePerling(const FVector& InLocation)
{
	const FVector OffsetedVector = NoiseSettings.NoiseVector(InLocation, NoiseOffsets);

	return NoiseSettings.OctaveNoise(OffsetedVector);
}
