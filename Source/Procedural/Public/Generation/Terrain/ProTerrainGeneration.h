// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Generation/Terrain/Components/ProLandscapeGenerationComponent.h"
#include "ProTerrainGeneration.generated.h"

class UProWorldGenerationComponent;
class UProceduralMeshComponent;

UCLASS()
class PROCEDURAL_API AProTerrainGeneration : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProLandscapeGenerationComponent* ProLandscapeGenerationComponent = nullptr;

protected:
	UFUNCTION(CallInEditor, Category = "Pro")
	void CallInEditor_RegenerateTerrain();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProceduralMeshComponent* ProceduralMeshComponent = nullptr;	

public:
	AProTerrainGeneration();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void RequestTerrainGeneration();

	const FGeneratedWorldTerrainSettings& GetWorldTerrainSettings() const { return GeneratedWorldTerrainSettings; };

	UProceduralMeshComponent* GetProceduralMeshComponent() { return ProceduralMeshComponent; };
	UProLandscapeGenerationComponent* GetProWorldGenerationComponent() { return ProLandscapeGenerationComponent; };

protected:
	FGeneratedWorldTerrainSettings GeneratedWorldTerrainSettings;

};
