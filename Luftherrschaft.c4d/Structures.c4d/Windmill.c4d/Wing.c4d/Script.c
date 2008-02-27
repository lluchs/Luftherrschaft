#strict

protected func Initialize(){
	SetAction("Turn");
	//Log("Init Wing");
	return 1;
}

protected func Wind2Turn()
{
  DoEnergy(Abs(GetWind() / 4 * 5), GetActionTarget() );
  //SetRDir(GetWind() / 4);
}

