modded class ActionGetInTransport : ActionBase
{
    Logbuddy m_Logbuddy;

    override void OnEndServer(ActionData action_data)
    {
        m_Logbuddy = Logbuddy.GetInstance();
        m_Logbuddy.m_Log.Log("ActionGetInTransport");

        PlayerBase player = PlayerBase.Cast(action_data.m_Player);

        LogbuddyPayload Payload = new LogbuddyPayload();
        Payload.AddPlayer(player, "driver");

        Payload.AddCar(CarScript.Cast(action_data.m_Target.GetObject()), "transport");

        Payload.AddActionItem("player", "driver");
        Payload.AddActionItem("vehicle", "transport");
        Payload.AddActionItem("position", player.GetPosition().ToString());

        m_Logbuddy.Ingest("ActionGetInTransport", Payload);

        super.OnEndServer(action_data);
    }
}