// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Networking/Public/Networking.h"
#include "NetworkController.generated.h"

class FSocket;

UCLASS()
class PAKOPELI_2020_API ANetworkController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetworkController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Lets keep the socket stuff here for now...
	UFUNCTION(BlueprintCallable)
	void TestServerConnection();

	UFUNCTION(BlueprintCallable)
	void TestClientConnection();

	UFUNCTION(BlueprintCallable)
	void SetClient();

private:



	bool isClient = false;

	FSocket* ListenSocket = nullptr;
	FSocket* ConnectionSocket = nullptr;

	FTimerHandle ListenHandle;
	FTimerHandle ConnectHandle;

	FIPv4Endpoint RemoteAddressForConnection;

	void TCPConnectionListener();
	void TCPSocketListener();

	FString StringFromBinaryArray(const TArray<uint8>& BinaryData);
};
