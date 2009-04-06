/*-- Druckluftbeh�lter --*/

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


// Aktuelle F�llung
public func GetAmount() { return(iFillLevel); }

// Maximale F�llung
public func MaxFill() { return LCAB_MaxFill; }

// F�llung erh�hen/verringern
public func DoFill(int iChange, bool fNoSound)
{
	if(!fNoSound)
		Sound("pfft");
  var iNewFill = BoundBy(iFillLevel + iChange, 0, MaxFill());
  if (iNewFill == iFillLevel) return;
  iNewFill -= iFillLevel; iFillLevel += iNewFill;
  UpdatePicture(); // Zahl aktualisieren
  // Tats�chliche �nderung des F�llstandes zur�ckgeben
  return iNewFill;
}

/*protected func Activate(pClonk) {
	return PlayerMessage(GetOwner(pClonk), "$TxtRestPression$", this, GetAmount());
}*/

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

/* Einen harten Aufschlag �berlebt die Flasche nicht! So; Be careful! */
protected func Hit2() {
  // in Bestandteile zerlegen
  CastObjects(LHSP, 4, 20);
  // mit kleiner Explosion von der Welt scheiden
  if (!GetAmount() <= 40)
   Schedule("Explode(GetAmount() / 40 + 20)", 108, 1);
  else 
   Schedule("Explode((GetAmount() ** 2 / (40 - GetAmount())) + 15)", 108, 1);
 return 1;
}
