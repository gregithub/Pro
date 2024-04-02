// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/ProMainMenu.h"
#include "Components/Button.h"
#include "GameMode/ProGameModeBase.h"
#include "Kismet/GameplayStatics.h"

UProMainMenu::UProMainMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{	
}

void UProMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

}

void UProMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	SetUserFocus(UGameplayStatics::GetPlayerController(this, 0));

	if (PlayButton != nullptr)
	{
		PlayButton->OnReleased.AddDynamic(this, &UProMainMenu::OnPlayButtonReleased);
		GenerateButton->OnReleased.AddDynamic(this, &UProMainMenu::OnGenerateButtonReleased);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayButton widget is invalid!"));
	}
}

void UProMainMenu::OnPlayButtonReleased()
{
	if (LevelToLoad_OnPlayButton.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("OnPlayButtonClicked level to load is invalid!"));
		return;
	}

	const FName LevelToLoadName = FName(LevelToLoad_OnPlayButton.GetAssetName());

	UGameplayStatics::OpenLevel(GetWorld(), LevelToLoadName);
}

void UProMainMenu::OnGenerateButtonReleased()
{
	if (AProGameModeBase* ProGameModeBase = Cast<AProGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		ProGameModeBase->RequestGenerateWorld();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ProGameModeBase fetch failed!"));
	}
}