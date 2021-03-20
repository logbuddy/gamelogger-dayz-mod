class LoggerSettings
{
	private string userId = "";
	private string serverId = "";
	private string apiKeyId = "";

	[NonSerialized()]
	private bool m_IsLoaded;

	void LoggerSettings(string settingsFilePath)
	{
		this.m_IsLoaded = this.Load(settingsFilePath) && this.userId && this.serverId && this.apiKeyId;
	}

	bool Load(string settingsFilePath)
	{
		if (FileExist(settingsFilePath))
		{
			JsonFileLoader<LoggerSettings>.JsonLoadFile(settingsFilePath, this);
			return true;
		}

		JsonFileLoader<LoggerSettings>.JsonSaveFile(settingsFilePath, this);
		return false;
	}

	bool IsLoaded()
	{
		return this.m_IsLoaded;
	}

	string GetUserId()
	{
		return this.userId;
	}

	string GetServerId()
	{
		return this.serverId;
	}

	string GetApiKeyId()
	{
		return this.apiKeyId;
	}
}
