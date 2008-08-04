/*-- Neues Script --*/

#strict 2
#appendto BARL

protected func Initialize() {
  UpdatePicture();
  return _inherited(...);
}

private func FillCheck() {
	if(GetID() != BARL)
		ScheduleCall(this, "FillCheck", 35);
  // Material an aktueller Position überprüfen
  var iMaterial;
  if (((iMaterial = GetMaterial()) == -1) || (iMaterial == iBackMaterial)) return;
  // Fass für dieses Material suchen
  var idBarrel;
  if(!(idBarrel = GetBarrelType(iMaterial))) return iBackMaterial = iMaterial;
  // Fass füllen
  iFillLevel += ExtractMaterialAmount(0, 0, iMaterial, BarrelMaxFill() - iFillLevel);
  iFillMaterial = iMaterial + 1;
  if(GetID() == BARL)
		ScheduleCall(this, "FillCheck", 35);
  ChangeDef(idBarrel);
  UpdatePicture();
  return 1;
}

public func BarrelDoFill() {
  ScheduleCall(this, "UpdatePicture", 1);
  ScheduleCall(this, "EmptyCheck", 1);
  return _inherited(...);
}

private func BarrelEject() {
  ScheduleCall(this, "UpdatePicture", 1);
  return _inherited(...);
}

public func EmptyCheck() {
	if(!iFillLevel) 
		ChangeDef(EmptyBarrelID());
}

/* Grafik anpassen */
public func UpdatePicture()
{
	if(GetAmount()>99)
  {
    SetGraphics(0,0,GetNumberID(GetAmount() / 100),1,GFXOV_MODE_Picture);    
    SetObjDrawTransform(400,0,-14000,0,400,+10000, this, 1);
  }
  else SetGraphics(0,0,0,1,0);
  
  if(GetAmount()>9)
  {
    SetGraphics(0,0,GetNumberID(GetAmount() / 10 - GetAmount() / 100 * 10),2,GFXOV_MODE_Picture);    
    SetObjDrawTransform(400,0,-7000,0,400,+10000, this, 2);
  }
  else SetGraphics(0,0,0,2,0);  

  SetGraphics(0,0,GetNumberID(GetAmount() % 10),3,GFXOV_MODE_Picture);   
  SetObjDrawTransform(400,0,0,0,400,+10000, this, 3);
  return 1;
}

/* ID des passenden Zahlobjektes ausgeben */
private func GetNumberID(i)
{
  return(C4Id(Format("SNB%d", i)));
}
