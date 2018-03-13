// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitActor.generated.h"


//�a�ϤW�i�઺����
UENUM(BlueprintType)
enum class MapUnit : uint8
{
	//�S�F��
	None,
	//��
	Sheep,
	//�p��
	LittleSheep,
	//�T
	Wolf,
	//��
	Dog,
	//��
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

	//���ʤO
	UPROPERTY(EditDefaultsOnly, Category = "SheepAndWolf")
		AMyScene* MyScene;
	

	//���ʤO
	UPROPERTY(EditDefaultsOnly, Category = "AbilityValue")
	int32 Movement;

	//�w�g�Y�U�{���F��
	UPROPERTY(EditDefaultsOnly, Category = "AbilityValue")
	int32 NumOfAte = 0;

	//����{�b��m,�HIndex(0~399)���
	int32 CurrentPos;
	//��쪺����:�T �p�� �� ��M���䤤�@��
	MapUnit UnitType;
	//���}�`
	virtual void Dead();
	
	
};
