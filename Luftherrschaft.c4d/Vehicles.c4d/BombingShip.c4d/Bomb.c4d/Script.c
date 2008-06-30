/*-- Bombe --*/

#strict

protected func Departure(object pContainer) {
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

protected func Hit() {
  Explode(200);
  return(1);
}
