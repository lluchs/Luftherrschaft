/*-- Speichersystem --*/

#strict 2

protected func Initialize() {
	if(GetID() == SAVS) {
		DebugLog("ERROR: Dieses Objekt darf nicht erstellt werden");
		RemoveObject();
	}
	UpdatePicture();
}

// Füllstand
local iFillLevel;

/* Füllung */

// Aktuelle Füllung
public func GetAmount() { return iFillLevel; }

// Maximale Füllung
public func MaxFill() { return eval(Format("%i_MaxFill", GetID())); }

// Füllung erhöhen/verringern
public func DoFill(int iChange, bool fNoSound)
{
  var iNewFill = BoundBy(iFillLevel + iChange, 0, MaxFill());
  if (iNewFill == iFillLevel) return;
  if(!fNoSound)
  	this -> ~FillSound();
  iNewFill -= iFillLevel; iFillLevel += iNewFill;
  UpdatePicture();
  // Tatsächliche Änderung des Füllstandes zurückgeben
  return iNewFill;
}

// Voll?
public func IsFull() 
{ 
  return iFillLevel == MaxFill();
}

/* Grafik anpassen */
public func UpdatePicture()
{
	if(!this -> ~FillPicture())
		return;
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
