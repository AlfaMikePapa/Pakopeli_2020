// Fill out your copyright notice in the Description page of Project Settings.


#include "MinigameState.h"
#include "Engine/World.h"
#include "Runtime/Sockets/Public/IPAddress.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"


void AMinigameState::ServerRPCSetControllerReference_Implementation(APlayerController* Controller)
{
	if(HasAuthority())
	{
		if(Controller)
		{
			ClientController = Controller;
		}
	}
}

void AMinigameState::ServerRPCClientConnected_Implementation()
{
	if(HasAuthority())
	{
		ClientConnected.Broadcast();
	}
}

void AMinigameState::ServerRPCClientReadyUp_Implementation()
{
	IsReadyToStart = true;

	if(HasAuthority())
	{
		ClientReadyUp.Broadcast();
	}
}

void AMinigameState::ServerRPCStartMinigame_Implementation()
{
	if(HasAuthority())
	{
		if(!IsReadyToStart)
		{
			StartMinigame();
		}
	}
}

void AMinigameState::ServerRPCTaskComplete_Implementation(float CompletionRate)
{	
	if(HasAuthority())
	{
		TaskCompletionRate = CompletionRate;

		if(TaskCompletionRate >= 1)
		{
			TaskComplete.Broadcast();
		}
		else
		{
			TaskStatusUpdate.Broadcast();
		}
		
	}
}

void AMinigameState::ServerRPCInboundSettings_Implementation(EGameType DesiredType)
{
	if(HasAuthority())
	{
		ControlledGame = DesiredType;
		SetupComplete.Broadcast();
	}
}

void AMinigameState::ClientRPCLoadSettings_Implementation()
{
	//UE_LOG(LogTemp, Error, TEXT("Load Settings"));
	//if(!HasAuthority())
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Like Really!"));
	//	LoadSettings.Broadcast();
	//}
}

void AMinigameState::ClientRPCInitializeHUD_Implementation()
{
	if(!HasAuthority())
	{
		InitializeClientHUD.Broadcast();
	}
}

const FString AMinigameState::GetNetworkURL(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (UWorld* World = WorldContextObject->GetWorld())
		{
			bool bCanBind = false;
			TSharedRef<FInternetAddr> LocalIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBind);
			return (LocalIp->IsValid() ? LocalIp->ToString(false) : World->URL.Host);
		}
	}
	return "WorldContextObject is FALSE";
}

void AMinigameState::StartMinigame()
{
	//Should never be called as client but lets make sure.
	if(!HasAuthority())
	{
		return;
	}

	switch(ControlledGame)
	{
		case EGameType::Minigame1:
		{
			UE_LOG(LogTemp, Error, TEXT("Starting minigame 1!"));
			break;
		}
		default:
			break;
	}
}
