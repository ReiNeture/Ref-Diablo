#include <amxmodx>
#include <d2lod>
#include <engine>
#include <fakemeta>

new PLUGIN_NAME[] = "D2 怪物變種"
new PLUGIN_AUTHOR[] = "Reff"
new PLUGIN_VERSION[] = "1.0"

#define MAX_MONSTERS 2

new const Before_Models[MAX_MONSTERS][] =
{
	"models/gonome.mdl",
	"models/zombie.mdl"
};

new const After_Models[MAX_MONSTERS][] =
{
	"models/gonome_x.mdl",
	"models/okasi_zombie.mdl"
};

new const Variety_Name[MAX_MONSTERS][] =
{
	"變異殭屍王",
	"#3 殭屍"
};

// 1 = 0.01%, 10 = 0.1%, 100 = 1%, 1000 = 10%, 10000 = 100%
new const Variety_Chance[MAX_MONSTERS] =
{
	200,
	10
};

new const Float:Variety_Health[MAX_MONSTERS] =
{
	30000.0,
	2000.0
};

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR);
	set_task(10.0, "monster_variety", _, _, _, "b");
}

public plugin_precache()
{
	for(new i = 0; i < MAX_MONSTERS; i++)
	{
		engfunc(EngFunc_PrecacheModel, After_Models[i]);
	}

	// 額外需載入模組
	engfunc(EngFunc_PrecacheModel, "models/okasi_zombieT.mdl");
}

public monster_variety()
{
	new Float:MinBox[3], Float:MaxBox[3], Float:orig[3], Float:fHealth;

	for(new i = 0; i < MAX_MONSTERS; i++)
	{
		new ent = FM_NULLENT;
		while ( ( ent = fm_find_ent_by_model(ent, "func_wall", Before_Models[i]) ) )
		{
			if( random_num(1, 10000) <= Variety_Chance[i] )
			{
				entity_get_vector(ent, EV_VEC_origin, orig);
				entity_get_vector(ent, EV_VEC_mins, MinBox);
				entity_get_vector(ent, EV_VEC_maxs, MaxBox);
				fHealth = entity_get_float(ent, EV_FL_health);

				entity_set_model(ent, After_Models[i])
				entity_set_float(ent, EV_FL_health, fHealth + Variety_Health[i])

				entity_set_vector(ent, EV_VEC_mins, MinBox)
				entity_set_vector(ent, EV_VEC_maxs, MaxBox)
				entity_set_origin(ent, orig)

				entity_set_int(ent, EV_INT_solid, 2)

				if( equal("models/gonome.mdl", Before_Models[i]) )
				{
					entity_set_int(ent, EV_INT_sequence, 3)
					client_printcolor(0, "/ctr警告/y! /g某個地方出現了 /ctr%s", Variety_Name[i]);
				}
			}
		}
	}
}

stock fm_find_ent_by_class(index, const classname[]) {
	return engfunc(EngFunc_FindEntityByString, index, "classname", classname)
}

stock fm_find_ent_by_model(index, const classname[], const model[]) {
	new ent = index, mdl[72];
	while ((ent = fm_find_ent_by_class(ent, classname))) {
		pev(ent, pev_model, mdl, sizeof mdl - 1);
		if (equal(mdl, model))
			return ent;
	}
	return 0;
}
