class LogbuddyRestCallback extends RestCallback
{
    ref LogbuddyCore m_LogbuddyCore;

    void SetLogbuddyCore(LogbuddyCore _LogbuddyCore)
    {
        m_LogbuddyCore = _LogbuddyCore;
    }

    override void OnError(int errorCode)
    {
        m_LogbuddyCore.m_Log.Log("RestCallback OnError, code " + errorCode.ToString(), LogbuddyLogger.LOGLEVEL_CRITICAL);
    }

    override void OnTimeout()
    {
        m_LogbuddyCore.m_Log.Log("RestCallback OnTimeout", LogbuddyLogger.LOGLEVEL_CRITICAL);
    }

    override void OnSuccess(string data, int dataSize)
    {
        m_LogbuddyCore.m_Log.Log("RestCallback OnSuccess size=" + dataSize.ToString(), LogbuddyLogger.LOGLEVEL_DEBUG);
    }

    override void OnFileCreated(string fileName, int dataSize)
    {
        m_LogbuddyCore.m_Log.Log("RestCallback OnFileCreated, file=" + fileName + " size=" + dataSize.ToString(), LogbuddyLogger.LOGLEVEL_DEBUG);
    }
}
