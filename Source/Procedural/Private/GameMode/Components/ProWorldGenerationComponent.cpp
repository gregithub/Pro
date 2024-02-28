// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Components/ProWorldGenerationComponent.h"

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

