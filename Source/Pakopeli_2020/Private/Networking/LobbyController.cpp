// Fill out your copyright notice in the Description page of Project Settings.


#include "Pakopeli_2020/Public/Networking/LobbyController.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Sets default values for this component's properties
ULobbyController::ULobbyController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void ULobbyController::ConnectToServer(FString IP)
{
#if !GAMESERVER

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		FString Command = "open ";
		Command.Append(IP);

		//Controller->ConsoleCommand("open 127.0.0.1");
		Controller->ConsoleCommand(Command);
	}

#endif
}

// Called when the game starts
void ULobbyController::BeginPlay()
{
	Super::BeginPlay();

	// ...

#if !GAMESERVER

	InitializeHUD.Broadcast();

#endif

#if GAMESERVER

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		Controller->ConsoleCommand("open Main_Level?listen");
	}

#endif
	
}


// Called every frame
void ULobbyController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

