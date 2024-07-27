// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ProCharacter.h"
#include "Components/StaticMeshComponent.h"

AProCharacter::AProCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AProCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
