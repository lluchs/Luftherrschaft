/*-- Neues Script --*/

#strict
#appendto BARL

protected func Initialize() {
  UpdatePicture();
  return _inherited(...);
}

private func FillCheck() {
  ScheduleCall(this, "UpdatePicture", 1);
  return _inherited(...);
}

public func BarrelDoFill() {
  ScheduleCall(this, "UpdatePicture", 1);
  return _inherited(...);
}

private func BarrelEject() {
  ScheduleCall(this, "UpdatePicture", 1);
  return _inherited(...);
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
