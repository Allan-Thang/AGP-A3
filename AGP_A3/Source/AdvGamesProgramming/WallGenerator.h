// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WallGenerator.generated.h"

UENUM()
enum Side
{
	Top,
	Bottom,
	Left,
	Right
};

UCLASS()
class ADVGAMESPROGRAMMING_API AWallGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* MeshComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateWalls(TArray<FVector> Tiles);
	void CheckAndPlaceTile(FVector CheckTile, float Check_X, float Check_Y, TArray<FVector>& Tiles, Side Side);
};