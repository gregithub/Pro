// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/Terrain/ProLandscapeChunk.h"
#include "Generation/Noise/ProNoise.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"

AProLandscapeChunk::AProLandscapeChunk()
{
	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	SetRootComponent(ProceduralMeshComponent);
	ProceduralMeshComponent->SetCastShadow(false);
	ProceduralMeshComponent->bUseAsyncCooking = true; //Async collision geometry

	PrimaryActorTick.bCanEverTick = false;
}

void AProLandscapeChunk::BeginPlay()
{
	Super::BeginPlay();
}

void AProLandscapeChunk::RequestCreateMeshSection(const FVector& InLocation, const FGeneratedWorldLandscapeSettings& InSettings)
{ 
	const int32 LocalSectionIndex = 0;

	const int32 NumOfVerticesPerAxis = 3;

	Vertices.Empty();
	Triangles.Empty();

	const float HeightNoiseMultiplier = 100.0f;

	for (int32 VerticesX = 0; VerticesX < NumOfVerticesPerAxis; VerticesX++)
	{
		for (int32 VerticesY = 0; VerticesY < NumOfVerticesPerAxis; VerticesY++)
		{
			FVector VerticeLocation = FVector(
				((float)VerticesX / (float)(NumOfVerticesPerAxis - 1) * InSettings.Global_ChunkSize),
				((float)VerticesY / (float)(NumOfVerticesPerAxis - 1) * InSettings.Global_ChunkSize),
				0.0f);

			VerticeLocation.Z = (ProNoise::SinglePerling((VerticeLocation + InLocation), InSettings.GetNoiseOffsets()) * HeightNoiseMultiplier);

			Vertices.Add(FVector(VerticeLocation));
		}
	}

	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;

	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(NumOfVerticesPerAxis, NumOfVerticesPerAxis, false, Triangles);
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVs, Normals, Tangents);
	
	//ProceduralMeshComponent->ClearMeshSection(LocalSectionIndex);

	ProceduralMeshComponent->CreateMeshSection(
		LocalSectionIndex,
		Vertices,
		Triangles,
		Normals,
		UVs,
		TArray<FColor>(),
		Tangents,
		false);

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