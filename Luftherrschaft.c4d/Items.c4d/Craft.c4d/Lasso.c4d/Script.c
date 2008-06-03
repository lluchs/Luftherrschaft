/*-- Lasso --*/

#strict 2
#include LHOK // basiert auf dem Enterhaken

local strength; // Stärke mit der geworfen wird

func Departure(object pObj, int iAltXDir, int iAltYDir) {
  if(!iAltXDir && !iAltYDir) {
    iAltXDir=GetXDir()*2;
    iAltYDir=GetYDir()*3;
  }
  // verfolgen
  SetPlrView(GetOwner(pObj),this);
  return _inherited(pObj,iAltXDir*strength/100,iAltYDir*strength/100);  
}

/* Steuerung */

func ControlDigDouble(object pClonk) {
  strength += 10;
  if(strength > 140) strength = 40;
  Message("$Strength$",pClonk,strength);
}

/* Aufschlag */

public func Hit() {
  Sound("Woodhit*");
  if(!Rope) return 0;
  Rope->SetAction("ConnectSingle",RopeEnd,this);
  Rope->~SetLength(Rope->~CalcLength());
  return 0;
}

/* ActionCall */

func FindConnect() {
  for(var obj in FindObjects(Find_AtPoint(), Find_Exclude(this), Find_Exclude(Rope), Find_OCF(OCF_Living | OCF_Grab | OCF_Chop | OCF_Collectible))) {
    Rope->~SetObject(obj, GetActNumber());
    Sound("Connect");
    Message("Gefangen!",obj);
    // Lasso verschwindet dann
    RemoveObject();
  }
}

public func CanConnect(object pObj) {
  // soll trotzdem verbinden?
  if(pObj->~LassoConnect()) return 1;
  // keine Bäume (hat zu viele)
  if(pObj->~IsTree()) return 0;
  // keine Gebäude
  if(GetCategory(pObj) & C4D_Structure) return 0;
  // keine sammelbaren Objekte
  if(GetOCF(pObj) & OCF_Collectible) return 0;
  // sonst schon
  return 1;
}

private func GetActNumber() {
  if(Rope->~GetActionTarget(0) == this) return 0;
  return 1;
}