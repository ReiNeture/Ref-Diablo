// Diablo II LOD的Events.
public Event_New_Round()
{
	set_task( 0.5, "Spawn_Items");
	set_task( 0.5, "Spawn_Items_Charsi");
	set_task( 0.5, "Spawn_Items_Akara");
	set_task( 0.5, "Spawn_Items_Miyu");

	g_FreezeTime = true;
}

public Logevent_Round_Start()
{
	g_FreezeTime = false;
}

public Event_Round_End()
{
	Remove_All_Coin_Ents();
	Remove_All_Item_Ents();
	Remove_All_Potion_Ents();
	Remove_All_Inventory_Ents();
	Remove_All_Charsi_Ents();
	Remove_All_Akara_Ents();
	Remove_All_Miyu_Ents();
}

public Ev_CurWeapon(id)
{
	if( !is_user_alive(id) )
		return PLUGIN_CONTINUE;
	
	new weapon = read_data(2)

	if ( weapon != CSW_KNIFE )
	{
		if ( g_Shooting[id] )
		{
			g_Shooting[id] = false;
			
			remove_task(id + TASKID_CROSSBOW);
		}
	}
	else
	{
		entity_set_string(id, EV_SZ_viewmodel, g_brassknuckles);
		entity_set_string(id, EV_SZ_weaponmodel, "");

		for (new item_id = 0; item_id <= g_charcounter; item_id++)
		{
			if ( g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] && item_data[item_id][CHAR_TYPE] == TYPE_WEAPON && g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 )
			{
				if ( equal(item_vmodel[item_id], "models/", 7) )
				{
					entity_set_string(id, EV_SZ_viewmodel, item_vmodel[item_id]);
				}
				else
				{
					entity_set_string(id, EV_SZ_viewmodel, g_brassknuckles);
				}
	
				if ( equal(item_pmodel[item_id], "models/", 7) )
				{
					entity_set_string(id, EV_SZ_weaponmodel, item_pmodel[item_id]);
				}
				else
				{
					entity_set_string(id, EV_SZ_weaponmodel, "");
				}
			}
		}
	}

	return PLUGIN_CONTINUE;
}
public Ev_JoinTeam()
{
	new id = read_data(1);
	static user_team[32];
    
	read_data(2, user_team, 31);

	if( !is_user_connected(id) )
		return 0;
    
	switch( user_team[0] )
	{
		case 'S':  
		{
			main_hero_menu_connect(id);
		}
	}

	return 0;
}
