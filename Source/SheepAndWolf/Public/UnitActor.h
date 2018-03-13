// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitActor.generated.h"


//地圖上可能的物件
UENUM(BlueprintType)
enum class MapUnit : uint8
{
	//沒東西
	None,
	//羊
	Sheep,
	//小羊
	LittleSheep,
	//狼
	Wolf,
	//狗
	Dog,
	//草
	Grass,
};


class AMyScene;


UCLASS()
class SHEEPANDWOLF_API AUnitActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//移動力
	UPROPERTY(EditDefaultsOnly, Category = "SheepAndWolf")
		AMyScene* MyScene;
	

	//移動力
	UPROPERTY(EditDefaultsOnly, Category = "AbilityValue")
	int32 Movement;

	//已經吃下肚的東西
	UPROPERTY(EditDefaultsOnly, Category = "AbilityValue")
	int32 NumOfAte = 0;

	//物件現在位置,以Index(0~399)表示
	int32 CurrentPos;
	//單位的種類:狼 小羊 羊 草和狗其中一種
	MapUnit UnitType;
	//單位陣亡
	virtual void Dead();
	
	
};
