// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "WaterFollowingComponent.generated.h"

UCLASS()
class PROCEDURAL_API AWaterFollowingActor : public AStaticMeshActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro)
	float WaterLevelHeight = 100.0f;

public:
	virtual void BeginPlay() override;

protected:
	UFUNCTION(/*DynamicBind*/)
	void OnLandscapeCoordinatesChanged(class UProLandscapeGenerationComponent* InProLandscapeGenerationComponent);

protected:
	void UpdatePosition(FVector InPosition);
	void UpdateScale(float InScale);

};

