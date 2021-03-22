class LoggerRestCallback extends RestCallback
{
    ref Logger m_Logger;

    void SetLogger(Logger logger)
    {
        m_Logger = logger;
    }

    override void OnError(int errorCode)
    {
        m_Logger.m_Log.Log("RestCallback OnError, code " + errorCode.ToString());
    }

    override void OnTimeout()
    {
        m_Logger.m_Log.Log("RestCallback OnTimeout");
    }

    override void OnSuccess(string data, int dataSize)
    {
        m_Logger.m_Log.Log("RestCallback OnSuccess size=" + dataSize.ToString());
    }

    override void OnFileCreated(string fileName, int dataSize)
    {
        m_Logger.m_Log.Log("RestCallback OnFileCreated, file=" + fileName + " size=" + dataSize.ToString());
    }
}
