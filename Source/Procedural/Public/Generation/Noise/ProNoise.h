// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

class PROCEDURAL_API ProNoise
{

public:
	ProNoise();
	~ProNoise();

	static float SinglePerling(const FVector& InLocation, const FProNoiseOffsets& InNoiseOffsets);

protected:

	// https://stackoverflow.com/questions/63211595/cubic-quintic-linear-interpolation
	static float InterpQuintic(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

protected:
};
