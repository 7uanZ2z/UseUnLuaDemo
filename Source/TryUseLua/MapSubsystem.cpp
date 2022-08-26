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

bool UMapSubsystem::MapPointIsValid(int x, int y) {
	if (x < 0 || y < 0) return false;
	if (y >= (int)width || x >= (int)length) return false;
	return true;
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
	initPointData.depth = -1;
	initPointData.up = false;
	initPointData.down = false;
	initPointData.left = false;
	initPointData.right = false;
	initPointData.DirectValue.Init(0, 4);
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

	TreeLength = length * width;
	TreeArray.Init(0, length * width + 5);
	PointValue.Init(0, length * width + 5);
	UE_LOG(LogTemp, Warning, TEXT("TreeArray.Num() %d"), TreeArray.Num());

	for (unsigned int i = 0; i < length; ++i) {
		for (unsigned int j = 0; j < width; ++j) {
			if (MapMatrix[i].a[j].IsMain == true && MapMatrix[i].a[j].depth != 1 && MapMatrix[i].a[j].depth != mainLength) {
				int nowx, nowy;
				bool Conn[4] = {false};
				Conn[0] = MapMatrix[i].a[j].up;
				Conn[1] = MapMatrix[i].a[j].down;
				Conn[2] = MapMatrix[i].a[j].left;
				Conn[3] = MapMatrix[i].a[j].right;
				int sum = 0;
				for (int k = 0; k < 4; ++k) {
					if (Conn[k] == false) {
						nowx = i + fx[k];
						nowy = j + fy[k];
						if (MapPointIsValid(nowx, nowy)) {
							if (MapMatrix[nowx].a[nowy].IsMain)
								MapMatrix[i].a[j].DirectValue[k] = 0;
							else MapMatrix[i].a[j].DirectValue[k] = 1;
						}
					}
					sum += MapMatrix[i].a[j].DirectValue[k];
				}
				//UE_LOG(LogTemp, Warning, TEXT("%d %d %d"), i, j, sum);
				SetPointValue(GetPointFromVector2D(i, j), sum);
			}
		}
	}
	
	PrintMap(mainLength);

	int BranchSum = 8;
	for (int T = 1; T <= BranchSum; ++T) {
		int PointSampling = FMath::RandRange(1, GetAllPointSum());
		int nowPoint = GetPointFromValue(PointSampling);
		FVector2D vec = GetVector2DFromPoint(nowPoint);
		UE_LOG(LogTemp, Warning, TEXT("vec.X %d  vec.Y %d"), (int)vec.X, (int)vec.Y);
		int sum = 0;
		for (int k = 0; k < 4; ++k) {
			sum += MapMatrix[vec.X].a[vec.Y].DirectValue[k];
		}

		int DirectSampling = FMath::RandRange(1, sum);
		int di = -1;
		int DirectSum[4] = { 0 };
		DirectSum[0] = MapMatrix[vec.X].a[vec.Y].DirectValue[0];
		for (int i = 1; i <= 3; ++i) {
			DirectSum[i] = DirectSum[i - 1] + MapMatrix[vec.X].a[vec.Y].DirectValue[i];
		}
		if (DirectSampling <= DirectSum[0]) {
			di = 0;
		}else if (DirectSampling <= DirectSum[1]) {
			di = 1;
		}else if (DirectSampling <= DirectSum[2]) {
			di = 2;
		}else if (DirectSampling <= DirectSum[3]) {
			di = 3;
		}
		/*
		for (int i = 0; i < 4; ++i) {
			UE_LOG(LogTemp, Warning, TEXT("i %d  DirectValue %d"), i, MapMatrix[vec.X].a[vec.Y].DirectValue[i]);
		}
		UE_LOG(LogTemp, Warning, TEXT("DirectSampling %d  sum %d di %d"), DirectSampling, sum, di);*/

		int nowx = vec.X + fx[di];
		int nowy = vec.Y + fy[di];

		if (MapMatrix[nowx].a[nowy].vis) {
			//UE_LOG(LogTemp, Warning, TEXT("new branch has visit %d  %d"), nowx, nowy);
			++BranchSum;
		}

		MapMatrix[nowx].a[nowy].vis = true;
		MapMatrix[nowx].a[nowy].depth = FMath::Min(MapMatrix[nowx].a[nowy].depth, MapMatrix[vec.X].a[vec.Y].depth + 1);
		if (di == 0) {
			MapMatrix[vec.X].a[vec.Y].up = true;
			MapMatrix[nowx].a[nowy].down = true;
		}else if (di == 1) {
			MapMatrix[vec.X].a[vec.Y].down = true;
			MapMatrix[nowx].a[nowy].up = true;
		}else if (di == 2) {
			MapMatrix[vec.X].a[vec.Y].left = true;
			MapMatrix[nowx].a[nowy].right = true;
		}else if (di == 3) {
			MapMatrix[vec.X].a[vec.Y].right = true;
			MapMatrix[nowx].a[nowy].left = true;
		}

		SetPointValue(GetPointFromVector2D(vec.X, vec.Y), PointValue[GetPointFromVector2D(vec.X, vec.Y)] - MapMatrix[vec.X].a[vec.Y].DirectValue[di]);
		MapMatrix[vec.X].a[vec.Y].DirectValue[di] = 0;

		bool Conn[4] = { false };
		Conn[0] = MapMatrix[nowx].a[nowy].up;
		Conn[1] = MapMatrix[nowx].a[nowy].down;
		Conn[2] = MapMatrix[nowx].a[nowy].left;
		Conn[3] = MapMatrix[nowx].a[nowy].right;
		sum = 0;
		for (int k = 0; k < 4; ++k) {
			MapMatrix[nowx].a[nowy].DirectValue[k] = 0;
			if (Conn[k] == false) {
				int newnowx = nowx + fx[k];
				int newnowy = nowy + fy[k];
				if (MapPointIsValid(newnowx, newnowy)) {
					//UE_LOG(LogTemp, Warning, TEXT("Valid newnox %d newnowy %d"), newnowx, newnowy);
					MapMatrix[nowx].a[nowy].DirectValue[k] = 2;
					if (MapMatrix[newnowx].a[newnowy].vis) {
						MapMatrix[nowx].a[nowy].DirectValue[k] = 1;
					}
				}
			}
			sum += MapMatrix[nowx].a[nowy].DirectValue[k];
		}

		int newnowx = nowx + fx[di];
		int newnowy = nowy + fy[di];
		if (!Conn[di] && MapPointIsValid(newnowx, newnowy)) {
			sum += 4 - MapMatrix[nowx].a[nowy].DirectValue[di];
			MapMatrix[nowx].a[nowy].DirectValue[di] = 4;
			
		}

		SetPointValue(GetPointFromVector2D(nowx, nowy), sum);

		FVector2D NowPoint(nowx + 1, nowy + 1);
		PointList.Emplace(NowPoint);
	}

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
			if (NowPoint.X == Point.X - 1) {
				MapMatrix[Point.X - 1].a[Point.Y - 1].up = true;
				MapMatrix[NowPoint.X - 1].a[NowPoint.Y - 1].down = true;
			}
			else if (NowPoint.X == Point.X + 1) {
				MapMatrix[Point.X - 1].a[Point.Y - 1].down = true;
				MapMatrix[NowPoint.X - 1].a[NowPoint.Y - 1].up = true;
			}
			else if (NowPoint.Y == Point.Y - 1) {
				MapMatrix[Point.X - 1].a[Point.Y - 1].left = true;
				MapMatrix[NowPoint.X - 1].a[NowPoint.Y - 1].right = true;
			}
			else if (NowPoint.Y == Point.Y + 1) {
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
		return FVector2D(((Index - 1) / width), ((Index - 1) % width));
	}
	UE_LOG(LogTemp, Warning, TEXT("UMapSubsystem::GetPointValue : Bad Index!"));
	return FVector2D(-1, -1);
}

int UMapSubsystem::GetPointFromVector2D(int x, int y) {
	return x * width + y + 1;
}

void UMapSubsystem::PrintMap(int mainLength){
	for (unsigned int i = 0; i < length; ++i) {
		FString ss = "";
		for (unsigned int j = 0; j < width; ++j) {
			if (MapMatrix[i].a[j].IsMain == true) {
				if (MapMatrix[i].a[j].depth == 1)
					ss += "S ";
				else if (MapMatrix[i].a[j].depth == mainLength)
					ss += "E ";
				else ss += FString::Printf(TEXT("%d "), PointValue[GetPointFromVector2D(i, j)]);
			}
			else {
				ss += "* ";
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ss);
	}
}