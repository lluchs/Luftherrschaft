#strict

//protected func Initialize(){SetAction("Turn");return(1);}

func Completion()
{
  SetR(Random(360));
}

func Wind2Turn()
{
  DoEnergy(Abs(GetRDir() * 5), GetActionTarget() );
  SetRDir(GetWind() / 4);
}
