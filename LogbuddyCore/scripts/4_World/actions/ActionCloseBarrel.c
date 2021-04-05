/*
modded class ActionCloseBarrel : ActionInteractBase
{
    override void OnEndServer(ActionData action_data)
    {
        super.OnEndServer(action_data);

        Logger logger = Logger.GetInstance();
        logger.Log("ActionCloseBarrel");
        LoggerJsonApiStruct json = new LoggerJsonApiStruct();

        string playerId = "unknown";
        string playerName = "unknown";
        vector playerPosition;

        PlayerBase player = PlayerBase.Cast(action_data.m_Player);
        if (player)
        {
            playerId = player.GetIdentity().GetPlainId();
            playerName = player.GetIdentity().GetName();
            playerPosition = player.GetPosition();
        }

        json.StoreString("playerId", playerId);
        json.StoreString("playerName", playerName);
        json.StoreVector("playerPosition", playerPosition);

        logger.Ingest("ActionCloseBarrel", json);
    }
}
*/