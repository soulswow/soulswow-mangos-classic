#include "Soulswow/SoulsCore.h"
#include "World/World.h"
#include "Server/WorldSession.h"
#include "Entities/Player.h"
#include "Chat/Chat.h"
#include "Tools/Language.h"
#include "Server/WorldPacket.h"

#include <format>
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

    InitializeSoulsState();
}

void SoulsCore::AddKillToDatabase( uint32 creature_entry )
{
    WorldDatabase.BeginTransaction();
    WorldDatabase.PExecute("INSERT INTO souls_kills VALUES(0,%u)", creature_entry);
    WorldDatabase.CommitTransaction();
}

uint64 SoulsCore::GetKillCountByCreatureId( uint32 creature_entry )
{
    auto result = WorldDatabase.PQuery("SELECT * FROM souls_kills WHERE entry=%s", std::to_string(creature_entry).c_str());
    if( result )
    {
        return result->GetRowCount();
    }

    return 0;
}

void SoulsCore::InitializeSoulsState()
{
    auto vancleef = GetKillCountByCreatureId(EDWIN_VANCLEEF);
    if(vancleef > 0)
    {
        SetSoulsState(TIER_2);
        return;
    }

    auto hogger = GetKillCountByCreatureId(HOGGER);
    if(hogger > 0)
    {
        SetSoulsState(TIER_1);
        return;
    }

    SetSoulsState(TIER_0);
    return;
}

void SoulsCore::SetSoulsState( SoulTierLevel state )
{
    sWorld.setConfig(CONFIG_UINT32_MAX_PLAYER_LEVEL, state);
    login_message = "Welcome to Souls WoW: Current level cap is " + std::to_string(state);
}

void SoulsCore::SendSoulsLoginMessage( Player* player )
{
    ChatHandler(player).PSendSysMessage("%s", login_message.c_str());
}

void SoulsCore::SendScreenMessage( std::string msg )
{
    WorldPacket data(SMSG_NOTIFICATION, (msg.size() + 1));
    data << msg;
    sWorld.SendGlobalMessage(data);
}

void SoulsCore::SendGameChatMessage( std::string msg )
{
    auto sessionMap = sWorld.GetSessionMap();
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
    
    if(creatureId == HOGGER)
    {
        if(GetKillCountByCreatureId(HOGGER) == 0)
        {
            AddKillToDatabase(HOGGER);
            SendScreenMessage("A weight lifts from your shoulder...");
            SendGameChatMessage("Souls WoW: Level cap raised to " + std::to_string(TIER_1));
            SetSoulsState(TIER_1);
        }
    }
    else if(creatureId == EDWIN_VANCLEEF)
    {
        if(GetKillCountByCreatureId(EDWIN_VANCLEEF) == 0)
        {
            AddKillToDatabase(EDWIN_VANCLEEF);
            SendScreenMessage("A weight lifts from your shoulder...");
            SendGameChatMessage("Souls WoW: Level cap raised to " + std::to_string(TIER_2));
            SetSoulsState(TIER_2);
        }
    }
    else
    {
        //SoulsPrint("Normal kill");
    }
}