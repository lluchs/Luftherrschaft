#strict 2

global func GetMiddlestVertex (obj) {
  // return (-1);//Vertex -1 ist 0/0 -> Mitte  Aber da rei�t es leichter : (
  var i,s = 1000,t,n;
  if (n = ObjectCall (obj, "GetVertexToConnect")) return (n-1 );
  // alle Vertices durchgehen
  for (i = GetVertexNum (obj); i >= 0; i--) {
    // Entfernung zur Mitte errechnen
    t = Distance(GetVertex(i,0,obj),GetVertex(i,1,obj),0,0);
    // s: k�rzester Abstand bis jetzt
    if (s > t) {
    // n: Vertex mit k�rzestem Abstand
      s = t;
      n = i;
    }
  }
  return n;
}
