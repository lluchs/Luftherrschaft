/*-- Spielermaterialien --*/

#strict 2

local wood, ziegel, werkzeug;
local shower_wood, shower_brick, shower_tool;

public func Initialize() {
  shower_wood = CreateObject(LHII,,,GetOwner());
  shower_wood->SetPosition(-246, +5);
  SetObjDrawTransform(566,,,,566,,shower_wood);
  shower_wood->Set("WOOD");
  shower_brick = CreateObject(LHII,,,GetOwner());
  shower_brick->SetPosition(-206, +5);
  SetObjDrawTransform(566,,,,566,,shower_brick);
  shower_brick->Set("BRIK");
  shower_tool = CreateObject(LHII,,,GetOwner());
  shower_tool->SetPosition(-166, +5);
  SetObjDrawTransform(566,,,,566,,shower_tool);
  shower_tool->Set("TOOL");
  /*
  Kurzinfo
  Endgröße 34 x 17 
  Position -6 x 5 gold
  pos      -46 x 5 score
  pos      -86 x 5 crew
  -->
  pos      -126 x 5 tool
  pos      -166 x 5 brick
  pos      -206 x 5 wood
  und 60 mehr, weil Offset bei -1 x -1 ist.
  */
}

public func WoodSell(object pWood) {
  if(GetID(pWood) != WOOD)
    return;
  wood++;
  RemoveObject(pWood);
}

public func AddZiegel(object pZiegel) {
  if(GetID(pZiegel) != ZIGL) // ID VOM ZIEGEL EINFÜGEN
    return;
  ziegel++;
  RemoveObject(pZiegel);
}

public func AddWerkzeug(object pWood, object pMetl) {
  if(GetID(pWood) != WOOD || GetID(pMetl) != METL)
    return;
  werkzeug++;
  RemoveObject(pWood);
  RemoveObject(pMetl);
}

public func Timer() {
  shower_wood->SetStatusMessage(Format("@%d", wood));
  shower_brick->SetStatusMessage(Format("@%d", ziegel));
  shower_tool->SetStatusMessage(Format("@%d", werkzeug));
}

public func GetWood() { return wood; }
public func GetZiegel() { return ziegel; }
public func GetWerkzeug() { return werkzeug; }