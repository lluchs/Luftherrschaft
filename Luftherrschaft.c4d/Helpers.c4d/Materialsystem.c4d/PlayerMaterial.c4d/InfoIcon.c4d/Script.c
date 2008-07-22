/*-- Informationicon --*/

#strict 2

func Initialize() {
  SetVisibility(VIS_Owner);
  return(1);
}

public func Set(string graphics) {
  SetGraphics(graphics, this, GetID(this), 0, 1);
}

public func SetStatusMessage(string msg) {
  CustomMessage(msg, this, GetOwner(), +30, +120);
}