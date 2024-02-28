// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProMainMenu.generated.h"

class UBorder;
class UButton;
class UProMainMenuButton;

UCLASS()
class PROCEDURAL_API UProMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Pro|UI", meta = (BindWidget))
	UBorder* MainBorder = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Pro|UI", meta = (BindWidget))
	UBorder* ButtonsBorder = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Pro|UI", meta = (BindWidget))
	UProMainMenuButton* PlayButton = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Pro|UI", meta = (BindWidget))
	UProMainMenuButton* GenerateButton = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Pro|Config")
	TSoftObjectPtr<UWorld> LevelToLoad_OnPlayButton = nullptr;

protected:
	UProMainMenu(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

protected:
	// DynamicBinds
	UFUNCTION()
	void OnPlayButtonClicked();

};
