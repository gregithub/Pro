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

	UpdateOffsets();
}

float UProNoiseComponent::OctaveNoise2D(const FVector2D& Pos, const ENoiseTerrainType InNoiseTerrainType) const
{
	float NoiseValue = 0.0f;

	const FProNoiseSettings& NoiseTypeSettings = GetNoiseSettingsType(InNoiseTerrainType);

	if (NoiseTypeSettings.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoiseTypeSettings is invalid!"));
		return NoiseValue;
	}

	float LocalFrequency = NoiseTypeSettings.Frequency;
	float LocalAmplitude = NoiseTypeSettings.Amplitude;

	const FVector2D OffsetedPosition = 
		Pos + FVector2D(NoiseTypeSettings.NoiseOffsets.X.X, NoiseTypeSettings.NoiseOffsets.X.Y);

	for (int32 Octave = 0; Octave < NoiseTypeSettings.Octaves; Octave++)
	{
		NoiseValue += FMath::PerlinNoise2D(OffsetedPosition * LocalFrequency) * LocalAmplitude;

		LocalAmplitude *= NoiseTypeSettings.Persistence;
		LocalFrequency *= 2;
	}

	return FMath::Clamp(NoiseValue, -1.0f, 1.0f);
}

void UProNoiseComponent::UpdateOffsets()
{
	if (NoiseSettings_Continentalness.bGenerateRandomOffsets)
	{
		NoiseSettings_Continentalness.GenerateOffsets(FMath::Rand());
	}
	else
	{
		NoiseSettings_Continentalness.GenerateOffsets(NoiseSettings_Continentalness.OffsetsSeed);
	}

	if (NoiseSettings_Erosion.bGenerateRandomOffsets)
	{
		NoiseSettings_Erosion.GenerateOffsets(FMath::Rand());
	}
	else
	{
		NoiseSettings_Erosion.GenerateOffsets(NoiseSettings_Erosion.OffsetsSeed);
	}

	if (NoiseSettings_PeaksAndValleys.bGenerateRandomOffsets)
	{
		NoiseSettings_PeaksAndValleys.GenerateOffsets(FMath::Rand());
	}
	else
	{
		NoiseSettings_PeaksAndValleys.GenerateOffsets(NoiseSettings_PeaksAndValleys.OffsetsSeed);
	}
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

	return FMath::Lerp(
		FMath::Lerp(Grad2(Permutation[AA], X, Y), Grad2(Permutation[BA], Xm1, Y), U),
		FMath::Lerp(Grad2(Permutation[AB], X, Ym1), Grad2(Permutation[BB], Xm1, Ym1), U),
		V);
}

float UProNoiseComponent::Grad2(int32 Hash, float X, float Y) const
{
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

float UProNoiseComponent::Fade(const float InValue) const
{
	return (InValue * InValue * InValue * (InValue * (InValue * 6 - 15) + 10)); // 6t^5 - 15t^4 + 10t^3
}

const float UProNoiseComponent::GetWorldHeightValueFromNoise(const float InNoiseValue, const ENoiseTerrainType InNoiseTerrainType) const
{
	float ReturnValue = 0.0f;

	if (NoiseCurveSettings == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetNoiseCurveSettings is invalid!"));
		return ReturnValue;
	}

	if (const UCurveFloat* NoiseCurveFloat = NoiseCurveSettings->GetNoiseCurveType(InNoiseTerrainType))
	{
		ReturnValue = NoiseCurveFloat->GetFloatValue(InNoiseValue);
	}

	return ReturnValue;
}

const FProNoiseSettings& UProNoiseComponent::GetNoiseSettingsType(const ENoiseTerrainType InTerrainType) const
{
	switch (InTerrainType)
	{
	case ENoiseTerrainType::Continentalness:
		return GetNoiseSettings_Continentalness();
		break;
	case ENoiseTerrainType::Erosion:
		return GetNoiseSettings_Erosion();
		break;
	case ENoiseTerrainType::PeaksAndValleys:
		return GetNoiseSettings_PeaksAndValleys();
		break;
	default:
		break;
	}

	// Should never happen;
	ensure(false);
	return GetNoiseSettings_Continentalness();
}