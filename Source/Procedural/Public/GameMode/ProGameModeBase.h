// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProGameModeBase.generated.h"

class UProWorldGenerationComponent;
class UProNoiseComponent;
class UProLandscapeGenerationComponent;

UCLASS()
class PROCEDURAL_API AProGameModeBase : public AGameMode
{
	GENERATED_BODY()

	protected:
	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly)
	UProNoiseComponent* ProNoiseComponent = nullptr;

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly)
	UProLandscapeGenerationComponent* ProLandscapeGenerationComponent = nullptr;

public:
	AProGameModeBase();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;

	static AProGameModeBase* GetInstance(const UObject* const WorldContextObject);

protected:
	UFUNCTION(Category = "Pro|Noise", CallInEditor)
	void RequestRecalculateTerrain();

public:
	UProNoiseComponent& GetProNoiseComponent() const { return *ProNoiseComponent; };

	UProLandscapeGenerationComponent* GetProLandscapeGenerationComponent() { return ProLandscapeGenerationComponent; };
};
