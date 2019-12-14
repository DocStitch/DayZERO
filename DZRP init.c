//Spawn helper function
void SpawnObject(string type, vector position, vector orientation)
{
    auto obj = GetGame().CreateObject(type, position);
    obj.SetPosition(position);
    obj.SetOrientation(orientation);
    obj.SetOrientation(obj.GetOrientation()); //Collision fix
    obj.Update();

    obj.SetAffectPathgraph(true, false);
    if (obj.CanAffectPathgraph())
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, obj);
}

void main()
{
	//INIT WEATHER BEFORE ECONOMY INIT------------------------
	Weather weather = g_Game.GetWeather();

	weather.MissionWeather(false);    // false = use weather controller from Weather.c

	weather.GetOvercast().Set( Math.RandomFloatInclusive(0.4, 0.6), 1, 0);
	weather.GetRain().Set( 0, 0, 1);
	weather.GetFog().Set( Math.RandomFloatInclusive(0.05, 0.1), 1, 0);

	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();

	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 9, reset_day = 20;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}

	// Spawn custom objects
SpawnObject( "ChristmasTree_Green", "3814.781982 318.942749 8991.261719", "0.000000 0.000000 0.000000" );
SpawnObject( "Land_Medical_Tent_Big", "3776.786621 311.458771 9019.118164", "-72.999992 0.000000 0.000000" );
SpawnObject( "Land_Medical_Tent_Shower", "3762.952637 313.196472 8998.555664", "135.999985 0.000000 0.000000" );
}

class CustomMission: MissionServer
{
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			int rndHlt = Math.RandomInt(55,100);
			itemEnt.SetHealth("","",rndHlt);
		}
	}

	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer(identity, characterName, "4008.43 334.032 9195.87", 0, "NONE");//Creates random player
		Class.CastTo(m_player, playerEnt);

		GetGame().SelectPlayer(identity, m_player);

		return m_player;
	}

	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		EntityAI itemTop;
		EntityAI itemEnt;
		ItemBase itemBs;
		float rand;

		itemTop = player.FindAttachmentBySlotName("Body");

		if ( itemTop )
		{
			itemEnt = itemTop.GetInventory().CreateInInventory("Rag");
			if ( Class.CastTo(itemBs, itemEnt ) )
				itemBs.SetQuantity(4);

			SetRandomHealth(itemEnt);

			itemEnt = player.GetInventory().CreateInInventory("Glock19");
			itemBs = ItemBase.Cast(itemEnt);

			itemEnt = itemTop.GetInventory().CreateInInventory("Mag_Glock_15Rnd");
			SetRandomHealth(itemEnt);

			itemEnt = player.GetInventory().CreateInInventory("CivilianBelt");
			itemBs = ItemBase.Cast(itemEnt);

			itemEnt = itemTop.GetInventory().CreateInInventory("Canteen");
			SetRandomHealth(itemEnt);

			string chemlightArray[] = { "Chemlight_White", "Chemlight_Yellow", "Chemlight_Green", "Chemlight_Red" };
			int rndIndex = Math.RandomInt(0, 4);
			itemEnt = itemTop.GetInventory().CreateInInventory(chemlightArray[rndIndex]);
			SetRandomHealth(itemEnt);

			rand = Math.RandomFloatInclusive(0.0, 1.0);
			if ( rand < 0.35 )
				itemEnt = player.GetInventory().CreateInInventory("PlateCarrierHolster");
			else if ( rand > 0.65 )
				itemEnt = player.GetInventory().CreateInInventory("PlateCarrierHolster");
			else
				itemEnt = player.GetInventory().CreateInInventory("PlateCarrierHolster");

			SetRandomHealth(itemEnt);
		}
	}
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}
