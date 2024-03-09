// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/ProGameInstance.h"

void UProGameInstance::SetWorldTerrainSettings(const FGeneratedWorldTerrainSettings& InTerrainWorldSettings)
{
	GeneratedWorldTerrainSettings = InTerrainWorldSettings;
}