/*
modded class ActionGetInTransport : ActionBase
{
    LogbuddyCore m_LogbuddyCore;

    
     override void OnEndServer(ActionData action_data)
    {
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

        super.OnEndServer(action_data);
    } 
}
*/