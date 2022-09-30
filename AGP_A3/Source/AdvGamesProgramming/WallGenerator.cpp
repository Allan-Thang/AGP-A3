// Fill out your copyright notice in the Description page of Project Settings.


#include "WallGenerator.h"

// Sets default values
AWallGenerator::AWallGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComponent;
	MeshComponent->SetMobility(EComponentMobility::Static);
	MeshComponent->SetCollisionProfileName("BlockAll");
}

// Called when the game starts or when spawned
void AWallGenerator::BeginPlay()
{
	Super::BeginPlay();

	// MeshComponent->AddInstance(FTransform(FVector(500.0f, 0.0f, 0.0f)));

	// Tiles.Add(FVector(100.0f, 100.0f,0.0f));
	// Tiles.Add(FVector(200.0f, 100.0f, 0.0f));
	// Tiles.Add(FVector(100.0f, 200.0f, 0.0f));
	// Tiles.Add(FVector(200.0f, 200.0f, 0.0f));
	//
	// GenerateWalls(Tiles);
}

// Called every frame
void AWallGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallGenerator::GenerateWalls(TArray<FVector> Tiles)
{
	TArray<FVector> TilesCopy = Tiles;

	for (FVector Tile : TilesCopy)
	{
		CheckAndPlaceTile(Tile, 0.0f, 100.0f, Tiles, Side::Top);
		CheckAndPlaceTile(Tile, 0.0f, -100.0f, Tiles, Side::Bottom);
		CheckAndPlaceTile(Tile, 100.0f, 0.0f, Tiles, Side::Left);
		CheckAndPlaceTile(Tile, -100.0f, 0.0f, Tiles, Side::Right);
	}
}

void AWallGenerator::CheckAndPlaceTile(FVector CheckTile, float Check_X, float Check_Y, TArray<FVector>& Tiles, Side Side)
{
	if (!Tiles.Contains(CheckTile + FVector(Check_X, Check_Y, 0.0f)))
	{
		// Tiles.Add(CheckTile + FVector(Check_X, Check_Y, 0.0f));

		FRotator WallRotation;
		FVector WallLocation;
		if (Side == Side::Top)
		{
			WallRotation = FRotator(0.0f, 90.0f, 0.0f);
			WallLocation = FVector(CheckTile + FVector(50.0f, 90.0f, 0.0f));
		}
		else if (Side == Side:: Bottom)
		{
			WallRotation = FRotator(0.0f, 90.0f, 0.0f);
			WallLocation = FVector(CheckTile + FVector(50.0f, -10.0f, 0.0f));
		}
		else if (Side == Side:: Left)
		{
			WallRotation = FRotator::ZeroRotator;
			WallLocation = FVector(CheckTile + FVector(90.0f, 50.0f, 0.0f));
		}
		else
		{
			WallRotation = FRotator::ZeroRotator;
			WallLocation = FVector(CheckTile + FVector(10.0f, 50.0f, 0.0f));
		}
		MeshComponent->AddInstance(FTransform(WallRotation, WallLocation));
	}
}