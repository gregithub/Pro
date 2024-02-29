// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Components/ProWorldGenerationComponent.h"
#include "GameMode/Components/Subcomponents/ProWorldGenerationSubomponentBase.h"

UProWorldGenerationComponent::UProWorldGenerationComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UProWorldGenerationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UProWorldGenerationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UProWorldGenerationComponent::RequestTerrainGeneration(const FGeneratedWorldTerrainSettings& InWorldSettings)
{
	if (InWorldSettings.IsValid() == false)
	{
		return;
	}

	TArray<TArray<int32>> GeneratedNoiseMap = TArray<TArray<int32>>();

	for (int32 CurrentX = 0; CurrentX <= InWorldSettings.GridSize.X; CurrentX++)
	{
		for (int32 CurrentY = 0; CurrentY <= InWorldSettings.GridSize.Y; CurrentY++)
		{
			GeneratedNoiseMap[CurrentX][CurrentY] = FMath::RandRange(0, 255);
		}
	}

	return;
}