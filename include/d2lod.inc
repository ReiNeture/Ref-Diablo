// Diablo II LOD 板塊檔案 //
#if defined _diablo2LOD_included
  #endinput
#endif
#define _diablo2LOD_included

#define MAX_P_SKILLS 20

// 獲得最大技能數並且讀取.
native MAX_SKILLS_ACTIVE();
// 獲得最大島具數並且讀取.
native MAX_ITEMS_ACTIVE();

// 職業類型.
enum
{
	AMAZON = 0,
	ASSASSIN,
	NECROMANCER,
	BARBARIAN,
	PALADIN,
	DRUID,
	SORCERESS
}

// 登入值.
enum
{
	UNLOGGED = 0,
	LOGGED
}

// 顯示值.
enum
{
	NOT_DISPLAY = 0,
	DISPLAY
}

// 道具類型.
enum
{
	TYPE_GLOVES = 0,
	TYPE_BOOTS,
	TYPE_BELT,
	TYPE_ARMOR,
	TYPE_HELM,
	TYPE_SHIELD,
	TYPE_WEAPON,
	TYPE_RING,
	TYPE_AMULET,
	TYPE_BOLTS
}

// 按E後呼叫這個Forward.
forward d2_skill_selected(id, skill_id);

// 使用技能後呼叫這個Forward.
forward d2_skill_fired(id);

// 攻擊玩家時改變傷害,第一個參數是受害者,第二個參數是攻擊者,第三個參數是傷害.
forward d2_takedamage(victim, attacker, Float:iDamage[1]);

// 用技能攻擊玩家時改變傷害,第一個參數是受害者,第二個參數是攻擊者,第三個參數是傷害.
forward d2_skill_takedamage(victim, attacker, Float:iDamage[1]);

// 用毒攻擊玩家時改變傷害,第一個參數是受害者,第二個參數是攻擊者,第三個參數是傷害.
forward d2_dagger_poisondamage(victim, attacker, Float:iDamage[1]);

// 用遠距離攻擊玩家時改變傷害,第一個參數是受害者,第二個參數是攻擊者,第三個參數是傷害.
forward d2_ranged_takedamage(victim, attacker, Float:iDamage[1]);

// 用火焰箭失攻擊玩家時呼叫這個Forward,第一個參數是玩家,第二個參數是實體物件.
forward d2_ranged_actshoot(id, entity);

// 玩家登入/登出時呼叫這個Forward.
forward d2_logged(id, log_type);

// 獲得玩家目前速度.
native Float:get_current_speed(id);

// 獲得玩家是否登入.
native get_player_logged(id);

// 獲得玩家的最大血量值.
native get_p_maxhealth(id);

// 設定玩家的最大血量值.
native set_p_maxhealth(id, value);

// 設定玩家的經驗.
native set_p_xp(id, value);

// 獲得玩家的經驗.
native get_p_xp(id);

// 獲得玩家的職業.
native get_p_hero(id);

// 獲得玩家的等級.
native get_p_level(id);

// 設定玩家的能量.
native set_p_mana(id, value);

// 獲得玩家的能量.
native get_p_mana(id);

// 獲得玩家的體力.
native get_p_vitality(id);

// 設定玩家的體力.
native set_p_vitality(id, value);

// 設定玩家的錢.
native set_p_gold(id, value);

// 設定玩家倉庫的錢.
native set_p_gold_inventory(id, value);

// 獲得玩家的錢.
native get_p_gold(id);

// 獲得玩家倉庫的錢.
native get_p_gold_inventory(id);

// 獲得玩家技能的值.
native get_p_skill(id, skill_id);

// 重置玩家模組.
native reset_p_model(id);

// 獲得玩家的道具數量.
native get_p_item_count(id, item_id);

// 獲得玩家的裝備穿著狀況.
native get_p_item_is_worn(id, item_id);

// 獲得玩家的裝備類型.
native bool:get_p_item_wear_type(id, type);

// 獲得玩家的裝備資料.
native bool:get_p_item_data(id, data);

// 判斷玩家是否在安全區.
native bool:get_p_in_safezone(id);

// 判斷玩家是否在保護時間.
native bool:is_p_protected(id);

// 檢查時間是不是凍結.
native is_freezetime();

// 檢查玩家是否靠近怪物
native bool:IsPlayerNearByMonster(id);

// 取得角色魔力點數
native get_p_magic(id);

// 取得裝備傷害加總
native Float:get_totaldmg_of_item(id);

// BP設定
native get_p_bosspoint(id);
native set_p_bosspoint(id, value);

// 其他物品設定
native get_p_drops(id, drops_id);
native set_p_drops(id, drops_id, value);
native get_drops_name(drops_id, szItems[], maxsize);

// 對玩家造成傷害,第一個參數是受害者,第二個參數是攻擊者,第三個參數是傷害,第四個參數是攻擊類型.
native dmg_kill_player(id, attacker, Float:damage, weaponDescription[])

// 讓特定物體掉落錢,第一個參數是物體,第二個參數是錢的類型,第三個參數是數值.
native drop_coins(victim, classname[], goldvalue)

stock find_itemplugin()
{
	for(new i = 0; i < get_pluginsnum(); ++i)
	{
		new temp[2], name[64]
		get_plugin(i, name, 63, temp, 1, temp, 1, temp, 1, temp, 1)
 		if(equali(name, "diablo2LOD.amxx")) 
		{
 			return i;
		}
	}

 	return -1;
}

stock find_itemindex()
{
	new temp[2], name[64], pluginname[64]
	get_plugin(-1, pluginname, 63, temp, 1, temp, 1, temp, 1, temp, 1)
	for (new i = 0; i < get_pluginsnum(); ++i)
	{
		get_plugin(i, name, 63, temp, 1, temp, 1, temp, 1, temp, 1)
 		if(equal(name, pluginname))
		{
 			return i
		}
	}

 	return -1
}
stock register_d2_skill(skill_name[], skill_desc[], skill_hero, skill_level, skill_max, skill_display)
{
	new SkillId = find_itemindex()
	new SkillPlugin = find_itemplugin()
	new SkillRegFunc = get_func_id("register_skill", SkillPlugin)

	new temp = callfunc_begin_i(SkillRegFunc, SkillPlugin)
	if(temp == -1 || temp == -2)
	{
		log_amx("找不到插件或是插件無效!")
		return PLUGIN_HANDLED;		
	}

	callfunc_push_int(SkillId)
	callfunc_push_str(skill_name)
	callfunc_push_str(skill_desc)
	callfunc_push_int(skill_hero)
	callfunc_push_int(skill_level)
	callfunc_push_int(skill_max)
	callfunc_push_int(skill_display)
	
	temp = callfunc_end()
	if(temp == -1 || temp == -2) 
	{
		return PLUGIN_HANDLED;
	}

	return temp;
}
stock client_printcolor(const id, const input[], any:...)
{
	new count = 1, players[32];

	static msg[191];
	vformat(msg,190,input,3);

	replace_all(msg,190,"/g","^4");// 綠色文字
	replace_all(msg,190,"/y","^1");// 橘色文字
	replace_all(msg,190,"/ctr","^3");// 隊伍顏色文字

	if (id) players[0] = id; 
	else get_players(players,count,"ch");

	for (new i=0;i<count;i++)
	{
		if (is_user_connected(players[i]))
		{
			message_begin(MSG_ONE_UNRELIABLE, get_user_msgid("SayText"), _, players[i]);
			write_byte(players[i]);
			write_string(msg);
			message_end();
		}
	}
}