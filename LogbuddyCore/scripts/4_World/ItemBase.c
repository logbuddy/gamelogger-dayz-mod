modded class ItemBase extends InventoryItem {

    override void EEItemLocationChanged (notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc) {

        super.EEItemLocationChanged (oldLoc, newLoc);

		EntityAI old_owner = oldLoc.GetParent();
		EntityAI new_owner = newLoc.GetParent();

        PlayerBase new_owner_player;
        PlayerBase old_owner_player;

        if(new_owner)
            Class.CastTo(new_owner_player, new_owner.GetHierarchyRootPlayer());

        if(old_owner)
            Class.CastTo(old_owner_player, old_owner.GetHierarchyRootPlayer());

        //
        // If both items lead to a player, it must be a player moving an item within his own inventory
        //
        if(new_owner_player && old_owner_player)
            return;

        if(!new_owner_player)
            return;

        string InventoryLocationTypeNames[6] = {
            "UNKNOWN",
            "GROUND",
            "ATTACHMENT",
            "CARGO",
            "HANDS",
            "PROXYCARGO"
        };

        if(newLoc.GetType() != InventoryLocationType.ATTACHMENT && newLoc.GetType() != InventoryLocationType.CARGO && newLoc.GetType() != InventoryLocationType.HANDS)
            return;

        string item_name = this.GetType();
        int item_id = this.GetID();

        string old_environment = InventoryLocationTypeNames[oldLoc.GetType()];
        string new_environment = InventoryLocationTypeNames[newLoc.GetType()];
        
        if(oldLoc.GetType() == InventoryLocationType.UNKNOWN)
            return;

        LogbuddyCore logbuddyCore = LogbuddyCore.GetInstance();
        if(logbuddyCore.m_Settings.GetActive() != 1)
            return;

        string player_log = new_owner_player.GetIdentity().GetName() + " (" + new_owner_player.GetIdentity().GetPlainId() + ")";
        string item_log = " took " + item_name + " (" + item_id + ")";
        string environment_log = " " + old_environment + ">" + new_environment;
        string location_log = " " + this.GetPosition();
        logbuddyCore.m_Log.Log(player_log + item_log + environment_log + location_log, LogbuddyLogger.LOGLEVEL_DEBUG);

        LogbuddyPayload Payload = new LogbuddyPayload();
        
        Payload.AddPlayer(new_owner_player, "taker");
        Payload.AddActionItem("player", "taker");
        Payload.AddActionItem("item", item_name);
        Payload.AddActionItem("item_id", item_id.ToString());
        Payload.AddActionItem("position", this.GetPosition().ToString());
        Payload.AddActionItem("oldloc", InventoryLocationTypeNames[oldLoc.GetType()]);
        Payload.AddActionItem("newloc", InventoryLocationTypeNames[newLoc.GetType()]);

        logbuddyCore.Ingest("logs", "ActionTakeItem", Payload);
    }
}
