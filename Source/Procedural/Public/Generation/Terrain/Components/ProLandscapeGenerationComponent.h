// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProceduralMeshComponent.h"
#include "ProLandscapeGenerationComponent.generated.h"


USTRUCT(BlueprintType)
struct FGeneratedWorldLandscapeSettings
{
	GENERATED_BODY()

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;

	FGeneratedWorldLandscapeSettings()
	{
		Vertices = TArray<FVector>();
		Triangles = TArray<int32>();
		Normals = TArray<FVector>();
		UVs = TArray<FVector2D>();
		Tangents = TArray<FProcMeshTangent>();
	}

	FGeneratedWorldLandscapeSettings(
		FVector2D InGridSize,
		float InWorldScale,
		float InCellsSize,
		TArray<FVector> InVertices,
		TArray<int32> InTriangles,
		TArray<FVector> InNormals,
		TArray<FVector2D> InUVs,
		TArray<FProcMeshTangent> InTangents
	)
	{
		Vertices = InVertices;
		Triangles = InTriangles;
		Normals = InNormals;
		UVs = InUVs;
		Tangents = InTangents;
	}

public:
	bool IsValid() const { return true; };
};

class AProGameModeBase;

UCLASS(BlueprintType, ClassGroup = (Pro), meta = (BlueprintSpawnableComponent))
class PROCEDURAL_API UProLandscapeGenerationComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	FVector2D GridSize = FVector2D(100, 100);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	float WorldScale = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	float CellsSize = 100.0f;

public:	
	UProLandscapeGenerationComponent(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool TryGenerateLandscapeSettings();

	const FGeneratedWorldLandscapeSettings& GetLandscapeSettings() const { return LandscapeSettings; };

protected:
	FGeneratedWorldLandscapeSettings LandscapeSettings;

};
