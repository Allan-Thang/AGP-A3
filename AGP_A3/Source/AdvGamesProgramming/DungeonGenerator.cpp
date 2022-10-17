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

	RoomCount = 10.0f;
	MapScale = 10.0f;
}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();

	GenerateDungeon();
	// DebugFunction();
}


// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADungeonGenerator::GenerateDungeon()
{
	MapSize = 100.0f * MapScale * RoomCount;
	Map = new FRoom;

	if (Map)
	{
		Map->MinPos = FVector(0.0f, 0.0f,0.0f);
		Map->MaxPos = Map->MinPos + FVector(MapSize, MapSize, 0);
		Map->Width = MapSize / 100.0f;
		Map->Height = MapSize / 100.0f;
	}

	RoomsArray.Add(Map);
	SplitIntoRooms(RoomCount-1);
	TrimRooms();
	for (FRoom* RoomToSplit : RoomsArray)
	{
		FloorTiles.Append(SplitRoomIntoTiles(RoomToSplit));
	}
	GenerateMST(RoomsArray);
	SpawnTiles(FloorTiles);
	if (WallGenerator)
	{
		WallGenerator->GenerateWalls(FloorTiles);
	}
	else UE_LOG(LogTemp, Warning, TEXT("No WallGenerator"));

	// TempSpawnTiles();
	// DebugFunction();

	if (SpawnPlayer)
	{
		APlayerCharacter* PlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(Player, RoomsArray[0]->MidPoint + FVector(0.0f, 0.0f, 200.0f), FRotator::ZeroRotator);
		PlayerCharacter->AutoPossessPlayer = EAutoReceiveInput::Player0;
	}
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
		}
		else
		{
			BSP_SplitRoom_Hor(RoomToSplit);
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
		const float WidthToCut_Left = floorf(iRoom->Width * FMath::RandRange(10,30)/100);
		const float WidthToCut_Right = floorf(iRoom->Width * FMath::RandRange(10,30)/100);
		const float HeightToCut_Top = floorf(iRoom->Height * FMath::RandRange(10,30)/100);
		const float HeightToCut_Bot = floorf(iRoom->Height * FMath::RandRange(10,30)/100);

		iRoom->MinPos += FVector(100.0f * WidthToCut_Left, 100.0f * HeightToCut_Bot, 0.0f);
		iRoom->MaxPos -= FVector(100.0f * WidthToCut_Right, 100.0f * HeightToCut_Top, 0.0f);

		iRoom->Width -= WidthToCut_Left + WidthToCut_Right;
		iRoom->Height -= HeightToCut_Top + HeightToCut_Bot;

		iRoom->MidPoint = FVector(iRoom->MinPos.X + iRoom->Width/2 * 100.0f, iRoom->MinPos.Y + iRoom->Height/2 * 100.0f, 0.0f);
	}
}

void ADungeonGenerator::GenerateMST(TArray<FRoom*> Rooms)
{
	FRoom* StartRoom = Rooms[0];

	FRoom* ZeroRoom = new FRoom;
	ZeroRoom->MidPoint = FVector(0.0f,0.0f,0.0f);

	FRoomPair FirstPair;
	FirstPair.CameFromRoom = ZeroRoom;
	FirstPair.Room = StartRoom;

	ConnectedRooms.Add(FirstPair);
	Rooms.Remove(StartRoom);
	PopulateConnectedRooms(Rooms);

	for (FRoomPair iRoomPair : ConnectedRooms)
	{
		if (iRoomPair.Room == FirstPair.Room) continue;
		AddCorridors(iRoomPair.Room, iRoomPair.CameFromRoom);
	}
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

void ADungeonGenerator::AddCorridors(FRoom* RoomA, FRoom* RoomB)
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

	FRoom* Corridor = new FRoom;

	/*
	 * Vertical Corridor
	 */
	if (RoomA_X1 < RoomB_X2 && RoomA_X2 > RoomB_X1)
	{
		float CorridorStartY;
		float CorridorEndY;

		if (RoomA_Y1 > RoomB_Y1) //A is above B
		{
			CorridorStartY = RoomB_Y2;
			CorridorEndY = RoomA_Y1;
		}
		else //B is above A
		{
			CorridorStartY = RoomA_Y2;
			CorridorEndY = RoomB_Y1;
		}

		/*  -----
		 * |  A  |
		 *  -----
		 *    -----
		 *   |  B  |
		 *    -----
		 */
		if (RoomA_X1 <= RoomB_X1 && RoomA_X2 <= RoomB_X2)
		{
			const FVector CorridorStart = FVector(RoundToHundred((RoomA_X2 + RoomB_X1)/2) - 100.0f, CorridorStartY + 100.0f, 0.0f);
			const FVector CorridorEnd = FVector(RoundToHundred((RoomA_X2 + RoomB_X1)/2), CorridorEndY - 100.0f, 0.0f);

			Corridor->MinPos = CorridorStart;
			Corridor->MaxPos = CorridorEnd;
		}

		/*    -----
		 *   |  A  |
		 *    -----
		 *  -----
		 * |  B  |
		 *  -----
		 */
		else if (RoomA_X1 >= RoomB_X1 && RoomA_X2 >= RoomB_X2)
		{
			const FVector CorridorStart = FVector(RoundToHundred((RoomA_X1 + RoomB_X2)/2) - 100.0f, CorridorStartY + 100.0f, 0.0f);
			const FVector CorridorEnd = FVector(RoundToHundred((RoomA_X1 + RoomB_X2)/2), CorridorEndY - 100.0f, 0.0f);

			Corridor->MinPos = CorridorStart;
			Corridor->MaxPos = CorridorEnd;
		}

		/*  -------
		 * |   A   |
		 *  -------
		 *    ---
		 *   | B |
		 *    ---
		 */
		else if (RoomA_X1 <= RoomB_X1 && RoomA_X2 >= RoomB_X2)
		{
			const FVector CorridorStart = FVector(RoundToHundred(RoomB->MidPoint.X) - 100.0f, CorridorStartY + 100.0f, 0.0f);
			const FVector CorridorEnd = FVector(RoundToHundred(RoomB->MidPoint.X), CorridorEndY - 100.0f, 0.0f);

			Corridor->MinPos = CorridorStart;
			Corridor->MaxPos = CorridorEnd;
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
			const FVector CorridorStart = FVector(RoundToHundred(RoomA->MidPoint.X) - 100.0f, CorridorStartY + 100.0f, 0.0f);
			const FVector CorridorEnd = FVector(RoundToHundred(RoomA->MidPoint.X), CorridorEndY - 100.0f, 0.0f);

			Corridor->MinPos = CorridorStart;
			Corridor->MaxPos = CorridorEnd;
		}
	}
	/*
	 * Horizontal Corridor
	 */
	else if (RoomA_Y1 < RoomB_Y2 && RoomA_Y2 > RoomB_Y1)
	{
		float CorridorStartX;
		float CorridorEndX;

		if (RoomA_X1 > RoomB_X1) //A is left of B. Corridors are build right to left bc Unreal :/
			{
				CorridorStartX = RoomB_X2;
				CorridorEndX = RoomA_X1;
			}
		else //B is left of A
			{
				CorridorStartX = RoomA_X2;
				CorridorEndX = RoomB_X1;
			}

		/*          -----
		 *  -----  |  B  |
		 * |  A  |  -----
		 *  -----
		 */
		if (RoomA_Y1 <= RoomB_Y1 && RoomA_Y2 <= RoomB_Y2)
		{
			const FVector CorridorStart = FVector(CorridorStartX + 100.0f, RoundToHundred((RoomA_Y2 + RoomB_Y1)/2) - 100.0f, 0.0f);
			const FVector CorridorEnd = FVector(CorridorEndX - 100.0f, RoundToHundred((RoomA_Y2 + RoomB_Y1)/2), 0.0f);

			Corridor->MinPos = CorridorStart;
			Corridor->MaxPos = CorridorEnd;
		}

		/*  -----
		 * |  A  |  -----
		 *  -----  |  B  |
		 *          -----
		 */
		else if (RoomA_Y1 >= RoomB_Y1 && RoomA_Y2 >= RoomB_Y2)
		{
			const FVector CorridorStart = FVector(CorridorStartX + 100.0f, RoundToHundred((RoomA_Y1 + RoomB_Y2)/2) - 100.0f, 0.0f);
			const FVector CorridorEnd = FVector(CorridorEndX - 100.0f, RoundToHundred((RoomA_Y1 + RoomB_Y2)/2), 0.0f);

			Corridor->MinPos = CorridorStart;
			Corridor->MaxPos = CorridorEnd;
		}

		/*  -----
		 * |     |  -----
		 * |  A  | |  B  |
		 * |     |  -----
		 *  -----
		 */
		else if (RoomA_Y1 <= RoomB_Y2 && RoomA_Y2 >= RoomB_Y2)
		{
			const FVector CorridorStart = FVector(CorridorStartX + 100.0f, RoundToHundred((RoomB_Y1 + RoomB_Y2)/2) - 100.0f, 0.0f);
			const FVector CorridorEnd = FVector(CorridorEndX - 100.0f, RoundToHundred((RoomB_Y1 + RoomB_Y2)/2), 0.0f);

			Corridor->MinPos = CorridorStart;
			Corridor->MaxPos = CorridorEnd;
		}

		/*          -----
		 *  -----  |     |
		 * |  A  | |  B  |
		 *  -----  |     |
		 *          -----
		 */
		else
		{
			const FVector CorridorStart = FVector(CorridorStartX + 100.0f, RoundToHundred((RoomA_Y1 + RoomA_Y2)/2) - 100.0f, 0.0f);
			const FVector CorridorEnd = FVector(CorridorEndX - 100.0f, RoundToHundred((RoomA_Y1 + RoomA_Y2)/2), 0.0f);

			Corridor->MinPos = CorridorStart;
			Corridor->MaxPos = CorridorEnd;
		}
	}
	else // No Overlap
	{
		FRoom* Corridor2 = new FRoom;

		FVector Corridor1Start;
		FVector Corridor1End;

		FVector Corridor2Start;
		FVector Corridor2End;

		if (RoomA_X1 > RoomB_X2 && RoomA_Y2 < RoomB_Y1) // B is Bottom-Right of A
		{
			Corridor1Start = FVector(RoomA_X1, RoomA_Y2 + 100.0f, 0.0f);
			Corridor1End = FVector(RoomA_X1 + 100.0f, RoomB_Y1 + 100.0f, 0.0f);

			Corridor2Start = FVector(RoomB_X2 + 100.0f, RoomB_Y1, 0.0f);
			Corridor2End = FVector(RoomA_X1 - 100.0f, RoomB_Y1 + 100.0f, 0.0f);
		}
		else if (RoomA_X1 > RoomB_X2 && RoomA_Y1 > RoomB_Y2) // B is Bottom-Left of A
		{
			Corridor1Start = FVector(RoomB_X2 - 100.0f, RoomB_Y2 + 100.0f, 0.0f);
			Corridor1End = FVector(RoomB_X2, RoomA_Y1 + 100.0f, 0.0f);

			Corridor2Start = FVector(RoomB_X2 + 100.0f, RoomA_Y1, 0.0f);
			Corridor2End = FVector(RoomA_X1 - 100.0f, RoomA_Y1 + 100.0f, 0.0f);
		}
		else if (RoomA_X2 < RoomB_X1 && RoomA_Y1 > RoomB_Y2) // B is Top-Left of A
		{
			Corridor1Start = FVector(RoomA_X2 - 100.0f, RoomB_Y2 - 100.0f, 0.0f);
			Corridor1End = FVector(RoomB_X1 - 100.0f, RoomB_Y2, 0.0f);

			Corridor2Start = FVector(RoomA_X2 - 100.0f, RoomB_Y2 + 100.0f, 0.0f);
			Corridor2End = FVector(RoomA_X2, RoomA_Y1 - 100.0f, 0.0f);
		}
		else if (RoomA_X2 < RoomB_X1 && RoomA_Y2 < RoomB_Y1) // B is Top-Right of A
		{
			Corridor1Start = FVector(RoomA_X2 + 100.0f, RoomA_Y2 - 100.0f, 0.0f);
			Corridor1End = FVector(RoomB_X1 + 100.0f, RoomA_Y2, 0.0f);

			Corridor2Start = FVector(RoomB_X1, RoomA_Y2 + 100.0f, 0.0f);
			Corridor2End = FVector(RoomB_X1 + 100.0f, RoomB_Y1 - 100.0f, 0.0f);
		}

		Corridor->MinPos = Corridor1Start;
		Corridor->MaxPos = Corridor1End;

		Corridor2->MinPos = Corridor2Start;
		Corridor2->MaxPos = Corridor2End;

		FloorTiles.Append(SplitRoomIntoTiles(Corridor2));
		DrawDebugLine(GetWorld(), Corridor2->MinPos + FVector(0.0f,0.0f, 100.0f), Corridor2->MaxPos + FVector(0.0f,0.0f,100.0f), FColor::Cyan, true, -1, 0, 10);
	}
	FloorTiles.Append(SplitRoomIntoTiles(Corridor));
	DrawDebugLine(GetWorld(), Corridor->MinPos + FVector(0.0f,0.0f, 100.0f), Corridor->MaxPos + FVector(0.0f,0.0f,100.0f), FColor::Cyan, true, -1, 0, 10);
}

TArray<FVector> ADungeonGenerator::SplitRoomIntoTiles(FRoom* RoomToSplit)
{
	TArray<FVector> Tiles;

	for (float y = RoomToSplit->MinPos.Y; y <= RoomToSplit->MaxPos.Y; y+=100.0f)
	{
		for (float x = RoomToSplit->MinPos.X; x <= RoomToSplit->MaxPos.X; x+=100.0f)
		{
			Tiles.Add(FVector(x, y, 0.0f));
		}
	}

	return Tiles;
}

void ADungeonGenerator::SpawnTiles(const TArray<FVector>& Tiles)
{
	for (FVector Tile : Tiles)
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
		// UE_LOG(LogTemp, Warning, TEXT("Pair - Room: %s, CameFrom: %s"), *iPair.Room->MidPoint.ToString(), *iPair.CameFromRoom->MidPoint.ToString());
		DrawDebugLine(GetWorld(), iPair.Room->MidPoint + FVector(0.0f,0.0f, 100.0f), iPair.CameFromRoom->MidPoint + FVector(0.0f,0.0f,100.0f), FColor::Purple, true, -1, 0, 10);
	}
}