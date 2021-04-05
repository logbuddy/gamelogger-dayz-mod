class LogbuddyPayload
{
    LogbuddyCore m_LogbuddyCore;

    ref array<ref LogbuddyPayloadObject> m_PayloadObjects;
    ref map<string, string> m_ActionItems;

    void LogbuddyPayload()
    {
        m_LogbuddyCore = LogbuddyCore.GetInstance();

        m_PayloadObjects = new array<ref LogbuddyPayloadObject>;
        m_ActionItems = new map<string, string>();
    }

    void AddActionItem(string Key, string Value)
    {
        m_ActionItems[Key] = Value;
    }

    void AddPlayer(PlayerBase player, string objref)
    {
        LogbuddyPayloadObject PayloadObject = new LogbuddyPayloadObject("player", objref);

        PayloadObject.AddItem("id", player.GetIdentity().GetPlainId());
        PayloadObject.AddItem("name", player.GetIdentity().GetName());

        m_PayloadObjects.Insert(PayloadObject);
    }

    void AddCar(CarScript Vehicle, string objref)
    {
        LogbuddyPayloadObject PayloadObject = new LogbuddyPayloadObject("vehicle", objref);

        PayloadObject.AddItem("name", Vehicle.ClassName());
        m_PayloadObjects.Insert(PayloadObject);
    }

    string AsJsonString()
    {
        string json = "{";

        json = json + "\"objects\": [";
        foreach (int i, LogbuddyPayloadObject PayloadObject: m_PayloadObjects)
        {
            if (i > 0) json = json + ",";
            json = json + PayloadObject.AsJsonString();
        }
        json = json + "],";

        json = json + "\"action\": {";
        int j = 0;
        foreach (string Key, string Value: m_ActionItems)
        {
            if (j > 0) json = json + ",";
            json = json + "\"" + Key + "\": " + "\"" + Value +"\"";
            j++;
        }
        json = json + "}";

        json = json + "}";
        return json;
    }
}
