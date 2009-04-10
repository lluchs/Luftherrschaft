/*-- Lasso --*/

#strict 2
#include LHOK // basiert auf dem Enterhaken

local strength; // Stärke mit der geworfen wird

func Initialize() {
  strength=100;
  return _inherited(...);
}

func Departure(object pObj, int iAltXDir, int iAltYDir) {
  if(!iAltXDir && !iAltYDir) {
    iAltXDir=GetXDir()*2;
    iAltYDir=GetYDir()*3;
  }
  // verfolgen
  SetPlrView(GetOwner(pObj),this);
  // ausführen
  _inherited(pObj,iAltXDir*strength/100,iAltYDir*strength/100);  
  Rope->SetLength(1000);
  return 1;
}

/* Steuerung */

func ControlDigDouble(object pClonk) {
  [$TxtChangeStrength$]
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
  // verlangsamen
  SetSpeed(GetXDir()/2,GetYDir()/2);
  return 0;
}

/* ActionCall */

func FindConnect() {
  for(var obj in FindObjects(Find_Distance(10),Find_Exclude(this),Find_Exclude(Rope),Find_NoContainer(),Find_Exclude(Contained(RopeEnd)),Find_OCF(OCF_Living | OCF_Grab),Find_Not(Find_Func("IsTree")))) {
    Rope->SetAction("ConnectSingle",RopeEnd,obj);
    Rope->~SetLength(Rope->~CalcLength());
    Sound("Connect");
    Message("%s gefangen!",RopeEnd,GetName(obj));
    // Lasso verschwindet dann
    RemoveObject();
  }
}

private func GetActNumber() {
  if(Rope->~GetActionTarget(0) == this) return 0;
  return 1;
}
