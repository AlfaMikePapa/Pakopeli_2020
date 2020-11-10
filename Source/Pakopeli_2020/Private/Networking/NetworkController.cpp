// Fill out your copyright notice in the Description page of Project Settings.



#include "Pakopeli_2020/Public/Networking/NetworkController.h"
#include "Networking/Public/Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "TimerManager.h"
#include "Networking/Public/Interfaces/IPv4/IPv4Address.h"
#include "Kismet/GameplayStatics.h"
#include "engine/World.h"
#include <string>

// Sets default values
ANetworkController::ANetworkController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANetworkController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANetworkController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

void ANetworkController::TestServerConnection()
{
	FSocket* Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	FIPv4Address ip(82, 128, 228, 104);
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(3000);

	bool connected = Socket->Connect(*addr);

	if (connected)
	{
		UE_LOG(LogTemp, Warning, TEXT("Connection succesfull"));

		UGameplayStatics::OpenLevel(GetWorld(), FName("Game3_Level"));

		FString toSend = "Minigame3 connected";

		

		//FArchive archiveToSend;

	/*	toSend.SerializeAsANSICharArray(archiveToSend);*/

		//uint8* buffer{};

		//FString::ToBlob(toSend, buffer, toSend.Len() / 2);

		//int32 sent = 0;
		//bool success = Socket->Send(buffer, toSend.Len() / 2, sent);
		//if (success)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("Packet SENT"));
		//}
		//else
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("Sending Failed"));
		//}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Connection failed!"));
	}

}

void ANetworkController::TestClientConnection()
{
	if (ListenSocket != nullptr)
	{
		return;
	}

	FString socketName("Server");
	/*FIPv4Endpoint Endpoint(FIPv4Address(127, 0, 0, 1), 3000);*/
	FIPv4Endpoint Endpoint(FIPv4Address(0, 0, 0, 0), 3000);
	
	ListenSocket = FTcpSocketBuilder(*socketName)
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.Listening(4);

	

	//Start listener
	GetWorldTimerManager().SetTimer(ListenHandle, this, &ANetworkController::TCPConnectionListener, 1.f, true, -1.0f);
}



void ANetworkController::SetClient()
{
	isClient = true;
}

void ANetworkController::TCPConnectionListener()
{
	if (!ListenSocket)
	{
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("Listening..."));

	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool Pending;

	//incomming connections
	if (ListenSocket->HasPendingConnection(Pending) && Pending)
	{
		if (ConnectionSocket)
		{
			ConnectionSocket->Close();
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
		}

		ConnectionSocket = ListenSocket->Accept(*RemoteAddress, TEXT("Received Socket Connection"));

		if (ConnectionSocket != NULL)
		{
			//Global cache of current Remote Address
			RemoteAddressForConnection = FIPv4Endpoint(RemoteAddress);

			UE_LOG(LogTemp, Error, TEXT("Accepted Connection!"));

			ListenSocket->Close();
			ListenSocket = nullptr;

			GetWorldTimerManager().SetTimer(ConnectHandle, this, &ANetworkController::TCPSocketListener, 0.01f, true);
		}
	}
}

void ANetworkController::TCPSocketListener()
{
	if (!ConnectionSocket)
	{
		return;
	}

	if (ConnectionSocket->GetConnectionState() == ESocketConnectionState::SCS_NotConnected)
	{
		UE_LOG(LogTemp, Warning, TEXT("closing..."));
		ConnectionSocket->Close();
	}

	if (ConnectionSocket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
	{
		UE_LOG(LogTemp, Warning, TEXT("waiting DATA!"));
		TArray<uint8> ReceivedBinaryData;

		uint32 Size;
		while (ConnectionSocket->HasPendingData(Size))
		{
			UE_LOG(LogTemp, Warning, TEXT("Has DATA!"));
			ReceivedBinaryData.Init(Size, 65507u);

			int32 Read = 0;
			ConnectionSocket->Recv(ReceivedBinaryData.GetData(), ReceivedBinaryData.Num(), Read);
		}

		if (ReceivedBinaryData.Num() <= 0)
		{
			return;
		}
	}
}

FString ANetworkController::StringFromBinaryArray(const TArray<uint8>& BinaryData)
{
	const std::string cstr(reinterpret_cast<const char*>(BinaryData.GetData()), BinaryData.Num());
	return FString(cstr.c_str());
}

