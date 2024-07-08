// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class PROCEDURAL_API ProNoise
{
public:
	ProNoise();
	~ProNoise();

	static float SinglePerling(const FVector& InLocation, int32 seed);

protected:

	// https://stackoverflow.com/questions/63211595/cubic-quintic-linear-interpolation
	static float InterpQuintic(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

protected:
};
