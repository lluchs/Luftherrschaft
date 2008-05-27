/*-- Der Enterhaken --*/

#strict 2

local Rope, RopeEnd;

public func Departure(object pContainer,int iAlternateXDir,int iAlternateYDir) {
  SetAction("Idle");
  if(!GetXDir()) return 0;
  if(Rope) return 0;
  RopeEnd = CreateObject(LRPE, 0, 0, GetOwner());
  Enter(pContainer, RopeEnd);
  Rope = CreateObject(171E, 0, 0, GetOwner());
  Rope->Activate(RopeEnd, this);
  RopeEnd->~SetRope(Rope, this);
  var iXDir,iYDir;
  iXDir = GetXDir() * 2;
  iYDir = GetYDir() * 3;
  if(iAlternateXDir || iAlternateYDir) {
    Exit(this, 0, -12);
    SetSpeed(iAlternateXDir,iAlternateYDir);
  }
  else
    SetSpeed(iXDir, iYDir);
  Sound("Catapult");
  return 0;
}

public func Entrance() {
  SetAction("Idle");
  return 0;
}

public func ControlThrow(object pClonk) {
  var iXDir;
  var iYDir;
  if(GetProcedure(pClonk) == "FLIGHT") {
    iXDir = GetXDir(pClonk) * 2;
    iYDir = Abs(GetYDir(pClonk) * 3) * -1;
    if(iYDir < -30) iYDir = -30;
    return Departure(pClonk, iXDir, iYDir);
  }
  return 0;
}

/* Aufschlag */

public func Hit() {
  Sound("Woodhit*");
  if(!Rope) return 0;
  // Anhalten
  SetSpeed();
  Rope->SetAction("ConnectSingle",RopeEnd,this);
  Rope->~SetLength(Rope->~CalcLength());
  return 0;
}

/* ContactCalls */

public func ContactLeft() { return SetAction("Hookleft"); }
public func ContactRight() { return SetAction("Hookright"); }
public func ContactBottom() { return SetAction("Hookdown"); }
public func ContactTop() { return SetAction("Hookup"); }

/* Seil gerissen */

func RopeBreak() {
  Rope=0;                                       
  if(RopeEnd) RemoveObject(RopeEnd);
  SetAction("Idle");                               
  return 0;
}