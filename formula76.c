variables
{
	global:
		0: POINT_SPAWN
		5: POINT_START
		6: POINT_2
		7: POINT_3
		8: POINT_FINISH
		9: POINT_JUMP_PAD
		15: POINT_SPEED_BOOST
		18: POINT_SLOWDOWN
		23: ARRAY_TELEPORTERS
		26: HIGH_SCORE
		27: LEADER
		28: LEADER_ICON
		29: INT_RANDOM_STAGE

	player:
		4: EFFECT_SPEED
		16: SPEED_BOOST_ELIGIBLE
		18: SLOWDOWN_ELIGIBLE
}

rule("INIT: GV(u)=0")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Is Game In Progress == False;
	}

	actions
	{
		Global.U = 0;
		Global.HIGH_SCORE = 0;
	}
}

rule("INIT: GV(z)=Map-number")
{
	event
	{
		Ongoing - Global;
	}

	actions
	{
		Global.Z = Round To Integer(X Component Of(Nearest Walkable Position(Vector(100, 100, 100))), Up);
		Skip If(Objective Position(0) == Null, 1);
		disabled Global.Z += Round To Integer(X Component Of(Objective Position(0)), Up) + 300;
		Global.INT_RANDOM_STAGE = Random Integer(1, 4);
	}
}

rule("INIT: starting position, PV(q)=powerup-eligible")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Has Spawned(Event Player) == True;
	}

	actions
	{
		Teleport(Event Player, Global.POINT_SPAWN);
		Event Player.SPEED_BOOST_ELIGIBLE = True;
		Event Player.SLOWDOWN_ELIGIBLE = True;
		Disallow Button(Event Player, Ultimate);
	}
}

rule("INIT: checkpoints, powerup, etc.")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Hero Of(Event Player) == Hero(Soldier: 76);
		Is Game In Progress == True;
	}

	actions
	{
		Disable Built-In Game Mode Respawning(Event Player);
		Disable Built-In Game Mode Scoring;
		Set Status(Event Player, Null, Phased Out, 9999);
		Event Player.V = 100;
		Event Player.O = Global.POINT_START;
		Create Effect(Event Player, Light Shaft, Blue, Event Player.O + Vector(0, -1.500, 0), 2.500, Visible To Position and Radius);
		Create Effect(Event Player, Ring, Green, Event Player.O, 2.500, Visible To Position and Radius);
		Create Icon(Event Player, Event Player.O, Flag, Visible To and Position, Green, True);
		Set Objective Description(Event Player, String("{0} m/s", Speed Of(Event Player)), Visible To and String);
		Skip If(Global.POINT_SPEED_BOOST == 0, 2);
		Create Icon(All Players(All Teams), Global.POINT_SPEED_BOOST, Bolt, None, Purple, False);
		Create Effect(All Players(All Teams), Ring, Purple, Global.POINT_SPEED_BOOST, 2, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Ring, White, Global.POINT_FINISH, 2.500, Visible To Position and Radius);
		Create Icon(All Players(All Teams), Global.POINT_FINISH, Flag, Visible To and Position, White, False);
		Skip If(Global.POINT_SLOWDOWN == 0, 2);
		Create Icon(All Players(All Teams), Global.POINT_SLOWDOWN, Warning, None, Red, False);
		Create Effect(All Players(All Teams), Ring, Red, Global.POINT_SLOWDOWN, 3, Visible To Position and Radius);
	}
}

rule("INIT: POINT_JUMP_PAD")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.POINT_JUMP_PAD != 0;
	}

	actions
	{
		Create Effect(All Players(All Teams), Ring, Yellow, Global.POINT_JUMP_PAD, 3, Visible To Position and Radius);
		Create Icon(All Players(All Teams), Global.POINT_JUMP_PAD, Arrow: Up, Visible To and Position, Yellow, False);
	}
}

rule("PLAYER: Clear rooted")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Is Game In Progress == True;
	}

	actions
	{
		Clear Status(Event Player, Rooted);
	}
}

rule("PLAYER: Acceleration")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Hero Of(Event Player) == Hero(Soldier: 76);
		Is Using Ability 1(Event Player) == True;
		Horizontal Speed Of(Event Player) > 5;
	}

	actions
	{
		Event Player.V += 1;
		Set Move Speed(Event Player, Event Player.V);
		Wait(0.100, Ignore Condition);
		Loop If Condition Is True;
		Event Player.V = 100;
		Set Move Speed(Event Player, 100);
		Allow Button(Event Player, Secondary Fire);
	}
}

rule("PLAYER: Death")
{
	event
	{
		Player Died;
		All;
		All;
	}

	actions
	{
		Set Move Speed(Event Player, 100);
		Wait(3, Ignore Condition);
		Respawn(Event Player);
		Wait(0.500, Ignore Condition);
		Teleport(Event Player, Global.POINT_SPAWN);
	}
}

rule("CROSSING: POINT_START (Moving lap start time to #4)")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Distance Between(Position Of(Event Player), Event Player.O) <= 3;
		Event Player.O == Global.POINT_START;
	}

	actions
	{
		Event Player.O = Global.POINT_2;
		Play Effect(Event Player, Ring Explosion Sound, White, Event Player, 200);
		Play Effect(Event Player, Good Explosion, Blue, Event Player, 2);
		disabled Event Player.T[1] = Total Time Elapsed;
	}
}

rule("CROSSING: POINT_2")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Distance Between(Position Of(Event Player), Event Player.O) <= 3;
		Event Player.O == Global.POINT_2;
	}

	actions
	{
		Modify Player Score(Event Player, 0);
		Event Player.O = Global.POINT_3;
		Play Effect(Event Player, Ring Explosion Sound, White, Event Player, 200);
		Play Effect(Event Player, Good Explosion, Blue, Event Player, 2);
		Event Player.T[2] = Total Time Elapsed;
	}
}

rule("CROSSING: POINT_3")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Distance Between(Position Of(Event Player), Event Player.O) <= 3;
		Event Player.O == Global.POINT_3;
	}

	actions
	{
		Modify Player Score(Event Player, 0);
		Event Player.O = Global.POINT_FINISH;
		Play Effect(Event Player, Ring Explosion Sound, White, Event Player, 200);
		Play Effect(Event Player, Good Explosion, Blue, Event Player, 2);
		Event Player.T[3] = Total Time Elapsed;
	}
}

rule("CROSSING: POINT_FINISH lap-start-time=pv(t[1]) last-lap-time=pv(t[0]) previous-last-lap-time=pv(t[11])")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Distance Between(Position Of(Event Player), Event Player.O) <= 3;
		Event Player.O == Global.POINT_FINISH;
	}

	actions
	{
		Modify Player Score(Event Player, 1);
		Event Player.O = Global.POINT_START;
		Play Effect(Event Player, Buff Explosion Sound, White, Event Player, 200);
		Play Effect(Event Player, Good Explosion, Blue, Event Player, 2);
		Event Player.T[11] = Event Player.T[0];
		Skip If(Event Player.T[4] == 0, 1);
		Event Player.T[0] = Total Time Elapsed - Event Player.T[4];
		Event Player.T[4] = Total Time Elapsed;
		Skip If(Event Player.T[11] == 0, 1);
		Event Player.T[12] = Event Player.T[0] - Event Player.T[11];
		Big Message(Event Player, String("{0} - {1}", String("{0} sec", Event Player.T[0]), String("{0} m/s", Horizontal Speed Of(
			Event Player))));
		Small Message(All Players(All Teams), String("{0} - {1} - {2}", Event Player, String("{0} sec", Event Player.T[0]), String(
			"{0} m/s", Horizontal Speed Of(Event Player))));
		Skip If(Event Player.T[0] == 0 || (Global.T[1] != 0 && Event Player.T[0] >= Global.T[1]), 2);
		Global.T[0] = Event Player;
		Global.T[1] = Event Player.T[0];
		Skip If(Global.T != 0 && Horizontal Speed Of(Event Player) <= Global.T[3], 2);
		Global.T[2] = Event Player;
		Global.T[3] = Horizontal Speed Of(Event Player);
		Skip If(Score Of(Event Player) <= Global.HIGH_SCORE, 2);
		Global.HIGH_SCORE = Score Of(Event Player);
		Global.LEADER = Event Player;
	}
}

rule("EFFECT: Speed < 20 m/s")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Horizontal Speed Of(Event Player) < 20;
		Is Moving(Event Player) == True;
	}

	actions
	{
		Destroy Effect(Event Player.EFFECT_SPEED);
		Clear Status(Event Player, Burning);
	}
}

rule("EFFECT: Speed 20-29 m/s")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Horizontal Speed Of(Event Player) >= 20;
		Horizontal Speed Of(Event Player) < 30;
		Is Using Ability 1(Event Player) == True;
	}

	actions
	{
		Destroy Effect(Event Player.EFFECT_SPEED);
		Create Effect(All Players(All Teams), Sparkles, Red, Event Player, 0.500, Visible To Position and Radius);
		Event Player.EFFECT_SPEED = Last Created Entity;
		Clear Status(Event Player, Burning);
	}
}

rule("EFFECT: Speed 30-39 m/s")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Is Using Ability 1(Event Player) == True;
		Horizontal Speed Of(Event Player) >= 30;
		Horizontal Speed Of(Event Player) < 40;
	}

	actions
	{
		Destroy Effect(Event Player.EFFECT_SPEED);
		Create Effect(All Players(All Teams), Sparkles, Blue, Event Player, 0.500, Visible To Position and Radius);
		Event Player.EFFECT_SPEED = Last Created Entity;
		Clear Status(Event Player, Burning);
	}
}

rule("EFFECT: Speed 40-49 m/s")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Is Using Ability 1(Event Player) == True;
		Horizontal Speed Of(Event Player) >= 40;
		Horizontal Speed Of(Event Player) < 50;
	}

	actions
	{
		Destroy Effect(Event Player.EFFECT_SPEED);
		Set Status(Event Player, Null, Burning, 9999);
	}
}

rule("EFFECT: Speed 50-59 m/s")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Is Using Ability 1(Event Player) == True;
		Horizontal Speed Of(Event Player) >= 50;
	}

	actions
	{
		Destroy Effect(Last Created Entity);
		Set Status(Event Player, Null, Burning, 9999);
		Create Effect(All Players(All Teams), Bad Aura, Purple, Event Player, 1, Visible To Position and Radius);
		Event Player.EFFECT_SPEED = Last Created Entity;
	}
}

rule("MAP: Ilios Ruins")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 66;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-72.544, 66.333, -181.805);
		Global.POINT_START = Vector(-4.601, 62.915, -174.538);
		Global.POINT_2 = Vector(71.390, 61.915, -142.976);
		Global.POINT_3 = Vector(116.970, 66.940, -178.668);
		Global.POINT_FINISH = Vector(-58.256, 61.915, -157.274);
		Global.POINT_SPEED_BOOST = Vector(53.206, 61.178, -142.624);
		Global.POINT_JUMP_PAD = Vector(28.592, 61.477, -177.477);
	}
}

rule("MAP: Ilios Well")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == -170;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-286.063, -1.751, 30.179);
		Global.POINT_START = Vector(-267.855, 0.248, 50.609);
		Global.POINT_2 = Vector(-148.738, -1.858, -90.319);
		Global.POINT_3 = Vector(-206.501, 2.552, -41.728);
		Global.POINT_FINISH = Vector(-244.454, 6.634, 1.231);
		Global.POINT_JUMP_PAD = Vector(-179.371, -4.284, -66.039);
	}
}

rule("MAP: Nepal sanctum")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 101;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(81.066, 133.298, 97.544);
		Global.POINT_START = Vector(73.735, 130.455, 65.927);
		Global.POINT_2 = Vector(83.193, 130.002, 0.004);
		Global.POINT_3 = Vector(81.094, 132.175, -91.555);
		Global.POINT_FINISH = Vector(66.536, 133.548, 0.146);
		Global.POINT_SPEED_BOOST = Vector(50.774, 131.281, 0.344);
		Global.POINT_JUMP_PAD = Vector(56.632, 128.543, -28.085);
	}
}

rule("MAP: Nepal Shrine")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == -13;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-22.729, 23.551, -51.067);
		Global.POINT_START = Vector(-56.918, 20.551, -49.383);
		Global.POINT_2 = Vector(-63.930, 14.551, 0.033);
		Global.POINT_3 = Vector(-23.575, 23.549, 51.174);
		Global.POINT_FINISH = Vector(-22.766, 21.048, -86.716);
		Global.POINT_SPEED_BOOST = Vector(-48.377, 20.443, 49.267);
	}
}

rule("MAP: Nepal Village")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == -130;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-143.752, -92.849, -77.601);
		Global.POINT_START = Vector(-194.197, -96.901, -57.851);
		Global.POINT_2 = Vector(-200.141, -92.412, 32.803);
		Global.POINT_3 = Vector(-159.995, -88.447, 41.706);
		Global.POINT_FINISH = Vector(-159.389, -97, -77.233);
		Global.POINT_SPEED_BOOST = Vector(-219.958, -92.025, 0.167);
	}
}

rule("MAP: Lijang Control Center")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 14;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(0.008, 271.551, 259.632);
		Global.POINT_START = Vector(-36.818, 268.551, 296.629);
		Global.POINT_2 = Vector(-44.488, 270.051, 352.656);
		Global.POINT_3 = Vector(65.412, 268.994, 347.295);
		Global.POINT_FINISH = Vector(-0.047, 276.048, 279.775);
		Global.POINT_JUMP_PAD = Vector(3.080, 268.881, 279.987);
		Global.POINT_SPEED_BOOST = Vector(-0.210, 271.551, 327.437);
	}
}

rule("MAP: Lijang Garden")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 102;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-82.982, 97.551, 146.107);
		Global.POINT_START = Vector(-0.102, 95.553, 178.068);
		Global.POINT_2 = Vector(81.749, 97.549, 147.776);
		Global.POINT_3 = Vector(0.198, 97.550, 112.265);
		Global.POINT_FINISH = Vector(-53.108, 99.551, 162.154);
	}
}

rule("MAP: Lijang Night Market")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 69;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(49.231, 3.548, -29.262);
		Global.POINT_START = Vector(25.312, 7.577, -32.350);
		Global.POINT_2 = Vector(0.640, 1.553, -9.778);
		Global.POINT_3 = Vector(-74.108, 7.052, -7.637);
		Global.POINT_FINISH = Vector(0.707, 0.551, -74.166);
		Global.POINT_JUMP_PAD = Vector(-43.578, 4.455, -14.037);
	}
}

rule("MAP: Black Forest")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 37;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(30.934, 13.548, -25.407);
		Global.POINT_START = Vector(9.421, 12.352, -23.618);
		Global.POINT_2 = Vector(-4.242, 19.012, 26.683);
		Global.POINT_3 = Vector(21.050, 10.549, 3.743);
		Global.POINT_FINISH = Vector(-5.384, 19.674, 5.147);
	}
}

rule("MAP: Castillo")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == -71;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-99.446, 34.117, 95.936);
		Global.POINT_START = Vector(-74.458, 33.545, 53.015);
		Global.POINT_2 = Vector(-116.700, 35.054, 71.260);
		Global.POINT_3 = Vector(-114.279, 34.115, 27.336);
		Global.POINT_FINISH = Vector(-116.184, 40.126, 61.024);
		Global.POINT_JUMP_PAD = Vector(-126.525, 35.157, 63.400);
	}
}

rule("MAP: Chateau Guillard")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 168;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(176.423, 1.548, 50.697);
		Global.POINT_START = Vector(229.287, 14.548, 61.252);
		Global.POINT_2 = Vector(196.278, 10.548, 110.570);
		Global.POINT_3 = Vector(205.996, 1.549, 77.493);
		Global.POINT_FINISH = Vector(183.733, 9.552, 55.824);
		Global.POINT_SPEED_BOOST = Vector(206.247, 10.580, 94.255);
	}
}

rule("MAP: Dorado")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 144;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(64.906, 15.445, 13.343);
		Global.POINT_START = Vector(87.163, 14.301, 38.397);
		Global.POINT_2 = Vector(129.210, 13.563, 51.221);
		Global.POINT_3 = Vector(127.208, 10.568, -22.372);
		Global.POINT_FINISH = Vector(106.500, 19.551, -3.577);
		Global.POINT_SPEED_BOOST = Vector(108.416, 11.595, -30.638);
		Global.POINT_JUMP_PAD = Vector(85.021, 20.455, 17.189);
	}
}

rule("MAP: Ecopoint Antarctica")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 18;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(13.481, 9.583, 59.073);
		Global.POINT_START = Vector(1.315, 13.550, 11.958);
		Global.POINT_2 = Vector(-2.517, 8.550, -54.684);
		Global.POINT_3 = Vector(-10.421, 10.078, -0.128);
		Global.POINT_FINISH = Vector(-2.315, 8.551, 54.888);
		Global.POINT_SPEED_BOOST = Vector(18.091, 4.467, -0.057);
	}
}

rule("MAP: Eichenwalde")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 124;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(88.290, 14.413, -65.328);
		Global.POINT_START = Vector(102.680, 14.272, -109.216);
		Global.POINT_2 = Vector(37.166, 6.802, -52.677);
		Global.POINT_3 = Vector(46, 21.382, -67.745);
		Global.POINT_FINISH = Vector(86.682, 14.196, -73.278);
		Global.POINT_SPEED_BOOST = Vector(67.223, 6.023, -83.701);
	}
}

rule("MAP: Hanamura")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 78;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(28.146, 1.551, -3.264);
		Global.POINT_START = Vector(-4.208, 5.517, 21.625);
		Global.POINT_2 = Vector(31.664, 3.348, -60.397);
		Global.POINT_3 = Vector(-21.068, 2.551, -94.907);
		Global.POINT_FINISH = Vector(13.384, 8.292, -21.430);
		Global.POINT_SPEED_BOOST = Vector(21.902, -2.545, -82.880);
	}
}

rule("MAP: Horizon Lunar Colony")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 56;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(37.629, 8.552, -54.195);
		Global.POINT_START = Vector(54.533, 9.549, -100.802);
		Global.POINT_2 = Vector(-46.156, 6.053, -101.304);
		Global.POINT_3 = Vector(-5.621, 11.551, -118.123);
		Global.POINT_FINISH = Vector(81.436, 7.992, -40.463);
		Global.POINT_SPEED_BOOST = Vector(11.993, 7.953, -82.377);
		Global.POINT_JUMP_PAD = Vector(-27.897, 3.660, -126.608);
	}
}

rule("MAP: Ilios Lighthouse")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 300;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(261.795, -22.372, -95.124);
		Global.POINT_START = Vector(289.804, -23.922, -88.970);
		Global.POINT_2 = Vector(331.342, -17.089, -56.983);
		Global.POINT_3 = Vector(311.854, -19.841, 24.068);
		Global.POINT_FINISH = Vector(357.445, -24.089, -51.463);
		Global.POINT_SPEED_BOOST = Vector(319.563, -23.210, -35.596);
	}
}

rule("MAP: Oasis City Center")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 186;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(191.681, 2.399, 175.784);
		Global.POINT_START = Vector(120.149, 2.551, 223.825);
		Global.POINT_2 = Vector(119.893, 2.548, 223.766);
		Global.POINT_3 = Vector(98.544, 2.401, 299.925);
		Global.POINT_FINISH = Vector(197.784, 2.293, 214.365);
	}
}

rule("PLAYER: Set score variable")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	actions
	{
		Event Player.SLOWDOWN_ELIGIBLE = Score Of(Event Player);
	}
}

rule("COMMUNICATE: Victory")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Score Of(Event Player) == 15;
	}

	actions
	{
		Communicate(Event Player, Emote Up);
	}
}

rule("EFFECT: Splat!")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Horizontal Speed Of(Event Player) < 4;
		Ray Cast Hit Normal(Vector(0, 0, 0), Vector(0, 0, 0), All Players(All Teams), Event Player, True) == True;
		Is Game In Progress == True;
	}

	actions
	{
		Play Effect(All Players(All Teams), Debuff Impact Sound, Purple, Event Player, 50);
		Play Effect(All Players(All Teams), Bad Explosion, Yellow, Event Player, 1);
	}
}

rule("STAT: Best time, top speed.  GV(u)=HUD-texts[0,1],player-effects[2,3]")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Is Game In Progress == True;
		Global.T != 0;
	}

	actions
	{
		Destroy HUD Text(Global.U[0]);
		Create HUD Text(All Players(All Teams), String("{0} {1}", String("Best"), Custom String("LAP")), Global.T[0], String("{0} sec",
			Global.T[1]), Left, 0, Yellow, White, White, Visible To and String, Default Visibility);
		Global.U[0] = Last Text ID;
		Destroy HUD Text(Global.U[1]);
		Create HUD Text(All Players(All Teams), String("Fastest"), Global.T[2], String("{0} m/s", Global.T[3]), Left, 1, Blue, White,
			White, Visible To and String, Default Visibility);
		Global.U[1] = Last Text ID;
		Play Effect(All Players(All Teams), Buff Impact Sound, White, Global.POINT_FINISH, 500);
		Destroy Effect(Global.U[2]);
		Create Effect(All Players(All Teams), Good Aura, Yellow, Global.T[0], 1, Visible To Position and Radius);
		Global.U[2] = Last Created Entity;
		Destroy Effect(Global.U[3]);
		Create Effect(All Players(All Teams), Good Aura, Blue, Global.T[2], 1, Visible To Position and Radius);
		Global.U[3] = Last Created Entity;
	}
}

rule("PLAYERS: Speedometer")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Is Game In Progress == True;
	}

	actions
	{
		Set Ultimate Charge(Event Player, Absolute Value(Horizontal Speed Of(Event Player)));
		Set Ultimate Ability Enabled(Event Player, True);
		Wait(0.250, Ignore Condition);
		Loop;
	}
}

rule("PLAYERS: Splat! (chance of voice line)")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Horizontal Speed Of(Event Player) < 4;
		Random Integer(1, 100) > 74;
	}

	actions
	{
		Communicate(Event Player, Voice Line Down);
	}
}

rule("PLAYERS: score over head")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Is Game In Progress == True;
	}

	actions
	{
		Create In-World Text(All Players(All Teams), String("{0} / {1}", Round To Integer(Score Of(Event Player), Up), 15), Event Player,
			0.750, Clip Against Surfaces, Visible To Position and String, White, Default Visibility);
	}
}

rule("COMMUNICATE: Chance to say hello within 4m")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Is Game In Progress == True;
		Distance Between(Event Player, Closest Player To(Event Player, All Teams)) < 4;
		Random Integer(1, 100) >= 90;
	}

	actions
	{
		Communicate(Event Player, Hello);
	}
}

rule("COMMUNICATE: Chance to say voice line within 2m")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Is Game In Progress == True;
		Distance Between(Event Player, Closest Player To(Event Player, All Teams)) < 2;
		Random Integer(1, 100) >= 80;
	}

	actions
	{
		Communicate(Event Player, Voice Line Right);
	}
}

rule("CROSSING: POINT_SPEED_BOOST")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Global.POINT_SPEED_BOOST != 0;
		Distance Between(Position Of(Event Player), Global.POINT_SPEED_BOOST) <= 2;
		Is Moving(Event Player) == True;
		Event Player.SPEED_BOOST_ELIGIBLE == True;
	}

	actions
	{
		Big Message(Event Player, String("Faster"));
		Event Player.V += 50;
		Set Move Speed(Event Player, Event Player.V);
		Play Effect(All Players(All Teams), Good Pickup Effect, Purple, Event Player, 3);
		Play Effect(All Players(All Teams), Buff Explosion Sound, Purple, Event Player, 10);
		Event Player.SPEED_BOOST_ELIGIBLE = False;
		Wait(10, Ignore Condition);
		Event Player.SPEED_BOOST_ELIGIBLE = True;
	}
}

rule("CROSSING: POINT_SLOWDOWN")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Global.POINT_SLOWDOWN != 0;
		Distance Between(Position Of(Event Player), Global.POINT_SLOWDOWN) <= 3;
		Is Moving(Event Player) == True;
		Event Player.SLOWDOWN_ELIGIBLE == True;
		Event Player.V > 100;
	}

	actions
	{
		Big Message(Event Player, String("Slower"));
		Event Player.V /= 2;
		Set Move Speed(Event Player, Event Player.V);
		Play Effect(All Players(All Teams), Bad Pickup Effect, Red, Event Player, 3);
		Play Effect(All Players(All Teams), Debuff Impact Sound, Red, Event Player, 10);
		Event Player.SLOWDOWN_ELIGIBLE = False;
		Wait(5, Ignore Condition);
		Event Player.SLOWDOWN_ELIGIBLE = True;
	}
}

rule("MAP: Blizzworld (stage 1)")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		(Current Map == Map(Blizzard World) || Current Map == Map(Blizzard World Winter)) == True;
		Global.INT_RANDOM_STAGE == 1;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-3.815, 7.613, 52.909);
		Global.POINT_START = Vector(3.054, -3.548, 16.216);
		Global.POINT_2 = Vector(3.222, 3.451, 74.522);
		Global.POINT_3 = Vector(3.046, 1.452, 12.643);
		Global.POINT_FINISH = Vector(3.145, -2.548, -14.286);
	}
}

rule("MAP: Blizzworld (stage 2) (50% chance until cross-stage variant is added)")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		(Current Map == Map(Blizzard World) || Current Map == Map(Blizzard World Winter)) == True;
		Global.INT_RANDOM_STAGE >= 2;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-79.475, 1.798, 64.043);
		Global.POINT_START = Vector(-20.844, 3.551, 67.832);
		Global.POINT_2 = Vector(-70.002, 3.298, 151.324);
		Global.POINT_3 = Vector(-76.901, 6.423, 66.433);
		Global.POINT_FINISH = Vector(-64.419, 2.149, 120.448);
		Global.POINT_SPEED_BOOST = Vector(-45.947, 0.265, 97.890);
		Global.POINT_SLOWDOWN = Vector(-56.641, 6.064, 97.352);
		Global.POINT_JUMP_PAD = Vector(-56.948, 0.342, 68.803);
	}
}

rule("MAP: Blizzworld (stage 3)")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		(Current Map == Map(Blizzard World) || Current Map == Map(Blizzard World Winter)) == True;
		Global.INT_RANDOM_STAGE >= 4;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-162.918, 2.902, 98.732);
		Global.POINT_START = Vector(-110.579, -2.702, 113.771);
		Global.POINT_2 = Vector(-139.933, 2.298, 105.269);
		Global.POINT_3 = Vector(-84.521, 3.298, 146.469);
		Global.POINT_FINISH = Vector(-118.197, 12.354, 120.267);
		Global.POINT_SPEED_BOOST = Vector(-45.947, 0.265, 97.890);
		Global.POINT_JUMP_PAD = Vector(-115.520, 6.299, 129.617);
	}
}

rule("MAP: Necropolis")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 30;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-30.324, 6.194, 26.667);
		Global.POINT_START = Vector(-0.342, 2.279, 12.610);
		Global.POINT_2 = Vector(0.727, 4.195, -23.064);
		Global.POINT_3 = Vector(-32.312, 2.477, 0.142);
		Global.POINT_FINISH = Vector(23.102, 6.312, 0.159);
		Global.POINT_SPEED_BOOST = Vector(-0.197, 2.377, -12.646);
		Global.POINT_JUMP_PAD = Vector(25.901, 1.817, -0.181);
	}
}

rule("MAP: Oasis Gardens")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 173;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(78.296, 7.043, -258.281);
		Global.POINT_START = Vector(103.226, 9.551, -285.832);
		Global.POINT_2 = Vector(187.719, 9.551, -217.596);
		Global.POINT_3 = Vector(131.932, 4.131, -203.530);
		Global.POINT_FINISH = Vector(121.509, 13.050, -227.400);
		Global.POINT_SPEED_BOOST = Vector(141.221, 8, -247.455);
		Global.POINT_JUMP_PAD = Vector(139.459, 7.992, -233.119);
	}
}

rule("MAP: Oasis University")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == -163;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-214.050, 21.885, 71.249);
		Global.POINT_START = Vector(-172.085, 17.548, 54.001);
		Global.POINT_2 = Vector(-187.928, 12.800, -0.114);
		Global.POINT_3 = Vector(-210.744, 19.552, -59.180);
		Global.POINT_FINISH = Vector(-214.036, 20.552, -0.064);
		Global.POINT_SPEED_BOOST = Vector(-188.029, 10.952, 0.129);
	}
}

rule("MAP: Paris")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == -3;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-0.398, 11.544, -49.909);
		Global.POINT_START = Vector(-45.446, 17.538, 26.431);
		Global.POINT_2 = Vector(-74.471, 13.538, -15.836);
		Global.POINT_3 = Vector(-34.750, 15.538, -8.173);
		Global.POINT_FINISH = Vector(10.783, 11.472, -61.286);
		Global.POINT_SPEED_BOOST = Vector(-20.005, 11.551, -19.144);
	}
}

rule("MAP: Petra")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 41;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-44.970, -11.460, 9.708);
		Global.POINT_START = Vector(-18.397, 1.638, -8.515);
		Global.POINT_2 = Vector(23.850, -7.811, 52.938);
		Global.POINT_3 = Vector(-17.379, -6.496, 28.646);
		Global.POINT_FINISH = Vector(2.253, -8.430, 9.041);
		Global.POINT_SPEED_BOOST = Vector(-30.081, -5.455, -0.093);
	}
}

rule("MAP: Temple of Anubis")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 21;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-33.800, 12.147, 44.882);
		Global.POINT_START = Vector(-53.794, 11.062, 30.387);
		Global.POINT_2 = Vector(3.973, 6.147, 113.761);
		Global.POINT_3 = Vector(-38.567, 7.052, 63.942);
		Global.POINT_FINISH = Vector(-4.797, 4.056, 45.794);
		Global.POINT_SPEED_BOOST = Vector(-7.864, 1.329, 101.490);
		Global.POINT_JUMP_PAD = Vector(-21.240, 6.842, 63.166);
	}
}

rule("MAP: Volskaya Industries")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 34;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-13.796, -1.433, 115.710);
		Global.POINT_START = Vector(19.360, -0.433, 55.788);
		Global.POINT_2 = Vector(-83.112, 1.569, 19.198);
		Global.POINT_3 = Vector(-103.218, 3.568, 78.822);
		Global.POINT_FINISH = Vector(-0.310, 3.472, 88.048);
		Global.POINT_SPEED_BOOST = Vector(-32.751, 3.569, 73.187);
	}
}

rule("PLAYER: Crosses POINT_JUMP_PAD")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Distance Between(Event Player, Global.POINT_JUMP_PAD) < 3;
	}

	actions
	{
		Apply Impulse(Event Player, Vector(0, 1, 0), 15, To Player, Incorporate Contrary Motion);
	}
}

rule("HUD: Player-lap-times-HUD-text=PV(u[0])")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Event Player.C > 0;
	}

	actions
	{
		Skip If(Event Player.U == 0, 1);
		Destroy HUD Text(Event Player.U[0]);
		Create HUD Text(Event Player, Custom String("LAP TIMES"), String("{0}: {1}", Custom String("Last"), Event Player.T), String(
			"{0}: {1}", Custom String("Current"), String("{0} sec", Event Player.C)), Right, 0, White, White, White, Visible To and String,
			Default Visibility);
		Event Player.U[0] = Last Text ID;
	}
}

rule("HUD: Player-relative-lap-times-HUD-text=PV(u[1])")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Event Player.T[12] != 0;
	}

	actions
	{
		Create HUD Text(Event Player, Null, Null, String("{0}: {1}", Custom String("Last Lap +/-"), Event Player.T[12]), Right, 1, White,
			White, White, Visible To and String, Default Visibility);
		Event Player.U[1] = Last Text ID;
	}
}

rule("PLAYER STAT: Current lap time=PV(c)")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Event Player.T[4] > 0;
	}

	actions
	{
		Event Player.C = Total Time Elapsed - Event Player.T[4];
		Wait(0.200, Ignore Condition);
		Loop;
	}
}

rule("MAP: Havana (Checkpoints across stages)")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 88;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-58.444, 5.499, -27.063);
		Global.POINT_START = Vector(-96.363, 5.398, -39.877);
		Global.POINT_2 = Vector(-27.211, 13.551, -51.218);
		Global.POINT_3 = Vector(27.993, 11.548, -91.529);
		Global.POINT_FINISH = Vector(139.848, 7.609, -46.608);
		Global.POINT_SPEED_BOOST = Vector(-54.604, 7.552, -72.609);
		Global.POINT_SLOWDOWN = Vector(119.274, 7.977, -46.749);
		Global.POINT_JUMP_PAD = Vector(118.265, 14.591, -58.110);
		Global.ARRAY_TELEPORTERS[1] = Vector(34.374, 13.551, -56.896);
		Global.ARRAY_TELEPORTERS[2] = Vector(34.451, 13.552, -62.568);
		Global.ARRAY_TELEPORTERS[3] = Vector(64.840, 7.362, -83.169);
		Global.ARRAY_TELEPORTERS[4] = Vector(68, 7.380, -83.559);
		Global.ARRAY_TELEPORTERS[5] = Vector(62.254, 7.552, -93.216);
		Global.ARRAY_TELEPORTERS[6] = Vector(58, 7.551, -93.216);
		Global.ARRAY_TELEPORTERS[7] = Vector(4.606, 7.549, -96.175);
		Global.ARRAY_TELEPORTERS[8] = Vector(0, 7.549, -96.175);
	}
}

disabled rule("MAP: Havana (all checkpoints in first part)")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global.Z == 88;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-58.444, 5.499, -27.063);
		Global.POINT_START = Vector(-96.368, 5.397, -40.221);
		Global.POINT_2 = Vector(-49.486, 6.299, -32.768);
		Global.POINT_3 = Vector(-24.909, 11.552, -97.665);
		Global.POINT_FINISH = Vector(-25.282, 11.605, -71.365);
		Global.POINT_SPEED_BOOST = Vector(-54.530, 7.551, -77.611);
		Global.POINT_SLOWDOWN = Vector(-15.853, 7.670, -64.424);
		Global.POINT_JUMP_PAD = Vector(-45.281, 11.532, -42.785);
		Global.ARRAY_TELEPORTERS[1] = Vector(34.374, 13.551, -56.896);
		Global.ARRAY_TELEPORTERS[2] = Vector(34.451, 13.552, -62.568);
		Global.ARRAY_TELEPORTERS[3] = Vector(64.840, 7.362, -83.169);
		Global.ARRAY_TELEPORTERS[4] = Vector(68, 7.380, -83.559);
		Global.ARRAY_TELEPORTERS[5] = Vector(62.254, 7.552, -93.216);
		Global.ARRAY_TELEPORTERS[6] = Vector(58, 7.551, -93.216);
		Global.ARRAY_TELEPORTERS[7] = Vector(4.606, 7.549, -96.175);
		Global.ARRAY_TELEPORTERS[8] = Vector(0, 7.549, -96.175);
	}
}

rule("INIT: Teleporters")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Is Game In Progress == True;
		Global.ARRAY_TELEPORTERS != 0;
	}

	actions
	{
		Create Effect(All Players(All Teams), Orb, Green, Global.ARRAY_TELEPORTERS[1], 3, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Orb, Red, Global.ARRAY_TELEPORTERS[2], 3, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Orb, Green, Global.ARRAY_TELEPORTERS[3], 3, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Orb, Red, Global.ARRAY_TELEPORTERS[4], 3, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Orb, Green, Global.ARRAY_TELEPORTERS[5], 3, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Orb, Red, Global.ARRAY_TELEPORTERS[6], 3, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Orb, Green, Global.ARRAY_TELEPORTERS[7], 3, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Orb, Red, Global.ARRAY_TELEPORTERS[8], 3, Visible To Position and Radius);
	}
}

rule("TELEPORTER: 1-2")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Distance Between(Event Player, Global.ARRAY_TELEPORTERS[1]) <= 3;
	}

	actions
	{
		Play Effect(All Players(All Teams), Ring Explosion, Green, Global.ARRAY_TELEPORTERS[1], 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Green, Global.ARRAY_TELEPORTERS[1], 10);
		Teleport(Event Player, Global.ARRAY_TELEPORTERS[2]);
		Play Effect(All Players(All Teams), Ring Explosion, Red, Global.ARRAY_TELEPORTERS[2], 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Red, Global.ARRAY_TELEPORTERS[2], 10);
	}
}

rule("TELEPORTER: 3-4")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Distance Between(Event Player, Global.ARRAY_TELEPORTERS[3]) <= 3;
	}

	actions
	{
		Play Effect(All Players(All Teams), Ring Explosion, Green, Global.ARRAY_TELEPORTERS[3], 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Green, Global.ARRAY_TELEPORTERS[3], 10);
		Teleport(Event Player, Global.ARRAY_TELEPORTERS[4]);
		Play Effect(All Players(All Teams), Ring Explosion, Red, Global.ARRAY_TELEPORTERS[4], 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Red, Global.ARRAY_TELEPORTERS[4], 10);
	}
}

rule("TELEPORTER: 5-6")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Distance Between(Event Player, Global.ARRAY_TELEPORTERS[5]) <= 3;
	}

	actions
	{
		Play Effect(All Players(All Teams), Ring Explosion, Green, Global.ARRAY_TELEPORTERS[5], 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Green, Global.ARRAY_TELEPORTERS[5], 10);
		Teleport(Event Player, Global.ARRAY_TELEPORTERS[6]);
		Play Effect(All Players(All Teams), Ring Explosion, Red, Global.ARRAY_TELEPORTERS[6], 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Red, Global.ARRAY_TELEPORTERS[6], 10);
	}
}

rule("TELEPORTER: 7-8")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Distance Between(Event Player, Global.ARRAY_TELEPORTERS[7]) <= 3;
	}

	actions
	{
		Play Effect(All Players(All Teams), Ring Explosion, Green, Global.ARRAY_TELEPORTERS[7], 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Green, Global.ARRAY_TELEPORTERS[7], 10);
		Teleport(Event Player, Global.ARRAY_TELEPORTERS[8]);
		Play Effect(All Players(All Teams), Ring Explosion, Red, Global.ARRAY_TELEPORTERS[8], 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Red, Global.ARRAY_TELEPORTERS[8], 10);
	}
}

rule("EFFECT: Leader Icon")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Is Game In Progress == True;
		Global.LEADER != 0;
		Entity Exists(Global.LEADER) == True;
	}

	actions
	{
		Create Icon(All Players(All Teams), Global.LEADER, Arrow: Down, Visible To and Position, Team 2, False);
		Global.LEADER_ICON = Last Created Entity;
	}
}

rule("STAT: Leader")
{
	event
	{
		Player Earned Elimination;
		All;
		All;
	}

	conditions
	{
		(Score Of(Event Player) > Global.HIGH_SCORE) == True;
	}

	actions
	{
		Global.HIGH_SCORE = Score Of(Event Player);
		Global.LEADER = Event Player;
	}
}

rule("START: Message")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Is Game In Progress == True;
	}

	actions
	{
		Big Message(All Players(All Teams), Custom String("No brakes, everybody splats!"));
	}
}

rule("STAT: Reset leader when leader leaves game")
{
	event
	{
		Player Left Match;
		All;
		All;
	}

	conditions
	{
		(Event Player == Global.LEADER) == True;
	}

	actions
	{
		Global.HIGH_SCORE = 0;
		Global.LEADER = 0;
		Destroy Icon(Global.LEADER_ICON);
	}
}

rule("MAP: King's Row (Winter) (all 3 stages with teleporters)")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		"Regular King's Row == 10, Winter King's Row == 17"
		Current Map == Map(King's Row Winter);
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-138.424, 7.526, 30.753);
		Global.POINT_START = Vector(-179.287, 1.739, 37.435);
		Global.POINT_2 = Vector(-59.471, 6.508, -16.984);
		Global.POINT_3 = Vector(-26.016, 1.424, 4.752);
		Global.POINT_FINISH = Vector(-95.472, -0.947, -46.511);
		Global.POINT_SPEED_BOOST = Vector(-54.767, 1.493, -24.222);
		Global.POINT_JUMP_PAD = Vector(-79.831, 0.902, -19.140);
		Global.ARRAY_TELEPORTERS[1] = Vector(-101.490, 2.513, -5.540);
		Global.ARRAY_TELEPORTERS[2] = Vector(-99.458, 2.595, -5.910);
		Global.ARRAY_TELEPORTERS[3] = Vector(-29.028, 10.551, -16.381);
		Global.ARRAY_TELEPORTERS[4] = Vector(-28.777, 10.201, -17.689);
		Global.ARRAY_TELEPORTERS[5] = Vector(-31.662, 1.414, -26.176);
		Global.ARRAY_TELEPORTERS[6] = Vector(-33.263, 1.412, -26.522);
		Global.ARRAY_TELEPORTERS[7] = Vector(-100.419, 2.555, -11.522);
		Global.ARRAY_TELEPORTERS[8] = Vector(-102.437, 2.502, -11.064);
	}
}

rule("MAP: King's Row")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		"Regular King's Row == 10, Winter King's Row == 17"
		Current Map == Map(King's Row);
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-36.584, 1.409, -24.038);
		Global.POINT_START = Vector(-95.482, -0.944, -46.273);
		Global.POINT_2 = Vector(-72.179, 6.052, -37.340);
		Global.POINT_3 = Vector(-61.783, 1.550, 2.889);
		Global.POINT_FINISH = Vector(-48.632, 10.672, -37.568);
		Global.POINT_SPEED_BOOST = Vector(-30.029, 6.512, -47.474);
		Global.POINT_JUMP_PAD = Vector(-79.831, 0.902, -19.140);
	}
}

disabled rule("TESTING: Interact = Teleport")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Is Button Held(Event Player, Interact) == True;
	}

	actions
	{
		Teleport(Event Player, Position Of(Event Player) + Facing Direction Of(Event Player));
	}
}

rule("MAP: Hollywood (with teleporters across stages)")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		(Current Map == Map(Hollywood) || Current Map == Map(Hollywood Halloween)) == True;
		Global.INT_RANDOM_STAGE == 4;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-51.695, 2.012, 55.924);
		Global.POINT_START = Vector(18.904, 7.552, 41.222);
		Global.POINT_2 = Vector(-24.025, 9.301, 10.538);
		Global.POINT_3 = Vector(48.611, 4.300, -55.973);
		Global.POINT_FINISH = Vector(-19.226, 5.300, -147.510);
		Global.POINT_SPEED_BOOST = Vector(-54.767, 1.493, -24.222);
		Global.POINT_JUMP_PAD = Vector(-79.831, 0.902, -19.140);
		Global.ARRAY_TELEPORTERS[1] = Vector(-10.082, 3.301, -14.617);
		Global.ARRAY_TELEPORTERS[2] = Vector(-8.928, 3.299, -13.952);
		Global.ARRAY_TELEPORTERS[3] = Vector(-14.978, 3.302, -79.877);
		Global.ARRAY_TELEPORTERS[4] = Vector(-14.165, 3.279, -78.636);
		Global.ARRAY_TELEPORTERS[5] = Vector(12.706, 3.297, -85.458);
		Global.ARRAY_TELEPORTERS[6] = Vector(12.371, 3.298, -86.793);
		Global.ARRAY_TELEPORTERS[7] = Vector(2.281, 3.298, -2.440);
		Global.ARRAY_TELEPORTERS[8] = Vector(1.531, 3.299, -1.192);
	}
}

rule("MAP: Hollywood (stage 1)")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		(Current Map == Map(Hollywood) || Current Map == Map(Hollywood Halloween)) == True;
		Global.INT_RANDOM_STAGE == 1;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-51.695, 2.362, 55.080);
		Global.POINT_START = Vector(18.904, 7.552, 41.222);
		Global.POINT_2 = Vector(-45.094, 3.302, -2.104);
		Global.POINT_3 = Vector(-19.562, 7.303, -9.392);
		Global.POINT_FINISH = Vector(-11.955, 2.557, 68.462);
		Global.POINT_SPEED_BOOST = Vector(-29.414, 8.719, 15.414);
		Global.POINT_JUMP_PAD = Vector(-26.730, 2.060, -2.828);
	}
}

rule("MAP: Hollywood (stage 2)")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		(Current Map == Map(Hollywood) || Current Map == Map(Hollywood Halloween)) == True;
		Global.INT_RANDOM_STAGE == 2;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-7.158, 3.157, -7.794);
		Global.POINT_START = Vector(7.051, 3.298, -31.029);
		Global.POINT_2 = Vector(49.450, 4.301, -56.437);
		Global.POINT_3 = Vector(-26.131, 7.299, -46.007);
		Global.POINT_FINISH = Vector(-9.048, 8.684, -40.798);
		Global.POINT_SPEED_BOOST = Vector(24.891, 2.641, -75.941);
	}
}

rule("MAP: Hollywood (stage 3) (layout designed with help from Virgo)")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		(Current Map == Map(Hollywood) || Current Map == Map(Hollywood Halloween)) == True;
		Global.INT_RANDOM_STAGE == 3;
	}

	actions
	{
		Global.POINT_SPAWN = Vector(-7.767, 12.592, -102.932);
		Global.POINT_START = Vector(-18.402, 11.301, -107.348);
		Global.POINT_2 = Vector(-48.043, 5.301, -85.512);
		Global.POINT_3 = Vector(-6.144, 4.300, -124.453);
		Global.POINT_FINISH = Vector(-27.979, 8.383, -137.872);
		Global.POINT_SPEED_BOOST = Vector(13.762, 5.302, -97.152);
		Global.POINT_JUMP_PAD = Vector(8.917, 5.302, -103.203);
	}
}
