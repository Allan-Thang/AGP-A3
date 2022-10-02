// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "WallGenerator.h"
#include "GameFramework/Actor.h"
#include "DungeonGenerator.generated.h"

UCLASS()
class ADVGAMESPROGRAMMING_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

	struct FRoom
	{
		FVector MinPos;
		FVector MaxPos;
		FVector MidPoint;

		float Width;
		float Height;

		FRoom* Parent;
		FRoom* Child1;
		FRoom* Child2;
	};

	struct FRoomPair
	{
		FRoom* CameFromRoom;
		FRoom* Room;
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	float RoomCount;
	UPROPERTY(EditAnywhere)
	float MapScale;

	TArray<FVector> FloorTiles;
	TArray<FVector> CorridorTiles;
	TArray<FRoom*> RoomsArray;
	TArray<FVector> RoomCentres;

	TArray<FRoomPair> ConnectedRooms;

	FRoom* Map;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	AWallGenerator* WallGenerator;

	UPROPERTY(EditAnywhere)
	bool SpawnPlayer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerCharacter> Player;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateMap();
	void MakeRoom();
	void SplitIntoRooms(unsigned TimesToSplit);
	void BSP_SplitRoom_Vert(FRoom* RoomToSplit);
	void BSP_SplitRoom_Hor(FRoom* RoomToSplit);
	void TrimRooms();
	void GenerateMST(TArray<FRoom*> Rooms);
	void PopulateConnectedRooms(TArray<FRoom*> Rooms);
	void AddCorridors(FRoom* RoomA, FRoom* RoomB);
	TArray<FVector> SplitRoomIntoTiles(FRoom* RoomToSplit);
	void SpawnTiles(const TArray<FVector>& Tiles);
	void TempSpawnTiles();
	float RoundToHundred(float Value);
	void DebugFunction();

private:
	float MapSize;
};