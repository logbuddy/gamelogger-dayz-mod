modded class ActionGetInTransport : ActionBase
{
    override void OnEndServer(ActionData action_data)
    {
        super.OnEndServer(action_data);

        Logger logger = Logger.GetInstance();
        logger.Log("ActionGetInTransport");

        LoggerPayload loggerPayload = new LoggerPayload();

        string playerId = "unknown";
        string playerName = "unknown";
        vector playerPosition;

        PlayerBase player = PlayerBase.Cast(action_data.m_Player);
        if(player)
        {
            playerId = player.GetIdentity().GetPlainId();
            playerName = player.GetIdentity().GetName();
            playerPosition = player.GetPosition();
        }

        string carName = "unknown";

        CarScript car = CarScript.Cast(action_data.m_Target.GetObject());
        if (car)
        {
            carName = car.ClassName();
        }

        loggerPayload.AddString("playerId", playerId);
        loggerPayload.AddString("playerName", playerName);
        loggerPayload.AddString("carName", carName);
        loggerPayload.AddString("playerPosition", playerPosition.ToString());

        logger.Ingest("ActionGetInTransport", loggerPayload);
    }
}