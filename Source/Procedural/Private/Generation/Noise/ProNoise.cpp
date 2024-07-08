// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/Noise/ProNoise.h"
#include "Math.h"

ProNoise::ProNoise()
{
}

ProNoise::~ProNoise()
{
}

float ProNoise::SinglePerling(const FVector& InLocation, int32 seed)
{
    float ReturnValue = FMath::PerlinNoise3D(InLocation);

    return ReturnValue;
}