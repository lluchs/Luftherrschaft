/*-- Sägewerk --*/

#strict
#include L035

// Anzahl Materialpixel, die in einen Fänger passen
static const LRCR_MaxFill = 1000;

local iFillLevel;

protected func Initialize() {
	iFillLevel = 0;
}
// SolidMask=0,10,35,2,0,0
protected func LiquidCheck() {
	if(iFillLevel < MaxFill())
		iFillLevel += 5;
	/*var iX = 0, iY = -27;
	CreateParticle("PSpark", iX, iY, 0, 0, 30, RGB(255, 255, 0));
	if(GetMaterial(iX, iY) == Material("Water")) {
		ExtractLiquid(iX, iY);
		iWasser += 1;
	}
	/*var i = 1, iX2;
	while(1) {
		iX2 = iX + i;
		if(GBackSolid(iX2, iY))
			break;
		if(GetMaterial(iX2, iY) == Material("Water")) {
			ExtractLiquid(iX, iY);
			iWasser += 1;
		}
	}
	while(1) {
		iX2 = iX - i;
		if(GBackSolid(iX2, iY))
			break;
		if(GetMaterial(iX2, iY) == Material("Water")) {
			ExtractLiquid(iX, iY);
			iWasser += 1;
		}
	}*/
	return 1;
}

// Aktuelle Füllung
public func GetAmount() { return(iFillLevel); }

// Maximale Füllung
public func MaxFill() { return(LRCR_MaxFill); }

// Füllung erhöhen/verringern
public func DoFill(int iChange)
{
  var iNewFill = BoundBy(iFillLevel + iChange, 0, MaxFill());
  if (iNewFill == iFillLevel) return;
  iNewFill -= iFillLevel; iFillLevel += iNewFill;
  // Tatsächliche Änderung des Füllstandes zurückgeben
  return iNewFill;
}

private func FindEmptyBarrel() {
	return FindObject2(Find_Container(this), Find_ID(EmptyBarrelID()), Find_Not(Find_Func("BarrelIsFull")));
}

private func FillBarrel(object pFass) {
	var iChangeLevel = pFass -> BarrelDoFill(pFass -> BarrelMaxFill(), Material("Water"));
	var iChange = DoFill(-iChangeLevel);
	if(iChange < iChangeLevel)
		pFass -> BarrelDoFill(iChangeLevel - iChange, Material("Water"));
}

/* Steuerung */

protected func ControlDig() {
	[$TxtFillBarrel$, Image=WBRL]
	var pFass = FindEmptyBarrel();
	if(pFass)
		return FillBarrel(pFass);
	Sound("Error");
	return;
}

protected func ContextFillBarrel() {
	[$TxtFillBarrel$, Image=WBRL]
	var pFass = FindEmptyBarrel();
	if(pFass)
		return FillBarrel(pFass);
	Sound("Error");
	return;
}
