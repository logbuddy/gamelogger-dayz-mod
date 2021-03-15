modded class MissionServer
{
    override void OnEvent(EventType eventTypeId, Param params)
    {
        super.OnEvent(eventTypeId, params);

        /*
        switch (eventTypeId)
        {
            case ScriptLogEventTypeID:
            {
                Param1<string> log_params = Param1<string>.Cast(params);
                
                Logger l = Logger.GetInstance();
                l.Ingest("ScriptLog", log_params.param1);
            }
        }
        */
    }
}