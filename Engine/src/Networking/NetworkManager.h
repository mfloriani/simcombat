#pragma once

#include <memory>

#include "UDPSocket.h"
#include "Constants.h"

namespace engine
{
	struct GameData
	{
		GameData() : enemyUnitType(UnitType::Unknown), enemyUnitQty(0) {}
		UnitType enemyUnitType;
		int enemyUnitQty;
	};

	class PacketBuffer
	{
	public:
		PacketBuffer(int size = 1470) : _size(size), _index(0)
		{ 
			_data = static_cast<char*>(std::malloc(size));
			Reset();
		}
		
		~PacketBuffer() { std::free(_data); }

		void WriteInteger(uint32_t value);
		void WriteShort(uint16_t value);
		void WriteChar(uint8_t value);
		void WriteString(std::string& value);

		uint32_t ReadInteger();
		uint16_t ReadShort();
		uint8_t ReadChar();
		std::string ReadString();

		void Reset() { std::memset(_data, 0, _size); }

		const char* GetDataConst() const { return _data; }
		char* GetData() { return _data; }
		int GetSize() const { return _size; }

	private:
		char* _data;	// data pointer
		int _size;		// buffer size in bytes
		int _index;		// index of the next byte to read/write
	};
	

	class NetworkManager
	{
	public:
		enum class NetworkManagerState
		{
			Unitialized,
			Intro,
			Lobby,
			Starting,
			Ready,
			Playing
		};

		enum PacketType
		{
			PCKT_Unknown,
			PCKT_Join,
			PCKT_NotMasterPeer,
			PCKT_ServerIsFull,
			PCKT_AlreadyJoined,
			PCKT_Welcome,
			PCKT_Ready,
			PCKT_StartGame,
			PCKT_Chat
		};

	public:
		static NetworkManager& Get()
		{
			static NetworkManager instance;
			return instance;
		}

		static bool StaticInitAsMasterPeer(int port, const std::string& name);
		static bool StaticInitAsPeer(const std::string& ip, const std::string& name);

		static void StartGame();
		static void GameHasStarted();
		static void SetPeerReady(UnitType unitType, int unitQty);

		static bool IsReadyToStartGame();

	public:
		~NetworkManager();
		
		void HandleIncomingPackets();
		void SendOutgoingPackets();
		void SendChatMsg(const std::string& msg);

		bool IsMasterPeer() const { return _isMasterPeer; }
		void SetState(NetworkManagerState newState) { _state = newState; }
		NetworkManagerState GetState() { return _state; }
		GameData GetStartGameData() { return _gameData; }
		int GetPlayerId() { return _playerId; }
		const std::string GetName() const { return _name; }

	private:
		NetworkManager() :
			_state(NetworkManagerState::Unitialized),
			_isMasterPeer(false),
			_name("NONAME"),
			_playerId(0),
			_timeLastIntro(0),
			_playerCount(0),
			_nextPlayerId(0),
			_isLobbyReady(false)
		{
			
		}

		NetworkManager(const NetworkManager& other) = delete;
		NetworkManager(NetworkManager&& other) = delete;

		bool InitAsMasterPeer(int port, const std::string& name);
		bool InitAsPeer(const SocketAddress& masterPeerAddr, const std::string& name);
		bool InitSocket(int port);

		int ReceivePacket(PacketBuffer& buffer, SocketAddress& fromAddr);

		int SendPacket(PacketBuffer& buffer, SocketAddress& toAddr);
		void SendJoinPacket();
		void SendReadyPacket(UnitType unitType, int unitQty);
		void SendStartPacket();

		void HandleJoinRequest();
		bool IsGameStarting() const { return _state == NetworkManagerState::Starting; }
		bool IsLobbyReady() const { return _isLobbyReady; }

		void HandleJoinPacket(PacketBuffer& buffer, SocketAddress& fromAddr);
		void HandleWelcomePacket(PacketBuffer& buffer, SocketAddress& fromAddr);
		void HandleReadyPacket(PacketBuffer& buffer, SocketAddress& fromAddr);
		void HandleChatPacket(PacketBuffer& buffer, SocketAddress& fromAddr);
		void HandleStartPacket(PacketBuffer& buffer, SocketAddress& fromAddr);

	private:
		std::shared_ptr<UDPSocket> _socket;
		SocketAddress _masterPeerAddr;
		NetworkManagerState _state;
		bool _isMasterPeer;
		std::string _name;
		int _playerId;
		float _timeLastIntro;
		int _playerCount;
		int _nextPlayerId;
		bool _isLobbyReady;

		GameData _gameData; // TODO: it's a hack... have to think how to do this properly

		std::unordered_map<uint32_t, std::string> _playerNameMap;
		std::unordered_map<uint32_t, SocketAddress> _playerSocketMap;
		std::unordered_map<SocketAddress, uint32_t> _socketPlayerMap;

		ClockTime _clock; // TODO: not sure is a good idea having instances of the clock, but as they are running in separated threads...
	};
}