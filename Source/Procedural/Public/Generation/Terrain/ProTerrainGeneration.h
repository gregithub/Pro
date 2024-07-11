// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Generation/Terrain/Components/ProLandscapeGenerationComponent.h"
#include "ProTerrainGeneration.generated.h"

class UProWorldGenerationComponent;

UCLASS()
class PROCEDURAL_API AProTerrainGeneration : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProLandscapeGenerationComponent* ProLandscapeGenerationComponent = nullptr;

protected:
	UFUNCTION(CallInEditor, Category = Pro)
	void CallInEditor_RegenerateTerrain();

public:
	AProTerrainGeneration();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void RequestTerrainGeneration();

	UProLandscapeGenerationComponent* GetProWorldGenerationComponent() { return ProLandscapeGenerationComponent; };
};
