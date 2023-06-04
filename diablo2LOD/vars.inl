// 所有新增的字元.

new d2_xp_kill, d2_max_random_value, d2_stat_points_levelup, d2_high_level_xp, d2_high_level, d2_enable_respawn, d2_spawn_cooldown, d2_spawn_protection, d2_spawn_protection_time, d2_block_kill_command, d2_minlevel_coins, d2_minlevel_items, d2_chance_to_drop, d2_chance_to_dropp, d2_minlevel_potions, d2_show_damage, d2_show_damage_received, d2_3rdperson, d2_bolts_value, d2_advert, d2_advert_time, d2_save_type, d2_save_by, d2_block_attack2, d2_cl_corpsestay;
new mysqlx_host, mysqlx_user, mysqlx_db, mysqlx_pass;

new g_Nvault, g_Nvault2, g_Nvault3;
new Handle:g_DBTuple, Handle:g_DBConn;

new MsgSayText;
new MsgHideCrosshair;
new MsgHideWeapon;
new gmsgClCorpse;
new g_iMaxPlayers;
new g_SyncHudCreate;
new g_SyncHudCreate_Avade;
new g_SyncHudCreate_Dmg;

new g_iFakeplayer;
new g_FakeEnt;

// 玩家的主要資訊.
new g_PlayerCharActive[33][MAX_CHARS + 1];
new g_PlayerChars[33];
new g_CurrentChar[33];
new g_PlayerHero[33][MAX_CHARS + 1];
new g_PlayerLevel[33][MAX_CHARS + 1];
new g_PlayerXp[33][MAX_CHARS + 1];
new g_PlayerSkPoints[33][MAX_CHARS + 1];
new g_PlayerStPoints[33][MAX_CHARS + 1];
new g_CurrentMana[33];
new g_Strength[33][MAX_CHARS + 1];
new g_Dexterity[33][MAX_CHARS + 1];
new g_Vitality[33][MAX_CHARS + 1];
new g_Energy[33][MAX_CHARS + 1];
new g_Coins[33][MAX_CHARS + 1];
new g_CoinsInv[33][MAX_CHARS + 1];
new g_Armor[33];
new g_MaxHealth[33];
new g_iPlayerItem[33][MAX_CHARS + 1][MAX_ITEMS];
new g_iPlayerItemRepair[33][MAX_CHARS + 1][MAX_ITEMS];
new g_iPlayerItemWorn[33][MAX_CHARS + 1][MAX_ITEMS];
new g_iPlayerItemInv[33][MAX_CHARS + 1][MAX_ITEMS];
new g_iPlayerItemBolts[33][MAX_CHARS + 1][MAX_ITEMS];

new g_iSkills[33][MAX_CHARS + 1][MAX_PLUGIN_SKILLS];

new g_iPlayerPotions[33][MAX_CHARS + 1][MAX_POTIONS];
new g_iPlayerPotionsInv[33][MAX_CHARS + 1][MAX_POTIONS];

new g_CoinsDropDeath[33];

new g_TradeInviter[33];
new g_TradeTarget[33];
new g_iPlayerTradeItem[33];

new Float: Current_Speed[33];

new bool: g_Shooting[33];
new bool: g_ItemsDeposited[33];
new bool: g_Player_Info[33];
new bool: g_PlayerProtected[33];
new bool: g_Player_FirstTime_Connected[33];
new bool: g_iLogged[33];
new g_iCam[33];

new g_szModel[33][MAX_MODEL_LENGTH];

new bool: g_FreezeTime;

// 不是給玩家用的字元.
new g_charcounter = -1;
new g_skillcounter = -1;
new gBuyCharacterMenu;

new g_skillindex[MAX_HEROES][MAX_PLUGIN_SKILLS + 1];
new g_skillhero[MAX_PLUGIN_SKILLS + 1];
new g_skilldisplay[MAX_PLUGIN_SKILLS + 1];
new g_skilllevel[MAX_HEROES][MAX_PLUGIN_SKILLS + 1];
new g_skillmax[MAX_PLUGIN_SKILLS + 1];
new g_skillname[MAX_HEROES][MAX_PLUGIN_SKILLS + 1][MAX_SKILL_NAME_SIZE + 1];
new g_skilldesc[MAX_HEROES][MAX_PLUGIN_SKILLS + 1][MAX_SKILL_DESC_SIZE + 1];

new item_name[MAX_ITEMS][32];
new item_pmodel[MAX_ITEMS][64];
new item_vmodel[MAX_ITEMS][64];
new item_data[MAX_ITEMS][MAX_CHAR_DATA];

new g_iItemTook, g_DamagedbySkill;
new g_iReturn;
new g_SkillSelected, g_SkillFired;
new g_TakeDamage, g_iLog, g_RangedDamage, g_DaggerPoisonDamage, g_ActRangedShoot;

new g_iFwd, g_iMsg;

// 倉庫.
new g_MapItemNum;
new g_ConfigsDir[64];
new g_ItemOriginDir[64];
new g_MapItemOrgins[MAX_MAPITEMS+1][3];
// 武器商人.
new g_MapItemNum1;
new g_MapItemOrgins1[MAX_MAPITEMS+1][3];
// 藥水商人.
new g_MapItemNum2;
new g_MapItemOrgins2[MAX_MAPITEMS+1][3];
// 角色商人.
new g_MapItemNum3;
new g_MapItemOrgins3[MAX_MAPITEMS+1][3];