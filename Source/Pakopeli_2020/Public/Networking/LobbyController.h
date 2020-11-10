// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LobbyController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLobbyEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PAKOPELI_2020_API ULobbyController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULobbyController();

	UPROPERTY(BlueprintAssignable)
	FLobbyEvent InitializeHUD;

	UFUNCTION(BlueprintCallable, category = "Connection")
	void ConnectToServer(FString IP);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
