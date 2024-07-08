// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/Noise/ProNoise.h"
#include "Math.h"

ProNoise::ProNoise()
{
}

ProNoise::~ProNoise()
{
}

float ProNoise::SinglePerling(const FVector& InLocation, const FProNoiseOffsets& InNoiseOffsets)
{
	FProNoiseSettings Settings = FProNoiseSettings();

	const FVector OffsetedVector = Settings.NoiseVector(InLocation, InNoiseOffsets);

	return Settings.OctaveNoise(OffsetedVector);
}