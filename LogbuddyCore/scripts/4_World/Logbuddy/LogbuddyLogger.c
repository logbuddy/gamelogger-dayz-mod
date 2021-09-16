class LogbuddyLogger
{
    static const int LOGLEVEL_CRITICAL = 0;
    static const int LOGLEVEL_QUEUE = 10;
    static const int LOGLEVEL_DEBUG = 20;

    static const string m_LogPath = LogbuddyCore.m_ProfilePath + "/log";
    static const string m_LogFile = m_LogPath + "/main.txt";
    
    private LogbuddySettings m_Settings;

    private int m_LogLevel;

    void LogbuddyLogger()
    {
    }

    void LogbuddySetSettings(LogbuddySettings logbuddySettings)
    {
        m_Settings = logbuddySettings;
    }

    void Log(string message, int LogbuddyLogLevel = LogbuddyLogger.LOGLEVEL_DEBUG)
    {
        //Print("[Logbuddy] Log request: Message log level " + LogbuddyLogLevel + ", System log level " + this.m_LogLevel);
        if(LogbuddyLogLevel > this.m_Settings.GetLogLevel())
            return;

        string logFile = LogbuddyLogger.m_LogFile;
        FileHandle file = OpenFile(logFile, FileMode.APPEND);

        if (file)
        {
            string timestamp = LogbuddyHelper.GetTimestamp();
            FPrintln(file, "[" + timestamp + "] " + message.Trim());
            CloseFile(file);
        }
        else
        {
            Print("[Logbuddy] Could not create logfile " + m_LogFile);
        }
    }
}


