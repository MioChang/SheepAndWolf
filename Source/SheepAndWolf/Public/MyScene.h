// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitActor.h"
#include "SheepUnit.h"
#include "DogUnit.h"
#include "WolfUnit.h"
#include "MyScene.generated.h"


UCLASS()
class SHEEPANDWOLF_API AMyScene : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyScene();

private:
	
protected:
	//這是要生成的物件:羊 狗 草 小羊 狼
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> bp_sheep;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> bp_dog;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> bp_grass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> bp_little;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> bp_wolf;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//遊戲限制
	int32 MaxDog = 1;
	int32 MaxSheep = 20;
	int32 MaxWolf = 1;
	int32 MAxGrass = 15;
	//第一次進入要先建場景
	bool isFirst = true;


	//現在回合
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int Turn = 0;

	//總共獲得金幣
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int Cold = 0;

	//狼吃掉小羊的次數
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int TotalLittleSheepEaten;

	//狼吃掉羊的次數
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int TotalSheepEaten;

	//草被吃的次數
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int TotalGrassEaten;

	//小羊吃滿草的次數
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int TotalLittleSheepFull;

	//羊吃滿草的次數
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int TotalSheepFull;

	//狗攻擊狼的次數
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int TotalDogAttack;

	//遊戲一開始的物件數目
	int32 InitSheep = 10;
	int32 InitDog = 1;
	int32 InitGrass = 5;
	//狼出現的間隔
	int32 WolfComeInterval = 0;

	
public:	
	//地圖現狀
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SheepAndWolf")
		TMap<int32, MapUnit> SceneMap;

	TArray<ASheepUnit*> Sheeps;
	TArray<ASheepUnit*> LittleSheeps;

	TArray<AUnitActor*> Grasses;

	AWolfUnit* Wolf;

	ADogUnit* Dog;

	//初始化遊戲參數
	UFUNCTION(BlueprintCallable,Category="Scene")
	void StartGame();
	//下個回合
	UFUNCTION(BlueprintCallable, Category = "Scene")
	void NextTurn();
	//跑到第一百個回合
	UFUNCTION(BlueprintCallable, Category = "Scene")
	void ToEndTurn();
	//建場景
	void CreateScene();

	//Position Index 0~399轉換成對應的世界座標
	FVector PosIndexToLocation(int32 PosIndex);

	//移動單位尋找可吃/攻擊的對象 , 回傳步數, 輸入是:移動單位位置, 移動單位移動力, 移動單位的目標
	int32 FindTarget( MapUnit Target , AUnitActor* Attacker);
	
};
