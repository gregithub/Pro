// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Generation/Terrain/Components/ProLandscapeGenerationComponent.h" // Todo: remove this after moving landscape settings to separate file
#include "ProceduralMeshComponent.h"
#include "ProLandscapeChunk.generated.h"

class UProceduralMeshComponent;

UCLASS()
class PROCEDURAL_API AProLandscapeChunk : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProceduralMeshComponent* ProceduralMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMaterialInterface* MaterialInterface = nullptr;

protected:
	UFUNCTION(CallInEditor, Category = Pro)
	void CallInEditor_PrintInfo();

public:	
	AProLandscapeChunk();
	virtual void BeginPlay() override;

	void RequestCreateMeshSection(const FGeneratedWorldLandscapeSettings& InSettings);

	void TryApplyMaterial();

	UProceduralMeshComponent* GetProceduralMeshComponent() { return ProceduralMeshComponent; };

	float GetMinNoiseTypeValue(const ENoiseTerrainType InNoiseTerrainType) const;
	float GetMaxNoiseTypeValue(const ENoiseTerrainType InNoiseTerrainType) const;

protected:
	void PrepareArrays(const int32 InVerticesNum);

	float CalculateHeight(const FVector2D& InVertexLocation);

protected:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UVs;
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;

	TArray<float> NoiseContinentalnessValues;
	TArray<float> NoiseErosionValues;
	TArray<float> NoisePeaksAndValleysValues;

};
