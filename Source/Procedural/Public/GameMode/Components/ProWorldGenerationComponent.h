// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProWorldGenerationComponent.generated.h"

class AProGameModeBase;
class UProTerrainGenerationSubcomponent;

UCLASS(BlueprintType, ClassGroup = (Pro), meta = (BlueprintSpawnableComponent))
class PROCEDURAL_API UProWorldGenerationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UProWorldGenerationComponent(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Initialize(AProGameModeBase* InProGameModeBase);
public:
	void RequestTerrainGeneration();

	UProTerrainGenerationSubcomponent* GetTerrainGenerationSubcomponent() const { ensure(TerrainGenerationSubcomponent); return TerrainGenerationSubcomponent; };

protected:
	UProTerrainGenerationSubcomponent* TerrainGenerationSubcomponent = nullptr;
	

protected:
	AProGameModeBase* OwnerGameModeBase = nullptr;
};
