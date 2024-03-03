// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Components/ProWorldGenerationComponent.h"
#include "GameMode/Components/Subcomponents/ProTerrainGenerationSubcomponent.h"


UProWorldGenerationComponent::UProWorldGenerationComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	TerrainGenerationSubcomponent = NewObject<UProTerrainGenerationSubcomponent>();
}

void UProWorldGenerationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UProWorldGenerationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UProWorldGenerationComponent::RequestTerrainGeneration()
{
	TerrainGenerationSubcomponent->RequestTerrainGeneration();
}