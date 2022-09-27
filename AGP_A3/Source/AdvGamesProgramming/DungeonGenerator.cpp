// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"

#include "DrawDebugHelpers.h"

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent =	CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComponent;
	MeshComponent->SetMobility(EComponentMobility::Static);
	MeshComponent->SetCollisionProfileName("BlockAll");

	MeshComponent->AddInstance(FTransform(FVector::ZeroVector));

	RoomCount = 10.0f;
	RoomSize_Min = 5.0f;
	RoomSize_Max = 10.0f;

	MapSize = 100.0f * RoomSize_Max * RoomCount;
	Map = new FRoom;
}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();

	if (Map)
	{
		Map->MinPos = FVector(0.0f, 0.0f,0.0f);
		Map->MaxPos = Map->MinPos + FVector(MapSize, MapSize, 0);
		Map->Width = MapSize / 100.0f;
		Map->Height = MapSize / 100.0f;
	}
	UE_LOG(LogTemp, Warning, TEXT("MapWidth: %f, MapHeight: %f"), Map->Width, Map->Height);
	// UE_LOG(LogTemp, Warning, TEXT("MapMaxPos: %s"), *(Map->MaxPos).ToString());
	UE_LOG(LogTemp, Warning, TEXT("MapMaxPos: %f"), Map->MaxPos.X);

	RoomsArray.Add(Map);
	SplitIntoRooms(RoomCount-1);
	TrimRooms();
	SplitIntoTiles();
	SpawnTiles();
	// TempSpawnTiles();
	// DebugFunction();
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

void ADungeonGenerator::SplitIntoRooms(unsigned TimesToSplit)
{
	if (TimesToSplit <= 0)
		return;
	// Choose which room to split by finding the Room with the largest area
	FRoom* RoomToSplit = RoomsArray[0];
	if (RoomToSplit)
	{
		for (FRoom* iRoom : RoomsArray)
		{
			if ((iRoom->Width * iRoom->Height) > (RoomToSplit->Width * RoomToSplit->Height))
			{
				RoomToSplit = iRoom;
			}
		}
		// Split vert if it wider than it is tall and vice versa.
		if (RoomToSplit->Width >= RoomToSplit->Height)
		{
			BSP_SplitRoom_Vert(RoomToSplit);
			UE_LOG(LogTemp, Warning, TEXT("Split Vertically"));
		}
		else
		{
			BSP_SplitRoom_Hor(RoomToSplit);
			// BSP_SplitRoom_Hor(RoomToSplit);
			UE_LOG(LogTemp, Warning, TEXT("Split Horizontally"));
		}

		TimesToSplit--;
		SplitIntoRooms(TimesToSplit);
	}
}

void ADungeonGenerator::BSP_SplitRoom_Vert(FRoom* RoomToSplit)
{
	if (RoomToSplit)
	{
		const float SplitPercent = FMath::RandRange(1, 9);
		const float SplitAtTile = floorf(RoomToSplit->Width * (SplitPercent/10));
		UE_LOG(LogTemp, Warning, TEXT("SplitPercent: %f"), SplitPercent);
		// const int SplitAtTile = FMath::RandRange(2.0f, RoomToSplit->Width-2);
		UE_LOG(LogTemp, Warning, TEXT("SplitAtTile: %f"), SplitAtTile);
		RoomToSplit->Child1 = new FRoom;
		RoomToSplit->Child1->Parent = RoomToSplit;
		RoomToSplit->Child1->MinPos = RoomToSplit->MinPos;
		RoomToSplit->Child1->MaxPos = RoomToSplit->MinPos + FVector(SplitAtTile * 100.0f, RoomToSplit->Height*100.0f, 0.0f);
		RoomToSplit->Child1->Width = SplitAtTile;
		RoomToSplit->Child1->Height = RoomToSplit->Height;

		RoomToSplit->Child2 = new FRoom;
		RoomToSplit->Child2->Parent = RoomToSplit;
		RoomToSplit->Child2->MinPos = RoomToSplit->MinPos + FVector(SplitAtTile * 100.0f, 0.0f, 0.0f);
		RoomToSplit->Child2->MaxPos = RoomToSplit->MaxPos;
		RoomToSplit->Child2->Width = RoomToSplit->Width - SplitAtTile;
		RoomToSplit->Child2->Height = RoomToSplit->Height;

		// UE_LOG(LogTemp, Warning, TEXT("SplitAtTile: %i, RoomToSplitWidth: %f, RoomToSplitHeight: %f,"), SplitAtTile, RoomToSplit->Width, RoomToSplit->Height);
		if (RoomsArray.Contains(RoomToSplit))
		{
			RoomsArray.Add(RoomToSplit->Child1);
			RoomsArray.Add(RoomToSplit->Child2);
			RoomsArray.Remove(RoomToSplit);
		}
	}
	else UE_LOG(LogTemp, Warning, TEXT("BSP_SplitRoom_Vert Failed!"));
}

void ADungeonGenerator::BSP_SplitRoom_Hor(FRoom* RoomToSplit)
{
	if (RoomToSplit)
	{
		const float SplitPercent = FMath::RandRange(1, 9);
		const float SplitAtTile = floorf(RoomToSplit->Height * (SplitPercent/10));



		RoomToSplit->Child1 = new FRoom;
		RoomToSplit->Child1->Parent = RoomToSplit;
		RoomToSplit->Child1->MinPos = RoomToSplit->MinPos;
		RoomToSplit->Child1->MaxPos = RoomToSplit->MinPos + FVector(RoomToSplit->Width*100.0f, SplitAtTile * 100.0f, 0.0f);
		RoomToSplit->Child1->Width = RoomToSplit->Width;
		RoomToSplit->Child1->Height = SplitAtTile;

		RoomToSplit->Child2 = new FRoom;
		RoomToSplit->Child2->Parent = RoomToSplit;
		RoomToSplit->Child2->MinPos = RoomToSplit->MinPos + FVector(0.0f, SplitAtTile * 100.0f, 0.0f);
		RoomToSplit->Child2->MaxPos = RoomToSplit->MaxPos;
		RoomToSplit->Child2->Width = RoomToSplit->Width;
		RoomToSplit->Child2->Height = RoomToSplit->Height - SplitAtTile;
		// UE_LOG(LogTemp, Warning, TEXT("SplitAtTile: %i, RoomToSplitWidth: %f, RoomToSplitHeight: %f,"), SplitAtTile, RoomToSplit->Width, RoomToSplit->Height);

		if (RoomsArray.Contains(RoomToSplit))
		{
			RoomsArray.Add(RoomToSplit->Child1);
			RoomsArray.Add(RoomToSplit->Child2);
			RoomsArray.Remove(RoomToSplit);
		}
	}
	else UE_LOG(LogTemp, Warning, TEXT("BSP_SplitRoom_Hori Failed!"));
}

void ADungeonGenerator::TrimRooms()
{
	for (FRoom* iRoom : RoomsArray)
	{
		// Get a percent 10<=n<=30 of the width/height to cut from all sides of the room;
		const float WidthToCut_Left = ceilf(iRoom->Width * FMath::RandRange(10,30)/100);
		const float WidthToCut_Right = ceilf(iRoom->Width * FMath::RandRange(10,30)/100);
		const float HeightToCut_Top = ceilf(iRoom->Height * FMath::RandRange(10,30)/100);
		const float HeightToCut_Bot = ceilf(iRoom->Height * FMath::RandRange(10,30)/100);

		// UE_LOG(LogTemp, Warning, TEXT("iRoom->MinPos: %s, iRoom->MaxPos Before: %s "), *iRoom->MinPos.ToString(), *iRoom->MaxPos.ToString());
		iRoom->MinPos += FVector(100.0f * WidthToCut_Left, 100.0f * HeightToCut_Bot, 0.0f);
		iRoom->MaxPos -= FVector(100.0f * WidthToCut_Right, 100.0f * HeightToCut_Top, 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("iRoom->MinPos: %s, iRoom->MaxPos: %s, Width: %f, Height: %f,"), *iRoom->MinPos.ToString(), *iRoom->MaxPos.ToString(), iRoom->Width, iRoom->Height);

		iRoom->Width -= WidthToCut_Left + WidthToCut_Right;
		iRoom->Height -= HeightToCut_Top + HeightToCut_Bot;
	}
}

void ADungeonGenerator::SplitIntoTiles()
{
	for (FRoom* iRoom : RoomsArray)
	{
		for (float y = iRoom->MinPos.Y; y < iRoom->MaxPos.Y; y+=100.0f)
		{
			for (float x = iRoom->MinPos.X; x < iRoom->MaxPos.X; x+=100.0f)
			{
				FloorTiles.Add(FVector(x, y, 0.0f));
			}
		}
	}
}

void ADungeonGenerator::SpawnTiles()
{
	for (FVector Tile : FloorTiles)
	{
		MeshComponent->AddInstance(FTransform(Tile));
	}
}

void ADungeonGenerator::TempSpawnTiles()
{
	DrawDebugLine(GetWorld(), Map->MinPos + FVector(0.0f,0.0f, 100.0f), Map->MinPos + FVector(Map->Width*100.0f, 0.0f, 100.0f), FColor::Orange, true, -1, 0, 10);
	DrawDebugLine(GetWorld(), Map->MinPos + FVector(0.0f,0.0f, 100.0f), Map->MinPos + FVector(0.0f, Map->Height*100.0f, 100.0f), FColor::Orange, true, -1, 0, 10);
	for (FRoom* iRoom : RoomsArray)
	{
		MeshComponent->AddInstance(FTransform(iRoom->MinPos + FVector(0.0f, 0.0f, 100.0f)));
		MeshComponent->AddInstance(FTransform(iRoom->MaxPos + FVector(0.0f, 0.0f, 200.0f)));
		DrawDebugLine(GetWorld(), iRoom->MinPos + FVector(0.0f,0.0f, 100.0f), iRoom->MaxPos + FVector(0.0f,0.0f,100.0f), FColor::Red, true, -1, 0, 10);
		DrawDebugLine(GetWorld(), iRoom->MaxPos + FVector(0.0f,0.0f,100.0f), iRoom->MinPos + FVector(iRoom->Width*100.0f, 0.0f, 100.0f), FColor::Blue, true, -1, 0, 10);
		DrawDebugLine(GetWorld(), iRoom->MaxPos + FVector(0.0f,0.0f,100.0f), iRoom->MinPos + FVector(0.0f, iRoom->Height*100.0f, 100.0f), FColor::Blue, true, -1, 0, 10);

	}
}

void ADungeonGenerator::DebugFunction()
{
	// UE_LOG(LogTemp, Warning, TEXT("Foo"));
	for (auto ele : RoomsArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("Room 1 - Min: %s, Max: %s, Width: %f, Height: %f"), *(ele->MinPos.ToString()), *(ele->MaxPos.ToString()), ele->Width, ele->Height);
	}
}