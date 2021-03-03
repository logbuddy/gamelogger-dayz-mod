modded class TraderMessage
{
    override static void ServerLog(string str)
    {
        GameLogger gl = GameLogger.GetInstance();
        gl.Ingest("Trader", str);
    }
}