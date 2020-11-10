// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NetworkControllerComponent.generated.h"

UENUM(BlueprintType)
enum EGameType
{
	None		UMETA(DisplayName = "None"),
	Minigame1	UMETA(DisplayName = "Minigame 1"),
	Minigame2	UMETA(DisplayName = "Minigame 2"),
	Minigame3	UMETA(DisplayName = "Minigame 3"),
	GameServer	UMETA(DisplayName = "Game Server")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNetworkEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PAKOPELI_2020_API UNetworkControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNetworkControllerComponent();

	UPROPERTY(BlueprintAssignable)
	FNetworkEvent CheckReadyStatus;

	UPROPERTY(BlueprintAssignable)
	FNetworkEvent InitializeHUD;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Controlled Game")
	TEnumAsByte<EGameType> ControlledGame = EGameType::None;

	UPROPERTY(EditAnywhere)
	bool Ready = false;

	UFUNCTION(BlueprintCallable)
	void ClientReadyUp();

	UFUNCTION(BlueprintCallable)
	bool IsReady();

	UFUNCTION(BlueprintCallable)
	void RunServer();

	//Client set up

	UFUNCTION(BlueprintCallable)
	void ConnectToServer();

	UFUNCTION(BlueprintCallable)
	void SetControlledMinigame(TEnumAsByte<EGameType> gameType);

	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EGameType> GetControlledMinigame();

#pragma region RPC Functions

	UFUNCTION(Server, Reliable)
	void ServerRPCSetReady();
	void ServerRPCSetReady_Implementation();

	UFUNCTION(Server, Reliable)
	void ServerRPCAssignType(EGameType Type);
	void ServerRPCAssignType_Implementation(EGameType Type);

	UFUNCTION(Client, Reliable)
	void ClientRPCRequestType();
	void ClientRPCRequestType_Implementation();

#pragma endregion
};
