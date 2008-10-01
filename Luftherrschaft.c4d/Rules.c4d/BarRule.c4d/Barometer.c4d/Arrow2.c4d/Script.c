/*-- Neues Objekt --*/

#strict 2

protected func Initialize()
{
  return 1;
}

public func CheckTemperature() // Wird alle 5 Frames aufgerufen
{
  if (Random(2)) var r = RandomX(-1,1); // Sorgt fürs authentische Zuckeln des Zeigers
  SetDTRotation(GetTemperature()*306/200+90+r); // Wetterformel
}
