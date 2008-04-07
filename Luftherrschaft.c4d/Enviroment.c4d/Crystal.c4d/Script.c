/*-- Kristalle --*/

#strict

protected func Initialize()
{
  // Eigene Position auf 0,0
  SetPosition();
  // Kristalle platzieren
  for (var x = 0; x < LandscapeWidth(); x += RandomX(15, 50))
  {
    var y = GetHorizonHeight(x);
    var iMat = GetMaterial(x, y + 1);
    if (iMat == Material("Earth") || iMat == Material("Rock") || iMat == Material("Granite") || iMat == Material("Coal"))
      CreateCrystal(x, y);
  }
  // Steuerungsobjekt wird nicht mehr benötigt
  RemoveObject();
  return 1;    
}
       
private func GetHorizonHeight(x)
{
  var y;
  while (y < LandscapeHeight() && !GBackSemiSolid(x, y))
    y += 10;
  while (y > LandscapeHeight() && GBackSemiSolid(x, y))
    --y;
  return y;
}
    
private func CreateCrystal(x, y)
{
  return CreateObject(CRY2, x, y, -1) -> Construction();
}
