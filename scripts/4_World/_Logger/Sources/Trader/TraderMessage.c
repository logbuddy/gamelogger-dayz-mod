modded class TraderMessage
{
    protected static bool Filter(string str)
    {
        if (str.Substring(0, 18) == "[TRADER] Player: (") return true;
        
        return false;
    }

    override static void ServerLog(string str)
    {
        Print(str);

        if(!TraderMessage.Filter(str)) return;

        Logger l = Logger.GetInstance();
        l.Ingest("Trader", str);
    }
}