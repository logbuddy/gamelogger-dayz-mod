/*
modded class ActionStartEngine: ActionContinuousBase
{
    LogbuddyCore m_LogbuddyCore;

    override void OnEndServer(ActionData action_data)
    {
        super.OnEndServer(action_data);

        m_LogbuddyCore = LogbuddyCore.GetInstance();

        if (m_LogbuddyCore.m_Settings.IsDebug())
            m_LogbuddyCore.m_Log.Log("ActionStartEngine");

        LogbuddyPayload Payload = new LogbuddyPayload();

        PlayerBase player = PlayerBase.Cast(action_data.m_Player);
        Payload.AddPlayer(player, "driver");

        HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
        if (!vehCommand)
            return;

        CarScript car;
        if (!Class.CastTo(car, vehCommand.GetTransport()))
            return

        Payload.AddCar(car, "transport");
        
        Payload.AddActionItem("vehicle", "transport");
        Payload.AddActionItem("position", player.GetPosition().ToString());

        m_LogbuddyCore.Ingest("ActionStartEngine", Payload);
    }
}
*/