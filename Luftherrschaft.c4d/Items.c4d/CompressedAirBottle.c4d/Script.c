/*-- Druckluftbehälter --*/

#strict

static const LCAB_MaxFill = 400;

local iFillLevel;

protected func Initialize() {
	UpdatePicture();
  return 1;
}

/* Callbacks */
public func IsFull() {
	return iFillLevel == MaxFill();
}

public func IsCompressedAirBottle() {
	return 1;
}


// Aktuelle Füllung
public func GetAmount() { return(iFillLevel); }

// Maximale Füllung
public func MaxFill() { return LCAB_MaxFill; }

// Füllung erhöhen/verringern
public func DoFill(int iChange, bool fNoSound)
{
	if(!fNoSound)
		Sound("pfft");
  var iNewFill = BoundBy(iFillLevel + iChange, 0, MaxFill());
  if (iNewFill == iFillLevel) return;
  iNewFill -= iFillLevel; iFillLevel += iNewFill;
  UpdatePicture(); // Zahl aktualisieren
  // Tatsächliche Änderung des Füllstandes zurückgeben
  return iNewFill;
}

protected func Activate(pClonk) {
	return PlayerMessage(GetOwner(pClonk), "$TxtRestPression$", this, GetAmount());
}

/* Grafik anpassen */
public func UpdatePicture()
{
	var iTemp;
	if(GetAmount()>99)
  {
  	iTemp = GetAmount() / 100;
  	Log("1.: %d", iTemp);
    SetGraphics(0,0,GetNumberID(iTemp),1,GFXOV_MODE_Picture);    
    SetObjDrawTransform(400,0,-14000,0,400,+10000, this, 1);
  }
  if(GetAmount()>9)
  {
  	iTemp = GetAmount() / 10 - GetAmount() / 100 * 10;
  	Log("2.: %d", iTemp);
    SetGraphics(0,0,GetNumberID(iTemp),2,GFXOV_MODE_Picture);    
    SetObjDrawTransform(400,0,-7000,0,400,+10000, this, 2);
  }
  else SetGraphics(0,0,0,1,0);  

  SetGraphics(0,0,GetNumberID(GetAmount() % 10),3,GFXOV_MODE_Picture);   
  SetObjDrawTransform(400,0,0,0,400,+10000, this, 3);
  return 1;
}

/* ID des passenden Zahlobjektes ausgeben */
private func GetNumberID(i)
{
  return(C4Id(Format("SNB%d", i)));
}

// einen zu harten Aufschlag überlebt die Flasche nicht
protected func Hit2() {
  // in Bestandteile zerlegen
  CastObjects(LHSP, 4, 20);
  // mit kleiner Explosion von der Welt scheiden
  Explode(2);
 return 1;
}
