// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonGenerator.generated.h"

UCLASS()
class ADVGAMESPROGRAMMING_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

	struct Room
	{
		FVector MinPos;
		FVector MaxPos;

		float Width;
		float Height;

		Room* Parent;
		Room* Child1;
		Room* Child2;
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	int RoomCount;
	int RoomSize_Max;
	int RoomSize_Min;

	TArray<FVector> FloorTiles;

	Room Map;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateMap();
	void MakeRoom();
	void SpawnTiles();
	void BSP_SplitRoom_Vert(Room RoomToSplit);
	void BSP_SplitRoom_Hor(Room RoomToSplit);

private:
	int MapSize;

};