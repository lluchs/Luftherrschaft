/*- Anker -*/

#strict 2

local Boat;

public func Launch(object pBoat) {
  Boat = pBoat;
  return 1;
}

public func Raise() {
  if(GetAction() == "Attachground") SetAction("Idle");
  return 0;
}

public func Hit() {
  if(Boat) {                             
    SetAction("Attachground");
    ObjectCall(Boat,"AnchorHit",this);
  }
  return 0;
}

public func GetVertexToConnect() { return 1; }

public func Entrance (obj) {
  Raise();
}