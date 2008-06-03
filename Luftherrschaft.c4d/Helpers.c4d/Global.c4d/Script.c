/*-- Global Script --*/

#strict 2

static const C4D_SelectMaterial = 512;
static const C4D_SelectKnowledge = 1024;
static const C4D_SelectHomebase = 2048;

static const Rope_Pull = -1;
static const Rope_Push = 1;
static const Rope_Hold = 0;

global func GetRealContainer(object pOf) {
  var pCon = pOf;
  while(Contained(pCon)) pCon = Contained(pCon);
  return pCon;
}

global func GetMiddlestVertex (obj) {
  // return (-1);//Vertex -1 ist 0/0 -> Mitte  Aber da reißt es leichter : (
  var i,s = 1000,t,n;
  if (n = ObjectCall (obj, "GetVertexToConnect")) return (n-1 );
  // alle Vertices durchgehen
  for (i = GetVertexNum (obj); i >= 0; i--) {
    // Entfernung zur Mitte errechnen
    t = Distance(GetVertex(i,0,obj),GetVertex(i,1,obj),0,0);
    // s: kürzester Abstand bis jetzt
    if (s > t) {
    // n: Vertex mit kürzestem Abstand
      s = t;
      n = i;
    }
  }
  return n;
}

// Liefert die Geschwindigkeit eines Objektes (X & Y)
global func GetSpeed(object pObj, int iPrecision) {
  if(!pObj) pObj = this;
  var xdir = Abs(GetXDir(pObj, iPrecision)), ydir = Abs(GetYDir(pObj, iPrecision));
  var speed = Sqrt(xdir ** 2 + ydir ** 2);
  return speed;
}

// Gibt die Richtung eines bewegten Objektes als Gradzahl an
global func GetMoveAngle(object pObj) {
  if(!pObj) pObj = this;
  var xdir=GetXDir(pObj), ydir=GetYDir(pObj);
  var angle=Angle(0,0,xdir,ydir);
  return angle;
}

global func GetActionTargetNum(object pObj,object pOfObject) {
  if(GetActionTarget(0,pOfObject) == pObj) return 0;
  return 1;
}