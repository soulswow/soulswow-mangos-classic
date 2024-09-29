#include "Soulswow/SoulsCore.h"
#include "World/World.h"
#include "Server/WorldSession.h"
#include "Entities/Player.h"
#include "Chat/Chat.h"
#include "Tools/Language.h"
#include "Server/WorldPacket.h"

#include <string>
#include <iostream>

SoulsCore::SoulsCore(){}

void SoulsCore::Initialize()
{
    WorldDatabase.BeginTransaction();

    auto result = WorldDatabase.Query("SHOW TABLES LIKE 'souls_kills'");
    if( !result )
    {
        sLog.outString("Souls WoW - SQL: CREATING TABLE");
        WorldDatabase.Query("CREATE TABLE souls_kills(id INT PRIMARY KEY AUTO_INCREMENT, entry INT)");
    }

    WorldDatabase.CommitTransaction();
    sLog.outString("Souls WoW - SQL: INITIALIZED");
}

void SoulsCore::AddKillToDatabase( uint32 creature_entry )
{
    WorldDatabase.BeginTransaction();
    WorldDatabase.PExecute("INSERT INTO souls_kills VALUES(0,%u)", creature_entry);
    WorldDatabase.CommitTransaction();
}

void SoulsCore::SendScreenMessage( std::string msg ){
    WorldPacket data(SMSG_NOTIFICATION, (msg.size() + 1));
    data << msg;
    sWorld.SendGlobalMessage(data);
}

void SoulsCore::SendGameChatMessage( std::string msg )
{
    std::unordered_map<uint32, WorldSession*> sessionMap = sWorld.GetSessionMap();
    for(std::unordered_map<uint32, WorldSession*>::const_iterator itr = sessionMap.begin(); itr != sessionMap.end(); itr++)
    {
        if (WorldSession* session = itr->second)
        {
            Player* player = session->GetPlayer();
            ChatHandler(player).PSendSysMessage("%s", msg.c_str());
        }
    }
}

void SoulsCore::OnCreatureDeath( uint32 creatureId )
{
    
    if(creatureId == 448) //HOGGER
    {
        if(hoggerKills == 0)
        {
            SendScreenMessage("A weight lifts from your shoulder...");
            SendGameChatMessage("Souls WoW: Level cap raised to 15");
            sWorld.SetMotd("Souls WoW: Level cap 15"); //doesn't save
        }
        hoggerKills++;
        AddKillToDatabase(creatureId);
        std::string msg = "Hogger slain: " + std::to_string(hoggerKills);
        SendGameChatMessage(msg);
    }
    else if(creatureId == 639) //EDWIN VANCLEEF
    {
        if(vancleefKills == 0)
        {
            SendScreenMessage("A weight lifts from your shoulder...");
            SendGameChatMessage("Souls WoW: Level cap raised to 21");
            sWorld.SetMotd("Souls WoW: Level cap 21"); //doesn't save
        }
        vancleefKills++;
        std::string msg = "Edwin VanCleef slain: " + std::to_string(vancleefKills);
        SendGameChatMessage(msg);
    }
    else
    {
        //SoulsPrint("Normal kill");
    }
}