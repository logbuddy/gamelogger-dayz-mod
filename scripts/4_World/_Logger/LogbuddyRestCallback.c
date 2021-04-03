class LogbuddyRestCallback extends RestCallback
{
    ref Logbuddy m_Logbuddy;

    void SetLogbuddy(Logbuddy _Logbuddy)
    {
        m_Logbuddy = _Logbuddy;
    }

    override void OnError(int errorCode)
    {
        m_Logbuddy.m_Log.Log("RestCallback OnError, code " + errorCode.ToString());
    }

    override void OnTimeout()
    {
        m_Logbuddy.m_Log.Log("RestCallback OnTimeout");
    }

    override void OnSuccess(string data, int dataSize)
    {
        m_Logbuddy.m_Log.Log("RestCallback OnSuccess size=" + dataSize.ToString());
    }

    override void OnFileCreated(string fileName, int dataSize)
    {
        m_Logbuddy.m_Log.Log("RestCallback OnFileCreated, file=" + fileName + " size=" + dataSize.ToString());
    }
}
