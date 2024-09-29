#ifndef SOULS_H
#define SOULS_H

#include "Platform/Define.h"

class WorldSession;
class ChatHandler;

enum SoulCreatures{
    HOGGER = 448,
    EDWIN_VANCLEEF = 639,
};

enum SoulTierLevel{
    TIER_0 = 5,
    TIER_1 = 15,
    TIER_2 = 16
};

class SoulsCore
{
public:
    SoulsCore();
    void Initialize();
    void OnCreatureDeath( uint32 creatureId );
    void SendSoulsLoginMessage( Player* player );
private:
    std::string login_message = "";
    
    void InitializeSoulsState();
    void SetSoulsState(SoulTierLevel state);
    void AddKillToDatabase( uint32 creature_entry );
    uint64 GetKillCountByCreatureId( uint32 creature_entry );
    void SendGameChatMessage( std::string msg);
    void SendScreenMessage( std::string msg );

};

#define sSouls MaNGOS::Singleton<SoulsCore>::Instance()
#endif