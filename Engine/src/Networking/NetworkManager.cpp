#include "pch.h"
#include "NetworkManager.h"
#include "SocketUtil.h"
#include "Time.h"
#include "Constants.h"
#include "Events\EventManager.h"
#include "Events\EngineEvent.h"
#include "Engine.h"

#include <sstream>
#include <cassert>

using namespace engine;

NetworkManager::~NetworkManager()
{
}

bool NetworkManager::StaticInitAsMasterPeer(int port, const std::string& name)
{
	return Get().InitAsMasterPeer(port, name);
}

bool NetworkManager::StaticInitAsPeer(const std::string& masterPeerIP, const std::string& name)
{
	auto addr = SocketUtil::CreateIPv4FromString(masterPeerIP);
	if (!addr)
	{
		ENGINE_ERROR("Error creating master peer IP from string {0}", masterPeerIP);
		return false;
	}
	return Get().InitAsPeer(*addr, name);
}

void NetworkManager::StartGame()
{
	if (Get().IsMasterPeer())
	{
		Get().SendStartPacket();
	}
}

void NetworkManager::GameHasStarted()
{
	Get().SetState(NetworkManagerState::Playing);
	//Get().SendChatMsg("I'm going to destroy you");
}

void NetworkManager::SetPeerReady(UnitType unitType, int unitQty)
{
	ENGINE_INFO("I'm ready to start");
	Get().SetState(NetworkManagerState::Ready);
	Get().SendReadyPacket(unitType, unitQty);
}

bool NetworkManager::IsReadyToStartGame()
{
	if (Get().IsMasterPeer())
	{
		return Get().IsLobbyReady();
	}
	else
	{
		return Get().IsGameStarting();
	}
	return false;
}

bool NetworkManager::InitAsMasterPeer(int port, const std::string& name)
{
	if (!InitSocket(port))
		return false;

	_state = NetworkManagerState::Lobby;
	_name = name;
	_isMasterPeer = true;
	_playerCount = 1;
	_playerId = 1;
	_nextPlayerId = _playerId;

	return true;
}

bool NetworkManager::InitAsPeer(const SocketAddress& masterPeerAddr, const std::string& name)
{
	if (!InitSocket(0))
		return false;

	_state = NetworkManagerState::Intro;
	_name = name;
	_masterPeerAddr = masterPeerAddr;

	return true;
}

bool NetworkManager::InitSocket(int port)
{
	SocketAddress myAddr(INADDR_ANY, port);
	
	_socket = std::make_shared<UDPSocket>();
	if (!_socket->Bind(myAddr))
	{
		return false;
	}

	if (!_socket->SetNonBlockingMode(true))
	{
		return false;
	}

	return true;
}

void NetworkManager::HandleIncomingPackets()
{
	//ENGINE_TRACE("NetworkManager::HandleIncomingPackets {0}", _state);

	if (_state == NetworkManagerState::Unitialized)
		return;

	SocketAddress fromAddr;
	PacketBuffer buffer;

	while (true)
	{
		//ENGINE_TRACE("NetworkManager::HandleIncomingPackets -> ReceivePacket()");
		int bytesReceived = ReceivePacket(buffer, fromAddr);
		
		if (bytesReceived > 0)
		{
			uint32_t packetType = buffer.ReadInteger();

			switch (packetType)
			{
			case PCKT_Join:
			{
				ENGINE_WARN("New player request to join");
				HandleJoinPacket(buffer, fromAddr);
				break;
			}
			case PCKT_NotMasterPeer:
			{
				ENGINE_WARN("Oops, not master peer");
				break;
			}
			case PCKT_ServerIsFull:
			{
				ENGINE_WARN("Oops, the server is full");
				break;
			}
			case PCKT_Welcome:
			{
				HandleWelcomePacket(buffer, fromAddr);
				break;
			}
			case PCKT_AlreadyJoined:
			{
				ENGINE_WARN("Oops, I'm already in the game");
				break;
			}
			case PCKT_Ready:
			{
				HandleReadyPacket(buffer, fromAddr);
				break;
			}
			case PCKT_StartGame:
			{
				HandleStartPacket(buffer, fromAddr);
				break;
			}
			case PCKT_Chat:
			{
				HandleChatPacket(buffer, fromAddr);
				break;
			}
			default:
				break;
			}
		}
		else if (bytesReceived == 0)
		{
			break; // nothing to read
		}
		else 
		{
			if (bytesReceived == -WSAECONNRESET)
			{
				ENGINE_WARN("Connection with the other peer was lost");
			}
			break;
		}
	}
}

int NetworkManager::ReceivePacket(PacketBuffer& buffer, SocketAddress& fromAddr)
{
	return _socket->Receive(buffer.GetData(), buffer.GetSize(), fromAddr);
}

int NetworkManager::SendPacket(PacketBuffer& buffer, SocketAddress& toAddr)
{
	return _socket->Send(buffer.GetDataConst(), buffer.GetSize(), toAddr);
}

void NetworkManager::HandleReadyPacket(PacketBuffer& buffer, SocketAddress& fromAddr)
{
	if (_isMasterPeer)
	{
		ENGINE_INFO("Peer said that it is ready");
		_isLobbyReady = true; // received ok from peer
	}
	else
	{
		ENGINE_INFO("Master said that it is ready");
	}

	_gameData.enemyUnitType = static_cast<UnitType>(buffer.ReadInteger());
	_gameData.enemyUnitQty = buffer.ReadInteger();
}

void NetworkManager::HandleChatPacket(PacketBuffer& buffer, SocketAddress& fromAddr)
{
	std::string msg = buffer.ReadString();
	ENGINE_TRACE("Chat: {0}", msg);
}

void NetworkManager::HandleStartPacket(PacketBuffer& buffer, SocketAddress& fromAddr)
{
	ENGINE_TRACE("Master said that the game is starting");
	_state = NetworkManagerState::Starting;

	auto ev = std::make_shared<StartGameEvent>();
	Engine::Get().BroadcastEvent(ev);
}

void NetworkManager::HandleJoinPacket(PacketBuffer& buffer, SocketAddress& fromAddr)
{
	if (_socketPlayerMap.find(fromAddr) != _socketPlayerMap.end()) // avoid adding same player again
	{
		ENGINE_WARN("{0} already joined the game", fromAddr.GetIPv4());
		PacketBuffer outBuffer;
		outBuffer.WriteInteger(PCKT_AlreadyJoined);
		SendPacket(outBuffer, fromAddr);
		return;
	}
	
	if (_playerCount >= MaxPlayers)
	{
		ENGINE_WARN("Server is full");
		PacketBuffer outBuffer;
		outBuffer.WriteInteger(PCKT_ServerIsFull);
		SendPacket(outBuffer, fromAddr);
		return;
	}
	
	if (!_isMasterPeer)
	{
		ENGINE_WARN("I am not the Master Peer");
		PacketBuffer outBuffer;
		outBuffer.WriteInteger(PCKT_NotMasterPeer);
		SendPacket(outBuffer, fromAddr);
		return;
	}

	std::string fromName = buffer.ReadString();
	
	++_playerCount;
	++_nextPlayerId;
	
	// register the new player
	_playerNameMap.emplace(_nextPlayerId, fromName);
	_playerSocketMap.emplace(_nextPlayerId, fromAddr);
	_socketPlayerMap.emplace(fromAddr, _nextPlayerId);

	PacketBuffer outBuffer;
	outBuffer.WriteInteger(PCKT_Welcome);
	outBuffer.WriteInteger(_playerId); // master peer id
	outBuffer.WriteString(_name); // master peer name
	outBuffer.WriteInteger(_nextPlayerId); // new player id

	SendPacket(outBuffer, fromAddr);
	
	ENGINE_TRACE("Welcoming peer [id: {0}, name: {1}]", _nextPlayerId, fromName);
}

void NetworkManager::HandleWelcomePacket(PacketBuffer& buffer, SocketAddress& fromAddr)
{
	if (!_isMasterPeer)
	{
		ENGINE_TRACE("I joined the server!");
		int masterPlayerId = buffer.ReadInteger(); // master peer id
		std::string masterPlayerName = buffer.ReadString(); // master peer name

		// register the master peer 
		_playerNameMap.emplace(masterPlayerId, masterPlayerName);
		_playerSocketMap.emplace(masterPlayerId, fromAddr);
		_socketPlayerMap.emplace(fromAddr, masterPlayerId);

		_playerId = buffer.ReadInteger(); // my player id
		_nextPlayerId = _playerId;
		_playerCount = _playerId;

		_state = NetworkManagerState::Lobby;
	}
}

void NetworkManager::SendOutgoingPackets()
{
	if (_state == NetworkManagerState::Unitialized)
		return;

	switch (_state)
	{
	case NetworkManagerState::Intro:
		HandleJoinRequest();
		break;
	case NetworkManagerState::Starting:
		break;
	case NetworkManagerState::Playing:
		
		break;
	default:
		break;
	}
}

void NetworkManager::SendChatMsg(const std::string& msg)
{
	PacketBuffer buffer;
	buffer.WriteInteger(PCKT_Chat);
	buffer.WriteString(std::string(msg));

	if (IsMasterPeer())
	{
		for (auto& peer : _playerSocketMap)
		{
			SendPacket(buffer, peer.second);
		}
	}
	else
	{
		SendPacket(buffer, _masterPeerAddr);
	}
}

void NetworkManager::SendReadyPacket(UnitType unitType, int unitQty)
{
	PacketBuffer buffer;
	buffer.WriteInteger(PCKT_Ready);
	//buffer.WriteString(_name);
	buffer.WriteInteger(static_cast<int>(unitType));
	buffer.WriteInteger(unitQty);

	if (Get().IsMasterPeer())
	{
		for (auto& peer : _playerSocketMap)
		{
			ENGINE_INFO("Sending ready packet to peer {0}", peer.first);
			SendPacket(buffer, peer.second);
		}
	}
	else // peer has to send to master
	{
		ENGINE_INFO("Sending ready packet to master");
		SendPacket(buffer, _masterPeerAddr);
	}
}

// only master can start the game
void NetworkManager::SendStartPacket()
{
	PacketBuffer buffer;
	buffer.WriteInteger(PCKT_StartGame);
	for (auto& peer : _playerSocketMap)
	{
		ENGINE_INFO("Sending start packet to peer {0}", peer.first);
		SendPacket(buffer, peer.second);
	}

	_state = NetworkManagerState::Starting;
	ENGINE_TRACE("Game is starting");
}



void NetworkManager::HandleJoinRequest()
{
	float time = _clock.GetTimeAsFloat();
	if (time > _timeLastIntro + TimeBetweenIntros)
	{
		SendJoinPacket();
		_timeLastIntro = time;
	}
}

void NetworkManager::SendJoinPacket()
{
	ENGINE_TRACE("[{0}] Sending join packet", _name);

	PacketBuffer buffer;
	buffer.WriteInteger(PCKT_Join);
	buffer.WriteString(_name);

	SendPacket(buffer, _masterPeerAddr);
}


//
// Packet Buffer
//

void PacketBuffer::WriteInteger(uint32_t value)
{
	assert((_index + sizeof uint32_t) <= _size || "Buffer overflow for integer value");

	*((uint32_t*)(_data + _index)) = value;
	_index += sizeof uint32_t;
}

void PacketBuffer::WriteShort(uint16_t value)
{
	assert((_index + sizeof uint16_t) <= _size || "Buffer overflow for short value");

	*((uint16_t*)(_data + _index)) = value;
	_index += sizeof uint16_t;
}

void PacketBuffer::WriteChar(uint8_t value)
{
	assert((_index + sizeof uint8_t) <= _size || "Buffer overflow for char value");

	*((uint8_t*)(_data + _index)) = value;
	_index += sizeof uint8_t;
}

void PacketBuffer::WriteString(std::string& value)
{
	assert((_index + value.size() + sizeof uint32_t) <= _size || "Buffer overflow for string value");

	WriteInteger(static_cast<uint32_t>(value.size()));
	for (const auto& c : value)
	{
		WriteChar(c);
	}
}

uint32_t PacketBuffer::ReadInteger()
{
	assert((_index + sizeof uint32_t) <= _size || "Buffer overflow for integer value");

	uint32_t value = *((uint32_t*)(_data + _index));
	_index += sizeof uint32_t;

	return value;
}

uint16_t PacketBuffer::ReadShort()
{
	assert((_index + sizeof uint16_t) <= _size || "Buffer overflow for short value");

	uint16_t value = *((uint16_t*)(_data + _index));
	_index += sizeof uint16_t;

	return value;
}

uint8_t PacketBuffer::ReadChar()
{
	assert((_index + sizeof uint8_t) <= _size || "Buffer overflow for char value");

	uint8_t value = *((uint8_t*)(_data + _index));
	_index += sizeof uint8_t;

	return value;
}

std::string PacketBuffer::ReadString()
{
	assert((_index + sizeof uint32_t) <= _size || "Buffer overflow for string length");

	uint32_t length = ReadInteger();

	assert((_index + length + sizeof uint32_t) <= _size || "Buffer overflow for string value");

	std::stringstream name;
	for (uint32_t i = 0; i < length; ++i)
	{
		name << ReadChar();
	}

	return name.str();
}


