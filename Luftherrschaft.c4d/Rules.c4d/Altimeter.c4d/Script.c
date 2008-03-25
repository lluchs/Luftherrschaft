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
 var altimeter=CreateObject(AM4Z,AbsX(50),AbsY(GetDefCoreVal("Height","DefCore",AM4Z)+100),iPlayer);
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

protected func Initialize()
{
 for(var cnt=0;cnt<GetPlayerCount();cnt++)
 {
  var iPlr=GetPlayerByIndex(cnt);
  if(!FindObject2(Find_ID(AM4Z),Find_Owner(iPlr))) CreateAltimeter(iPlr);
 }
} 

protected func Destruction()
{
 if(ObjectCount2(Find_ID(AR4Z)) > 1)return;
 for(var cnt=0;cnt<GetPlayerCount();cnt++)
 {
  var iPlr=GetPlayerByIndex(cnt);
  RemoveAltimeter(iPlr);
 }
}

