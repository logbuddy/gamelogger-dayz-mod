modded class TraderMessage
{
    protected static bool Filter(string str)
    {
        if (str.Substring(0, 18) == "[TRADER] Player: (") return false;
        return true;
    }

    override static void ServerLog(string str)
    {
        Print(str); // should still go to the regular log
        if(TraderMessage.Filter(str)) return;

        m_LogbuddyCore = LogbuddyCore.GetInstance();
        m_LogbuddyCore.m_Log.Log("ActionGetInTransport");

        PlayerBase player = PlayerBase.Cast(action_data.m_Player);

        LogbuddyPayload Payload = new LogbuddyPayload();
        Payload.AddPlayer(player, "driver");

        Payload.AddCar(CarScript.Cast(action_data.m_Target.GetObject()), "transport");

        Payload.AddActionItem("player", "driver");
        Payload.AddActionItem("vehicle", "transport");
        Payload.AddActionItem("position", player.GetPosition().ToString());

        m_LogbuddyCore.Ingest("ActionGetInTransport", Payload);






        LoggerJsonApiStruct json = new LoggerJsonApiStruct();
        json.StoreString("message", str);

        Logger l = LoggerCore.GetInstance();
        l.Ingest("Trader", json);
    }
}
