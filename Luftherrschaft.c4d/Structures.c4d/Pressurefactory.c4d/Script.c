/*-- Druckluftfabrik --*/

#strict

#include WRKS
#include L120
#include DOOR
#include SAVS

//static const LSMN_MaxFill = 2000;
//static const LSMN_MaxFill1 = 600;

private func MaxFill_Pressure() { return 2000; }
private func MaxFill_Water() { return 600; }

protected func Initialize(){
	SetEntrance(1); // aktiver Eingang
	_inherited();
	return 1;
}

protected func Collection2(object pObject) {
	if(!pObject -> ~IsCompressedAirBottle()) // haben wir eine Druckluftflasche?
		return; // keine Druckluftflasche, fertig.
	return FillBottle(pObject); // Flasche füllen!
}

protected func RejectCollect(id idObj, object pObj) {
	//Log("Aufgerufen, %i", idObj);
	if(idObj == LCAB) {
		FillBottle(pObj);
		return 1;
	}
	if(idObj == WBRL) {
		DoFill(Abs(pObj -> BarrelDoFill(-pObj -> BarrelMaxFill())), 1);
		Sound("Splash1");
		return 1;
	}
	return;
}

private func FillBottle(object pBottle) {
	// Check: muss eine Druckluftflasche sein und sich in der Fabrik, die genügend Energie braucht, befinden
	if(!EnergyCheck(50) || !pBottle -> ~IsCompressedAirBottle())
		return;
	DoFill(- pBottle -> DoFill(GetAmount()));
	return 1;
}

private func FindBottle() {
	return FindObject2(Find_Container(this), Find_ID(LCAB), Find_Not(Find_Func("IsFull")), Find_OCF(OCF_Fullcon));
}

/* Steuerung */

protected func ContainedDig() {
	[$TxtFillBottle$|Image=LCAB]
	var pFlasche = FindBottle();
	if(pFlasche)
		return FillBottle(pFlasche);
	Sound("Error");
	return;
}

protected func ContextFillBottle() {
	[$TxtFillBottle$|Image=LCAB|Condition=FindBottle]
	var pFlasche = FindBottle();
	if(pFlasche)
		return FillBottle(pFlasche);
	Sound("Error");
	return;
}

/* Produkteigenschaften (überladbar) */
public func ProductType() { return C4D_Object; }
public func ProductCondition() { return "IsCompressedAirBottle"; }
