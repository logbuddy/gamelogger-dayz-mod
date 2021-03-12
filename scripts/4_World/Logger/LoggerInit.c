class LoggerInit : ItemBase
{
    override void EEInit()
    {
        super.EEInit();

        if (IsMissionHost())
        {
            Logger l = Logger.GetInstance();
        }
    }
}