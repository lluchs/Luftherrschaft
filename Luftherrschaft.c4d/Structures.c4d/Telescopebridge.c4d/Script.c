/*-- Teleskopbrücke --*/

#strict 2

local iStep, iSpecial, fUsed;

protected func Initialize()
{
  
  return 1;
}

protected func ControlDigDouble()
{
  CheckNextStep();
  return 1;
}

protected func CheckNextStep()
{
  if (GBackSemiSolid(2+(12*iStep),10)) if (iStep) { SetAction(Format("FreezedStep%d",iStep)); return 1; }
  //CreateObject(ROCK,2+(12*iStep),10)->SetCategory(1);
  SetAction(Format("Step%d",iStep));
  iStep++;
  fUsed=true;
  return 1;
}

protected func ChangeSolidMask()
{
  var iStep2 = iStep-1;
  if (GetAction() != Format("Step%d",iStep2)) return 0;
  var iX, iY, iX2, iY2;
  var Phas = GetPhase()+1;
  SetSolidMask(27+(100*Phas)+(800*iStep2),14,14+iSpecial+(Phas)+(8*iStep2),5,27,14);
  //Log("%d %d %d %d %d %d",iX,iY,iX2,iY2, Phas, iStep2);
  iSpecial++;
  return 1;
}

protected func SetSolidMaskEnding()
{
  if (iStep = 1) SetSolidMask(927,14,24,5,27,14);
  if (iStep = 2) SetSolidMask(3227,14,39,5,27,14);
  if (iStep = 3) SetSolidMask(3227,14,60,5,27,14);
  if (iStep = 4) SetSolidMask(3227,14,73,5,27,14);
  return 1;
}