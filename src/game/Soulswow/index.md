# as much logic as possible should be stored away in souls_wow class

# currently the only external hook is in creature.cpp Creature::SetDeathState

        sSouls.OnDeath()

# .notify command shows a text to all players in the middle of the screen

# sends message in red [EventMessage]: message_content style
    sWorld.SendWorldText(LANG_EVENTMESSAGE, msg.c_str());

# sLog.outString("     Running with Souls WoW - Mod      ");