/*-- Gerüst --*/

#strict 2

local shipsmith;

protected func Initialize() {
  SetVisibility(VIS_All);
  SetAction("BuildUp");
}

public func Init(object pShipsmith) {
  shipsmith = pShipsmith;
}

private func Ready() {
  shipsmith->GerustComplete();
}

private func MakeInvisible() {
  SetVisibility(VIS_God);
}

public func ConstructionCompleted() {
  SetAction("BuildDown");
}