/*-- Gildet --*/

#strict 2
#include L103
#include DOOR

local hComponents, hContainer;

protected func Initialize() {
	hComponents = CreateHash();
	hContainer = CreateHash();
	SetEntrance();
}

private func OnlyMatSysComponents(object pObj, array &aIDs) {
	var fFlag, idComponent, ID;
	for(var i = 0; idComponent = GetComponent(0, i, pObj); i++) {
		for(ID in GetMatSysIDs())
			if(ID == idComponent) {
				fFlag = true;
				break;
			}
		if(!fFlag)
			return;
		if(GetType(aIDs))
			aIDs[i] = idComponent;
	}
	return 1;
}

private func ContainedMatSysComponents(object pContainer, array &hash, array &aContainer) {
	var fFlag;
	for(var pObj in FindObjects(Find_Container(pContainer))) {
		HashPut(hash, pObj, []);
		if(OnlyMatSysComponents(pObj, HashGet(hash, pObj))) {
			fFlag = true;
			PushBack(pObj, aContainer);
			continue; // alle Components müssen markiert werden
		}
		HashErase(hash, pObj);
	}
	return fFlag;
}

private func OpenDoor(object pObj, bool fExit) {
	SetEntrance(1);
	if(fExit)
		ExitCheck(pObj);
	return 1;
}

protected func ExitCheck(object pObj) {
	if(!Contained(pObj))
		return SetEntrance();
	ScheduleCall(this, "ExitCheck", 5);
}

protected func ActivateEntrance(object pObj) {
	if(GetCommand(pObj) == "Exit")
		return OpenDoor(pObj, true);

	if(GetOCF(pObj) & OCF_CrewMember) {
		var aDummy = CreateArray();
		ContainedMatSysComponents(pObj, hComponents, aDummy);
		return OpenDoor();
	}

	HashPut(hContainer, pObj, []);
	if(ContainedMatSysComponents(pObj, hComponents, HashGet(hContainer, pObj)))
		return OpenDoor();
	HashErase(hContainer, pObj);

	HashPut(hComponents, pObj, []);
	if(OnlyMatSysComponents(pObj, HashGet(hComponents, pObj))) {
		// Hier evtl. Größenüberprüfung (und Anpassung)
		return OpenDoor();
	}
	HashErase(hComponents, pObj);

	SetCommand(pObj, "None");
	return;
}

protected func Collection2(object pObj) {
	SetEntrance();
	if(GetOCF(pObj) & OCF_CrewMember) {
		if(GetOwner() == NO_OWNER)
			SetOwner(GetOwner(pObj));
		/*for(var pObject in FindObjects(Find_Container(pObj)))
			if(HashGet(hComponents, pObject))
				Collection2(pObject);*/
		return;
	}
	var aContents = HashGet(hContainer, pObj);
	if(aContents)
		for(var pObject in aContents)
			Collection2(pObject);
	var aComponents = HashGet(hComponents, pObj);
	if(!aComponents && GetRealContainer(pObj) == this)
		return SetCommand(pObj, "Exit");
	if(!aComponents && GetRealContainer(pObj) != this)
		return;
	for(idComponent in aComponents)
		MatSysDoFill(GetComponent(idComponent, 0, pObj), GetOwner(), idComponent);
	HashErase(hComponents, pObj);
	RemoveObject(pObj);
	return 1;
}
