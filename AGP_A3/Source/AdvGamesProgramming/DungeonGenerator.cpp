// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RoomCount = 10;
	RoomSize_Min = 10;
	RoomSize_Max = 100;

	MapSize = 100 * RoomSize_Max * RoomCount;

}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();

	Map.MinPos = FVector(0.0f, 0.0f,0.0f);
	Map.MaxPos = Map.MinPos + FVector(MapSize, MapSize, 0);
	Map.Width = Map.MaxPos.X / 100;
	Map.Height = Map.MaxPos.Y / 100;
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADungeonGenerator::GenerateMap()
{
}

void ADungeonGenerator::MakeRoom()
{
}

void ADungeonGenerator::SpawnTiles()
{
}

void ADungeonGenerator::BSP_SplitRoom_Vert(Room RoomToSplit)
{
	int SplitAtPercent = FMath::RandRange(1, 10);
}

void ADungeonGenerator::BSP_SplitRoom_Hor(Room RoomToSplit)
{
	int SplitAtPercent = FMath::RandRange(1, 10);
}