// 這裡定義了所有Const.

/*================================================================================
[如果你不知道這裡的意思,就不要動他!!!]
=================================================================================*/
#define MAX_LEVELS 100
#define MAX_HEROES 7
#define MAX_MAPITEMS 500
#define MAX_ITEMS 50
#define MAX_POTIONS 9
#define WORN_TYPES 2
#define	MAX_SKILLS 20
#define MAX_PLUGIN_SKILLS 40 // 技能數量最大限制.
#define MAX_SKILL_NAME_SIZE 64
#define MAX_SKILL_DESC_SIZE 128
#define MAX_MODEL_LENGTH 32

#define ITEM_NOT_WORN 0
#define ITEM_WORN 1

#define CHAR_ACTIVE 1
#define CHAR_DELETED 0

#define UNLOGGED 0
#define LOGGED 1

#define PTS_HP_MINOR 0
#define PTS_HP_GREATER 1
#define PTS_HP_SUPER 2
#define PTS_MP_MINOR 3
#define PTS_MP_GREATER 4
#define PTS_MP_SUPER 5
#define PTS_RP_MINOR 6
#define PTS_RP_GREATER 7
#define PTS_RP_SUPER 8

#define TASKID_MANAREGEN 6666
#define TASKID_HUDINFO 7666
#define TASKID_FIRSTCONNECT 12444
#define TASKID_POISON 16000
#define TASKID_CROSSBOW 22000

// 射擊時間.
#define SHOOT_TIME 1

// 角色資料.
#define MAX_CHAR_DATA 16
#define CHAR_ARMOR 0
#define CHAR_DAMAGE 1
#define CHAR_COST 2
#define CHAR_DURABILITY 3
#define CHAR_STR 4
#define CHAR_DEX 5
#define CHAR_BLOCK 6
#define CHAR_LEVEL 7
#define CHAR_GIVEVIT 8
#define CHAR_GIVEENE 9
#define CHAR_TYPE 10
#define CHAR_HASP 11
#define CHAR_PDAMAGE 12
#define CHAR_PDURATION 13
#define CHAR_CROSSBOWDAMAGE 15

// 裝備類型.
#define TYPE_GLOVES 0
#define TYPE_BOOTS 1
#define TYPE_BELT 2
#define TYPE_ARMOR 3
#define TYPE_HELM 4
#define TYPE_SHIELD 5
#define TYPE_WEAPON 6
#define TYPE_RING 7
#define TYPE_AMULET 8
#define TYPE_BOLTS 9

#define PRESSED(%0) \
	(((button & (%0)) == (%0)) && ((oldbuttons & (%0)) != (%0)))
	
#define RELEASED(%0) \
	(((button & (%0)) != (%0)) && ((oldbuttons & (%0)) == (%0)))


new const gTableCreateQuery[] =
"						\
    CREATE TABLE IF NOT EXISTS `d2_data`	\
    (						\
        `player_steamid` VARCHAR(25) NOT NULL,	\
        `player_ip` VARCHAR(20) NOT NULL,     	\
        `player_name` VARCHAR(35) NOT NULL,   	\
        `player_data` VARCHAR(4096) NOT NULL, 	\
        `player_data2` VARCHAR(4096) NOT NULL,	\
        `player_data3` VARCHAR(4096) NOT NULL,	\
        PRIMARY KEY(`player_name`,`player_ip`,`player_steamid`),\
        KEY `player_data` (`player_data`),    	\
        KEY `player_data2` (`player_data2`),  	\
        KEY `player_data3` (`player_data3`)   	\
    ) TYPE=MyISAM; 				\
";
/*================================================================================
[結束定義.]
=================================================================================*/


/*================================================================================
[可編輯的定義.]
=================================================================================*/
#define DISTANCE_INVENTORY_PLAYER 300.0
#define DISTANCE_NEAR_MONSTER 300.0
#define DISTANCE_E_BUTTON_USE_ENT 70.0
#define MAX_PLAYER_ITEMS 7
#define MAX_INVENTORY_ITEMS 7
#define MAX_PLAYER_POTIONS 10
#define MAX_INVENTORY_POTIONS 10
#define MAX_CHARS 15
/*================================================================================
[結束定義.]
=================================================================================*/


/*================================================================================
[一些定義.]
=================================================================================*/
#define OFFSET_TEAM 114

#define fm_cs_set_user_nobuy(%1) set_pdata_int(%1, 235, get_pdata_int(%1, 235) & ~(1<<0) )
#define fm_set_user_team(%1,%2)	set_pdata_int( %1, OFFSET_TEAM, %2)
#define fm_get_user_team(%1) get_pdata_int( %1, OFFSET_TEAM)

#define TEAM_SELECT_VGUI_MENU_ID 2

#define HIDE_TIMER (1<<4) // 隱藏時間.
#define HIDE_RADAR (1<<3) // 隱藏子彈數量,血量,護甲.
#define HIDE_MONEY (1<<5) // 隱藏錢.
#define HIDE_AMMO (1<<0) // 隱藏準心.武器列表.
#define HIDE_FLASH (1<<1) // 隱藏手電筒.

const m_pPlayer	= 41;
const m_flPrimaryAttack = 46;
const m_flNextSecondaryAttack = 47;
const m_pClientActiveItem = 374;
const m_bitsDamageType = 76;
const m_flWait = 42;
/*================================================================================
[結束這些定義.]
=================================================================================*/

/*================================================================================
[預緩存/下載檔案.]
=================================================================================*/
new const LevelUp[] = "d2lod/levelup.wav";

new const g_crossbow_shoot_sound[] = "weapons/xbow_fire1.wav";

new const Custom_Models[MAX_HEROES][] = 
{
	"genshin_klee",
	"black_knight",
	"black_knight",
	"black_knight",
	"black_knight",
	"black_knight",
	"black_knight"
}

new const g_w_mp[] = "models/d2lod/w_mp.mdl";
new const g_w_hp[] = "models/d2lod/w_hp.mdl";
new const g_w_coins[] = "models/d2lod/w_coins.mdl";
new const g_w_item[] = "models/skeleton.mdl";
new const g_w_inventory[] = "models/d2lod/w_inventory.mdl";
new const g_w_charsi[] = "models/d2lod/charsi2.mdl";
new const g_w_akara[] = "models/d2lod/akara.mdl";

new const g_brassknuckles[] = "models/d2lod/v_knuckles.mdl";

// 其他自定義武器.
new const g_w_crossbow[] = "models/d2lod/crossbow_bolt.mdl";
new const g_v_crossbow[] = "models/d2lod/v_crossbow.mdl";
new const g_p_crossbow[] = "models/d2lod/p_crossbow.mdl";
/*================================================================================
[結束這些需要下載的檔案.]
=================================================================================*/

/*================================================================================
[職業名稱.]
=================================================================================*/
new const Diablo2_Fake_Player_Name[] =  "Diablo II LOD - Fake Player";

new const HEROES[MAX_HEROES][] = 
{
    "可莉",
    "錦木千束",
    "法師",
    "野蠻人",
    "聖劍士",
    "德魯伊",
    "術士"
} 


/*================================================================================
[結束定義職業名稱.]
=================================================================================*/


/*================================================================================
[藥水設定.]
=================================================================================*/
new const Potion_Names[MAX_POTIONS][] = {
	"小型生命藥水",
	"中型生命藥水",
	"大型生命藥水",
	"小型能量藥水",
	"中型能量藥水",
	"大型能量藥水",
	"小型混合藥水",
	"中型混合藥水",
	"大型混合藥水"
}

new const Potion_Cost[MAX_POTIONS] = {
	35,
	65,
	125,
	30,
	50,
	120,
	55,
	80,
	150
}

new const Potion_Multi[MAX_POTIONS] = {
	30,
	80,
	150,
	30,
	80,
	160,
	2,
	1,
	2
}
/*================================================================================
[結束藥水設定.]
=================================================================================*/


/*================================================================================
[可編譯的數值.]
=================================================================================*/
new const Float:HEROES_REGAIN_MANA[MAX_HEROES] = { 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0 }; // 各職業能量的回覆時間(秒)

new const HEROES_DEFENCE_DEXTERITY[MAX_HEROES] = { 1, 2, 1, 1, 3, 2, 1 }; // 各職業的1點護甲增加多少防禦力.

new const Float: HEROES_STRENGTH_PERSTAT[MAX_HEROES] = { 0.6, 0.8, 0.5, 0.9, 0.7, 0.6, 0.5 }; // 各職業的1點力量增加多少傷害.
new const HEROES_DEXTERITY_PERSTAT[MAX_HEROES] = { 3, 4, 2, 4, 3, 3, 2 }; // 各職業的1點敏捷增加多少迴避率.
new const HEROES_VITALITY_PERSTAT[MAX_HEROES] = { 2, 2, 1, 3, 2, 3, 1 }; // 各職業的1點體力增加多少血量最大值.
new const HEROES_ENERGY_PERSTAT[MAX_HEROES] = { 2, 2, 4, 1, 2, 3, 4 }; // 各職業的1點能量增加多少最大能量值.

new const LEVELS[MAX_LEVELS] = { 
100, 200, 300, 400, 700, 900, 1100, 1400, 1700, 2000,
2400, 2900, 3000, 3500, 4200, 5000, 6000, 7500, 9000, 11000,
14000, 17000, 22000, 26000, 31000, 3700, 43000, 49000, 55000, 65000,
70000, 73000, 76000, 80000, 83000, 85000, 90000, 95000, 100000, 110000,
120000, 130000, 140000, 150000, 160000, 170000, 180000, 190000, 200000, 210000,
220000, 240000, 260000, 270000, 280000, 290000, 300000, 320000, 330000, 350000,
360000, 370000, 380000, 390000, 400000, 420000, 430000, 450000, 460000, 480000,
500000, 510000, 520000, 530000, 540000, 550000, 560000, 580000, 600000, 610000,
630000, 650000, 680000, 710000, 740000, 750000, 760000, 780000, 800000, 840000,
880000, 930000, 950000, 1000000, 1100000, 1300000, 1500000, 1700000, 2000000, 2500000
}; // 每一等所需經驗值.

new const MAX_PLAYER_MONEY[MAX_LEVELS] = { 
50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
50000, 50000, 50000, 50000, 50000, 100000, 100000, 100000, 100000, 100000,
100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000,
150000, 150000, 150000, 150000, 150000, 150000, 150000, 150000, 150000, 150000,
200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000,
300000, 300000, 300000, 300000, 300000, 300000, 300000, 300000, 300000, 300000,
400000, 400000, 400000, 400000, 400000, 400000, 400000, 400000, 400000, 400000,
500000, 500000, 500000, 500000, 500000, 500000, 500000, 500000, 500000, 500000,
700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000,
900000, 900000, 900000, 900000, 1000000, 1000000, 1000000, 1000000, 1000000, 1000000
}; // 每一等的錢最大上限.

new const MAX_PLAYER_MONEY_INV[MAX_LEVELS] = { 
50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
50000, 50000, 50000, 50000, 50000, 100000, 100000, 100000, 100000, 100000,
100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000,
150000, 150000, 150000, 150000, 150000, 150000, 150000, 150000, 150000, 150000,
200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000,
300000, 300000, 300000, 300000, 300000, 300000, 300000, 300000, 300000, 300000,
400000, 400000, 400000, 400000, 400000, 400000, 400000, 400000, 400000, 400000,
500000, 500000, 500000, 500000, 500000, 500000, 500000, 500000, 500000, 500000,
700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000,
900000, 900000, 900000, 900000, 1000000, 1000000, 1000000, 1000000, 1000000, 1000000
}; // 每一等倉庫的錢最大上限.
/*================================================================================
[結束編輯數值.]
=================================================================================*/

/*================================================================================
[其他.]
=================================================================================*/
// 這些不是玩家Const.
new const g_Objective_Ents[][] = {
	"func_bomb_target",
	"info_bomb_target",
	"hostage_entity",
	"monster_scientist",
	"func_hostage_rescue",
	"info_hostage_rescue",
	"info_vip_start",
	"func_vip_safetyzone",
	"func_escapezone",
	"armoury_entity"
}
enum { USE_OFF, USE_ON, USE_SET, USE_TOGGLE };

new const inventory_dir_name[] = "d2_origins";

new mod_name[32] = "Diablo II: LOD";