/*-- Barometer --*/

#strict 2

local pArrow1, pArrow2; // Die zwei Pfeile

// Spannweite: 306°. Leer: 54°. Grün: 194°. Gelb: 68°. Rot: 44°.

// Initializung

protected func Initialize()
{
  SetVisibility(VIS_Owner);
  SetPosition(125,100);
  pArrow1 = CreateObject(LPB1,27,24,GetOwner());
  pArrow2 = CreateObject(LPB2,5,29,GetOwner());
  return 1;
}
