// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProGameModeBase.generated.h"

class UProWorldGenerationComponent;

UCLASS()
class PROCEDURAL_API AProGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	AProGameModeBase();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;
};
