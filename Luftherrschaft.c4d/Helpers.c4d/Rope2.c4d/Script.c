/*-- Das Seil --*/
#strict
local Length, CalcedLength;

public func CalcLength (Target1, Target2) {
  //Anzahl Paare
  var i=GetVertexNum ()-1;
  var Count=0;
  //für jedes Paar berechnen
  while (i--) Count+= VerticeLength (i, Target1, Target2);
return (Count);
}

private func VerticeLength (i, Target1, Target2) {
  //erster Vertex: Geschwindigkeit von 1.Actiontarget berücksichtigen
  if (i == 0) return (Distance (
    GetVertex (0) + GetXDir (Target1) / 2, 
    GetVertex (0, 1) + GetYDir (Target1) / 2,
    GetVertex (1),
    GetVertex (1, 1)));                                        
  //vorletzter Vertex: Geschwindigkeit von 2.Actiontarget berücksichtigen
  if (i == (GetVertexNum () - 2)) return (Distance (
    GetVertex (i),
    GetVertex (i, 1),          
    GetVertex (i + 1) + GetXDir (Target2) / 2, 
    GetVertex (i + 1, 1) + GetYDir (Target2)) / 2);
  //Ansonsten nur die Positionen
  return (Distance (
    GetVertex (i),
    GetVertex (i, 1),
    GetVertex (i + 1),
    GetVertex (i + 1, 1)));
}

func MoveRope (obj1, obj2) {
  var a,b;
//Hier, weil das Seil immer verschwinden soll, wenn ein angeknotetes Objekt verschwindet.
  if (!obj1) return (RemoveObject ());
  if (!obj2) return (RemoveObject ());
  //Wir brauchen mindesten 3 Vertices, damit die Algorithmen funktionieren
  if (GetVertexNum () < 3)
    InsertVertex (1, (GetVertex (0) + GetVertex (1)) / 2, (GetVertex (0, 1) + GetVertex (1, 1)) / 2);
  SetRopeEnd (obj1, 0, Local (2), 2);
  SetRopeEnd (obj2, GetVertexNum () - 1, Local (3), -2);
  var vertexdistance = 7 + 3 * GetVertexNum ();
//  var vertexdistance = 25;
  for (var i = GetVertexNum () - 2; i > 0; i--) {
  //Neue Position des Vertices
    a = (GetVertex (i - 1) + GetVertex (i + 1)) / 2;
    b = (GetVertex (i - 1, 1) + GetVertex (i + 1, 1)) / 2;
    //Überschüssige Vertices, die nahe genug aneinander sind entfernen
    if (GetVertexNum () > 3)
    if (Distance (GetVertex (i - 1, 0), GetVertex (i - 1, 1), GetVertex (i + 1, 0), GetVertex (i + 1, 1)) < vertexdistance)
    if (PathFree (GetVertex (i - 1, 0), GetVertex (i - 1, 1), GetVertex (i + 1, 0), GetVertex (i + 1, 1))) {
      RemoveVertex (i);
      continue;
    }
    //bei zu langem Abstand einen neuen Vertex einfügen
    if (Distance (GetVertex (i - 1, 0), GetVertex (i - 1, 1), GetVertex (i, 0), GetVertex (i, 1)) > (5 + vertexdistance)) {
      InsertVertex (i, (GetVertex (i - 1, 0) + GetVertex (i, 0)) / 2, (GetVertex (i - 1, 1) + GetVertex (i, 1)) / 2);
      continue;
    }
    //Wenn durch verschieben des Vertex das Seil durch solides ginge, nicht verschieben, und bei genug Platz einen zusätzlichen Vertex einfügen
    if (!PathFree (a, b, GetVertex (i - 1), GetVertex (i - 1, 1))) {
      if (Distance (GetVertex (i - 1, 0), GetVertex (i - 1, 1), GetVertex (i, 0), GetVertex (i, 1)) > vertexdistance)
        InsertVertex (i, (GetVertex (i-1, 0)+GetVertex (i, 0))/2, (GetVertex (i-1, 1)+GetVertex (i, 1))/2);
      continue;
    }
    if (!PathFree (a, b, GetVertex (i + 1), GetVertex (i + 1, 1))) {
      if (Distance (GetVertex (i+1, 0),GetVertex (i+1, 1),GetVertex (i, 0),GetVertex (i, 1)) > vertexdistance)
        InsertVertex (i+1, (GetVertex (i+1, 0) + GetVertex (i, 0))/2, (GetVertex (i+1, 1) + GetVertex (i, 1))/2);
      continue;
    }
    SetVertex (i, 0, a);
    SetVertex (i, 1, b);
  }
}

func SetRopeEnd (obj, ropevertex, objvertex, whichend) {
  var x, y;
  x = GetX (obj) + GetVertex (objvertex, 0, obj);
  y = GetY (obj) + GetVertex (objvertex, 1, obj);
  //Wenn durch die Bewegung des angebundenen Objekts das Seil durch solides ginge, einen Vertex an der letzten Position des Objekts einfügen
  //wenn das Objekt selbst in fester Materie steckt, hülfe das auch nichts
  if (!GBackSolid (x - GetX (), y - GetY ())) {
    if (!PathFree (x, y, GetVertex (ropevertex + whichend, 0), GetVertex (ropevertex + whichend, 1))) {
      InsertVertex (ropevertex - BoundBy (whichend, -1, 0), x, y);
    } else {
      SetVertex (ropevertex, 0, x);
      SetVertex (ropevertex, 1, y);
    }
  }
}

private func InsertVertex (index, x, y) {
  if (!AddVertex ()) return ();
  for (var i = GetVertexNum () - 1; i > index; i-- ) {
    SetVertex (i, 0, GetVertex (i - 1, 0));
    SetVertex (i, 1, GetVertex (i - 1, 1));
  }
  SetVertex (index, 0, x);
  SetVertex (index, 1, y);
  return (1);
}

func Connecting () {
  var Target1 = GetRealContainer (GetActionTarget (0));
  var Target2 = GetRealContainer (GetActionTarget (1));
  MoveRope (GetActionTarget (0), GetActionTarget (1));
  if (Target1 == Target2) return ();
  CalcedLength = CalcLength (Target1, Target2);
//  MoveRope (Target1, Target2);
  var Difference = CalcedLength - Length;
  Length = Max (Length + ObjectCall (GetActionTarget (0), "RopeAskChangeLength", Difference, this ()), 3);
  Length = Max (Length + ObjectCall (GetActionTarget (1), "RopeAskChangeLength", Difference, this ()), 3);
  Difference = CalcedLength - Length;
  if (Difference <= 0) return ();
  var Masse = GetMass (Target1) + GetMass (Target2);
  Do_Stuff (Target1, Target2, Masse, Difference, 1, Local (2));
  Do_Stuff (Target2, Target1, Masse, Difference, GetVertexNum () - 2, Local (3));
}

func GetRealContainer (obj) {
  var ret = obj;
  while (Contained (ret)) ret = Contained (ret);
  return (ret);
}

func Do_Stuff (Target1, Target2, Masse, Difference, RopeVertex, ObjVertex) {
  var Strength = (Difference * GetMass (Target2)) / Masse;
  /*Wenn Crewmember, dann wird Physical Fight miteingerechnet: Je größer, desto weniger wird gezogen
  Masse: Je schwerer das gegenüberliegende Objekt, desto mehr wird gezogen*/
  if (GetProcedure (Target1) eq "WALK")
    Strength = Max (0, Strength - (GetPhysical ("Fight", 0, Target1) / 30000 ));
  if (GetProcedure (Target1) eq "FLOAT")
    Strength = Max (0, Strength - (GetPhysical ("Float", 0, Target1) / 100 ));
  PullObject (GetVertex (RopeVertex), GetVertex (RopeVertex, 1), Strength, Target1, ObjVertex);  
}

private func PullObject (toX, toY, length, obj, vtx) {
  var len=Min (length,Distance (toX, toY,GetX (obj)+GetVertex (vtx, 0, obj), GetY (obj)+GetVertex (vtx, 1, obj)));
  var winkel = MakeThisAngleUseful (Angle (GetX (obj)+GetVertex (vtx, 0, obj), GetY (obj)+GetVertex (vtx, 1, obj), toX, toY), -180, +180);
//  var winkel2 = Angle (GetX (obj), GetY (obj), GetVertex (vtx, 0, obj), GetVertex (vtx, 1, obj));
  var winkel2 = MakeThisAngleUseful (Angle (0, 0, GetVertex (vtx, 0, obj), GetVertex (vtx, 1, obj)), -180, +180);
  var x = Sin (winkel, len*10);
  var y = Cos (winkel, len*10);
  if (Stuck (obj)) return ();
  SetXDir (BoundBy (GetXDir (obj, 100) + x, -1000, 1000), obj, 100);
  SetYDir (BoundBy (GetYDir (obj, 100) - y, -1000, 1000), obj, 100);
//  SetRDir (GetRDir (obj, 100) - BoundBy (winkel - winkel2, -2, 2), obj, 100);
  SetRDir (GetRDir (obj, 100) + (winkel - winkel2) / 10, obj, 100);
  if (length > 30) 
  SetPosition (GetX (obj) + BoundBy (x,-1,1), GetY (obj) - BoundBy (y,-1,1), obj);
  //Drehung verbessern
 }

func MakeThisAngleUseful (winkel, m, p) {
  while (winkel > p)
    winkel = winkel - 360;

  while (winkel < m)
    winkel = winkel + 360;

  return (winkel);
}

public func Activate (obj1, obj2) {
  Local (2) = GetMiddlestVertex (obj1);
  Local (3) = GetMiddlestVertex (obj2);
  SetVertex (0, 0, GetX (obj1) + GetVertex (Local (2), 0, obj1));
  SetVertex (0, 1, GetY (obj1) + GetVertex (Local (2), 1, obj1));
  SetVertex (1, 0, GetX (obj2) + GetVertex (Local (3), 0, obj2));
  SetVertex (1, 1, GetY (obj2) + GetVertex (Local (3), 1, obj2));
  SetAction ("Connect", obj1, obj2 );
  Length = CalcLength ();
}

public func CheckConVertices () {
  Local (2) = GetMiddlestVertex (GetActionTarget ());
  Local (3) = GetMiddlestVertex (GetActionTarget (1));
}

private func GetMiddlestVertex (obj) {
// return (-1);//Vertex -1 ist 0/0 -> Mitte  Aber da reißt es leichter : (
  var i,s = 1000,t,n;
  if (n = ObjectCall (obj, "GetVertexToConnect")) return (n-1 );
  //alle Vertices durchgehen
  for (i = GetVertexNum (obj); i >= 0; i--)
  {//x-Differenz zur Mitte + y-Differenz (eigentlich müsste ich Distance benutzen)
    t= (Abs (GetVertex (i, 0, obj))+Abs (GetVertex (i, 1, obj)));
    //s: kürzester Abstand bis jetzt
    if (s > t)
    {//n: Vertex mit kürzestem Abstand
      s = t;
      n = i;
    }
  }
  return (n);
}

/*Mit SetObject kann man die angeknüpften Objekte ändern. num=0 für Actiontarget0 und num=1 für Actiontarget1*/
public func SetObject (obj, num) {
//keine Gebäude
  if (GetCategory (obj) & C4D_Structure ()) return ();
//1.Actiontarget
  if (num == 0)
  {
  //Action und 2.Actiontarget bleiben erhalten
    Local (2 ) = GetMiddlestVertex (obj);
    SetAction ("Connect", obj, GetActionTarget (1));
  }
//2.Actiontarget
  if (Par (1)==1)
  {
    Local (3 ) = GetMiddlestVertex (obj);
    SetAction ("Connect", GetActionTarget (), obj);
  }
  return (1);
}

protected Initialize:
//  SetAction ("Connect");
  Local (0)=82;
  //Local (1)=30;
  Local (1)=82;
return (1);

LineBreak:
  Sound ("LineBreak");
  if (Equal (Par (0),0)) BreakMessage ();
  return (1);

public func GetLength () { return (Length); }

protected func Destruction () {
  //Benachrichtigungen
  ObjectCall (GetActionTarget (), "RopeBreak", GetID (GetActionTarget (1)));
  ObjectCall (GetActionTarget (1), "RopeBreak", GetID (GetActionTarget ()));
}