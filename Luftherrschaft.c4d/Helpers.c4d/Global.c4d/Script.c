/*-- Global Script --*/

#strict 2

static const C4D_SelectMaterial = 512;
static const C4D_SelectKnowledge = 1024;
static const C4D_SelectHomebase = 2048;

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
