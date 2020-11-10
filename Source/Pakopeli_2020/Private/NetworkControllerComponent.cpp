// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkControllerComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UNetworkControllerComponent::UNetworkControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

#if GAMESERVER
	
	ControlledGame = EGameType::GameServer;

#endif
}


// Called when the game starts
void UNetworkControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

#if GAMESERVER
	InitializeHUD.Broadcast();
#endif
	
#if !GAMESERVER
	ClientRPCRequestType();
#endif
	
}


// Called every frame
void UNetworkControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNetworkControllerComponent::ClientReadyUp()
{
	UE_LOG(LogTemp, Error, TEXT("Ready up!"));
	Ready = true;
	ServerRPCSetReady();
}

bool UNetworkControllerComponent::IsReady()
{
	return Ready;
}

void UNetworkControllerComponent::RunServer()
{
#if GAMESERVER

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		Controller->ConsoleCommand("open Main_Level?listen");
	}

#endif
}

void UNetworkControllerComponent::ConnectToServer()
{
#if !GAMESERVER

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		Controller->ConsoleCommand("open 127.0.0.1");
	}

#endif
}

void UNetworkControllerComponent::SetControlledMinigame(TEnumAsByte<EGameType> gameType)
{
	ControlledGame = gameType;
}

TEnumAsByte<EGameType> UNetworkControllerComponent::GetControlledMinigame()
{
	return ControlledGame;
}

void UNetworkControllerComponent::ServerRPCSetReady_Implementation()
{
	Ready = true;

	if (GetOwner()->HasAuthority())
	{
		CheckReadyStatus.Broadcast();
		UE_LOG(LogTemp, Error, TEXT("And now the server updates the huditything"));
	}

}

void UNetworkControllerComponent::ServerRPCAssignType_Implementation(EGameType Type)
{
	if(GetOwner()->HasAuthority())
	{
		
		ControlledGame = Type;

		UE_LOG(LogTemp, Error, TEXT("RPC received! %d"), ControlledGame.GetValue());
	}
}

void UNetworkControllerComponent::ClientRPCRequestType_Implementation()
{
	if (!GetOwner()->HasAuthority())
	{
#if !GAMESERVER
//		UE_LOG(LogTemp, Error, TEXT("RPC Request sent!"));
//
//#if MINIGAME1
//		ControlledGame = EGameType::Minigame1;
//#endif
//
//#if MINIGAME2
//		ControlledGame = EGameType::Minigame2;
//#endif
//
//#if MINIGAME3
//		ControlledGame = EGameType::Minigame3;
//#endif

		InitializeHUD.Broadcast();
		
		/*ServerRPCAssignType(ControlledGame);*/

#endif
	}
	else
	{
#if !GAMESERVER
		//UE_LOG(LogTemp, Error, TEXT("RPC Request sent!"));

		//ControlledGame = EGameType::Minigame3;

		/*ServerRPCAssignType(ControlledGame);*/

#endif
	}
}

