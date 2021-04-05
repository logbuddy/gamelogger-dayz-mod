class LogbuddyPayload
{
    Logbuddy m_Logbuddy;

    ref array<ref LogbuddyPayloadObject> m_LogbuddyPayloadObjects;
    ref map<string, string> m_LogbuddyActionItems;

    void LogbuddyPayload()
    {
        m_Logbuddy = Logbuddy.GetInstance();

        m_LogbuddyPayloadObjects = new array<ref LogbuddyPayloadObject>;
        m_LogbuddyActionItems = new map<string, string>();
    }

    void AddActionItem(string Key, string Value)
    {
        m_LogbuddyActionItems[Key] = Value;
    }

    void AddPlayer(PlayerBase player, string objref)
    {
        LogbuddyPayloadObject PayloadObject = new LogbuddyPayloadObject("player", objref);

        PayloadObject.AddItem("id", player.GetIdentity().GetPlainId());
        PayloadObject.AddItem("name", player.GetIdentity().GetName());

        m_LogbuddyPayloadObjects.Insert(PayloadObject);
    }

    void AddCar(CarScript Vehicle, string objref)
    {
        LogbuddyPayloadObject PayloadObject = new LogbuddyPayloadObject("vehicle", objref);

        PayloadObject.AddItem("name", Vehicle.ClassName());
        m_LogbuddyPayloadObjects.Insert(PayloadObject);
    }

    string AsJsonString()
    {
        string json = "{";

        json = json + "\"objects\": [";
        foreach (int i, LogbuddyPayloadObject PayloadObject: m_LogbuddyPayloadObjects)
        {
            if (i > 0) json = json + ",";
            json = json + PayloadObject.AsJsonString();
        }
        json = json + "],";

        json = json + "\"action\": {";
        int j = 0;
        foreach (string Key, string Value: m_LogbuddyActionItems)
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
