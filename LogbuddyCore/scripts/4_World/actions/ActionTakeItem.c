/*
modded class ActionTakeItem : ActionInteractBase
{
    LogbuddyCore m_LogbuddyCore;

    override void OnEndServer(ActionData action_data)
    {
        super.OnEndServer(action_data);

        m_LogbuddyCore = LogbuddyCore.GetInstance();

        if (m_LogbuddyCore.m_Settings.IsDebug())
            m_LogbuddyCore.m_Log.Log("ActionTakeItem");

        PlayerBase player = PlayerBase.Cast(action_data.m_Player);

        LogbuddyPayload Payload = new LogbuddyPayload();
        Payload.AddPlayer(player, "taker");

        ItemBase targetItem;
        ItemBase.CastTo(targetItem, action_data.m_Target.GetObject());

        Payload.AddActionItem("player", "taker");
        Payload.AddActionItem("item", targetItem.GetType());
        Payload.AddActionItem("item_id", targetItem.GetID().ToString());
        Payload.AddActionItem("position", player.GetPosition().ToString());

        m_LogbuddyCore.Ingest("ActionTakeItem", Payload);
    }
}
*/