class LogbuddyRestCallback extends RestCallback
{
    ref LogbuddyCore m_LogbuddyCore;

    void SetLogbuddyCore(LogbuddyCore _LogbuddyCore)
    {
        m_LogbuddyCore = _LogbuddyCore;
    }

    override void OnError(int errorCode)
    {
        m_LogbuddyCore.m_Log.Log("RestCallback OnError, code " + errorCode.ToString());
    }

    override void OnTimeout()
    {
        m_LogbuddyCore.m_Log.Log("RestCallback OnTimeout");
    }

    override void OnSuccess(string data, int dataSize)
    {
        m_LogbuddyCore.m_Log.Log("RestCallback OnSuccess size=" + dataSize.ToString());
    }

    override void OnFileCreated(string fileName, int dataSize)
    {
        m_LogbuddyCore.m_Log.Log("RestCallback OnFileCreated, file=" + fileName + " size=" + dataSize.ToString());
    }
}
