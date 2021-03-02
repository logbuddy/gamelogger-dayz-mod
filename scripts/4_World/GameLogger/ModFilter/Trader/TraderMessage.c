modded class TraderMessage
{
    override static void ServerLog(string str)
    {
        GameLogger.Ingest("Trader", str);
    }
}