// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProWorldGenerationComponent.generated.h"

USTRUCT(BlueprintType)
struct FGeneratedWorldSettings
{
	GENERATED_BODY()


};


UCLASS(BlueprintType, ClassGroup = (Pro), meta = (BlueprintSpawnableComponent))
class PROCEDURAL_API UProWorldGenerationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UProWorldGenerationComponent(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
