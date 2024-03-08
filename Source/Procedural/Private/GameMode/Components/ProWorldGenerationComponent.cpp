// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Components/ProWorldGenerationComponent.h"
#include "ProceduralMeshComponent.h"
#include "GameMode/ProGameModeBase.h"
#include "GameMode/Components/Subcomponents/ProTerrainGenerationSubcomponent.h"


UProWorldGenerationComponent::UProWorldGenerationComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	TerrainGenerationSubcomponent = NewObject<UProTerrainGenerationSubcomponent>();
}

void UProWorldGenerationComponent::Initialize(AProGameModeBase* InProGameModeBase)
{
	if (InProGameModeBase != nullptr)
	{
		OwnerGameModeBase = InProGameModeBase;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InProGameModeBase is invalid!"));
	}
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
	if (UProceduralMeshComponent* ProceduralMeshComponent = OwnerGameModeBase->GetProceduralMeshComponent())
	{
		FGeneratedWorldTerrainSettings GeneratedWorldSettings = TerrainGenerationSubcomponent->RequestTerrainGeneration();

		ProceduralMeshComponent->CreateMeshSection(
			0,
			GeneratedWorldSettings.Vertices,
			GeneratedWorldSettings.Triangles,
			GeneratedWorldSettings.Normals,
			GeneratedWorldSettings.UVO,
			TArray<FColor>(), 
			TArray<FProcMeshTangent>(), 
			true);
	}

	return;
}