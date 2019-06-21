rule("Init: GV(u)=0")
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
		Set Global Variable(U, 0);
	}
}

rule("Init: GV(z)=Map-number")
{
	event
	{
		Ongoing - Global;
	}

	actions
	{
		Set Global Variable(Z, Round To Integer(X Component Of(Nearest Walkable Position(Vector(100, 100, 100))), Up));
		Skip If(Compare(Objective Position(0), ==, Null), 1);
		Modify Global Variable(Z, Add, Add(Round To Integer(X Component Of(Objective Position(0)), Up), 300));
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
		Teleport(Event Player, Global Variable(A));
		Set Player Variable(Event Player, Q, True);
		Set Player Variable(Event Player, S, True);
		Disallow Button(Event Player, Ultimate);
	}
}

rule("PLAYER: checkpoints, powerup, etc.")
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
		Set Player Variable(Event Player, V, 100);
		Set Player Variable(Event Player, O, Global Variable(F));
		Create Effect(Event Player, Light Shaft, Blue, Add(Player Variable(Event Player, O), Vector(0, -1.500, 0)), 2.500,
			Visible To Position and Radius);
		Create Effect(Event Player, Ring, Green, Player Variable(Event Player, O), 2.500, Visible To Position and Radius);
		Create Icon(Event Player, Player Variable(Event Player, O), Flag, Visible To and Position, Green, True);
		Set Objective Description(Event Player, String("{0} m/s", Speed Of(Event Player), Null, Null), Visible To and String);
		Skip If(Compare(Global Variable(P), ==, 0), 2);
		Create Icon(All Players(All Teams), Global Variable(P), Bolt, None, Purple, False);
		Create Effect(All Players(All Teams), Ring, Purple, Global Variable(P), 2, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Ring, White, Global Variable(I), 2.500, Visible To Position and Radius);
		Create Icon(All Players(All Teams), Global Variable(I), Flag, Visible To and Position, White, False);
		Skip If(Compare(Global Variable(S), ==, 0), 2);
		Create Icon(All Players(All Teams), Global Variable(S), Warning, None, Red, False);
		Create Effect(All Players(All Teams), Ring, Red, Global Variable(S), 3, Visible To Position and Radius);
	}
}

rule("Player: START race")
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
		Set Objective Description(All Players(All Teams), String("{0}!", String("Go", Null, Null, Null), Null, Null),
			Visible To and String);
		Clear Status(Event Player, Rooted);
	}
}

rule("Player: SPRINTing increases speed")
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
		Modify Player Variable(Event Player, V, Add, 1);
		Set Move Speed(Event Player, Player Variable(Event Player, V));
		Wait(0.100, Ignore Condition);
		Loop If Condition Is True;
		Set Player Variable(Event Player, V, 100);
		Set Move Speed(Event Player, 100);
		Allow Button(Event Player, Secondary Fire);
	}
}

rule("Player: Death")
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
		Teleport(Event Player, Global Variable(A));
	}
}

rule("Player: Checkpoint 1 (Global variable F) (Moving lap start time to #4)")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Distance Between(Position Of(Event Player), Player Variable(Event Player, O)) <= 3;
		Player Variable(Event Player, O) == Global Variable(F);
	}

	actions
	{
		Set Player Variable(Event Player, O, Global Variable(G));
		Play Effect(Event Player, Ring Explosion Sound, White, Event Player, 200);
		Play Effect(Event Player, Good Explosion, Blue, Event Player, 2);
		disabled Set Player Variable At Index(Event Player, T, 1, Total Time Elapsed);
	}
}

rule("Player: Checkpoint 2 (Global variable g)")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Distance Between(Position Of(Event Player), Player Variable(Event Player, O)) <= 3;
		Player Variable(Event Player, O) == Global Variable(G);
	}

	actions
	{
		Modify Player Score(Event Player, 0);
		Set Player Variable(Event Player, O, Global Variable(H));
		Play Effect(Event Player, Ring Explosion Sound, White, Event Player, 200);
		Play Effect(Event Player, Good Explosion, Blue, Event Player, 2);
		Set Player Variable At Index(Event Player, T, 2, Total Time Elapsed);
	}
}

rule("Player: Checkpoint 3 (Global variable H)")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Distance Between(Position Of(Event Player), Player Variable(Event Player, O)) <= 3;
		Player Variable(Event Player, O) == Global Variable(H);
	}

	actions
	{
		Modify Player Score(Event Player, 0);
		Set Player Variable(Event Player, O, Global Variable(I));
		Play Effect(Event Player, Ring Explosion Sound, White, Event Player, 200);
		Play Effect(Event Player, Good Explosion, Blue, Event Player, 2);
		Set Player Variable At Index(Event Player, T, 3, Total Time Elapsed);
	}
}

rule("Player: Checkpoint 4 (global variable I) lap-start-time=pv(t[1]) last-lap-time=pv(t[0]) previous-last-lap-time=pv(t[11])")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Distance Between(Position Of(Event Player), Player Variable(Event Player, O)) <= 3;
		Player Variable(Event Player, O) == Global Variable(I);
	}

	actions
	{
		Modify Player Score(Event Player, 1);
		Set Player Variable(Event Player, O, Global Variable(F));
		Play Effect(Event Player, Buff Explosion Sound, White, Event Player, 200);
		Play Effect(Event Player, Good Explosion, Blue, Event Player, 2);
		Set Player Variable At Index(Event Player, T, 11, Value In Array(Player Variable(Event Player, T), 0));
		Skip If(Compare(Value In Array(Player Variable(Event Player, T), 4), ==, 0), 1);
		Set Player Variable At Index(Event Player, T, 0, Subtract(Total Time Elapsed, Value In Array(Player Variable(Event Player, T),
			4)));
		Set Player Variable At Index(Event Player, T, 4, Total Time Elapsed);
		Skip If(Compare(Value In Array(Player Variable(Event Player, T), 11), ==, 0), 1);
		Set Player Variable At Index(Event Player, T, 12, Subtract(Value In Array(Player Variable(Event Player, T), 0), Value In Array(
			Player Variable(Event Player, T), 11)));
		Big Message(Event Player, String("{0} - {1}", String("{0} sec", Value In Array(Player Variable(Event Player, T), 0), Null, Null),
			String("{0} m/s", Horizontal Speed Of(Event Player), Null, Null), Null));
		Small Message(All Players(All Teams), String("{0} - {1} - {2}", Event Player, String("{0} sec", Value In Array(Player Variable(
			Event Player, T), 0), Null, Null), String("{0} m/s", Horizontal Speed Of(Event Player), Null, Null)));
		Skip If(Or(Compare(Value In Array(Player Variable(Event Player, T), 0), ==, 0), And(Compare(Value In Array(Global Variable(T), 1),
			!=, 0), Compare(Value In Array(Player Variable(Event Player, T), 0), >=, Value In Array(Global Variable(T), 1)))), 2);
		Set Global Variable At Index(T, 0, Event Player);
		Set Global Variable At Index(T, 1, Value In Array(Player Variable(Event Player, T), 0));
		Skip If(And(Compare(Global Variable(T), !=, 0), Compare(Horizontal Speed Of(Event Player), <=, Value In Array(Global Variable(T),
			3))), 2);
		Set Global Variable At Index(T, 2, Event Player);
		Set Global Variable At Index(T, 3, Horizontal Speed Of(Event Player));
	}
}

rule("Speed: < 20 m/s.  PV(e)=current-speed-effect")
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
		Destroy Effect(Player Variable(Event Player, E));
		Clear Status(Event Player, Burning);
	}
}

rule("Speed: 20-29 m/s")
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
		Destroy Effect(Player Variable(Event Player, E));
		Create Effect(All Players(All Teams), Sparkles, Red, Event Player, 0.500, Visible To Position and Radius);
		Set Player Variable(Event Player, E, Last Created Entity);
		Clear Status(Event Player, Burning);
	}
}

rule("Speed: 30-39 m/s")
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
		Destroy Effect(Player Variable(Event Player, E));
		Create Effect(All Players(All Teams), Sparkles, Blue, Event Player, 0.500, Visible To Position and Radius);
		Set Player Variable(Event Player, E, Last Created Entity);
		Clear Status(Event Player, Burning);
	}
}

rule("Speed: 40-49 m/s")
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
		Destroy Effect(Player Variable(Event Player, E));
		Set Status(Event Player, Null, Burning, 9999);
	}
}

rule("Speed: 50-59 m/s")
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
		Set Player Variable(Event Player, E, Last Created Entity);
	}
}

rule("HUD: PLAYER variable V (Disabled)")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Compare(0, >, 1) == True;
	}

	actions
	{
		Create HUD Text(Event Player, Null, Player Variable(Event Player, V), Null, Right, 0, White, White, White, Visible To and String);
	}
}

rule("Map: Ilios RUINS")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 66;
	}

	actions
	{
		Set Global Variable(A, Vector(-72.544, 66.333, -181.805));
		Set Global Variable(F, Vector(-4.601, 62.915, -174.538));
		Set Global Variable(G, Vector(71.390, 61.915, -142.976));
		Set Global Variable(H, Vector(116.970, 66.940, -178.668));
		Set Global Variable(I, Vector(-58.256, 61.915, -157.274));
		Set Global Variable(P, Vector(53.206, 61.178, -142.624));
		Set Global Variable(J, Vector(28.592, 61.477, -177.477));
	}
}

rule("Map: Ilios WELL")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == -167.000;
	}

	actions
	{
		Set Global Variable(A, Vector(-286.063, -1.751, 30.179));
		Set Global Variable(F, Vector(-267.855, 0.248, 50.609));
		Set Global Variable(G, Vector(-148.738, -1.858, -90.319));
		Set Global Variable(H, Vector(-206.501, 2.552, -41.728));
		Set Global Variable(I, Vector(-244.454, 6.634, 1.231));
		Set Global Variable(J, Vector(-179.371, -4.284, -66.039));
	}
}

rule("Map: Nepal sanctum")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 101;
	}

	actions
	{
		Set Global Variable(A, Vector(81.066, 133.298, 97.544));
		Set Global Variable(F, Vector(73.735, 130.455, 65.927));
		Set Global Variable(G, Vector(83.193, 130.002, 0.004));
		Set Global Variable(H, Vector(81.094, 132.175, -91.555));
		Set Global Variable(I, Vector(66.536, 133.548, 0.146));
		Set Global Variable(P, Vector(50.774, 131.281, 0.344));
		Set Global Variable(J, Vector(56.632, 128.543, -28.085));
	}
}

rule("Map: Nepal SHRINE")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == -13.000;
	}

	actions
	{
		Set Global Variable(A, Vector(-22.729, 23.551, -51.067));
		Set Global Variable(F, Vector(-56.918, 20.551, -49.383));
		Set Global Variable(G, Vector(-63.930, 14.551, 0.033));
		Set Global Variable(H, Vector(-43.276, 16.550, 77.992));
		Set Global Variable(I, Vector(-22.766, 21.048, -86.716));
		Set Global Variable(P, Vector(-48.377, 20.443, 49.267));
	}
}

rule("Map: Nepal VILLAGE")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == -130.000;
	}

	actions
	{
		Set Global Variable(A, Vector(-143.752, -92.849, -77.601));
		Set Global Variable(F, Vector(-194.197, -96.901, -57.851));
		Set Global Variable(G, Vector(-200.141, -92.412, 32.803));
		Set Global Variable(H, Vector(-159.995, -88.447, 41.706));
		Set Global Variable(I, Vector(-159.389, -97.000, -77.233));
		Set Global Variable(P, Vector(-219.958, -92.025, 0.167));
	}
}

rule("Map: Lijang Control Center")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 13;
	}

	actions
	{
		Set Global Variable(A, Vector(0.008, 271.551, 259.632));
		Set Global Variable(F, Vector(-36.818, 268.551, 296.629));
		Set Global Variable(G, Vector(-44.488, 270.051, 352.656));
		Set Global Variable(H, Vector(65.412, 268.994, 347.295));
		Set Global Variable(I, Vector(-0.047, 276.048, 279.775));
		Set Global Variable(J, Vector(3.080, 268.881, 279.987));
		Set Global Variable(P, Vector(-0.210, 271.551, 327.437));
	}
}

rule("Map: Lijang GARDEN")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 102;
	}

	actions
	{
		Set Global Variable(A, Vector(-82.982, 97.551, 146.107));
		Set Global Variable(F, Vector(-0.102, 95.553, 178.068));
		Set Global Variable(G, Vector(81.749, 97.549, 147.776));
		Set Global Variable(H, Vector(0.198, 97.550, 112.265));
		Set Global Variable(I, Vector(-53.108, 99.551, 162.154));
	}
}

rule("Map: Lijang Night Market")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 69;
	}

	actions
	{
		Set Global Variable(A, Vector(49.231, 3.548, -29.262));
		Set Global Variable(F, Vector(25.312, 7.577, -32.350));
		Set Global Variable(G, Vector(0.640, 1.553, -9.778));
		Set Global Variable(H, Vector(-74.108, 7.052, -7.637));
		Set Global Variable(I, Vector(0.707, 0.551, -74.166));
		Set Global Variable(J, Vector(-43.578, 4.455, -14.037));
	}
}

rule("Map: blizzworld (old, short course) (disabled)")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 10000;
	}

	actions
	{
		Set Global Variable(A, Vector(100, 0, 100));
		Set Global Variable(F, Vector(3.054, -3.548, 16.216));
		Set Global Variable(G, Vector(3.222, 3.451, 74.522));
		Set Global Variable(H, Vector(3.046, 1.452, 12.643));
		Set Global Variable(I, Vector(3.145, -2.548, -14.286));
	}
}

rule("Map: Black Forest")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 37;
	}

	actions
	{
		Set Global Variable(A, Vector(30.934, 13.548, -25.407));
		Set Global Variable(F, Vector(9.421, 12.352, -23.618));
		Set Global Variable(G, Vector(-4.242, 19.012, 26.683));
		Set Global Variable(H, Vector(21.050, 10.549, 3.743));
		Set Global Variable(I, Vector(-5.384, 19.674, 5.147));
	}
}

rule("Map: Castillo")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == -71.000;
	}

	actions
	{
		Set Global Variable(A, Vector(-99.446, 34.117, 95.936));
		Set Global Variable(F, Vector(-74.458, 33.545, 53.015));
		Set Global Variable(G, Vector(-116.700, 35.054, 71.260));
		Set Global Variable(H, Vector(-114.279, 34.115, 27.336));
		Set Global Variable(I, Vector(-116.184, 40.126, 61.024));
		Set Global Variable(J, Vector(-126.525, 35.157, 63.400));
	}
}

rule("Map: Chateau Guillard")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 168;
	}

	actions
	{
		Set Global Variable(A, Vector(176.423, 1.548, 50.697));
		Set Global Variable(F, Vector(229.287, 14.548, 61.252));
		Set Global Variable(G, Vector(196.278, 10.548, 110.570));
		Set Global Variable(H, Vector(205.996, 1.549, 77.493));
		Set Global Variable(I, Vector(183.733, 9.552, 55.824));
		Set Global Variable(P, Vector(217.846, 5.455, 111.749));
	}
}

rule("Map: Dorado")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 144;
	}

	actions
	{
		Set Global Variable(A, Vector(64.906, 15.445, 13.343));
		Set Global Variable(F, Vector(87.163, 14.301, 38.397));
		Set Global Variable(G, Vector(129.210, 13.563, 51.221));
		Set Global Variable(H, Vector(127.208, 10.568, -22.372));
		Set Global Variable(I, Vector(106.500, 19.551, -3.577));
		Set Global Variable(P, Vector(108.416, 11.595, -30.638));
		Set Global Variable(J, Vector(85.021, 20.455, 17.189));
	}
}

rule("Map: Ecopoint Antarctica")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 18;
	}

	actions
	{
		Set Global Variable(A, Vector(13.481, 9.583, 59.073));
		Set Global Variable(F, Vector(1.315, 13.550, 11.958));
		Set Global Variable(G, Vector(-2.517, 8.550, -54.684));
		Set Global Variable(H, Vector(-10.421, 10.078, -0.128));
		Set Global Variable(I, Vector(-2.315, 8.551, 54.888));
		Set Global Variable(P, Vector(18.091, 4.467, -0.057));
	}
}

rule("Map: Eichenwalde")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 124;
	}

	actions
	{
		Set Global Variable(A, Vector(88.290, 14.413, -65.328));
		Set Global Variable(F, Vector(102.680, 14.272, -109.216));
		Set Global Variable(G, Vector(37.166, 6.802, -52.677));
		Set Global Variable(H, Vector(46, 21.382, -67.745));
		Set Global Variable(I, Vector(86.682, 14.196, -73.278));
		Set Global Variable(P, Vector(67.223, 6.023, -83.701));
	}
}

rule("Map: Hanamura")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 78;
	}

	actions
	{
		Set Global Variable(A, Vector(28.146, 1.551, -3.264));
		Set Global Variable(F, Vector(-4.208, 5.517, 21.625));
		Set Global Variable(G, Vector(31.664, 3.348, -60.397));
		Set Global Variable(H, Vector(-21.068, 2.551, -94.907));
		Set Global Variable(I, Vector(13.384, 8.292, -21.430));
		Set Global Variable(P, Vector(21.902, -2.545, -82.880));
	}
}

rule("Map: Hollywood")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 25;
	}

	actions
	{
		Set Global Variable(A, Vector(-7.158, 3.157, -7.794));
		Set Global Variable(F, Vector(7.051, 3.298, -31.029));
		Set Global Variable(G, Vector(49.450, 4.301, -56.437));
		Set Global Variable(H, Vector(-26.131, 7.299, -46.007));
		Set Global Variable(I, Vector(-9.048, 8.684, -40.798));
		Set Global Variable(P, Vector(24.891, 2.641, -75.941));
	}
}

rule("Map: Horizon Lunar Colony")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 56;
	}

	actions
	{
		Set Global Variable(A, Vector(37.629, 8.552, -54.195));
		Set Global Variable(F, Vector(54.533, 9.549, -100.802));
		Set Global Variable(G, Vector(-46.156, 6.053, -101.304));
		Set Global Variable(H, Vector(-5.621, 11.551, -118.123));
		Set Global Variable(I, Vector(81.436, 7.992, -40.463));
		Set Global Variable(P, Vector(11.993, 7.953, -82.377));
		Set Global Variable(J, Vector(-27.897, 3.660, -126.608));
	}
}

rule("Player: Damage = slow (doesn't work, damage not possible)")
{
	event
	{
		Player took damage;
		All;
		All;
	}

	actions
	{
		Modify Player Variable(Event Player, V, Multiply, 0.500);
	}
}

rule("Map: ilios lighthouse")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 300;
	}

	actions
	{
		Set Global Variable(A, Vector(261.795, -22.372, -95.124));
		Set Global Variable(F, Vector(289.804, -23.922, -88.970));
		Set Global Variable(G, Vector(331.342, -17.089, -56.983));
		Set Global Variable(H, Vector(311.854, -19.841, 24.068));
		Set Global Variable(I, Vector(357.445, -24.089, -51.463));
		Set Global Variable(P, Vector(319.563, -23.210, -35.596));
	}
}

rule("Map: King's Row")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 17;
	}

	actions
	{
		Set Global Variable(A, Vector(-36.584, 1.409, -24.038));
		Set Global Variable(F, Vector(-95.482, -0.944, -46.273));
		Set Global Variable(G, Vector(-72.179, 6.052, -37.340));
		Set Global Variable(H, Vector(-61.783, 1.550, 2.889));
		Set Global Variable(I, Vector(-48.632, 10.672, -37.568));
		Set Global Variable(P, Vector(-30.029, 6.512, -47.474));
		Set Global Variable(J, Vector(-79.831, 0.902, -19.140));
	}
}

rule("Map: Oasis City Center")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 186;
	}

	actions
	{
		Set Global Variable(A, Vector(191.681, 2.399, 175.784));
		Set Global Variable(F, Vector(120.149, 2.551, 223.825));
		Set Global Variable(G, Vector(119.893, 2.548, 223.766));
		Set Global Variable(H, Vector(98.544, 2.401, 299.925));
		Set Global Variable(I, Vector(197.784, 2.293, 214.365));
	}
}

rule("Player: Set score variable")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	actions
	{
		Set Player Variable(Event Player, S, Score Of(Event Player));
	}
}

rule("Player: Victory")
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

rule("Player: Splat!")
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
		Global Variable(T) != 0;
	}

	actions
	{
		Destroy HUD Text(Value In Array(Global Variable(U), 0));
		Create HUD Text(All Players(All Teams), String("{0} {1}", String("Best", Null, Null, Null), String("Time", Null, Null, Null),
			Null), Value In Array(Global Variable(T), 0), String("{0} sec", Value In Array(Global Variable(T), 1), Null, Null), Left, 0,
			Yellow, White, White, Visible To and String);
		Set Global Variable At Index(U, 0, Last Text ID);
		Destroy HUD Text(Value In Array(Global Variable(U), 1));
		Create HUD Text(All Players(All Teams), String("Fastest", Null, Null, Null), Value In Array(Global Variable(T), 2), String(
			"{0} m/s", Value In Array(Global Variable(T), 3), Null, Null), Left, 1, Blue, White, White, Visible To and String);
		Set Global Variable At Index(U, 1, Last Text ID);
		Play Effect(All Players(All Teams), Buff Impact Sound, White, Global Variable(I), 500);
		Destroy Effect(Value In Array(Global Variable(U), 2));
		Create Effect(All Players(All Teams), Good Aura, Yellow, Value In Array(Global Variable(T), 0), 1, Visible To Position and Radius);
		Set Global Variable At Index(U, 2, Last Created Entity);
		Destroy Effect(Value In Array(Global Variable(U), 3));
		Create Effect(All Players(All Teams), Good Aura, Blue, Value In Array(Global Variable(T), 2), 1, Visible To Position and Radius);
		Set Global Variable At Index(U, 3, Last Created Entity);
	}
}

rule("Player: Speedometer")
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

rule("Player: Splat! (chance of voice line)")
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

rule("Players: score over head")
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
		Create In-World Text(All Players(All Teams), String("{0} / {1}", Round To Integer(Score Of(Event Player), Up), 15, Null),
			Event Player, 0.750, Clip Against Surfaces, Visible To Position and String);
	}
}

rule("Rockets: Reduce speed (doesn't work, no damage possible)")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Is Firing Secondary(Event Player) == True;
	}

	actions
	{
		Modify Player Variable(Event Player, V, Multiply, 0.750);
	}
}

rule("PLAYERS: Chance to say hello in proxmity")
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

rule("PLAYERS: Chance to say voice line within 2m")
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

rule("POWERUP: GV(P), PV(q)=powerup-eligible")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Global Variable(P) != 0;
		Distance Between(Position Of(Event Player), Global Variable(P)) <= 2;
		Is Moving(Event Player) == True;
		Player Variable(Event Player, Q) == True;
	}

	actions
	{
		Big Message(Event Player, String("Faster", Null, Null, Null));
		Modify Player Variable(Event Player, V, Add, 50);
		Set Move Speed(Event Player, Player Variable(Event Player, V));
		Play Effect(All Players(All Teams), Good Pickup Effect, Purple, Event Player, 3);
		Play Effect(All Players(All Teams), Buff Explosion Sound, Purple, Event Player, 10);
		Set Player Variable(Event Player, Q, False);
		Wait(10, Ignore Condition);
		Set Player Variable(Event Player, Q, True);
	}
}

rule("SLOWDOWN: Slowdown=GV(s), PV(s)=slowdown-eligible")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Global Variable(S) != 0;
		Distance Between(Position Of(Event Player), Global Variable(S)) <= 3;
		Is Moving(Event Player) == True;
		Player Variable(Event Player, S) == True;
		Player Variable(Event Player, V) > 100;
	}

	actions
	{
		Big Message(Event Player, String("Slower", Null, Null, Null));
		Modify Player Variable(Event Player, V, Divide, 2);
		Set Move Speed(Event Player, Player Variable(Event Player, V));
		Play Effect(All Players(All Teams), Bad Pickup Effect, Red, Event Player, 3);
		Play Effect(All Players(All Teams), Debuff Impact Sound, Red, Event Player, 10);
		Set Player Variable(Event Player, S, False);
		Wait(5, Ignore Condition);
		Set Player Variable(Event Player, S, True);
	}
}

rule("MAP: blizzworld")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 54;
	}

	actions
	{
		Set Global Variable(A, Vector(-79.475, 1.798, 64.043));
		Set Global Variable(F, Vector(-20.844, 3.551, 67.832));
		Set Global Variable(G, Vector(-70.002, 3.298, 151.324));
		Set Global Variable(H, Vector(-76.901, 6.423, 66.433));
		Set Global Variable(I, Vector(-64.419, 2.149, 120.448));
		Set Global Variable(P, Vector(-45.947, 0.265, 97.890));
		Set Global Variable(S, Vector(-56.641, 6.064, 97.352));
		Set Global Variable(J, Vector(-56.948, 0.342, 68.803));
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
		Global Variable(Z) == 30;
	}

	actions
	{
		Set Global Variable(A, Vector(-30.324, 6.194, 26.667));
		Set Global Variable(F, Vector(-0.342, 2.279, 12.610));
		Set Global Variable(G, Vector(0.727, 4.195, -23.064));
		Set Global Variable(H, Vector(-32.312, 2.477, 0.142));
		Set Global Variable(I, Vector(23.102, 6.312, 0.159));
		Set Global Variable(P, Vector(-0.197, 2.377, -12.646));
	}
}

rule("Map: Oasis Gardens")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 173;
	}

	actions
	{
		Set Global Variable(A, Vector(78.296, 7.043, -258.281));
		Set Global Variable(F, Vector(103.226, 9.551, -285.832));
		Set Global Variable(G, Vector(187.719, 9.551, -217.596));
		Set Global Variable(H, Vector(131.932, 4.131, -203.530));
		Set Global Variable(I, Vector(121.509, 13.050, -227.400));
		Set Global Variable(P, Vector(141.221, 8, -247.455));
		Set Global Variable(J, Vector(139.459, 7.992, -233.119));
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
		Global Variable(Z) == -163.000;
	}

	actions
	{
		Set Global Variable(A, Vector(-214.050, 21.885, 71.249));
		Set Global Variable(F, Vector(-172.085, 17.548, 54.001));
		Set Global Variable(G, Vector(-187.928, 12.800, -0.114));
		Set Global Variable(H, Vector(-210.744, 19.552, -59.180));
		Set Global Variable(I, Vector(-214.036, 20.552, -0.064));
		Set Global Variable(P, Vector(-188.029, 10.952, 0.129));
	}
}

rule("Map: Paris")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == -3.000;
	}

	actions
	{
		Set Global Variable(A, Vector(-0.398, 11.544, -49.909));
		Set Global Variable(F, Vector(-45.446, 17.538, 26.431));
		Set Global Variable(G, Vector(-74.471, 13.538, -15.836));
		Set Global Variable(H, Vector(-34.750, 15.538, -8.173));
		Set Global Variable(I, Vector(10.783, 11.472, -61.286));
		Set Global Variable(P, Vector(-20.005, 11.551, -19.144));
	}
}

rule("Map: Petra")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 41;
	}

	actions
	{
		Set Global Variable(A, Vector(-44.970, -11.460, 9.708));
		Set Global Variable(F, Vector(-18.397, 1.638, -8.515));
		Set Global Variable(G, Vector(23.850, -7.811, 52.938));
		Set Global Variable(H, Vector(-17.379, -6.496, 28.646));
		Set Global Variable(I, Vector(2.253, -8.430, 9.041));
		Set Global Variable(P, Vector(-30.081, -5.455, -0.093));
	}
}

rule("Map: Temple of anubis")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 21;
	}

	actions
	{
		Set Global Variable(A, Vector(-33.800, 12.147, 44.882));
		Set Global Variable(F, Vector(-53.794, 11.062, 30.387));
		Set Global Variable(G, Vector(3.973, 6.147, 113.761));
		Set Global Variable(H, Vector(-38.567, 7.052, 63.942));
		Set Global Variable(I, Vector(-4.797, 4.056, 45.794));
		Set Global Variable(P, Vector(-7.864, 1.329, 101.490));
		Set Global Variable(J, Vector(-21.240, 6.842, 63.166));
	}
}

rule("Map: Volskaya Industries")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(Z) == 34;
	}

	actions
	{
		Set Global Variable(A, Vector(-13.796, -1.433, 115.710));
		Set Global Variable(F, Vector(19.360, -0.433, 55.788));
		Set Global Variable(G, Vector(-83.112, 1.569, 19.198));
		Set Global Variable(H, Vector(-103.218, 3.568, 78.822));
		Set Global Variable(I, Vector(-0.310, 3.472, 88.048));
		Set Global Variable(P, Vector(-32.751, 3.569, 73.187));
	}
}

rule("Init: create Jump-pad GV(J)")
{
	event
	{
		Ongoing - Global;
	}

	conditions
	{
		Global Variable(J) != 0;
	}

	actions
	{
		Create Effect(All Players(All Teams), Ring, Yellow, Global Variable(J), 3, Visible To Position and Radius);
		Create Icon(All Players(All Teams), Global Variable(J), Arrow: Up, Visible To and Position, Yellow, False);
	}
}

rule("Player: Jump-pad gv(j)")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Distance Between(Event Player, Global Variable(J)) < 3;
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
		Player Variable(Event Player, C) > 0;
	}

	actions
	{
		Skip If(Compare(Player Variable(Event Player, U), ==, 0), 1);
		Destroy HUD Text(Value In Array(Player Variable(Event Player, U), 0));
		Create HUD Text(Event Player, String("Time", Null, Null, Null), String("{0} sec", Value In Array(Player Variable(Event Player, T),
			0), Null, Null), String("{0} sec", Player Variable(Event Player, C), Null, Null), Right, 0, White, White, White,
			Visible To and String);
		Set Player Variable At Index(Event Player, U, 0, Last Text ID);
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
		Value In Array(Player Variable(Event Player, T), 12) != 0;
	}

	actions
	{
		Create HUD Text(Event Player, Null, Null, String("{0}: {1}", String("Run", Null, Null, Null), Value In Array(Player Variable(
			Event Player, T), 12), Null), Right, 1, White, White, White, Visible To and String);
		Set Player Variable At Index(Event Player, U, 1, Last Text ID);
	}
}

rule("PLAYER STAT: Current lap time")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	conditions
	{
		Value In Array(Player Variable(Event Player, T), 4) > 0;
	}

	actions
	{
		Set Player Variable(Event Player, C, Subtract(Total Time Elapsed, Value In Array(Player Variable(Event Player, T), 4)));
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
		Global Variable(Z) == 88;
	}

	actions
	{
		Set Global Variable(A, Vector(-58.444, 5.499, -27.063));
		Set Global Variable(F, Vector(-96.363, 5.398, -39.877));
		Set Global Variable(G, Vector(-27.211, 13.551, -51.218));
		Set Global Variable(H, Vector(27.993, 11.548, -91.529));
		Set Global Variable(I, Vector(139.848, 7.609, -46.608));
		Set Global Variable(P, Vector(-54.604, 7.552, -72.609));
		Set Global Variable(S, Vector(119.274, 7.977, -46.749));
		Set Global Variable(J, Vector(118.265, 14.591, -58.110));
		Set Global Variable At Index(X, 1, Vector(34.374, 13.551, -56.896));
		Set Global Variable At Index(X, 2, Vector(34.451, 13.552, -62.568));
		Set Global Variable At Index(X, 3, Vector(64.840, 7.362, -83.169));
		Set Global Variable At Index(X, 4, Vector(68, 7.380, -83.559));
		Set Global Variable At Index(X, 5, Vector(62.254, 7.552, -93.216));
		Set Global Variable At Index(X, 6, Vector(58, 7.551, -93.216));
		Set Global Variable At Index(X, 7, Vector(4.606, 7.549, -96.175));
		Set Global Variable At Index(X, 8, Vector(0, 7.549, -96.175));
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
		Global Variable(Z) == 88;
	}

	actions
	{
		Set Global Variable(A, Vector(-58.444, 5.499, -27.063));
		Set Global Variable(F, Vector(-96.368, 5.397, -40.221));
		Set Global Variable(G, Vector(-49.486, 6.299, -32.768));
		Set Global Variable(H, Vector(-24.909, 11.552, -97.665));
		Set Global Variable(I, Vector(-25.282, 11.605, -71.365));
		Set Global Variable(P, Vector(-54.530, 7.551, -77.611));
		Set Global Variable(S, Vector(-15.853, 7.670, -64.424));
		Set Global Variable(J, Vector(-45.281, 11.532, -42.785));
		Set Global Variable At Index(X, 1, Vector(34.374, 13.551, -56.896));
		Set Global Variable At Index(X, 2, Vector(34.451, 13.552, -62.568));
		Set Global Variable At Index(X, 3, Vector(64.840, 7.362, -83.169));
		Set Global Variable At Index(X, 4, Vector(68, 7.380, -83.559));
		Set Global Variable At Index(X, 5, Vector(62.254, 7.552, -93.216));
		Set Global Variable At Index(X, 6, Vector(58, 7.551, -93.216));
		Set Global Variable At Index(X, 7, Vector(4.606, 7.549, -96.175));
		Set Global Variable At Index(X, 8, Vector(0, 7.549, -96.175));
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
	}

	actions
	{
		Create Effect(All Players(All Teams), Orb, Green, Value In Array(Global Variable(X), 1), 3, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Orb, Red, Value In Array(Global Variable(X), 2), 3, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Orb, Green, Value In Array(Global Variable(X), 3), 3, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Orb, Red, Value In Array(Global Variable(X), 4), 3, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Orb, Green, Value In Array(Global Variable(X), 5), 3, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Orb, Red, Value In Array(Global Variable(X), 6), 3, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Orb, Green, Value In Array(Global Variable(X), 7), 3, Visible To Position and Radius);
		Create Effect(All Players(All Teams), Orb, Red, Value In Array(Global Variable(X), 8), 3, Visible To Position and Radius);
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
		Distance Between(Event Player, Value In Array(Global Variable(X), 1)) <= 3;
	}

	actions
	{
		Play Effect(All Players(All Teams), Ring Explosion, Green, Value In Array(Global Variable(X), 1), 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Green, Value In Array(Global Variable(X), 1), 10);
		Teleport(Event Player, Value In Array(Global Variable(X), 2));
		Play Effect(All Players(All Teams), Ring Explosion, Red, Value In Array(Global Variable(X), 2), 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Red, Value In Array(Global Variable(X), 2), 10);
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
		Distance Between(Event Player, Value In Array(Global Variable(X), 3)) <= 3;
	}

	actions
	{
		Play Effect(All Players(All Teams), Ring Explosion, Green, Value In Array(Global Variable(X), 3), 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Green, Value In Array(Global Variable(X), 3), 10);
		Teleport(Event Player, Value In Array(Global Variable(X), 4));
		Play Effect(All Players(All Teams), Ring Explosion, Red, Value In Array(Global Variable(X), 4), 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Red, Value In Array(Global Variable(X), 4), 10);
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
		Distance Between(Event Player, Value In Array(Global Variable(X), 5)) <= 3;
	}

	actions
	{
		Play Effect(All Players(All Teams), Ring Explosion, Green, Value In Array(Global Variable(X), 5), 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Green, Value In Array(Global Variable(X), 5), 10);
		Teleport(Event Player, Value In Array(Global Variable(X), 6));
		Play Effect(All Players(All Teams), Ring Explosion, Red, Value In Array(Global Variable(X), 6), 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Red, Value In Array(Global Variable(X), 6), 10);
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
		Distance Between(Event Player, Value In Array(Global Variable(X), 7)) <= 3;
	}

	actions
	{
		Play Effect(All Players(All Teams), Ring Explosion, Green, Value In Array(Global Variable(X), 7), 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Green, Value In Array(Global Variable(X), 7), 10);
		Teleport(Event Player, Value In Array(Global Variable(X), 8));
		Play Effect(All Players(All Teams), Ring Explosion, Red, Value In Array(Global Variable(X), 8), 10);
		Play Effect(All Players(All Teams), Ring Explosion Sound, Red, Value In Array(Global Variable(X), 8), 10);
	}
}
