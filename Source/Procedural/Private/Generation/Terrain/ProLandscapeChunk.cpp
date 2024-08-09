// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/Terrain/ProLandscapeChunk.h"
#include "Generation/Noise/ProNoiseComponent.h"
#include "PhysicsEngine/BodySetup.h"
#include "KismetProceduralMeshLibrary.h"

AProLandscapeChunk::AProLandscapeChunk()
{
	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	SetRootComponent(ProceduralMeshComponent);
	ProceduralMeshComponent->bUseComplexAsSimpleCollision = false;
	ProceduralMeshComponent->SetCastShadow(false);
	ProceduralMeshComponent->bUseAsyncCooking = false; //Async collision geometry

	PrimaryActorTick.bCanEverTick = false;
}

void AProLandscapeChunk::BeginPlay()
{
	Super::BeginPlay();
}

void AProLandscapeChunk::RequestCreateMeshSection(const FGeneratedWorldLandscapeSettings& InSettings)
{ 
	if (InSettings.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("FGeneratedWorldLandscapeSettings is invalid!"));
		return;
	}

	const int32 LocalChunkVerticesPerAxis = InSettings.ChunkVerticesPerAxis;

	PrepareArrays(LocalChunkVerticesPerAxis);

	for (int32 VertexX = 0; VertexX < LocalChunkVerticesPerAxis; VertexX++)
	{
		for (int32 VertexY = 0; VertexY < LocalChunkVerticesPerAxis; VertexY++)
		{
			const float VertexPosX = ((float)VertexX / (float)(LocalChunkVerticesPerAxis - 1));
			const float VertexPosY = ((float)VertexY / (float)(LocalChunkVerticesPerAxis - 1));

			const float GlobalVertexPosX = (VertexPosX * InSettings.Global_ChunkSize);
			const float GlobalVertexPosY = (VertexPosY * InSettings.Global_ChunkSize);

			const float Height = CalculateHeight(FVector2D(GlobalVertexPosX, GlobalVertexPosY));

			Vertices.Add(FVector(GlobalVertexPosX, GlobalVertexPosY, Height));
			UVs.Add(FVector2D((VertexPosX), (VertexPosY)));
		}
	}

	/*FVector2D QuadCenter = FVector2D(((float)j + 0.5) / ((float)NumX), ((float)i + 0.5) / ((float)NumY));
	UV1s.Add(QuadCenter);
	UV1s.Add(QuadCenter);
	UV1s.Add(QuadCenter);
	UV1s.Add(QuadCenter);*/

	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(InSettings.ChunkVerticesPerAxis, InSettings.ChunkVerticesPerAxis, false, Triangles);
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVs, Normals, Tangents);
	
	const int32 LocalMeshSectionIndex = 0;

	ProceduralMeshComponent->ClearMeshSection(LocalMeshSectionIndex);

	ProceduralMeshComponent->AddCollisionConvexMesh(Vertices);

	ProceduralMeshComponent->CreateMeshSection(LocalMeshSectionIndex, Vertices, Triangles, Normals, UVs, TArray<FColor>(), Tangents, true);

	TryApplyMaterial();
}

float AProLandscapeChunk::CalculateHeight(const FVector2D& InVertexLocation2D)
{
	float HeightValue = 0.0f;

	if (UProNoiseComponent* ProNoiseComponent = UProNoiseComponent::GetInstance(this))
	{
		
		const FVector GlobalVertexLocation = (FVector(InVertexLocation2D.X, InVertexLocation2D.Y, 0.0f) + FVector(GetActorLocation()));

		const FVector2D GlobalVertexLocation2D = FVector2D(GlobalVertexLocation.X, GlobalVertexLocation.Y);

		{
			const ENoiseTerrainType ContinentalnessTerrainTypeEnum = ENoiseTerrainType::Continentalness;

			if (ProNoiseComponent->GetNoiseSettingsType(ContinentalnessTerrainTypeEnum).bApplyNoise)
			{
				const float ContinentalnessNoiseValue = ProNoiseComponent->GetNoiseTerrainTypeValue(GlobalVertexLocation2D, ContinentalnessTerrainTypeEnum);

				const float ContinentalnessHeightValue = ProNoiseComponent->GetWorldHeightValueFromNoise(ContinentalnessNoiseValue, ContinentalnessTerrainTypeEnum);

				HeightValue += ContinentalnessHeightValue;

				NoiseContinentalnessValues.Add(ContinentalnessNoiseValue);
			}
		}

		{
			const ENoiseTerrainType ErrosionTerrainTypeEnum = ENoiseTerrainType::Erosion;

			if (ProNoiseComponent->GetNoiseSettingsType(ErrosionTerrainTypeEnum).bApplyNoise)
			{
				const float ErosionNoiseValue = ProNoiseComponent->GetNoiseTerrainTypeValue(GlobalVertexLocation2D, ErrosionTerrainTypeEnum);

				const float ErosionHeightValue = ProNoiseComponent->GetWorldHeightValueFromNoise(ErosionNoiseValue, ErrosionTerrainTypeEnum);

				HeightValue += ErosionHeightValue;

				NoiseErosionValues.Add(ErosionNoiseValue);
			}
		}

		{
			const ENoiseTerrainType PeaksAndValleysTerrainTypeEnum = ENoiseTerrainType::PeaksAndValleys;

			if (ProNoiseComponent->GetNoiseSettingsType(PeaksAndValleysTerrainTypeEnum).bApplyNoise)
			{
				const float PeaksAndValleysNoiseValue = ProNoiseComponent->GetNoiseTerrainTypeValue(GlobalVertexLocation2D, PeaksAndValleysTerrainTypeEnum);

				const float PeaksAndValleysHeightValue = ProNoiseComponent->GetWorldHeightValueFromNoise(PeaksAndValleysNoiseValue, PeaksAndValleysTerrainTypeEnum);

				HeightValue += PeaksAndValleysHeightValue;

				NoisePeaksAndValleysValues.Add(PeaksAndValleysNoiseValue);
			}
		}
	}

	return HeightValue;
}

void AProLandscapeChunk::TryApplyMaterial()
{
	if (MaterialInterface != nullptr)
	{
		ProceduralMeshComponent->SetMaterial(0, MaterialInterface);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MaterialInterface is invalid!"));
		return;
	}
}

void AProLandscapeChunk::PrepareArrays(const int32 InVerticesNum)
{
	//Reserve arrays space so we don't waste performance expanding them in runtime.
	{
		Vertices.Empty();
		Triangles.Empty();
		UVs.Empty();
		Normals.Empty();
		Tangents.Empty();

		const int32 SquaredChunkVerticesPerAxis = (InVerticesNum * InVerticesNum);

		Vertices.Reserve(SquaredChunkVerticesPerAxis);
		UVs.Reserve(SquaredChunkVerticesPerAxis);
		Normals.Reserve(SquaredChunkVerticesPerAxis);
		Tangents.Reserve(SquaredChunkVerticesPerAxis);
		Triangles.Reserve(((InVerticesNum - 1) * (InVerticesNum - 1) * 6));
	}
}

void AProLandscapeChunk::CallInEditor_PrintInfo()
{
	if (NoiseContinentalnessValues.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoisePeaksAndValleysValues: "));

		for (int32 i = 0; i < NoiseContinentalnessValues.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s:%s!"), *FString::FromInt(i), *FString::SanitizeFloat(NoiseContinentalnessValues[i]));
		}
	}

	if (NoiseErosionValues.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoiseErosionValues: "));

		for (int32 i = 0; i < NoiseErosionValues.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s:%s!"), *FString::FromInt(i), *FString::SanitizeFloat(NoiseErosionValues[i]));
		}
	}

	if (NoisePeaksAndValleysValues.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoisePeaksAndValleysValues: "));

		for (int32 i = 0; i < NoisePeaksAndValleysValues.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s:%s!"), *FString::FromInt(i), *FString::SanitizeFloat(NoisePeaksAndValleysValues[i]));
		}
	}

	if (Vertices.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Vertices height: "));

		for (int32 i = 0; i < Vertices.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s:%s!"), *FString::FromInt(i), *FString::SanitizeFloat(Vertices[i].Z));
		}
	}
}

float AProLandscapeChunk::GetMinNoiseTypeValue(const ENoiseTerrainType InNoiseTerrainType) const
{
	float MinValue = 2.0f;

	TArray<float> ValuesToCheck;

	switch (InNoiseTerrainType)
	{
	case ENoiseTerrainType::Continentalness:
	{
		ValuesToCheck = NoiseContinentalnessValues;
	}
	break;
	case ENoiseTerrainType::Erosion:
	{
		ValuesToCheck = NoiseErosionValues;
	}
	break;
	case ENoiseTerrainType::PeaksAndValleys:
	{
		ValuesToCheck = NoisePeaksAndValleysValues;
	}
	break;
	default:
		break;
	}

	for (float ValueToCheck : ValuesToCheck)
	{
		if (ValueToCheck < MinValue)
		{
			MinValue = ValueToCheck;
		}
	}

	return MinValue;
}

float AProLandscapeChunk::GetMaxNoiseTypeValue(const ENoiseTerrainType InNoiseTerrainType) const
{
	float MaxValue = -2.0f;

	TArray<float> ValuesToCheck;

	switch (InNoiseTerrainType)
	{
	case ENoiseTerrainType::Continentalness:
	{
		ValuesToCheck = NoiseContinentalnessValues;
	}
	break;
	case ENoiseTerrainType::Erosion:
	{
		ValuesToCheck = NoiseErosionValues;
	}
	break;
	case ENoiseTerrainType::PeaksAndValleys:
	{
		ValuesToCheck = NoisePeaksAndValleysValues;
	}
	break;
	default:
		break;
	}

	for (float ValueToCheck : ValuesToCheck)
	{
		if (ValueToCheck > MaxValue)
		{
			MaxValue = ValueToCheck;
		}
	}

	return MaxValue;
}