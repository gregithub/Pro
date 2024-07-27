// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ProNoiseComponent.generated.h"

UENUM(BlueprintType)
enum class ENoiseTerrainType : uint8
{
	Continentalness = 0,
	Erosion = 1,
	PeaksAndValleys = 2,
};

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
	const UCurveFloat* GetNoiseCurveType(const ENoiseTerrainType InTerrainType) const
	{
		switch (InTerrainType)
		{
		case ENoiseTerrainType::Continentalness:
			return GetCurve_Continentalness();
			break;
		case ENoiseTerrainType::Erosion:
			return GetCurve_Erosion();
			break;
		case ENoiseTerrainType::PeaksAndValleys:
			return GetCurve_PeaksAndValleys();
			break;
		default:
			break;
		}
		return nullptr;
	}

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
		bInitialized = true;
	}

	FProNoiseOffsets() {}

	void SetOffsets(FRandomStream& RandomStream)
	{
		X = RandomStream.VRand() * RandomOffsetScale;
		Y = RandomStream.VRand() * RandomOffsetScale;
		Z = RandomStream.VRand() * RandomOffsetScale;
	}

	bool IsInitalized() const { return bInitialized; };

	FVector X, Y, Z;

private:
	// Offset scale chosen to keep reasonably small offsets while still de-correlating the noise in each dimension.
	static constexpr double RandomOffsetScale = 100.0;
	bool bInitialized = false;

};

USTRUCT(BlueprintType)
struct FProNoiseSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	bool bApplyNoise = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	float Amplitude = 2;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	float Frequency = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	int32 Octaves = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	float Persistence = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	FVector2D VectorOffset = FVector2D(0.5f, 0.5f);

	FProNoiseOffsets NoiseOffsets;

public:
	void GenerateOffsets(const int32 InSeed)
	{
		FRandomStream RandomStream = FRandomStream(InSeed);

		NoiseOffsets = FProNoiseOffsets(RandomStream);
	}

	bool IsValid() const { return NoiseOffsets.IsInitalized(); };
	bool GetApplyNoise() const { return bApplyNoise; };
};


UCLASS(ClassGroup = Pro, meta = (BlueprintSpawnableComponent))
class PROCEDURAL_API UProNoiseComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNoiseCurveSettings* NoiseCurveSettings = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	FProNoiseSettings NoiseSettings_Continentalness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	FProNoiseSettings NoiseSettings_Erosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pro|Noise")
	FProNoiseSettings NoiseSettings_PeaksAndValleys;

public:
	static UProNoiseComponent* GetInstance(const UObject* const WorldContextObject);

public:
	UProNoiseComponent();
	void BeginPlay() override;

	float GetNoiseTerrainTypeValue(const FVector2D& InLocation, const ENoiseTerrainType InNoiseTerrainType);

	const UNoiseCurveSettings* GetNoiseCurveSettings() const { return NoiseCurveSettings; };

	const FProNoiseSettings& GetNoiseSettingsType(const ENoiseTerrainType InTerrainType) const;

	const FProNoiseSettings& GetNoiseSettings_Continentalness()  const { return NoiseSettings_Continentalness; };
	const FProNoiseSettings& GetNoiseSettings_Erosion()  const { return NoiseSettings_Erosion; };
	const FProNoiseSettings& GetNoiseSettings_PeaksAndValleys() const { return NoiseSettings_PeaksAndValleys; };

protected:
	float CalcNoise2D(const FVector2D& InLocation, const FProNoiseSettings& InNoiseSettings) const;

	float OctaveNoise2D(const FVector2D& Pos, const FProNoiseSettings& InNoiseSettings) const;

	float ProPerlinNoise2D(const FVector2D& Location) const;

	float Fade(const float InValue) const;

	float Grad2(int32 Hash, float X, float Y) const;

	// random permutation of 256 numbers, repeated 2x
	const int32 Permutation[512] = {
		63, 9, 212, 205, 31, 128, 72, 59, 137, 203, 195, 170, 181, 115, 165, 40, 116, 139, 175, 225, 132, 99, 222, 2, 41, 15, 197, 93, 169, 90, 228, 43, 221, 38, 206, 204, 73, 17, 97, 10, 96, 47, 32, 138, 136, 30, 219,
		78, 224, 13, 193, 88, 134, 211, 7, 112, 176, 19, 106, 83, 75, 217, 85, 0, 98, 140, 229, 80, 118, 151, 117, 251, 103, 242, 81, 238, 172, 82, 110, 4, 227, 77, 243, 46, 12, 189, 34, 188, 200, 161, 68, 76, 171, 194,
		57, 48, 247, 233, 51, 105, 5, 23, 42, 50, 216, 45, 239, 148, 249, 84, 70, 125, 108, 241, 62, 66, 64, 240, 173, 185, 250, 49, 6, 37, 26, 21, 244, 60, 223, 255, 16, 145, 27, 109, 58, 102, 142, 253, 120, 149, 160,
		124, 156, 79, 186, 135, 127, 14, 121, 22, 65, 54, 153, 91, 213, 174, 24, 252, 131, 192, 190, 202, 208, 35, 94, 231, 56, 95, 183, 163, 111, 147, 25, 67, 36, 92, 236, 71, 166, 1, 187, 100, 130, 143, 237, 178, 158,
		104, 184, 159, 177, 52, 214, 230, 119, 87, 114, 201, 179, 198, 3, 248, 182, 39, 11, 152, 196, 113, 20, 232, 69, 141, 207, 234, 53, 86, 180, 226, 74, 150, 218, 29, 133, 8, 44, 123, 28, 146, 89, 101, 154, 220, 126,
		155, 122, 210, 168, 254, 162, 129, 33, 18, 209, 61, 191, 199, 157, 245, 55, 164, 167, 215, 246, 144, 107, 235,

		63, 9, 212, 205, 31, 128, 72, 59, 137, 203, 195, 170, 181, 115, 165, 40, 116, 139, 175, 225, 132, 99, 222, 2, 41, 15, 197, 93, 169, 90, 228, 43, 221, 38, 206, 204, 73, 17, 97, 10, 96, 47, 32, 138, 136, 30, 219,
		78, 224, 13, 193, 88, 134, 211, 7, 112, 176, 19, 106, 83, 75, 217, 85, 0, 98, 140, 229, 80, 118, 151, 117, 251, 103, 242, 81, 238, 172, 82, 110, 4, 227, 77, 243, 46, 12, 189, 34, 188, 200, 161, 68, 76, 171, 194,
		57, 48, 247, 233, 51, 105, 5, 23, 42, 50, 216, 45, 239, 148, 249, 84, 70, 125, 108, 241, 62, 66, 64, 240, 173, 185, 250, 49, 6, 37, 26, 21, 244, 60, 223, 255, 16, 145, 27, 109, 58, 102, 142, 253, 120, 149, 160,
		124, 156, 79, 186, 135, 127, 14, 121, 22, 65, 54, 153, 91, 213, 174, 24, 252, 131, 192, 190, 202, 208, 35, 94, 231, 56, 95, 183, 163, 111, 147, 25, 67, 36, 92, 236, 71, 166, 1, 187, 100, 130, 143, 237, 178, 158,
		104, 184, 159, 177, 52, 214, 230, 119, 87, 114, 201, 179, 198, 3, 248, 182, 39, 11, 152, 196, 113, 20, 232, 69, 141, 207, 234, 53, 86, 180, 226, 74, 150, 218, 29, 133, 8, 44, 123, 28, 146, 89, 101, 154, 220, 126,
		155, 122, 210, 168, 254, 162, 129, 33, 18, 209, 61, 191, 199, 157, 245, 55, 164, 167, 215, 246, 144, 107, 235
	};
};
