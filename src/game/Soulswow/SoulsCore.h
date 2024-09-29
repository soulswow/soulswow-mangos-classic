#ifndef SOULS_H
#define SOULS_H

#include "Platform/Define.h"

class WorldSession;
class ChatHandler;

class SoulsCore
{
public:
    SoulsCore();
    void Initialize();
    void AddKillToDatabase( uint32 creature_entry );
    void GetKillsByCreatureId( uint32 creature_entry );
    void SendGameChatMessage( std::string msg);
    void SendScreenMessage( std::string msg );
    void OnCreatureDeath( uint32 creatureId );
private:
    uint16 hoggerKills = 0;
    uint16 vancleefKills = 0;
};

#define sSouls MaNGOS::Singleton<SoulsCore>::Instance()
#endif