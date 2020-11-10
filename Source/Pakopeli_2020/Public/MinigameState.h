// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NetworkControllerComponent.h"
#include "MinigameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerStateEvent);

UCLASS()
class PAKOPELI_2020_API AMinigameState : public APlayerState
{
	GENERATED_BODY()
	
private:

	bool IsConnected = false;

public:

	UPROPERTY(BlueprintReadOnly)
	bool IsReadyToStart = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsStarted = false;

	UPROPERTY(BlueprintReadOnly)
	float TaskCompletionRate = 0.f;

	//Server RPC (Called on client, executed on server)

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCSetControllerReference(APlayerController* Controller);
	void ServerRPCSetControllerReference_Implementation(APlayerController* Controller);
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCClientConnected();
	void ServerRPCClientConnected_Implementation();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCClientReadyUp();
	void ServerRPCClientReadyUp_Implementation();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCStartMinigame();
	void ServerRPCStartMinigame_Implementation();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCTaskComplete(float CompletionRate);
	void ServerRPCTaskComplete_Implementation(float CompletionRate);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCInboundSettings(EGameType DesiredType);
	void ServerRPCInboundSettings_Implementation(EGameType DesiredType);

	//Client RPC (Called on server, executed on client)

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientRPCLoadSettings();
	void ClientRPCLoadSettings_Implementation();
	
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientRPCInitializeHUD();
	void ClientRPCInitializeHUD_Implementation();

	//Broadcast events

	UPROPERTY(EditAnywhere, Category = "Class references")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(BlueprintAssignable)
	FPlayerStateEvent ClientConnected;

	UPROPERTY(BlueprintAssignable)
	FPlayerStateEvent ClientReadyUp;

	UPROPERTY(BlueprintAssignable)
	FPlayerStateEvent TaskComplete;

	UPROPERTY(BlueprintAssignable)
	FPlayerStateEvent TaskStatusUpdate;

	UPROPERTY(BlueprintAssignable)
	FPlayerStateEvent InitializeClientHUD;

	UPROPERTY(BlueprintAssignable)
	FPlayerStateEvent LoadSettings;

	UPROPERTY(BlueprintAssignable)
	FPlayerStateEvent SetupComplete;

	// Starts for minigame fields
	UPROPERTY(EditAnywhere)
	AActor* Game1Start = nullptr;
	
	UPROPERTY(EditAnywhere)
	AActor* Game2Start = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* Game3Start = nullptr;

	// minigame field objects
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Game1Board;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EGameType> ControlledGame = EGameType::None;

	UPROPERTY(BlueprintReadOnly)
	APlayerController* ClientController = nullptr;

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static const FString GetNetworkURL(UObject* WorldContextObject);

private:

	void StartMinigame();
};
