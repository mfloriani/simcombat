#include "pch.h"
#include "NetworkingSystem.h"
#include "Networking\NetworkManager.h"
#include "Events\Event.h"
#include "Events\EventManager.h"
#include "Events\EngineEvent.h"

using namespace engine;

NetworkingSystem::NetworkingSystem() : System(), _isRunning(true)
{
}

NetworkingSystem::~NetworkingSystem()
{
}

bool NetworkingSystem::Init()
{
    //EventManager::Get().Subscribe(EventType::HostGame, this);
    //EventManager::Get().Subscribe(EventType::JoinGame, this);
    //EventManager::Get().Subscribe(EventType::QuitGame, this);
    //EventManager::Get().Subscribe(EventType::PeerReady, this);

    return false;
}

void NetworkingSystem::OnUpdate(float dt)
{
    auto& networking = NetworkManager::Get();
    networking.HandleIncomingPackets();
    networking.SendOutgoingPackets();
}

void NetworkingSystem::OnEvent(Event* e)
{
    ENGINE_TRACE("NetworkingSystem::OnEvent {0}", e->type);

    switch (e->type)
    {
    case EventType::QuitGame:
        _isRunning = false;
        break;
    //case EventType::HostGame:
    //    networking.StaticInitAsMasterPeer(GAME_PORT, "Host");
    //    break;
    //case EventType::JoinGame:
    //    networking.StaticInitAsPeer("localhost", "Guest");
    //    break;
    //case EventType::PeerReady:
    //    //HandlePeerReadyEvent(e);
    //    break;
    default:
        break;
    }
}

//void NetworkingSystem::HandlePeerReadyEvent(Event* e)
//{
//    auto ev = static_cast<PeerReadyEvent*>(e);
//
//    networking.SetPeerReady(ev->unitType, ev->unitQty);
//
//}
