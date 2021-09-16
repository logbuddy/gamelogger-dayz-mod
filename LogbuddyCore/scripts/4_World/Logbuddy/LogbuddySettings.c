class LogbuddySettings
{
	private int LogbuddyActive = 0;
	private string LogbuddyDatabase = "unknown";
	private int LogbuddyLogLevel = 0;
	private int LogbuddySettingsUpdateMinutes = 1;
	private int LogbuddyPollFrequencyMinutes = 1;
	private int LogbuddyMaxEventsPerPoll = 2;

	[NonSerialized()]
	protected static ref LogbuddySettings s_Instance;

	[NonSerialized()]
	private bool m_IsLoaded;

	[NonSerialized()]
	private string m_SettingsFilePath;

	[NonSerialized()]
	private LogbuddyCore m_LogbuddyCore;

	static LogbuddySettings GetInstance(string settingsFilePath, LogbuddyCore logbuddyCore)
    {
        if(!s_Instance) s_Instance = new LogbuddySettings(settingsFilePath, logbuddyCore);
        return s_Instance;
    }

	void LogbuddySettings(string settingsFilePath, LogbuddyCore logbuddyCore)
	{
		this.m_LogbuddyCore = logbuddyCore;
		this.m_LogbuddyCore.m_Log.Log("Starting LogbuddySettings with file " + settingsFilePath, LogbuddyLogger.LOGLEVEL_CRITICAL);

		this.m_SettingsFilePath = settingsFilePath;
		this.m_IsLoaded = this.Load();

		if(this.LogbuddySettingsUpdateMinutes > 0) {
			this.m_LogbuddyCore.m_Log.Log("Setting initial LogbuddySettings.LogbuddySettingsUpdateMinutes to " + this.LogbuddySettingsUpdateMinutes, LogbuddyLogger.LOGLEVEL_CRITICAL);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.UpdateSettings, this.GetLogbuddySettingsUpdateMinutes() * 1000 * 60, false);
		}
	}

	bool Load()
	{
		if (FileExist(this.m_SettingsFilePath))
		{
			JsonFileLoader<LogbuddySettings>.JsonLoadFile(this.m_SettingsFilePath, this);
			this.m_LogbuddyCore.m_Log.Log("Loading LogbuddySettings from  " + this.m_SettingsFilePath, LogbuddyLogger.LOGLEVEL_DEBUG);
			this.m_LogbuddyCore.m_Log.Log("LogbuddyActive: " + this.LogbuddyActive, LogbuddyLogger.LOGLEVEL_DEBUG);
    		this.m_LogbuddyCore.m_Log.Log("LogbuddyLogLevel: " + this.LogbuddyLogLevel, LogbuddyLogger.LOGLEVEL_DEBUG);
    		this.m_LogbuddyCore.m_Log.Log("LogbuddySettingsUpdateMinutes: " + this.LogbuddySettingsUpdateMinutes, LogbuddyLogger.LOGLEVEL_DEBUG);
			this.m_LogbuddyCore.m_Log.Log("LogbuddyPollFrequencyMinutes: " + this.LogbuddyPollFrequencyMinutes, LogbuddyLogger.LOGLEVEL_DEBUG);
			this.m_LogbuddyCore.m_Log.Log("LogbuddyMaxEventsPerPoll: " + this.LogbuddyMaxEventsPerPoll, LogbuddyLogger.LOGLEVEL_DEBUG);
			return true;
		}

		this.m_LogbuddyCore.m_Log.Log("LogbuddySettings file doesn't exist; creating", LogbuddyLogger.LOGLEVEL_CRITICAL);
		JsonFileLoader<LogbuddySettings>.JsonSaveFile(this.m_SettingsFilePath, this);
		return false;
	}

	void UpdateSettings()
	{
		this.Load();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.UpdateSettings, this.GetLogbuddySettingsUpdateMinutes() * 1000 * 60, false);
	}

	bool IsLoaded()
	{
		return this.m_IsLoaded;
	}

	int GetActive()
	{
		return this.LogbuddyActive;
	}

	string GetLogbuddyDatabase()
	{
		return this.LogbuddyDatabase;
	}

	int GetLogLevel()
	{
		return this.LogbuddyLogLevel;
	}

	int GetLogbuddySettingsUpdateMinutes()
	{
		return this.LogbuddySettingsUpdateMinutes;
	}

	int GetLogbuddyPollFrequencyMinutes()
	{
		return this.LogbuddyPollFrequencyMinutes;
	}

	int GetLogbuddyMaxEventsPerPoll()
	{
		return this.LogbuddyMaxEventsPerPoll;
	}
}
