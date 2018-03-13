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
	//�o�O�n�ͦ�������:�� �� �� �p�� �T
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

	//�C������
	int32 MaxDog = 1;
	int32 MaxSheep = 20;
	int32 MaxWolf = 1;
	int32 MAxGrass = 15;
	//�Ĥ@���i�J�n���س���
	bool isFirst = true;


	//�{�b�^�X
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int Turn = 0;

	//�`�@��o����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int Cold = 0;

	//�T�Y���p�Ϫ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int TotalLittleSheepEaten;

	//�T�Y���Ϫ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int TotalSheepEaten;

	//��Q�Y������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int TotalGrassEaten;

	//�p�ϦY���󪺦���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int TotalLittleSheepFull;

	//�ϦY���󪺦���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int TotalSheepFull;

	//�������T������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int TotalDogAttack;

	//�C���@�}�l������ƥ�
	int32 InitSheep = 10;
	int32 InitDog = 1;
	int32 InitGrass = 5;
	//�T�X�{�����j
	int32 WolfComeInterval = 0;

	
public:	
	//�a�ϲ{��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SheepAndWolf")
		TMap<int32, MapUnit> SceneMap;

	TArray<ASheepUnit*> Sheeps;
	TArray<ASheepUnit*> LittleSheeps;

	TArray<AUnitActor*> Grasses;

	AWolfUnit* Wolf;

	ADogUnit* Dog;

	//��l�ƹC���Ѽ�
	UFUNCTION(BlueprintCallable,Category="Scene")
	void StartGame();
	//�U�Ӧ^�X
	UFUNCTION(BlueprintCallable, Category = "Scene")
	void NextTurn();
	//�]��Ĥ@�ʭӦ^�X
	UFUNCTION(BlueprintCallable, Category = "Scene")
	void ToEndTurn();
	//�س���
	void CreateScene();

	//Position Index 0~399�ഫ���������@�ɮy��
	FVector PosIndexToLocation(int32 PosIndex);

	//���ʳ��M��i�Y/��������H , �^�ǨB��, ��J�O:���ʳ���m, ���ʳ�첾�ʤO, ���ʳ�쪺�ؼ�
	int32 FindTarget( MapUnit Target , AUnitActor* Attacker);
	
};
