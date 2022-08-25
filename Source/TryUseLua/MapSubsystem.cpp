// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UMapSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Warning, TEXT("__FUNCTION__"));
}

void UMapSubsystem::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(LogTemp, Warning, TEXT("__FUNCTION__"));
}

int32 UMapSubsystem::AddScore(int32 BaseScore)
{
	Score = UKismetMathLibrary::Max(0, Score + BaseScore);
	// µ÷ÓÃÎ¯ÍÐ
	ScoreChange.Broadcast(Score);
	return Score;
}

AActor* UMapSubsystem::GetActorInLevelByName(FString Name)
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OutActors);
	for (AActor* actor : OutActors)
	{
		if (actor->GetName() == Name)
			return actor;
	}
	return nullptr;
}

TArray<FMapArray> UMapSubsystem::GetMapMatrix()
{
	return MapMatrix;
}

TArray<FVector2D> UMapSubsystem::SetNewMap(int l, int w, int mainLength)
{
	length = l;
	width = w;
	mainlength = mainLength;

	FMapPointData initPointData;
	initPointData.vis = false;
	initPointData.IsMain = false;
	initPointData.up = false;
	initPointData.down = false;
	initPointData.left = false;
	initPointData.right = false;
	initPointData.DirectValue.Init(0, 4);
	initPointData.Value = 0;
	for (unsigned int i = 0; i < length; ++i) {
		MapMatrix.Add(FMapArray());
		for (unsigned int j = 0; j < width; ++j) {
			MapMatrix[i].a.Emplace(initPointData);
		}
	}

	TArray<FVector2D> PointList;
	while (PointList.Num() != mainLength) {
		PointList.Reset();

		FVector startPoint;
		startPoint.X = FMath::RandRange(1, length);
		startPoint.Y = FMath::RandRange(1, width);
		MapMatrix[startPoint.X - 1].a[startPoint.Y - 1].vis = true;
		MapMatrix[startPoint.X - 1].a[startPoint.Y - 1].IsMain = true;
		MapMatrix[startPoint.X - 1].a[startPoint.Y - 1].depth = 1;

		PointList.Emplace(startPoint);

		dfs(PointList);
	}

	for (unsigned int i = 0; i < length; ++i) {
		for (unsigned int j = 0; j < width; ++j) {
			if(MapMatrix[i].a[j].vis == true)
				UE_LOG(LogTemp, Warning, TEXT("%d %d %d %d %d %d %d"), i, j, MapMatrix[i].a[j].depth,
					MapMatrix[i].a[j].up, MapMatrix[i].a[j].down, MapMatrix[i].a[j].left, MapMatrix[i].a[j].right);
		}
	}

	TreeLength = length * width;
	TreeArray.Init(0, length * width + 5);
	PointValue.Init(0, length * width + 5);
	UE_LOG(LogTemp, Warning, TEXT("%d"), TreeArray.Num())

	SetPointValue(1, 1);
	SetPointValue(2, 2);
	SetPointValue(5, 5);
	SetPointValue(7, 4);
	SetPointValue(10, 5);
	for (int i = 0; i <= 10; ++i)
		UE_LOG(LogTemp, Warning, TEXT("pointvalue[i] %d"), PointValue[i]);

	UE_LOG(LogTemp, Warning, TEXT("getpointfromValue(%d) %d"), 10, GetPointFromValue(10));

	for(int i = 0; i <= 18; ++i)
		UE_LOG(LogTemp, Warning, TEXT("getpointfromValue(%d) %d"), i, GetPointFromValue(i));
	//UE_LOG(LogTemp, Warning, TEXT("GetAllPointSum() %d"), GetAllPointSum());
	//UE_LOG(LogTemp, Warning, TEXT("%d"), MapMatrix[i].a[j]);

	return PointList;
}

void UMapSubsystem::dfs(TArray<FVector2D> &PointList) {
	TArray<FVector2D> RandomPointList;
	if (mainlength == PointList.Num())
		return;

	FVector2D Point = PointList[PointList.Num() - 1];
	if(Point.Y + 1 <= width && MapMatrix[Point.X - 1].a[Point.Y + 1 - 1].vis == false){
		RandomPointList.Emplace(Point.X, Point.Y + 1);
	}
	if (Point.Y - 1 >= 1 && MapMatrix[Point.X - 1].a[Point.Y - 1 - 1].vis == false) {
		RandomPointList.Emplace(Point.X, Point.Y - 1);
	}
	if (Point.X - 1 >= 1 && MapMatrix[Point.X - 1 - 1].a[Point.Y - 1].vis == false) {
		RandomPointList.Emplace(Point.X - 1, Point.Y);
	}
	if (Point.X + 1 <= length && MapMatrix[Point.X + 1 - 1].a[Point.Y - 1].vis == false) {
		RandomPointList.Emplace(Point.X + 1, Point.Y);
	}

	TArray<int> RandomOrder;
	for (int i = 1; i <= RandomPointList.Num(); ++i) {
		RandomOrder.Emplace(FMath::RandRange(0, RandomPointList.Num() - 1));
	}

	for (int i = 0; i < RandomOrder.Num(); ++i) {
		int index = RandomOrder[i];
		FVector2D NowPoint = RandomPointList[index];
		MapMatrix[NowPoint.X - 1].a[NowPoint.Y - 1].vis = true;
		MapMatrix[NowPoint.X - 1].a[NowPoint.Y - 1].IsMain = true;
		PointList.Emplace(NowPoint);
		MapMatrix[NowPoint.X - 1].a[NowPoint.Y - 1].depth = MapMatrix[Point.X - 1].a[Point.Y - 1].depth + 1;
		dfs(PointList);
		if (mainlength == PointList.Num()) {
			if (NowPoint.Y == Point.Y + 1) {
				MapMatrix[Point.X - 1].a[Point.Y - 1].up = true;
				MapMatrix[NowPoint.X - 1].a[NowPoint.Y - 1].down = true;
			}
			else if (NowPoint.X == Point.X - 1) {
				MapMatrix[Point.X - 1].a[Point.Y - 1].down = true;
				MapMatrix[NowPoint.X - 1].a[NowPoint.Y - 1].up = true;
			}
			else if (NowPoint.X == Point.X + 1) {
				MapMatrix[Point.X - 1].a[Point.Y - 1].left = true;
				MapMatrix[NowPoint.X - 1].a[NowPoint.Y - 1].right = true;
			}
			else if (NowPoint.Y == Point.Y - 1) {
				MapMatrix[Point.X - 1].a[Point.Y - 1].right = true;
				MapMatrix[NowPoint.X - 1].a[NowPoint.Y - 1].left = true;
			}
			return;
		}
		PointList.Pop();
		MapMatrix[NowPoint.X - 1].a[NowPoint.Y - 1].vis = false;
	}
}

int UMapSubsystem::TreeLowbit(int x) {
	return x & (-x);
}

void UMapSubsystem::TreeAdd(int x, int v)
{
	for (int i = x; i <= TreeLength; i += TreeLowbit(i)) {
		TreeArray[i] += v;
	}
}

int UMapSubsystem::TreeGetSum(int x)
{
	if (x == 0) return 0;
	int sum = 0;
	for (int i = x; i > 0; i -= TreeLowbit(i))
		sum += TreeArray[i];
	return sum;
}

void UMapSubsystem::SetPointValue(int Index, int Value)
{
	if (1 <= Index && Index <= TreeLength) {
		TreeAdd(Index, Value - PointValue[Index]);
		PointValue[Index] = Value;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("UMapSubsystem::SetPointValue : Bad Index!"));
	}
}

int UMapSubsystem::GetPointValue(int Index)
{
	if (1 <= Index && Index <= TreeLength) {
		return PointValue[Index];
	}
	UE_LOG(LogTemp, Warning, TEXT("UMapSubsystem::GetPointValue : Bad Index!"));
	return -1;
}

int UMapSubsystem::GetPointFromValue(int Value)
{
	int L = 1, R = TreeLength, ans = -1;
	if (Value < 1 || Value > GetAllPointSum()) {
		UE_LOG(LogTemp, Warning, TEXT("UMapSubsystem::GetPointFromValue : Bad Value!"));
		return -1;
	}
	while (L <= R) {
		int Mid = ((R - L) >> 1) + L;
		if (PointValue[Mid] > 0 && TreeGetSum(Mid - 1) < Value && Value <= TreeGetSum(Mid)) {
			ans = Mid;
			break;
		}
		if (Value <= TreeGetSum(Mid)) {
			R = Mid - 1;
		}
		else {
			L = Mid + 1;
		}
		// UE_LOG(LogTemp, Warning, TEXT("Mid %d L %d R %d TreeGetSum %d"), Mid, L, R, TreeGetSum(Mid));
	}
	return ans;
}

int UMapSubsystem::GetAllPointSum()
{
	return TreeGetSum(TreeLength);
}

FVector2D UMapSubsystem::GetVector2DFromPoint(int Index)
{
	if (1 <= Index && Index <= TreeLength) {
		return FVector2D(((Index + 1) / width) + 1, ((Index + 1) % width) + 1);
	}
	UE_LOG(LogTemp, Warning, TEXT("UMapSubsystem::GetPointValue : Bad Index!"));
	return -1;
}
