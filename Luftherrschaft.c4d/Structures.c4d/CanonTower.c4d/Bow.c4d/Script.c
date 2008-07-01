/*-- Ballista --*/

#strict 2
#include CTC1

local iRestArrow, idArrow;

public func CannonFire() {
  if(!rdy2fire)
    return 0;
  var iVal = GetValue( , idArrow);
  if(iVal > GetWealth(GetOwner()))
    return Sound("Error");
  SetWealth(GetOwner(), GetWealth() - iVal);
  var pFire = CreateContents(idArrow,this,0);
  var iAngle;
  iAngle = 286 + GetPhase() * 15;
  // Austrittspunkt und -geschwindigkeit
  var iX,iY,iXDir,iYDir;
  iX = Sin(iAngle,15);
  iY = Cos(iAngle,15) - 10;
  iXDir = +Sin(iAngle, 150);
  iYDir = -Cos(iAngle, 150);
  Enter(this,pFire);
  Exit(pFire, iX, iY, Random(360));
  SetXDir(iXDir,pFire,10);
  SetYDir(iYDir,pFire,10);
  Sound("Arrow");
  rdy2fire = true;
  // lädt schnell nach
  ScheduleCall(this, "SetReadyState", 4, 0, false);
  SetPlrView(GetOwner(), pFire);
  return true;
}