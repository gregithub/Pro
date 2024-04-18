// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameMode/Components/Subcomponents/ProTerrainGenerationSubcomponent.h"
#include "ProLandscapeGeneration.generated.h"

class UProWorldGenerationComponent;
class UProceduralMeshComponent;

UCLASS()
class PROCEDURAL_API AProLandscapeGeneration : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProWorldGenerationComponent* WorldGenerationComponent = nullptr;

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


	const FGeneratedWorldTerrainSettings& GetWorldTerrainSettings() const { return GeneratedWorldTerrainSettings; };

	UProceduralMeshComponent* GetProceduralMeshComponent() { return ProceduralMeshComponent; };
	UProWorldGenerationComponent* GetProWorldGenerationComponent() { return WorldGenerationComponent; };

protected:
	FGeneratedWorldTerrainSettings GeneratedWorldTerrainSettings;

};
