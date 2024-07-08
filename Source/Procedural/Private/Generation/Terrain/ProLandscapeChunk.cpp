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

	Vertices.Empty();
	Triangles.Empty();

	const int32 RandomSeed = 0;

	const float NoiseValue = ProNoise::SinglePerling(InLocation, RandomSeed);

	const float Height = (NoiseValue * InSettings.Global_WorldScale);

	Vertices.Add(FVector(0.0f, 0.0f, 0.0f));
	Vertices.Add(FVector(0.0f, 1.0f * InSettings.Global_ChunkSize, Height));
	Vertices.Add(FVector(1.0f * InSettings.Global_ChunkSize, 0.0f, Height));
	Vertices.Add(FVector(1.0f * InSettings.Global_ChunkSize, 1.0f * InSettings.Global_ChunkSize, Height));

	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);

	Triangles.Add(3);
	Triangles.Add(2);
	Triangles.Add(1);

	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;

	//UKismetProceduralMeshLibrary::CreateGridMeshTriangles((Global_ChunksNum2D.X + 1), (Global_ChunksNum2D.Y + 1), true, LandscapeSettings.Triangles);
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