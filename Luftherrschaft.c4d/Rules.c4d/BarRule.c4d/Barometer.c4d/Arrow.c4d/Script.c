/*-- Neues Objekt --*/

#strict 2


protected func Initialize()
{
  SetDTRotation(GetY(GetCursor(GetOwner()))*306/LandscapeHeight()-34, -22);
  return 1;
}

public func CheckPressure()
{
  if (Random(2)) var r = RandomX(-1,1); // Sorgt fürs authentische Zuckeln des Zeigers
  SetDTRotation(GetY(GetCursor(GetOwner()))*306/LandscapeHeight()-34+r, -22);
}
