/*-- Kanone --*/

#strict

public func TurnLeft(object pCaller) {
  SetAction("TurnLeft");
}

public func TurnRight(object pCaller) {
  SetAction("TurnRight");
}

public func StopTurning() {
  var strAction = GetAction();
  var iPhase = GetPhase();
  // Warteaktion
  SetAction("Wait");
  // richtige Phase setzen
  SetPhase(iPhase);
}

protected func Waiting() {
  var iPhase = GetPhase() - 1;
  SetPhase(iPhase);
  Message("@Action: Wait | Phase jetzt: %d | Phase davor: %d",this, GetPhase(), iPhase);
}

public func CannonFire(object pFire) {
  // anhalten
  StopTurning();
  var iAngle, iPhase;
  iPhase = GetPhase();
  iAngle = (10 - iPhase) * 19 + 78;
  // Austrittspunkt und -geschwindigkeit
  var iX,iY,iXDir,iYDir;
  iX = Sin(iAngle,15);
  iY = Cos(iAngle,15);
  iXDir = +Sin(iAngle, 50);
  iYDir = -Cos(iAngle, 50);
  Enter(this,pFire);
  Exit(pFire, iX, iY, Random(360), iXDir, iYDir, RandomX(-5,5));
  Sound("Blast*");
  return true;
}

func MsgPhase() { Message("Action: %s | Phase: %d",this,GetAction(),GetPhase()); }
