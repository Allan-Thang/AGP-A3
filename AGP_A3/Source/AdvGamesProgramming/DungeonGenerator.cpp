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
	GenerateMST(RoomsArray);
	SpawnTiles();
	TempSpawnTiles();
	DebugFunction();
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
		RoomToSplit->Child1->MidPoint = FVector((RoomToSplit->Child1->MinPos.X + RoomToSplit->Child1->MaxPos.X)/2, (RoomToSplit->Child1->MinPos.Y + RoomToSplit->Child1->MaxPos.Y)/2, 0.0f);
		RoomToSplit->Child1->Width = SplitAtTile;
		RoomToSplit->Child1->Height = RoomToSplit->Height;

		RoomToSplit->Child2 = new FRoom;
		RoomToSplit->Child2->Parent = RoomToSplit;
		RoomToSplit->Child2->MinPos = RoomToSplit->MinPos + FVector(SplitAtTile * 100.0f, 0.0f, 0.0f);
		RoomToSplit->Child2->MaxPos = RoomToSplit->MaxPos;
		RoomToSplit->Child2->MidPoint = FVector((RoomToSplit->Child2->MinPos.X + RoomToSplit->Child2->MaxPos.X)/2, (RoomToSplit->Child2->MinPos.Y + RoomToSplit->Child2->MaxPos.Y)/2, 0.0f);
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

		iRoom->MidPoint = FVector(iRoom->MinPos.X + iRoom->Width/2 * 100.0f, iRoom->MinPos.Y + iRoom->Height/2 * 100.0f, 0.0f);
	}
}

void ADungeonGenerator::GenerateMST(TArray<FRoom*> Rooms)
{
	FRoom* StartRoom = Rooms[0];
	// ConnectedRooms[StartRoom] = StartRoom;

	FRoomPair FirstPair;
	// Swap this later
	// FirstPair.CameFromRoom = nullptr;
	FRoom* ZeroRoom = new FRoom;
	ZeroRoom->MidPoint = FVector(0.0f,0.0f,0.0f);
	FirstPair.CameFromRoom = ZeroRoom;

	FirstPair.Room = StartRoom;

	ConnectedRooms.Add(FirstPair);
	Rooms.Remove(StartRoom);
	PopulateConnectedRooms(Rooms);
}

void ADungeonGenerator::PopulateConnectedRooms(TArray<FRoom*> Rooms)
{
	if (Rooms.Num() == 0) return;

	FRoomPair PairToAdd;
	PairToAdd.CameFromRoom = ConnectedRooms[0].Room;
	PairToAdd.Room = Rooms[0];

	for (FRoomPair iRoomPair : ConnectedRooms)
	{
		FRoom* RoomToAdd = Rooms[0];
		for (FRoom* iRoom : Rooms)
		{
			if (FVector::Dist(iRoomPair.Room->MidPoint, iRoom->MidPoint) < FVector::Dist(iRoomPair.Room->MidPoint, RoomToAdd->MidPoint))
			{
				RoomToAdd = iRoom;
			}
		}
		if (FVector::Dist( iRoomPair.Room->MidPoint, RoomToAdd->MidPoint) < FVector::Dist(PairToAdd.Room->MidPoint, PairToAdd.CameFromRoom->MidPoint))
		{
			PairToAdd.Room = RoomToAdd;
			PairToAdd.CameFromRoom = iRoomPair.Room;
		}
	}

	bool CycleCheck = false;
	for (auto Pair : ConnectedRooms)
	{
		CycleCheck = Pair.Room == PairToAdd.Room;
		if (CycleCheck) break;
	}

	if (!CycleCheck)
	{
		ConnectedRooms.Add(PairToAdd);
		Rooms.Remove(PairToAdd.Room);
		PopulateConnectedRooms(Rooms);
	}
}

void ADungeonGenerator::FindOverlap(FRoom* RoomA, FRoom* RoomB)
{
	/*
	 * X1 = Lower X
	 * X2 = Upper X
	 * Y1 = Lower Y
	 * Y2 = Upper Y
	 */
	const float RoomA_X1 = RoomA->MinPos.X;
	const float RoomA_X2 = RoomA->MinPos.X + RoomA->Width * 100.0f;
	const float RoomA_Y1 = RoomA->MinPos.Y;
	const float RoomA_Y2 = RoomA->MinPos.Y + RoomA->Height * 100.0f;

	const float RoomB_X1 = RoomB->MinPos.X;
	const float RoomB_X2 = RoomB->MinPos.X + RoomB->Width * 100.0f;
	const float RoomB_Y1 = RoomB->MinPos.Y;
	const float RoomB_Y2 = RoomB->MinPos.Y + RoomB->Height * 100.0f;

	/*  -----                -----
	 * |  A  |              |  A  |
	 *  -----        /       -----
	 *    -----            -----
	 *   |  B  |          |  B  |
	 *    -----            -----
	 */
	if (RoomA_X1 < RoomB_X2 && RoomA_X2 > RoomB_X1)
	{

		/*  -----
		 * |  A  |
		 *  -----
		 *    -----
		 *   |  B  |
		 *    -----
		 */
		if (RoomA_X1 < RoomB_X1 && RoomA_X2 < RoomB_X2)
		{
			return;
		}

		/*    -----
		 *   |  A  |
		 *    -----
		 *  -----
		 * |  B  |
		 *  -----
		 */
		else if (RoomA_X1 > RoomB_X2 && RoomA_X2 > RoomB_X2)
		{
			return;
		}

		/*  -------
		 * |   A   |
		 *  -------
		 *    ---
		 *   | B |
		 *    ---
		 */
		else if (RoomA_X1 < RoomB_X1 && RoomA_X2 > RoomB_X2)
		{
			return;
		}

		/*    ---
		 *   | A |
		 *    ---
		 *  -------
		 * |   B   |
		 *  -------
		 */
		else
		{
			return;
		}
	}
	/*  -----                          -----
	 * |  A  |  -----      /   -----  |  B  |
	 *  -----  |  B  |        |  A  |  -----
	 *          -----          -----
	 */
	else if (RoomA_Y1 < RoomB_Y2 && RoomA_Y2 < RoomB_Y1)
	{

		/*          -----
		 *  -----  |  B  |
		 * |  A  |  -----
		 *  -----
		 */
		if (RoomA_Y1 < RoomB_Y1 && RoomA_Y2 < RoomB_Y2)
		{

		}

		/*  -----
		 * |  A  |  -----
		 *  -----  |  B  |
		 *          -----
		 */
		else if (RoomA_Y1 > RoomB_Y1 && RoomA_Y2 > RoomB_Y2)
		{

		}

		/*  -----
		 * |     |  -----
		 * |  A  | |  B  |
		 * |     |  -----
		 *  -----
		 */
		else if (RoomA_Y1 < RoomB_Y2 && RoomA_Y2 > RoomB_Y2)
		{

		}

		/*          -----
		 *  -----  |     |
		 * |  A  | |  B  |
		 *  -----  |     |
		 *          -----
		 */
		else
		{

		}
	}
	FVector CorridorMidpoint = FVector(0.0f, 0.0f, 0.0f);
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
		DrawDebugBox(GetWorld(), iRoom->MidPoint, FVector(100.0f, 100.0f, 100.0f), FColor::Purple, true, -1, 0, 10);
	}
}

float ADungeonGenerator::RoundToHundred(float Value)
{
	int Value_I = Value;
	return ((Value_I % 100) < 50) ? Value_I - (Value_I % 100) : Value_I + (100 - Value_I % 100);
}

void ADungeonGenerator::DebugFunction()
{
	// UE_LOG(LogTemp, Warning, TEXT("Foo"));
	// for (auto ele : RoomsArray)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Room 1 - Min: %s, Max: %s, Width: %f, Height: %f"), *(ele->MinPos.ToString()), *(ele->MaxPos.ToString()), ele->Width, ele->Height);
	// }

	for (auto iPair : ConnectedRooms)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pair - Room: %s, CameFrom: %s"), *iPair.Room->MidPoint.ToString(), *iPair.CameFromRoom->MidPoint.ToString());
		DrawDebugLine(GetWorld(), iPair.Room->MidPoint + FVector(0.0f,0.0f, 100.0f), iPair.CameFromRoom->MidPoint + FVector(0.0f,0.0f,100.0f), FColor::Purple, true, -1, 0, 10);
	}
}