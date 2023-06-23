#include <amxmodx>
#include <amxmisc>
#include <fun>
#include <cstrike>
#include <hamsandwich>
#include <fakemeta>
#include <engine>
#include <nvault>
#include <sqlx>
#include <xs>

#define VERSION	"1.9"

// Main includes
#include "diablo2LOD/const.inl"
#include "diablo2LOD/vars.inl"
#include "diablo2LOD/nvault.inl"
#include "diablo2LOD/events.inl"
#include "diablo2LOD/forwards.inl"
#include "diablo2LOD/cvars.inl"
#include "diablo2LOD/messages.inl"
#include "diablo2LOD/menu.inl"
#include "diablo2LOD/motd.inl"
#include "diablo2LOD/public.inl"
#include "diablo2LOD/entity.inl"
#include "diablo2LOD/commands.inl"
#include "diablo2LOD/items.inl"
#include "diablo2LOD/classes.inl"
#include "diablo2LOD/effects.inl"
#include "diablo2LOD/stocks.inl"
#include "diablo2LOD/mysql.inl"
#include "diablo2LOD/calc.inl"

public plugin_init()
{
	register_plugin("Diablo II LOD", VERSION, "xbatista");
	
	register_cvar("d2lod_version", VERSION, FCVAR_SPONLY|FCVAR_SERVER)
	set_cvar_string("d2lod_version", VERSION)

	// 讀取所有Cvar.
	LoadCvars();
	
	// Events.
	register_event("HLTV", "Event_New_Round", "a", "1=0", "2=0");
	register_event("CurWeapon", "Ev_CurWeapon", "be", "1=1");
	register_event( "TeamInfo", "Ev_JoinTeam", "a")
	register_logevent("Logevent_Round_Start", 2, "1=Round_Start");
	register_logevent("Event_Round_End", 2, "1=Round_End");

	register_touch("Coins", "player", "Coins_Pickup")
	register_touch("ItemsDeath", "player", "Item_Touch")
	register_touch("PotionsDeath", "player", "Potion_Touch")

	register_think("trigger_camera", "PlayerCamera_Think")

	// Forwards.
	RegisterHam(Ham_TakeDamage, "player", "fwd_PlayerDamaged");
	RegisterHam(Ham_TakeDamage, "func_wall", "fwd_PlayerDamagedMonster");
	RegisterHam(Ham_Spawn, "player", "fwd_PlayerSpawn", 1);
	RegisterHam(Ham_TraceAttack, "player", "fwd_TraceAttack");
	RegisterHam(Ham_Killed, "player", "fwd_Killed");
	RegisterHam(Ham_Killed, "player", "fwd_KilledPost", 1);
	RegisterHam(Ham_Touch, "weaponbox", "Fwd_Touch");
	RegisterHam(Ham_Touch, "armoury_entity", "Fwd_Touch");
	RegisterHam( Ham_Weapon_PrimaryAttack, "weapon_knife", "fwd_AttackSpeed" , 1 );
	RegisterHam( Ham_Item_Deploy , "weapon_knife", "fwd_AttackSpeed", 1);
	new szWeaponName[ 24 ];
	for ( new WeaponId = 1; WeaponId <= 30; WeaponId++ )
	{
		if ( get_weaponname ( WeaponId, szWeaponName, charsmax ( szWeaponName ) ) )
		{
			RegisterHam( Ham_CS_Item_GetMaxSpeed , szWeaponName, "fwd_ItemGetMaxSpeed", 1 );
		}
	}
	
	register_forward(FM_ClientKill, "fwd_ClientKill");
	register_forward(FM_CmdStart, "fwd_CmdStart");
	register_forward(FM_GetGameDescription,	"fwd_GameDescription");
	register_forward(FM_Touch, "Entity_Touched");
	register_forward(FM_SetModel, "SetModel_Post", 1)

	// 獲得/註冊訊息.
	MsgSayText = get_user_msgid("SayText");
	MsgHideWeapon	= get_user_msgid("HideWeapon")
	MsgHideCrosshair = get_user_msgid("Crosshair")
	gmsgClCorpse = get_user_msgid("ClCorpse")

	register_message(get_user_msgid("StatusIcon"),	"Message_StatusIcon")
	register_message(get_user_msgid("VGUIMenu"), "Message_VGUIMenu")

	g_iItemTook = CreateMultiForward("D2_Item_Gone", ET_IGNORE, FP_CELL, FP_CELL);
	g_DamagedbySkill = CreateMultiForward("d2_skill_takedamage", ET_IGNORE, FP_CELL, FP_CELL, FP_ARRAY);
	g_SkillSelected = CreateMultiForward("d2_skill_selected", ET_IGNORE, FP_CELL, FP_CELL);
	g_SkillFired = CreateMultiForward("d2_skill_fired", ET_IGNORE, FP_CELL);
	g_TakeDamage = CreateMultiForward("d2_takedamage", ET_IGNORE, FP_CELL, FP_CELL, FP_ARRAY);
	g_iLog = CreateMultiForward("d2_logged", ET_IGNORE, FP_CELL, FP_CELL);
	g_RangedDamage = CreateMultiForward("d2_ranged_takedamage", ET_IGNORE, FP_CELL, FP_CELL, FP_ARRAY);
	g_DaggerPoisonDamage = CreateMultiForward("d2_dagger_poisondamage", ET_IGNORE, FP_CELL, FP_CELL, FP_ARRAY);
	g_ActRangedShoot = CreateMultiForward("d2_ranged_actshoot", ET_IGNORE, FP_CELL, FP_CELL);
	
	gBuyCharacterMenu = B1 | B2 | B3 | B4 | B5 | B6 | B7 | B8 | B9 | B0;
	gMakeItemMenu = B8;
	register_menucmd(register_menuid("buy_chisato_menu"), gBuyCharacterMenu, "buy_chisato_menu");
	register_menucmd(register_menuid("buy_grasswonder_menu"), gBuyCharacterMenu, "buy_grasswonder_menu");
	register_menucmd(register_menuid("ResetApspMenu"), gBuyCharacterMenu, "reset_apsp_menu");

	register_menucmd(register_menuid("make_refknife_menu"), gMakeItemMenu, "make_refknife_menu");
	register_menucmd(register_menuid("make_karambit_menu"), gMakeItemMenu, "make_karambit_menu");
	register_menucmd(register_menuid("make_karambit_hardened_menu"), gMakeItemMenu, "make_karambit_hardened_menu");
	
	// 其他.
	g_iMaxPlayers = get_maxplayers();
	g_SyncHudCreate = CreateHudSyncObj();
	g_SyncHudCreate_Avade = CreateHudSyncObj();
	g_SyncHudCreate_Dmg = CreateHudSyncObj();

	if ( get_pcvar_num(d2_advert) )
	{
		set_task( get_pcvar_float(d2_advert_time), "Adv_Message", 0, _, _, "b");
	}

	if ( get_pcvar_num(d2_save_type) )
	{
		// 連線到資料庫.
		set_task( 1.0, "MYSQLX_Init");
	}
	
	new Ent, Count;
		
	while( ( Ent = find_ent_by_class( Ent, "info_player_start" ) ) > 0 )
		if( Count++ > 1 )
			break;
		
	if( Count <= 1 )
		g_iFakeplayer = -1;

	set_task( 2.0, "AddFakeClient" );

	set_task( 1.0, "Spawn_Items");
	set_task( 1.0, "Spawn_Items_Charsi")
	set_task( 1.0, "Spawn_Items_Akara")
	set_task( 1.0, "Spawn_Items_Miyu")

	g_FakeEnt = create_entity( "info_target" );
	register_clcmd("getr", "getr");
}
public getr(id)
{
	for (new skill_id = 0; skill_id <= g_skillcounter; skill_id++)
	{
		g_iSkills[id][g_CurrentChar[id]][skill_id] = g_skillmax[skill_id];
	}
}

public plugin_natives()
{
	register_native("get_current_speed", "native_get_current_speed", 1);
	register_native("get_player_logged", "native_get_player_logged", 1);
	register_native("set_p_xp", "Set_Player_Xp", 1);
	register_native("get_p_xp", "native_get_p_xp", 1);
	register_native("get_p_hero", "native_get_p_hero", 1);
	register_native("get_p_level", "native_get_p_level", 1);
	register_native("set_p_mana", "Set_Player_Mana", 1);
	register_native("get_p_mana", "native_get_p_mana", 1);
	register_native("get_p_vitality", "native_get_p_vitality", 1);
	register_native("set_p_vitality", "native_set_p_vitality", 1);
	register_native("set_p_gold", "Set_Player_Coins", 1);
	register_native("set_p_gold_inventory", "Set_Player_CoinsInv", 1);
	register_native("get_p_gold_inventory", "native_get_p_gold_inventory", 1);
	register_native("get_p_gold", "native_get_p_gold", 1);
	register_native("get_p_item_wear_type", "Player_WItemType", 1);
	register_native("get_p_item_wear_data", "Player_WItemDataActive", 1);
	register_native("MAX_SKILLS_ACTIVE", "native_MAX_SKILLS_ACTIVE", 1);
	register_native("MAX_ITEMS_ACTIVE", "native_MAX_ITEMS_ACTIVE", 1);
	register_native("get_p_in_safezone", "native_get_p_in_safezone", 1);
	register_native("is_p_protected", "native_is_p_protected", 1);
	register_native("dmg_kill_player", "Native_Dmg_Kill_Player");
	register_native("get_p_skill", "native_get_p_skill", 1);
	register_native("is_freezetime", "native_is_freezetime", 1);
	register_native("reset_p_model", "Set_Custom_Model", 1);
	register_native("get_p_item_count", "native_get_p_item_count", 1);
	register_native("get_p_item_is_worn", "native_get_p_item_is_worn", 1);
	register_native("get_p_maxhealth", "native_get_p_maxhealth", 1);
	register_native("set_p_maxhealth", "native_set_p_maxhealth", 1);
	register_native("IsPlayerNearByMonster", "native_get_p_near_monster", 1);
	register_native("drop_coins", "Native_Create_Coins");
	register_native("get_p_magic", "native_get_p_magic", 1);
	register_native("get_p_strength", "native_get_p_strength", 1);
	register_native("get_totaldmg_of_item", "get_totaldmg_of_item", 1);
	register_native("get_p_bosspoint", "native_get_p_bosspoint", 1);
	register_native("set_p_bosspoint", "Set_Player_Bosspoint", 1);
	register_native("set_p_drops", "native_set_p_drops", 1);
	register_native("get_p_drops", "native_get_p_drops", 1);
	register_native("get_drops_name", "native_get_drops_name", 0);
}

public client_connect(id)
{
	set_user_info(id, "_vgui_menus", "1");

	client_cmd(id, "cl_corpsestay %f", get_pcvar_float(d2_cl_corpsestay) );

	g_Player_FirstTime_Connected[id] = true;

	// 重置所有玩家資料.
	Reset_Data(id);
	
	// 讀取所有玩家資料.
	if ( !is_user_bot(id) && g_iFakeplayer != id )
	{
		if ( !get_pcvar_num(d2_save_type) )
		{
			Load_Data(id);
		}
		else
		{
			set_task( 1.0, "MYSQLX_GetAllData", id);
		}
	}
}
public client_disconnect(id)
{
	End_poison_damage(id);

	// 刪除一些任務.
	remove_task( id + TASKID_FIRSTCONNECT );

	new iEnt = g_iCam[id]
	if( !is_valid_ent(iEnt) )
	{
		g_iCam[id] = 0
		// remove_entity(iEnt)
	}

	// 回覆一些值.
	g_Player_FirstTime_Connected[id] = false;
	g_Player_Info[id] = false;
	g_iLogged[id] = false;

	if( g_iFakeplayer == id ) 
	{
		g_iFakeplayer = 0;
		set_task( 1.5, "AddFakeClient" );
	}

	// 回覆模組.
	g_szModel[id][0] = 0
	Set_Plugin_State(id, false)

	// 儲存所有玩家資料.
	if ( !is_user_bot(id) && g_iFakeplayer != id )
	{
		if ( !get_pcvar_num(d2_save_type) )
		{
			Save_Data( id );
		}
		else
		{

			MYSQLX_Save_T( id );
		}
	}
}

public plugin_precache()  
{
	LoadIniItems();

	engfunc(EngFunc_PrecacheSound, LevelUp);
	engfunc(EngFunc_PrecacheSound, g_crossbow_shoot_sound);
	engfunc(EngFunc_PrecacheSound, h_sound);

	engfunc(EngFunc_PrecacheModel, "models/rpgrocket.mdl");

	engfunc(EngFunc_PrecacheModel, g_w_mp);
	engfunc(EngFunc_PrecacheModel, g_w_hp);
	engfunc(EngFunc_PrecacheModel, g_w_coins);
	engfunc(EngFunc_PrecacheModel, g_w_item);
	engfunc(EngFunc_PrecacheModel, g_w_inventory);
	engfunc(EngFunc_PrecacheModel, g_w_charsi);
	engfunc(EngFunc_PrecacheModel, g_w_akara);
	engfunc(EngFunc_PrecacheModel, g_w_miyu);
	engfunc(EngFunc_PrecacheModel, g_brassknuckles);

	engfunc(EngFunc_PrecacheModel, g_w_crossbow);
	engfunc(EngFunc_PrecacheModel, g_v_crossbow);
	engfunc(EngFunc_PrecacheModel, g_p_crossbow);

	// 清除所有實體.
	register_forward(FM_Spawn, "fwd_Remove_Ents");

	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		if ( item_data[item_id][CHAR_TYPE] != TYPE_WEAPON )
			continue;
		
		if ( equal(item_vmodel[item_id], "models/", 7) )
		{
			engfunc(EngFunc_PrecacheModel, item_vmodel[item_id]);
		}

		if ( equal(item_pmodel[item_id], "models/", 7) )
		{
			engfunc(EngFunc_PrecacheModel, item_pmodel[item_id]);
		}
	}

	new Mdl_Path[100];

	for(new i = 0; i < MAX_HEROES; i++)
	{
		formatex( Mdl_Path, sizeof Mdl_Path - 1, "models/player/%s/%s.mdl", Custom_Models[i], Custom_Models[i])

		engfunc(EngFunc_PrecacheModel, Mdl_Path);

		formatex( Mdl_Path, sizeof Mdl_Path - 1, "models/player/%s/%sT.mdl", Custom_Models[i], Custom_Models[i])
		if ( file_exists( Mdl_Path ) )
		{
			engfunc(EngFunc_PrecacheModel, Mdl_Path);
		}

	}
}

public plugin_cfg()
{
	get_configsdir(g_ConfigsDir, 63);
	formatex(g_ItemOriginDir, 63, "%s/%s", g_ConfigsDir, inventory_dir_name);

	new File[192];
	
	formatex( File, charsmax( File ), "%s/d2conf.cfg", g_ConfigsDir );
	
	if( !file_exists( File ) )
	{
		server_print( "檔案 %s 不存在!", File );
		write_file( File, " ", -1 );
	}
	else
	{	
		server_print( "%s 成功讀取.", File );
		server_cmd( "exec %s", File );
	}

	if( !dir_exists(g_ItemOriginDir) ) 
	{
		mkdir(g_ItemOriginDir);
	} 
	else 
	{
		new CurMap[32];
		get_mapname(CurMap, 31);
		Load_Origins(CurMap);
		Load_Origins_Charsi(CurMap);
		Load_Origins_Akara(CurMap);
		Load_Origins_Miyu(CurMap);
	}

	// 開啟Vault讀取程序.
	g_Nvault = nvault_open( "d2lod" );
	g_Nvault2 = nvault_open( "d2lod2" );
	g_Nvault3 = nvault_open( "d2lod3" );
	g_Nvault4 = nvault_open( "d2lod4" );
}

public plugin_end()
{
	// 換圖\伺服器關閉\伺服器重啟時,關閉Vault讀取程序.
	nvault_close( g_Nvault );
	nvault_close( g_Nvault2 );
	nvault_close( g_Nvault3 );
	nvault_close( g_Nvault4 );
}

public Float:native_get_current_speed(id)
{
	return Current_Speed[id];
}
public native_get_player_logged(id)
{
	return g_iLogged[id];
}
public native_get_p_xp(id)
{
	return g_PlayerXp[id][g_CurrentChar[id]];
}
public native_get_p_hero(id)
{
	return g_PlayerHero[id][g_CurrentChar[id]];
}
public native_get_p_level(id)
{
	return g_PlayerLevel[id][g_CurrentChar[id]];
}
public native_get_p_mana(id)
{
	return g_CurrentMana[id];
}
public native_get_p_vitality(id)
{
	return g_Vitality[id][g_CurrentChar[id]];
}
public native_set_p_vitality(id, value)
{
	g_Vitality[id][g_CurrentChar[id]] = value;
}
public native_get_p_gold_inventory(id)
{
	return g_CoinsInv[id][g_CurrentChar[id]];
}
public native_get_p_gold(id)
{
	return g_Coins[id][g_CurrentChar[id]];
}
public native_get_p_magic(id)
{
	return g_Energy[id][g_CurrentChar[id]];
}
public native_get_p_strength(id)
{
	return g_Strength[id][g_CurrentChar[id]];
}
public native_get_p_bosspoint(id)
{
	return g_Bosspoints[id][g_CurrentChar[id]];
}
public native_get_p_drops(id, drops_id)
{
	return g_iPlayerDrops[id][g_CurrentChar[id]][drops_id];
}
public native_set_p_drops(id, drops_id, value)
{
	if( -1 < drops_id < MAX_DROPS )
		g_iPlayerDrops[id][g_CurrentChar[id]][drops_id] = value;
}
public native_get_drops_name(drops_id, szItems[], maxsize)
{
	param_convert(2);
	formatex(szItems, maxsize, "%s", Drops_Name[drops_id]);
}

public native_MAX_SKILLS_ACTIVE()
{
	return g_skillcounter + 1;
}
public native_MAX_ITEMS_ACTIVE()
{
	return g_charcounter + 1;
}
public bool:native_get_p_in_safezone(id)
{
	if ( !is_user_alive(id) || DISTANCE_INVENTORY_PLAYER <= 0.0 ) return false;

	new Float:Porigin[3];
	entity_get_vector( id, EV_VEC_origin, Porigin);

	new ent = -1
	while( (ent = find_ent_in_sphere( ent, Porigin, DISTANCE_INVENTORY_PLAYER)) != 0) 
	{
		static classname[32];
		pev( ent, pev_classname, classname, charsmax(classname) );

		if( equal(classname, "Inventory") || equal(classname, "Charsi")
		|| equal(classname, "Akara") )
		{
			return true;
		}
	}

	return false;
}
public bool:native_get_p_near_charsi(id)
{
	if ( !is_user_alive(id) || DISTANCE_INVENTORY_PLAYER <= 0.0 ) return false;

	new Float:Porigin[3];
	entity_get_vector( id, EV_VEC_origin, Porigin);

	new ent = -1
	while( (ent = find_ent_in_sphere( ent, Porigin, DISTANCE_INVENTORY_PLAYER)) != 0) 
	{
		static classname[32];
		pev( ent, pev_classname, classname, charsmax(classname) );

		if( equal(classname, "Charsi") )
		{
			return true;
		}
	}

	return false;
}
public bool:native_get_p_near_akara(id)
{
	if ( !is_user_alive(id) || DISTANCE_INVENTORY_PLAYER <= 0.0 ) return false;

	new Float:Porigin[3];
	entity_get_vector( id, EV_VEC_origin, Porigin);

	new ent = -1
	while( (ent = find_ent_in_sphere( ent, Porigin, DISTANCE_INVENTORY_PLAYER)) != 0) 
	{
		static classname[32];
		pev( ent, pev_classname, classname, charsmax(classname) );

		if( equal(classname, "Akara") )
		{
			return true;
		}
	}

	return false;
}
public bool:native_get_p_near_miyu(id)
{
	if ( !is_user_alive(id) || DISTANCE_INVENTORY_PLAYER <= 0.0 ) return false;

	new Float:Porigin[3];
	entity_get_vector( id, EV_VEC_origin, Porigin);

	new ent = -1
	while( (ent = find_ent_in_sphere( ent, Porigin, DISTANCE_INVENTORY_PLAYER)) != 0) 
	{
		static classname[32];
		pev( ent, pev_classname, classname, charsmax(classname) );

		if( equal(classname, "Miyu") )
		{
			return true;
		}
	}

	return false;
}
public bool:native_get_p_near_inventory(id)
{
	if ( !is_user_alive(id) || DISTANCE_INVENTORY_PLAYER <= 0.0 ) return false;

	new Float:Porigin[3];
	entity_get_vector( id, EV_VEC_origin, Porigin);

	new ent = -1
	while( (ent = find_ent_in_sphere( ent, Porigin, DISTANCE_INVENTORY_PLAYER)) != 0) 
	{
		static classname[32];
		pev( ent, pev_classname, classname, charsmax(classname) );

		if( equal(classname, "Inventory") )
		{
			return true;
		}
	}

	return false;
}
public bool:native_is_p_protected(id)
{
	if ( g_PlayerProtected[id] )
		return true;
	
	return false;
}
public native_get_p_skill(id, skill_id)
{
	return g_iSkills[id][g_CurrentChar[id]][skill_id];
}
public native_is_freezetime()
{
	return g_FreezeTime;
}
public native_get_p_item_count(id, item_id)
{
	return g_iPlayerItem[id][g_CurrentChar[id]][item_id];
}
public native_get_p_item_is_worn(id, item_id)
{
	return g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id];
}
public native_get_p_maxhealth(id)
{
	return g_MaxHealth[id];
}
public native_set_p_maxhealth(id, value)
{
	g_MaxHealth[id] = value;

	if ( g_MaxHealth[id] < 0 )
	{
		g_MaxHealth[id] = 0;
	}
}
// 檢查玩家是否靠近怪物.
public bool:native_get_p_near_monster(id)
{
	if ( !is_user_alive(id) || DISTANCE_NEAR_MONSTER <= 0.0 ) return false;

	new Float:Porigin[3];
	entity_get_vector( id, EV_VEC_origin, Porigin);

	new ent = -1
	while( (ent = find_ent_in_sphere( ent, Porigin, DISTANCE_NEAR_MONSTER)) != 0) 
	{
		if ( pev( ent, pev_flags ) & FL_MONSTER )
		{
			return true;
		}
	}

	return false;
}