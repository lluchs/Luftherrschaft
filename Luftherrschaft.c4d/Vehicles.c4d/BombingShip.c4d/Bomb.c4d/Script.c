/*-- Bombe --*/

#strict

protected func Departure(object pContainer) {
	if(GetID(pContainer) != LVBS)
		return;
  SetAction("Fall");
  if(GetXDir() > 0)
    SetR(90);
  else
    SetR(270);
}

protected func ChangeRotation() {
  var iR;
  iR=90+GetYDir()*9/10;
  if(GetYDir() > 100) iR=180;
  if(GetXDir() < 0) iR*=-1;
  SetR(iR);
}

protected func Hit3() {
  Explode(200); // *husthust*
  CastObjects(LOSP, 20, 20);
  return 1;
}

protected func Hit() {
	if(GetAction() == "Fall")
		SetAction("Idle");
}
