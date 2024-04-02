// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProLandscapeGeneration.generated.h"

class UProceduralMeshComponent;

UCLASS()
class PROCEDURAL_API AProLandscapeGeneration : public AActor
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(CallInEditor, Category = "Pro")
	void CallInEditor_RegenerateTerrain();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProceduralMeshComponent* ProceduralMeshComponent = nullptr;	

public:
	AProLandscapeGeneration();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void RequestTerrainGeneration();

	UProceduralMeshComponent* GetProceduralMeshComponent() { return ProceduralMeshComponent; };
};
