// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameFramework/Actor.h"
#include "MapSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FMapPointData {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int depth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool vis;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsMain;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool up;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool down;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool left;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int> DirectValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Value;
};

USTRUCT(BlueprintType)
struct FMapArray
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMapPointData> a;
};

/**
 * 
 */
UCLASS()
class TRYUSELUA_API UMapSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	// ����ί��
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreChanged, int32, CurrentScore);
	UPROPERTY(BlueprintAssignable)
	FScoreChanged ScoreChange;

	UFUNCTION(BlueprintCallable, Category = "MapSubsystem")
	int32 AddScore(int32 BaseScore);

	UFUNCTION(BlueprintCallable, Category = "MapSubsystem")
	AActor* GetActorInLevelByName(FString Name);

	UFUNCTION(BlueprintCallable, Category = "MapSubsystem")
	TArray<FMapArray> GetMapMatrix();

	UFUNCTION(BlueprintCallable, Category = "MapSubsystem")
	TArray<FVector2D> SetNewMap(int l, int w, int mainLength);

	UFUNCTION(BlueprintCallable, Category = "MapSubsystem")
	void SetPointValue(int Index, int Value);

	UFUNCTION(BlueprintCallable, Category = "MapSubsystem")
	int GetPointValue(int Index);

	UFUNCTION(BlueprintCallable, Category = "MapSubsystem")
	int GetPointFromValue(int Value);

	UFUNCTION(BlueprintCallable, Category = "MapSubsystem")
	int GetAllPointSum();

	UFUNCTION(BlueprintCallable, Category = "MapSubsystem")
	FVector2D GetVector2DFromPoint(int Index);
private:
	int fx[4] = { 0, 0, -1, 1};
	int fy[4] = { 1, -1, 0, 0};

	TArray<FMapArray> MapMatrix;
	unsigned int length;
	unsigned int width;
	unsigned int mainlength;
	void dfs(TArray<FVector2D>& PointList);

	TArray<int> PointValue;
	TArray<int> TreeArray;
	int TreeLength;
	int TreeLowbit(int x);
	void TreeAdd(int x, int v);
	int TreeGetSum(int x);
	
	int32 Score;
};
