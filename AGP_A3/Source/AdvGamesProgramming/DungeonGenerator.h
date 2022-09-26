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

	struct FRoom
	{
		FVector MinPos;
		FVector MaxPos;

		float Width;
		float Height;

		FRoom* Parent;
		FRoom* Child1;
		FRoom* Child2;
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	int RoomCount;
	UPROPERTY(EditAnywhere)
	int RoomSize_Max;
	UPROPERTY(EditAnywhere)
	int RoomSize_Min;

	TArray<FVector> FloorTiles;
	TArray<FRoom*> RoomsArray;

	FRoom* Map;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* MeshComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateMap();
	void MakeRoom();
	void SpawnTiles();
	void SplitIntoRooms(unsigned TimesToSplit);
	void BSP_SplitRoom_Vert(FRoom* RoomToSplit);
	void BSP_SplitRoom_Hor(FRoom* RoomToSplit);
	void DebugFunction();

private:
	int MapSize;
};