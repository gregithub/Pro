// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/Components/ProWorldGenerationComponent.h"
#include "GameFramework/GameMode.h"
#include "ProGameModeBase.generated.h"

UCLASS()
class PROCEDURAL_API AProGameModeBase : public AGameMode
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProWorldGenerationComponent* WorldGenerationComponent = nullptr;

public:
	AProGameModeBase();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;

	bool RequestGenerateWorld();

public:
	UProWorldGenerationComponent* GetProWorldGenerationComponent() { return WorldGenerationComponent; };

};
