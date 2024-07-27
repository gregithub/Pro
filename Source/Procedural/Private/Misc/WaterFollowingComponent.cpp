// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/WaterFollowingComponent.h"
#include "GameMode/ProGameModeBase.h"
#include "Generation/Terrain/Components/ProLandscapeGenerationComponent.h"

void AWaterFollowingActor::BeginPlay()
{
	Super::BeginPlay();

	if (AProGameModeBase* ProGameModeBase = AProGameModeBase::GetInstance(this))
	{
		if (UProLandscapeGenerationComponent* ProLandscapeGenerationComponent = ProGameModeBase->GetProLandscapeGenerationComponent())
		{
			OnLandscapeCoordinatesChanged(ProLandscapeGenerationComponent);

			ProLandscapeGenerationComponent->OnCoordinatesChanged.AddUObject(this, &AWaterFollowingActor::OnLandscapeCoordinatesChanged);
		}
	}
}

void AWaterFollowingActor::UpdatePosition(FVector InPosition)
{
	FVector NewLocation = FVector(InPosition.X, InPosition.Y, WaterLevelHeight);

 	SetActorLocation(NewLocation);
}

void AWaterFollowingActor::UpdateScale(float InScale)
{
	SetActorScale3D(FVector(InScale, InScale, 1.0f));
}

void AWaterFollowingActor::OnLandscapeCoordinatesChanged(UProLandscapeGenerationComponent* InProLandscapeGenerationComponent)
{
	if (InProLandscapeGenerationComponent != nullptr)
	{
		UpdatePosition(InProLandscapeGenerationComponent->GetCurrentCenterPosition());
		UpdateScale((float)InProLandscapeGenerationComponent->GetLandscapeSettings().Global_MapSize);
	}
}