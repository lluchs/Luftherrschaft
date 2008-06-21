/*-- Das Seil --*/

#strict 2

local Length, CalcedLength;

/*
Unbenannte lokale Variablen
0 - 82
1 - 82
2 - Mittigster Vertex von ActionTarget 1
3 -    ''        ''    '' ActionTarget 2
*/

protected func Initialize() {
  Local(0)=82;
  Local(1)=82;
  return 1;
}

public func CalcLength(object Target1, object Target2) {
  // Anzahl Paare
  var i=GetVertexNum() - 1;
  var Count=0;
  // für jedes Paar berechnen
  while (i--) Count += VerticeLength (i, Target1, Target2);
  return Count;
}

private func VerticeLength(int i,object Target1,object Target2) {
  // erster Vertex: Geschwindigkeit von 1.Actiontarget berücksichtigen
  if(i == 0)
    return Distance(GetVertex (0) + GetXDir (Target1) / 2,GetVertex (0, 1) + GetYDir (Target1) / 2,GetVertex (1),GetVertex (1, 1));
  // vorletzter Vertex: Geschwindigkeit von 2.Actiontarget berücksichtigen
  if(i == (GetVertexNum () - 2))
    return Distance(GetVertex (i),GetVertex (i, 1),GetVertex (i + 1) + GetXDir (Target2) / 2,GetVertex (i + 1, 1) + GetYDir (Target2)) / 2;
  // Ansonsten nur die Positionen
  return Distance(GetVertex (i),GetVertex (i, 1),GetVertex (i + 1),GetVertex (i + 1, 1));
}

public func MoveRope(object pObj1,object pObj2) {
  var a,b;
  // Hier, weil das Seil immer verschwinden soll, wenn ein angeknotetes Objekt verschwindet.
  if(!pObj1 || !pObj2) return RemoveObject();
  // Wir brauchen mindesten 3 Vertices, damit die Algorithmen funktionieren
  if(GetVertexNum () < 3)
    InsertVertex(1, (GetVertex (0) + GetVertex (1)) / 2, (GetVertex (0, 1) + GetVertex (1, 1)) / 2);
  SetRopeEnd(pObj1, 0, Local(2), 2);
  SetRopeEnd(pObj2, GetVertexNum () - 1, Local (3), -2);
  var vertexdistance = 7 + 3 * GetVertexNum ();
  // für jeden Vertex ausführen
  for(var i = GetVertexNum () - 2; i > 0; i--) {
  // Neue Position des Vertices
    a = (GetVertex (i - 1) + GetVertex (i + 1)) / 2;
    b = (GetVertex (i - 1, 1) + GetVertex (i + 1, 1)) / 2;
  // Überschüssige Vertices, die nahe genug aneinander sind entfernen
    if(GetVertexNum () > 3)
      if(Distance(GetVertex (i - 1, 0), GetVertex (i - 1, 1), GetVertex (i + 1, 0), GetVertex (i + 1, 1)) < vertexdistance)
        if(PathFree(GetVertex (i - 1, 0), GetVertex (i - 1, 1), GetVertex (i + 1, 0), GetVertex (i + 1, 1))) {
          RemoveVertex (i);
          continue;
        }
  // bei zu langem Abstand einen neuen Vertex einfügen
    if(Distance(GetVertex(i - 1, 0), GetVertex (i - 1, 1), GetVertex (i, 0), GetVertex (i, 1)) > (5 + vertexdistance)) {
      InsertVertex(i, (GetVertex (i - 1, 0) + GetVertex (i, 0)) / 2, (GetVertex (i - 1, 1) + GetVertex (i, 1)) / 2);
      continue;
    }
  // Wenn durch verschieben des Vertex das Seil durch solides ginge, nicht verschieben, und bei genug Platz einen zusätzlichen Vertex einfügen
    if(!PathFree (a, b, GetVertex (i - 1), GetVertex (i - 1, 1))) {
      if(Distance (GetVertex (i - 1, 0), GetVertex (i - 1, 1), GetVertex (i, 0), GetVertex (i, 1)) > vertexdistance)
        InsertVertex (i, (GetVertex (i-1, 0)+GetVertex (i, 0))/2, (GetVertex (i-1, 1)+GetVertex (i, 1))/2);
      continue;
    }
    if(!PathFree (a, b, GetVertex (i + 1), GetVertex (i + 1, 1))) {
      if(Distance (GetVertex (i+1, 0),GetVertex (i+1, 1),GetVertex (i, 0),GetVertex (i, 1)) > vertexdistance)
        InsertVertex (i+1, (GetVertex (i+1, 0) + GetVertex (i, 0))/2, (GetVertex (i+1, 1) + GetVertex (i, 1))/2);
      continue;
    }
    SetVertex (i, 0, a);
    SetVertex (i, 1, b);
  }
  return 1;
}

func SetRopeEnd(object pObj,int iRopeVertex,int iObjVertex,int iWhichEnd) {
  var x, y;
  x = GetX (pObj) + GetVertex (iObjVertex, 0, pObj);
  y = GetY (pObj) + GetVertex (iObjVertex, 1, pObj);
  // Wenn durch die Bewegung des angebundenen Objekts das Seil durch solides ginge,
  // einen Vertex an der letzten Position des Objekts einfügen
  // wenn das Objekt selbst in fester Materie steckt, hülfe das auch nichts
  if(!GBackSolid (x - GetX (), y - GetY ())) {
    if(!PathFree (x, y, GetVertex(iRopeVertex + iWhichEnd, 0), GetVertex (iRopeVertex + iWhichEnd, 1))) {
      InsertVertex (iRopeVertex - BoundBy (iWhichEnd, -1, 0), x, y);
    }
    else {
      SetVertex (iRopeVertex, 0, x);
      SetVertex (iRopeVertex, 1, y);
    }
  }
  return 1;
}

private func InsertVertex(int iIndex,int iX,int iY) {
  if (!AddVertex ()) return 0;
  for (var i = GetVertexNum () - 1; i > iIndex; i-- ) {
    SetVertex (i, 0, GetVertex (i - 1, 0));
    SetVertex (i, 1, GetVertex (i - 1, 1));
  }
  SetVertex (iIndex, 0, iX);
  SetVertex (iIndex, 1, iY);
  return (1);
}

/* ActionCall From Connect */

func Connecting() {
  var Target1 = GetRealContainer(GetActionTarget(0));
  var Target2 = GetRealContainer(GetActionTarget(1));
  MoveRope(GetActionTarget(0), GetActionTarget(1));
  if(Target1 == Target2) return 0;
  if(!Target1 || !Target2) return 0;
  CalcedLength = CalcLength(Target1, Target2);
  var Difference = CalcedLength - Length;
  Length = Max(Length + GetActionTarget(0)->~RopeAskChangeLength(Difference, this), 3);
  Length = Max(Length + GetActionTarget(1)->~RopeAskChangeLength(Difference, this), 3);
  Difference = CalcedLength - Length;
  if(Difference <= 0) return 0;
  var iMass = GetMass(Target1) + GetMass (Target2);
  Do_Stuff(Target1, Target2, iMass, Difference, 1, Local(2));
  Do_Stuff(Target2, Target1, iMass, Difference, GetVertexNum () - 2, Local(3));
  return 1;
}

func Do_Stuff(object pPullingObject,object Target2,int iMass,int iDifference,int RopeVertex,int ObjVertex) {
  var Strength = (iDifference * GetMass(Target2)) / iMass;
  // Wenn Crewmember, dann wird Physical Fight miteingerechnet: Je größer, desto weniger wird gezogen
  // Masse: Je schwerer das gegenüberliegende Objekt, desto mehr wird gezogen
  if(GetProcedure(pPullingObject) == "Walk")
    Strength = Max(0, Strength - (GetPhysical ("Fight", 0, pPullingObject) / 30000));
  if(GetProcedure(pPullingObject) == "Float")
    Strength = Max(0, Strength - (GetPhysical ("Float", 0, pPullingObject) / 100));
  PullObject(GetVertex(RopeVertex), GetVertex(RopeVertex, 1), Strength, pPullingObject, ObjVertex);  
}

private func PullObject(int toX,int toY,int length,object obj,int vtx) {
  var len = Min(length,Distance(toX, toY,GetX(obj) + GetVertex(vtx, 0, obj), GetY (obj)+GetVertex (vtx, 1, obj)));
  var iAngle = MakeThisAngleUseful(Angle(GetX (obj)+GetVertex (vtx, 0, obj), GetY (obj)+GetVertex (vtx, 1, obj), toX, toY), -180, +180);
  var iAngle2 = MakeThisAngleUseful(Angle(0, 0, GetVertex (vtx, 0, obj), GetVertex (vtx, 1, obj)), -180, +180);
  var x = Sin(iAngle, len*10);
  var y = Cos(iAngle, len*10);
  if(Stuck (obj)) return 0;
  SetXDir(BoundBy(GetXDir (obj, 100) + x, -1000, 1000), obj, 100);
  SetYDir(BoundBy(GetYDir (obj, 100) - y, -1000, 1000), obj, 100);
  SetRDir(GetRDir(obj, 100) + (iAngle - iAngle2) / 10, obj, 100);
  if (length > 30)
  SetPosition (GetX (obj) + BoundBy (x,-1,1), GetY (obj) - BoundBy (y,-1,1), obj);
 }

func MakeThisAngleUseful (winkel, m, p) {
  while (winkel > p)
    winkel = winkel - 360;
  while (winkel < m)
    winkel = winkel + 360;
  return winkel;
}

public func Activate(object obj1,object obj2) {
  Local(2) = GetMiddlestVertex(obj1);
  Local(3) = GetMiddlestVertex(obj2);
  // Vertices erzeugen/ändern
  SetVertex(0, 0, GetX (obj1) + GetVertex (Local (2), 0, obj1));
  SetVertex(0, 1, GetY (obj1) + GetVertex (Local (2), 1, obj1));
  SetVertex(1, 0, GetX (obj2) + GetVertex (Local (3), 0, obj2));
  SetVertex(1, 1, GetY (obj2) + GetVertex (Local (3), 1, obj2));
  SetAction("Connect", obj1, obj2);
  Length = CalcLength();
}

public func CheckConVertices () {
  Local(2) = GetMiddlestVertex (GetActionTarget ());
  Local(3) = GetMiddlestVertex (GetActionTarget (1));
}

/* Mit SetObject kann man die angeknüpften Objekte ändern. num=0 für Actiontarget0 und num=1 für Actiontarget1 */

public func SetObject(object obj,int num) {
  // keine Gebäude
  if (GetCategory(obj) & C4D_Structure) return 0;
  // 1.Actiontarget
  if (num == 0) {
    // Action und 2.Actiontarget bleiben erhalten
    Local(2) = GetMiddlestVertex (obj);
    SetAction("Connect", obj, GetActionTarget(1));
  }
  // 2.Actiontarget
  if(num == 1) {
    Local(3) = GetMiddlestVertex(obj);
    SetAction("Connect", GetActionTarget(), obj);
  }
  return 1;
}

public func LineBreak(object pBreak) {
  Sound ("LineBreak");
  if(!pBreak) BreakMessage();
  return 1;
}

func BreakMessage() {
  // Meldung bei Leitungsbausatz am einen oder anderen Ende ausgeben
  var pObj = GetActionTarget(0);
  if(GetID(pObj) != LNKT)
    pObj = GetActionTarget(1);
  Message("$RopeBroken$",pObj);
  return 1;
}

public func GetLength () { return (Length); }

protected func Destruction () {
  // Benachrichtigungen
  if(GetActionTarget(0)) GetActionTarget(0)->~RopeBreak(GetID(GetActionTarget(1)));
  if(GetActionTarget(1)) GetActionTarget(1)->~RopeBreak(GetID(GetActionTarget(0)));
  RemoveObject(Rope);
}