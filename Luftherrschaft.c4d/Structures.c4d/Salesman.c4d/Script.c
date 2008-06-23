/*-- Händler --*/

#strict 2
#include L156

local ObjsWeapon, ObjsMaterial;

protected func Initialize()
{
  SetAction("NoTrader");

  var aArray;
  var Count = RandomX(7,17);
  ObjsWeapon = [ARWP,XARP,FARP,FLNT,FBMP,GUNP,SFLN,EFLN,STFN,TFLN,ARWP,XARP,FARP,FLNT,FBMP,GUNP,SFLN,EFLN,STFN,TFLN];
  ObjsMaterial = [COAL,CNCR,CRYS,LOAD,GOLD,METL,ORE1,WOOD,SPHR,BARL,CNKT,FLAG,LNKT,COAL,CNCR,CRYS,LOAD,GOLD,METL,ORE1,WOOD,SPHR,BARL,CNKT,FLAG,LNKT];

  while(Count > 0)
  {
   if(Random(2))
   {
    aArray = ObjsWeapon[Random(GetLength(ObjsWeapon))];
    CreateContents(aArray,this());
    Count--;
    ObjsWeapon--;
    DeleteArrayItem(GetArrayItemPosition(aArray, ObjsWeapon), ObjsWeapon);
   }
   else
   {
    aArray = ObjsMaterial[Random(GetLength(ObjsMaterial))];
    CreateContents(aArray,this());
    Count--;
    ObjsMaterial--;
    DeleteArrayItem(GetArrayItemPosition(aArray, ObjsMaterial), ObjsMaterial);
   }
  }
}

private func Trader_comes()
{

}


private func Trader_visible()
{
  ScheduleCall(0,"SetAction(\"TradermakeVisible\")",6000);
}

public func ControlThrow()
{
  var a = ["Dieb!","Verschwinde, das sind meine Waren!","Du hast hier nichts verloren!","Finger weg!"];
  Message(a[Random(GetLength(a))], this());
  return(1);
}

protected func ControlUp(object pClonk)
{
  if (GetAction() != "TaderIsVisible") return;
  CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
  AddMenuItem("","",GetID(this()),pClonk,0,0,0);
  AddMenuItem("Kann ich ihnen helfen?","",0,pClonk,0,0,0);
  AddMenuItem("Zeig mir deine Ware.","BuyMenu",0,pClonk,0,pClonk,0);
  AddMenuItem("Auf Wiedersehen!","EndTalk",0,pClonk,0,pClonk,0); 
}

protected func BuyMenu(id ID, object pClonk)
{
  CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
  AddMenuItem("","",GetID(this()),pClonk,0,0,0);
  var i;
  while(Contents(i,0))
  {
   AddMenuItem(Format("%s kaufen: %d {{GOLD}}",GetName(Contents(i,0)),GetValue(Contents(i,0))),"Buy",GetID(Contents(i,0)),pClonk,0,pClonk,0);
   i++;
  }
  AddMenuItem("Auf Wiedersehen!","EndTalk",0,pClonk,0,pClonk,0); 
}

protected func Buy(id ID, object pClonk)
{
  if (GetWealth(GetOwner(pClonk))>=GetValue(0,ID)) if (ContentsCount(,pClonk) <= 3)
  {
   CreateContents(ID, pClonk);
   DoWealth(GetOwner(pClonk),-GetValue(0,ID));
   RemoveObject(FindContents(ID,this()));
   Sound("UnCash");
  }
  else
   Message("%s hat nicht genug|Platz im Inventar!", pClonk, GetName(pClonk));
  else
   Sound("CommandFailure1");
  BuyMenu(0,pClonk);
}

public func AddItem()
{
  if (ContentsCount() > 20) return;
  var AddObjsWeapon = [ARWP,XARP,FARP,FLNT,FBMP,GUNP,SFLN,EFLN,STFN,TFLN];
  var AddObjsMaterial = [COAL,CNCR,CRYS,LOAD,GOLD,METL,ORE1,WOOD,SPHR,BARL,CNKT,FLAG,LNKT];
  if (Random(2))
   CreateContents(AddObjsWeapon[Random(GetLength(AddObjsWeapon))],this());
  else
   CreateContents(AddObjsMaterial[Random(GetLength(AddObjsMaterial))],this());
}

protected func EndTalk(id ID, object pClonk)
{
  CloseMenu(pClonk);
}