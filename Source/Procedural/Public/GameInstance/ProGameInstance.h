// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameMode/Components/Subcomponents/ProTerrainGenerationSubcomponent.h"
#include "ProGameInstance.generated.h"

UCLASS()
class PROCEDURAL_API UProGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void SetWorldTerrainSettings(const FGeneratedWorldTerrainSettings& InTerrainWorldSettings);

	const FGeneratedWorldTerrainSettings& GetWorldTerrainSettings() const { return GeneratedWorldTerrainSettings; };

protected:
	FGeneratedWorldTerrainSettings GeneratedWorldTerrainSettings;

};
