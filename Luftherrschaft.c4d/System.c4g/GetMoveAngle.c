#strict 2

// Gibt die Richtung eines bewegten Objektes als Gradzahl an
global func GetMoveAngle(object pObj) {
  if(!pObj) pObj = this;
  var xdir=GetXDir(pObj), ydir=GetYDir(pObj);
  var angle=Angle(0,0,xdir,ydir);
  return angle;
}
