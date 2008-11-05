/* Attach - Effekt */

#strict 2
// AddEffect("Attach", [Objekt das attached werden soll], 1, 1, 0, 0, [Objekt an das attached werden soll]);
global func FxAttachStart(object pTarget, int iEffectNumber, int iTemp, object pObj) {
	if(!iTemp) {
		if(!pObj || !pTarget)
			return -1;
		EffectVar(0, pTarget, iEffectNumber) = pObj;
	}
}

global func FxAttachTimer(object pTarget, int iEffectNumber) {
	var pObj = EffectVar(0, pTarget, iEffectNumber);
	if(!pObj)
		return -1;
	pTarget -> SetPosition(GetX(pObj), GetY(pObj));
	pTarget -> SetXDir(GetXDir(pObj));
	pTarget -> SetYDir(GetYDir(pObj));
}

global func Attach2(object pObj, object pTarget) {
	// Kein Objekt vorhanden oder schon irgendwo attached?
	if (!pObj || !pTarget && !(pTarget = this) || GetEffect("Attach", pTarget))
		return;
	
	return AddEffect("Attach", pTarget, 1, 1, 0, 0, pObj);
}
global func StopAttaching(object pObj) {
	// Kein Objekt vorhanden?
	if (!pObj && !(pObj = this))
		return;
	
	return RemoveEffect("Attach", pObj);
}
