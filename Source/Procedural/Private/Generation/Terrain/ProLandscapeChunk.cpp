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
	ProceduralMeshComponent->bUseAsyncCooking = true; //Async collision geometry

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

	PrepareArrays(InSettings.ChunkVerticesPerAxis);

	for (int32 VertexX = 0; VertexX < InSettings.ChunkVerticesPerAxis; VertexX++)
	{
		for (int32 VertexY = 0; VertexY < InSettings.ChunkVerticesPerAxis; VertexY++)
		{
			const float VertexPosX = ((float)VertexX / (float)(InSettings.ChunkVerticesPerAxis - 1) * InSettings.Global_ChunkSize);
			const float VertexPosY = ((float)VertexY / (float)(InSettings.ChunkVerticesPerAxis - 1) * InSettings.Global_ChunkSize);
			const float Height = CalculateHeight(FVector2D(VertexPosX, VertexPosY));

			Vertices.Add(FVector(VertexPosX, VertexPosY, Height));
			UVs.Add(FVector2D((VertexPosX * InSettings.Global_UVScale), (VertexPosY * InSettings.Global_UVScale)));
		}
	}

	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(InSettings.ChunkVerticesPerAxis, InSettings.ChunkVerticesPerAxis, false, Triangles);
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVs, Normals, Tangents);
	
	const int32 LocalMeshSectionIndex = 0;

	ProceduralMeshComponent->ClearMeshSection(LocalMeshSectionIndex);

	ProceduralMeshComponent->CreateMeshSection(LocalMeshSectionIndex, Vertices, Triangles, Normals, UVs, TArray<FColor>(), Tangents, false);

	ProceduralMeshComponent->AddCollisionConvexMesh(Vertices);
	
	ProceduralMeshComponent->GetBodySetup()->CreatePhysicsMeshes();

	TryApplyMaterial();
}

float AProLandscapeChunk::CalculateHeight(const FVector2D& InVertexLocation2D)
{
	float HeightValue = 0.0f;

	if (UProNoiseComponent* ProNoiseComponent = UProNoiseComponent::GetInstance(this))
	{
		if (const UNoiseCurveSettings* NoiseCurveSettings = ProNoiseComponent->GetNoiseCurveSettings())
		{
			const FVector GlobalVertexLocation = (FVector(InVertexLocation2D.X, InVertexLocation2D.Y, 0.0f) + FVector(GetActorLocation()));

			float Noise_Continentalnes = ProNoiseComponent->SinglePerling_Continentalness(GlobalVertexLocation);
			float Noise_Errosion = ProNoiseComponent->SinglePerling_Erosion(GlobalVertexLocation);
			float Noise_PeaksAndValleys = ProNoiseComponent->SinglePerling_PeaksANdValleys(GlobalVertexLocation);

			NoiseContinentalnessValues.Add(Noise_Continentalnes);
			NoiseErosionValues.Add(Noise_Errosion);
			NoisePeaksAndValleysValues.Add(Noise_PeaksAndValleys);

			HeightValue = (
				(NoiseCurveSettings->GetCurve_Continentalness()->GetFloatValue(Noise_Continentalnes)) +
				(NoiseCurveSettings->GetCurve_Erosion()->GetFloatValue(Noise_Errosion))); /* +
				(NoiseCurveSettings->GetCurve_PeaksAndValleys()->GetFloatValue(Noise_PeaksAndValleys)));*/
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
	if(NoisePeaksAndValleysValues.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoisePeaksAndValleysValues: "));

		for (int32 i = 0; i< NoisePeaksAndValleysValues.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s:%s!"), *FString::FromInt(i),*FString::SanitizeFloat(NoisePeaksAndValleysValues[i]));
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

}