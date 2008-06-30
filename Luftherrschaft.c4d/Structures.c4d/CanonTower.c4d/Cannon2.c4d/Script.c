/*-- Doppelläufige Kanone --*/

#strict 2

#include CTC1 // basiert auf der einfachen Kanone

// braucht 2 Dinge zum Feuern
public func CannonFire(object pFire1, object pFire2) {
  if(!pFire1 || !pFire2)
    return Sound("Error");
  var iAngle = 286 + GetPhase() * 15;
  var iX,iY,iXDir,iYDir;
  iX = Sin(iAngle,30);
  iY = Cos(iAngle,30) - 10;
  Enter(this,pFire1);
  Enter(this,pFire2);
  // schießt nicht so stark
  iXDir = Sin(iAngle, 105);
  iYDir = -Cos(iAngle, 105);
  Exit(pFire1, iX - 5, iY, Random(360));
  SetXDir(iXDir,pFire1,10);
  SetYDir(iYDir,pFire1,10);
  Sound("Blast2", pFire1);
  // zweiter Schuss leicht zeitverzögert
  ScheduleCall(this,"FireObject", 6, 0, pFire2, [iX + 5, iY, Random(360), iXDir + RandomX(-2,2), iYDir + RandomX(-2,2), RandomX(-4,4)]);
  rdy2fire = true;
  // braucht länger
  ScheduleCall(this, "SetReadyState", 40, 0, false);
  SetPlrView(GetOwner(), pFire1);
  return 1;
}

// bloß ein Alias für Exit, da sonst ein Fehler kommt bei ScheduleCall()
public func FireObject(object pFire, array param) {
  Exit(pFire, param[0], param[1], param[2]);
  SetXDir(param[3],pFire,10);
  SetYDir(param[4],pFire,10);
  Sound("Blast2", pFire);
}