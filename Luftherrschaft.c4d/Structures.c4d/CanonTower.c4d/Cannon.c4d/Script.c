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
  if(!rdy2fire)
    return 0;
  var iAngle;
  iAngle = 286 + GetPhase() * 15;
  // Austrittspunkt und -geschwindigkeit
  var iX,iY,iXDir,iYDir;
  iX = Sin(iAngle,15);
  iY = Cos(iAngle,15) - 10;
  iXDir = +Sin(iAngle, 120);
  iYDir = -Cos(iAngle, 120);
  Enter(this,pFire);
  Exit(pFire, iX, iY, Random(360));
  SetXDir(iXDir,pFire,10);
  SetYDir(iYDir,pFire,10);
  Sound("Blast2");
  rdy2fire = true;
  ScheduleCall(this, "SetReadyState", 28, 0, false);
  SetPlrView(GetOwner(), pFire);
  return true;
}

public func ReadyToFire() { return !rdy2fire; }

protected func SetReadyState(bool newstate) {
  rdy2fire = newstate;
}