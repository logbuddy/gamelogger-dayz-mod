modded class ActionGetInTransport : ActionBase
{
    Logger m_Logger;

    override void OnEndServer(ActionData action_data)
    {
        m_Logger = Logger.GetInstance();
        m_Logger.m_Log.Log("ActionGetInTransport");

        LoggerPayload Payload = new LoggerPayload();
        Payload.AddPlayer(PlayerBase.Cast(action_data.m_Player), "driver");

        Payload.AddCar(CarScript.Cast(action_data.m_Target.GetObject()), "transport");

        Payload.AddActionItem("player", "driver");
        Payload.AddActionItem("vehicle", "transport");

        m_Logger.Ingest("ActionGetInTransport", Payload);

        super.OnEndServer(action_data);
    }
}