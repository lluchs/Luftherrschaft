/*-- Das Seilende --*/
#strict
local Pull, Rope, Hook;
                
public func SetRope (r, h) {
  Rope = r;
  Hook = h;
}

func Departure () {
  Pull = 10;
}
func Entrance () {
  Pull = 0;
}

//Steuerung
func ControlLeft (clonk) {
  if (GetProcedure (clonk) eq "FLIGHT") Pull = 0; else Pull = 1;
}
func ControlRight (clonk) {
  if (GetProcedure (clonk) eq "FLIGHT") Pull = 0; else Pull = 1;
}
func ControlUp (clonk) {
  if (GetProcedure (clonk) eq "FLIGHT") Pull = -1;
  else if (GetProcedure (clonk) eq "WALK") Pull = -1;
  else Pull = 1;
}
func ControlDown (clonk) {
  Pull = 2;
}
func Activate (clonk) {
  [Seil loslassen]
  Enter (clonk, Hook);
  RemoveObject (Rope);
  RemoveObject ();
  return (1);
}

public RopeBreak:return(RemoveObject());

func RopeAskChangeLength (len, obj) {
  if (obj != Rope) return ();
  if (Contained (Contained ())) return (Activate (Contained ()));
  // Ohne Kontrolle dem Wunsch des Seils nachgeben
  if (!Contained ()) return (len + 1);
  // Bei zu starkem Zug kann der CLonk das Seil nicht mehr festhalten
  if (Pull <= 0) if (len >= 15) if (ObjectCall (obj, "GetLength") < 1000) return (1);
  // Nicht zu viel Seil lassen
  if (Pull >= 0) if (len <= -10) return (-1);
  // Am Seil ziehen
  if (Pull < 0) if (len <= 3) return (Pull);
  if (Pull < 0) if (len <= 6) return (-1);
  // Seil geben, nicht zuviel
  if (Pull > 0) if (len >= 0) if (ObjectCall (obj, "GetLength") < 1000) return (Pull);
  return();
}