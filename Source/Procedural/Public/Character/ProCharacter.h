// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProCharacter.generated.h"

UCLASS()
class PROCEDURAL_API AProCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AProCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
