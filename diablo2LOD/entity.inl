// Diablo II LOD 裡面所有實體的東西.

// 實體的錢.
public Coins_Pickup(ptr, ptd)
{
	if( is_user_alive(ptd) && pev_valid(ptr) ) 
	{ 	
		new victim = entity_get_edict(ptr , EV_ENT_owner)

		Set_Player_Coins(ptd, g_Coins[ptd][g_CurrentChar[ptd]] + g_CoinsDropDeath[victim])
					
		remove_entity(ptr)
	}
}

Create_Coins(victim)
{
	new Float:Origin[3];
	entity_get_vector(victim, EV_VEC_origin, Origin)

	new Float:fGlowColors[3] = {255.0, 255.0, 0.0}
	new coin_ent = create_entity("info_target")
	if(pev_valid(coin_ent))
	{
		entity_set_string(coin_ent, EV_SZ_classname, "Coins")
				
		entity_set_int(coin_ent, EV_INT_solid, SOLID_TRIGGER)
		entity_set_int(coin_ent, EV_INT_movetype, MOVETYPE_TOSS)
		entity_set_edict(coin_ent, EV_ENT_owner, victim)
		entity_set_int(coin_ent, EV_INT_renderfx, kRenderFxGlowShell)
		entity_set_vector(coin_ent, EV_VEC_rendercolor, fGlowColors)

		Origin[0] += 30.0;
		Origin[1] += -20.0;
		entity_set_vector(coin_ent, EV_VEC_origin, Origin)
		
		entity_set_model(coin_ent, g_w_coins)
		entity_set_size(coin_ent, Float:{-2.5, -2.5, -1.5}, Float:{2.5, 2.5, 1.5})

		drop_to_floor(coin_ent)
	}
}
public Native_Create_Coins( iPlugin ) 
{
	new victim = get_param( 1 );

	new Classname[32];
	get_string( 2, Classname, sizeof ( Classname ) - 1 );

	new gold = get_param( 3 );

	new Float:Origin[3];
	entity_get_vector(victim, EV_VEC_origin, Origin)
	
	new Float:fGlowColors[3] = {255.0, 255.0, 0.0}
	new coin_ent = create_entity("info_target")
	if(pev_valid(coin_ent))
	{
		entity_set_string(coin_ent, EV_SZ_classname, Classname)

		entity_set_int(coin_ent , EV_INT_iuser1, gold)
				
		entity_set_int(coin_ent, EV_INT_solid, SOLID_TRIGGER)
		entity_set_int(coin_ent, EV_INT_movetype, MOVETYPE_TOSS)
		entity_set_edict(coin_ent, EV_ENT_owner, victim)
		entity_set_int(coin_ent, EV_INT_renderfx, kRenderFxGlowShell)
		entity_set_vector(coin_ent, EV_VEC_rendercolor, fGlowColors)

		Origin[0] += 30.0;
		Origin[1] += -20.0;
		entity_set_vector(coin_ent, EV_VEC_origin, Origin)
		
		entity_set_model(coin_ent, g_w_coins)
		entity_set_size(coin_ent, Float:{-2.5, -2.5, -1.5}, Float:{2.5, 2.5, 1.5})

		drop_to_floor(coin_ent)
	}
}

public Remove_All_Coin_Ents()
{
	new coin_ent = find_ent_by_class(-1, "Coins")
	
	while ( coin_ent ) 
	{
		remove_entity(coin_ent)
		coin_ent = find_ent_by_class(coin_ent, "Coins")
	}
}

// 實體 - 思考, 倉庫, 武器商人, 藥水商人.
Create_Inventory(Origin[3])
{
	new Float:flOrigin[3]
	IVecFVec(Origin, flOrigin)
	
	new inv_ent = create_entity("info_target")

	if(pev_valid(inv_ent))
	{
		entity_set_string(inv_ent, EV_SZ_classname, "Inventory")
				
		entity_set_int(inv_ent, EV_INT_solid, SOLID_BBOX)
		entity_set_int(inv_ent, EV_INT_movetype, MOVETYPE_NONE)
		entity_set_edict(inv_ent, EV_ENT_owner, 0)

		drop_to_floor(inv_ent)

		entity_set_vector(inv_ent, EV_VEC_origin, flOrigin)

		entity_set_model(inv_ent, g_w_inventory)
		entity_set_size(inv_ent, Float:{-5.5, -5.5, -17.5}, Float:{5.5, 5.5, 17.5})

		entity_set_float(inv_ent,EV_FL_nextthink,halflife_time() + 0.01)
	}
}
Create_Charsi(Origin[3])
{
	new Float:flOrigin[3]
	IVecFVec(Origin, flOrigin)

	new charsi_ent = create_entity("info_target")

	if(pev_valid(charsi_ent))
	{
		entity_set_string(charsi_ent, EV_SZ_classname, "Charsi")
				
		entity_set_int(charsi_ent, EV_INT_solid, SOLID_BBOX)
		entity_set_int(charsi_ent, EV_INT_movetype, MOVETYPE_NONE)
		entity_set_edict(charsi_ent, EV_ENT_owner, 0)

		drop_to_floor(charsi_ent)

		entity_set_vector(charsi_ent, EV_VEC_origin, flOrigin)
		
		entity_set_model(charsi_ent, g_w_charsi)
		entity_set_size(charsi_ent, Float:{-12.5, -12.5, -56.5}, Float:{12.5, 12.5, 66.5})		

		entity_set_float(charsi_ent,EV_FL_nextthink,halflife_time() + 0.01)
	}
}

Create_Akara(Origin[3])
{
	new Float:flOrigin[3]
	IVecFVec(Origin, flOrigin)

	new akar_ent = create_entity("info_target")

	if(pev_valid(akar_ent))
	{
		entity_set_string(akar_ent, EV_SZ_classname, "Akara")
				
		entity_set_int(akar_ent, EV_INT_solid, SOLID_BBOX)
		entity_set_int(akar_ent, EV_INT_movetype, MOVETYPE_NONE)
		entity_set_edict(akar_ent, EV_ENT_owner, 0)

		drop_to_floor(akar_ent)

		entity_set_vector(akar_ent, EV_VEC_origin, flOrigin)
		
		entity_set_model(akar_ent, g_w_akara)
		entity_set_size(akar_ent, Float:{-12.5, -12.5, -56.5}, Float:{12.5, 12.5, 66.5})		
		entity_set_float(akar_ent,EV_FL_nextthink,halflife_time() + 0.01)
	}
}

Save_Origin(CurMap[], Origin[3])
{	
	new MapFile[64], Text[64]
	format(MapFile, 63, "%s/%s.cfg", g_ItemOriginDir, CurMap)

	if(!file_exists(MapFile)) 
	{
		new Comments[64]
		format(Comments, 63, "; %s 的地圖道具座標.", CurMap)
		write_file(MapFile, Comments, -1)
	}
	
	format(Text, 63, "%i %i %i", Origin[0], Origin[1], Origin[2])
	write_file(MapFile, Text, -1)
}
Save_Origin_Charsi(CurMap[], Origin[3])
{	
	new MapFile[64], Text[64]
	format(MapFile, 63, "%s/%s_charsi.cfg", g_ItemOriginDir, CurMap)

	if(!file_exists(MapFile)) 
	{
		new Comments[64]
		format(Comments, 63, "; %s 的地圖道具座標.", CurMap)
		write_file(MapFile, Comments, -1)
	}
	
	format(Text, 63, "%i %i %i", Origin[0], Origin[1], Origin[2])
	write_file(MapFile, Text, -1)
}
Save_Origin_Akara(CurMap[], Origin[3])
{	
	new MapFile[64], Text[64]
	format(MapFile, 63, "%s/%s_akara.cfg", g_ItemOriginDir, CurMap)

	if(!file_exists(MapFile)) 
	{
		new Comments[64]
		format(Comments, 63, "; %s 的地圖道具座標.", CurMap)
		write_file(MapFile, Comments, -1)
	}
	
	format(Text, 63, "%i %i %i", Origin[0], Origin[1], Origin[2])
	write_file(MapFile, Text, -1)
}

Load_Origins(CurMap[])
{
	new MapFile[64]
	format(MapFile, 63, "%s/%s.cfg", g_ItemOriginDir, CurMap)

	if(!file_exists(MapFile))
		return PLUGIN_CONTINUE;

	g_MapItemNum = 0
	for(new i = 1; i <= MAX_MAPITEMS; ++i) 
	{
		g_MapItemOrgins[i][0] = 0
		g_MapItemOrgins[i][1] = 0
		g_MapItemOrgins[i][2] = 0
	}
	
	new Text[64], Line = 0, Len = 0;

	while(read_file(MapFile, Line++, Text, 63, Len))
	{
		if((Text[0]==';') || !Len) {
		 	continue
		}
		
		if(g_MapItemNum >= MAX_MAPITEMS) 
		{
			log_amx("達到地圖道具最大數量,請增加MAX_MAPITEMS.")
			break
		}
		
		new iOrigin[3][16];
		parse(Text, iOrigin[0], 15, iOrigin[1], 15, iOrigin[2], 15)
		
		g_MapItemNum++
		g_MapItemOrgins[g_MapItemNum][0] = str_to_num(iOrigin[0])
		g_MapItemOrgins[g_MapItemNum][1] = str_to_num(iOrigin[1])
		g_MapItemOrgins[g_MapItemNum][2] = str_to_num(iOrigin[2])
	}
	
	return PLUGIN_CONTINUE
}
Load_Origins_Charsi(CurMap[])
{
	new MapFile[64]
	format(MapFile, 63, "%s/%s_charsi.cfg", g_ItemOriginDir, CurMap)

	if(!file_exists(MapFile))
		return PLUGIN_CONTINUE;

	g_MapItemNum1 = 0
	for(new i = 1; i <= MAX_MAPITEMS; ++i) 
	{
		g_MapItemOrgins1[i][0] = 0
		g_MapItemOrgins1[i][1] = 0
		g_MapItemOrgins1[i][2] = 0
	}
	
	new Text[64], Line = 0, Len = 0;

	while(read_file(MapFile, Line++, Text, 63, Len))
	{
		if((Text[0]==';') || !Len) {
		 	continue
		}
		
		if(g_MapItemNum1 >= MAX_MAPITEMS) 
		{
			log_amx("達到地圖道具最大數量,請增加MAX_MAPITEMS.")
			break
		}
		
		new iOrigin[3][16];
		parse(Text, iOrigin[0], 15, iOrigin[1], 15, iOrigin[2], 15)
		
		g_MapItemNum1++
		g_MapItemOrgins1[g_MapItemNum][0] = str_to_num(iOrigin[0])
		g_MapItemOrgins1[g_MapItemNum][1] = str_to_num(iOrigin[1])
		g_MapItemOrgins1[g_MapItemNum][2] = str_to_num(iOrigin[2])
	}
	
	return PLUGIN_CONTINUE
}
Load_Origins_Akara(CurMap[])
{
	new MapFile[64]
	format(MapFile, 63, "%s/%s_akara.cfg", g_ItemOriginDir, CurMap)

	if(!file_exists(MapFile))
		return PLUGIN_CONTINUE;

	g_MapItemNum2 = 0
	for(new i = 1; i <= MAX_MAPITEMS; ++i) 
	{
		g_MapItemOrgins2[i][0] = 0
		g_MapItemOrgins2[i][1] = 0
		g_MapItemOrgins2[i][2] = 0
	}
	
	new Text[64], Line = 0, Len = 0;

	while(read_file(MapFile, Line++, Text, 63, Len))
	{
		if((Text[0]==';') || !Len) {
		 	continue
		}
		
		if(g_MapItemNum2 >= MAX_MAPITEMS) 
		{
			log_amx("達到地圖道具最大數量,請增加MAX_MAPITEMS.")
			break
		}
		
		new iOrigin[3][16];
		parse(Text, iOrigin[0], 15, iOrigin[1], 15, iOrigin[2], 15)
		
		g_MapItemNum2++
		g_MapItemOrgins2[g_MapItemNum][0] = str_to_num(iOrigin[0])
		g_MapItemOrgins2[g_MapItemNum][1] = str_to_num(iOrigin[1])
		g_MapItemOrgins2[g_MapItemNum][2] = str_to_num(iOrigin[2])
	}
	
	return PLUGIN_CONTINUE
}

public Spawn_Items()
{
	for(new i = 1; i <= MAX_MAPITEMS; ++i)
	{
		if((g_MapItemOrgins[i][0] == 0) 
		&& (g_MapItemOrgins[i][1] == 0) 
		&& g_MapItemOrgins[i][2] == 0) { 
				continue
		}
		Create_Inventory(g_MapItemOrgins[i]);
	}
}
public Spawn_Items_Charsi()
{
	for(new i = 1; i <= MAX_MAPITEMS; ++i)
	{
		if((g_MapItemOrgins1[i][0] == 0) 
		&& (g_MapItemOrgins1[i][1] == 0) 
		&& g_MapItemOrgins1[i][2] == 0) { 
				continue
		}
		Create_Charsi(g_MapItemOrgins1[i]);
	}
}
public Spawn_Items_Akara()
{
	for(new i = 1; i <= MAX_MAPITEMS; ++i)
	{
		if((g_MapItemOrgins2[i][0] == 0) 
		&& (g_MapItemOrgins2[i][1] == 0) 
		&& g_MapItemOrgins2[i][2] == 0) { 
				continue
		}
		Create_Akara(g_MapItemOrgins2[i]);
	}
}

RemoveMapItems()
{
	new MapFile[64], CurMap[32];
	get_mapname(CurMap, 31);
	format(MapFile, 63, "%s/%s.cfg", g_ItemOriginDir, CurMap);

	if(file_exists(MapFile)) {
		delete_file(MapFile)
	}

	g_MapItemNum = 0

	for(new i = 1; i <= MAX_MAPITEMS; ++i) 
	{
		g_MapItemOrgins[i][0] = 0
		g_MapItemOrgins[i][1] = 0
		g_MapItemOrgins[i][2] = 0
	}
}
RemoveMapItems_Charsi()
{
	new MapFile[64], CurMap[32];
	get_mapname(CurMap, 31);
	format(MapFile, 63, "%s/%s_charsi.cfg", g_ItemOriginDir, CurMap);

	if(file_exists(MapFile)) {
		delete_file(MapFile)
	}

	g_MapItemNum1 = 0

	for(new i = 1; i <= MAX_MAPITEMS; ++i) 
	{
		g_MapItemOrgins1[i][0] = 0
		g_MapItemOrgins1[i][1] = 0
		g_MapItemOrgins1[i][2] = 0
	}
}
RemoveMapItems_Akara()
{
	new MapFile[64], CurMap[32];
	get_mapname(CurMap, 31);
	format(MapFile, 63, "%s/%s_akara.cfg", g_ItemOriginDir, CurMap);

	if(file_exists(MapFile)) {
		delete_file(MapFile)
	}

	g_MapItemNum2 = 0

	for(new i = 1; i <= MAX_MAPITEMS; ++i) 
	{
		g_MapItemOrgins2[i][0] = 0
		g_MapItemOrgins2[i][1] = 0
		g_MapItemOrgins2[i][2] = 0
	}
}

public Remove_All_Inventory_Ents()
{
	new inv_ent = find_ent_by_class(-1, "Inventory")
	
	while ( inv_ent ) 
	{
		remove_entity(inv_ent)
		inv_ent = find_ent_by_class(inv_ent, "Inventory")
	}
}
public Remove_All_Charsi_Ents()
{
	new char_ent = find_ent_by_class(-1, "Charsi")
	
	while ( char_ent ) 
	{
		remove_entity(char_ent)
		char_ent = find_ent_by_class(char_ent, "Charsi")
	}
}
public Remove_All_Akara_Ents()
{
	new akara_ent = find_ent_by_class(-1, "Akara")
	
	while ( akara_ent ) 
	{
		remove_entity(akara_ent)
		akara_ent = find_ent_by_class(akara_ent, "Akara")
	}
}

// 玩家掉落的物品實體.
Drop_Items(victim)
{
	new Float:flOrigin[3], Origin[3];
	get_user_origin(victim, Origin, 0);

	IVecFVec(Origin, flOrigin)

	for(new item_id = 0; item_id <= g_charcounter ; item_id++) 
	{
		if( g_iPlayerItemWorn[victim][g_CurrentChar[victim]][item_id] 
		&& g_iPlayerItem[victim][g_CurrentChar[victim]][item_id] > 0 )
		{
			new item_ent = create_entity("info_target");
			new Float:fGlowColors[3] = {255.0, 255.0,255.0}

			if(pev_valid(item_ent))
			{
				entity_set_string(item_ent, EV_SZ_classname, "ItemsDeath")
				
				entity_set_int(item_ent, EV_INT_solid, SOLID_TRIGGER)
				entity_set_int(item_ent, EV_INT_movetype, MOVETYPE_TOSS)
				entity_set_edict(item_ent, EV_ENT_owner, 0)
				entity_set_int(item_ent, EV_INT_iuser4, item_id)
				entity_set_int(item_ent, EV_INT_iuser3, g_iPlayerItemRepair[victim][g_CurrentChar[victim]][item_id])
				entity_set_int(item_ent, EV_INT_renderfx, kRenderFxGlowShell)
				entity_set_vector(item_ent, EV_VEC_rendercolor, fGlowColors)

				drop_to_floor(item_ent)

				flOrigin[2] += 15.0;
				entity_set_vector(item_ent, EV_VEC_origin, flOrigin)
				
				entity_set_model(item_ent, g_w_item)
				entity_set_size(item_ent, Float:{-2.5, -2.5, -1.5}, Float:{2.5, 2.5, 1.5})
				
				g_iPlayerItem[victim][g_CurrentChar[victim]][item_id]--;

				if ( g_iPlayerItem[victim][g_CurrentChar[victim]][item_id] < 1 )
				{
					g_iPlayerItemWorn[victim][g_CurrentChar[victim]][item_id] = ITEM_NOT_WORN;

					ExecuteForward( g_iItemTook, g_iReturn, victim, item_id);
				}
			}
		}
	}
}

public Item_Touch(ptr, ptd)
{
	if( is_user_alive(ptd) && pev_valid(ptr) ) 
	{ 	
		new item_id = entity_get_int(ptr, EV_INT_iuser4)
		new item_damage_value = entity_get_int(ptr, EV_INT_iuser3)

		if(Get_Player_Items(ptd) < MAX_PLAYER_ITEMS)
		{
			client_printcolor(ptd, "/g撿到了/ctr%s!", item_name[item_id]);

			g_iPlayerItem[ptd][g_CurrentChar[ptd]][item_id]++;

			if ( item_data[item_id][CHAR_DURABILITY] > 0 )
			{
				g_iPlayerItemRepair[ptd][g_CurrentChar[ptd]][item_id] = item_damage_value;
			}

			if ( g_iPlayerItem[ptd][g_CurrentChar[ptd]][item_id] <= 1 )
			{
				g_iPlayerItemWorn[ptd][g_CurrentChar[ptd]][item_id] = ITEM_NOT_WORN;
			}

			remove_entity(ptr);
		}
	}
}

public Remove_All_Item_Ents()
{
	new item_ent = find_ent_by_class(-1, "ItemsDeath")
	
	while ( item_ent ) 
	{
		remove_entity(item_ent)
		item_ent = find_ent_by_class(item_ent, "ItemsDeath")
	}
}


// 玩家掉落的藥水實體.
Drop_Potions(victim)
{
	new Float:flOrigin[3], Origin[3];
	get_user_origin(victim, Origin, 0);

	IVecFVec(Origin, flOrigin)

	for(new item_id = 0; item_id < MAX_POTIONS ; item_id++) 
	{
		if( g_iPlayerPotions[victim][g_CurrentChar[victim]][item_id] > 0 )
		{
			new item_ent = create_entity("info_target");

			if(pev_valid(item_ent))
			{
				entity_set_string(item_ent, EV_SZ_classname, "PotionsDeath")
				
				entity_set_int(item_ent, EV_INT_solid, SOLID_TRIGGER)
				entity_set_int(item_ent, EV_INT_movetype, MOVETYPE_TOSS)
				entity_set_edict(item_ent, EV_ENT_owner, 0)
				entity_set_int(item_ent, EV_INT_iuser4, item_id)


				drop_to_floor(item_ent)

				flOrigin[0] += random_float(-15.0, -40.0);
				flOrigin[1] += random_float(15.0, 40.0);
				entity_set_vector(item_ent, EV_VEC_origin, flOrigin)
				
				if ( item_id == PTS_HP_MINOR || item_id == PTS_HP_GREATER || item_id == PTS_HP_SUPER )
				{
					entity_set_model(item_ent, g_w_hp)
				}
				else
				{
					entity_set_model(item_ent, g_w_mp)
				}

				entity_set_size(item_ent, Float:{-2.5, -2.5, -1.5}, Float:{2.5, 2.5, 1.5})

				g_iPlayerPotions[victim][g_CurrentChar[victim]][item_id]--;
			}
		}
	}
}
public Potion_Touch(ptr, ptd)
{
	if( is_user_alive(ptd) && pev_valid(ptr) ) 
	{ 	
		new item_id = entity_get_int(ptr, EV_INT_iuser4)

		if(Get_Player_Potions(ptd) < MAX_PLAYER_POTIONS)
		{
			client_printcolor(ptd, "/g撿到了/ctr%s!", Potion_Names[item_id]);

			g_iPlayerPotions[ptd][g_CurrentChar[ptd]][item_id]++;

			remove_entity(ptr);
		}
	}
}

public Remove_All_Potion_Ents()
{
	new item_ent = find_ent_by_class(-1, "PotionsDeath")
	
	while ( item_ent ) 
	{
		remove_entity(item_ent)
		item_ent = find_ent_by_class(item_ent, "PotionsDeath")
	}
}

// 玩家視角.
Create_PlayerCamera( id ) 
{ 
	if ( !is_user_alive(id) )
		return;

	new iEnt = g_iCam[id]

	if( !is_valid_ent(iEnt) )
	{
		iEnt = create_entity("trigger_camera")

		entity_set_int(iEnt, EV_INT_spawnflags, SF_CAMERA_PLAYER_TARGET|SF_CAMERA_PLAYER_POSITION)

		DispatchSpawn(iEnt)

		set_pdata_float(iEnt, m_flWait, 999999.0, 4)

		g_iCam[id] = iEnt
	}

	ExecuteHam( Ham_Use, iEnt, id, id, USE_OFF, 1.0);
	ExecuteHam( Ham_Use, iEnt, id, id, USE_SET, 1.0);

} 
public PlayerCamera_Think( entid )
{
	new id = get_cam_owner(entid);
	if( !id )
	{
		return
	}
    
	new Float:origin[3], Float:angle[3], Float:vBack[3];
	entity_get_vector( id, EV_VEC_origin, origin );
	entity_get_vector( id, EV_VEC_v_angle, angle );

	angle_vector( angle, ANGLEVECTOR_FORWARD, vBack );

	origin[2] += 20.0; // 我們正在接近視角.
    
	// 視角往後150個單位.
	origin[0] += (-vBack[0] * 150.0);
	origin[1] += (-vBack[1] * 150.0);
	origin[2] += (-vBack[2] * 150.0);

	entity_set_origin( entid, origin );
	entity_set_vector( entid, EV_VEC_angles, angle );   
    
	// 只有思考.
	entity_set_float( entid, EV_FL_nextthink, get_gametime() );
}

// 弓箭.
public act_shoot(Param[], id)
{
	id -= TASKID_CROSSBOW;

	new item_id = Param[0];

	g_Shooting[id] = false;

	new crossbow_ent = create_entity("info_target");
	
	if ( !pev_valid(crossbow_ent) || !is_user_alive(id) )
		return;
	
	fm_set_animation(id, 5);

	g_iPlayerItemBolts[id][g_CurrentChar[id]][item_id]--;

	if ( item_data[item_id][CHAR_TYPE] == TYPE_BOLTS && g_iPlayerItemBolts[id][g_CurrentChar[id]][item_id] < 1 )
	{
		g_iPlayerItem[id][g_CurrentChar[id]][item_id]--;

		if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] < 1 )
		{
			g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] = ITEM_NOT_WORN;
		}
	}

	set_pev(crossbow_ent ,pev_classname, "Crossbow")
	engfunc(EngFunc_SetModel, crossbow_ent, g_w_crossbow)
	
	set_pev( crossbow_ent, pev_owner, id)
	set_pev( crossbow_ent, pev_movetype, MOVETYPE_FLY)
	set_pev( crossbow_ent, pev_solid, SOLID_BBOX)
	
	set_pev( crossbow_ent, pev_mins, Float:{-2.0,-2.0,-2.0})
	set_pev( crossbow_ent, pev_maxs, Float:{2.0,2.0,2.0})
	
	new Float:fAim[3],Float:fAngles[3],Float:fOrigin[3]
	velocity_by_aim(id,64,fAim)
	vector_to_angle(fAim,fAngles)
	pev(id,pev_origin,fOrigin)
	
	fOrigin[0] += fAim[0]
	fOrigin[1] += fAim[1]
	fOrigin[2] += fAim[2]
	
	set_pev(crossbow_ent,pev_origin,fOrigin)
	set_pev(crossbow_ent,pev_angles,fAngles)
	
	new Float:fVel[3]
	velocity_by_aim(id, 1400, fVel)	
	set_pev(crossbow_ent,pev_velocity,fVel)

	ExecuteForward( g_ActRangedShoot, g_iReturn, id, crossbow_ent );
	
	emit_sound(id, CHAN_ITEM, g_crossbow_shoot_sound, 1.0, ATTN_NORM, 0, PITCH_NORM);
	set_task(0.5, "crossbow_set_back_model", id);
}
public crossbow_set_back_model(id)
{
	if ( is_user_alive(id) )
	{
		new model[33];
		pev(id, pev_viewmodel2, model, 32);
		
		if (equali(model, g_v_crossbow))
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
	}
}
public AddFakeClient()
{      
	if( g_iFakeplayer == -1 )
		return;
		
	new szName[33];
	formatex(szName , charsmax(szName) , Diablo2_Fake_Player_Name );


	new g_BotID = engfunc(EngFunc_CreateFakeClient, szName)

	if ( !pev_valid( g_BotID ) )
		return;
			
	engfunc(EngFunc_FreeEntPrivateData, g_BotID )

	set_pev( g_BotID, pev_flags, pev( g_BotID, pev_flags ) | FL_FAKECLIENT );

	static szRejectReason[128]
	dllfunc(DLLFunc_ClientConnect, g_BotID , szName, "127.0.0.1", szRejectReason)

	dllfunc( DLLFunc_ClientPutInServer, g_BotID);

	cs_set_user_team( g_BotID, CS_TEAM_CT );
	ExecuteHamB( Ham_CS_RoundRespawn, g_BotID );
					
	set_pev( g_BotID, pev_effects, pev( g_BotID, pev_effects ) | EF_NODRAW );
	set_pev( g_BotID, pev_solid, SOLID_NOT );
	dllfunc( DLLFunc_Think, g_BotID );
		
	g_iFakeplayer = g_BotID;
} 