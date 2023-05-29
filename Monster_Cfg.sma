#include <amxmodx>

new const PLUGIN[] = "怪物座標創造"
new const VERSION[] = "3.0"
new const AUTHOR[] = "Timmi the savage"

new g_szMonName[16], g_iDelay, g_iAmount, g_iAngle, g_iAngleMax, g_iAngleMin

public plugin_init()
{
	register_plugin(PLUGIN, VERSION, AUTHOR)
	register_clcmd("monster_cfg", "MonsterNameMenu")
}

public MonsterNameMenu(id)
{
	new iNameMenu = menu_create("\y選擇你要新增的怪物", "NameMenu_Handle")
	
	menu_additem(iNameMenu, "\w異型戰士", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w直昇機", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w藤壺怪", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w警官", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w警官", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w大媽", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\wBloater", "", ADMIN_MENU)
	
	menu_additem(iNameMenu, "\w鱷魚", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w首腦", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w巨人", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w小型巨人", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\wG-Man", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w人類刺客", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w食腦蟲", "", ADMIN_MENU)
	
	menu_additem(iNameMenu, "\w人類戰士", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w百目狗", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w魚龍", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w弗地剛人", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w水蛭", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w科學家", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w聖甲蟲", "", ADMIN_MENU)
	
	menu_additem(iNameMenu, "\w鷹爪", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w殭屍", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w殭屍王", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w老鼠", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\wRoach", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w人類戰士", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w軍校軍官", "", ADMIN_MENU)
	
	menu_additem(iNameMenu, "\w警官屍體", "", ADMIN_MENU)
	menu_additem(iNameMenu, "\w人類戰士屍體", "", ADMIN_MENU)
	
	menu_setprop(iNameMenu, MPROP_EXIT, MEXIT_ALL)
	
	menu_display(id, iNameMenu, 0)
	
	return PLUGIN_HANDLED
}

public NameMenu_Handle(id, this, iNameItem)
{
	if (iNameItem == MENU_EXIT)
	{
		menu_destroy(this)
		return PLUGIN_HANDLED
	}
	
	switch(iNameItem)
	{
		case 0: g_szMonName = "agrunt"
		case 1: g_szMonName = "apache"
		case 2: g_szMonName = "barnacle"
		case 3: g_szMonName = "barney"
		case 4: g_szMonName = "otis"
		case 5: g_szMonName = "bigmomma"
		case 6: g_szMonName = "bloater"
		case 7: g_szMonName = "bullsquid"
		case 8: g_szMonName = "controller"
		case 9: g_szMonName = "gargantua"
		case 10: g_szMonName = "babygarg"
		case 11: g_szMonName = "gman"
		case 12: g_szMonName = "hassassin"
		case 13: g_szMonName = "headcrab"
		case 14: g_szMonName = "hgrunt"
		case 15: g_szMonName = "houndeye"
		case 16: g_szMonName = "ichthyosaur"
		case 17: g_szMonName = "islave"
		case 18: g_szMonName = "leech"
		case 19: g_szMonName = "scientist"
		case 20: g_szMonName = "snark"
		case 21: g_szMonName = "tencacle"
		case 22: g_szMonName = "zombie"
		case 23: g_szMonName = "gonome"
		case 24: g_szMonName = "rat"
		case 25: g_szMonName = "roach"
		case 26: g_szMonName = "massn"
		case 27: g_szMonName = "gruntcmdr"
		case 28: g_szMonName = "barneydead"
		case 29: g_szMonName = "hgruntdead"
	}
	
	menu_destroy(this)
	client_print(id, print_chat, "[AMXX] 怪物名稱: %s", g_szMonName)
	MonsterAmountMenu(id)
	
	return PLUGIN_HANDLED
}

public MonsterAmountMenu(id)
{
	new iAmountMenu = menu_create("\y重生幾隻", "AmountMenu_Handle")
	
	menu_additem(iAmountMenu, "\w1", "", ADMIN_MENU)
	menu_additem(iAmountMenu, "\w2", "", ADMIN_MENU)
	menu_additem(iAmountMenu, "\w3", "", ADMIN_MENU)
	menu_additem(iAmountMenu, "\w4", "", ADMIN_MENU)
	menu_additem(iAmountMenu, "\w5", "", ADMIN_MENU)
	menu_additem(iAmountMenu, "\w6", "", ADMIN_MENU)
	menu_additem(iAmountMenu, "\w7", "", ADMIN_MENU)
	
	menu_setprop(iAmountMenu, MPROP_EXIT, MEXIT_ALL)
	
	menu_display(id, iAmountMenu, 0)
	
	return PLUGIN_HANDLED
}

public AmountMenu_Handle(id, this, iAmountItem)
{
	if (iAmountItem == MENU_EXIT)
	{
		menu_destroy(this)
		return PLUGIN_HANDLED
	}
	
	switch(iAmountItem)
	{
		case 0: g_iAmount = 1
		case 1: g_iAmount = 2
		case 2: g_iAmount = 3
		case 3: g_iAmount = 4
		case 4: g_iAmount = 5
		case 5: g_iAmount = 6
		case 6: g_iAmount = 7
	}
	
	menu_destroy(this)
	client_print(id, print_chat, "[AMXX] 重生數量: %d", g_iAmount)
	MonsterDelayMenu(id)
	
	return PLUGIN_HANDLED
}

public MonsterDelayMenu(id)
{
	new iDelayMenu = menu_create("\y重生時間", "DelayMenu_Handle")
	
	menu_additem(iDelayMenu, "\w1", "", ADMIN_MENU)
	menu_additem(iDelayMenu, "\w5", "", ADMIN_MENU)
	menu_additem(iDelayMenu, "\w10", "", ADMIN_MENU)
	menu_additem(iDelayMenu, "\w15", "", ADMIN_MENU)
	menu_additem(iDelayMenu, "\w20", "", ADMIN_MENU)
	menu_additem(iDelayMenu, "\w25", "", ADMIN_MENU)
	menu_additem(iDelayMenu, "\w30", "", ADMIN_MENU)
	
	menu_additem(iDelayMenu, "\w40", "", ADMIN_MENU)
	menu_additem(iDelayMenu, "\w50", "", ADMIN_MENU)
	menu_additem(iDelayMenu, "\w60", "", ADMIN_MENU)
	menu_additem(iDelayMenu, "\w120", "", ADMIN_MENU)
	menu_additem(iDelayMenu, "\w180", "", ADMIN_MENU)
	menu_additem(iDelayMenu, "\w300", "", ADMIN_MENU)
	menu_additem(iDelayMenu, "\w3000", "", ADMIN_MENU)
	
	menu_setprop(iDelayMenu, MPROP_EXIT, MEXIT_ALL)
	
	menu_display(id, iDelayMenu, 0)
	
	return PLUGIN_HANDLED
}

public DelayMenu_Handle(id, this, iDelayItem)
{
	if (iDelayItem == MENU_EXIT)
	{
		menu_destroy(this)
		return PLUGIN_HANDLED
	}
	
	switch(iDelayItem)
	{
		case 0: g_iDelay = 1
		case 1: g_iDelay = 5
		case 2: g_iDelay = 10
		case 3: g_iDelay = 15
		case 4: g_iDelay = 20
		case 5: g_iDelay = 25
		case 6: g_iDelay = 30
		case 7: g_iDelay = 40
		case 8: g_iDelay = 50
		case 9: g_iDelay = 60
		case 10: g_iDelay = 120
		case 11: g_iDelay = 180
		case 12: g_iDelay = 300
		case 13: g_iDelay = 3000
	}
	
	menu_destroy(this)
	client_print(id, print_chat, "[AMXX] 重生時間: %d", g_iDelay)
	MonsterAngleMenu(id)
	
	return PLUGIN_HANDLED
}

public MonsterAngleMenu(id)
{
	new iAngleMenu = menu_create("\y怪物面向的角度", "AngleMenu_Handle")
	
	menu_additem(iAngleMenu, "\w15", "", ADMIN_MENU)
	menu_additem(iAngleMenu, "\w30", "", ADMIN_MENU)
	menu_additem(iAngleMenu, "\w45", "", ADMIN_MENU)
	menu_additem(iAngleMenu, "\w60", "", ADMIN_MENU)
	menu_additem(iAngleMenu, "\w90", "", ADMIN_MENU)
	menu_additem(iAngleMenu, "\w120", "", ADMIN_MENU)
	menu_additem(iAngleMenu, "\w150", "", ADMIN_MENU)
	
	menu_additem(iAngleMenu, "\w180", "", ADMIN_MENU)
	menu_additem(iAngleMenu, "\w210", "", ADMIN_MENU)
	menu_additem(iAngleMenu, "\w240", "", ADMIN_MENU)
	menu_additem(iAngleMenu, "\w270", "", ADMIN_MENU)
	menu_additem(iAngleMenu, "\w300", "", ADMIN_MENU)
	menu_additem(iAngleMenu, "\w330", "", ADMIN_MENU)
	menu_additem(iAngleMenu, "\w最大跟最小", "", ADMIN_MENU)
	
	menu_setprop(iAngleMenu, MPROP_EXIT, MEXIT_ALL)
	
	menu_display(id, iAngleMenu, 0)
	
	return PLUGIN_HANDLED
}

public AngleMenu_Handle(id, this, iAngleItem)
{
	if (iAngleItem == MENU_EXIT)
	{
		menu_destroy(this)
		return PLUGIN_HANDLED
	}
	
	switch(iAngleItem)
	{
		case 0: g_iAngle = 15
		case 1: g_iAngle = 30
		case 2: g_iAngle = 45
		case 3: g_iAngle = 60
		case 4: g_iAngle = 90
		case 5: g_iAngle = 120
		case 6: g_iAngle = 150
		case 7: g_iAngle = 180
		case 8: g_iAngle = 210
		case 9: g_iAngle = 240
		case 10: g_iAngle = 270
		case 11: g_iAngle = 300
		case 12: g_iAngle = 330
		case 13:
		{
			menu_destroy(this)
			MonsterAngleMaxMenu(id)
			return PLUGIN_HANDLED
		}
	}
	
	menu_destroy(this)
	client_print(id, print_chat, "[AMXX] 重生角度: %d", g_iAngle)
	MakeMonsterConfigFile(id)
	
	return PLUGIN_HANDLED
}

public MonsterAngleMaxMenu(id)
{
	new iMaxAngleMenu = menu_create("\y重生最大角度", "MaxAngleMenu_Handle")
	
	menu_additem(iMaxAngleMenu, "\w100", "", ADMIN_MENU)
	menu_additem(iMaxAngleMenu, "\w120", "", ADMIN_MENU)
	menu_additem(iMaxAngleMenu, "\w140", "", ADMIN_MENU)
	menu_additem(iMaxAngleMenu, "\w160", "", ADMIN_MENU)
	menu_additem(iMaxAngleMenu, "\w180", "", ADMIN_MENU)
	menu_additem(iMaxAngleMenu, "\w200", "", ADMIN_MENU)
	menu_additem(iMaxAngleMenu, "\w220", "", ADMIN_MENU)
	
	menu_additem(iMaxAngleMenu, "\w240", "", ADMIN_MENU)
	menu_additem(iMaxAngleMenu, "\w260", "", ADMIN_MENU)
	menu_additem(iMaxAngleMenu, "\w280", "", ADMIN_MENU)
	menu_additem(iMaxAngleMenu, "\w300", "", ADMIN_MENU)
	menu_additem(iMaxAngleMenu, "\w320", "", ADMIN_MENU)
	menu_additem(iMaxAngleMenu, "\w340", "", ADMIN_MENU)
	menu_additem(iMaxAngleMenu, "\w360", "", ADMIN_MENU)
	
	menu_setprop(iMaxAngleMenu, MPROP_EXIT, MEXIT_ALL)
	
	menu_display(id, iMaxAngleMenu, 0)
	
	return PLUGIN_HANDLED
}

public MaxAngleMenu_Handle(id, this, iMaxAngleItem)
{
	if (iMaxAngleItem == MENU_EXIT)
	{
		menu_destroy(this)
		return PLUGIN_HANDLED
	}
	
	switch(iMaxAngleItem)
	{
		case 0: g_iAngleMax = 100
		case 1: g_iAngleMax = 120
		case 2: g_iAngleMax = 140
		case 3: g_iAngleMax = 160
		case 4: g_iAngleMax = 180
		case 5: g_iAngleMax = 200
		case 6: g_iAngleMax = 220
		case 7: g_iAngleMax = 240
		case 8: g_iAngleMax = 260
		case 9: g_iAngleMax = 280
		case 10: g_iAngleMax = 300
		case 11: g_iAngleMax = 320
		case 12: g_iAngleMax = 340
		case 13: g_iAngleMax = 360
	}
	
	menu_destroy(this)
	client_print(id, print_chat, "[AMXX] 重生最大角度: %d", g_iAngleMax)
	MonsterAngleMinMenu(id)
	
	return PLUGIN_HANDLED
}

public MonsterAngleMinMenu(id)
{
	new iMinAngleMenu = menu_create("\y重生最小角度", "MinAngleMenu_Handle")
	
	menu_additem(iMinAngleMenu, "\w20", "", ADMIN_MENU)
	menu_additem(iMinAngleMenu, "\w40", "", ADMIN_MENU)
	menu_additem(iMinAngleMenu, "\w60", "", ADMIN_MENU)
	menu_additem(iMinAngleMenu, "\w80", "", ADMIN_MENU)
	menu_additem(iMinAngleMenu, "\w100", "", ADMIN_MENU)
	menu_additem(iMinAngleMenu, "\w120", "", ADMIN_MENU)
	menu_additem(iMinAngleMenu, "\w140", "", ADMIN_MENU)
	
	menu_additem(iMinAngleMenu, "\w160", "", ADMIN_MENU)
	menu_additem(iMinAngleMenu, "\w180", "", ADMIN_MENU)
	menu_additem(iMinAngleMenu, "\w200", "", ADMIN_MENU)
	menu_additem(iMinAngleMenu, "\w220", "", ADMIN_MENU)
	menu_additem(iMinAngleMenu, "\w240", "", ADMIN_MENU)
	menu_additem(iMinAngleMenu, "\w260", "", ADMIN_MENU)
	menu_additem(iMinAngleMenu, "\w280", "", ADMIN_MENU)
	
	menu_setprop(iMinAngleMenu, MPROP_EXIT, MEXIT_ALL)
	
	menu_display(id, iMinAngleMenu, 0)
	
	return PLUGIN_HANDLED
}

public MinAngleMenu_Handle(id, this, iMinAngleItem)
{
	if (iMinAngleItem == MENU_EXIT)
	{
		menu_destroy(this)
		return PLUGIN_HANDLED
	}
	
	switch(iMinAngleItem)
	{
		case 0: g_iAngleMin = 20
		case 1: g_iAngleMin = 40
		case 2: g_iAngleMin = 60
		case 3: g_iAngleMin = 80
		case 4: g_iAngleMin = 100
		case 5: g_iAngleMin = 120
		case 6: g_iAngleMin = 140
		case 7: g_iAngleMin = 160
		case 8: g_iAngleMin = 180
		case 9: g_iAngleMin = 200
		case 10: g_iAngleMin = 220
		case 11: g_iAngleMin = 240
		case 12: g_iAngleMin = 260
		case 13: g_iAngleMin = 280
	}
	
	menu_destroy(this)
	client_print(id, print_chat, "[AMXX] 重生最小角度: %d", g_iAngleMin)
	MakeMonsterConfigFile(id)
	
	return PLUGIN_HANDLED
}

public MakeMonsterConfigFile(id)
{
	static Data[64]
	static FilePath[64]
	static iOrigin[3]
	static szMapName[32]
	
	get_mapname(szMapName, charsmax(szMapName))
	format(FilePath, charsmax(FilePath), "/maps/%s_monster.cfg", szMapName)
	
	get_user_origin(id, iOrigin)
	format(Data, charsmax(Data), "{^norigin/%d %d %d^ndelay/%d", iOrigin[0], iOrigin[1], iOrigin[2], g_iDelay)
	write_file(FilePath, Data, -1)
	
	if (g_iAngle == 0)
	{
		format(Data, charsmax(Data), "angle_max/%d^nangle_min/%d", g_iAngleMax, g_iAngleMin)
		write_file(FilePath, Data, -1)
	}
	else
	{
		format(Data, charsmax(Data), "angle/%d", g_iAngle)
		write_file(FilePath, Data, -1)
		g_iAngle = 0
	}
	
	format(Data, charsmax(Data), "monster/%s^n}",g_szMonName)
	write_file(FilePath, Data, -1)
	
	set_hudmessage(75, 200, 200, -1.0, 0.86, 0, 6.0, 2.0, 0.1, 0.5, -1)
	show_hudmessage(id,"檔案成功寫入%s!", FilePath)
}
