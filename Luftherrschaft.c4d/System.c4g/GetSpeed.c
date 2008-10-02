#strict 2

// Liefert die Geschwindigkeit eines Objektes (X & Y)
global func GetSpeed(object pObj, int iPrecision) {
  if(!pObj) pObj = this;
  var xdir = Abs(GetXDir(pObj, iPrecision)), ydir = Abs(GetYDir(pObj, iPrecision));
  var speed = Sqrt(xdir ** 2 + ydir ** 2);
  return speed;
}
