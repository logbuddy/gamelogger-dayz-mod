modded class PluginMCKLogs extends PluginBase
{
	override void LogMCKActivity(string text)
	{
		super.LogMCKActivity(text);

		if (GetGame().IsMultiplayer())
		{
			LogbuddyCore logbuddyCore = LogbuddyCore.GetInstance();
			if(logbuddyCore.m_Settings.GetActive() == 1)
			{
				logbuddyCore.m_Log.Log(text, LogbuddyLogger.LOGLEVEL_DEBUG);

				LogbuddyPayload Payload = new LogbuddyPayload();
				
				Payload.AddActionItem("logtext", text);
				logbuddyCore.Ingest("mck-logs", "MuchCarKey", Payload);
			}
		}
	}
};