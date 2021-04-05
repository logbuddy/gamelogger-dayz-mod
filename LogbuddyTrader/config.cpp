class CfgPatches
{
	class LogbuddyTrader
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {
			"DZ_Data",
			"DZ_Scripts"
		};
	};   
};

class CfgMods
{
	class Logger
	{
		dir = "inkihh\Logbuddy\LogbuddyTrader";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 0;
		name = "LogbuddyTrader";
		credits = "";
		author = "inkihh";
		authorID = "0";
		version = "1";
		extra = 0;
		type = "mod";

		dependencies[] =
		{
			"World"
		};

		class defs
		{
			class worldScriptModule
			{
				value="";
				files[] =
				{
					"inkihh\Logbuddy\LogbuddyTrader\scripts\4_World"
				};
			};
		};
	};
};
