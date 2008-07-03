/*-- Kanone --*/

#strict 2

local rdy2fire;

public func TurnLeft(object pCaller) {
  SetPhase(GetPhase() - 1);
}

public func TurnRight(object pCaller) {
  SetPhase(GetPhase() + 1);
}

public func CannonFire(object pFire) {
  if(rdy2fire)
    return 0;
  var iAngle;
  iAngle = 285 + GetPhase() * 15;
  // Austrittspunkt und -geschwindigkeit
  var iX,iY,iXDir,iYDir;
  iX = Sin(iAngle,16);
  iY = Cos(iAngle,16);
  iXDir = +Sin(iAngle, ProjSpeed());
  iYDir = -Cos(iAngle, ProjSpeed());
  Enter(this,pFire);
  Exit(pFire, iX, -iY, Random(360));
  SetXDir(iXDir,pFire,10);
  SetYDir(iYDir,pFire,10);
  Sound(ShootSound());
  rdy2fire = true;
  ScheduleCall(this, "SetReadyState", ReloadTime(), 0, false);
  SetPlrView(GetOwner(), pFire);
  return true;
}

public func ReadyToFire() { return !rdy2fire; }

protected func SetReadyState(bool newstate) {
  rdy2fire = newstate;
  if(newstate)
    Sound("Connect");
}

local pCursor; // Bla zeigen der Projektilflugbahn

// zum zeigen der Projektilflugbahn
public func ProjectilePreview() {
  var iAngle, iX, iY;
  iAngle = 285 + GetPhase() * 15;
  iX = Sin(iAngle, 40);
  iY = Cos(iAngle, 40);
  if(!pCursor)
    pCursor = CreateObject(ROCK);
  pCursor->~SetPosition(GetX() + iX, GetY() - iY);
  pCursor->SetCategory(C4D_StaticBack);
}

public func ProjSpeed() { return 120; }
public func ReloadTime() { return 28; }
public func ShootSound() { return "Blast2"; }