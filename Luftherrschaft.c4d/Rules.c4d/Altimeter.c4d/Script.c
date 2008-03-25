/*-- Altimeter --*/

#strict

protected func InitializePlayer(int iPlr)
{
 if(ObjectCount2(Find_ID(AR4Z)))
  CreateAltimeter(iPlr);
}

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}

global func CreateAltimeter(int iPlayer)
{
 var altimeter=CreateObject(AM4Z,50,GetDefCoreVal("Height","DefCore",AM4Z)+100,iPlayer);
 altimeter->SetVisibility(VIS_Owner);
 altimeter->DoInit();
}

global func RemoveAltimeter(int iPlayer)
{
 var alti=FindObject2(Find_ID(AM4Z),Find_Owner(iPlayer));
 if(!alti)return 0;
 alti->RemoveObject();
 return 1;
}
