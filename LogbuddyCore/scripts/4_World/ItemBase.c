modded class ItemBase extends InventoryItem {

    override void EEItemLocationChanged (notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc) {

		EntityAI old_owner = oldLoc.GetParent();
		EntityAI new_owner = newLoc.GetParent();

        PlayerBase player;

        if(new_owner)
            Class.CastTo(player, new_owner.GetHierarchyRootPlayer());

        string InventoryLocationTypeNames[6] = {
            "UNKNOWN",
            "GROUND",
            "ATTACHMENT",
            "CARGO",
            "HANDS",
            "PROXYCARGO"
        };

        if(player && (newLoc.GetType()==InventoryLocationType.ATTACHMENT || newLoc.GetType()==InventoryLocationType.CARGO || newLoc.GetType()==InventoryLocationType.HANDS)) {

            string item_name = this.GetType();
            int item_id = this.GetID();

            string old_environment = InventoryLocationTypeNames[oldLoc.GetType()];
            string new_environment = InventoryLocationTypeNames[newLoc.GetType()];
        
            if(oldLoc.GetType() != InventoryLocationType.UNKNOWN) {
                   
                LogbuddyCore logbuddyCore = LogbuddyCore.GetInstance();
                if(logbuddyCore.m_Settings.GetActive() == 1)
                {
                    string player_log = player.GetIdentity().GetName() + " (" + player.GetIdentity().GetPlainId() + ")";
                    string item_log = " took " + item_name + " (" + item_id + ")";
                    string environment_log = " " + old_environment + ">" + new_environment;
                    string location_log = " " + this.GetPosition();
                    logbuddyCore.m_Log.Log(player_log + item_log + environment_log + location_log, LogbuddyLogger.LOGLEVEL_DEBUG);

                    LogbuddyPayload Payload = new LogbuddyPayload();
                    
                    Payload.AddPlayer(player, "taker");
                    Payload.AddActionItem("player", "taker");
                    Payload.AddActionItem("item", item_name);
                    Payload.AddActionItem("item_id", item_id.ToString());
                    Payload.AddActionItem("position", this.GetPosition().ToString());

                    logbuddyCore.Ingest("item-logs", "ActionTakeItem", Payload);
                }
            }
        }

        super.EEItemLocationChanged (oldLoc, newLoc);

    }
}
