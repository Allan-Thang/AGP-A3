// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent =	CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComponent;
	MeshComponent->SetMobility(EComponentMobility::Static);
	MeshComponent->SetCollisionProfileName("BlockAll");


	RoomCount = 10;
	RoomSize_Min = 10;
	RoomSize_Max = 100;

	MapSize = 100 * RoomSize_Max * RoomCount;
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
		Map->Width = Map->MaxPos.X / 100;
		Map->Height = Map->MaxPos.Y / 100;
	}

	RoomsArray.Add(Map);
	SplitIntoRooms(RoomCount-1);
	DebugFunction();
	for (auto ele : RoomsArray)
	{
		MeshComponent->AddInstance(FTransform(ele->MinPos));
	}
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

void ADungeonGenerator::SplitIntoRooms(unsigned TimesToSplit)
{
	if (TimesToSplit <= 0)
		return;
	// Choose which room to split by finding the Room with the largest area
	FRoom* RoomToSplit = RoomsArray[0];
	if (RoomToSplit)
	{
		for (FRoom* IRoom : RoomsArray)
		{
			if ((IRoom->Width * IRoom->Height) > (RoomToSplit->Width * RoomToSplit->Height))
			{
				RoomToSplit = IRoom;
			}
		}
		// Split vert if it wider than it is tall and vice versa.
		if (RoomToSplit->Width > RoomToSplit->Height)
		{
			BSP_SplitRoom_Vert(RoomToSplit);
			// UE_LOG(LogTemp, Warning, TEXT("Split Veritcally"));
		}
		else
		{
			BSP_SplitRoom_Hor(RoomToSplit);
			// UE_LOG(LogTemp, Warning, TEXT("Split Horizontally"));
		}

		TimesToSplit--;
		SplitIntoRooms(TimesToSplit);
	}
}

void ADungeonGenerator::BSP_SplitRoom_Vert(FRoom* RoomToSplit)
{
	if (RoomToSplit)
	{
		const int SplitAtTile = FMath::RandRange(1.0f, RoomToSplit->Width);
		RoomToSplit->Child1 = new FRoom;
		RoomToSplit->Child1->Parent = RoomToSplit;
		RoomToSplit->Child1->MinPos = RoomToSplit->MinPos;
		RoomToSplit->Child1->MaxPos = FVector(SplitAtTile * 100.0f, RoomToSplit->MaxPos.Y, 0.0f);
		RoomToSplit->Child1->Width = SplitAtTile;
		RoomToSplit->Child1->Height = RoomToSplit->Height;

		RoomToSplit->Child2 = new FRoom;
		RoomToSplit->Child2->Parent = RoomToSplit;
		RoomToSplit->Child2->MinPos = FVector(SplitAtTile * 100.0f, RoomToSplit->MinPos.Y, 0.0f);
		RoomToSplit->Child2->MaxPos = RoomToSplit->MaxPos;
		RoomToSplit->Child2->Width = RoomToSplit->Width - SplitAtTile;
		RoomToSplit->Child2->Height = RoomToSplit->Height;

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
		const int SplitAtTile = FMath::RandRange(1.0f, RoomToSplit->Height);
		RoomToSplit->Child1 = new FRoom;
		RoomToSplit->Child1->Parent = RoomToSplit;
		RoomToSplit->Child1->MinPos = RoomToSplit->MinPos;
		RoomToSplit->Child1->MaxPos = FVector(RoomToSplit->MaxPos.X, SplitAtTile * 100.0f, 0.0f);
		RoomToSplit->Child1->Width = RoomToSplit->Width;
		RoomToSplit->Child1->Height = SplitAtTile;

		RoomToSplit->Child2 = new FRoom;
		RoomToSplit->Child2->Parent = RoomToSplit;
		RoomToSplit->Child2->MinPos = FVector(RoomToSplit->MinPos.X, SplitAtTile * 100.0f, 0.0f);
		RoomToSplit->Child2->MaxPos = RoomToSplit->MaxPos;
		RoomToSplit->Child2->Width = RoomToSplit->Width;
		RoomToSplit->Child2->Height = RoomToSplit->Height - SplitAtTile;

		if (RoomsArray.Contains(RoomToSplit))
		{
			RoomsArray.Add(RoomToSplit->Child1);
			RoomsArray.Add(RoomToSplit->Child2);
			RoomsArray.Remove(RoomToSplit);
		}
	}
	else UE_LOG(LogTemp, Warning, TEXT("BSP_SplitRoom_Vert Failed!"));
}

void ADungeonGenerator::DebugFunction()
{
	// UE_LOG(LogTemp, Warning, TEXT("Foo"));
	for (auto ele : RoomsArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *(ele->MinPos.ToString()));
	}
}