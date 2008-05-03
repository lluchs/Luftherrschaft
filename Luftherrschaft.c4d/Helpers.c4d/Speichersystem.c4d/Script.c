/*-- Speichersystem --*/

#strict 2

protected func Initialize() {
	if(GetID() == SAVS) {
		DebugLog("ERROR: Dieses Objekt darf nicht erstellt werden");
		RemoveObject();
	}
	aFillLevel = CreateArray();
	UpdatePicture();
}

// Füllstand
local aFillLevel;

/* Füllung */

// Aktuelle Füllung
public func & GetAmount(int iCount) { return aFillLevel[iCount]; }

// Maximale Füllung
public func MaxFill(int iCount) {
	var szVar = Format("%i_MaxFill", GetID());;
	if(iCount)
		szVar = Format("%s%d", szVar, iCount);
	return eval(szVar);
}

// Füllung erhöhen/verringern
public func DoFill(int iChange, int iCount, bool fNoSound)
{
  var iNewFill = BoundBy(GetAmount(iCount) + iChange, 0, MaxFill());
  if (iNewFill == GetAmount(iCount)) return;
  if(!fNoSound)
  	this -> ~FillSound(iCount);
  iNewFill -= GetAmount(iCount); GetAmount(iCount) += iNewFill;
  UpdatePicture();
  // Tatsächliche Änderung des Füllstandes zurückgeben
  return iNewFill;
}

// Voll?
public func IsFull(int iCount) 
{ 
  return GetAmount(iCount) == MaxFill();
}

/* Grafik anpassen */
public func UpdatePicture()
{
	var iCount = this -> ~FillPicture();
	if(!iCount)
		return;
	iCount--;
	if(GetAmount(iCount)>99)
  {
    SetGraphics(0,0,GetNumberID(GetAmount(iCount) / 100),1,GFXOV_MODE_Picture);    
    SetObjDrawTransform(400,0,-14000,0,400,+10000, this, 1);
  }
  else SetGraphics(0,0,0,1,0);
  
  if(GetAmount(iCount)>9)
  {
    SetGraphics(0,0,GetNumberID(GetAmount(iCount) / 10 - GetAmount(iCount) / 100 * 10),2,GFXOV_MODE_Picture);    
    SetObjDrawTransform(400,0,-7000,0,400,+10000, this, 2);
  }
  else SetGraphics(0,0,0,2,0);  

  SetGraphics(0,0,GetNumberID(GetAmount(iCount) % 10),3,GFXOV_MODE_Picture);   
  SetObjDrawTransform(400,0,0,0,400,+10000, this, 3);
  return 1;
}

/* ID des passenden Zahlobjektes ausgeben */
private func GetNumberID(i)
{
  return(C4Id(Format("SNB%d", i)));
}
