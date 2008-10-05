/*-- Gildet --*/

#strict 2
#include L103
#include DOOR

protected func Initialize() {
	hOldContents = CreateHash();
}
protected func ContainedUp(object pClonk) {
	CreateMenu(GetID(), pClonk, this, C4MN_Extra_Components, "Ins Materialsystem transferieren...");
	var bFlag;
	for(var ID in GetMatSysIDs())
		if(TransferObject(ID, 0, 0, 1)) {
			AddMenuItem(GetName(0, ID), "TransferObject", ID, pClonk);
			bFlag = 1;
		}
	if(!bFlag) {
		CloseMenu(pClonk);
		Sound("Error");
	}
	else
		CheckContentsChange(pClonk);
}

local hOldContents;
protected func CheckContentsChange(pClonk) {
	if(!GetMenu(pClonk)) {
		HashErase(hOldContents, pClonk);
		return;
	}
	var aContents = FindObjects(Find_Container(this), Find_Not(Find_OCF(OCF_CrewMember)));
	var aUsed = CreateArray(), pObj, iPos, iSelection;
	if(HashGet(hOldContents, pClonk) && aContents != HashGet(hOldContents, pClonk)) {
		for(pObj in aContents) {
			iPos = GetOtherArrayItemPosition(pObj, HashGet(hOldContents, pClonk), aUsed);
			if(iPos == -1) {
				HashErase(hOldContents, pClonk);
				iSelection = GetMenuSelection(pClonk);
				CloseMenu(pClonk);
				ContainedUp(pClonk);
				SelectMenuItem(iSelection, pClonk);
				return 1;
			}
			PushBack(iPos, aUsed);
		}
	}
	HashPut(hOldContents, pClonk, aContents);
	ScheduleCall(this, "CheckContentsChange", 50, 0, pClonk);
}

// Sucht ein Item im Array, dessen Position nicht in aPos vorkommt
private func GetOtherArrayItemPosition(value, array aArray, array aPos) {
	var iNumber = -1;
	for(var cnt = 0; cnt < GetLength(aArray); cnt++) {
		if(aArray[cnt] == value && GetArrayItemPosition(cnt, aPos) == -1) {
			iNumber = cnt;
			break;
		}
		else 
			continue;
	}
	return iNumber;
}

protected func TransferObject(id ID, int iParameter, bool bRight, bool bTry) {
	var bFlag;
	// 1. direkt als ID vorhanden?
	for(var pObj in FindObjects(Find_ID(ID), Find_Container(this))) {
		//Log("1");
		if(bTry)
			return 1;
		RemoveObject(pObj);
		MatSysDoFill(1, GetOwner(), ID);
		if(!bRight)
			return 1;
		bFlag = 1;
	}
	
	// 2. ID als Component vorhanden?
	var iComponent;
	for(pObj in FindObjects(Find_Container(this))) {
		if(iComponent = GetComponent(ID, 0, pObj)) {
			//Log("2");
			if(bTry)
				return 1;
			RemoveObject(pObj);
			MatSysDoFill(iComponent, GetOwner(), ID);
			if(!bRight)
				return 1;
			bFlag = 1;
		}
	}
	
	// 3. Components als IDs / Components / im MatSys vorhanden?
	var idComponent, iCount, iMatSysCount, iDiff, hUsedObjects, aUsedComponents, hUsedMatSys, iter, node;
	while(1) { // hier fehlt Einrückung
	hUsedObjects = CreateHash();
	aUsedComponents = CreateArray();
	hUsedMatSys = CreateHash();
	for(var i = 0; idComponent = GetComponent(0, i, 0, ID); i++) { // Components der MatSysID durchgehen
		// nach der Components - ID im Inhalt suchen
		iCount = ObjectCount2(Find_Container(this), Find_ID(idComponent));
		iComponent = GetComponent(idComponent, 0, 0, ID);
		if(iCount > iComponent) {
			HashPut(hUsedObjects, idComponent, iComponent);
			continue;
		}
		else {
			if(iCount)
				HashPut(hUsedObjects, idComponent, iCount);
			// Leider nicht genug - nach Alternativen suchen!
			iDiff = iComponent - iCount;
			// in den Components der Inhaltsobjekte
			for(pObj in FindObjects(Find_Container(this))) {
				if(iComponent = GetComponent(idComponent, 0, pObj)) {
					iDiff--;
					PushBack(pObj, aUsedComponents);
					if(iDiff <= 0)
						continue;
				}
			}
			// im MatSys suchen
			if(idComponent != ID && (iMatSysCount = MatSysGetAmount(GetOwner(), idComponent))) {
				iDiff -= iMatSysCount;
				HashPut(hUsedMatSys, idComponent, iMatSysCount + iDiff);
			}
			if(iDiff <= 0)
				continue;
			if(!bFlag && !bTry)
				Sound("Error");
			return;
		}
	}
	if(bTry)
		return 1;
	// Entfernen!
	/*for(var i = 0; idComponent = GetComponent(0, i, 0, ID); i++)
		for(var i2 = GetComponent(idComponent, 0, 0, ID); i2; i2--)
			RemoveObject(FindObject2(Find_Container(this), Find_ID(idComponent)));*/
	iter = HashIter(hUsedObjects);
	while(node = HashIterNext(iter))
		for(0; node[1]; node[1]--)
			RemoveObject(FindObject2(Find_Container(this), Find_ID(node[0])));
	for(pObj in aUsedComponents)
		RemoveObject(pObj);
	iter = HashIter(hUsedMatSys);
	while(node = HashIterNext(iter))
		MatSysDoFill(-node[1], GetOwner(), node[0]);
	//Log("3");
	//Log("%v | %v | %v", hUsedObjects, aUsedComponents, hUsedMatSys);
	MatSysDoFill(1, GetOwner(), ID);
	if(!bRight)
		return 1;
	bFlag = 1;
	}
}
