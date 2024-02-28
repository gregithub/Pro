// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/ProMainMenu.h"
#include "UI/MainMenu/Components/ProMainMenuButton.h"
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
		PlayButton->OnClicked.AddDynamic(this, &UProMainMenu::OnPlayButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayButton widget is invalid!"));
	}
}

void UProMainMenu::OnPlayButtonClicked()
{
	if (LevelToLoad_OnPlayButton.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("OnPlayButtonClicked level to load is invalid!"));
		return;
	}

	const FName LevelToLoadName = FName(LevelToLoad_OnPlayButton.GetAssetName());

	UGameplayStatics::OpenLevel(GetWorld(), LevelToLoadName);
}