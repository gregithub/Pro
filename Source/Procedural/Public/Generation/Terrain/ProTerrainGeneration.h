// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Generation/Terrain/Components/ProLandscapeGenerationComponent.h"
#include "ProTerrainGeneration.generated.h"

class UProWorldGenerationComponent;
class UProceduralMeshComponent;

//TODO: this should be managing other mesh chunks actors and async creating/destroying them in runtime based on camera location

UCLASS()
class PROCEDURAL_API AProTerrainGeneration : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProceduralMeshComponent* ProceduralMeshComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProLandscapeGenerationComponent* ProLandscapeGenerationComponent = nullptr;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMaterialInterface* MaterialInterface = nullptr;

protected:
	UFUNCTION(CallInEditor, Category = Pro)
	void CallInEditor_RegenerateTerrain();

public:
	AProTerrainGeneration();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void RequestTerrainGeneration();


	UProceduralMeshComponent* GetProceduralMeshComponent() { return ProceduralMeshComponent; };
	UProLandscapeGenerationComponent* GetProWorldGenerationComponent() { return ProLandscapeGenerationComponent; };

};
